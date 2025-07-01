#ifndef ARCH_H__
#define ARCH_H__

#ifdef __riscv_e
#define NR_REGS 16
#else
#define NR_REGS 32
#endif

// mepc寄存器 - 存放触发异常的PC
// mstatus寄存器 - 存放处理器的状态
// mcause寄存器 - 存放触发异常的原因
struct Context {
  // fix the order of these members to match trap.S
  uintptr_t gpr[NR_REGS], mcause, mstatus, mepc;
  void *pdir;
};

// # define ARGS_ARRAY ("ecall", "a7", "a0", "a1", "a2", "a0")
#ifdef __riscv_e
#define GPR1 gpr[15] // a5
#else
#define GPR1 gpr[17] // a7
#endif

#define GPR2 gpr[10]   // a0
#define GPR3 gpr[11]   // a1
#define GPR4 gpr[12]   // a2
#define GPRx gpr[10]  // a0

#endif
