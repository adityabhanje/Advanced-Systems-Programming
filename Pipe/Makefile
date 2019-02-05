exec: \
compile
	./combiner ./mapper ./reducer

compile: \
mapper.c \
reducer.c \
combiner.c
	gcc mapper.c -o mapper
	gcc reducer.c -o reducer
	gcc combiner.c -o combiner
	echo "compiling"
	touch compile
