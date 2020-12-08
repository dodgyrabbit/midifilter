# midifilter
`midifilter` is a small command line utility that allows you to filter MIDI messages. This has been used on a Raspberry PI and Ubuntu Linux. Useful if your MIDI device is sending messages that you don't want. For example, in my case, my Piano was sending some unwanted
Note Off messages with velocity=0. By running `midifilter` one can filter out what you don't want and expose a virtual MIDI port 
that other software packages can then listen to instead.

Note that although I haven't tried it, the documentation for RtMidi (the library exposing MIDI functionality) states that its 
Windows version does not support virtual MIDI ports.

## Installation

### RtMidi
RtMidi is a prerequisite. Download it from the [official site](https://www.music.mcgill.ca/~gary/rtmidi/index.html#download).
The site has instructions, but this is what I did. 
* Extract files to a local folder
* `./configure`
* `make` (check that no errors)
* `make install` (puts the lib files and includes in the right folders)
* `sudo ldconfig` this refreshes your library cache and make sure the rtmidi library can be found

### g++ Compiler
On both my Raspberry PI and Ubuntu 2004 systems, this was preinstalled.

### VMPK
It is optional, but the Virtual MIDI Piano Keyboard allows you to experiment with the filtering. I found it useful to open two
instances. You can wire the OUTPUT of one as the INPUT on `midifilter` and the INPUT of the other on the virtual port from 
`midifilter`.
### midifilter
Run `make`. A file name `./midifilter` will be created on succesful build.

## How it works
midifilter opens an existing MIDI input port and reads MIDI messages from it. It also opens a virtual MIDI output port
and writes all the messages, except the ones you don't want.

The program is really simple and is currently hard coded for the input device as well as the filtering logic. However, the program is
extremely simple and can trivially be modified.
