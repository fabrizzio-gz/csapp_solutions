	.globl	find_range_alt374
	.type	find_range_alt374, @function
find_range_alt374:
        movl $1, %eax                 # Default result = 1 if x == 0
        vxorps %xmm1, %xmm1, %xmm1    # Generate %xmm1 = 0
        vucomiss %xmm1, %xmm0         # compare x : 0
        movl $2, %edi                 
        cmova %edi, %eax              # if x > 0, result = 2
        movl $0, %edi
        cmovb %edi, %eax              # if x < 0, result = 0
        movl $3, %edi                 
        cmovp %edi, %eax              # if x is NaN, result = 3
        ret

