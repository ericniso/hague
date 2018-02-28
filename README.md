# Hague

An adjacency-list based De Bruijn graphs implementation


### Build requirements

* C compiler
* make
* zlib
* gengetopt

Tested on Ubuntu 16.04 and Windows 10 using Ubuntu bash


### Build

Download from Github

```
$ git clone git git@github.com:ericniso/hague.git

$ cd hague
```

Download submodules

```
$ git submodule init
$ git submodule update
```

Compile binaries, executable file is in `debug` folder

```
$ make bin
```

Compile debug binaries, executable file is in `debug` folder

```
$ make debug
```

Compile shared library, lib file is in `lib` folder

```
$ make lib
```

Compile all previous targets

```
$ make all
```


### Test

After compiling the source code, we recommend to run automated tests in order to check for compilation success

```
$ make test
```


### Documentation

Docs can be generated using

```
$ make doc
```

This will generated source code documentation both in `html` and `latex` in `docs` folder


### Usage

```
$ hague -f "/path/to/fasta/file" -k "k-mer-length"
```

Input file can be compressed `.gz` or not

Such command will generate the `de Bruijn` graph and output the result in `csv` format, here is an example output:

```
$ hague -f /home/nicolaas/abracadabra.fa.gz -k 5

Source, Target, Label
abra, brac, abrac
brac, raca, braca
raca, acad, racad
acad, cada, acada
cada, adab, cadab
adab, dabr, adabr
dabr, abra, dabra
```

From left to right we can see:
* `Source` is the source node, whose key is a `(k-1)-mer`
* `Target` is the node that the source node is pointing to through and edge
* `Label` is the edge label connecting the previous two nodes, and represents the corresponding `k-mer` string

If you want to redirect the output to a file you can specify a filename using the `-o` option:

```
$ hague -f "/path/to/fasta/file" -k "k-mer-length" -o "/path/to/output/file"
```

There's an additional feature, which is the superstring reconstruction, invoked by adding `-w` option:

```
$ hague -f "/path/to/fasta/file" -k "k-mer-length" -w [-o "/path/to/output/file"]
```

Currently this is only a work in progress, since it only works if the graph is Eulerian (semi-Eulerian)


 ### Authors

Eric Nisoli, Lorenzo Mammana


### License

MIT LICENSE
