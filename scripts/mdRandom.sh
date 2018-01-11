#!/bin/bash

# this is a simple bash script to mdencode a file signature with an output filename and signature
# it specifies the block size and modulus size and format
# it also can include the block and file signatures 

FILE="$1"
EXT=".md"
OUTPUTFILENAME="$FILE$EXT"
bh=`cat /dev/urandom | tr -dc '0-1' | fold -w 64 | head -n 1`
fh=`cat /dev/urandom | tr -dc '0-1' | fold -w 64 | head -n 1`

# echo "fh $bh $fh $#"

if [ $# -ne 1 ]; then
    echo $0: usage: $0 filename
    exit 1
fi

# md -file=$FILE -block=100 -mod=128 -format=4000 -fh=$bh -bh=$fh -out=$OUTPUTFILENAME
md -file=$FILE -block=100 -mod=128 -format=4000 -fh=$bh -bh=$fh
 
