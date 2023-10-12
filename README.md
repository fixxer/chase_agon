# Robot Chase
Robot Chase - a simple game for [Agon Light](https://github.com/breakintoprogram/agon-docs/wiki).

Based on the game code from [acn/vt100-games](https://git.imzadi.de/acn/vt100-games/) and [Agon-MOS-SDCC-Template](https://github.com/nihirash/Agon-MOS-SDCC-Template).

## The game

You are trapped in a maze of high voltage power fences and posts. There are also 
killer robots trying to destroy you.
 
Your only hope is to lure the robots into a fence or each other to destroy them.
         
If things get hopeless, you can do a mega jump, but you could end up anywhere,
including on top of a power fence or a robot!

## Keys

Use the keys to move, even diagonally.
Use ``J`` to jump and ``0`` to pass a round.

    W   E   R   J - Jump
      \ | /     0 - Pass
    S - * - F
      / | \
    X   C   V   Q - Quit

    * - You
    X - High Voltage Line
    + - Killer Robot

## Compiling

You need [SDCC](https://sdcc.sourceforge.net/) installed to compile the sources.

    mkdir out && make clean && make