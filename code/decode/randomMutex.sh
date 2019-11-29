#!/bin/bash

# this is a test file to test the mdzip and mdlist and mdunzip programs

RANDOM_FILE="randomfile"
FILE_SIZE="500"
BLOCK_SIZE="11"
MODBITS="64"

# check if the file size is specified
if [ "$1" != '' ]
then
   FILE_SIZE="$1" 
fi

echo "Creating a random file $RANDOM_FILE"
head -c $FILE_SIZE /dev/urandom > $RANDOM_FILE 

echo "\nZipping the file $RANDOM_FILE"
./mdzip -file=randomfile -mod=$MODBITS -block=$BLOCK_SIZE -out=randomfile.mdz

echo "\nmdlist $RANDOM_FILE" 
./mdlist randomfile.mdz

echo "\nmdunzip $RANDOM_FILE"
./mdunzipwg -file=randomfile.mdz -out=randomfile.mdz.out -thread=16
