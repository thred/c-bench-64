/// @file
/// Functions for performing input and output.
  // Commodore 64 PRG executable file
.file [name="crc8-kick.prg", type="prg", segments="Program"]
.segmentdef Program [segments="Basic, Code, Data"]
.segmentdef Basic [start=$0801]
.segmentdef Code [start=$80d]
.segmentdef Data [startAfter="Code"]
.segment Basic
:BasicUpstart(__start)
  .const BINARY = 2
  .const OCTAL = 8
  .const DECIMAL = $a
  .const HEXADECIMAL = $10
  .const LIGHT_BLUE = $e
  .const OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS = 1
  .const OFFSET_STRUCT___6526_TOD_HOUR = $b
  .const OFFSET_STRUCT___6526_TOD_MIN = $a
  .const OFFSET_STRUCT___6526_TOD_SEC = 9
  .const OFFSET_STRUCT___6526_TOD_10 = 8
  .const OFFSET_STRUCT___6526_CRA = $e
  .const OFFSET_STRUCT___6526_CRB = $f
  .const OFFSET_STRUCT___6526_TA_LO = 4
  .const OFFSET_STRUCT___6526_TA_HI = 5
  .const OFFSET_STRUCT___6526_TB_LO = 6
  .const OFFSET_STRUCT___6526_TB_HI = 7
  .const STACK_BASE = $103
  .const SIZEOF_STRUCT_PRINTF_BUFFER_NUMBER = $c
  /// Color Ram
  .label COLORRAM = $d800
  /// Default address of screen character matrix
  .label DEFAULT_SCREEN = $400
  .label brk = $c000
  .label func = $c000
  // The number of bytes on the screen
  // The current cursor x-position
  .label conio_cursor_x = $20
  // The current cursor y-position
  .label conio_cursor_y = $22
  // The current text cursor line start
  .label conio_line_text = $23
  // The current color cursor line start
  .label conio_line_color = $25
  .label crc = $21
.segment Code
__start: {
    lda #0
    sta.z conio_cursor_x
    sta.z conio_cursor_y
    lda #<DEFAULT_SCREEN
    sta.z conio_line_text
    lda #>DEFAULT_SCREEN
    sta.z conio_line_text+1
    lda #<COLORRAM
    sta.z conio_line_color
    lda #>COLORRAM
    sta.z conio_line_color+1
    jsr conio_c64_init
    jsr main
    rts
}
// Set initial cursor position
conio_c64_init: {
    // Position cursor at current line
    .label BASIC_CURSOR_LINE = $d6
    ldx.z BASIC_CURSOR_LINE
    cpx #$19
    bcc __b1
    ldx #$19-1
  __b1:
    jsr gotoxy
    rts
}
// Output one character at the current cursor position
// Moves the cursor forward. Scrolls the entire screen if needed
// void cputc(__register(A) char c)
cputc: {
    .const OFFSET_STACK_C = 0
    tsx
    lda STACK_BASE+OFFSET_STACK_C,x
    cmp #'\n'
    beq __b1
    ldy.z conio_cursor_x
    sta (conio_line_text),y
    lda #LIGHT_BLUE
    sta (conio_line_color),y
    inc.z conio_cursor_x
    lda #$28
    cmp.z conio_cursor_x
    bne __breturn
    jsr cputln
  __breturn:
    rts
  __b1:
    jsr cputln
    rts
}
main: {
    .label f = 2
    .label t = $1e
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s
    sta.z printf_str.s
    lda #>s
    sta.z printf_str.s+1
    jsr printf_str
    jsr tod_init
    jsr tod_freq
    sta.z f
    lda #0
    sta.z f+1
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s1
    sta.z printf_str.s
    lda #>s1
    sta.z printf_str.s+1
    jsr printf_str
    jsr printf_sint
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s2
    sta.z printf_str.s
    lda #>s2
    sta.z printf_str.s+1
    jsr printf_str
    jsr benchmark_name
    jsr tod_reset
    jsr benchmark
    jsr tod_get10
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<@s4
    sta.z printf_str.s
    lda #>@s4
    sta.z printf_str.s+1
    jsr printf_str
    jsr benchmark_check
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s4
    sta.z printf_str.s
    lda #>s4
    sta.z printf_str.s+1
    jsr printf_str
    lda.z t
    sta.z printf_sint.value
    lda.z t+1
    sta.z printf_sint.value+1
    jsr printf_sint
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s5
    sta.z printf_str.s
    lda #>s5
    sta.z printf_str.s+1
    jsr printf_str
    // allow the program to be stopped at a breakpoint to caputre the output
    lda #$60
    sta brk
    // Write RTS opcode (0x60) at $c000 for return
    jsr func
    rts
  .segment Data
    s: .text @"Calibrating TOD frequency\n"
    .byte 0
    s1: .text "Detected "
    .byte 0
    s2: .text @" Hz mains frequency\n\n"
    .byte 0
    s4: .text @"\nTotal time: "
    .byte 0
    s5: .text @" s\n"
    .byte 0
}
.segment Code
// Set the cursor to the specified position
// void gotoxy(char x, __register(X) char y)
gotoxy: {
    .const x = 0
    .label __5 = $1c
    .label __6 = $18
    .label __7 = $18
    .label line_offset = $18
    .label __8 = $1a
    .label __9 = $18
    cpx #$19+1
    bcc __b2
    ldx #0
  __b2:
    lda #x
    sta.z conio_cursor_x
    stx.z conio_cursor_y
    txa
    sta.z __7
    lda #0
    sta.z __7+1
    lda.z __7
    asl
    sta.z __8
    lda.z __7+1
    rol
    sta.z __8+1
    asl.z __8
    rol.z __8+1
    clc
    lda.z __9
    adc.z __8
    sta.z __9
    lda.z __9+1
    adc.z __8+1
    sta.z __9+1
    asl.z line_offset
    rol.z line_offset+1
    asl.z line_offset
    rol.z line_offset+1
    asl.z line_offset
    rol.z line_offset+1
    lda.z line_offset
    clc
    adc #<DEFAULT_SCREEN
    sta.z __5
    lda.z line_offset+1
    adc #>DEFAULT_SCREEN
    sta.z __5+1
    lda.z __5
    sta.z conio_line_text
    lda.z __5+1
    sta.z conio_line_text+1
    lda.z __6
    clc
    adc #<COLORRAM
    sta.z __6
    lda.z __6+1
    adc #>COLORRAM
    sta.z __6+1
    lda.z __6
    sta.z conio_line_color
    lda.z __6+1
    sta.z conio_line_color+1
    rts
}
// Print a newline
cputln: {
    lda #$28
    clc
    adc.z conio_line_text
    sta.z conio_line_text
    bcc !+
    inc.z conio_line_text+1
  !:
    lda #$28
    clc
    adc.z conio_line_color
    sta.z conio_line_color
    bcc !+
    inc.z conio_line_color+1
  !:
    lda #0
    sta.z conio_cursor_x
    inc.z conio_cursor_y
    jsr cscroll
    rts
}
/// Print a NUL-terminated string
// void printf_str(__zp($a) void (*putc)(char), __zp(7) const char *s)
printf_str: {
    .label s = 7
    .label putc = $a
  __b1:
    ldy #0
    lda (s),y
    inc.z s
    bne !+
    inc.z s+1
  !:
    cmp #0
    bne __b2
    rts
  __b2:
    pha
    jsr icall2
    pla
    jmp __b1
  icall2:
    jmp (putc)
}
// void tod_init(char freq)
tod_init: {
    jsr tod_detect_freq
    tax
    lda #$7f
    and $dd00+OFFSET_STRUCT___6526_CRA
    sta $dd00+OFFSET_STRUCT___6526_CRA
    rts
}
tod_freq: {
    txa
    rts
}
// Print a signed integer using a specific format
// void printf_sint(void (*putc)(char), __zp(2) int value, char format_min_length, char format_justify_left, char format_sign_always, char format_zero_padding, char format_upper_case, char format_radix)
printf_sint: {
    .label value = 2
    // Handle any sign
    lda #0
    sta printf_buffer
    lda.z value+1
    bmi __b1
    jmp __b2
  __b1:
    lda #0
    sec
    sbc.z value
    sta.z value
    lda #0
    sbc.z value+1
    sta.z value+1
    lda #'-'
    sta printf_buffer
  __b2:
    ldx #DECIMAL
    jsr utoa
    lda printf_buffer
    sta.z printf_number_buffer.buffer_sign
  // Print using format
    lda #0
    sta.z printf_number_buffer.format_upper_case
    lda #<cputc
    sta.z printf_number_buffer.putc
    lda #>cputc
    sta.z printf_number_buffer.putc+1
    lda #0
    sta.z printf_number_buffer.format_zero_padding
    sta.z printf_number_buffer.format_justify_left
    tax
    jsr printf_number_buffer
    rts
}
benchmark_name: {
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s
    sta.z printf_str.s
    lda #>s
    sta.z printf_str.s+1
    jsr printf_str
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s1
    sta.z printf_str.s
    lda #>s1
    sta.z printf_str.s+1
    jsr printf_str
    rts
  .segment Data
    s: .text @"crc8.c\n"
    .byte 0
    s1: .text @"Calculates the CRC8 of the C64 Kernal\n"
    .byte 0
}
.segment Code
tod_reset: {
    lda #0
    sta $dd00+OFFSET_STRUCT___6526_TOD_HOUR
    sta $dd00+OFFSET_STRUCT___6526_TOD_MIN
    sta $dd00+OFFSET_STRUCT___6526_TOD_SEC
    sta $dd00+OFFSET_STRUCT___6526_TOD_10
    rts
}
benchmark: {
    jsr CRC8
    stx.z crc
    rts
}
tod_get10: {
    .label __0 = $15
    .label __2 = $c
    .label __4 = $f
    .label __5 = $a
    .label __6 = 9
    .label __7 = 4
    .label time = $1e
    .label return = $1e
    ldy $dd00+OFFSET_STRUCT___6526_TOD_MIN
    ldx $dd00+OFFSET_STRUCT___6526_TOD_SEC
    lda $dd00+OFFSET_STRUCT___6526_TOD_10
    sta.z time
    lda #0
    sta.z time+1
    lda #$f
    sax.z __0
    lda.z __0
    asl
    asl
    clc
    adc.z __0
    asl
    clc
    adc.z time
    sta.z time
    bcc !+
    inc.z time+1
  !:
    txa
    lsr
    lsr
    lsr
    lsr
    sta.z __2
    asl
    clc
    adc.z __2
    asl
    asl
    asl
    clc
    adc.z __2
    asl
    asl
    clc
    adc.z time
    sta.z time
    bcc !+
    inc.z time+1
  !:
    tya
    and #$f
    sta.z __4
    asl
    asl
    asl
    clc
    adc.z __4
    asl
    asl
    clc
    adc.z __4
    asl
    clc
    adc.z __4
    sta.z __5
    lda #0
    sta.z __5+1
    asl.z __5
    rol.z __5+1
    asl.z __5
    rol.z __5+1
    asl.z __5
    rol.z __5+1
    clc
    lda.z time
    adc.z __5
    sta.z time
    lda.z time+1
    adc.z __5+1
    sta.z time+1
    tya
    lsr
    lsr
    lsr
    lsr
    sta.z __6
    asl
    asl
    clc
    adc.z __6
    asl
    clc
    adc.z __6
    asl
    clc
    adc.z __6
    asl
    asl
    clc
    adc.z __6
    asl
    clc
    adc.z __6
    asl
    clc
    adc.z __6
    asl
    sta.z __7
    lda #0
    rol
    sta.z __7+1
    asl.z __7
    rol.z __7+1
    asl.z __7
    rol.z __7+1
    asl.z __7
    rol.z __7+1
    clc
    lda.z return
    adc.z __7
    sta.z return
    lda.z return+1
    adc.z __7+1
    sta.z return+1
    rts
}
benchmark_check: {
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s
    sta.z printf_str.s
    lda #>s
    sta.z printf_str.s+1
    jsr printf_str
    ldx.z crc
    jsr printf_uchar
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s1
    sta.z printf_str.s
    lda #>s1
    sta.z printf_str.s+1
    jsr printf_str
    lda #$a2
    cmp.z crc
    beq __b1
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s3
    sta.z printf_str.s
    lda #>s3
    sta.z printf_str.s+1
    jsr printf_str
    jsr printf_uint
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s4
    sta.z printf_str.s
    lda #>s4
    sta.z printf_str.s+1
    jsr printf_str
    rts
  __b1:
    lda #<cputc
    sta.z printf_str.putc
    lda #>cputc
    sta.z printf_str.putc+1
    lda #<s2
    sta.z printf_str.s
    lda #>s2
    sta.z printf_str.s+1
    jsr printf_str
    rts
  .segment Data
    s: .text "CRC8="
    .byte 0
    s1: .text "  "
    .byte 0
    s2: .text @"OK\n"
    .byte 0
    s3: .text "FAIL - expected "
    .byte 0
}
.segment Code
// Scroll the entire screen if the cursor is beyond the last line
cscroll: {
    lda #$19
    cmp.z conio_cursor_y
    bne __breturn
    lda #<DEFAULT_SCREEN
    sta.z memcpy.destination
    lda #>DEFAULT_SCREEN
    sta.z memcpy.destination+1
    lda #<DEFAULT_SCREEN+$28
    sta.z memcpy.source
    lda #>DEFAULT_SCREEN+$28
    sta.z memcpy.source+1
    jsr memcpy
    lda #<COLORRAM
    sta.z memcpy.destination
    lda #>COLORRAM
    sta.z memcpy.destination+1
    lda #<COLORRAM+$28
    sta.z memcpy.source
    lda #>COLORRAM+$28
    sta.z memcpy.source+1
    jsr memcpy
    ldx #' '
    lda #<DEFAULT_SCREEN+$19*$28-$28
    sta.z memset.str
    lda #>DEFAULT_SCREEN+$19*$28-$28
    sta.z memset.str+1
    jsr memset
    ldx #LIGHT_BLUE
    lda #<COLORRAM+$19*$28-$28
    sta.z memset.str
    lda #>COLORRAM+$19*$28-$28
    sta.z memset.str+1
    jsr memset
    sec
    lda.z conio_line_text
    sbc #$28
    sta.z conio_line_text
    lda.z conio_line_text+1
    sbc #0
    sta.z conio_line_text+1
    sec
    lda.z conio_line_color
    sbc #$28
    sta.z conio_line_color
    lda.z conio_line_color+1
    sbc #0
    sta.z conio_line_color+1
    dec.z conio_cursor_y
  __breturn:
    rts
}
tod_detect_freq: {
    .label __1 = $a
    .label cbl = $a
    lda #$40
    sta $dd00+OFFSET_STRUCT___6526_CRB
    lda #$80
    sta $dd00+OFFSET_STRUCT___6526_CRA
    lda #$10
    sta $dd00+OFFSET_STRUCT___6526_TA_LO
    lda #$27
    sta $dd00+OFFSET_STRUCT___6526_TA_HI
    lda #$c8
    sta $dd00+OFFSET_STRUCT___6526_TB_LO
    lda #0
    sta $dd00+OFFSET_STRUCT___6526_TB_HI
    jsr tod_reset
    lda #$41
    sta $dd00+OFFSET_STRUCT___6526_CRB
    lda #$81
    sta $dd00+OFFSET_STRUCT___6526_CRA
  // wait for tod to count 1s
  __b2:
    lda $dd00+OFFSET_STRUCT___6526_TOD_SEC
    beq __b2
    //  cal=CIA2.ta_lo;
    //  cah=CIA2.ta_hi;
    lda $dd00+OFFSET_STRUCT___6526_TB_LO
    sta.z cbl
    lda #0
    sta.z cbl+1
    lda #<$c8
    sec
    sbc.z __1
    sta.z __1
    lda #>$c8
    sbc.z __1+1
    sta.z __1+1
    //  printf("count2 = %d %d %d %d\n",cah, cal, cbh, cbl);
    //  printf("elapsed ~= %d0ms\n",CNT-cbl);
    bne __b1
    lda.z __1
    cmp #$5a+1
    bcs __b1
  !:
    lda #$3c
    rts
  __b1:
    lda #$32
    rts
}
// Converts unsigned number value to a string representing it in RADIX format.
// If the leading digits are zero they are not included in the string.
// - value : The number to be converted to RADIX
// - buffer : receives the string representing the number and zero-termination.
// - radix : The radix to convert the number to (from the enum RADIX)
// void utoa(__zp(2) unsigned int value, __zp(7) char *buffer, __register(X) char radix)
utoa: {
    .label digit_value = 4
    .label buffer = 7
    .label digit = $e
    .label value = 2
    .label max_digits = 6
    .label digit_values = $a
    cpx #DECIMAL
    beq __b2
    cpx #HEXADECIMAL
    beq __b3
    cpx #OCTAL
    beq __b4
    cpx #BINARY
    beq __b5
    // Unknown radix
    lda #'e'
    sta printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    lda #'r'
    sta printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS+1
    sta printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS+2
    lda #0
    sta printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS+3
    rts
  __b2:
    lda #<RADIX_DECIMAL_VALUES
    sta.z digit_values
    lda #>RADIX_DECIMAL_VALUES
    sta.z digit_values+1
    lda #5
    sta.z max_digits
    jmp __b1
  __b3:
    lda #<RADIX_HEXADECIMAL_VALUES
    sta.z digit_values
    lda #>RADIX_HEXADECIMAL_VALUES
    sta.z digit_values+1
    lda #4
    sta.z max_digits
    jmp __b1
  __b4:
    lda #<RADIX_OCTAL_VALUES
    sta.z digit_values
    lda #>RADIX_OCTAL_VALUES
    sta.z digit_values+1
    lda #6
    sta.z max_digits
    jmp __b1
  __b5:
    lda #<RADIX_BINARY_VALUES
    sta.z digit_values
    lda #>RADIX_BINARY_VALUES
    sta.z digit_values+1
    lda #$10
    sta.z max_digits
  __b1:
    lda #<printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z buffer
    lda #>printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z buffer+1
    ldx #0
    txa
    sta.z digit
  __b6:
    lda.z max_digits
    sec
    sbc #1
    cmp.z digit
    beq !+
    bcs __b7
  !:
    ldx.z value
    lda DIGITS,x
    ldy #0
    sta (buffer),y
    inc.z buffer
    bne !+
    inc.z buffer+1
  !:
    lda #0
    tay
    sta (buffer),y
    rts
  __b7:
    lda.z digit
    asl
    tay
    lda (digit_values),y
    sta.z digit_value
    iny
    lda (digit_values),y
    sta.z digit_value+1
    cpx #0
    bne __b10
    cmp.z value+1
    bne !+
    lda.z digit_value
    cmp.z value
    beq __b10
  !:
    bcc __b10
  __b9:
    inc.z digit
    jmp __b6
  __b10:
    jsr utoa_append
    inc.z buffer
    bne !+
    inc.z buffer+1
  !:
    ldx #1
    jmp __b9
}
// Print the contents of the number buffer using a specific format.
// This handles minimum length, zero-filling, and left/right justification from the format
// void printf_number_buffer(__zp($a) void (*putc)(char), __zp($d) char buffer_sign, char *buffer_digits, __register(X) char format_min_length, __zp(6) char format_justify_left, char format_sign_always, __zp($e) char format_zero_padding, __zp($10) char format_upper_case, char format_radix)
printf_number_buffer: {
    .label __19 = 4
    .label buffer_sign = $d
    .label padding = $15
    .label format_zero_padding = $e
    .label putc = $a
    .label format_justify_left = 6
    .label format_upper_case = $10
    cpx #0
    beq __b6
    jsr strlen
    // There is a minimum length - work out the padding
    ldy.z __19
    lda.z buffer_sign
    beq __b13
    iny
  __b13:
    txa
    sty.z $ff
    sec
    sbc.z $ff
    sta.z padding
    cmp #0
    bpl __b1
  __b6:
    lda #0
    sta.z padding
  __b1:
    lda.z format_justify_left
    bne __b2
    lda.z format_zero_padding
    bne __b2
    lda.z padding
    cmp #0
    bne __b8
    jmp __b2
  __b8:
    lda.z padding
    sta.z printf_padding.length
    lda #' '
    sta.z printf_padding.pad
    jsr printf_padding
  __b2:
    lda.z buffer_sign
    beq __b3
    pha
    jsr icall3
    pla
  __b3:
    lda.z format_zero_padding
    beq __b4
    lda.z padding
    cmp #0
    bne __b10
    jmp __b4
  __b10:
    lda.z padding
    sta.z printf_padding.length
    lda #'0'
    sta.z printf_padding.pad
    jsr printf_padding
  __b4:
    lda.z format_upper_case
    beq __b5
    jsr strupr
  __b5:
    lda #<printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z printf_str.s
    lda #>printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z printf_str.s+1
    jsr printf_str
    lda.z format_justify_left
    beq __breturn
    lda.z format_zero_padding
    bne __breturn
    lda.z padding
    cmp #0
    bne __b12
    rts
  __b12:
    lda.z padding
    sta.z printf_padding.length
    lda #' '
    sta.z printf_padding.pad
    jsr printf_padding
  __breturn:
    rts
  icall3:
    jmp (putc)
}
// __register(X) char CRC8(__zp(4) const char *data, unsigned int length)
CRC8: {
    .const length = $2000
    .label data = 4
    .label i = 7
    ldx #0
    lda #<$e000
    sta.z data
    lda #>$e000
    sta.z data+1
    txa
    sta.z i
    sta.z i+1
  __b1:
    lda.z i+1
    cmp #>length
    bcc __b2
    bne !+
    lda.z i
    cmp #<length
    bcc __b2
  !:
    rts
  __b2:
    ldy #0
    lda (data),y
    stx.z $ff
    eor.z $ff
    tax
    ldy #8
  __b3:
    cpy #0
    bne __b4
    inc.z data
    bne !+
    inc.z data+1
  !:
    inc.z i
    bne !+
    inc.z i+1
  !:
    jmp __b1
  __b4:
    txa
    and #$80
    cmp #0
    bne __b6
    txa
    asl
    tax
  __b7:
    dey
    jmp __b3
  __b6:
    txa
    asl
    eor #$1d
    tax
    jmp __b7
}
// Print an unsigned char using a specific format
// void printf_uchar(void (*putc)(char), __register(X) char uvalue, char format_min_length, char format_justify_left, char format_sign_always, char format_zero_padding, char format_upper_case, char format_radix)
printf_uchar: {
    .const format_min_length = 2
    .const format_justify_left = 0
    .const format_zero_padding = 1
    .const format_upper_case = 0
    .label putc = cputc
    // Handle any sign
    lda #0
    sta printf_buffer
  // Format number into buffer
    jsr uctoa
    lda printf_buffer
    sta.z printf_number_buffer.buffer_sign
  // Print using format
    lda #format_upper_case
    sta.z printf_number_buffer.format_upper_case
    lda #<putc
    sta.z printf_number_buffer.putc
    lda #>putc
    sta.z printf_number_buffer.putc+1
    lda #format_zero_padding
    sta.z printf_number_buffer.format_zero_padding
    lda #format_justify_left
    sta.z printf_number_buffer.format_justify_left
    ldx #format_min_length
    jsr printf_number_buffer
    rts
}
// Print an unsigned int using a specific format
// void printf_uint(void (*putc)(char), unsigned int uvalue, char format_min_length, char format_justify_left, char format_sign_always, char format_zero_padding, char format_upper_case, char format_radix)
printf_uint: {
    .const uvalue = $a2
    .const format_min_length = 2
    .const format_justify_left = 0
    .const format_zero_padding = 1
    .const format_upper_case = 0
    .label putc = cputc
    // Handle any sign
    lda #0
    sta printf_buffer
  // Format number into buffer
    lda #<uvalue
    sta.z utoa.value
    lda #>uvalue
    sta.z utoa.value+1
    ldx #HEXADECIMAL
    jsr utoa
    lda printf_buffer
    sta.z printf_number_buffer.buffer_sign
  // Print using format
    lda #format_upper_case
    sta.z printf_number_buffer.format_upper_case
    lda #<putc
    sta.z printf_number_buffer.putc
    lda #>putc
    sta.z printf_number_buffer.putc+1
    lda #format_zero_padding
    sta.z printf_number_buffer.format_zero_padding
    lda #format_justify_left
    sta.z printf_number_buffer.format_justify_left
    ldx #format_min_length
    jsr printf_number_buffer
    rts
}
// Copy block of memory (forwards)
// Copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination.
// void * memcpy(__zp($13) void *destination, __zp($11) void *source, unsigned int num)
memcpy: {
    .label src_end = $16
    .label dst = $13
    .label src = $11
    .label source = $11
    .label destination = $13
    lda.z source
    clc
    adc #<$19*$28-$28
    sta.z src_end
    lda.z source+1
    adc #>$19*$28-$28
    sta.z src_end+1
  __b1:
    lda.z src+1
    cmp.z src_end+1
    bne __b2
    lda.z src
    cmp.z src_end
    bne __b2
    rts
  __b2:
    ldy #0
    lda (src),y
    sta (dst),y
    inc.z dst
    bne !+
    inc.z dst+1
  !:
    inc.z src
    bne !+
    inc.z src+1
  !:
    jmp __b1
}
// Copies the character c (an unsigned char) to the first num characters of the object pointed to by the argument str.
// void * memset(__zp($11) void *str, __register(X) char c, unsigned int num)
memset: {
    .label end = $13
    .label dst = $11
    .label str = $11
    lda #$28
    clc
    adc.z str
    sta.z end
    lda #0
    adc.z str+1
    sta.z end+1
  __b2:
    lda.z dst+1
    cmp.z end+1
    bne __b3
    lda.z dst
    cmp.z end
    bne __b3
    rts
  __b3:
    txa
    ldy #0
    sta (dst),y
    inc.z dst
    bne !+
    inc.z dst+1
  !:
    jmp __b2
}
// Used to convert a single digit of an unsigned number value to a string representation
// Counts a single digit up from '0' as long as the value is larger than sub.
// Each time the digit is increased sub is subtracted from value.
// - buffer : pointer to the char that receives the digit
// - value : The value where the digit will be derived from
// - sub : the value of a '1' in the digit. Subtracted continually while the digit is increased.
//        (For decimal the subs used are 10000, 1000, 100, 10, 1)
// returns : the value reduced by sub * digit so that it is less than sub.
// __zp(2) unsigned int utoa_append(__zp(7) char *buffer, __zp(2) unsigned int value, __zp(4) unsigned int sub)
utoa_append: {
    .label buffer = 7
    .label value = 2
    .label sub = 4
    .label return = 2
    ldx #0
  __b1:
    lda.z sub+1
    cmp.z value+1
    bne !+
    lda.z sub
    cmp.z value
    beq __b2
  !:
    bcc __b2
    lda DIGITS,x
    ldy #0
    sta (buffer),y
    rts
  __b2:
    inx
    lda.z value
    sec
    sbc.z sub
    sta.z value
    lda.z value+1
    sbc.z sub+1
    sta.z value+1
    jmp __b1
}
// Computes the length of the string str up to but not including the terminating null character.
// __zp(4) unsigned int strlen(__zp(7) char *str)
strlen: {
    .label len = 4
    .label str = 7
    .label return = 4
    lda #<0
    sta.z len
    sta.z len+1
    lda #<printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z str
    lda #>printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z str+1
  __b1:
    ldy #0
    lda (str),y
    cmp #0
    bne __b2
    rts
  __b2:
    inc.z len
    bne !+
    inc.z len+1
  !:
    inc.z str
    bne !+
    inc.z str+1
  !:
    jmp __b1
}
// Print a padding char a number of times
// void printf_padding(__zp($a) void (*putc)(char), __zp($f) char pad, __zp($c) char length)
printf_padding: {
    .label i = 9
    .label putc = $a
    .label length = $c
    .label pad = $f
    lda #0
    sta.z i
  __b1:
    lda.z i
    cmp.z length
    bcc __b2
    rts
  __b2:
    lda.z pad
    pha
    jsr icall4
    pla
    inc.z i
    jmp __b1
  icall4:
    jmp (putc)
}
// Converts a string to uppercase.
// char * strupr(char *str)
strupr: {
    .label str = printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    .label src = 7
    lda #<str
    sta.z src
    lda #>str
    sta.z src+1
  __b1:
    ldy #0
    lda (src),y
    cmp #0
    bne __b2
    rts
  __b2:
    ldy #0
    lda (src),y
    jsr toupper
    ldy #0
    sta (src),y
    inc.z src
    bne !+
    inc.z src+1
  !:
    jmp __b1
}
// Converts unsigned number value to a string representing it in RADIX format.
// If the leading digits are zero they are not included in the string.
// - value : The number to be converted to RADIX
// - buffer : receives the string representing the number and zero-termination.
// - radix : The radix to convert the number to (from the enum RADIX)
// void uctoa(__register(X) char value, __zp(7) char *buffer, char radix)
uctoa: {
    .const max_digits = 2
    .label digit_value = 6
    .label buffer = 7
    .label digit = $d
    .label started = $10
    lda #<printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z buffer
    lda #>printf_buffer+OFFSET_STRUCT_PRINTF_BUFFER_NUMBER_DIGITS
    sta.z buffer+1
    lda #0
    sta.z started
    sta.z digit
  __b1:
    lda.z digit
    cmp #max_digits-1
    bcc __b2
    lda DIGITS,x
    ldy #0
    sta (buffer),y
    inc.z buffer
    bne !+
    inc.z buffer+1
  !:
    lda #0
    tay
    sta (buffer),y
    rts
  __b2:
    ldy.z digit
    lda RADIX_HEXADECIMAL_VALUES_CHAR,y
    sta.z digit_value
    lda.z started
    bne __b5
    cpx.z digit_value
    bcs __b5
  __b4:
    inc.z digit
    jmp __b1
  __b5:
    jsr uctoa_append
    inc.z buffer
    bne !+
    inc.z buffer+1
  !:
    lda #1
    sta.z started
    jmp __b4
}
// Convert lowercase alphabet to uppercase
// Returns uppercase equivalent to c, if such value exists, else c remains unchanged
// __register(A) char toupper(__register(A) char ch)
toupper: {
    cmp #'a'
    bcc __breturn
    cmp #'z'
    bcc __b1
    beq __b1
    rts
  __b1:
    clc
    adc #'A'-'a'
  __breturn:
    rts
}
// Used to convert a single digit of an unsigned number value to a string representation
// Counts a single digit up from '0' as long as the value is larger than sub.
// Each time the digit is increased sub is subtracted from value.
// - buffer : pointer to the char that receives the digit
// - value : The value where the digit will be derived from
// - sub : the value of a '1' in the digit. Subtracted continually while the digit is increased.
//        (For decimal the subs used are 10000, 1000, 100, 10, 1)
// returns : the value reduced by sub * digit so that it is less than sub.
// __register(X) char uctoa_append(__zp(7) char *buffer, __register(X) char value, __zp(6) char sub)
uctoa_append: {
    .label buffer = 7
    .label sub = 6
    ldy #0
  __b1:
    cpx.z sub
    bcs __b2
    lda DIGITS,y
    ldy #0
    sta (buffer),y
    rts
  __b2:
    iny
    txa
    sec
    sbc.z sub
    tax
    jmp __b1
}
.segment Data
  // The digits used for numbers
  DIGITS: .text "0123456789abcdef"
  // Values of hexadecimal digits
  RADIX_HEXADECIMAL_VALUES_CHAR: .byte $10
  // Values of binary digits
  RADIX_BINARY_VALUES: .word $8000, $4000, $2000, $1000, $800, $400, $200, $100, $80, $40, $20, $10, 8, 4, 2
  // Values of octal digits
  RADIX_OCTAL_VALUES: .word $8000, $1000, $200, $40, 8
  // Values of decimal digits
  RADIX_DECIMAL_VALUES: .word $2710, $3e8, $64, $a
  // Values of hexadecimal digits
  RADIX_HEXADECIMAL_VALUES: .word $1000, $100, $10
  s4: .text @"\n"
  .byte 0
  // Buffer used for stringified number being printed
  printf_buffer: .fill SIZEOF_STRUCT_PRINTF_BUFFER_NUMBER, 0
