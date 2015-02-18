---
bibliography: references.bib
---

Notes
=====

Existing Languages and Software Systems
---------------------------------------

- [A Diary of Whispered Truths](http://niall-moody.itch.io/a-diary-of-whispered-truths)
- [Acme](http://acme.cat-v.org/)
- Alternate Reality Kit [@ungar:2013]
- Array languages: APL, J, A+, K, Q
- BASIC
- [BECOME A GREAT ARTIST IN JUST 10 SECONDS](http://ludumdare.com/compo/ludum-dare-27/?action=preview&uid=4987)
- BEFLIX [@Knowlton:1964:CTP:1464122.1464130]
- [CEPL](https://github.com/cbaggers/cepl)
- [Cinder](http://libcinder.org/)
- [ChucK](http://chuck.cs.princeton.edu/)
- [CodeChisel3D](http://robert.kra.hn/projects/live-programming-with-three-and-webvr)
- [CLM](https://ccrma.stanford.edu/software/clm/)
- [Common Music](http://commonmusic.sourceforge.net/)
- [Conception](https://github.com/shurcooL/Conception)
- [Context Free](http://www.contextfreeart.org/)
- [Csound](http://www.csounds.com/)
- [Cyril](http://cyrilcode.com/)
- [Design By Numbers](http://dbn.media.mit.edu/) [@maeda:2001]
- [Emacs](http://www.gnu.org/software/emacs/)
- EXPLOR [@Knowlton:1975:MFV:988049.988052]
- [Extempore](http://extempore.moso.com.au/)
- Factor
- [fish shell](http://fishshell.com/)
- [Flocking](http://flockingjs.org/)
- [Flowhub](https://flowhub.io/)
- [Fluxus](http://www.pawfal.org/fluxus/)
- Forth
- [GAmuza](http://gamuza.d3cod3.org/)
- [Gibber](http://gibber.mat.ucsb.edu/)
- [Gnuplot](http://www.gnuplot.info/)
- GRAP
- GRASS
- HyperCard
- [Icosa](http://forum.makega.me/t/icosa-art-creation-toy-wip-updated/1109)
- [Impromptu](http://impromptu.moso.com.au/)
- [ixi lang](http://www.ixi-audio.net/)
- Joy
- LambdaMOO
- [Light Table](http://www.lighttable.com/)
- [LiveCodeLab](http://livecodelab.net/)
- [Lively Kernel](http://lively-kernel.org/)
- Logo [@papert:1971:246]
- [LÖVE](http://love2d.org/)
- [LuaAV](http://lua-av.mat.ucsb.edu/)
- [MagicaVoxel](https://voxel.codeplex.com/)
- [Max](http://cycling74.com/products/max/)
- [Minecraft](https://minecraft.net/)
- Jeff Minter's Lightsynths
    - [Psychedelia](http://minotaurproject.co.uk/psychedelia.php) (1984)
    - [Colourspace](http://minotaurproject.co.uk/colourspace.php) (1985, 1986)
    - [Trip-A-Tron](http://minotaurproject.co.uk/ttron.php) (1987)
    - [VLM](http://minotaurproject.co.uk/vlm.php) (1990-2003)
    - [Neon](http://minotaurproject.co.uk/neon.php) (2004)
- [MoonScript](http://moonscript.org/)
- Oberon
- [OpenCV](http://opencv.org/)
- [OpenFrameworks](http://www.openframeworks.cc/)
- [Overtone](http://overtone.github.io/) [@Aaron:2013:SPO:2505341.2505346]
- [pbrt](http://www.pbrt.org/)
- [Pd](http://puredata.info/)
- PIC [@Bentley:1986:PPL:6424.315691]
- [\@pixelsorter Twitter bot](http://wayspurrchen.com/pixelsorter/)
- [Polycode](http://polycode.org/)
- PostScript
- [Praxis LIVE](http://www.praxislive.org/)
- [Processing](http://processing.org/)
- [PuzzleScript](http://www.puzzlescript.net/)
- [Pygame](http://www.pygame.org/)
- [Scratch](http://scratch.mit.edu/)
- [Scsh](http://scsh.net/) [@shivers:1994]
- Self [@ungar:2013]
- Sketchpad [@Sutherland:1963:SMG:1461551.1461591]
- Smalltalk [@goldberg:1989]
- [Snap!](http://snap.berkeley.edu/)
- [Sonic Pi](http://www.cl.cam.ac.uk/projects/raspberrypi/sonicpi/) [@Aaron:2013:SPO:2505341.2505346]
- [Structure Synth](http://structuresynth.sourceforge.net/)
- [Subtext](http://www.subtext-lang.org/)
- [SuperCollider](http://supercollider.sourceforge.net/)
- [Tidal](http://yaxu.org/tidal/)
- [TouchDesigner](https://www.derivative.ca/)
- [TouchOSC](http://hexler.net/software/touchosc)
- Twine
- Unix [@kernighan:1984]
- [vvvv](http://vvvv.org/)

Some Lisps
----------

- LISP [@mccarthy:1960][@levin:1962]
- Interlisp
- Maclisp
- [Common Lisp](http://www.lispworks.com/documentation/HyperSpec/Front/index.htm)
- Emacs Lisp
- Acornsoft Lisp
- [Scheme](http://schemers.org/) [@r5rs]
    - [s7](https://ccrma.stanford.edu/software/snd/snd/s7.html)
    - [TinyScheme](http://tinyscheme.sourceforge.net/)
- [PicoLisp](http://picolisp.com/)
- [newLISP](http://www.newlisp.org/)
- [Racket](http://racket-lang.org/)
- [Clojure](http://clojure.org/)
- [Arc](http://arclanguage.org/)

Language design for live programming
------------------------------------

### Units of program composition

- function/procedure
- generator
- filter
- object
- component
- inheritance
- mixin
- trait
- module

### Errors

Some sources of errors:

- syntax error
- unexpected shape/structure of object (such as unexpected number of arguments for a function)
- unexpected type of object (such as unexpected type of argument for a function)
- reference to an undefined variable
- divide by zero
- index out of bounds
- error from an external system

Some strategies for handling and preventing errors:

- static analysis
- static type system
- functions that can operate on a variable number of arguments
- missing function arguments are given default values (nil or undefined in the worst case)
- functions that can operate on variable types (such as arithmetic on scalars or arrays)
- implicit type conversion (with or without warning)
- Smalltalk #doesNotUnderstand:
- minimise the number of types
- return a value if possible and a suitable value exists
    - in IEEE 754 floating point arithmetic, 1.0/0.0 => +Infinity
    - in Common Lisp, (car '()) => nil (whereas in Scheme it raises an error)
- error values such as NaN, nil, undefined
- return error codes or error objects (for example see [Go](https://golang.org/))
- exceptions
- a program editor, such as for a visual programming language, that constrains the expressible programs to those that minimise runtime errors; for example static type checking can be provided at the editor stage which blocks the user from constructing programs with type errors

### Programs as data

- declarative programming
- genetic algorithms
- pipes and operations in a dataflow language
- text to be parsed
- Smalltalk image
- abstract syntax trees
- s-expressions
- JSON documents
- bytecode
- assembly language
- machine code

Writing a Lisp interpreter
--------------------------

- "The Art of the Interpreter" by Guy Steele Jr. and Gerald Sussman [@steele:1978]
- "Lisp in Small Pieces" by Christian Queinnec
- "Scheme 9 from Empty Space" by Nils M Holm [@holm:2010]
- "Structure and Interpretation of Computer Programs" by Harold Abelson, Gerald Jay Sussman, and Julie Sussman [@abelson:1996]

Procedural generation and simulation algorithms
-----------------------------------------------

- [Boids](http://www.red3d.com/cwr/boids/)
- Cellular automata
- Fractals
- Genetic algorithms [@todd:1992]
- Lindenmayer systems
- Perlin Noise
- Simplex Noise

Applications of procedural generation
-------------------------------------

- [ANGELINA](http://www.gamesbyangelina.org/)
- Civilization
- [Dwarf Fortress](http://www.bay12games.com/dwarves/)
- [Games of Darren Grey](http://gamesofgrey.com/)
- [libnoise](http://libnoise.sourceforge.net/)
- [libtcod](http://roguecentral.org/doryen/libtcod/)
- [No Man's Sky](http://www.no-mans-sky.com/)
- Rogue
- [Spelunky](http://www.spelunkyworld.com/)
- [Ultima Ratio Regum](http://www.ultimaratioregum.co.uk/game/)

Procedural generation resources
-------------------------------

- "Generative Art" by Matt Pearson
- "Generative Design" by Hartmut Bohnacker, Benedikt Gross, Julia Laub, and Claudius Lazzeroni [@bohnacker:2012]
- "[The Nature of Code](http://natureofcode.com/)" by Daniel Shiffman [@shiffman:2012]
- [Procedural Content Generation in Games](http://pcgbook.com/)
- [Procedural Content Generation Wiki](http://pcg.wikidot.com/)

Circuit bending, data bending, and visualization
------------------------------------------------

- [Kurt Werner's circuit-bendable digital models of instruments](https://ccrma.stanford.edu/~kwerner/)
- [Here's What Happens When You Edit Photos Like Music](http://thecreatorsproject.vice.com/blog/heres-what-happens-when-you-edit-photos-like-music)
- "[Visualizing Persistent Data Structures](http://youtu.be/2XH_q494U3U)" by Dann Toliver at Strange Loop 2014

Data formats
------------

### Image formats

- [ExtraFile](http://extrafile.org/)
- GIF
- JPEG
- Netpbm formats
    - [PBM](http://netpbm.sourceforge.net/doc/pbm.html)
    - [PGM](http://netpbm.sourceforge.net/doc/pgm.html)
    - [PPM](http://netpbm.sourceforge.net/doc/ppm.html)
    - [PAM](http://netpbm.sourceforge.net/doc/pam.html)
- PNG
- TIFF

### Polygon mesh formats

- OBJ
- [PLY](http://paulbourke.net/dataformats/ply/)
- STL

### Voxel formats

- [BINVOX](http://www.cs.princeton.edu/~min/binvox/binvox.html)

Hardware
--------

- [Adafruit NeoPixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/overview)
- [Arduino](http://www.arduino.cc/)
- Atari Video Music (1976)
- [BeagleBoard](http://beagleboard.org/)
- [Digital Warrior](http://digitalwarrior.co/)
- [FIGnition](https://sites.google.com/site/libby8dev/fignition)
- [FRAMED*](https://www.kickstarter.com/projects/1353046055/framed-a-revolutionary-platform-for-digital-art)
- Korg SQ-1 hardware sequencer
- Lemur
- [littleBits](http://littlebits.cc/)
- [Leap Motion](https://www.leapmotion.com)
- [Livid Instruments](http://lividinstruments.com/)
- [MaKey MaKey](http://www.makeymakey.com/)
- [Midi Fighter](http://midifighter.com/)
- [Monome](http://monome.org/)
- [Osmo](https://www.playosmo.com/)
- [Raspberry Pi](http://www.raspberrypi.org/) [@shiftrunstop62]
- [Teensy](https://www.pjrc.com/teensy/)
- [TinkerKit](http://www.tinkerkit.com/)
- Video Synthesizers [@collopy:2014]

Communication Protocols
-----------------------

- [acme(4)](http://man.cat-v.org/plan_9/4/acme) and [acme(1)](http://man.cat-v.org/plan_9/1/acme)
- [Interacting with the Extempore Compiler](http://benswift.me/2012/09/26/interacting-with-the-extempore-compiler/)
- HTTP
- I2C
- Lighting control: DMX and Art-Net
- MIDI
- [Open Sound Control (OSC)](http://opensoundcontrol.org/)
- Serial Peripheral interface (SPI)
- [SuperCollider Server Commands](http://doc.sccode.org/Reference/Server-Command-Reference.html)
- WebSocket

Journals, Conferences, and such
-------------------------------

- [CACHe Archives](http://computer-arts-society.com/static/cas/cache/)
- [Collaboration and learning through live coding (Dagstuhl Seminar 13382)](https://www.dagstuhl.de/en/program/calendar/semhp/?semnr=13382) [@blackwell_et_al:DR:2014:4420]
- [Computer Arts Society](http://computer-arts-society.com)
- [Creative Applications Network](http://www.creativeapplications.net/)
- [ACM SIGPLAN International Workshop on Functional Art, Music, Modelling and Design (FARM)](http://functional-art.org/)
- [HOLO](http://www.creativeapplications.net/holo/)
- [LIVE 2013](https://liveprogramming.github.io/2013/index.html)
- [Ludum Dare](http://www.ludumdare.com/)
- [MIDI Hack](http://www.midihack.com/)
- [NODE](http://node.vvvv.org/)
- [Not only computing--also art](http://computer-arts-society.com/static/cas/cache/Archive/Lansdown/LansdownBCS1974.htm)
- Symposium on Principles of Programming Languages (POPL)
    - [POPL 2013 papers](https://github.com/23Skidoo/popl13-papers-links)
    - [POPL 2014 papers](https://github.com/gasche/popl2014-papers)
    - [POPL 2015 papers](https://github.com/yallop/popl2015-papers)
- [PROCJAM](http://www.procjam.com/)
    - [PROCJAM 2014 talks](http://www.youtube.com/playlist?list=PLxGbBc3OuMgiHnQfqerWtgJRA3fMzasra)
- [ReCode Project](http://recodeproject.com/)
- [Resonate](http://resonate.io/)
- [Roguelike Radio](http://www.roguelikeradio.com/)
- [TOPLAP](http://toplap.org/)

History and Preservation
------------------------

- [The Centre for Computing History](http://www.computinghistory.org.uk/)
- [Computer History Museum](http://www.computerhistory.org/)
- [The Dead Media Project](http://www.deadmedia.org/)
- BBC Domesday Project [@mckie:2002][@brown:2003]
- [Internet Archive](https://archive.org/)
- [The Long Now Foundation](http://longnow.org/)
- [MESS (Multi Emulator Super System)](http://www.mess.org/)
- [The National Museum of Computing](http://www.tnmoc.org/)
- [Old-computers.com](http://www.old-computers.com/)
- [Planet Virtual Boy](http://www.planetvb.com/)

References
----------
