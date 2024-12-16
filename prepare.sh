echo "Installing Boost library"
wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
tar -xzf boost_1_82_0.tar.gz
cd boost_1_82_0 || (echo "Failed to get boost library" && exit 1)
./bootstrap.sh
sudo ./b2 install
