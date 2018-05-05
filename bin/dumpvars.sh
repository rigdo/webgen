#!/bin/sh

if [ "$2" = "" ]
then
	echo "usage dumpvars.sh settingsdir service"
	exit 1
fi
#unset LS_COLORS

settingsdir=$1

if [ ! -f ${settingsdir}/$2 ]
then
	exit 1
fi

. ${settingsdir}/$2
set 

