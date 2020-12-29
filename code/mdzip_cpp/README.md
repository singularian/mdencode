# MDencode mdzip C++

This is the C++ mdzip/mdunzip code. It is a second variant of the GO mdzip/mdunzip.  
They currently aren't format compatible. It has some improvements over the GO version.   

TODO: add the unzip process to mdunzip. mdunzip only provides a list feature to dump the mdzip files.   

## mdzip usage

```
MDEncode MDzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDzip filename
  -b,--block INT:POSITIVE:INT in [1 - 100]
                              Blocksize number
  -m,--mod INT:POSITIVE       Modulus size number
  -t,--threads INT:POSITIVE   Thread count number
  --fhs INT:POSITIVE:INT in [1 - 33] ...
                              File Hashlist csv string
  --fh INT:POSITIVE:INT in [1 - 33] ...
                              File Hashlist integers list
  -r,--bhs INT:POSITIVE:INT in [1 - 33] ...
                              Block Hashlist csv string
  -s,--bh INT:POSITIVE:INT in [1 - 33] ...
                              Block Hashlist integers list
  -k,--keylist TEXT           Keylist csv string
  --rand BOOLEAN              Randomize Keylist
  -x,--list BOOLEAN           Display the Block list
  -l,--log BOOLEAN            Run Logging



Examples:
   ./mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   ./mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   ./mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4 


Hashlist:

ID          Hash Name   Description                   Key         Blocksize   
==============================================================================
1           cit64       Cityhash 64                   false       8           
2           crc32       CRC 32                        false       4           
3           crc64       CRC 64                        false       8           
4           fast32      Fasthash 32                   true        4           
5           fast64      Fasthash 64                   true        8           
6           fnv32       FNV-1  32                     false       4           
7           fnv32a      FNV-1a 32                     false       4           
8           fnv64       FNV-1  64                     false       8           
9           fnv64a      FNV-1a 64                     false       8           
10          hw64        Highway Hash 64               true        8           
11          met641      Metro Hash 64 v1              true        8           
12          met642      Metro Hash 64 v2              true        8           
13          md2         MD2                           false       16          
14          md4         MD4                           false       16          
15          md5         MD5                           false       16          
16          md6         MD6                           false       20          
17          md62        MD6 Quicker                   true        20          
18          png         Pengyhash 64                  true        8           
19          ripe160     Ripe MD 160                   false       20          
20          sea64       Seahash 64                    true        8           
21          sip64       Siphash 64                    true        8           
22          sha1_64     SHA1 64                       false       8           
23          sha1_128    SHA1 128                      false       16          
24          sha1        SHA1                          false       20          
25          sha256      SHA 256                       false       32          
26          sha384      SHA 384                       false       48          
27          sha512      SHA 512                       false       64          
28          spk32       Spooky 32                     true        4           
29          spk64       Spooky 64                     true        8           
30          xxh32       xxHash32                      true        4           
31          xxh64       xxHash64                      true        8           
32          whp         Whirlpool                     false       64          
33          wy64        WYhash 64                     true        8           
```

## mdunzip usage


```
MDEncode MDunzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--threads INT:POSITIVE   Thread count number
  -l,--list BOOLEAN           List the mdzip file
  --log BOOLEAN               Run Logging



Examples:
   ./mdunzip --file=filename.mdz --thread=16 
   ./mdunzip --file=test.mdz --thread=16 
   ./mdunzip --file=test.mdz --list=true


Hashlist:

ID          Hash Name   Description                   Key         Blocksize   
==============================================================================
1           cit64       Cityhash 64                   false       8           
2           crc32       CRC 32                        false       4           
3           crc64       CRC 64                        false       8           
4           fast32      Fasthash 32                   true        4           
5           fast64      Fasthash 64                   true        8           
6           fnv32       FNV-1  32                     false       4           
7           fnv32a      FNV-1a 32                     false       4           
8           fnv64       FNV-1  64                     false       8           
9           fnv64a      FNV-1a 64                     false       8           
10          hw64        Highway Hash 64               true        8           
11          met641      Metro Hash 64 v1              true        8           
12          met642      Metro Hash 64 v2              true        8           
13          md2         MD2                           false       16          
14          md4         MD4                           false       16          
15          md5         MD5                           false       16          
16          md6         MD6                           false       20          
17          md62        MD6 Quicker                   true        20          
18          png         Pengyhash 64                  true        8           
19          ripe160     Ripe MD 160                   false       20          
20          sea64       Seahash 64                    true        8           
21          sip64       Siphash 64                    true        8           
22          sha1_64     SHA1 64                       false       8           
23          sha1_128    SHA1 128                      false       16          
24          sha1        SHA1                          false       20          
25          sha256      SHA 256                       false       32          
26          sha384      SHA 384                       false       48          
27          sha512      SHA 512                       false       64          
28          spk32       Spooky 32                     true        4           
29          spk64       Spooky 64                     true        8           
30          xxh32       xxHash32                      true        4           
31          xxh64       xxHash64                      true        8           
32          whp         Whirlpool                     false       64          
33          wy64        WYhash 64                     true        8           
```


## MDencode links

[mdencode home](https://github.com/singularian/mdencode)  
[mdencode code](https://github.com/singularian/mdencode/tree/master/code)  
[mdencode mdzip go code](https://github.com/singularian/mdencode/tree/master/code/mdzip_go)  

