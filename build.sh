#!/bin/bash

# param $1    Directory to enter, make, and exit out of.
function do_make()
{
	pushd . > /dev/null 2>&1

	cd $1
	make config=debug
	RETVAL=$?

	if [ $RETVAL -ne 0 ]; then
		echo "Exiting $1 with code $RETVAL"
		exit
	fi

	popd > /dev/null 2>&1
}

premake4 gmake
RETVAL=$?

if [ $RETVAL -ne 0 ]; then
	exit;
fi

do_make src
do_make samples

cd bin
ln -sf ../src/insolence/assets
ln -sf ../src/insolence/shaders
export LD_LIBRARY_PATH=.
./insolence_samples
