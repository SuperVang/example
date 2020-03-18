# precompile
sudo apt-get install autoconf automake libtool curl make g++ unzip

# install from apt
sudo apt install protobuf-compiler libprotobuf-dev

# To get the source, download one of the release .tar.gz or .zip packages in the release page:
https://github.com/protocolbuffers/protobuf/releases

# or clone the source code
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf
git submodule update --init --recursive
./autogen.sh

# compile
./configure
make
make check
sudo make install
sudo ldconfig