# Command Line Usage of mdencode

```                                                                                                                                              
Usage of md:                                                                                                                                     
                                                                                                                                                 
  -file string                                                                                                                                   
        Input Filename                                                                                                                           
  -block string                                                                                                                                  
        File Block Size Bytes (default "40")                                                                                                     
  -fh string                                                                                                                                     
        File Hash Bit String List (default "011")                                                                                                
  -bh string                                                                                                                                     
        Block Hash Bit String List (default "011")                                                                                               
  -mod string                                                                                                                                    
        Modulus Size in Bits (default "32")                                                                                                      
  -format int                                                                                                                                    
        Output Format (default 10)                                                                                                               
  -fr                                                                                                                                            
        Generate A Random File Hash Bit String List                                                                                              
  -br                                                                                                                                            
        Generate A Random Block Hash Bit String List                                                                                             
  -fbr                                                                                                                                           
        Generate A Random File Hash and Block Hash Bit String List                                                                               
  -blockr                                                                                                                                        
        Generate A Random File Block Size                                                                                                        
  -modr                                                                                                                                          
        Generate A Random File Block Modulus                                                                                                     
  -all                                                                                                                                           
        Randomize everything                                                                                                                     
  -out string                                                                                                                                    
        Output Filename                                                                                                                          
  -dir string                                                                                                                                    
        Input Directory                                                                                                                          
  -append                                                                                                                                        
        Append To Output File                                                                                                                    
  -line                                                                                                                                          
        File Hash as one line                                                                                                                    
  -byte                                                                                                                                          
        Append the File Byteblock to the Block Hash List                                                                                         
  -blockint                                                                                                                                      
        Append the File Byteblock Bigint to the Block Hash List                                                                                  
  -log string                                                                                                                                    
        Log Filename                                                                                                                             
  -initdb string                                                                                                                                 
        Create SQLite3 Empty DB File                                                                                                             
  -key string                                                                                                                                    
        Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")                                                         
                                                                                                                                                 
Version: 1.0 復甦 復活                                                                                                                               
                                                                                                                                                 
Formats:                                                                                                                                         
0 .. 98 - Text                                                                                                                                   
101     - CSV                                                                                                                                    
102     - CSV                                                                                                                                    
1000    - Binary                                                                                                                                 
2000    - SQL Lite 3 DB File                                                                                                                     
3000    - Inform                                                                                                                                 
4000    - JSON                                                                                                                                   
5000    - GO XML                                                                                                                                 
5001    - XML Formatter                                                                                                                          
                                                                                                                                                 
Examples:                                                                                                                                        

md -file=Makefile -block=100 -fh=1 -bh=0 -format=0 -line=true
md -file=Makefile -block=100 fh=1111111 -bh=0 -format=10 -line=false
md -file=Makefile -block=300 -fh=0 -bh=111 -format=10 -line
md -file=Makefile -block=100 -fh=1011111 -bh=1100111 -format=19 -line=true -out=outputfile
md -file=Makefile -block=100 -fh=10100111 -bh=100111   -format=19 -append=true -line=true -out=outputfile
md -file=Makefile -block=100 -fh=1011111101 -bh=10011101 -format=3000 -append=false -line=true -out=outputfile.inform
md -file=Makefile -block=100 -fh=1011111001 -bh=100111   -format=4000 -append=true -line=true -out=outputfile.json
md -file=Makefile -block=1000 -mod=0 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=64 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=128 -fh=1011111 -bh=100111  -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=350 -mod=32 -fh=101011 -br
md -file=Makefile -block=300 -mod=32 -fr -br
```

# Command Line Examples

```sh
md -file=Makefile -block=100 -fh=1 -bh=0 -format=0 -line=true
md -file=Makefile -block=100 fh=1111111 -bh=0 -format=10 -line=false
md -file=Makefile -block=300 -fh=0 -bh=111 -format=10 -line
md -file=Makefile -block=100 -fh=1011111 -bh=1100111 -format=19 -line=true -out=outputfile
md -file=Makefile -block=100 -fh=10100111 -bh=100111   -format=19 -append=true -line=true -out=outputfile
md -file=Makefile -block=100 -fh=1011111101 -bh=10011101 -format=3000 -append=false -line=true -out=outputfile.inform
md -file=Makefile -block=100 -fh=1011111001 -bh=100111   -format=4000 -append=true -line=true -out=outputfile.json
md -file=Makefile -block=1000 -mod=0 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=64 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=128 -fh=1011111 -bh=100111  -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=350 -mod=32 -fh=101011 -br
md -file=Makefile -block=300 -mod=32 -fr -br
```

# Example mddbreport usage

```
Usage of mddbreport:
  -append
        Append To Output File
  -dumpall
        Dump all the sql file signatures
  -file string
        SQLite3 DB Signature Filename
  -fileid uint
        Fileid
  -format int
        Output Format (default 4)
  -out string
        Output Filename

Version: 1.0 復甦 復活

Examples:
mddbreport -file=md.db -fileid=1 -format=10
mddbreport -file=md.db -fileid=1 -format=10 -out=outputfile.txt -append=true
mddbreport -file=md.db -fileid=1 -format=5000 -out=outputfile.xml -append=false

```


