#!/bin/bash

md -file=america.txt -block=100 -mod=128 -format=3000 -out=america.txt_b100_mod128_inform.md
md -file=america.txt -block=100 -mod=128 -format=4000 -out=america.txt_b100_mod128_json.md
md -file=america.txt -block=100 -mod=128 -format=5000 -out=america.txt_b100_mod128_xml.md

mddbreport -file=america.db -format=10 -out=example.db.txt
mddbreport -file=america.db -format=5000 -out=example.db.xml
 
