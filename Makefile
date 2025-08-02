
all: ;\
	bash simplify.sh  ;\
  gcc -ggdb -Wall -Wextra -Wpedantic ./goldbach.c -o native ;\
  python3 convert.py goldbach_simple.c ;\

