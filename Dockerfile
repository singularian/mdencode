FROM golang:latest AS builder

# RUN mkdir /app 
# ADD . /app/
# WORKDIR /app 
# RUN go build -o main .
# RUN adduser -S -D -H -h /app appuser
# USER appuser

WORKDIR /go/src/app

# Copy go mod and sum files
COPY go.mod go.sum ./

# Download all dependencies. Dependencies will be cached if the go.mod and go.sum files are not changed
RUN go mod download

# Copy the source from the current directory to the Working Directory inside the container
COPY . .

CMD ["ls -alt /go/src/app"]
