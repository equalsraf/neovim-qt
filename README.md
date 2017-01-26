
Neovim client library and GUI, in Qt5.

## Requirements

* Qt5
* Neovim

## Build

You can build using CMake and your build system of choice. It should build in any
system where Qt5 and Msgpack can build.

For Unix call

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

for Windows both MSVC and Mingw are supported. Use the following

    $ mkdir build
    $ cd build
    $ cmake ..
    $ cmake --build .

The binaries will be under build/bin/. The GUI binary is called nvim-qt.

## Configuration
Commands for interacting with the GUI (e.g. to change the font) can be found by
running [:help neovim-gui-shim](./src/gui/runtime/doc/neovim_gui_shim.txt).

## Design

The *NeovimConnector* class is used to setup the connection to Neovim. It also
provides you with low level methods for RPC - in general you should be using
the signals/slots in the QObject returned by NeovimConnector::neovimObject()

1. To call a function call the corresponding slot
2. The result of the call is delivered by the corresponding signal,
   by convention these signals are named 'on\_' + slot\_name
3. The Neovim() class is automagically generated from the Neovim
   metadata
5. For Neovim functions that take the **Object** type we use **QVariant**
6. To catch Neovim Notifications connect to the Neovim::neovimNotification
   signal

## To Update the RPC bindings

Part of the code is auto-generated by calling Neovim to get the API metadata,
and generating C++ code. This is done using a python script
(generate\_bindings.py) if you just want to use neovim-qt as is you don't need
to worry about this, I already include the generated code in the repository.

The bindings source templates are stored under the bindings/ folder the 
generated code is in src/auto/.

To generate the bindings yourself you need

- python
- python-msgpack
- jinja2

Just run `make bindings` in Unix or the equivalent build command in Windows.

