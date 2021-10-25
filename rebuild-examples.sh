#!/bin/sh

(cd docs && \
     for cur_file in tut??.cpp; do
         executable=${cur_file/.cpp/}
         echo "Compiling and running ${executable}"
         c++ -o $executable -I../include $cur_file && ./${executable} && rm -f $executable
     done)

