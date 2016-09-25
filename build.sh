#!/bin/bash

CONFIGURATION="debug"
OPERATING_SYSTEM=""
RUN_AFTER_BUILD="false"

if [ "$SYSTEMROOT" == "C:\\Windows" ]; then
	PLATFORM="WINDOWS"
else
	PLATFORM="LINUX"
fi

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
 -r|--run            Run 'insolence_samples'

 -w|--webgl          Build for WebGL
 -R|--release        Release build
 -h|--help           Display this help text
"
}

# $1    Directory to enter, make, and exit out of.
function do_make()
{
	pushd . > /dev/null 2>&1

	cd $1
	echo "Building for $CONFIGURATION"

	if [[ $PLATFORM == "LINUX" ]]; then
		make config="$CONFIGURATION" -j
	elif [[ $PLATFORM == "WEBGL" ]]; then
		emmake make config="$CONFIGURATION" -j
	fi

	if [ $? -ne 0 ]; then
		exit
	fi

	popd > /dev/null 2>&1
}

function linux_configure
{
	premake4 --file=insolence_projects.lua gmake

	if [ $? -ne 0 ]; then
		exit;
	fi

	do_make src
	do_make samples

	cd bin
	export LD_LIBRARY_PATH=.
	ln -sf ../src/insolence/assets > /dev/null 2>&1
	ln -sf ../src/insolence/shaders > /dev/null 2>&1
	cd -
}

function windows_configure
{
	premake4 --file=insolence_projects.lua vs2012

	sed -i "s/v110/v140/g" src/insolence.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" src/insolence.vcxproj
	sed -i "s/v110/v140/g" samples/insolence_samples.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" samples/insolence_samples.vcxproj
	sed -ie "5,7d" insolence_samples.sln

	scripts/winbuild.bat
}

function webgl_configure
{
	premake4 --file=insolence_projects.lua gmake

	do_make src
	do_make samples

	cd bin

	ln -s insolence_samples insolence_samples.bc /dev/null 2&>1

	local webgl_command="\
emcc \
libinsolence.bc insolence_samples.bc \
--preload-file shaders --preload-file assets --memory-init-file 1 ../lib/libassimp.so \
-s USE_SDL=2 -s USE_FREETYPE=1 -s TOTAL_MEMORY=32777216 \
-o insolence_samples.js"

	if [ "$CONFIGURATION" == "webgl-release" ]; then
		webgl_command="${webgl_command} -O3"
	fi

	$webgl_command

	if [ $? -ne 0 ]; then
		exit
	fi

	cp "../templates/insolence.html" "insolence_samples.html"
	cp "../templates/insolence.css" .
	cp "../templates/insolence.svg" .

	local IFS=''
	local i=0
	while read -r line; do
		i=$((i + 1))
		if [ $i -eq 1 ]; then
			echo "var project_name = \"insolence_samples\"" > "insolence.js"
			continue;
		fi
		echo  "$line" >> "insolence.js"
	done < "../templates/insolence.js"
}

function android_configure
{
	type premake5 > /dev/null 2>&1

	if [ $? -ne 0 ]; then
		echo "premake5 is required for Android builds."
		exit
	fi

	if [ ! -d "android/res/drawable-hdpi" ]; then
		echo $("\
android/res/drawable* directories missing. Add the following directories
containing the file 'ic_launcher.png':
 * drawable-hdpi
 * drawable-mdpi
 * drawable-xhdpi
 * drawable-xxhdpi")
		exit
	fi

	if [ ! -d "android/src/org" ]; then
		echo $("\
android/src/org/* SDL files missing. Download the SDL source and link
{SDL_SOURCE}/android-project/src/org")
		exit
	fi

	if [ ! -d "android/jni/SDL2" ]; then
		echo $("\
android/jni/SDL2/* missing. Download the SDL source and link
{SDL_SOURCE}/android-project/jni/SDL2")
		exit
	fi

	premake5 --file=insolence_android.lua androidmk
	cd android

	mkdir jni/include > /dev/null 2>&1
	ln -sf ../../../src/insolence jni/include/

	mkdir assets > /dev/null 2>&1
	ln -sf ../../src/insolence/assets assets/
	ln -sf ../../src/insolence/shaders assets/


	sed -i "s/\$(LOCAL_PATH)\/\/usr\//\/usr\//g" jni/include/insolence/insolence.mk
	sed -i "s/\$(LOCAL_PATH)\/\/usr\//\/usr\//g" jni/src/main.mk

	ndk-build PM5_CONFIG=$CONFIGURATION

	if [ $? -ne 0 ]; then
		exit
	fi

	cd ..
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
			;;
		a) #--android
			if [ "$PLATFORM" == "WINDOWS" ]; then
				echo "Android build is unsupported for Windows."
				exit
			fi
			PLATFORM="ANDROID"
			;;
		w) #--webgl
			if [ "$PLATFORM" == "WINDOWS" ]; then
				echo "WebGL build is currently unsupported on Windows."
				exit
			fi
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

if [[ "$PLATFORM" == "WEBGL" ]]; then
	if [[ "$CONFIGURATION" == "debug" ]]; then
		CONFIGURATION="webgl-debug"
	elif [[ "$CONFIGURATION" == "release" ]]; then
		CONFIGURATION="webgl-release"
	fi
fi

case "$PLATFORM" in
	"WINDOWS")
		windows_configure
		;;
	"LINUX")
		linux_configure
		;;
	"WEBGL")
		webgl_configure
		;;
	"ANDROID")
		android_configure
		;;
esac

if [ "$RUN_AFTER_BUILD" != "true" ]; then
	exit
fi

case "$PLATFORM" in
	"WINDOWS" | "LINUX")
		cd bin
		./insolence_samples
		;;
	"WEBGL")
		cd bin
		xdg-open insolence_samples.html
		;;
	"ANDROID")
		cd android
		ant ${CONFIGURATION} install
		exit
		;;
esac
