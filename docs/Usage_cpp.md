# Command Line Usage of MDzip C++


## MDZip C++ Usage Example

```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ ./mdzip             
MDEncode MDzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDzip filename
  -b,--block INT:POSITIVE:INT in [1 - 100]
                              Blocksize number
  -m,--mod INT:POSITIVE       Modulus size number
  --fhs                       File Hashlist csv string
  --fh                        File Hashlist integers list
  -r,--bhs                    File Hashlist csv string
  -s,--bh                     Block Hashlist integers list
  -k,--keylist TEXT           Keylist csv string
  --randbh                    Randomize the Block Hash Keylist
  --inc                       Increment the Block Hash Keylist
  --dec                       Decrement the Block Hash Keylist
  -l,--log                    Run Logging



MDzip Examples:
   mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=false
   mdzip --file=randfile --block=14 --mod=32 --fh 13     --bh 5        --randbh
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh --dec
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh --inc

MDunzip Examples:
   mdunzip --file=filename.mdz --thread=16 
   mdunzip --file=test.mdz --thread=16 
   mdunzip --file=test.mdz --list
   mdunzip --file=filename.mdz --list --unzip 
   mdunzip --file=filename.mdz --valmdzip                        
```                                                                                                 

# MDunzip C++ Usage 

```     
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ mdunzip
MDEncode MDunzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -l,--list                   List the mdzip file
  -u,--unzip                  MDunzip a file
  -o,--over                   Overwrite an existing mdunzip output file
  --log                       Run Logging
  --debug                     Run Dubug
  --val                       Run the File Signatures on the output uncompressed file
  --valmdzip                  Validate the mdzip file



MDunzip Examples:
   mdunzip --file=filename.mdz --thread=16 
   mdunzip --file=test.mdz --thread=32 
   mdunzip --file=test.mdz     --list --val
   mdunzip --file=filename.mdz --list --unzip
   mdunzip --file=filename.mdz --valmdzip

MDzip Examples:
   mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=false
   mdzip --file=randfile --block=14 --mod=32 --fh 13     --bh 5        --randbh
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh --dec
   mdzip --file=randFileTest --mod=64 --bh=1-4 --bhs=23-25,26 --fh=1 6-7 15-20 --randbh --inc
```
                                                                                           

# MDzip C++ No Header Simplified Usage 

This is the usage for a cut down simplified version of MDzip.

Features  

- One 64-bit fasthash 64 signature
- 14 byte signature block
- 32 bit modulus
- 64-bit fasthash 64 randomizable signature key
- A capability to use other signatures with 64 bits

```
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ mdzipnh
MDEncode Minimized MDzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDzip filename
  -k,--key UINT:POSITIVE      Set fasthash key number
  -r,--rand BOOLEAN           Randomize the Key
  -l,--log BOOLEAN            Run Logging


 
Examples:
   mdzipnh --file=test.txt --key=1000 
   mdzipnh --file=test.txt --rand
   mdzipnh --file=test.txt  

   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list
   mdunzipnh --file=filename.mdsz --list --unzip
   mdunzipnh --file=filename.mdsz --valmdzip
```

# MDunzip C++ No Header Simplified Usage 

This is the usage for a cut down simplified version of MDunzip with just One 64-bit fasthash 64 signature and a 14-byte block with a 32-bit modulus.

```
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ mdunzipnh
MDEncode Minimized MDunzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -l,--list                   List the mdzip file
  -u,--unzip                  MDunzip a file
  -o,--over                   Overwrite an existing mdunzip output file
  --log                       Run Logging
  --valmdzip                  Validate the mdzip file



Examples:
   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list
   mdunzipnh --file=filename.mdsz --list --unzip
   mdunzipnh --file=filename.mdsz --valmdzip

   mdzipnh --file=test.txt --key=1000 
   mdzipnh --file=test.txt --rand
   mdzipnh --file=test.txt  

```

# MDzip Siphash 47 Usage 

mdzipsip47 can use two formats.  
- siphash40 with a 32 bit modulus and a 10 block file.
- siphash48 with a 24 bit modulus and a 10 block file (--sip48 option).

```
MDEncode MDzip Sip 47 C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDzip filename
  --fhs                       File Hashlist csv string
  --fh                        File Hashlist integers list
  --sip48                     Use Siphash48 for the block hash and modulus 24
  -k,--keylist TEXT           Keylist csv string
  --randbh                    Randomize the Block Hash Keylist
  --inc                       Increment the Block Hash Keylist
  --dec                       Decrement the Block Hash Keylist
  -l,--log                    Run Logging



   MDzip Examples:
      mdzipsip47 --file=test.txt 
      mdzipsip47 --file=test.txt --fh 1 2 3 
      mdzipsip47 --file=test.txt --fh 11     --randbh
      mdzipsip47 --file=test.txt --fh 11     --randbh=false
      mdzipsip47 --file=randfile --fh 13     --randbh --inc
      mdzipsip47 --file=randfile --fh 13     --randbh --dec

   MDunzip Examples:
      mdunzipsip47 --file=filename.mdz --thread=16 
      mdunzipsip47 --file=test.mdz --thread=16 
      mdunzipsip47 --file=test.mdz --list
      mdunzipsip47 --file=filename.mdz --list --unzip 
      mdunzipsip47 --file=filename.mdz --valmdzip 
```

# MDunzip Siphash 47 Usage



```
MDEncode MDunzip Sip 47 C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -l,--list                   List the mdzip file
  -u,--unzip                  MDunzip a file
  -o,--over                   Overwrite an existing mdunzip output file
  --log                       Run Logging
  --debug                     Run Dubug
  --val                       Run the File Signatures on the output uncompressed file
  --valmdzip                  Validate the mdzip file

```

# MDzip C++ Test

This is the Usage for mdtest.

```
MDEncode GMP C++ Test Program
Usage: ./mdtest [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -b,--block UINT:POSITIVE:INT in [1 - 100]
                              Blocksize number
  -m,--mod INT:POSITIVE       Modulus size number
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -r,--bh                     Block Hashlist csv string
  -s,--hl                     Block Hashlist integers list
  --randbh                    Randomize the Block Hash Keylist
  -x,--hex TEXT               Hex Byteblock string
  -l,--log                    Run Logging
  -q,--skip                   Skip the Modulus Scan



Examples:
   mdtest -b 12 -m 64 -t 16
   mdtest --block=12 --mod=64    --threads=16
   mdtest --block=12 --mod=128   --threads=16
   mdtest --mod=64 --threads=16 --hex=0011
   mdtest --mod=64 --threads=16 --hex=FFd033FF202020202011
   mdtest --mod=64 --threads=16 --hex=FFd033FF202020202011 --log --hl 1 2 3 4 5
   mdtest --mod=64 --threads=16 --hex=FFd033FF202020202011 --log --bh 1,5,7
   mdtest --mod=64 --threads=32  --hl 1 2 3 4 5 --randbh --block=12
   mdtest --mod=64 --threads=32 --bh=6-8,23,33,34 --hl 1 2 3 4-8 --randbh --block=12
                         
```
