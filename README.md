# mdencode
A message digest signature generator for files with modular floors
mdencode is written in golang and uses an interface for pluggin formatters
mdencode allows for the creation of sqlite3 db file signatures

# overview

This is a golang command line file signature generator with a modular floor.
It can generate a file signature at the file or block level and format and write the output in different formats.
It can write to an sqllite db

# Output Formats

1. TEXT
2. INFORM
3. XML
4. JSON
5. BINARY
6. CSV
7. SQLITE3 DB
8. BUILD YOUR OWN


# mdencode usage

Usage of md:
     
  -bh string
     Block Hash Boolean String List (default "011")
     
  -fh string
     File Hash Boolean String List (default "011")   
     
  -block string
     File Block Size Bytes (default "40")   
 
 -mod string
     Modulus Size in Bits (default "32")
     
   -append
     Append To Output File    

  -file string
     Input filename
     
  -format int
     Output Format (default 4)
     
  -key string
     Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")
  
  -line
     File Hash as one line
  
  -log string
     Log Filename
 
 -byte
     Generate the File Byteblock
     
  -out string
     Output Filename

Version: 1.0 復甦 復活

Output Formats
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

# md example usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by go flags args.

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ md -file=Makefile -block=100 -mod=128 -bh=111 -fh=11011 -format=10 -line=true

Makefile
/home/nsross/projects/src/github.com/singularian/mdencode/code/encode/
1558:8:100:128:md4:md5:sha224:sha256-md4:md5:sha1:1.0:2017-11-23 09:33:32.083687811 -0500 EST
z: md4:md5:sha224:sha256 a066c3c3fe90ac16e6760261f2ec4210:d41d8cd98f00b204e9800998ecf8427e:d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f:e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
100:ba4da86b9127bff0c182c58cdb960acc:23417647afff4d83ababf66431fb7355:46e774051493be26d1f557347f0a48a74668e994:797:156046432538903766113216376986191994992
100:fc1816c3f3c8d8ccfe0dcfda1d9450e6:6fa717598f0fccfa87d712cabab3e6e4:a1067bb6bcf9829c9f36b071b4ef4bb5b4fb9479:798:61384294535547225472044466722774273092
100:f755d94871fbf6974164eb0ad05c472a:10fb607003759c939f5004191bfe59dc:8161de6b6c675c20506e35505efa62b67010b9d8:798:105425720449915288927656420490829450308
100:c65fa83ce9a2161bd6cd9d3753265106:407062d79cb533cd992821eb3273ee7d:4e852edfdb668d26de4724772beb49d371a469c9:798:144081703943664589730216165720332190023
100:c34a6d09d5a00be68d418ed20a212650:474513176f910f1f656a2342c816c884:d8c1698b50ec6e37228128c069ea549b9da9a325:798:63072968340747723343883595144607130725
100:c9361cf1d878e275adb5ea1c375ed6dd:06ba713f145849f556047233b29acf14:7e17c9ce58ec9fdaaf8cf649ca5b1b94b95d5f7d:798:137487485354948139249947760744556347989
100:7a32b4172f4b2f84aa93cd0cc73cd4d1:b7e4c5839de02ab718ddf633353aa91e:1628678e91565290a11590f886d35b3798f1bb59:798:137487485354948139249947760744556347989
100:6379d854c35371d0bba1373350a3431c:a94ab9c8efbe7821c5432fefd116a614:6f813d5e2a110dcb4eb365880c147a87cf5ea741:798:156067065117997145121715642749173331041
100:4a796dff1b455c77cb56ae5718b43157:3de58a2eb4c36269e1de3cb6ab0aaf61:70090ac41e759afe8d90e927bd6053ed566e71a7:798:147712599435436507965025664839365518437
100:98068bf526914d2d8ac1f3066f239999:3f662874cce4d2d63212b118ab800045:86d06cdc74546cc16967e2485d3ee3ecff11a795:798:154716865761017993889276371155228255333
100:66e4f76d2f679af519c846c67cd681be:936d51770e1c7c55d4145a564c3fc8eb:06a3058758f9698d5e2461857068afb35ca55dbf:797:153162576302221144332104219346258126697
100:799e63c77dd8be08fca6ebc79a5d1413:f8df5e83a7952f48d7d7afd0e96b0590:e00ca135dd1f61a33d6b07b914e76859ed62875a:798:132172131070534005830780023095214173557
100:f6064cdae7fd6c8c24756ec3f7f4aaf6:65a5fb75d49c92da615804155a8a9b8e:d7e7abf1fe344689df574085be4070b55a35b4b9:798:12500112453260141801211768598085003381
100:fb2aaeb9cfd641d6523711b87d68a6e1:62c87b5a456b7942e7193332d6b18343:17c528663c00ae9dc3b13965c66743e3d625b817:798:158720269090271827991004656045368412681
100:11b7c8218ba14185b4288fbddb2c4b99:885597de816cf8086612b7f0c7091312:46359dcc0ab600d5c4d581079603480ed95228e8:798:144134984942068198018378939922176495948
58:0bf37e8aa18b4b35d0ddc9a027649ecb:f918abc0f68b1a7fc0027f1b813439f7:cc556ee447d84eef7cfd9ea614e1254d14e017d1:461:153464703125801938365299123247572066826

# md example usage of the input file Makefile and the output format json

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode_private/code/encode$ md -file=Makefile -block=100 -mod=128 -bh=111 -fh=11011 -format=600 -line=true


{"mdfile": {"FileName":"Makefile","FilePath":"/home/nsross/projects/src/github.com/singularian/mdencode_private/code/encode/","Filesize":1965,"Blocksize":100,"Modsize":128,"Format":600,"Hashlist":"md4:md5:sha224:sha256-md4:md5:sha1","Version":"1.0","Time":"2017-11-23 09:42:31.782896468 -0500 EST"},
{"Mdtype":"File","Hashname":"md4:md5:sha224:sha256","Filehashlist":"d1f56aebff336585d02d0e3e9555abe2:d41d8cd98f00b204e9800998ecf8427e:d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f:e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"ba4da86b9127bff0c182c58cdb960acc:23417647afff4d83ababf66431fb7355:46e774051493be26d1f557347f0a48a74668e994","Modexp":"797","Mod":"156046432538903766113216376986191994992"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"fc1816c3f3c8d8ccfe0dcfda1d9450e6:6fa717598f0fccfa87d712cabab3e6e4:a1067bb6bcf9829c9f36b071b4ef4bb5b4fb9479","Modexp":"798","Mod":"61384294535547225472044466722774273092"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"f755d94871fbf6974164eb0ad05c472a:10fb607003759c939f5004191bfe59dc:8161de6b6c675c20506e35505efa62b67010b9d8","Modexp":"798","Mod":"105425720449915288927656420490829450308"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"c65fa83ce9a2161bd6cd9d3753265106:407062d79cb533cd992821eb3273ee7d:4e852edfdb668d26de4724772beb49d371a469c9","Modexp":"798","Mod":"144081703943664589730216165720332190023"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"c34a6d09d5a00be68d418ed20a212650:474513176f910f1f656a2342c816c884:d8c1698b50ec6e37228128c069ea549b9da9a325","Modexp":"798","Mod":"63072968340747723343883595144607130725"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"edc5b0659ae82655c4b2fcad865e5fd2:0ea35a91a8fb71f46311bc8b995b48eb:33867c6195be74675c0f6450c01b75df998bf6fb","Modexp":"798","Mod":"133449460504322302647394520592401983343"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"ed917a1ce22abe41131e84e8a7936c69:92de3e0d06e1be43b639de62954e4209:63e942c0af23938c8bff2ec9020f6257df9561a0","Modexp":"795","Mod":"126631222937300731277338325196405547117"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"e7fba24b21eb5a9f6066fee81faf9681:d7e6ec077bb612e5bea738a2eba02e8e:5d1f3f2a3d86193fc65caab7d74082f2077ab04d","Modexp":"798","Mod":"126631222937300731277338325196405560145"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"c114987c19b3e45648e1c95a8183c0fc:c197eb86b09f84344842b47f2863a96b:f321331ba5b4b6db69ffde9a30fe6f9d0c756de1","Modexp":"798","Mod":"163839464886283642510567873608298030446"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"f4df5501764aa5ac2c8707eada4befee:cd2dd65050d9fc826782e50b73224154:53447d81f53b23e8dc122a61e444cdbbd2770273","Modexp":"798","Mod":"147712458010061911259042185134299305289"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"bbbb04f9e2078dab07d74a25ddb95844:e57b1a35f71e9eb7f14f220de7b0b734:693beecbf60d757d434abd473384320a98b319b6","Modexp":"798","Mod":"88171879613181476710298929616805785187"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"53d0de926725d903ecd50fb3495f476d:1c64c06cd5ea6da41c1e0d6dd3fb673f:1470898fc723948c8c973f2358e9f9dc038e27d7","Modexp":"798","Mod":"134846006847564845548935135491706718729"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"5b67c856bd095e04772ddbfd67610688:aa5136c1c76fb3dd47099b304e4978ee:3b7e75082c85f21339005ae9933a5392a67c9f28","Modexp":"798","Mod":"110748041961944185288318008279550749804"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"4580b0d5643e8b0b218d3270271ba039:1300716009bc9b8866747c7b11e58ca4:e9a4091108e78cbb86788374aafa93d64b5ddac0","Modexp":"797","Mod":"146731376226994984992244459415634911341"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"7dd4285e595e2f0888565d5222b92c2a:91cf9dfabedc315ff4af0555d2439b05:07082139648b4512ce752a3fdcae3151ca8ce6d4","Modexp":"798","Mod":"13829144281666849637390549979458725733"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"957c4aacb20e76c3a6858d4b90051426:7a578c509de0a28e20b92f26c9c0e744:c01e23ff40f8b68575f8592a6cc470725bef47a6","Modexp":"798","Mod":"137458035771563856829882724740275201133"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"448bfc5ce90b9fd5dd3eac79ac93d48b:4ee6542d2591affc0c5a15a318d0ed6b:2d7be6df630a344d29b057dcbd4f712588569497","Modexp":"798","Mod":"140152593897019162818493335240340414509"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"26b5e185623ffb5685e9e2379b088d47:423459cec73065a1def2c1ac6d7082f9:4d3773f000364bbaf6c1de12f6c52b074005d957","Modexp":"798","Mod":"140142433198991058355354192238827499372"},
{"Mdtype":"Block","Blocksize":"100","Hashlist":"1f19a060ff5cab718760271aab347982:556e870cc9c2b617f7837898dcc78ad6:d6599b6c670e43bff406de60bdb2436dc7ce9c1d","Modexp":"795","Mod":"133449218859975254409795845259234927982"},
{"Mdtype":"Block","Blocksize":"65","Hashlist":"e7b8d346603309f572d4457592037cb0:118f609208aa6ef8127f7e915b86d94f:04154a63899f1373aa9b22c60d4cbe82c73b7eb4","Modexp":"518","Mod":"153464703125801938365299123247572066826"}}

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


# mddbreport xml format example

<md><hd><filename>Makefile</filename><filepath>/home/nsross/projects/src/github.com/singularian/mdencode/code/encode/</filepath><filesize>1558</filesize><format>100</format><blocksize>40</blocksize><modulussize>32</modulussize><hashlist>md5:sha1-md5:sha1</hashlist><time>2017-11-23 09:31:38.770963168 -0500 EST</time></hd>
<filehash><hashname>md5:sha1</hashname><hash>cf3bf9b0cc9d76c4807fbe7aa70dd6d2:da39a3ee5e6b4b0d3255bfef95601890afd80709</hash></filehash>
<b><size>40</size><hashlist>493a1de47de4c76a1fe678e3b1c1263d:255078b58fa4f01057bf5568d55f5b2b63d0b8de</hashlist><modexp>317</modexp><mod>1851880057</mod></b>
<b><size>40</size><hashlist>38925f2bf8b95ab371933c07c7aa053b:baecfb418ea6c44c970454247da56a2a85bdd6fc</hashlist><modexp>317</modexp><mod>544499813</mod></b>
<b><size>40</size><hashlist>4def53e92581113421ae20a47ecd45a1:15e7e4c7daef5fd91c71ac9844e95e3a8f144b27</hashlist><modexp>317</modexp><mod>1634624544</mod></b>
<b><size>40</size><hashlist>5ed3a775ad27c883fcbd8471cc39aac0:6f8ddd4c76be6ff44e69c590b427f1d3648ea36d</hashlist><modexp>318</modexp><mod>1684108389</mod></b>
<b><size>40</size><hashlist>9f3caadd45eb733501fcd04a43458bfa:716c95ab525c13fd59b2bac483d1ed93221489fe</hashlist><modexp>317</modexp><mod>1430867012</mod></b>
<b><size>40</size><hashlist>fe4d60885d5e7387dd0a3dcd0c9d6f3c:0dcebe01bb9b501a2bb401aebaae95a603c0c89b</hashlist><modexp>318</modexp><mod>1919508846</mod></b>
<b><size>40</size><hashlist>1975f015a73caf96dc11b73eb60cb4e8:1ddb89b8edccc0be007602fea9eed3af5f96a73c</hashlist><modexp>317</modexp><mod>1598310738</mod></b>
<b><size>40</size><hashlist>a2eb9495034ea231fe0aa4329e77a952:65004e18cef38a033336c09faa8c64723d44a9b1</hashlist><modexp>317</modexp><mod>542336372</mod></b>
<b><size>40</size><hashlist>66e6ebcb8883924ed68fa3a8b325bf7d:6e7d6afdc03e243f827b2e4c2c33e099b177a3b4</hashlist><modexp>318</modexp><mod>1751478885</mod></b>
<b><size>40</size><hashlist>a44771da23e3471800386005f29ce873:4363afab384d225ec553818b14a812c6187d202e</hashlist><modexp>317</modexp><mod>1179402567</mod></b>
<b><size>40</size><hashlist>8585982fbfd642c103f06daf60cef152:644ff6d5554ebc551b6b890c71bab35cfaa8a07a</hashlist><modexp>318</modexp><mod>1848601956</mod></b>
<b><size>40</size><hashlist>58bda4988c3cf4f818f10cc617ee39b2:88292dbf48ed93152e438656bdebf9871f109bc9</hashlist><modexp>318</modexp><mod>1953002850</mod></b>
<b><size>40</size><hashlist>2e15d3915184e7bf81b0611f2f87d630:8b250fe69e7b9567ce6fa7e0cde5680132acf3cf</hashlist><modexp>317</modexp><mod>1416195429</mod></b>
<b><size>40</size><hashlist>30f0b49c43f9d7dff8dd4bae6beeb530:e47b0be48d3cdb9e902b809e4ec9dc32c1dd3e7a</hashlist><modexp>317</modexp><mod>2067944777</mod></b>
<b><size>40</size><hashlist>21641e406f71a0b96aa19aae38d95b2f:649d84ecf54480c8ae424b58d893410504060b13</hashlist><modexp>318</modexp><mod>612057685</mod></b>
<b><size>40</size><hashlist>11901b1a6f8d3b7ab44253391c28d029:e7e7e95362dab696eb72f99bb9e245173109e2ac</hashlist><modexp>318</modexp><mod>1768711200</mod></b>
<b><size>40</size><hashlist>9ea9af485765de616f892ac6e1c11410:0cfdd0f02a0d4383f47a49d501b302b7c164c16f</hashlist><modexp>317</modexp><mod>1397836846</mod></b>
<b><size>40</size><hashlist>5bda4dcf0c7cee7070e85f18cb809b6d:f3d19c8b8805cc11497c8e822bf48b7daf52ba28</hashlist><modexp>318</modexp><mod>544040046</mod></b>
<b><size>40</size><hashlist>a6a4018580f6d64ba58aa3681d01267d:1a8042eb9157fd5bf61c755b128220110ce72582</hashlist><modexp>318</modexp><mod>1684173413</mod></b>
<b><size>40</size><hashlist>f475f42ca96939fa6d30e6ad9be57928:f2d1542171ab4d7b62768eae8502a02919b761ff</hashlist><modexp>318</modexp><mod>1853060193</mod></b>
<b><size>40</size><hashlist>4f74a362d756d2c8678aca6c57554df5:8390d7ce88d18197d82ec790e0a96efc4544e22e</hashlist><modexp>318</modexp><mod>1397836846</mod></b>
<b><size>40</size><hashlist>f01287cbed23a6af5369bcb71adffd7e:1d0f8ae4c93a7427fd7098de7dd5d549e671f290</hashlist><modexp>318</modexp><mod>1699959116</mod></b>
<b><size>40</size><hashlist>91f3d6c68614d06c9d06e53ccaa5c14f:d833c9a428d8b454419688d70eaa2973839d0b73</hashlist><modexp>315</modexp><mod>1697539951</mod></b>
<b><size>40</size><hashlist>fc08e24fc83c60be0e67231f3bd32a47:e2253988d2cfa3982dd815572d247ee5bf3c3fa3</hashlist><modexp>315</modexp><mod>1862935305</mod></b>
<b><size>40</size><hashlist>eda11ade4af4c1814dadac5e1dd7c2d7:8e5c5484eca0d6419c4b2d9b21d3ee7b2a102ff2</hashlist><modexp>318</modexp><mod>1668244581</mod></b>
<b><size>40</size><hashlist>1df1a938172eabbc7a1ee64c405cb3d9:b2d49643bcb7b5fdace8a7b3f7de2ab526d81b1c</hashlist><modexp>317</modexp><mod>1684368995</mod></b>
<b><size>40</size><hashlist>a1f922a9b454cd0de214f8a78504da7c:4cfde2d4bc360936e51b74d09c788db9386b76ec</hashlist><modexp>318</modexp><mod>1930061669</mod></b>
<b><size>40</size><hashlist>a1544ddb840e5d34bd6454d0c45dc276:ef96b8c803ae4d7ababb3910204c9f42ee869d6c</hashlist><modexp>318</modexp><mod>1768318255</mod></b>
<b><size>40</size><hashlist>83f24f81c758a50073c7dc934c8b8be2:2b9ed66b94e1e8d9982cffc9defdd9d511d9a4c3</hashlist><modexp>318</modexp><mod>1865249649</mod></b>
<b><size>40</size><hashlist>9c2b713afcd930a122c1ebf800be25ec:91f827eb85262bbba58903c59198d3c41e770daf</hashlist><modexp>318</modexp><mod>1747938677</mod></b>
<b><size>40</size><hashlist>f46f97e0bbc9f5ce6c20be10355cb469:b10961e20e9d2e1db42c1eec5673b7cf1a97d6b5</hashlist><modexp>318</modexp><mod>796027248</mod></b>
<b><size>40</size><hashlist>37c08392f161c86b84356474c260fe4f:4589e6860c9f25c280a1b38c5b305cb4a3678703</hashlist><modexp>318</modexp><mod>1936289896</mod></b>
<b><size>40</size><hashlist>6d7c24fcac4489298484a6067e0f874d:c7e3b8504c9a9d9b4bf65474597ed37ea97a0590</hashlist><modexp>318</modexp><mod>168388463</mod></b>
<b><size>40</size><hashlist>0e20dcca42982f58b40d09d8db3e6e07:a1b0db9f6e60d2265705ad8db1ab18e04957eada</hashlist><modexp>317</modexp><mod>1702109229</mod></b>
<b><size>40</size><hashlist>2e53ec3a0470f4552664e184c0d561a0:aac70169f8a235ab9b01f543082814d286673d14</hashlist><modexp>318</modexp><mod>1949960713</mod></b>
<b><size>40</size><hashlist>797e7c9d041c629e1ef0a54ea737cc66:b156eabfe6d455e4b5e63b21b76009f17fde942c</hashlist><modexp>318</modexp><mod>1684368995</mod></b>
<b><size>40</size><hashlist>7a260f932d7d2c59f276ffff0b83d3e4:c0274903d8432b0c063867f89aae540d88cd167a</hashlist><modexp>318</modexp><mod>1868850530</mod></b>
<b><size>40</size><hashlist>05aaf28a3bf3ed6b9b8afe09f5372145:9ccd573dfe1cf87d3d3849d48599d6c25cfa9367</hashlist><modexp>318</modexp><mod>1846151538</mod></b>
<b><size>38</size><hashlist>20ae9e8ab2b3422153495cc81afdcbc7:b8b91ef0604d76bfc1941c8525a42f940667a45d</hashlist><modexp>302</modexp><mod>1819025930</mod></b>
</md>

# License 

https://github.com/singularian/mdencode/blob/master/LICENSE
