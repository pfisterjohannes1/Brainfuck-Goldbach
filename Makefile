


all: ;\
	./simplify.sh  ;\
	./compile.sh   ;\
	python3 convert.py goldbach_simple.c ;\

