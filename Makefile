C = g++
INCLS = -I./include -I./glm -I/usr/include/boost/
LIBS = -L./lib -L/System/Library/Frameworks -framework GLUT -framework OpenGL -lglew
OUTNAME = raytrace
FILES = ./src/*.cpp
STDOPTS = -O3 -fopenmp

build: ${FILES}
	${C} ${FILES} ${INCLS} ${LIBS} -o ${OUTNAME} ${STDOPTS}

fast: ${FILES}
	${C} ${FILES} ${INCLS} ${LIBS} -o ${OUTNAME} ${STDOPTS} -ffast-math

gprof: ${FILES}
	${C} ${FILES} ${INCLS} ${LIBS} -o ${OUTNAME} ${STDOPTS} -pg

gproffast: ${FILES}
	${C} ${FILES} ${INCLS} ${LIBS} -o ${OUTNAME} ${STDOPTS} -ffast-math -pg

debug: ${FILES}
	${C} ${FILES} ${INCLS} ${LIBS} -o ${OUTNAME} -ggdb -fopenmp

clean:
	rm -rf ${OUTNAME} ${OUTNAME}.dSYM
