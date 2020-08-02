all: Controller TestSolver
TestSolver: TestSolver.o
	@g++ TestSolver.o -g -o TestSolver
	@-rm TestSolver.o

Controller: Controller.o
	@g++ Controller.o -g -o Controller
	@-rm Controller.o

%.o: %.c
	@g++ -std=c++17 -g $< -o $@


clean:
	@-rm TestSolver Controller

