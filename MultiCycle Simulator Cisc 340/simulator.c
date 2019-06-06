#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#define NUMREGS 8
#define NUMMEMORY 65536

typedef struct state_struct{
	int pc;
	int mem[NUMMEMORY];//should contain NUMMEMORY
	int reg[NUMREGS]; //should contain NUMREGS
	int num_memory;
} statetype;

void print_state(statetype *stateptr){
	int i;
	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", stateptr->pc);
	printf("\tmemory:\n");
	for(i=0; i<stateptr->num_memory; i++){
		printf("\tmem[%d]=%d\n", i, stateptr->mem[i]);
	}
	printf("\tregisters:\n");
	for(i=0; i<NUMREGS; i++){ //should be i<NUMREGS
		printf("\t\treg[%d]=%d\n", i, stateptr->reg[i]);
	}
	printf("end state\n");
}

void print_stats(int n_instrs){
	printf("INSTRUCTIONS: %d\n", n_instrs);
}

int convert_num(int num){
	if (num & (1<<15) ) {
		num -= (1<<16);
	}
	return(num);
}

int main(int argc, char **argv){
	FILE *file;
	int iflag = 0;
	int flag;
	char string[20];
	int instruction;
	int instructions = 0;
	int noHalt = 1;
	int lineNum = 0;
	int totInstructions = 0;

	statetype state;

	state.pc = 0;

	while((flag = getopt(argc, argv, "i:")) != -1){ //cool flag stuff
		switch(flag){
			case 'i':
				iflag = 1;
				break;
			case '?':
				fprintf(stderr, "Invalid flag\n");
				return 1;
		}
	}

	if(iflag == 0 || argv[2] == NULL){//if no input is given
		fprintf(stderr, "please provide an input file\n");
		return -1;
	}

	file = fopen(argv[2], "r");

	for(int i=0; i<20; i++){
		string[i] = '\0';
	}

	while(fgets(string, 20, file) != NULL){
		int powers[8] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
		int exponish = 0;
		instruction = 0;
		instructions++;

		for(int i=19; i>=0; i--){
			if(string[i] != '\0' && string[i] != '\n'){
				if(string[i] == '-'){
					instruction = instruction * -1;
				}
				else if(!(string[i] == '\0')){
				//	printf("%s", string);
					instruction = instruction + (string[i]-48) * powers[exponish];
					exponish++;
				}
			}
		}

		state.mem[lineNum] = instruction;
		lineNum++;

		for(int i=0; i<20; i++){
			string[i] = '\0';
		}
		state.num_memory++;
	}

	for(int i=0; i<NUMREGS; i++){
		state.reg[i] = 0;
	}

	while(1){
		int opcode = 0;
		int regA = 0;
		int regB = 0;
		int destReg = 0;
		int offset_field = 0;

		print_state(&state);
		//totInstructions++;

		//setting registers and stuff
		opcode = ((state.mem[state.pc] & (7 << 22)) >> 22);

		if(opcode <= 5){//sets regA and regB for r, i, and j types
			regA = ((state.mem[state.pc] & (7 << 19)) >> 19);
			regB = ((state.mem[state.pc] & (7 << 16)) >> 16);
		}

		if(opcode <= 1){//sets destReg for r types
			destReg = (state.mem[state.pc] & 7);
		}

		if(opcode == 2 || opcode == 3 || opcode ==4){//sets immediate
			offset_field = convert_num((state.mem[state.pc] & 0xFFFF));
		}

		//Actually doing stuff
		if(opcode == 0){
			state.reg[destReg] = state.reg[regA] + state.reg[regB];
			state.pc++;
			totInstructions = totInstructions + 2;
		}

		if(opcode == 1){
			state.reg[destReg] = ~(state.reg[regA] &  state.reg[regB]);
			state.pc++;
			totInstructions = totInstructions + 2;
		}

		if(opcode == 2){
			state.reg[regA] = state.mem[(state.reg[regB] + offset_field)];
			state.pc++;
			totInstructions = totInstructions + 5;
		}

		if(opcode == 3){
			state.mem[(state.reg[regB] + offset_field)] = state.reg[regA];
			state.pc++;
			if((state.reg[regB] + offset_field) >= state.num_memory){
				state.num_memory = state.reg[regB] + offset_field - 1;
			}
			totInstructions = totInstructions + 5;
		}

		if(opcode == 4){
			if(state.reg[regA] == state.reg[regB]){
				state.pc += (1 + offset_field);
			}
			else{
				state.pc++;
			}
			totInstructions = totInstructions + 3;
		}

		if(opcode == 5){
			state.reg[regA] = state.pc + 1;
			state.pc = state.reg[regB];
			totInstructions++;
		}

		if(opcode == 6){//sets noHalt for halt instructions
			state.pc++;
			print_state(&state);
			totInstructions++;
			printf("Machine Halted\n");
			printf("CYCLES: %i\n", totInstructions);
			return 0;
		}

		if(opcode == 7){
			totInstructions++;
			state.pc++;
		}
	}
}

