===================================================

                   "Only Words"

        My first Sega Master System effort

                    by Mike G.

                  4th March 2001

===================================================

HISTORY
-------

Version 0.1 - released 04/03/01.


BACKGROUND
----------

   "I entered the tunnel on the 31st of May 1977.
        Don't ask stupid questions.
           I had nothing else to do I was bored."


Last week when idly looking round a second-hand
bookshop, I chanced upon a couple of titles which
I knew, right then, I just had to have.

The first one was "Introduction to Digital
Computer Technology - Second Edition" by Louis
Nashelsky. Published in 1972, it's a superb book
which covers everything - everything - one
would ever want to know about digital electronics
and more besides. I don't think there's a modern
book which compares, and it's fascinating to
read about some of the technologies which were
current in the early 1970s but which have since
fallen by the wayside. Magnetic core memory,
anyone? Oh, and to top it off, the chapters are
numbered in binary, which I thought was a very
cute touch.

OK, so it's not strictly relevant, but it was
such a nice find I thought I'd share it with you
anyway :-)

The second book was the real gem - "An
Introduction to Z80 Machine Code" by Penfold and
Penfold, one of the little Babani paperbacks
which British bookshops are overrun with. I've
been looking for a Z80 book for quite some time,
and this 1984 title was just the impetus I
needed to bite the bullet and start programming
the Sega Master System.

As Babani titles are wont to be, it's a very
basic book. Registers, interrupts, pinouts,
instruction set and opcodes, example programs, 
I/O. That's it. At no point does it attempt to
delve into programming techniques or how to achieve
certain tasks in machine code. But I was able to
use my rudimentary knowledge of 6502 coding on
the BBC Micro and the instruction table given
in the book - not to mention the superb
SMS documentation thoughtfully created by talented
individuals - to put together a simple demo program
to print a message on the screen.


THE DEMO
--------

It's been done before, of course, many times (see
Charles Doty's demo and the SMSC efforts), but I
thought I'd "reinvent the wheel" and go my own
way. Apart from the obligatory di / im 1 which
near all SMS games seem to have as their opening
instructions, I didn't refer to anyone else's code
when writing the demo. What you're seeing is
therefore the effort of a complete novice - no
doubt the veteran Z80 coders among you will
cringe at some of it :-)


A COMPARISON
------------

I couldn't help comparing my experience of writing
this demo with that of writing some simple programs
in 6502 assembly for the BBC Micro many (10+!) years
ago.

The 6502 is a very "simple" processor compared to the
Z80. Accumulator, two 8-bit index registers, 8-bit
stack pointer, memory-mapped I/O. It doesn't have
anything like the Z80's macro-like "block"
instructions so moving data around has to be done
with loops and conditional branches.

The 6502 supports indirect addressing and I was
annoyed that the Z80 doesn't have this, though the
existence of the HL register and two 16-bit index
registers largely makes up for it.

The differences between the BBC and SMS as platforms
are more marked. The BBC has a very nice operating
system which obviates the need to access hardware
directly and do dirty stuff like write a character
set to RAM or do direct video memory writes for
screen output. Thanks to the OS routines, writing a
demo like this for the BBC takes minutes rather
than hours and requires little effort.

Conversely, the amount of effort it takes to do
something as simple as write a few words to the
screen on the Master System is immense, and (to
newcomers) probably discouraging. The SMS has no
operating system so we must do everything
ourselves - writing character data to the tile
area of VRAM, initialising VDP registers, writing
CRAM with palette data, and writing the name table
with tile information.

The official SMS developers were probably given
development libraries by Sega along with the dev
kits, to cover the "bare metal" and leave the
programmers to get on with being creative and
artistic, but as amateurs we don't have that luxury.

On the other hand we do have some very good
emulators for code testing and debugging, which
certainly weren't around in 1985-1995 :-)


CHARACTER SET
-------------

I did get some help from the trusty BBC OS, though,
by lifting the character set from its ROM! See the
"font" source file for more information on this. It's
a very nice font and one which I'll be using a lot
more in the future.

Anyone else is of course welcome to use the font
source file, if you think you'd find it useful. I
don't think Acorn would object, as they no longer
exist :-(


TESTING
-------

Meka's palette viewer, tile viewer and technical
information sections proved invaluable in the early
stages, as I tried to determine that the correct
data was being written. Finally I was able to get
some text on the screen and test it on the real
SMS with the SMSReWritable cartridge. A bit of fine
tuning, source code commenting, and it became release
version 0.1.

The source code comments probably aren't of any
value to newcomers as it's a bit like a partially
sighted man leading the blind, but heck, I had fun
doing it so there you go.


THANKS
------

to Zoop, Eric, Heliophobe, Jason, Charles, Maxim,
Asynchronous, KTy, Consolemu, and others on the S8-Dev
forum who have made the Sega 8-bit scene what it is
today.

to Jon, Richard Talbot-Watkins, Neon Spiral Injector,
Charles MacDonald, Sean Young for writing some superb
documents and providing an insight into what goes on
under the SMS's bonnet. ("Hood", for US readers.)

to Steph, Kerry, and all who have provided me with
light in times of darkness, and vice versa :-)

to Aston Manor Breweries for producing Frosty Jack,
the most potent and will-sapping drink you can
produce with apples and still call it cider. And at
GBP1.99 for two litres, it's still excellent value
for money! See http://www.frostyjack.co.uk.

to me, for showing tenacity for the first time in my
life and actually finishing this damned demo!


THAT'S ALL FOR NOW
------------------

See you later. There are more delights to follow...


===================================================
Mike Gordon
mike@mikeg2.freeserve.co.uk

Home Page - http://www.mikeg2.freeserve.co.uk/

SMS Power! - www.smspower.org

SMSReader Project - www.smspower.org/smsreader/
===================================================
