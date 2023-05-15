for i in 8 16 256 512; do
    build/X86/gem5.debug configs/hw2/run.py O3 --l1i_size=64kB --l1d_size=${i}kB
    mv m5out/stats.txt m5out/64_${i}_O3_stats.txt
    build/X86/gem5.debug configs/hw2/run.py MINOR --l1i_size=64kB --l1d_size=${i}kB
    mv m5out/stats.txt m5out/64_${i}_MINOR_stats.txt
done