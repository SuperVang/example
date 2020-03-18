# compile from github
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
sudo make install

# compile from apt
sudo apt install libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
