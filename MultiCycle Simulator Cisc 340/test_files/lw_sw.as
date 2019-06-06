	lw	1	0	one
	lw	2	0	one
	lw	3	0	one
	lw	4	0	one
	lw	5	0	one
	lw	6	0	one
	lw	7	0	max
start	add	1	1	2
	lw	3	0	2
	lw	4	0	2
	sw	5	0	2
	sw	6	0	2
	beq	1	7	end
	beq	0	0	start
	noop
end	halt
one	.fill	1
max	.fill	10
