	lw	1	0	iless
	lw	2	0	jalra
	lw	3	0	jalrb
	lw	4	0	jalrc
	lw	5	0	one
	add	7	5	7
	beq	7	1	end
	jalr	6	2
	jalr	6	4
	jalr	6	3
end	halt
iless	.fill	10
jalra	.fill	8
jalrb	.fill	7
jalrc	.fill	5
one	.fill	1
