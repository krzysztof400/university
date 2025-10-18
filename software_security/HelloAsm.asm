.data
                .align 2
var_short:      .word 0x0FFF
                .align 4
tab_3_int:      .int 10,20,30
ma_chine:       .asciz "Hello World !"
                .align 8
var_long_long:  .quad 0xff

.bss
    .lcomm buffer, 10

.text
.global main
main:
    movl %esp, %ebp
    leal var_short, %eax

        xorl, %eax, %eax
    ret