

gcc -DGEN_SIMPLE=1 -E ./goldbach.c -o goldbach_simple.c

sed '/^#/d' -i goldbach_simple.c
