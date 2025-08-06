#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>
#include <math.h>

#include "klib-macros.h"

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      // 对于mips32的syscall和riscv32的ecall, 保存的是自陷指令的PC, 因此软件需要在适当的地方对保存的PC加上4, 使得将来返回到自陷指令的下一条指令.
      // ENVIRONMENT_CALL_FROM_M_MODE 11
      case 11: {
        int type = c->GPR1;
        if (type < 0) {
          ev.event = EVENT_YIELD;
        } else if (type >= 0) {
          ev.event = EVENT_SYSCALL;
        }else {
          printf("not support: c->GPR1 = %d\n", c->GPR1);
          panic("not support: c->GPR1 = %d\n");
        }
        c->mepc += 4;
        break;
      }
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

// kstack是栈的范围, entry是内核线程的入口, arg则是内核线程的参数
/*
        |               |
        +---------------+ <---- kstack.end
        |               |
        |    context    |
        |               |
        +---------------+ <--+
        |               |    |
        |               |    |
        |               |    |
        |               |    |
        +---------------+    |
        |       cp      | ---+
        +---------------+ <---- kstack.start
        |               |
 */
// kstack是栈的范围, entry是内核线程的入口, arg则是内核线程的参数. 此外, kcontext()要求内核线程不能从entry返回, 否则其行为是未定义的(也就是entry是个while(1)的).
Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* context = (Context*)(kstack.end - sizeof(Context));
  // or Context *context = (Context *)kstack.end - 1; 取决于将end看作是什么类型的指针
  // mepc:Machine Exception Program Counter
  context->mepc = (uintptr_t)entry;
  context->GPR2 = (uintptr_t)arg;
  context->mstatus = 0x00001800;
  return context;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
