; Print strings to the screen

[org 0x7c00] 	; tell the assembler to offset all label addresses by 0x7c00. 
				;this is the memory address where the BIOS puts the bootsector

call clearscr

mov ah, 0x0e ; 0e is TTY mode. Interrupt 0x10 needs parameters in Register ah

mov al, 'H' ; TTY mode expects character in al
int 0x10 ; finally call the ISR
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

call newline

mov si, boot_msg ; copy address of boot_msg to si (source index register, used for string and memory array copying)

print_string:
	lodsb ; load byte at current address into al and increment address in si
	call print_char	

	cmp si, boot_msg_end ; while si is not at boot_msg_end
	jne print_string

end:
	jmp $ ; forever jump to current address (do nothing)


boot_msg:
	db 'Bootloader v.0.1: Booting the System...'
boot_msg_end:

; print the character currently set in al
print_char:
	pusha
	mov ah, 0x0e
	int 0x10
	popa
	ret


; move cursor down one line
newline:
	pusha
	mov ah, 0x03 ; get cursor information
	int 0x10
	inc dh ; increment line
	mov dl, 0 ; reset cursor to first column
	mov ah, 0x02 ; update cursor information
	int 0x10
	popa
	ret

; clear the whole screen
clearscr:
	pusha
	;mov al, 0x03
	;mov ah, 0x00
	mov ax, 0x0003 ; al=03, ah=00. video mode 03 = text 80 x 25 16 color. switching video mode clears the screen. In our case this works, because we already are in this mode
	int 0x10
	popa
	ret

; padding the file and adding magic number
times 510-($-$$) db 0 ; db...Declare Byte. $ -> address of the current line, $$ -> beginning of the current section. ($-$$) evaluates how far the program is in the current section
dw 0xAA55 ; dw...Declare Word

