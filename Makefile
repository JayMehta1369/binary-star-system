binary: binary.o aux.o
	c++ -o binary binary.o aux.o -ltrapfpe -lpgplot -lcpgplot -lX11 -lm 

binary.o: binary.cpp aux.h
	c++ -c binary.cpp

aux.o: aux.cpp aux.h
	c++ -c aux.cpp
