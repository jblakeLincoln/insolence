#!/bin/bash

# $1    Directory to enter, make, and exit out of.
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

function linux_make
{
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
}

function windows_make
{
	premake4 vs2012

	sed -i "s/v110/v140/g" src/insolence.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" src/insolence.vcxproj
	sed -i "s/v110/v140/g" samples/insolence_samples.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" samples/insolence_samples.vcxproj
	sed -ie "5,7d" insolence_samples.sln

	scripts/winbuild.bat
}

if [ "$SYSTEMROOT" == "C:\\Windows" ]; then
	windows_make
else
	linux_make
fi
