	lw	1	0	iless
	lw	2	0	jless
	lw	5	0	one
start	beq	3	1	end
	add	3	5	3
	add	4	0	0
in	beq	4	2	start
	add	4	5	4
	beq	0	0	in
end	halt
iless	.fill	20
jless	.fill	2
one	.fill	1
