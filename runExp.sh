#!/bin/bash

ulimit -s unlimited

echo "#!/bin/bash"
echo
echo "ulimit -s unlimited"
echo

for((i=1;i<=10;i++))
do
	for intance in instances/*.exam
	do
		out=${intance/instances\/}
		out=${out/.exam}
		output=$out"_"$i".log"
		error=$out"_"$i".error"

		echo "./ExaminationTimetabling" $intance "30 500 100 >" $output "2>" $error
	done
done
