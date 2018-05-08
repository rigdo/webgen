#!/bin/sh

export LD_LIBRARY_PATH=/usr/local/lib

export WEBGUI_DUMPVARS=bin/dumpvars.sh
export WEBGUI_SAVEVAR=bin/savevar.sh
export WEBGUI_SETTNGSDIR="settings"

export PATH=${PATH}:bin

#valgrind --trace-children=yes ./webgui --docroot . --http-address 0.0.0.0 --http-port 8081
#exec valgrind  ./webgui --docroot . --http-address 0.0.0.0 --http-port 8081
#strace -f ./webgui --docroot . --http-address 0.0.0.0 --http-port 8081
#echo '--docroot . --http-address 0.0.0.0 --http-port 8081'
#gdb ./webgui
ulimit -s 1024
#export WT_TMP_DIR=/home/bond
exec ./localgui --docroot . --http-address 0.0.0.0 --http-port 8081
#echo "--docroot . --http-address 0.0.0.0 --http-port 8081"
#exec gdb ./webgui
