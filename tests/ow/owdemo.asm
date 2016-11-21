;-----------------------------------------
;             "Only Words"
;
;       (my first attempt at any
;        kind of SMS programming!)
;
;	for the Sega Master System
;
;                  by
;
;              Mike Gordon
;      mike@mikeg2.freeserve.co.uk
;
;              Version 0.1
;
; Created one lazy Sunday afternoon on
;      the fourth of March, 2001.
;
;-----------------------------------------


;=====================================================
;
; Please disregard my comments as the rantings of a
; clueless novice! Feel at liberty to scoff at my
; hopeless first try at writing Z80 code :-)
;
; NB. Any feedback is welcome. I can take it (sniff).
;
;=====================================================



.org $0000	; means "assemble starting from address $0000 in
		; the object file". (The object file is the output
		; file, i.e. the actual ROM image which the assembler 
		; creates from this source file.)

	
 	di		; disable interrupts
 	im 1		; set interrupt mode 1 (the only mode that's
			; useful on the vanilla SMS)

			; (NB: We don't enable interrupts, 
			;  as we don't need them in this program!)

	ld sp, $DFF0	; set stack pointer to a sensible value, 
			; at the end of user RAM. The stack
			; grows downwards from this address.

 	jp main		; jump to the main part of the program*
        
   
.org $0066


	retn		; the Z80 jumps here in the event of the Pause
			; button being pressed, which generates a non
			; maskable interrupt (NMI). We need to return
			; from this NMI even though we don't do anything
			; with Pause as such.



	; * which starts right here.

main:	; <- this is called a label, it's not part of Z80 ASM but is
	; used as a marker during assembly. It saves having to put
	; jp [address] when we have no way of knowing what [address]
	; will turn out to be :-)


	; This is the VDP register initialisation routine. We write some
	; sensible default values to the VDP registers.

	ld hl, vdp_reg	; points to start of VDP register data. This data
			; is in the vdpdef file, and vdp_reg is a label
			; which points to the start of this data.

 	ld b, vdp_size	; number of bytes to be written. (We need to
			; write a data byte followed by a control byte
			; for each register, so this will be equal to
			; the number of registers to be written multiplied
			; by 2.)

			; For more info on this, see the vpddef file for
			; an explanation of the register values and what
			; they mean. Also read docs by Charles MacDonald,
			; Richard Talbot-Watkins, et al.


	ld c, $BF	; Port $BF is the VDP control port. Writing to this
			; port allows VDP register writes, and also
			; allows the CRAM or VRAM address registers to be set 
			; (again, see the docs).

			; Newcomers should note that the Z80 has a separate
			; control signal, called _IORQ, for I/O reads and
			; writes as opposed to memory access. It means that
			; there is effectively a separate 8-bit address space
			; for hardware devices. This is called port-mapped
			; I/O, and addresses in this space which correspond
			; to I/O devices are known as ports. The Z80 has
			; special instructions, the in and out instructions,
			; for dealing with these.


	otir		; This is one of the repeating instructions for
			; which the Z80 is famous. It basically means
			; "output the byte stored in the memory address
			; pointed to by the HL register pair to the port
			; address stored in the C register, increment the
			; HL register pair and decrement the B register,
			; then repeat until B is equal to zero."

			; Phew! Doing the same thing on the 6502 would
			; require a loop with conditional branch, this
			; shows how compact Z80 code can be in
			; comparison.


  	; Now we need to clear VRAM, which will still have stuff stored
	; in it from the fancy BIOS bumper routine.

	ld a, $00	; We output two bytes to the VDP control port
	out ($BF), a	; to indicate that we want to write to VRAM
	ld a, %01000000	; and to set the VRAM address pointer to
 	out ($BF), a	; $0000.

	ld bc, $4000	; The VRAM is 16 Kilobytes in size, which
			; is equivalent to $4000 bytes in hexadecimal.
			; That's how many bytes we need to write to
			; VRAM in order to clear it, so we set the
			; BC register pair up as a counter for this
			; operation.

			; Yes, another nice feature of the Z80 is
			; that registers (B, C, D, E, H, L) can be
			; manipulated as pairs (BC, DE, HL) to store
			; 16-bit values. 



vram_clear_loop:	; This label is used to indicate the start of
			; a loop.


 	ld a, $00	; Output $00 to the VDP data port ($BE) to store
	out ($BE), a	; $00 (zero) at the current VRAM address.
			; The VRAM address pointer is incremented
			; automatically.

	dec c				; I don't like this at all.
	jr nz, vram_clear_loop		; What we're doing is
	 				; decrementing the B and C
					; registers and jumping
	dec b				; back to the start of the
	jr nz, vram_clear_loop		; loop if either is not
					; zero. This gives us
					; $4000 iterations of the
					; loop.

			; How much nicer it would be to say:
			;
			; 	dec bc
			; 	jr nz, vram_clear_loop
			;
			; but no! Decrementing a register
			; pair has no effect on the status
			; flags so it's of no use in
			; conditional branching. Shame :-(


	; There's probably a far more elegant way of achieving the
	; above, but I'm a novice myself, so haven't progressed
	; to optimising code yet.

	; Now we need to write to CRAM to set the palette.

	ld hl, palette	; load HL with start address of palette
			; data.

	ld b, $20	; Load B with number of bytes to be written
			; to CRAM. There are 32 ($20 hex) palette
			; entries - 16 for the tile palette,
			; 16 for the sprite palette.

	ld c, $BE	; load C with the VDP data port address.
 	
 	ld a, $00	; write two bytes to the VDP control port
	out ($BF), a	; to indicate a CRAM write operation 
	ld a, $C0	; starting from address $00.
	out ($BF), a	; Then, writes to the VDP data port ($BE)
			; will be stored in CRAM and automatically
			; increment the CRAM address pointer.

 	otir		; see above for explanation. Transfers
			; the 32 bytes of palette data to CRAM.


 	; We now need to store the tile data for the character
	; set, starting at VRAM address $0000.

	; See the VDP docs for an explanation of how the
	; tile-mapped graphics system works. Basically the
	; tile data is stored as four bytes per line (32 bytes
	; per tile), with each byte representing a bit of the
	; palette colour used for each of the 8 pixels in a
	; row of the tile.


tile_init:

	ld a, $00	; two writes to the VDP control port
	out ($BF), a	; which indicate we want to write to
 	ld a, $40	; VRAM starting from address $0000.
 	out ($BF), a

	ld hl, char_set		; Load HL with start address of
				; character set data.

 	ld bc, char_set_size	; Load BC with size of character
				; set in bytes (we use it as
				; a counter).

				; This will be equivalent to the
				; number of characters multiplied
				; by 8 (8 bytes per character).

tile_loop:

 	ld a, (HL)	; load accumulator with character data byte
			; (represents one line of the character)

	out ($BE), a	; output this data four times to the VDP
 	out ($BE), a	; data port. This means the characters use
 	out ($BE), a	; colour 15 from the palette - the VDP docs
 	out ($BE), a	; explain how this works.


 	inc hl		; increment HL to point to next data byte

	dec c			; This messy system for decrementing
				; BC with conditional branching
 	jr nz, tile_loop	; once again. Ugh.

 	dec b			; By the way, jr (as opposed to jp)
				; means a *relative* jump. It can
	jr nz, tile_loop	; only be used to jump to addresses
				; within a certain range, but is
				; faster than jp so is ideal for
				; implementing loops.


	; At this point, the character set should be visible in
	; the Tile Viewer if you're using the Meka emulator.

	
	; Our next task is to update the *name table* with the
	; message data. The name table is a 32 by 24 table of 16-bit
	; words in VRAM, which tells the VDP which tile to display
	; in that position on the screen, which palette to use,
	; whether to flip the tile, etc.

	; We will start writing to the name table at $3840 hex, one
	; row from the top of the TV screen.


#define name_table_address $3840	; This define is simply a way
					; of saying to the assembler
					; "Wherever you find
					; name_table_address in the
					; source, substitute $3840
					; in its place."

					; Names are easier to remember
					; than numbers, and it's
					; possible to change an
					; address or value without
					; having to change each instance
					; in the code :-)


	ld bc, name_table_address	; load BC with name table
					; start address in VRAM.

	ld a, c			; Writes the low byte of the VRAM address
	out ($BF), a		; to the VDP control port.

				; Since we loaded the BC pair with a
				; 16-bit value, B will contain the high
				; byte and C the low byte.

 	ld a, b			; We need to XOR the high byte of the
	xor $40			; name table address with $40 to set
				; the "write" bit of the control byte,
				; indicating to the VDP we want to
				; write to VRAM rather than read from it.
				
 	out ($BF), a		; Then we output this byte to the VDP
				; control port.
	

	ld hl, text_data	; Load the HL pair with the start
				; address of the text message data.

	
start_of_loop:

	ld a, (hl)		; Load byte of message into accumulator.

	cp $00			; A zero byte ($00) indicates the end of
	jr z, end_of_loop	; the message, so branch out of the loop
				; if this byte is equal to zero. The cp
				; instruction means "compare with
				; accumulator", and sets the zero flag if
				; a match is found.

	sub $20			; This is code conversion. Tile number x
				; contains the character value for ASCII
				; ($20+x). So we subtract $20 from the
				; byte to get the correct tile number.

	out ($BE), a		; Output tile number to the VDP data
				; register. This is the "even byte" of
				; the name table pair.

	ld a, %00000000		; The "odd byte" of the name table pair
	out ($BE), a		; contains the most significant bit of
				; the tile number and various flags
				; denoting whether to flip a tile and
				; which of the two palettes to use. The
				; VDP docs have more information on
				; this.				

	inc hl			; increment the HL pair to point to the
	jr start_of_loop	; next character of the message and
				; jump back to start of loop.

end_of_loop:

	

	; Finally we have to turn on the VDP to see the result of
	; our hard work, so the message is displayed on the screen.
	; This is done by setting bit 6 of VDP register 1.
	

	ld a, %11000000		; set bits 6 and 7
	out ($BF), a
	ld a, %10000001		; indicates a write to VDP reg. 1
	out ($BF), a


	; You should now see the text on the screen!!!

end_of_demo:

	jp end_of_demo		; This just goes round in an endless
				; loop until you turn the SMS off.



	; Right that's the program out of the way, now for the data.

vdp_reg:

.include "vdpdef"
			; means "insert the contents of file vdpdef
			; at this point". vdpdef contains the default
			; VDP register values.

vdp_reg_end:

 	nop

palette:

.include "palette"
			; palette contains the default palette entries.


char_set:

.include "font"
			; font contains the character set data.

char_set_end:

	nop

	; A couple of defines (see above for what they mean).

	; char_set_size is defined as the value of char_set_end
	; minus the value of char_set. It's the number of bytes
	; in the character set.

#define char_set_size char_set_end-char_set

	; Similarly for vdp_size:

#define vdp_size vdp_reg_end-vdp_reg


text_data:

.include "text"
			; include the ASCII data for the text to
			; be displayed.

sdsc_data:

.db "Only Words", $00
			; SDSC title data, zero terminated.


.org $7FE0

	; The SDSC header data based on proposed spec.

.db "SDSC"
.db $00			; Major program version no.
.db $01			; Minor program version no.
.db $04			; Release day
.db $03			; 	  month
.db $01, $20		;         year

.org $7FEC

.db sdsc_data%256  	; ROM address of title string (low byte)
.db sdsc_data/256	;			      (high byte)	
.db $FF, $FF		; ROM address of release notes ($FFFF = null)


	; Now lastly, the SMS header data.

.org $7FF0

.db "TMR SEGA"

.db $20, $20

.db $AE, $8E		; Checksum data low byte, high byte. (This must
			; be changed if the source is modified.)

.db $20, $20, $00

.db $4C			; Indicates US/European country code (4) and 32
			; kilobyte ROM ($0C).
   
.org $8000

.end	; this indicates the end of the source file to the
	; assembler.
