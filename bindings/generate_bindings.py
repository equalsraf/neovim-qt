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
TYPES = {
        'Integer': 'int64_t',
        'Boolean': 'bool',
        'String': 'QByteArray',
        'Object': 'QVariant',
        'Buffer': 'int64_t',
        'Window': 'int64_t',
        'Tabpage': 'int64_t',
    }
class UnsupportedType(Exception): pass
def qt_typefor(typename):
    """
    Some Neovim data types are non trivial e.g. ArrayOf(String)
    or fixed size arrays as ArrayOf(Integer, 2). For some cases
    a simple typedef is enough (see function.h) but for these cases
    the generator needs to know in advance.
    """
    if typename == 'void':
        return typename
    if typename in TYPES:
        # Basic types
        return TYPES[typename]

    m = ARRAY_OF.match(typename)
    if m:
        return 'QList<%s>' % qt_typefor(m.groups()[0])
    elif typename == 'ArrayOf(Integer, 2)':
        # We don't actually implement fixed size array types
        # We only support this one for positions
        return 'QPoint'

    raise UnsupportedType(typename)

class Function:
    """
    Representation for a Neovim API Function
    """
    def __init__(self, nvim_fun):
        self.valid = False
        self.fun = nvim_fun
        self.argtypes = []
        self.realargtypes = []
        self.argnames = []
        self.name =  self.fun['name']
        self.return_type = self.fun['return_type']
        try:
            self.real_return_type = qt_typefor(self.return_type)
            for typ,name in self.fun['parameters']:
                self.argnames.append(name)
                self.argtypes.append(typ)
                self.realargtypes.append(qt_typefor(typ))
        except UnsupportedType as ex:
            print('Found unsupported type(%s) when adding function %s(), skipping' % (ex,self.name))
            return
        self.argcount = len(self.argtypes)
        self.can_fail = self.fun.get('can_fail', False)

        # Build the argument string - makes it easier for the templates
        self.argstring = ', '.join(['%s %s' % tv for tv in zip(self.realargtypes, self.argnames)])
        self.valid = True

    def real_signature(self):
        params = ''
        for i in range(len(self.argtypes)):
            params += '%s %s' % (qt_typefor(self.argtypes[i]), self.argnames[i])
            if i != len(self.argtypes)-1:
                params += ', '
        notes = ''
        if self.can_fail:
            notes += '!fails'
        return '%s %s(%s) %s' % (qt_typefor(self.fun['return_type']),self.fun['name'],params, notes)
    def signature(self):
        params = ''
        for i in range(len(self.argtypes)):
            params += '%s %s' % (self.argtypes[i], self.argnames[i])
            if i != len(self.argtypes)-1:
                params += ', '
        notes = ''
        if self.can_fail:
            notes += '!fails'
        return '%s %s(%s) %s' % (self.fun['return_type'],self.fun['name'],params, notes)


def print_api(api):

    for key in api.keys():
        if key == 'functions':
            print('Functions')
            for f in api[key]:
                fundef = Function(f)
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
            generate_file(name, outpath, **env)

    else:
        print('Neovim api info:')
        print_api(api)
