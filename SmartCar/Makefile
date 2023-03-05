test: testImage.o image-v0.1.2.o
	g++  -g -o test testImage.o image-v0.1.2.o -Xlinker -zmuldefs
testImage.o: testImage.cpp para.h image-v0.1.2.h
	g++ -g -c testImage.cpp -Xlinker -zmuldefs
image-v0.1.2.o: image-v0.1.2.c image-v0.1.2.h para.h
	g++ -g -c image-v0.1.2.c -Xlinker -zmuldefs