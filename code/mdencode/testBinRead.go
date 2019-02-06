package main

import (
	"fmt"
	"encoding/binary"
	"os"
	"bufio"
	"bytes"
)


func main() {

	var filename = "default.mdbin"
	file, _ := os.Open(filename)

	// if err != nil {
	//    return nil, err
	//}
	defer file.Close()

	stats, _ := file.Stat()
	var size int64 = stats.Size()
	bytes := make([]byte, size)

	bufr := bufio.NewReader(file)
	_,_ = bufr.Read(bytes)

	fmt.Println("buf ", bytes)
	fileSize,_      := Convert64(bytes[0:8])
	blockSize,_     := Convert64(bytes[8:16])
	modSize,_       := Convert64(bytes[16:24])
	fileNameLen,_   := Convert64(bytes[24:32])
	filePathLen,_   := Convert64(bytes[32:40])
	fileHashLen,_   := Convert64(bytes[40:48])

	fmt.Println("fileSize ", fileSize)
	fmt.Println("blockSize ", blockSize)
	fmt.Println("modSize ", modSize)
	fmt.Println("fileNameLength ", fileNameLen)
	fmt.Println("filePathLength ", filePathLen)
	fmt.Println("fileHashLength ", fileHashLen)

	// get the filename
	// /home/nsross/projects/src/github.com/singularian/mdencode/code/mdencode/
	var start uint64
	var end   uint64
	// start = filePathLen 
	// end = start + fileNameLen
	// fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	start = 48 
	end = start + filePathLen
	fmt.Println("filePath ", string(bytes[start:end]), start, end)
	// fileName
        start = end 
        end = start + fileNameLen
        fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	// get hashlist string
	start = end
	end = end + fileHashLen
	fmt.Println("hashlistname ", string(bytes[start:end]))

	// return bytes, err

}

func Convert(data []byte) (uint32, error) {
var v uint32
err := binary.Read(bytes.NewReader(data), binary.BigEndian, &v)
if err != nil {
return 0, err
}
return v, nil
}

func Convert64(data []byte) (uint64, error) {
	var v uint64
	err := binary.Read(bytes.NewReader(data), binary.BigEndian, &v)
	if err != nil  {
		return 0, err
	}
	return v, nil
}
