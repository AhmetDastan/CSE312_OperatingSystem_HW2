All: compile clean

compile: main.cpp CdRom.cpp FileManager.cpp
	@g++ main.cpp CdRom.cpp FileManager.cpp -o exe -lm

run:
	@./exe

clean: 
	@rm -f *.o