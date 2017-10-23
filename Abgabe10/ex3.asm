loop:
	jmp loop ; will always jump back to label loop

times 510-($-$$) db 0 ; pad the assembly program with zero (db 0) until 510th byte

dw 0xAA55 ; save the magic number to the last two bootsector bytes. again, keep in mind that x86 is Little Endian, so 16 Bit word 0xAA55 will be stored as 0x55 0xAA.
