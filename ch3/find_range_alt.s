	.globl	find_range_alt
	.type	find_range_alt, @function
# float x in %xmm0
find_range_alt:
        vxorps %xmm1, %xmm1, %xmm1      # Generate %xmm1 = 0
        vucomiss %xmm1, %xmm0           # Compare x : 0
        jp .L1                          # Conditional jump if x is NaN
        ja .L2                          # Conditional jump if x > 0
        jb .L3                          # Conditional jump if x < 0
        movl $1, %eax                   # If none of the above, result = 1
        ret
.L1:
        movl $3, %eax                   # NaN. result = 3
        ret
.L2:
        movl $2, %eax                   # x > 0. result = 2
        ret
.L3:   
        movl $0, %eax                   # x < 0. result = 0
        ret
