All: compile clean

compile: main.cpp CdRom.cpp
	@g++ main.cpp CdRom.cpp -o exe -lm

run:
	@./exe

clean: 
	@rm -f