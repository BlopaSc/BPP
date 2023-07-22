# BPP, B++ or Blopa++

Is a collection of C++ libraries and classes which can come in handy.

The repository is built and (currently) maintained by Blopa Sc., it comes with no guarantees but feel free to use.

If you want to collaborate with the development of the project, feel free to message me, fork the project, or send a pull request. If you discover a bug: please notify me; if you already patched it: please send a pull request to the repository.

Check out the **documentation**: [BPP documentation](https://blopasc.github.io/BPP/)

## Installation and requirements

### Requirements

The only requirement for this library is that it is written using the C++20, therefore you will require and updated compiler to be able to compile the code. This also means you will need to add the following flag to your compiler:

~~~
-std=c++20
~~~

### Installation

This library doesn't requires any kind of installation, you can include the documents straight into your code.

## Directories and Namespaces

### Namespaces

All libraries belonging to B++ will be contained inside the `bpp` namespace. However to keep order withing the namespace, `bpp` will contain additional namespaces within it to organize the different libraries within it.

### Directories

The main directories are listed below as well as their general purpose. The corresponding nested list will include the namespaces used inside the directory and the corresponding classes within the namespace. (E.g: `TreeAVL` would be located in the `bpp::collections::map` namespace).

- collections: All data structures related classes, iterators and algorithms.
	- `map`: TreeAVL, Trie
	- `sequence`: Combination
	- `serialize`: serialize/deserialize functions for data structures and basic data types
- utility: Utility functions and structures used as part of other modules.
	- `byte_ordering`: host to byte-ordering functions and vice-versa

## Contributors

- [BlopaSc](https://github.com/BlopaSc)
