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

* 默认的除法单元的操作的延时：1

在`gem5-stable/configs/hw2/cpu.py`由以下语句决定：

```python
options.add_option('--alu-div-operation-latency', type=int, default=1,
                   help="Latency of integer divide operations. Default is 1")
```

* 默认的整数ALU单元的延时：3(Minor CPU)/1(O3 CPU)

这两种CPU的定义均没有出现在提供的配置文件中。需要在gem5的源码中查找。

Minor CPU的整数ALU单元延时定义位于`gem5-stable/src/cpu/minor/MinorCPU.py`的109行：

```python
class MinorDefaultIntFU(MinorFU):
    opClasses = minorMakeOpClassSet(['IntAlu'])
    timings = [MinorFUTiming(description="Int",
        srcRegsRelativeLats=[2])]
    opLat = 3
```

O3 CPU的整数ALU单元延时定义位于`gem5-stable/src/cpu/FuncUnit.py`的63行（在`gem5-stable/src/cpu/o3/FuncUnitConfig.py`中未找到opLat字段，因此需要查看其基类`FUDesc`）：

```python
class OpDesc(SimObject):
    # ...
    opLat = Param.Cycles(1, "cycles until result is available")
    # ...
```

* 默认的ALU(add/sub)和整数乘除(mul/div)的运算单元数量：4/1

由`gem5-stable/configs/hw2/cpu.py`的以下语句决定：

```python
options.add_option('--alu-units', type=int, default=4,
                   help="Number of integer ALUs units. Default is 4")
options.add_option('--int-div-units', type=int, default=1,
                   help="Number of integer div ALUs units. Default is 1")
```

### 2.2（*选做）采用 `objdump -S` 指令导出`daxpy-gem5`的汇编代码 

指令如下：

```bash
objdump -S labs/lab2/daxpy-gem5 > labs/lab2/daxpy-gem5.s
```

提取出`daxpy-gem5.s`中关于iaxpy loop的汇编，感兴趣的部分位于590-605（共14）行：

```assembly
40177f:	e8 a4 08 00 00       	callq  402028 <m5_dump_reset_stats>
401784:	4c 89 e1             	mov    %r12,%rcx
401787:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
40178e:	00 00 
401790:	8b 03                	mov    (%rbx),%eax # load X[i]
401792:	48 83 c3 04          	add    $0x4,%rbx # address shift of X[i]
401796:	99                   	cltd   
401797:	41 f7 fd             	idiv   %r13d # X[i] / alpha
40179a:	01 01                	add    %eax,(%rcx) # Y[i] += X[i] / alpha
40179c:	48 83 c1 04          	add    $0x4,%rcx # address shift of Y[i]
4017a0:	4c 39 e3             	cmp    %r12,%rbx # compare i and N
4017a3:	75 eb                	jne    401790 <main+0x120> # jump to 401790 if i != N
4017a5:	31 f6                	xor    %esi,%esi
4017a7:	31 ff                	xor    %edi,%edi
4017a9:	48 81 c3 40 9c 00 00 	add    $0x9c40,%rbx
4017b0:	e8 73 08 00 00       	callq  402028 <m5_dump_reset_stats>
```

与计算无关的指令为：

```assembly
401784:	4c 89 e1             	mov    %r12,%rcx # 加载Y[i]的地址
401787:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1) # 用于对齐
40178e:	00 00  # 用于对齐
401790:	8b 03                	mov    (%rbx),%eax # 加载X[i]的地址
401796:	99                   	cltd   # sign-extends the value in EAX into the EDX:EAX register pair.
4017a3:	75 eb                	jne    401790 <main+0x120> # 循环跳转
```

由于在O3 CPU中，默认的整数ALU单元的延时和默认的除法单元的操作的延时均为1，如果指令间充分并行/且没有冲突，IPC的上限为8。当然在实际操作中，IPC会小于8。

### 2.3 采用提供的`run.py`（而非之前使用的`se.py`）分别用 Minor 和 O3 CPU 类型仿真 daxpy-gem5

首先需要声明的是，由于主体程序被`m5_dump_reset_stats(0, 0);`分成了三段，因此要想查看感兴趣部分的参数，我们需要在第二个`Begin Simulation Statistics`和`End Simulation Statistics`之间的部分进行查看。

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

据此可以得到IPC：

| CPU         | IPC      |
| ----------- | -------- |
| Minor       | 0.177595 |
| O3          | 0.490326 |

这里可以看到，O3 CPU的IPC与上一节的预估有着极大的差距。

#### 2.3.2 如果除法单元的操作延时变更为10 cycles，Minor 和O3类型的 CPU，哪一种性能上受到的影响比较大，为什么？

指令分别如下：

```bash
$ build/X86/gem5.debug configs/hw2/run.py MINOR --alu-div-operation-latency 10
```

```bash
$ build/X86/gem5.debug configs/hw2/run.py O3 --alu-div-operation-latency 10
```

结果如下：

| CPU\div latency | 1        | 10       |
| ----------- | -------- | -------- |
| Minor       | 0.177595 | 0.076236 |
| O3          | 0.490326 | 0.101261 |

除法操作延时变更为10 cycles后，Minor CPU的IPC变为原来的42.9%，而O3 CPU的IPC变为原来的20.6%，即O3 CPU的性能下降更多。

这是由于，O3 CPU使用乱序执行，充分利用了指令间的并行性，这时候，限制IPC提升的主要因素是单个指令的延时。

#### 2.3.3 在默认的延时设定下，每一种 CPU 类型下branch prediction error数量分别是多少？优化branch predictor是否必要？

查看与分支预测有关的属性：

```bash
grep branch ./m5out/minor_stats.txt ./m5out/o3_stats.txt
```

结果如下：

```bash
# minor CPU
...
system.cpu.branchPred.condPredicted                 0                       # Number of conditional branches predicted
system.cpu.branchPred.condIncorrect                 0                       # Number of conditional branches incorrect
...

# O3 CPU
./m5out/o3_stats.txt:system.cpu.branchPred.condPredicted             20008                       # Number of conditional branches predicted
./m5out/o3_stats.txt:system.cpu.branchPred.condIncorrect                 8                       # Number of conditional branches incorrect
```

对于minor CPU，由于默认情况下其不支持分支预测，遂不对其进行进一步分析；而对于O3 CPU，其预测次数为20008次，错误次数为8次，因此其分支预测错误率为0.0004，可以说是非常低的。因此，对于O3 CPU，优化分支预测并不是必要的。

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

初步可以观测到，L1 Instruction Cache的大小对性能几乎没有影响。这是因为，感兴趣的部分只有14条汇编指令，对指令的访问不应该成为性能的主要瓶颈。对L1 Data Cache进行进一步仿真（固定L1 Instruction Cache大小为16kB）：

```bash
for i in 8 16 32 64 128 256 512; do
    build/X86/gem5.debug configs/hw2/run.py O3 --l1i_size=64kB --l1d_size=${i}kB
    mv m5out/stats.txt m5out/64_${i}_O3_stats.txt
    build/X86/gem5.debug configs/hw2/run.py MINOR --l1i_size=64kB --l1d_size=${i}kB
    mv m5out/stats.txt m5out/64_${i}_MINOR_stats.txt
done
```

| CPU type\L1 data size | 8        | 16       | 32       | 64       | 128      | 256      | 512      |
| --------------------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
| Minor                 | 0.170336 | 0.170336 | 0.172466 | 0.177595 | 0.185852 | 0.186057 | 0.186057 |
| O3                    | 0.482776 | 0.482776 | 0.486356 | 0.490326 | 0.499697 | 0.499838 | 0.499838 |

可以发现，当L1 data Cache的size在32kB-128kB之间时，提升性能的效果最好，而在其他区间中，性能提升的效果不明显。我们不妨从程序本身的角度分析这一现象：

1. 当cache size大于128kB时，由于程序中访存的主体是两个长度为10000的数组`X[N]`和`Y[N]`，总大小为10000x2x4=80kiB，可以完整放入cache中，因此cache size的增大并不能带来性能的提升。分析stat文件可知，程序的cache命中率几乎达到了100%：

```
system.cpu.dcache.overall_hits::.cpu.data        30004                       # number of overall hits
system.cpu.dcache.overall_hits::total           30004                       # number of overall hits
```

2. 当cache size小于32kB时，我们需要结合cache的特性分析。使用`gdb`获取X和Y的地址分布：

```bash
(gdb) print(&X)
$2 = (int (*)[10000]) 0x7ffffffe98d0
(gdb) print(&Y)
$3 = (int (*)[10000]) 0x7fffffff3510
```

由于0x7fffffff3510 - 0x7ffffffe98d0 = 40000，可见X和Y在地址上是连续的，地址相差40000。由于cache是2路组相连，块大小为64 bytes。每当发生cache miss时，cache都会从X和Y中分别读取16个整数，接下来的16次访存操作中就一定不会发生miss。因此，只要cache的大小仍然在128 bytes以上，cache的命中率就不会继续下降。分析stat文件可知，程序的cache命中率在cache size为8kB时约为6.25%，与分析一致。

```
system.cpu.dcache.demand_miss_rate::.cpu.data     0.062510                       # miss rate for demand accesses
system.cpu.dcache.demand_miss_rate::total     0.062510    
```

#### 2.4.2 分析不同替换策略对代码执行性能的影响

gem5中，随机替换法使用`RandomRP()`类，最近最少使用法使用`LRURP()`类，除此之外，其他设置均保持默认。RANDOM（随机替换法）在替换时，会随机选择一个块进行替换；LRU（最近最少使用法）在替换时，会选择最近最少使用的内容作为替换对象。将这两种替换方法与默认的MRU（最近最常使用算法，先移除最近最常使用的条目）进行比较：

| CPU\replace strategy      | LRU      | Random   | MRU      |
| ------------------------- | -------- | -------- | -------- |
| Minor                     | 0.175156 | 0.177766 | 0.177595 |
| O3                        | 0.488265 | 0.491776 | 0.490326 |

让人颇为惊奇的是，随机替换法取得了最好的效果。这可能与问题情境有关：当循环访问`X[i]`和`Y[i]`时，数据是顺序访问的，不会有最近最少和最近最常使用的区别，而随机替换反而减小了替换开销，因此随机替换法性能更好。

### 3 设计方法综合探究

> 假设你得到一个设计X86 CPU的机会。有两种处理器架构可以选择：一款是O3 CPU，1GHz。一款是高速的In order CPU, 3GHz；有两种除法运算单元可以选择：一款是高速除法器，可以每1个周期执行一个除法运算，有10周期的issue延时，既可以用于O3 CPU也可以用于In order CPU；第二种是流水线型除法器，可以每10个周期执行一次除法操作，issue延时为1个周期，但只能用于In order CPU。如果奇葩的CTO只关心执行iaxpy程序的性能，你应该如何设计CPU？请简述理由，向CTO推销你想采取的CPU模型和除法器选择。

首先考虑in order CPU，对两种架构的除法器分别进行仿真（注意`system.clk_domain.clock = '3GHz'`）：

高速除法器：

```bash
build/X86/gem5.debug configs/hw2/run.py MINOR --alu-div-operation-latency 1 --alu-div-issue-latency 10
```

仿真结果：

``` 
system.cpu.ipc                               0.080266
```

流水线除法器：

```bash
build/X86/gem5.debug configs/hw2/run.py MINOR --alu-div-operation-latency 10 --alu-div-issue-latency 1
```

仿真结果：

```
system.cpu.ipc                               0.075799
```

可见，高速除法器的性能稍好。实际上，就本例而言，由于除法是顺序执行的，因此将除法流水线化意义不大——尽管发射延时小，但流水线除法器并不能在in order场景下充分发挥流水优势。

随后是O3 CPU，O3 CPU仅能使用高速除法器（注意`system.clk_domain.clock = '1GHz'`）：

```bash
build/X86/gem5.debug configs/hw2/run.py O3 --alu-div-operation-latency 1 --alu-div-issue-latency 10
```

仿真结果：

```
system.cpu.ipc                               0.489540
```

考虑到两种CPU之间的主频差距，可以看到O3 CPU的性能要优于两种in order CPU。因此，我会向CTO推荐O3 CPU + 高速除法器的组合。