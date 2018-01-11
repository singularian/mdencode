#!/bin/bash

# this is a simple bash script to mdencode a file signature with an output filename and signature
# it specifies the block size and modulus size and format
# it also can include the block and file signatures 

FILE="$1"
EXT=".md"
OUTPUTFILENAME="$FILE$EXT"

if [ $# -ne 1 ]; then
    echo $0: usage: $0 filename
    exit 1
fi

md -file=$FILE -block=100 -mod=128 -format=4000 -fh=10000111 -bh=10001111 -out=$OUTPUTFILENAME
 
