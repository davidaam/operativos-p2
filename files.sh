#! /bin/bash
for i in $(seq 1 10)
do
	rm -rf "$i"
	mkdir "$i"
	for j in $(seq 1 10)
	do
		echo "texto $i $j" > "$i/$i $j"
	done
done