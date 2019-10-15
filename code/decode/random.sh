
head -c 103 /dev/urandom > randomfile

./mdzip -file=randomfile -mod=64 -block=11 -out=randomfile.mdz
./mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=16
