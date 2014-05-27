
A library to talk with neovim in Qt

## Build

    $ mkdir build
    $ cd build
    $ cmake ..

## Design

The NeovimConnector class is used to setup the connection to Neovim. It also
provides you with some VERY low level methods to call RPC functions - but in general
you should be using the QObject returned by NeovimConnector::neovimObject()

1. To call a function just call the corresponding slot - slots 
   have the same names as the original neovim functions - 
   these are async (return void)
2. To get the result of the call catch the corresponding signals,
   by convention these signals are named 'on\_' + slot\_name
3. The Neovim() class is automagically generated using a lua script
   (based on the dispatch generator from neovim)
5. For Neovim functions that take the **Object** type we use **QVariant**,
   this seems to be the natural equivalent in Qt

## Requirements

* Qt5
* libmsgpack

## To update the RPC bindings

From the project root

    lua scripts/code-gen.lua ../neovim/src/nvim/api/*.h src/auto/

The first path, points to the API headers in the neovim source code.
Afterwards just follow the build instructions above.

## TODO

A crapload of them

* Error checking (start with the QIODevice related errros)
* Function call errors are not being handled ATM
* Deep check QVariants before starting to serialized them
* We can be more pedantic about argcount in startRequest
* I currently do not implement synchronous calls, I suppose it
  would be possible to generate more code to do it using
  QEventLoop
* Start considering testing ...
* Make it pretty, i.e. hide msgpack internals, PIMPL the
  NeovimConnector/Function class, etc
* Add some cmake rules to call the lua generator

