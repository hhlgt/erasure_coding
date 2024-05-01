CRT_DIR=$(pwd)
set -e
echo $CRT_DIR
cd $CRT_DIR
STORAGE_DIR=$CRT_DIR"/storage"
DATA_DIR=$CRT_DIR"/data"
TOOLS_DIR=$CRT_DIR"/tools"
mkdir -p $STORAGE_DIR
mkdir -p $DATA_DIR
cd $TOOLS_DIR
python generate_file.py 1 1 K

