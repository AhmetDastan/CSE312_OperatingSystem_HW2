All: compile clean

compile: main.cpp
	@g++ main.cpp -o exe -lm

run:
	@./exe

clean: 
	@rm -f