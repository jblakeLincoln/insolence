#!/bin/bash

premake4 gmake
RETVAL=$?

if [ $RETVAL -ne 0 ]; then
	exit;
fi

make config=release
RETVAL=$?

if [ $RETVAL -eq 0 ]; then
	cd bin
	ln -sf ../src/insolence/assets
	ln -sf ../src/insolence/shaders
	export LD_LIBRARY_PATH=.
	./insolence_samples
fi
