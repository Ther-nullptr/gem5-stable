for i in 8 16 32; do
    for j in 32 64 128; do
        build/X86/gem5.debug configs/hw2/run.py O3 --l1i_size=${i}kB --l1d_size=${j}kB
        mv m5out/stats.txt m5out/${i}_${j}_O3_stats.txt
        build/X86/gem5.debug configs/hw2/run.py MINOR --l1i_size=${i}kB --l1d_size=${j}kB
        mv m5out/stats.txt m5out/${i}_${j}_MINOR_stats.txt
    done
done