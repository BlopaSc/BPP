# BPP, B++ or Blopa++

Is a collection of C++ libraries and classes which can come in handy.

The repository is built and mantained by Blopa SC., it comes with no guarantees but feel free to use.

If you discover a bug, feel free to send me a message with your discovery; and/or if you patched it, please send a pull request to the repository.

The documentation for the contained library can be found [here](https://blopasc.github.io/BPP/). 

## About me

I am just a programmer who likes building data structures and some other clever algorithms in his free time. I am trying to make the library as similar to STL as possible to allow the easy utilization of the library, but sometimes differences may occur.

I know these kind of projects should usually be worked with forks, but I will be commiting to the master branch a lot. This is my repository and I don't expect anyone else to be working on it. However, I will do my best to stick to a single class at a time.

## Installation and requirements

### Requirements

The only requirement for this library is that it is written using the C++20, therefore you will require and updated compiler to be able to compile the code. This also means you will need to add the following flag to your compiler:

~~~
-std=c++20
~~~

### Installation

This library doesn't requires any kind of installation, you can include the documents straight into your code, or if you are using a makefile you can handle compilation through the .h with the .cpp.

## Directories and Namespaces

### Namespaces

All libraries belonging to B++ will be contained inside the `bpp` namespace. However to keep order withing the namespace, `bpp` will contain additional namespaces within it to organize the different libraries within it.

### Directories

The main directories are listed below as well as their general purpose. The corresponding nested list will include the namespaces used inside the directory and the corresponding classes within the namespace. (E.g: `TreeAVL` would be located in the `bpp::map` namespace).

- Collections: In here you will find all data-structures related classes and algorithms.
	- `map`: TreeAVL

