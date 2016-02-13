#!/bin/bash

premake4 gmake
RETVAL=$?

if [ $RETVAL -ne 0 ]; then
	exit;
fi

make config=release
RETVAL=$?

if [ $RETVAL -eq 0 ]; then
	bin/insolence
fi
