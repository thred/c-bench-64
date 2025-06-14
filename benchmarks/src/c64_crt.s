	.module crt0

	;; Ordering of segments for the linker.
        .area _CODE
        .area GSINIT
        .area GSFINAL
	.area CODE
        .area RODATA
        .area XINIT

	.area _DATA
        .area DATA
        .area BSS

        .area ZP      (PAG)
        .area OSEG    (PAG, OVR)


        .area GSINIT
	.dw   0x0801
	.dw   __next_line
        .dw   0x01a4        ; Line number
        .db   0x9E             ; SYS token
;       .db   <(((__sdcc_gs_init_startup / 10000) % 10) + '0)
;//        .db   (((__sdcc_gs_init_startup /  1000) % 10) + '0') 
;//        .db   (((__sdcc_gs_init_startup /   100) % 10) + '0')
;//        .db   (((__sdcc_gs_init_startup /    10) % 10) + '0')
;//        .db   (((__sdcc_gs_init_startup /     1) % 10) + '0')
	.db '2','0','6','1'
        .db   0x00             ; End of BASIC line
__next_line:
	.dw   0               ; BASIC end marker

__sdcc_gs_init_startup:
        ldx     #0xff
        txs
	lda 0x01
	and #0xfe
	sta 0x01	; clear LORAM to get 52K contiguos RAM

;        ldx     #0x01         ; MSB of stack ptr
;        stx     __BASEPTR+1

;; initialize DATA
        lda #<s_XINIT
        sta ___memcpy_PARM_2
        lda #>s_XINIT
        sta ___memcpy_PARM_2+1
        lda #<l_XINIT
        sta ___memcpy_PARM_3
        lda #>l_XINIT
        sta ___memcpy_PARM_3+1
        lda #<s_DATA
        ldx #>s_DATA
        jsr ___memcpy

;; clear BSS
        lda #0x00
        sta _memset_PARM_2
        lda #<l_BSS
        sta _memset_PARM_3
        lda #>l_BSS
        sta _memset_PARM_3+1
        lda #<s_BSS
        ldx #>s_BSS
        jsr _memset

; switch to lowercase
	lda #14
	jsr 0xffd2

        .area GSFINAL
__sdcc_program_startup:
        jsr     _main
        jmp     .
