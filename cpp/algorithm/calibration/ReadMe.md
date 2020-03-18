
# tasi calibration



# navy calibration
note: https://blog.csdn.net/u010368556/article/details/81585422
paper: Robot sensor calibration: solving AX=XB on the Euclidean group

# compile newest version eigen
git clone https://github.com/eigenteam/eigen-git-mirror.git
cd eigen-git-mirror && mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DEIGEN_TEST_CXX11=ON ..
sudo make install

# compile newest version sophus
git clone https://github.com/strasdat/Sophus.git
cd Sophus && mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/ -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF ..
sudo make install

# my notes
手眼标定问题：AX=XB，在机器人中A用于描述机械臂的相对于机械臂自身的运动变化，B描述机械臂上所安装的传感器在同一时刻内相对于传感器自身的运动变化，X描述了传感器相对于机械臂的刚体变换，即P(A)=X*P(B)。