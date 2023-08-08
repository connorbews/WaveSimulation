mkdir lib
mkdir obj
cd lib

git clone https://github.com/glfw/glfw.git
cd glfw
cmake .
make
sudo make install

cd ..
sudo rm -r glfw
mv /usr/local/lib/libglfw3.a libglfw.a