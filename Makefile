C = g++
INCLS = -I./glm -I/usr/include/boost/
OUTNAME = raytrace
FILES = ./src/*.cpp
STDOPTS = -O3 -fopenmp

build: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} ${STDOPTS}

fast: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} ${STDOPTS} -ffast-math

gprof: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} ${STDOPTS} -pg

gproffast: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} ${STDOPTS} -ffast-math -pg

debug: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -ggdb -fopenmp

clean:
	rm -rf ${OUTNAME} ${OUTNAME}.dSYM
