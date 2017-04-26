// opcodes
#define OP_ADD 0
#define OP_SUB 1
#define OP_AND 2
#define OP_OR  3
#define OP_SLL 4
#define OP_SRA 5
#define OP_SRL 6
#define OP_BEQ 7
#define OP_BGT 8
#define OP_BLE 9
#define OP_BNE 10
#define OP_JAL 11
#define OP_LW  12
#define OP_SW  13
#define OP_LHI 14
#define OP_HALT 15

// main memory
#define MEM_SIZE (1 << 16)

// extract single bit
static inline int sb(int x, int bit)
{
	return (x >> bit) & 1;
}

// extract multiple bits
static inline int sbs(int x, int msb, int lsb)
{
	if (msb == 31 && lsb == 0)
		return x;
	return (x >> lsb) & ((1 << (msb - lsb + 1)) - 1);
}
