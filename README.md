# R-GPM: Relation-based Graph Pattern Mining

This project contains the framework 
used in the paper "Graph Pattern Mining and Learning through User-defined Relations" published in ICDM 2018. The extend version of such work 
is available [here](https://arxiv.org/abs/1809.05241).

## Getting Started


### Prerequisites


Besides g++ compiler (version 5.4), 
we need to install the follow libraries in order to compile R-GPM:

```
libboost-all-dev
libtbb-dev
libgsl-dev
libsparsehash-dev
```

### Installing

Run the follow command in the root directory:

```
makefile
```

## Running the tests

The applications are available in the direction *apps*.
For example, the graph pattern mining application 
which uses percolation as a subgraph relation 
is in the directory *apps/percolation*.
To compile a particular application we need to 
execute the follow command in such application directory: 

```
./compile.sh
```

Then, a executable (binary) 
will be generated and we can 
run the application and get a description 
of its parameters
by executing 
the follow command line:

```
./app -h
```

## License

This project is licensed under the GNU LGPL.

## Acknowledgments

This work was partially supported by
CNPq, CAPES, Fapemig, INCT-Cyber, MasWeb, EuBraBigSea,
Atmosphere and also by the ARO, under the U.S.
Army Research Laboratory award W911NF-09-2-0053. Any
opinions, findings and conclusions or recommendations expressed
in this material are those of the authors and do not
necessarily reflect the views of ARO.
