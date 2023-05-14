# LAB 2

## 1 搭建实验环境

### 1.1 测试程序编译

编译命令如下：

```bash
g++ labs/lab2/iaxpy.cpp util/m5/src/abi/x86/m5op.S -o labs/lab2/daxpy-gem5 --static --std=gnu++0x -O2 -I ./include 
```

### 1.2 仿真环境搭建

编译X86仿真程序：

```bash
scons build/X86/gem5.debug -j 8
```

使用该仿真程序测试上一步中得到的`daxpy-gem5`，测试指令如下：

```bash
build/X86/gem5.debug configs/example/se.py -c labs/lab2/daxpy-gem5
```

## 2 关于处理器性能的问题

### 2.1 设置仿真脚本

* 默认O3 CPU的commit width：8

首先查看`hw2/`中O3 CPU的定义：

```python
class MyO3CPU(DerivO3CPU):

    def __init__(self, options=None):
        super(MyO3CPU, self).__init__()

        self.fuPool = MyO3FUPool(options)
```

该文件仅重写了Function Unit中的一些配置，而没有找到与commit width相关的配置。因此查找其父类`DerivO3CPU`中的定义。该定义位于`gem5-stable/src/cpu/o3/O3CPU.py`中，在该文件的119行，可以看到commit width的定义：

```python
commitWidth = Param.Unsigned(8, "Commit width")
```
   
* 默认ROB(reorder buffer)的entries size：192

minor CPU在执行指令时是in-order的，因此不需要ROB。我们只需要查找O3 CPU中的ROB的定义即可。该定义位于`gem5-stable/src/cpu/o3/O3CPU.py`中，在该文件的159行，可以看到commit width的定义：

```python
numROBEntries = Param.Unsigned(192, "Number of reorder buffer entries")
```

* 默认的L1 Cache的大小：16kB(instruction)/64kB(data)

在`gem5-stable/configs/hw2/caches.py`中，可以看到L1ICache和L1DCache的定义：

```python
class L1ICache(L1Cache):
    """Simple L1 instruction cache with default values"""
    # Set the default size
    size = '16kB'

    # ...

class L1DCache(L1Cache):
    """Simple L1 data cache with default values"""
    # Set the default size
    size = '64kB'
```

* 默认的L1 Cache映射和替换策略：2路组相连、Most recently used

`gem5-stable/configs/hw2/caches.py`的54行给出了替换策略：

```python
assoc = 2
# ...
replacement_policy = MRURP()
```

* 默认的除法单元的操作的延时：

* 默认的整数ALU单元的延时：

* 默认的ALU(add/sub)和整数乘除(mul/div)的运算单元数量：


### 2.2（*选做）采用 `objdump -S` 指令导出`daxpy-gem5`的汇编代码 

指令如下：

```bash
objdump -S labs/lab2/daxpy-gem5 > labs/lab2/daxpy-gem5.s
```

提取出`daxpy-gem5.s`中关于iaxpy loop的汇编，感兴趣的部分位于590~605行：

```assembly
40177f:	e8 a4 08 00 00       	callq  402028 <m5_dump_reset_stats>
401784:	4c 89 e1             	mov    %r12,%rcx
401787:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
40178e:	00 00 
401790:	8b 03                	mov    (%rbx),%eax
401792:	48 83 c3 04          	add    $0x4,%rbx
401796:	99                   	cltd   
401797:	41 f7 fd             	idiv   %r13d
40179a:	01 01                	add    %eax,(%rcx)
40179c:	48 83 c1 04          	add    $0x4,%rcx
4017a0:	4c 39 e3             	cmp    %r12,%rbx
4017a3:	75 eb                	jne    401790 <main+0x120>
4017a5:	31 f6                	xor    %esi,%esi
4017a7:	31 ff                	xor    %edi,%edi
4017a9:	48 81 c3 40 9c 00 00 	add    $0x9c40,%rbx
4017b0:	e8 73 08 00 00       	callq  402028 <m5_dump_reset_stats>
```

### 2.3 采用提供的`run.py`（而非之前使用的`se.py`）分别用 Minor 和 O3 CPU 类型仿真 daxpy-gem5

#### 2.3.1 Minor CPU和O3 CPU上运行的IPC分别是多少？

仿真Minor CPU：

```bash
build/X86/gem5.debug configs/hw2/run.py MINOR
```

IPC结果如下：

```bash
$ grep -r -n "ipc" /home/nullptr/gem5-stable/m5out/minor_stats.txt   
19:system.cpu.ipc                               0.358846                       # IPC: instructions per cycle
814:system.cpu.ipc                               0.177595                       # IPC: instructions per cycle
1380:system.cpu.ipc                               0.200624                       # IPC: instructions per cycle
```

仿真O3 CPU：

```bash
build/X86/gem5.debug configs/hw2/run.py O3
```

IPC结果如下：

```bash
$ grep "IPC" -r -n /home/nullptr/gem5-stable/m5out/o3_stats.txt  
55:system.cpu.ipc                               1.122804                       # IPC: Instructions Per Cycle
56:system.cpu.ipc_total                         1.122804                       # IPC: Total IPC of All Threads
1131:system.cpu.ipc                               0.490326                       # IPC: Instructions Per Cycle
1132:system.cpu.ipc_total                         0.490326                       # IPC: Total IPC of All Threads
2114:system.cpu.ipc                               1.069072                       # IPC: Instructions Per Cycle
2115:system.cpu.ipc_total                         1.069072                       # IPC: Total IPC of All Threads
```

#### 2.3.2 如果除法单元的操作延时变更为10 cycles，Minor 和 O3类型的 CPU，哪一种性能上受到的影响比较大，为什么？

指令分别如下：

```bash
$ build/X86/gem5.debug configs/hw2/run.py MINOR --alu-div-operation-latency 10
```

```bash
$ build/X86/gem5.debug configs/hw2/run.py O3 --alu-div-operation-latency 10
```

结果如下：

| div latency | 1        | 10       |
| ----------- | -------- | -------- |
| Minor       | 0.177595 | 0.076236 |
| O3          | 0.490326 | 0.101261 |

### 2.4 Cache 设计的性能分析

#### 2.4.1 分析L1 Cache大小对性能的影响

L1 Cache的默认配置为L1ICache 16kB，L1DCache 64kB。按照2倍的比例分别放大/缩小Cache大小，脚本如下：

```bash
for i in 8 16 32; do
    for j in 32 64 128; do
        build/X86/gem5.debug configs/hw2/run.py O3 --l1i_size=${i}kB --l1d_size=${j}kB
        mv m5out/stats.txt m5out/${i}_${j}_O3_stats.txt
        build/X86/gem5.debug configs/hw2/run.py MINOR --l1i_size=${i}kB --l1d_size=${j}kB
        mv m5out/stats.txt m5out/${i}_${j}_MINOR_stats.txt
    done
done

```

Minor CPU的仿真结果如下：

| L1 instruction size \ L1 data size | 32       | 64       | 128      |
| ---------------------------------- | -------- | -------- | -------- |
| 8                                  | 0.172457 | 0.177586 | 0.185842 |
| 16                                 | 0.172466 | 0.177595 | 0.185852 |
| 32                                 | 0.172466 | 0.177595 | 0.185852 |

O3 CPU的仿真结果如下：

| L1 instruction size \ L1 data size | 32       | 64       | 128      |
| ---------------------------------- | -------- | -------- | -------- |
| 8                                  | 0.486356 | 0.490326 | 0.499697 |
| 16                                 | 0.486356 | 0.490326 | 0.499697 |
| 32                                 | 0.486356 | 0.490326 | 0.499697 |

#### 2.4.2 分析不同替换策略对代码执行性能的影响

gem5中，随机替换法使用`RandomRP()`类，最近最少使用法使用`LRURP()`类，除此之外，其他设置均保持默认。随机替换法在替换时，会随机选择一个块进行替换；最近最少使用法在替换时，会选择最近最少使用的内容作为替换对象。

| replace strategy      | LRU      | Random   |
| --------------------- | -------- | -------- |
| Minor                 | 0.175156 | 0.177766 |
| O3                    | 0.488265 | 0.491776 |

### 3 设计方法综合探究