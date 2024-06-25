CRT_DIR=$(pwd)
set -e

PACKAGES_DIR=$CRT_DIR"/third_party_packages"
GF_INSTALL_DIR=$CRT_DIR"/third_party/gf-complete"
GF_DIR=$PACKAGES_DIR"/gf-complete"
JERASURE_INSTALL_DIR=$CRT_DIR"/third_party/jerasure"
JERASURE_DIR=$PACKAGES_DIR"/jerasure"

# gf-complete
mkdir -p $GF_INSTALL_DIR
cd $GF_INSTALL_DIR
rm * -rf
cd $PACKAGES_DIR
rm -rf gf-complete
tar -xvzf gf-complete.tar.gz
cd $GF_DIR
autoreconf -if
./configure --prefix=$GF_INSTALL_DIR
make -j6
make install

# jerasure
mkdir -p $JERASURE_INSTALL_DIR
cd $JERASURE_INSTALL_DIR
rm * -rf
cd $PACKAGES_DIR
rm -rf jerasure
tar -xvzf jerasure.tar.gz
cd $JERASURE_DIR
autoreconf -if
./configure --prefix=$JERASURE_INSTALL_DIR LDFLAGS=-L$GF_INSTALL_DIR/lib CPPFLAGS=-I$GF_INSTALL_DIR/include
make -j6
make install

cd $PACKAGES_DIR
rm -rf gf-complete
rm -rf jerasure