#!/bin/bash

order=20
for ((i = 1; i <= 256; i*=2)); do
  avg=0
  min=0
  max=0
  thr=0

  for ((j = 0; j < 3; j++)); do
    # order=$((10 * $i))
    # echo "order: $order"
    result=$(./client ${1} ${2} ${i} ${order} ${3} 2>&1 1> t.txt)
    echo $result
    arr=(${result// / })
    avg=$(echo "$avg + ${arr[0]}" | bc)
    min=$(echo "$min + ${arr[1]}" | bc)
    max=$(echo "$max + ${arr[2]}" | bc)
    thr=$(echo "$thr + ${arr[3]}" | bc)
  done
  echo "num_comustor: $i"
  echo $(echo "scale=2; $avg / 3" | bc) $(echo "scale=2; $min / 3" | bc) $(echo "scale=2; $max / 3" | bc) $(echo "scale=2; $thr / 3" | bc)

done
