#!/bin/bash

CONFIGURATION="debug"
OPERATING_SYSTEM=""
RUN_AFTER_BUILD="false"

function echo_usage
{
echo "
Usage:
 build.sh [options]

Builds the insolence framework as a shared library, and a sample project
which can optionally be run. On Linux, this is the de facto build system.
On Windows, this script can be used within a Bash environment
(i.e. Cygwin). Configuring the project via premake is an option anywhere.

Options:
 -c                  Set build configuration to 'release'
 -r|--run            Run 'insolence_samples'
 -h|--help           Display this help text
"
}

# $1    Directory to enter, make, and exit out of.
function do_linux_make()
{
	pushd . > /dev/null 2>&1

	cd $1
	echo "Building for $CONFIGURATION"
	make config="$CONFIGURATION"
	RETVAL=$?

	if [ $RETVAL -ne 0 ]; then
		echo "Exiting $1 with code $RETVAL"
		exit
	fi

	popd > /dev/null 2>&1
}

function linux_configure
{
	premake4 gmake
	RETVAL=$?

	if [ $RETVAL -ne 0 ]; then
		exit;
	fi

	do_linux_make src
	do_linux_make samples

	cd bin
	export LD_LIBRARY_PATH=.
	ln -sf ../src/insolence/assets
	ln -sf ../src/insolence/shaders
	cd -
}

function windows_configure
{
	premake4 vs2012

	sed -i "s/v110/v140/g" src/insolence.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" src/insolence.vcxproj
	sed -i "s/v110/v140/g" samples/insolence_samples.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" samples/insolence_samples.vcxproj
	sed -ie "5,7d" insolence_samples.sln

	scripts/winbuild.bat
}

for arg in "$@"; do
	shift
	case "$arg" in
		"--run")           set -- "$@" "-r" ;;
		"--help")          set -- "$@" "-h" ;;
		*)                 set -- "$@" "$arg"
	esac
done

while getopts "hcre" opt; do
	case "$opt" in
		c)
			CONFIGURATION="release"
			;;
		r)
			RUN_AFTER_BUILD="true"
			;;
		h)
			echo_usage
			exit
			;;
		*)
			;;
	esac
done
shift $((OPTIND-1))

if [ "$SYSTEMROOT" == "C:\\Windows" ]; then
	OPERATING_SYSTEM="windows"
else
	OPERATING_SYSTEM="linux"
fi

case "$OPERATING_SYSTEM" in
	"windows")
		windows_configure
		;;
	"linux")
		linux_configure
		;;
esac

if [ "$RUN_AFTER_BUILD" == "true" ]; then
	cd bin
	./insolence_samples
fi
