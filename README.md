
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
3. The Neovim() class is automagically generated from the Neovim
   metadata
5. For Neovim functions that take the **Object** type we use **QVariant**,
   this seems to be the natural equivalent in Qt

## Requirements

* Qt5
* libmsgpack
* Neovim

## To update the RPC bindings

To regenerate the neovim API bindings run

    make neovim_auto

If your Neovim version is not in your path, pass the path to cmake as

    cmake -DNEOVIM_EXEC=/usr/bin/nvim ..

## TODO

A crapload of them

* Error checking (start with the QIODevice related errros)
* Deep check QVariants before starting to serialized them
* We can be more pedantic about argcount in startRequest
* I currently do not implement synchronous calls, I suppose it
  would be possible to generate more code to do it using
  QEventLoop
* Make it pretty, i.e. hide msgpack internals, PIMPL the
  NeovimConnector/Function class, etc

