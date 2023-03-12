# Program:
#     CPU frequency simulation
Target=Frequency_out       
Unit=MHz
Root=/home/nullptr/gem5-stable/labs/lab1/"$Target"     # Target dictionary for stats.txt
Gem5=/home/nullptr/gem5-stable/build/ARM/gem5.debug # gem5 dictionary
Script=/home/nullptr/gem5-stable/configs/example/se.py  # config script dictionary 
Bench=/home/nullptr/gem5-stable/labs/lab1/queens                            # benchmark name
i=1000 # frequency: MHz
CPUtype=MinorCPU

mkdir $Root
while [ $i != 3000 ]                    # Max frequency: 3GHz = 3000MHz
do
    # Add your code here for gem5 simulation
    # You can use the following command to run gem5 simulation
    echo $i
    $Gem5 --outdir=m5out/${i}${CPUtype} $Script -c $Bench --cpu-type=$CPUtype --caches --l2cache --l1d_size=8kB --l1i_size=8kB --l2_size=1MB --l1d_assoc=4 --l1i_assoc=4 --l2_assoc=8 --cacheline_size=64 --cpu-clock=${i}${Unit} --num-cpus=1 --output=$Root/${i}_${CPUtype}.txt
    # add i for 100
    i=$(($i+100))
done;
echo "finished"
exit 0