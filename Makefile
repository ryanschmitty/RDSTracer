C = g++
INCLS = -I./glm -I/usr/include/boost
OUTNAME = raytrace
FILES = ./src/*.cpp

build: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -O3

fast: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -O3 -ffast-math

gprof: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -O3 -pg

gproffast: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -O3 -ffast-math -pg

debug: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -ggdb

clean:
	rm ${OUTNAME}
