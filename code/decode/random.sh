#!/bin/bash

RANDOM_FILE="randomfile"

echo "Creating a random file $RANDOM_FILE"
head -c 103 /dev/urandom > $RANDOM_FILE 

echo ""
echo "Zipping the file $RANDOM_FILE"
./mdzip -file=randomfile -mod=64 -block=11 -out=randomfile.mdz

echo ""
echo "mdlist $RANDOM_FILE" 
./mdlist randomfile.mdz

echo ""
echo "mdunzip $RANDOM_FILE"
./mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=16
