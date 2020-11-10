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

## Usage example

### How to run:
```sh
./CVRPController <Competitor ID> <Instance path> <Is rounded [0/1]> <PassMark Value> <Time limit> <Instance BKS> <If BKS is optimal [0/1]> <Path to solver>
```

### Example:
```sh
./CVRPController Wolverine X-n120-k6.vrp 1 2064 1800 13332 1 Solver1
```
_For more examples and usage, please refer to the [Competition Rules](http://dimacs.rutgers.edu/files/1815/9845/6740/CVRP_Competition_Rules.pdf)._

## Meta

Bruno Passeti – bruno@bravadus.com.br

Rodrigo Ramalho – rodrigo@bravadus.com.br

Distributed under the MIT license. See ``LICENSE`` for more information.
