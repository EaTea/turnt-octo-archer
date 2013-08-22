#!/bin/bash

g++ io.cpp -c
g++ classifier.cpp -c
g++ main.cpp io.o classifier.o -o main
