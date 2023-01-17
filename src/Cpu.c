#include "Cpu.h"
#include "Memory.h"

struct state* gb_state;

#include <string.h>

//#include "lib/tester.h"

static int num_mem_accesses;
static struct mem_access mem_accesses[16];

void cpu_init(size_t tester_instruction_mem_size, u8* tester_instruction_mem) {

	init_memory(tester_instruction_mem_size, tester_instruction_mem);

	gb_state->reg16.BC = 0x00;
	gb_state->reg16.DE = 0x00;
	gb_state->reg16.HL = 0x00;
	gb_state->reg16.AF = 0x00;

	gb_state->SP = 0x00;
	gb_state->PC = 0x00;

	gb_state->halted = false;
	gb_state->interrupts_master_enabled = false;
	gb_state->num_mem_accesses = 0;
	gb_state->mem_accesses 
}

void cpu_set_state(struct state* state) {

	(void)state;

	num_mem_accesses = 0;

	gb_state->reg16.BC = state->reg16.BC;
	gb_state->reg16.DE = state->reg16.DE;
	gb_state->reg16.HL = state->reg16.HL;
	gb_state->reg16.AF = state->reg16.AF;

	gb_state->SP = state->SP;
	gb_state->PC = state->PC;

	gb_state->halted = state->halted;
	gb_state->interrupts_master_enabled = state->interrupts_master_enabled;
	gb_state->num_mem_accesses = 0;

}

void cpu_get_state(struct state* state) {

	state->num_mem_accesses = num_mem_accesses;
	memcpy(state->mem_accesses, mem_accesses, sizeof(mem_accesses));

	state->reg16.BC = gb_state->reg16.BC;
	state->reg16.DE = gb_state->reg16.DE;
	state->reg16.HL = gb_state->reg16.HL;
	state->reg16.AF = gb_state->reg16.AF;

	state->SP = gb_state->SP;
	state->PC = gb_state->PC;

	state->halted = gb_state->halted;
	state->interrupts_master_enabled = gb_state->interrupts_master_enabled;

}

int cpu_step(void) {

	int cycles = 0;

	//First load instruction from PC;
	u8 cur = MEMORY[gb_state->PC];

	//Determine instruction to perform based on OP code
	switch (cur) {
		case 0x0E:
			cycles = 8;
			gb_state->PC += 2;
			LD_C_d8();
			break;
	}

	return cycles;
}

void LD_C_d8() {
	u8 arg = MEMORY[gb_state->PC++];
	gb_state->reg8.C = arg;
	return;
}
























//void LD_BC_d16();
//void LD_DE_d16();
//void LD_HL_d16();
//void LD_SP_d16();
//void LD_a16_SP();
//void LD_HL_SPpr8();
//void LD_SP_HL();
//
//
//void LD_BC_A();
//void LD_DE_A();
//void LD_HLp_A();
//void LD_HLm_A();
//
//void LD_A_BC();
//void LD_A_DE();
//void LD_A_HLp();
//void LD_A_HLm();
//
//
//void PUSH_BC();
//void PUSH_DE();
//void PUSH_HL();
//void PUSH_AF();
//
//
//void POP_BC();
//void POP_DE();
//void POP_HL();
//void POP_AF();






//8bit LOAD INSTRUCTIONS






// THE LOAD Bs

//
////LD B in address B
//void LD_B_B();
//
////LD B in address D
//void LD_D_B();
//
////LD B in address H
//void LD_H_B();
//
////LD B in address HL
//void LD_HL_B();


// THE LOAD Cs


////LD C in address B
//void LD_B_C();
//
////LD C in address D
//void LD_D_C();
//
////LD C in address H
//void LD_H_C();
//
////LD C in address HL
//void LD_HL_C();
//
//
//// THE LOAD Ds

//
//void LD_B_D();
//void LD_D_D();
//void LD_H_D();
//void LD_HL_D();
//

// THE LOAD Es


//void LD_B_E();
//void LD_D_E();
//void LD_H_E();
//void LD_HL_E();
//
//
//// THE LOAD Hs
//
//
//void LD_B_H();
//void LD_D_H();
//void LD_H_H();
//void LD_HL_H();


// THE LOAD Ls
//
//
//void LD_B_L();
//void LD_D_L();
//void LD_H_L();
//void LD_HL_L();
//
//
//// THE LOAD HLs
//
//
//void LD_B_HL();
//void LD_D_HL();
//void LD_H_HL();
//
//
//// THE LOAD As
//
//void LD_B_A();
//void LD_D_A();
//void LD_H_A();
//void LD_HL_A();
//
////THE LOAD Bs
//
//void LD_C_B();
//void LD_E_B();
//void LD_L_B();
//void LD_A_B();
//
//
////THE LOAD Cs
//
//
//void LD_C_C();
//void LD_E_C();
//void LD_L_C();
//void LD_A_C();
//
//
////THE LOAD Ds
//

//void LD_C_D();
//void LD_E_D();
//void LD_L_D();
//void LD_A_D();
//
//
////THE LOAD Es
//
//
//void LD_C_E();
//void LD_E_E();
//void LD_L_E();
//void LD_A_E();
//
//
////THE LOAD Hs 
//
//
//void LD_C_H();
//void LD_E_H();
//void LD_L_H();
//void LD_A_H();
//
//
////THE LOAD Ls
//
//
//void LD_C_L();
//void LD_E_L();
//void LD_L_L();
//void LD_A_L();
//
//
////THE LOAD HLs
//
//
//void LD_C_HL();
//void LD_E_HL();
//void LD_L_HL();
//void LD_A_HL();


//THE LOAD As


//void LD_C_A();
//void LD_E_A();
//void LD_L_A();
//void LD_A_A();
//
//
////NOW INSTRUCTIONS TO LOAD IMMEDIATE 8bit DATA
//
//
//void LD_B_d8();
//void LD_D_d8();
//void LD_H_d8();
//void LD_HL_d8();
//
//void LD_C_d8() {
//	u8 arg = MEMORY[gb_state->PC++];
//	gb_state->reg8.C = arg;
//	return;
//}

//void LD_E_d8();
//void LD_L_d8();
//void LD_A_d8();
//
//
//
//
//
////NEXT IS ALL 8bit ARITHMETIC INSTRUCTIONS
//
//
//
//
////Bs
//void ADD_A_B();
//void SUB_B();
//void AND_B();
//void OR_B();
//void ADC_A_B();
//void SBC_A_B();
//void XOR_B();
//void CP_B();
//void INC_B();
//void DEC_B();
//
////Cs
//void ADD_A_C();
//void SUB_C();
//void AND_C();
//void OR_C();
//void ADC_A_C();
//void SBC_A_C();
//void XOR_C();
//void CP_C();
//void INC_C();
//void DEC_C();
//
////Ds
//void ADD_A_D();
//void SUB_D();
//void AND_D();
//void OR_D();
//void ADC_A_D();
//void SBC_A_D();
//void XOR_D();
//void CP_D();
//void INC_D();
//void DEC_D();
//
////Es
//void ADD_A_E();
//void SUB_E();
//void AND_E();
//void OR_E();
//void ADC_A_E();
//void SBC_A_E();
//void XOR_E();
//void CP_E();
//void INC_E();
//void DEC_E();
//
////Hs
//void ADD_A_H();
//void SUB_H();
//void AND_H();
//void OR_H();
//void ADC_A_H();
//void SBC_A_H();
//void XOR_H();
//void CP_H();
//void INC_H();
//void DEC_H();
//
////Ls
//void ADD_A_L();
//void SUB_L();
//void AND_L();
//void OR_L();
//void ADC_A_L();
//void SBC_A_L();
//void XOR_L();
//void CP_L();
//void INC_L();
//void DEC_L();
//
////HLs
//void ADD_A_HL();
//void SUB_HL();
//void AND_HL();
//void OR_HL();
//void ADC_A_HL();
//void SBC_A_HL();
//void XOR_HL();
//void CP_HL();
//void INC_HL();
//void DEC_HL();
//
////As
//void ADD_A_A();
//void SUB_A();
//void AND_A();
//void OR_A();
//void ADC_A_A();
//void SBC_A_A();
//void XOR_A();
//void CP_A();
//void INC_A();
//void DEC_A();
//
////8bit memory addresses
//void ADD_A_d8();
//void SUB_d8();
//void AND_d8();
//void OR_d8();
//void ADC_A_d8();
//void SBC_A_d8();
//void XOR_d8();
//void CP_d8();
//
////FLAG CHANGERS
//void CPL();
//void CCF();
//void DAA();
//void SCF();
//
//
//
//
//
////NEXT IS ALL 16bit ARITHMETIC INSTRUCTIONS
//
//
//
//
//void INC_BC();
//void INC_DE();
//void INC_HL();
//void INC_SP();
//void DEC_BC();
//void DEC_DE();
//void DEC_HL();
//void DEC_SP();
//void ADD_HL_BC();
//void ADD_HL_DE();
//void ADD_HL_HL();
//void ADD_HL_SP();
//
//
//
//
//
////NEXT IS ALL JUMP AND CALL INSTRUCTIONS
//
//
//
//
//void JR_NZ_r8();
//void JR_NC_r8();
//void JR_r8();
//void JR_Z_r8();
//void JR_C_r8();
//void RET_NZ();
//void RET_NC();
//void JP_NZ_a16();
//void JP_NC_a16();
//void JP_a16();
//void CALL_NZ_a16();
//void CALL_NC_a16();
//void CALL_Z_a16();
//void CALL_C_a16();
//void CALL_a16();
//void JP_HL();
//
//void RST_00H();
//void RST_10H();
//void RST_20H();
//void RST_30H();
//void RST_08H();
//void RST_18H();
//void RST_28H();
//void RST_38H();
//
//void RET_Z();
//void RET_C();
//void RET();
//void RETI();
//
//
//
//
////NEXT IS ALL SYSTEM INSTRUCTIONS
//
//
//
//
//void NOP();
//void STOP();
//void HALT();
//void EI();
//
////TODO: WRITE CPU INSTRUCTIONS FOR THE CB PREFIX
