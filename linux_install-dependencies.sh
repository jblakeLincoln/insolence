// This script installs all necessary packages for compilation
// In the case of GLFW, it will download the source, compile it, and delete the download

cd "$( dirname "${BASH_SOURCE[0]}" )"

sudo apt-get --assume-yes install build-essential cmake premake4
sudo apt-get --assume-yes install libglu1-mesa-dev xorg-dev libglew-dev
sudo apt-get --assume-yes install libglm-dev libbullet-dev
sudo apt-get --assume-yes install libdevil-dev libassimp-devv
sudo apt-get --assume-yes install libopenal-dev libalut-dev libvorbis-dev

if [ ! -f "glfw-3.1.zip" ]
then
	wait yes | sudo wget http://sourceforge.net/projects/glfw/files/glfw/3.1/glfw-3.1.zip
fi

if [ ! -e "glfw-3.1" ]
then
	yes | unzip glfw-3.1.zip
fi

cd glfw-3.1/

if [ ! -e "build" ]
then
	mkdir build
	cd build/
	sudo cmake ..
	sudo make
	sudo make install
	cd ..
fi

cd ..
sudo rm glfw-3.1.zip
sudo rm -r glfw-3.1
