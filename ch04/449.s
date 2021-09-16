        .pos 0
	irmovq stack, %rsp      
	call main
	halt

# Array of 4 elements
        .align 8
data:
        .quad   0x0000000000000005
        .quad   0x0000000000000004
        .quad   0x0000000000000003
        .quad   0x0000000000000002
        .quad   0x0000000000000001

main:
        irmovq  data, %rdi
	irmovq  $5, %rsi
	call    bubble_p
	ret
bubble_p:
        addq    %rsi, %rsi      # %rsi = 2*count
        addq    %rsi, %rsi      # %rsi = 4*count
        addq    %rsi, %rsi      # %rsi = 8*count
        irmovq  $8, %r8
        subq    %r8, %rsi        # %rsi = 8*(count -1)
        addq    %rdi, %rsi      # %rsi = data + 8*(count-1) (last)
	## leaq	-8(%rdi,%rsi,8), %rsi 
	jmp	L2
L3:                            # inner loop update
        irmovq  $8, %r8
	addq	%r8, %rax        # p++
L5:                            # inner loop condition
        rrmovq  %rax, %r8       # copy %rax (last) to compare (p)
        subq    %rsi, %r8               
        ## cmpq	%rsi, %rax      
	jge	L7             # p >= last
	mrmovq	8(%rax), %rdx   # %rdx = *(p+1)
	mrmovq	(%rax), %rcx    # %rcx = *p
        rrmovq  %rdx, %r8       # copy data to compare
	subq	%rcx, %r8       # calculate delta %r8 = *(p+1) - *p 
        ##
        irmovq  $0, %r9
        cmovg   %r9, %r8        # if (delta>0) delta=0
        subq    %r8, %rdx       # update *(p+1)
        addq    %r8, %rcx       # update *p
        rmmovq	%rdx, 8(%rax)   # write *(p+1)
	rmmovq	%rcx, (%rax)    # write *p
        ##
	jmp	L3             
L7:                            # outer loop 
        irmovq  $8, %r8
        subq	%r8, %rsi       # last--
L2:                            # outer loop condition
        rrmovq  %rsi, %r8       # copy %rsi data (last) to compare
        subq    %rdi, %r8       
        ## cmpq	%rdi, %rsi
	jle	L8             # last <= data
	rrmovq	%rdi, %rax      # %rax = last (p)
	jmp	L5
L8:                            # finish
	ret

.pos 0x200
stack:
