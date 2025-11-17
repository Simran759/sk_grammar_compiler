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

    mov rax, 10
    mov [rbp - 8], rax ; Declare a
section .data
S0 db "hello", 0
section .text
    mov rax, S0
    mov [rbp - 24], rax ; Declare y
section .data
S1 db "helloworld", 0
section .text
    mov rax, S1
    mov [rbp - 32], rax ; Declare c
    mov rax, 4
    push rax
    mov rax, 2
    push rax
    mov rax, 1
    pop rbx
    add rax, rbx
    pop rbx
    add rax, rbx
    mov [rbp - 40], rax ; Declare p
    mov rax, [rbp - 40]
    mov rdi, format_num
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, [rbp - 32]
    mov rdi, format_str
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, 5
    push rax
    mov rax, [rbp - 8]
    pop rbx
    imul rax, rbx
    mov [rbp - 16], rax ; Assign to b
    mov rax, 10
    push rax
    mov rax, [rbp - 8]
    pop rbx
    cmp rax, rbx
    jge L0
    mov rax, [rbp - 8]
    mov rdi, format_num
    mov rsi, rax
    mov rax, 0
    call printf
    jmp L1
L0:
    mov rax, [rbp - 24]
    mov rdi, format_str
    mov rsi, rax
    mov rax, 0
    call printf
L1:
    mov rax, [rbp - 16]
    mov rdi, format_num
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, [rbp - 8]
    mov rdi, format_num
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, 0
    mov [rbp - 48], rax ; Declare i
L2:
    mov rax, 5
    push rax
    mov rax, [rbp - 48]
    pop rbx
    cmp rax, rbx
    jge L3
    mov rax, [rbp - 24]
    mov rdi, format_str
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, 1
    push rax
    mov rax, [rbp - 48]
    pop rbx
    add rax, rbx
    mov [rbp - 48], rax ; Assign to i
    jmp L2
L3:

.exit_program:
    mov rax, 60     ; syscall for exit
    mov rdi, 0      ; exit code 0
    syscall
