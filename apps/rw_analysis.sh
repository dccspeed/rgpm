#! /usr/bin/env sh

log=$1

temp1=`mktemp tempXXXX.out`
temp2=`mktemp tempXXXX.out`
temp3=`mktemp tempXXXX.out`
temp4=`mktemp tempXXXX.out`

echo "temp1: $temp1"
echo "temp2: $temp2"
echo "temp3: $temp3"
echo "temp4: $temp4"

#get real supp
patt=`grep "^Pattern Code: " $log | cut -f 3 -d " "` 
suppReal=`grep "^patt: $patt" $log | cut -f 4 -d " "`
snconst=`grep "^SN constant:" $log | cut -f 3 -d " "`

echo "patt $patt supp $suppReal snconst $snconst"
suppPartial=`echo "$suppReal - $snconst" | bc -l`

#patt: 227016548 supp: 1083.000000 
#patt: 3172333349 supp: 184476.000000
#patt: 1284580084 supp: 250348.000000

p="@Pattern estimated frequency all tours: " 
grep "$p" $log | sed "s/$p//g" | awk 'NR % 2' > $temp1

#p2="@Pattern estimated frequency tour with const: "
p2="@Pattern estimated frequency tour: "
grep "$p2" $log | sed "s/$p2//g" > $temp2
#echo "grep \"$p2\" $log | sed \"s/$p2//g\" > $temp2"

p3="@RW Tour number: "
grep "$p3" $log | cut -f 6 -d " " >>  $temp3
ntours=`grep "$p3" $log | wc -l`
ntours_avg=`grep "$p3" $log | cut -f 6 -d " " | paste -sd+ | bc -l`
ntours_avg=`echo "$ntours_avg / $ntours" | bc -l`
echo "toursavg: $ntours_avg"

plot="$log.dat"
echo "#ntour #avgavg #erravg" > $plot

#i=1
#for v in `cat $temp1`
#do
#	echo "v $v i $i"
#	err_avg=0;
#	for k in `head -n $i $temp2`
#	do
#		#echo "k $k"
#		err=`echo "$v - $k" | bc -l`
#		err2=`echo "$err^2" | bc -l`
#		err_avg=`echo "$err_avg + $err2" | bc -l`
#		#echo "err $err $err2"
#	done 
#	if [ $i -ne 1 ];
#	then
#		err_avg=`echo "$err_avg / $(($i - 1))" | bc -l`
#		#echo "err_avg $err_avg $i $(($i - 1))"
#		echo "$(($i - 1)) $v $err_avg" >> $plot 
#	else 
#		echo "$(($i -1)) $v 0" >> $plot
#	fi
#	i=`echo "$i + 1" | bc -l`
#done 


SIZES="10 20 40 80"
TIMES=1000


for v in $SIZES
do 
avg2=0
err_avg2=0
for i in `seq 1 $TIMES`
do
	#echo "size $v time $i" 
	idxbegin=`echo "($i - 1) * $v + 1" | bc -l`
	idxend=`echo "$idxbegin + $v - 1" | bc -l`

	avg=`sed -n ${idxbegin},${idxend}p $temp2 | paste -sd+ | bc -l`	
	#echo "sed -n ${idxbegin},${idxend}p $temp2 | paste -sd+ | bc -l"
	avg=`echo "$avg / $v" | bc -l`
	avg2=`echo "$avg2 + $avg" | bc -l`
	err=`echo "$suppPartial - $avg" | bc -l`
	#err=`echo "$suppReal - $avg" | bc -l`
	err2=`echo "$err^2" | bc -l`
	err_avg2=`echo "$err_avg2 + $err2" | bc -l`
	#err_avg=0
	#for k in `cat $temp4`
	#do
		#echo "k $k"
	#	err=`echo "$avg - $k" | bc -l`
	#	err2=`echo "$err^2" | bc -l`
	#	err_avg=`echo "$err_avg + $err2" | bc -l`
		#echo "err $err $err2"
	#done 
	#err_avg=`echo "$err_avg / $(($v - 1))" | bc -l`
	#err_avg2=`echo "$err_avg2 + $err_avg" | bc -l` 
done
avg2=`echo "$avg2 / $TIMES" | bc -l`
err_avg2=`echo "$err_avg2 / $TIMES" | bc -l`

echo "$v $avg2 $err_avg2" 
echo "$v $avg2 $err_avg2" >> $plot
done

#rm temp*.out

#rm -f sumarize_results_groups.log


#targets=($(sed -n 1,${line_number}p $log | grep -n "##group estimation" | cut -f 1 -d ":"))

#length=${#targets[@]}

#number_of_groups=1
#for ((i = 0; i != length; i++))
#do
#   j=`echo "$i + 1" | bc -l`

#   curr=${targets[i]}
#   next=${targets[j]}
#   if [ -z "${targets[j]}" ]; then
#        next=$line_number
#        #echo empty
#   fi
   #echo "target $i: '${targets[i]}' next target $j: '${targets[j]}'"
#   echo "target $i: '$curr' next target $j: '$next'"


#        keys=`sed -n ${curr},${next}p $log | grep "##GroupKey" | cut -f 2 -d " " | sort | uniq | tr "\n" " "`

#       for key in $keys
#        do

#                query=`echo "$key" | sed "s/\[//g" | sed "s/\]//g"`
#                sed -n ${curr},${next}p $log | grep "##GroupKey: \[${query}\]" | cut -f 5,7 -d " " | awk '{print NR, $0}' > group${number_of_groups}_${log}
#                echo "sed -n ${curr},${next}p $log | grep \"\#\#GroupKey: \[$query\]\" "
#
#                number_of_groups=`echo "$number_of_groups + 1" | bc -l`
#        done

#        #break

#done



