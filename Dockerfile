FROM golang:1.15 AS builder

# optional create user
# RUN mkdir /app 
# ADD . /app/
# WORKDIR /app 
# RUN go build -o main .
# RUN adduser -S -D -H -h /app appuser
# USER appuser

# install openssl and libgmp
RUN apt update && apt-get install -y \
    openssl \
    libssl-dev \
    libgmp3-dev \
&& rm -rf /var/lib/apt/lists/*

# setup the GOBIN environment
ENV GOBIN /go/bin
ENV PATH="${GOBIN}:${PATH}"
RUN echo "export PATH=$PATH" > /etc/environment

# set the work directory
WORKDIR /go/src/app/

# make mdencode
RUN mkdir mdencode

# change to the app directory
WORKDIR /go/src/app/mdencode

# Copy go mod and sum files
COPY go.mod go.sum ./

# Download all dependencies. Dependencies will be cached if the go.mod and go.sum files are not changed
RUN go mod download

# Copy the source from the current directory to the Working Directory inside the container
COPY . .

# check the mdencode source directory
RUN ls -lha /go/src/app/mdencode

# change to the build /go/src/app/mdencode/code
# WORKDIR /go/src/app/mdencode/code/testdecode_cpp/external/fnv
# RUN make install
# RUN make

# change to the build directory
WORKDIR /go/src/app/mdencode/build

# run the make build for mdencode and mdzip
RUN make mdencode
RUN make mdzip
RUN make buildcpp

# check the mdencode and mdzip binary execs
RUN mdsig
RUN mdencode
# check the mdzip go binary execs
RUN mdzip
RUN mdunzip

# check the cpp binary
RUN ls -lha
RUN /go/src/app/mdencode/build/mdzipcpp
RUN /go/src/app/mdencode/build/mdunzipcpp
RUN /go/src/app/mdencode/build/decoderRandomTestHC2
