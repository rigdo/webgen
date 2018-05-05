#!/bin/sh

if [ "$2" = "" ]
then
	echo "usage: savevar.sh settingsdir service varname varvalue"
	exit 1
fi
settingsdir="$1"
service="$2"
name="$3"
value="$4"

#settingsdir=settings

mkdir -p ${settingsdir}
configfile=${settingsdir}/${service}
configfiletmp=`mktemp`

grep -Eq "^ *${name}=" $configfile 2>/dev/null || echo "${name}=" >> $configfile
sed -e "s|^ *${name}=.*|${name}='$value'|" < $configfile > $configfiletmp
mv $configfiletmp $configfile

