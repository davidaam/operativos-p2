#! /bin/bash
echo "Dar valor a"
read a
echo "a = $a"

if [ $a -gt 0 ]
then
	echo "$a es positivo"
elif [ $a -lt 0 ]
then
	echo "$a es negativo"
else
	echo "$a es cero"
fi