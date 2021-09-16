        .pos 0
	irmovq stack, %rsp      
	call main
	halt

# Array of 4 elements
        .align 8
data:
        .quad   0x0000000000000004
        .quad   0x0000000000000003
        .quad   0x0000000000000002
        .quad   0x0000000000000001

main:
        irmovq  data, %rdi
	irmovq  $4, %rsi
	call    bubble_p
	ret
bubble_p:
        addq    %rsi, %rsi      ; %rsi = 2*count
        addq    %rsi, %rsi      ; %rsi = 4*count
        addq    %rsi, %rsi      ; %rsi = 8*count
        rrmovq  $8, %r8
        subq    %r8, %rsi        ; %rsi = 8*(count -1)
        addq    %rdi, %rsi      ; %rsi = data + 8*(count-1) (last)
	## leaq	-8(%rdi,%rsi,8), %rsi 
	jmp	.L2
.L3:                            ; inner loop update
	addq	$8, %rax        ; p++
.L5:                            ; inner loop condition
        rrmovq  %rax, %r8       ; copy %rax (last) to compare (p)
        subq    %rsi, %r8               
        ## cmpq	%rsi, %rax      
	jnb	.L7             ; p >= last
	mrmovq	8(%rax), %rdx   ; %rdx = *(p+1)
	mrmovq	(%rax), %rcx    ; %rcx = *p
        rrmovq  %rdx, %r8       ; copy data to compare
	subq	%rcx, %r8       
	jge	.L3             ; *(p+1) >= *p
	rmmovq	%rcx, 8(%rax)   ; *(p+1) = *p
	movq	%rdx, (%rax)    ; *p = *(p+1) (old value)
	jmp	.L3             
.L7:                            ; outer loop 
        rrmovq  $8, %r8
        subq	%r8, %rsi       ; last--
.L2:                            ; outer loop condition
        rrmovq  %rsi, %r8       ; copy %rsi data (last) to compare
        subq    %rdi, %r8       
        ## cmpq	%rdi, %rsi
	jbe	.L8             ; last <= data
	rrmovq	%rdi, %rax      ; %rax = last (p)
	jmp	.L5
.L8:                            ; finish
	ret

.pos 0x200
stack:
