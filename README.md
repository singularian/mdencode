# mdencode
md signature generator for files

# overview

This is a golang command line file signature generator with a modular floor.
It can generate a file signature at the file or block level and format and write the output in different formats.
It can write to an sqllite db

# mdencode usage

Usage of md:
  -append
     Append To Output File
  -bh string
     Block Hash Boolean String List (default "011")
  -block string
     File Block Size Bytes (default "40")
  -byte
     Generate the File Byteblock
  -fh string
     File Hash Boolean String List (default "011")
  -file string
     Filename
  -format int
     Output Format (default 4)
  -key string
     Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")
  -line
     File Hash as one line
  -log string
     Log Filename
  -mod string
     Modulus Size in Bits (default "32")
  -out string
     Output Filename

Version: 1.0 復甦 復活

Formats
0 - 98 - Text
99 - Inform
100 - Json
101 - CSV
102 - CSV
600 - Json
1000 - Binary
2000 - SQL Lite 3 DB File

md -file=Makefile -block=100 -line=false -bh=0 -fh=1111111 -format=19 -line

md -file=Makefile -block=100 -line=false -bh=111 -fh=0 -format=19 -line

md -file=Makefile -block=100 -line=false -bh=111111 -fh=1111111 -format=19 -line=true -out=outputfile

md -file=Makefile -block=100 -line=false -bh=111111 -fh=1111111 -format=19 -append=true -line=true -out=outputfile

# mddbreport usage
This example shows how to generate an sql lite file db signature

md -file=Makefile -format=2000 -line=true -out=ssss

It can also generate a formatted signature from the ssss.db sqlite 3 file.

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ mddbreport ssss.db

file 1: /home/nsross/projects/src/github.com/singularian/mdencode/code/encode/ Makefile 1.0

# mddbreport text format example

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ mddbreport ssss.db 1

Makefile
/home/nsross/projects/src/github.com/singularian/mdencode/code/encode/
1558:8:40:32:md5:sha1-md5:sha1:1.0:2017-11-23 09:14:13.074325487 -0500 EST
z: md5:sha1 cf3bf9b0cc9d76c4807fbe7aa70dd6d2:da39a3ee5e6b4b0d3255bfef95601890afd80709
40:493a1de47de4c76a1fe678e3b1c1263d:255078b58fa4f01057bf5568d55f5b2b63d0b8de:317:1851880057
40:38925f2bf8b95ab371933c07c7aa053b:baecfb418ea6c44c970454247da56a2a85bdd6fc:317:544499813
40:4def53e92581113421ae20a47ecd45a1:15e7e4c7daef5fd91c71ac9844e95e3a8f144b27:317:1634624544
40:5ed3a775ad27c883fcbd8471cc39aac0:6f8ddd4c76be6ff44e69c590b427f1d3648ea36d:318:1684108389
40:9f3caadd45eb733501fcd04a43458bfa:716c95ab525c13fd59b2bac483d1ed93221489fe:317:1430867012
40:fe4d60885d5e7387dd0a3dcd0c9d6f3c:0dcebe01bb9b501a2bb401aebaae95a603c0c89b:318:1919508846
40:1975f015a73caf96dc11b73eb60cb4e8:1ddb89b8edccc0be007602fea9eed3af5f96a73c:317:1598310738
40:a2eb9495034ea231fe0aa4329e77a952:65004e18cef38a033336c09faa8c64723d44a9b1:317:542336372
40:66e6ebcb8883924ed68fa3a8b325bf7d:6e7d6afdc03e243f827b2e4c2c33e099b177a3b4:318:1751478885
40:a44771da23e3471800386005f29ce873:4363afab384d225ec553818b14a812c6187d202e:317:1179402567
40:8585982fbfd642c103f06daf60cef152:644ff6d5554ebc551b6b890c71bab35cfaa8a07a:318:1848601956
40:58bda4988c3cf4f818f10cc617ee39b2:88292dbf48ed93152e438656bdebf9871f109bc9:318:1953002850
40:2e15d3915184e7bf81b0611f2f87d630:8b250fe69e7b9567ce6fa7e0cde5680132acf3cf:317:1416195429
40:30f0b49c43f9d7dff8dd4bae6beeb530:e47b0be48d3cdb9e902b809e4ec9dc32c1dd3e7a:317:2067944777
40:21641e406f71a0b96aa19aae38d95b2f:649d84ecf54480c8ae424b58d893410504060b13:318:612057685
40:11901b1a6f8d3b7ab44253391c28d029:e7e7e95362dab696eb72f99bb9e245173109e2ac:318:1768711200
40:9ea9af485765de616f892ac6e1c11410:0cfdd0f02a0d4383f47a49d501b302b7c164c16f:317:1397836846
40:5bda4dcf0c7cee7070e85f18cb809b6d:f3d19c8b8805cc11497c8e822bf48b7daf52ba28:318:544040046
40:a6a4018580f6d64ba58aa3681d01267d:1a8042eb9157fd5bf61c755b128220110ce72582:318:1684173413
40:f475f42ca96939fa6d30e6ad9be57928:f2d1542171ab4d7b62768eae8502a02919b761ff:318:1853060193
40:4f74a362d756d2c8678aca6c57554df5:8390d7ce88d18197d82ec790e0a96efc4544e22e:318:1397836846
40:f01287cbed23a6af5369bcb71adffd7e:1d0f8ae4c93a7427fd7098de7dd5d549e671f290:318:1699959116
40:91f3d6c68614d06c9d06e53ccaa5c14f:d833c9a428d8b454419688d70eaa2973839d0b73:315:1697539951
40:fc08e24fc83c60be0e67231f3bd32a47:e2253988d2cfa3982dd815572d247ee5bf3c3fa3:315:1862935305
40:eda11ade4af4c1814dadac5e1dd7c2d7:8e5c5484eca0d6419c4b2d9b21d3ee7b2a102ff2:318:1668244581
40:1df1a938172eabbc7a1ee64c405cb3d9:b2d49643bcb7b5fdace8a7b3f7de2ab526d81b1c:317:1684368995
40:a1f922a9b454cd0de214f8a78504da7c:4cfde2d4bc360936e51b74d09c788db9386b76ec:318:1930061669
40:a1544ddb840e5d34bd6454d0c45dc276:ef96b8c803ae4d7ababb3910204c9f42ee869d6c:318:1768318255
40:83f24f81c758a50073c7dc934c8b8be2:2b9ed66b94e1e8d9982cffc9defdd9d511d9a4c3:318:1865249649
40:9c2b713afcd930a122c1ebf800be25ec:91f827eb85262bbba58903c59198d3c41e770daf:318:1747938677
40:f46f97e0bbc9f5ce6c20be10355cb469:b10961e20e9d2e1db42c1eec5673b7cf1a97d6b5:318:796027248
40:37c08392f161c86b84356474c260fe4f:4589e6860c9f25c280a1b38c5b305cb4a3678703:318:1936289896
40:6d7c24fcac4489298484a6067e0f874d:c7e3b8504c9a9d9b4bf65474597ed37ea97a0590:318:168388463
40:0e20dcca42982f58b40d09d8db3e6e07:a1b0db9f6e60d2265705ad8db1ab18e04957eada:317:1702109229
40:2e53ec3a0470f4552664e184c0d561a0:aac70169f8a235ab9b01f543082814d286673d14:318:1949960713
40:797e7c9d041c629e1ef0a54ea737cc66:b156eabfe6d455e4b5e63b21b76009f17fde942c:318:1684368995
40:7a260f932d7d2c59f276ffff0b83d3e4:c0274903d8432b0c063867f89aae540d88cd167a:318:1868850530
40:05aaf28a3bf3ed6b9b8afe09f5372145:9ccd573dfe1cf87d3d3849d48599d6c25cfa9367:318:1846151538
38:20ae9e8ab2b3422153495cc81afdcbc7:b8b91ef0604d76bfc1941c8525a42f940667a45d:302:1819025930

# mddbreport json format example

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ mddbreport ssss.db 1 600

{"mdfile": {"FileName":"Makefile","FilePath":"/home/nsross/projects/src/github.com/singularian/mdencode/code/encode/","Filesize":1558,"Blocksize":40,"Modsize":32,"Format":600,"Hashlist":"md5:sha1-md5:sha1","Version":"1.0","Time":"2017-11-23 09:14:15.906471611 -0500 EST"},
{"Mdtype":"File","Hashname":"md5:sha1","Filehashlist":"cf3bf9b0cc9d76c4807fbe7aa70dd6d2:da39a3ee5e6b4b0d3255bfef95601890afd80709"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"493a1de47de4c76a1fe678e3b1c1263d:255078b58fa4f01057bf5568d55f5b2b63d0b8de","Modexp":"317","Mod":"1851880057"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"38925f2bf8b95ab371933c07c7aa053b:baecfb418ea6c44c970454247da56a2a85bdd6fc","Modexp":"317","Mod":"544499813"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"4def53e92581113421ae20a47ecd45a1:15e7e4c7daef5fd91c71ac9844e95e3a8f144b27","Modexp":"317","Mod":"1634624544"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"5ed3a775ad27c883fcbd8471cc39aac0:6f8ddd4c76be6ff44e69c590b427f1d3648ea36d","Modexp":"318","Mod":"1684108389"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"9f3caadd45eb733501fcd04a43458bfa:716c95ab525c13fd59b2bac483d1ed93221489fe","Modexp":"317","Mod":"1430867012"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"fe4d60885d5e7387dd0a3dcd0c9d6f3c:0dcebe01bb9b501a2bb401aebaae95a603c0c89b","Modexp":"318","Mod":"1919508846"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"1975f015a73caf96dc11b73eb60cb4e8:1ddb89b8edccc0be007602fea9eed3af5f96a73c","Modexp":"317","Mod":"1598310738"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"a2eb9495034ea231fe0aa4329e77a952:65004e18cef38a033336c09faa8c64723d44a9b1","Modexp":"317","Mod":"542336372"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"66e6ebcb8883924ed68fa3a8b325bf7d:6e7d6afdc03e243f827b2e4c2c33e099b177a3b4","Modexp":"318","Mod":"1751478885"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"a44771da23e3471800386005f29ce873:4363afab384d225ec553818b14a812c6187d202e","Modexp":"317","Mod":"1179402567"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"8585982fbfd642c103f06daf60cef152:644ff6d5554ebc551b6b890c71bab35cfaa8a07a","Modexp":"318","Mod":"1848601956"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"58bda4988c3cf4f818f10cc617ee39b2:88292dbf48ed93152e438656bdebf9871f109bc9","Modexp":"318","Mod":"1953002850"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"2e15d3915184e7bf81b0611f2f87d630:8b250fe69e7b9567ce6fa7e0cde5680132acf3cf","Modexp":"317","Mod":"1416195429"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"30f0b49c43f9d7dff8dd4bae6beeb530:e47b0be48d3cdb9e902b809e4ec9dc32c1dd3e7a","Modexp":"317","Mod":"2067944777"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"21641e406f71a0b96aa19aae38d95b2f:649d84ecf54480c8ae424b58d893410504060b13","Modexp":"318","Mod":"612057685"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"11901b1a6f8d3b7ab44253391c28d029:e7e7e95362dab696eb72f99bb9e245173109e2ac","Modexp":"318","Mod":"1768711200"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"9ea9af485765de616f892ac6e1c11410:0cfdd0f02a0d4383f47a49d501b302b7c164c16f","Modexp":"317","Mod":"1397836846"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"5bda4dcf0c7cee7070e85f18cb809b6d:f3d19c8b8805cc11497c8e822bf48b7daf52ba28","Modexp":"318","Mod":"544040046"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"a6a4018580f6d64ba58aa3681d01267d:1a8042eb9157fd5bf61c755b128220110ce72582","Modexp":"318","Mod":"1684173413"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"f475f42ca96939fa6d30e6ad9be57928:f2d1542171ab4d7b62768eae8502a02919b761ff","Modexp":"318","Mod":"1853060193"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"4f74a362d756d2c8678aca6c57554df5:8390d7ce88d18197d82ec790e0a96efc4544e22e","Modexp":"318","Mod":"1397836846"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"f01287cbed23a6af5369bcb71adffd7e:1d0f8ae4c93a7427fd7098de7dd5d549e671f290","Modexp":"318","Mod":"1699959116"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"91f3d6c68614d06c9d06e53ccaa5c14f:d833c9a428d8b454419688d70eaa2973839d0b73","Modexp":"315","Mod":"1697539951"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"fc08e24fc83c60be0e67231f3bd32a47:e2253988d2cfa3982dd815572d247ee5bf3c3fa3","Modexp":"315","Mod":"1862935305"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"eda11ade4af4c1814dadac5e1dd7c2d7:8e5c5484eca0d6419c4b2d9b21d3ee7b2a102ff2","Modexp":"318","Mod":"1668244581"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"1df1a938172eabbc7a1ee64c405cb3d9:b2d49643bcb7b5fdace8a7b3f7de2ab526d81b1c","Modexp":"317","Mod":"1684368995"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"a1f922a9b454cd0de214f8a78504da7c:4cfde2d4bc360936e51b74d09c788db9386b76ec","Modexp":"318","Mod":"1930061669"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"a1544ddb840e5d34bd6454d0c45dc276:ef96b8c803ae4d7ababb3910204c9f42ee869d6c","Modexp":"318","Mod":"1768318255"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"83f24f81c758a50073c7dc934c8b8be2:2b9ed66b94e1e8d9982cffc9defdd9d511d9a4c3","Modexp":"318","Mod":"1865249649"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"9c2b713afcd930a122c1ebf800be25ec:91f827eb85262bbba58903c59198d3c41e770daf","Modexp":"318","Mod":"1747938677"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"f46f97e0bbc9f5ce6c20be10355cb469:b10961e20e9d2e1db42c1eec5673b7cf1a97d6b5","Modexp":"318","Mod":"796027248"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"37c08392f161c86b84356474c260fe4f:4589e6860c9f25c280a1b38c5b305cb4a3678703","Modexp":"318","Mod":"1936289896"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"6d7c24fcac4489298484a6067e0f874d:c7e3b8504c9a9d9b4bf65474597ed37ea97a0590","Modexp":"318","Mod":"168388463"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"0e20dcca42982f58b40d09d8db3e6e07:a1b0db9f6e60d2265705ad8db1ab18e04957eada","Modexp":"317","Mod":"1702109229"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"2e53ec3a0470f4552664e184c0d561a0:aac70169f8a235ab9b01f543082814d286673d14","Modexp":"318","Mod":"1949960713"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"797e7c9d041c629e1ef0a54ea737cc66:b156eabfe6d455e4b5e63b21b76009f17fde942c","Modexp":"318","Mod":"1684368995"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"7a260f932d7d2c59f276ffff0b83d3e4:c0274903d8432b0c063867f89aae540d88cd167a","Modexp":"318","Mod":"1868850530"},
{"Mdtype":"Block","Blocksize":"40","Hashlist":"05aaf28a3bf3ed6b9b8afe09f5372145:9ccd573dfe1cf87d3d3849d48599d6c25cfa9367","Modexp":"318","Mod":"1846151538"},
{"Mdtype":"Block","Blocksize":"38","Hashlist":"20ae9e8ab2b3422153495cc81afdcbc7:b8b91ef0604d76bfc1941c8525a42f940667a45d","Modexp":"302","Mod":"1819025930"}}

# License 

https://github.com/singularian/mdencode/blob/master/LICENSE
