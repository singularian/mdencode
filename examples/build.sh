#!/bin/bash

md -file=america.txt -block=100 -mod=128 -format=9 -fh=10000111 -bh=10001111 -out=america.txt_b100_mod128_f9.md
md -file=america.txt -block=100 -mod=128 -format=3000 -fh=10000111 -bh=10001111 -out=america.txt_b100_mod128_inform.md
md -file=america.txt -block=100 -mod=128 -format=4000 -fh=10000111 -bh=10001111 -out=america.txt_b100_mod128_json.md
md -file=america.txt -block=100 -mod=128 -format=5000 -fh=10000111 -bh=10001111 -out=america.txt_b100_mod128_xml.md

# md -file=america.txt -format=2000 -line=true -out=example.db
md -file=america.txt -format=2000 -line=true -out=example
md -file=america.txt -format=2000 -line=true -out=america

mddbreport -file=america.db -fileid=1 -format=10 -out=example.db.txt
mddbreport -file=america.db -fileid=1 -format=4000 -out=example.db.json
mddbreport -file=america.db -fileid=1 -format=5000 -out=example.db.xml
 
