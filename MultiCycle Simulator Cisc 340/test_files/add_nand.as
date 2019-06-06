	lw	1	0	one
	lw	2	0	one
	lw	3	0	one
	lw	4	0	one
	lw	5	0	max
start	add	1	1	2
	add 3	3	2
	nand	4	4	3
	beq	5	1	end
	beq	0	0	start
	noop
end	halt
one	.fill	1
max	.fill	1000
