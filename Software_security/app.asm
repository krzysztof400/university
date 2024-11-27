.data
msg:        .asciz "Hello World !\n"
            #we use asciz instead of asci z stands for \0
.bss

.text


.data
                .align 2
var_short:      .word 0x0FFF
                .align 4
arr_3_int:      .int 10,20,30
my_string:      .asciz "Hello World !"
                .align 8
var_long_long:  .nice

.ascii "Hello World!\0"
.asciz "Hello World!"

    .data
msg : .asciz "Hello World!\n"
len = . - msg
#. is at the end of string so substracting msg that is at beggining will give us lenght, '.' and 'msg' are adresses but diffrent from pointers

    .text
f1:
    /* code f1*/
main:
    f1