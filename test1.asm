section .data
    format_num db "%d", 10, 0
    format_str db "%s", 10, 0

section .text
global main
extern printf

main:
    push rbp
    mov rbp, rsp
    sub rsp, 64 ; Allocate stack space for variables

