# AM(Abstract machine)

## 模块

AM给我们展示了程序与计算机的关系: 利用计算机硬件的功能实现AM, 为程序的运行提供它们所需要的运行时环境.

```text
(在NEMU中)实现硬件功能 -> (在AM中)提供运行时环境 -> (在APP层)运行程序
```

AM(Abstract machine)作为一个向程序提供运行时环境的库, AM根据程序的需求把库划分成以下模块

```txt
AM = TRM + IOE + CTE + VME + MPE
```

+ TRM(Turing Machine) - 图灵机, 最简单的运行时环境, 为程序提供基本的计算能力
+ IOE(I/O Extension) - 输入输出扩展, 为程序提供输出输入的能力
+ CTE(Context Extension) - 上下文扩展, 为程序提供上下文管理的能力
+ VME(Virtual Memory Extension) - 虚存扩展, 为程序提供虚存管理的能力
+ MPE(Multi-Processor Extension) - 多处理器扩展, 为程序提供多处理器通信的能力 (MPE超出了ICS课程的范围, 在PA中不会涉及)

## 文件结构

abstract-machine/目录下的源文件组织如下(部分目录下的文件并未列出):

```bash
abstract-machine
├── am                                  # AM相关
│   ├── include
│   │   ├── amdev.h
│   │   ├── am.h
│   │   └── arch                        # 架构相关的头文件定义
│   ├── Makefile
│   └── src
│       ├── mips
│       │   ├── mips32.h
│       │   └── nemu                    # mips32-nemu相关的实现
│       ├── native
│       ├── platform
│       │   └── nemu                    # 以NEMU为平台的AM实现
│       │       ├── include
│       │       │   └── nemu.h
│       │       ├── ioe                 # IOE
│       │       │   ├── audio.c
│       │       │   ├── disk.c
│       │       │   ├── gpu.c
│       │       │   ├── input.c
│       │       │   ├── ioe.c
│       │       │   └── timer.c
│       │       ├── mpe.c               # MPE, 当前为空
│       │       └── trm.c               # TRM
│       ├── riscv
│       │   ├── nemu                    # riscv32(64)相关的实现
│       │   │   ├── cte.c               # CTE
│       │   │   ├── start.S             # 程序入口
│       │   │   ├── trap.S
│       │   │   └── vme.c               # VME
│       │   └── riscv.h
│       └── x86
│           ├── nemu                    # x86-nemu相关的实现
│           └── x86.h
├── klib                                # 常用函数库, klib是kernel library的意思, 用于提供一些兼容libc的基础功能.
├── Makefile                            # 公用的Makefile规则
└── scripts                             # 构建/运行二进制文件/镜像的Makefile
├── isa
│   ├── mips32.mk
│   ├── riscv32.mk
│   ├── riscv64.mk
│   └── x86.mk
├── linker.ld                       # 链接脚本
├── mips32-nemu.mk
├── native.mk
├── platform
│   └── nemu.mk
├── riscv32-nemu.mk
├── riscv64-nemu.mk
└── x86-nemu.mk
```

整个AM项目分为两大部分:
+ abstract-machine/am/ - 不同架构的AM API实现, 目前我们只需要关注NEMU相关的内容即可. 此外, abstract-machine/am/include/am.h列出了AM中的所有API.
+ abstract-machine/klib/ - 一些架构无关的库函数, 方便应用程序的开发