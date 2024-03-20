order=40
avg=0
min=0
max=0
thr=0

for ((j = 0; j < 3; j++)); do
  result=$(./client ${1} ${2} ${4} ${order} ${3} 2>&1 1> t.txt)
  echo $result
  arr=(${result// / })
  avg=$(echo "$avg + ${arr[0]}" | bc)
  min=$(echo "$min + ${arr[1]}" | bc)
  max=$(echo "$max + ${arr[2]}" | bc)
  thr=$(echo "$thr + ${arr[3]}" | bc)
done
echo $(echo "scale=2; $avg / 3" | bc) $(echo "scale=2; $min / 3" | bc) $(echo "scale=2; $max / 3" | bc) $(echo "scale=2; $thr / 3" | bc)

