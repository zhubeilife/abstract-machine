#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>
#include <math.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      // 对于mips32的syscall和riscv32的ecall, 保存的是自陷指令的PC, 因此软件需要在适当的地方对保存的PC加上4, 使得将来返回到自陷指令的下一条指令.
      case 11: ev.event = EVENT_YIELD; c->mepc += 4; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  // 在RISC-V的用户程序中，使用ecall指令请求syscall。操作系统提供不同功能的syscall，对应不同的请求调用号。因此请求前，需要向a7寄存器中写入所请求的syscall的调用号
  // 这里强制改成了11，为了让difftest通过
  // rv的手册发现是自己没好好看手册的问题，mcause=0xb表示的是environment call from M-mode，由于我们全程都在M模式下跑，因此ecall对应的mcause就是0xb
  asm volatile("li a7, 11; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
