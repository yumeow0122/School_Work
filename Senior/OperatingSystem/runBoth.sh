#!/bin/bash

rm -rf source/out*
rm ./main
docker run -it --rm -v $PWD:/tmp -w /tmp gcc /bin/bash -c "gcc -lpthread -o main source.c; ./main"
rm -rf source/out*

rm -rf thread/out*
rm ./main
docker run -it --rm -v $PWD:/tmp -w /tmp gcc /bin/bash -c "gcc -lpthread -o main thread.c; ./main"
rm -rf thread/out*
