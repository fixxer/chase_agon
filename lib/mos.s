    .area _CODE
    .globl _putchar
    .globl __putchar

    .globl _getchar
    .globl __getchar

    .globl _mos_editline
    .globl __mos_editline

    .ascii "MOS LIBRARY (c) Nihirash"

_putchar:
__putchar:
    ld hl, #2
    add hl, sp
    ld a, (hl)
    rst 0x10
    ret


_getchar:
__getchar:
    xor a
    rst 0x08
    ld l, a
    ret


_mos_editline:
__mos_editline:
    ld iy, #2
    add iy, sp
    ld l, (iy)	; Address of the buffer
    ld h, 1(iy)
    ld c, 2(iy)	; Buffer length
    ld b, 3(iy)
    ld e, 4(iy) ; 0 to not clear buffer, 1 to clear
    ld a, #9
    rst 0x08	; Invoke the line editor
    ret    
