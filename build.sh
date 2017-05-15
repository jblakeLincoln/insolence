#!/bin/bash

CONFIGURATION="debug"
RUN_AFTER_BUILD="false"
SELECTED_SAMPLE=""
DEFAULT_SAMPLE="blank_project"
PLATFORM="LINUX"

function echo_usage
{
echo "
Usage:
 build.sh [options]

Builds the insolence framework as a shared library, and a sample project
which can optionally be run. On Linux, this is the de facto build system.

Options:
 -r|--run            Run a sample project specified by name.
                     Defaults to 'blank_project'.

 -a|--android        Build for Android
 -w|--webgl          Build for WebGL
 -R|--release        Release build
 -h|--help           Display this help text
"
}

function prebuild()
{
	pushd . > /dev/null 2>&1

	mkdir bin > /dev/null 2>&1
	cd bin
	ln -sf ../samples/samples_assets
	ln -sf ../src/insolence/shaders
	cd ..

	if [ ! -z ${SELECTED_SAMPLE} ]; then
		cd samples/${SELECTED_SAMPLE}
		echo "PROJECT_NAME = ${SELECTED_SAMPLE}" > Makefile
		echo "$(cat ../Makefile_template)" >> Makefile
	fi

	popd > /dev/null 2>&1
}

function linux_make()
{
	pushd . > /dev/null 2>&1

	cd src
	make $CONFIGURATION

	if [ $? -ne 0 ]; then
		exit
	fi

	cd ..

	if [ ! -z ${SELECTED_SAMPLE} ]; then
		cd samples/${SELECTED_SAMPLE}
		make $CONFIGURATION

		if [ $? -ne 0 ]; then
			exit
		fi

	fi

	popd > /dev/null 2>&1
}

function webgl_make()
{
	pushd . > /dev/null 2>&1

	cd src
	make webgl-${CONFIGURATION}
	if [ $? -ne 0 ]; then
		exit
	fi
	cd ..

	if [ ! -z ${SELECTED_SAMPLE} ]; then
		cd samples/${SELECTED_SAMPLE}
		make webgl-${CONFIGURATION}

		cd ../../bin
		make webgl-output-${CONFIGURATION} -f ../samples/${SELECTED_SAMPLE}/Makefile
		if [ $? -ne 0 ]; then
			exit
		fi
		cp "../templates/insolence.html" "${SELECTED_SAMPLE}.html"
		cp "../templates/insolence.css" .
		cp "../templates/insolence.svg" .

		# Move in the insolence.js template file, and insert the project name at the top of it.
		echo "var project_name = \"${SELECTED_SAMPLE}\"" > "insolence.js"
		echo "$(cat ../templates/insolence.js)" >> "insolence.js"
	fi

	popd > /dev/null 2>&1
}

function android_make()
{
	pushd . > /dev/null 2>&1

	local sample=${SELECTED_SAMPLE}

	if [ -z $sample ]; then
		sample=${DEFAULT_SAMPLE}
	fi

	cd android/jni

	# We require links to the SDL2 source to build the module for Insolence
	if [ ! -d "SDL2" ]; then
		echo \
"No SDL2 directory found!
android/jni/SDL2 should contain:
	\${SDL_SOURCE}/Android.mk
	\${SDL_SOURCE}/include
	\${SDL_SOURCE}/src

android/jni/src should contain:
	\${SDL_SOURCE}/android-project/src/org"
		exit
	fi

	cd src

	echo "SAMPLE_NAME = ${sample}" > "main.mk"
	echo "$(cat main_template.mk)" >> "main.mk"

	cd ../include
	ln -sf ../../../src/insolence

	cd ../..

	mkdir -p assets
	cd assets
	ln -sf ../../samples/samples_assets
	ln -sf ../../src/insolence/shaders
	cd ..

	ndk-build PM5_CONFIG=${CONFIGURATION}

	popd > /dev/null 2>&1
}

for arg in "$@"; do
	shift
	case "$arg" in
		"--run")           set -- "$@" "-r" ;;
		"--android")       set -- "$@" "-a" ;;
		"--webgl")         set -- "$@" "-w" ;;
		"--release")       set -- "$@" "-R" ;;
		"--help")          set -- "$@" "-h" ;;
		*)                 set -- "$@" "$arg"
	esac
done

while getopts "hcawrRe" opt; do
	case "$opt" in
		r) #--run
			RUN_AFTER_BUILD="true"
			if [[ ! -z "$2" ]]; then
				SELECTED_SAMPLE=${2}
			else
				SELECTED_SAMPLE=${DEFAULT_SAMPLE}
			fi
			;;
		a) #--android
			PLATFORM="ANDROID"
			;;
		w) #--webgl
			PLATFORM="WEBGL"
			;;
		R)
			CONFIGURATION="release"
			;;
		h) #--help
			echo_usage
			exit
			;;
		*)
			;;
	esac
done
shift $((OPTIND-1))

prebuild

case "$PLATFORM" in
	"LINUX")
		linux_make
		;;
	"WEBGL")
		webgl_make
		;;
	"ANDROID")
		android_make
		;;
esac

if [ "$RUN_AFTER_BUILD" != "true" ]; then
	exit
fi

case "$PLATFORM" in
	"LINUX")
		cd bin
		export LD_LIBRARY_PATH=.
		if [ ${CONFIGURATION} == "debug" ]; then
			./${SELECTED_SAMPLE}-debug
		else
			./${SELECTED_SAMPLE}
		fi
		;;
	"WEBGL")
		cd bin
		echo ${SELECTED_SAMPLE}
		if [ ${CONFIGURATION} == "debug" ]; then
			xdg-open ${SELECTED_SAMPLE}-debug.html
		else
			xdg-open ${SELECTED_SAMPLE}.html
		fi
		;;
	"ANDROID")
		cd android
		ant ${CONFIGURATION} install
		exit
		;;
esac
