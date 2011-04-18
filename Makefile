C = g++
INCLS = -I./glm -I/usr/include/boost
OUTNAME = raytrace
FILES = ./*.cpp

build: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -O3

gprof: ${FILES}
	${C} ${FILES} ${INCLS} -o ${OUTNAME} -O3 -pg

clean:
	rm ${OUTNAME}
