# install from apt repository
sudo add-apt-repository ppa:ubuntugis/ppa
sudo apt update
sudo apt install gdal-bin libgdal-dev libproj-dev

# install from source
# remove old proj and gdal which install by apt
sudo apt purge libproj-dev proj-bin proj-data libgdal-dev gdal-bin
# compile proj
git clone https://github.com/OSGeo/PROJ.git
cd PROJ
./configure --prefix=/usr/local
sudo make
sudo make install
# compile gdal
git clone https://github.com/OSGeo/gdal.git
cd gdal
./configure --with-proj=/usr/local
sudo make
sudo make install


