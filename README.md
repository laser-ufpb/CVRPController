# CVRPController
> CVRPController is used to run and calculate score for the 12th DIMACS Implementation Challenge: CVRP track. Specified in [Competition Rules](http://dimacs.rutgers.edu/files/1815/9845/6740/CVRP_Competition_Rules.pdf).

## Installation

Linux, MacOS & OpenBSD:

```sh
git clone https://github.com/laser-ufpb/CVRPController
cd CVRPController
mkdir build
cd build
cmake ..
make
```

Testing controller with a dummy solver:

```sh
make test
```

The output will be at `build/` as `DIMACS-CVRP-Dummy-X-n101-k25.out`.

## Usage example

### How to run:
```sh
./CVRPController <Competitor ID> <Instance path> <Is rounded [0/1]> <CPU mark> <Time limit> <Instance BKS> <If BKS is optimal [0/1]> <Path to solver>
```

### Example:
```sh
./CVRPController Wolverine X-n120-k6.vrp 1 2064 1800 13332 1 Solver1
```

### How to run all instances:
Generating the script:
```sh
sh genScript.sh <Competitor ID> <CPU mark> <Solver path> > CVRP-Script1.sh
```
Running all instances:
```sh
sh CVRP-Script1.sh
```

### Example:
```sh
sh genScript.sh Wolverine 2064 Solver1 > CVRP-Script1.sh
sh CVRP-Script1.sh
```
_For more examples and usage, please refer to the [Competition Rules](http://dimacs.rutgers.edu/files/1815/9845/6740/CVRP_Competition_Rules.pdf)._

## Meta

Bruno Passeti – bruno@bravadus.com.br (UFPB)

Rodrigo Ramalho – rodrigo@bravadus.com.br (UFPB)

Distributed under the MIT license. See ``LICENSE`` for more information.
