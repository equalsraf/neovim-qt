#!/usr/bin/env python
"""
C++ source code generator to create bindings from Neovim API functions
using Qt types/signals/slots
"""

import msgpack
import sys, subprocess, os
import re
import jinja2
import datetime

INPUT = 'bindings'

def decutf8(inp):
    """
    Recursively decode bytes as utf8 into unicode
    """
    if isinstance(inp, bytes):
        return inp.decode('utf8')
    elif isinstance(inp, list):
        return [decutf8(x) for x in inp]
    elif isinstance(inp, dict):
        return {decutf8(key):decutf8(val) for key,val in inp.items()}
    else:
        return inp

def get_api_info(nvim):
    """
    Call the neovim binary to get the api info
    """
    args = [nvim, '--api-info']
    info = subprocess.check_output(args)
    return decutf8(msgpack.unpackb(info))

def generate_file(name, outpath, **kw):
    from jinja2 import Environment, FileSystemLoader
    env=Environment(loader=FileSystemLoader('bindings'), trim_blocks=True)
    template = env.get_template(name)
    with open(os.path.join(outpath, name), 'w') as fp:
        fp.write(template.render(kw))

ARRAY_OF = re.compile('ArrayOf\(\s*(\w+)\s*\)')
class UnsupportedType(Exception): pass
class NeovimTypeVal:
    """
    Representation for Neovim Parameter/Return
    """
    # msgpack simple types types
    SIMPLETYPES = {
            'void': 'void',
            'Integer': 'int64_t',
            'Boolean': 'bool',
            'String': 'QByteArray',
            'Object': 'QVariant',
            'Array': 'QVariantList',
        }
    # msgpack extension types
    EXTTYPES = {
            'Window': 'int64_t',
            'Buffer': 'int64_t',
            'Tabpage': 'int64_t',
        }
    PAIRTYPE = 'ArrayOf(Integer, 2)'
    # Unbound Array types
    UNBOUND_ARRAY = re.compile('ArrayOf\(\s*(\w+)\s*\)')

    def __init__(self, typename, name=''):
        self.name = name
        self.neovim_type = typename
        self.ext = False
        self.native_type = NeovimTypeVal.nativeType(typename)

        self.sendmethod = 'send'
        self.decodemethod = 'decodeMsgpack'
        if typename in self.SIMPLETYPES:
            pass
        elif typename in self.EXTTYPES:
            self.ext = True
            # FIXME
            #self.sendmethod = 'send%s' % typename
            #self.decodemethod = 'decodeMsgpackAs%s' % typename
        elif self.UNBOUND_ARRAY.match(typename):
            m = self.UNBOUND_ARRAY.match(typename)
            elemtype = m.groups()[0]
            self.sendmethod = 'sendArrayOf'
        elif typename == self.PAIRTYPE:
            self.native_type = 'QPoint'
        else:
            raise UnsupportedType(typename)

    @classmethod
    def nativeType(cls, typename):
        """Return the native type for this Neovim type."""
        if typename == 'void':
            return typename
        elif typename in cls.SIMPLETYPES:
            return cls.SIMPLETYPES[typename]
        elif typename in cls.EXTTYPES:
            return cls.EXTTYPES[typename]
        elif cls.UNBOUND_ARRAY.match(typename):
            m = cls.UNBOUND_ARRAY.match(typename)
            return 'QList<%s>' % cls.nativeType(m.groups()[0])
        elif typename == cls.PAIRTYPE:
            return 'QPoint'
        raise UnsupportedType(typename)

class Function:
    """
    Representation for a Neovim API Function
    """
    def __init__(self, nvim_fun):
        self.valid = False
        self.fun = nvim_fun
        self.parameters = []
        self.name =  self.fun['name']
        try:
            self.return_type = NeovimTypeVal(self.fun['return_type'])
            for param in self.fun['parameters']:
                self.parameters.append(NeovimTypeVal(*param))
        except UnsupportedType as ex:
            print('Found unsupported type(%s) when adding function %s(), skipping' % (ex,self.name))
            return
        self.argcount = len(self.parameters)
        self.can_fail = self.fun.get('can_fail', False)

        # Build the argument string - makes it easier for the templates
        self.argstring = ', '.join(['%s %s' % (tv.native_type, tv.name) for tv in self.parameters])
        self.valid = True

    def real_signature(self):
        params = ''
        for p in self.parameters:
            params += '%s %s' % (p.native_type, p.name)
            params += ', '
        notes = ''
        if self.can_fail:
            notes += '!fails'
        return '%s %s(%s) %s' % (self.return_type.native_type,self.name,params, notes)
    def signature(self):
        params = ''
        for p in self.parameters:
            params += '%s %s' % (p.neovim_type, p.name)
            params += ', '
        notes = ''
        if self.can_fail:
            notes += '!fails'
        return '%s %s(%s) %s' % (self.return_type.neovim_type,self.name,params, notes)


def print_api(api):

    for key in api.keys():
        if key == 'functions':
            print('Functions')
            for f in api[key]:
                fundef = Function(f)
                if not fundef.valid:
                    continue
                sig = fundef.signature()
                realsig = fundef.real_signature()
                print('\t%s'% sig)
                if sig != realsig:
                    print('\t[aka %s]\n' % realsig)
            print('')
        elif key == 'types':
            print('Data Types')
            for typ in api[key]:
                print('\t%s' % typ)
            print('')
        elif key == 'error_types':
            print('Error Types')
            for err,desc in api[key].items():
                print('\t%s:%d' % (err,desc['id']))
            print('')
        elif key == 'features':
            pass
        else:
            print('Unknown API info attribute: %s', key)

if __name__ == '__main__':

    if len(sys.argv) < 2 or len(sys.argv) > 3 :
        print('Usage:')
        print('\tgenerate_bindings <nvim>')
        print('\tgenerate_bindings <nvim> [path]')
        sys.exit(-1)

    nvim = sys.argv[1]
    outpath = None if len(sys.argv) < 3 else sys.argv[2]
    try:
        api = get_api_info(sys.argv[1])
    except subprocess.CalledProcessError as ex:
        print(ex)
        sys.exit(-1)

    if outpath:
        print('Writing auto generated bindings to %s' % outpath)
        if not os.path.exists(outpath):
            os.makedirs(outpath)
        for name in os.listdir(INPUT):
            if name.startswith('.'):
                continue
            if not name.endswith('.h') and not name.endswith('.cpp'):
                continue
            env = {}
            env['date'] = datetime.datetime.now()
            functions = [Function(f) for f in api['functions'] if f['name'] != 'vim_get_api_info']
            env['functions'] = [f for f in functions if f.valid]
            exttypes = { typename:info['id'] for typename,info in api['types'].items()}
            env['exttypes'] = exttypes
            generate_file(name, outpath, **env)

    else:
        print('Neovim api info:')
        print_api(api)
