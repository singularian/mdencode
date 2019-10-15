#!/bin/bash

RANDOM_FILE="randomfile"
FILE_SIZE="100"
BLOCK_SIZE="11"
MODBITS="64"

# check if the file size is specified
if [ "$1" != '' ]
then
   FILE_SIZE="$1" 
fi

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
./mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=16
