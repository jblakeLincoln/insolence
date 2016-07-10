#!/bin/bash

IFS=''
PROJECT_NAME=""
LINKED=false
INSOLENCE_DIR="$(pwd)"

function echo_usage {
echo "
Usage:
 create_project.sh [options] <name>

Creates a new project template with files to build the new project. A new
directory named for the first input parameter will be created alongside
the insolence directory.

Options:
 -l                    Creates sys links with the insolence directory so that
                       changes to insolence are immediately fed in to the
                       project.
 -h|--help             Display this help text.
"
}

BUILD_USAGE=$(echo "Builds a game project linking insolence.\n\
Requires the insolence shared library files to be manually copied.\n")

#
# Creates the new game directory, creates directories, and moves in
# bash scripts and the template main.cpp.
#
function create_game_directory
{
	mkdir -p "$DESTINATION_DIR"
	mkdir -p "$DESTINATION_DIR/bin"
	mkdir -p "$DESTINATION_DIR/include"
	mkdir -p "$DESTINATION_DIR/scripts"
	mkdir -p "$DESTINATION_DIR/src"

	cp "$INSOLENCE_DIR/scripts/winbuild.bat" "$DESTINATION_DIR/scripts"
	cp "$INSOLENCE_DIR/scripts/template_main.cpp" "$DESTINATION_DIR/src/main.cpp"
	cp "$INSOLENCE_DIR/build.sh" "$DESTINATION_DIR"

	# Set up build file for using the name of the new project.
	sed -i '/ln -sf/d' "$DESTINATION_DIR/build.sh"
	sed -i '/src\/insolence/d' "$DESTINATION_DIR/build.sh"
	sed -i '/do_linux_make samples/d' "$DESTINATION_DIR/build.sh"
	sed -i 's/insolence_samples/'$PROJECT_NAME'/g' "$DESTINATION_DIR/build.sh"
	sed -i 's/samples\/insolence_samples/src\/'$PROJECT_NAME'/g' "$DESTINATION_DIR/build.sh"

	# Put a new usage message in the new build file.
	BUILD_LINE_BEGIN=$(awk '/build.sh/{print NR+1}' build.sh)
	BUILD_LINE_END=$(awk '/Options:/{print NR-2}' build.sh)
	sed -i "${BUILD_LINE_BEGIN}, ${BUILD_LINE_END}d" $DESTINATION_DIR/build.sh
	sed -i "${BUILD_LINE_BEGIN}a ${BUILD_USAGE}" $DESTINATION_DIR/build.sh

	# If we're linking with the insolence directory, include the shared object.
	if [[ "$LINKED" == "true" ]]; then
		ln -sf "$INSOLENCE_DIR/bin/libinsolence.so" "$DESTINATION_DIR/bin"
		ln -sf "$INSOLENCE_DIR/src/insolence" "$DESTINATION_DIR/include"
		ln -sf -t "$DESTINATION_DIR/bin" "../../insolence/src/insolence/shaders"
	else
		cp "$INSOLENCE_DIR/src/insolence/shaders" "$DESTINATION_DIR/bin" -r
		rsync -r --include "*/" --include "*.h" --exclude="*" \
--prune-empty-dirs src/ "$DESTINATION_DIR/include/"
	fi
}

#
# Creates a new premake.lua file for the new project.
#
function create_premake_solution_cfg
{
echo \
'solution "'${PROJECT_NAME}'"
	language "C++"
    objdir "obj"
	targetdir "bin"
	configurations { "Debug", "Release" }

	includedirs {
		"include",
	}
	libdirs { "bin" }
	links { "insolence"}

	dofile("./src/game.lua")
' > "$DESTINATION_DIR/premake4.lua"
}

#
# Creates a new game.lua file for the new project.
#
function create_premake_game_cfg
{
	cd "$DESTINATION_DIR"

echo \
'project "'${PROJECT_NAME}'"
	kind "ConsoleApp"
	files { "**.h", "**.cpp" }
' > "src/game.lua"

	while read line; do
		if [[ ! -z $line ]]; then
			line="\t$line"
		fi

		echo -e "$line" >> "src/game.lua"
	done < "$INSOLENCE_DIR/settings.lua"

	cd $INSOLENCE_DIR
}

###############
###############
###############

for arg in "$@"; do
	shift
	case "$arg" in
		"--linked")        set -- "$@" "-l" ;;
		"--help")          set -- "$@" "-h" ;;
		*)                 set -- "$@" "$arg"
	esac
done

while getopts "hln:" opt; do
	case "$opt" in
		l)
			LINKED="true"
			;;
		h)
			echo_usage
			exit
			;;
		*)
			echo_usage
			exit
			;;
	esac
done
shift $((OPTIND-1))

PROJECT_NAME="$1"
DESTINATION_DIR="../$PROJECT_NAME"

if [ -z "$PROJECT_NAME" ]; then
	echo_usage
	exit
fi

create_game_directory
create_premake_solution_cfg
create_premake_game_cfg
