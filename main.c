#include "x64_modrm.h"
#include <stdlib.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
	(byte & 0x80 ? '1' : '0'), \
	(byte & 0x40 ? '1' : '0'), \
	(byte & 0x20 ? '1' : '0'), \
	(byte & 0x10 ? '1' : '0'), \
	(byte & 0x08 ? '1' : '0'), \
	(byte & 0x04 ? '1' : '0'), \
	(byte & 0x02 ? '1' : '0'), \
	(byte & 0x01 ? '1' : '0')

void print_res(uint8_t val)
{
	printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(val));
	printf("%x\n", val);
}

void test_modrm_sib(int op1, int op2, effect_t which, int disp_type, int base, int scale_index, int scale)
{
	struct MODRM modrm;
	struct SIB sib;

	// this means need to use SIB addressing
	if (scale > 1)
		if (which == OP1) {
			op1 = ENONE;
		}
		else if (which == OP2) {
			op2 = ENONE;
		} else {
			fprintf(stderr, "incorrect, SIB addressing only available with effective addressing!\n");
			exit(1);
		}

	uint8_t res = calc_modrm_x64(op1, op2, which, disp_type, &modrm);
	printf("[MODRM] ");
	print_res(modrm_to_byte(modrm));

	if (res == CALC_SUCCESS_HAS_SIB) {
		calc_sib_x64(base, scale_index, scale, &sib);
		printf("\t[SIB] ");
		print_res(sib_to_byte(sib));
	}
}


int main()
{
	// mov	esp, dword ptr [ebp + ebp * 1 + 0x102030]
	test_modrm_sib(EBX, ECX, OP2, disp32, EDX, ECX, 1);
	// mov ebx, [ecx + esp * 2 + 0x102030]
	test_modrm_sib(EBX, ECX, OP2, disp32, ECX, ESP, 2);
}
