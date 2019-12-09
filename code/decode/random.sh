#!/bin/bash

# this is a test file to test the mdzip and mdlist and mdunzip programs

RANDOM_FILE="randomfile"
# FILE_SIZE="26"
# BLOCK_SIZE="13"
# MODBITS="64"

echo "args $#"
if [ $# -ne 4 ]; then
	echo "$0 FILESIZE BLOCKSIZE MODBITS THREADS"
	echo "$0 100 11 64 16"
	exit 2
else
	FILE_SIZE=$1
	BLOCK_SIZE=$2
	MODBITS=$3
	THREADS=$4
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
./mdzip -file=randomfile -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz

echo ""
echo "mdlist $RANDOM_FILE" 
./mdlist randomfile.mdz

echo ""
echo "mdunzip $RANDOM_FILE"
./mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=$THREADS
