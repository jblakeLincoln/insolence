#!/bin/bash

CONFIGURATION="debug"
OPERATING_SYSTEM=""
RUN_AFTER_BUILD="false"
SELECTED_SAMPLE=""
DEFAULT_SAMPLE="blank_project"

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
 -r|--run            Run a sample project specified by name.
                     Defaults to 'blank_project'.

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

function do_make_samples
{
	if [[ -z ${SELECTED_SAMPLE} ]]; then
		return
	fi

	create_sample_lua
	premake4 --file=insolence_samples.lua gmake
	do_make "samples/${SELECTED_SAMPLE}"
}

function linux_configure
{
	premake4 --file=insolence.lua gmake

	if [ $? -ne 0 ]; then
		exit;
	fi

	do_make src
	do_make_samples

	cd bin
	export LD_LIBRARY_PATH=.
	ln -sf ../samples/samples_assets > /dev/null 2>&1
	ln -sf ../src/insolence/shaders > /dev/null 2>&1
	cd -
}

function create_sample_lua
{
	for d in samples/*/; do
		sample_name=$(basename $d)
		echo \
'project "'${sample_name}'"
	objdir "../../obj/'${sample_name}'"
	files { "**.h", "**.cpp" }
	dofile("../samples.lua")
' >	"samples/${sample_name}/${sample_name}.lua"

	done
}

function windows_configure
{
	premake4 --file=insolence.lua vs2012

	sed -i "s/v110/v140/g" src/insolence.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" src/insolence.vcxproj
	sed -i "s/v110/v140/g" samples/insolence_samples.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" samples/insolence_samples.vcxproj
	sed -ie "5,7d" insolence_samples.sln

	scripts/winbuild.bat
}

function webgl_configure
{
	premake4 --file=insolence.lua gmake

	do_make src
	do_make_samples

	cd bin

	ln -s ${SELECTED_SAMPLE} ${SELECTED_SAMPLE}.bc /dev/null 2&>1

	local webgl_command="\
emcc \
libinsolence.bc \
--preload-file shaders --memory-init-file 1 \
-s USE_SDL=2 -s USE_FREETYPE=1"

	if [[ -z "$SELECTED_SAMPLE" ]]; then
		webgl_command="${webgl_command} ${SELECTED_SAMPLE}.bc -o ${SELECTED_SAMPLE}.js --preload-file sample_assets"
	fi

	if [ "$CONFIGURATION" == "webgl-release" ]; then
		webgl_command="${webgl_command} -O3 -s TOTAL_MEMORY=65554432"
	else
		webgl_command="${webgl_command} -s ALLOW_MEMORY_GROWTH=1"
	fi

	$webgl_command

	if [ $? -ne 0 ]; then
		exit
	fi

	cp "../templates/insolence.html" "${SELECTED_SAMPLE}.html"
	cp "../templates/insolence.css" .
	cp "../templates/insolence.svg" .

	local IFS=''
	local i=0
	while read -r line; do
		i=$((i + 1))
		if [ $i -eq 1 ]; then
			echo "var project_name = \"${SELECTED_SAMPLE}\"" > "insolence.js"
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
			if [[ ! -z "$2" ]]; then
				SELECTED_SAMPLE=${2}
			else
				SELECTED_SAMPLE=${DEFAULT_SAMPLE}
			fi
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
		./${SELECTED_SAMPLE}
		;;
	"WEBGL")
		cd bin
		xdg-open ${SELECTED_SAMPLE}.html
		;;
	"ANDROID")
		cd android
		ant ${CONFIGURATION} install
		exit
		;;
esac
