BITS 64

section .multiboot align=4
  dd 0x1BADB002        ; magic
  dd 0x00010003        ; flags: ALIGN_PAGE + MEMORY_INFO
  dd -(0x1BADB002 + 0x00010003)  ; checksum

section .text
extern start
global _start
_start:
  mov rdi, 0
  call start
  cli
  hlt
