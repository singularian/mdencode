FROM golang:1.14.1 AS builder

# RUN mkdir /app 
# ADD . /app/
# WORKDIR /app 
# RUN go build -o main .
# RUN adduser -S -D -H -h /app appuser
# USER appuser

ENV GOBIN /go/bin

WORKDIR /go/src/app/

RUN mkdir mdencode

WORKDIR /go/src/app/mdencode

# Copy go mod and sum files
COPY go.mod go.sum ./

# Download all dependencies. Dependencies will be cached if the go.mod and go.sum files are not changed
RUN go mod download

# Copy the source from the current directory to the Working Directory inside the container
COPY . .

RUN ls -lha /go/src/app/mdencode

WORKDIR /go/src/app/mdencode/build

RUN make mdencode
RUN make mdzip

