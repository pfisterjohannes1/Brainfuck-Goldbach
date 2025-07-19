

gcc -DGEN_SIMPLE=1 -E ./division.c -o division_simple.c

sed '/^#/d' -i division_simple.c
