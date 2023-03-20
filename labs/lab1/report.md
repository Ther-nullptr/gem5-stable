# LAB 1

## 1 gem5的安装和测试

编译指定架构的处理器：

```bash
$ scons build/ARM/gem5.debug -j 4
```

编译`build/ARM/gem5.debug`后，使用如下指令进行测试：

```bash
$ build/ARM/gem5.debug configs/example/se.py -c tests/test-progs/hello/bin/arm/linux/hello
```

输出结果如下：

![image-20230312093003055](https://s2.loli.net/2023/03/12/lQXjg8tkrHSUV1f.png)

## 2 交叉编译

### 2.1 交叉编译器的安装使用

安装ARM指令集交叉编译器`gcc-aarch64-linux-gnu`：

```bash
$ sudo apt install gcc-aarch64-linux-gnu          
```

对八皇后程序`queens.c`进行交叉编译：

```bash
$ aarch64-linux-gnu-gcc -o labs/lab1/queens labs/lab1/queens.c --static
```

根据所提供的参数，在ARM架构上对可执行程序进行仿真：

```bash
$ build/ARM/gem5.debug configs/example/se.py -c labs/lab1/queens --cpu-type=MinorCPU --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l1d_assoc=4 --l1i_assoc=4 --l2_size=1MB --l2_assoc=8 --cacheline_size=64  --cpu-clock=1GHz --num-cpus=1 
```

仿真结果如下：

![image-20230312102110686](https://s2.loli.net/2023/03/12/q9wh1EPOkQxyCsi.png)

### 2.2 如何得到其他数量的皇后对应的解？如何得到指定皇后数对应的解的总个数？

查看`labs/lab1/queens.c`源代码：

```cpp
char *usage =
        "Usage:  %s [-ac] n\n\
\tn\tNumber of queens (rows and columns). An integer from 1 to 100.\n\
\t-a\tFind and print all solutions.\n\
\t-c\tCount all solutions, but do not print them.\n";
```

可以通过`-a`参数指定输出问题的所有解、`-c`参数指定输出问题的解的个数，仿真时可以使用`--options`选项将命令行参数传给二进制文件，若想得到其他数量的皇后（如4个）对应的解：

```bash
$ build/ARM/gem5.debug configs/example/se.py -c labs/lab1/queens --cpu-type=MinorCPU --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l1d_assoc=4 --l1i_assoc=4 --l2_size=1MB --l2_assoc=8 --cacheline_size=64  --cpu-clock=1GHz --num-cpus=1  --options="-a 4"
```

![image-20230312104226795](https://s2.loli.net/2023/03/12/j5JExGXwgoVNe2p.png)

若想得到指定皇后数对应的解的总个数：

```bash
$ build/ARM/gem5.debug configs/example/se.py -c labs/lab1/queens --cpu-type=MinorCPU --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l1d_assoc=4 --l1i_assoc=4 --l2_size=1MB --l2_assoc=8 --cacheline_size=64  --cpu-clock=1GHz --num-cpus=1  --options="-c 4"
```

![image-20230312104335378](https://s2.loli.net/2023/03/12/vZ2iOA8n3MDLmRs.png)

### 2.3 交叉编译的目的

为了能够为运行编译器的平台以外的平台创建可执行代码。如在x86平台上使用`aarch64-linux-gnu-gcc`交叉编译器生成可以在arm平台上运行的可执行文件。

### 2.4 (选做)自行搭建交叉编译器

编译指定架构的处理器：

```bash
$ scons build/MIPS/gem5.debug -j 4
```

安装ARM指令集交叉编译器`mips-linux-gnu-gcc`：

```bash
$ sudo apt-get install emdebian-archive-keyring
$ sudo apt-get install linux-libc-dev-mips-cross libc6-mips-cross libc6-dev-mips-cross binutils-mips-linux-gnu gcc-mips-linux-gnu g++-mips-linux-gnu
```

对八皇后程序`queens.c`进行交叉编译：

```bash
$ mipsel-linux-gnu-gcc -o labs/lab1/queens_mips labs/lab1/queens.c --static
```

> 注意这里必须使用`mipsel-linux-gnu-gcc`而不是`mips-linux-gnu-gcc`，这是由于如果用后者会出现以下错误（gem5仅支持小端法的MIPS）：
> ![image-20230313160803870.png](https://s2.loli.net/2023/03/13/crZ8buVi9E2x5qn.png)

根据所提供的参数，在MIPS架构上对可执行程序进行仿真：

```bash
$ build/MIPS/gem5.debug configs/example/se.py -c labs/lab1/queens_mips --cpu-type=TimingSimpleCPU --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l1d_assoc=4 --l1i_assoc=4 --l2_size=1MB --l2_assoc=8 --cacheline_size=64  --cpu-clock=1GHz --num-cpus=1 
```

![image-20230313160803870.png](https://s2.loli.net/2023/03/13/crZ8buVi9E2x5qn.png)

这里并没有运行成功，是由于gem5对于MIPS架构的支持不够全面，并不支持prefench这一操作。

## 3 流水线处理器的性能分析

### 3.1 Minor流水线结构

输出trace文件：

```bash
$ build/ARM/gem5.debug --debug-flags=MinorTrace --debug-start=10000 --debug-file=trace.out configs/example/se.py -c labs/lab1/queens --cpu-type=MinorCPU --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l1d_assoc=4 --l1i_assoc=4 --l2_size=1MB --l2_assoc=8 --cacheline_size=64  --sys-clock=1GHz --num-cpus=1 
```

运行可视化：

```bash
$ python2 ./util/minorview.py ./m5out/trace.out
```

运行截图如下：

![image-20230312144137820](https://s2.loli.net/2023/03/12/V1qhgbstSU5zrN3.png)

#### a)

可见，Minor处理器由4级流水线构成，每一级的名称和作用如下：

| stage number | 名称    | 功能                                                        |
| ------------ | ------- | ----------------------------------------------------------- |
| 1            | Fench1  | instruction fetch unit responsible for fetching cache lines |
| 2            | Fench2  | line to instruction decomposition                           |
| 3            | Decode  | instruction to micro-op decomposition                       |
| 4            | Execute | instruction execution and data memory interface             |

#### b)

Execute stage包括以下模块：

* scoreboard: 控制指令的发布。它包含一个flight instructions的数量，这些指令将写入每个通用CPU的整数或浮点寄存器。只有当记分牌上的指令数为0时，指令才会被发出，这些指令将写入一个指令源寄存器。一旦指令被发出，指令的每个目标寄存器的记分牌计数将被递增。
* inFlightInsts: 作为存储in flight instructions的队列，并遵循正确的执行顺序，先进先出。
* inputBuffer: 作为指令的缓冲区，等待提交执行。
* Int,Mul,Div...: 这些模块可能类似于经典MIPS五级流水线中的ALU，执行整型（Intx2、Mul、Div），浮点型（Float）运算，还包括一个Misc单元（Minimal instruction set computer）和一个Mem单元。
* Commit: 提交`inFlightInsts`中的指令。

#### c)

* forward FIFO: 寄存上一级的数据，显示了当前tick时被推入的数据（在左边），传输中的数据，以及其输出的可用数据（在右边）。
* backward FIFO: （Fetch2和Fetch1之间）显示分支预测的信息，Fench2预测的信息回传给Fench1中的指令在遇到分支时发挥决策。

### 3.2 处理器频率——性能特性

运行脚本如下：

```bash
# Program:
#     CPU frequency simulation
Target=Frequency_out       
Unit=MHz
Root=/home/nullptr/gem5-stable/labs/lab1/"$Target"     # Target dictionary for stats.txt
Gem5=/home/nullptr/gem5-stable/build/ARM/gem5.debug # gem5 dictionary
Script=/home/nullptr/gem5-stable/configs/example/se.py  # config script dictionary 
Bench=/home/nullptr/gem5-stable/labs/lab1/queens                            # benchmark name
i=1000 # frequency: MHz
CPUtype=TimingSimpleCPU

mkdir $Root
while [ $i != 3000 ]                    # Max frequency: 3GHz = 3000MHz
do
    # Add your code here for gem5 simulation
    # You can use the following command to run gem5 simulation
    echo $i
    $Gem5 $Script -c $Bench --cpu-type=$CPUtype --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l2_size=1MB --l1d_assoc=4 --l1i_assoc=4 --l2_assoc=8 --cacheline_size=64 --sys-clock=${i}${Unit} --num-cpus=1 --output=$Root/${i}_${CPUtype}
    # add i for 100
    i=$(($i+100))
done;
echo "finished"
exit 0
```

运行仿真：

```bash
$ ./sim_freq.sh | tee timing.log       
$ ./sim_freq.sh | tee minor.log
```

结果如下：

![image-20230312165704656](https://s2.loli.net/2023/03/12/u1RQWz9Sf2U5GC6.png)

#### (a)

可以看出，随着频率增长，处理器运行时间变短，且变化速率越来越慢（呈非线性关系）。从1GHz->3GHz，timing CPU提升了约2.08倍，minor CPU提升了约1.86倍。推测原因如下：

CPU的时钟周期中，90%的时间都在访存。CPU的访存瓶颈来源于CPU时钟速度与内存时钟速度无法匹配，换句话说，内存延迟是造成CPU访存瓶颈的罪魁祸首。提升CPU频率之后，由于cache、memory的访存速度没有明显提升，无法与CPU匹配，导致速度被拖慢（设想以下场景：CPU只是频率变高，但执行代码的逻辑并没有发生变化，对于cache、memory的访问逻辑也没有发生改变。这个时候，假如发生了cache未被命中的情况，从memory中提取数据的时间仍然没有缩短，且占据了大量时间，大大抵消了增加主频带来的好处）。

#### (b)

timing CPU上升更多。这是因为minor CPU是4周期流水线CPU，而timing CPU在每个周期中计算一个指令，只在访存阶段需要等待多个周期读取内存——也就是说，timing CPU在缓存访问时停滞不前，等待内存系统的响应，然后再继续进行。这个时候内存时钟对速度的影响没有那么明显，CPU时钟也成为影响访存速度的一个重要因素。
### 3.3 处理器频率——性能特性

观察八皇后问题的源程序，发现解决八皇后问题中涉及到的计算均为整数计算，所以推测应该修改整数运算单元。`stats.txt`的统计信息也证实了这一点：
![image.png](https://s2.loli.net/2023/03/20/smqoMgIzL5A8xRZ.png)

对以下参数进行修改：

**version 1**：仅修改整数算子

```diff
class MinorDefaultIntFU(MinorFU):
    opClasses = minorMakeOpClassSet(['IntAlu'])
    timings = [MinorFUTiming(description="Int",
        srcRegsRelativeLats=[2])]
-    opLat = 3
+    opLat = 1

class MinorDefaultIntMulFU(MinorFU):
    opClasses = minorMakeOpClassSet(['IntMult'])
    timings = [MinorFUTiming(description='Mul',
        srcRegsRelativeLats=[0])]
-    opLat = 3
+    opLat = 1

class MinorDefaultIntDivFU(MinorFU):
    opClasses = minorMakeOpClassSet(['IntDiv'])
    issueLat = 9
-    opLat = 9
+    opLat = 4
```

**version 2**：仅修改浮点算子

```diff
class MinorDefaultFloatSimdFU(MinorFU):
    opClasses = minorMakeOpClassSet([
        'FloatAdd', 'FloatCmp', 'FloatCvt', 'FloatMisc', 'FloatMult',
        'FloatMultAcc', 'FloatDiv', 'FloatSqrt',
        'SimdAdd', 'SimdAddAcc', 'SimdAlu', 'SimdCmp', 'SimdCvt',
        'SimdMisc', 'SimdMult', 'SimdMultAcc', 'SimdShift', 'SimdShiftAcc',
        'SimdDiv', 'SimdSqrt', 'SimdFloatAdd', 'SimdFloatAlu', 'SimdFloatCmp',
        'SimdFloatCvt', 'SimdFloatDiv', 'SimdFloatMisc', 'SimdFloatMult',
        'SimdFloatMultAcc', 'SimdFloatSqrt', 'SimdReduceAdd', 'SimdReduceAlu',
        'SimdReduceCmp', 'SimdFloatReduceAdd', 'SimdFloatReduceCmp',
        'SimdAes', 'SimdAesMix',
        'SimdSha1Hash', 'SimdSha1Hash2', 'SimdSha256Hash',
        'SimdSha256Hash2', 'SimdShaSigma2', 'SimdShaSigma3'])

    timings = [MinorFUTiming(description='FloatSimd',
        srcRegsRelativeLats=[2])]
-    opLat = 6
+    opLat = 3
```

优化后的结果如下：

![image-20230312174244718](https://s2.loli.net/2023/03/12/A84mkpKy9oesWQH.png)

可以看出，只有修改整数运算单元时，才能提升计算速度。这与之前的推测相一致。

## 参考文献

[^1]: https://www.gem5.org/documentation/learning_gem5/part1/example_configs/
[^2]: https://www.gem5.org/documentation/general_docs/cpu_models/minor_cpu
[^3]: https://askubuntu.com/questions/1059570/unmet-dependencies-that-i-actually-have
[^4]: https://www.gem5.org/documentation/general_docs/cpu_models/minor_cpu
[^5]: http://old.gem5.org/Visualization.html
[^6]: https://blog.csdn.net/leionway/article/details/90479487
[^7]: https://blog.csdn.net/wfxzf/article/details/88974144
[^8]: https://cirosantilli.com/linux-kernel-module-cheat/#gem5-cpu-types