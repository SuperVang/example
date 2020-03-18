# install mongodb first
sudo apt install mongodb

# release download
libmongoc:https://github.com/mongodb/mongo-c-driver/releases
libmongocxx:https://github.com/mongodb/mongo-cxx-driver/releases

# download from github
git clone https://github.com/mongodb/mongo-c-driver.git
git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1

# precompile for libmongoc
sudo apt-get install cmake libssl-dev libsasl2-dev

# compile libmongoc
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
sudo make install

# compile libmongocxx
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
sudo make install