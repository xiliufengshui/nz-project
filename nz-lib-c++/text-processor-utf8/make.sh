#!/bin/bash

rm -f *.o
rm -f libtext_processor.a

g++ -fPIC -c text_processor.cpp svm.cpp
ar -crsv libtext_processor.a *.o

rm -f *.o

g++ main.cpp libtext_processor.a
