#!/bin/bash

# this is a test file to test the mdzip and mdlist and mdunzip programs

RANDOM_FILE="randomfile"
RANDOM_FILE_MDZ="randomfile.mdz"
# FILE_SIZE="26"
# BLOCK_SIZE="13"
# MODBITS="64"

echo "args $#"
if [ $# -ne 5 ]; then
	echo "$0 FILESIZE BLOCKHASHLISTBINARY BLOCKSIZE MODBITS THREADS"
	echo "$0 100 11 11 64 16"
	echo "$0 100 101111000111 11 64 16"
	echo "$0 100 1010111 11 64 16"
	exit 2
else
	FILE_SIZE=$1
	BLOCK_BINARY_ARGS=$2
	BLOCK_SIZE=$3
	MODBITS=$4
	THREADS=$5
	# exit 0
fi

echo "File Size $FILE_SIZE"
echo "Block Size $BLOCK_SIZE"
echo "Modulus Bit Size $MODBITS"
echo "Thread Size $THREADS"

# check if the file size is specified
# if [ "$1" != '' ]
# then
#   FILE_SIZE="$1" 
# fi

echo "Creating a random file $RANDOM_FILE"
head -c $FILE_SIZE /dev/urandom > $RANDOM_FILE 

echo ""
echo "Zipping the file $RANDOM_FILE"
mdzip -file=randomfile -bh=$BLOCK_BINARY_ARGS -fh=001001 -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz 
# Invalid keylist
# mdzip -file=randomfile -bh=$BLOCK_BINARY_ARGS -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz -keylist=hw256:4334FFFZZZ,
# valid keylist
# mdzip -file=randomfile -bh=$BLOCK_BINARY_ARGS -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz -keylist=hw256:4334FFF,
# mdzip -file=randomfile -bh=$BLOCK_BINARY_ARGS -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz -keylist=hw256:4334FFF
# mdzip -file=randomfile -bh=$BLOCK_BINARY_ARGS -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz -keylist=hw256:4334FFF,hw64:787733777333777333773FF,
#  mdzip -file=randomfile -bh=$BLOCK_BINARY_ARGS -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz -keylist=hw256:24334FFF,

echo ""
echo "mdlist $RANDOM_FILE" 
mdlist randomfile.mdz

# check if the zip file exists
if test -f "$RANDOM_FILE_MDZ"; then
        echo ""
else
        echo "zip file failure"
        exit 2
fi

echo ""
echo "mdunzip $RANDOM_FILE"
mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=$THREADS -val
