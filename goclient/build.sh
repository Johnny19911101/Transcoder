mkdir -p ${GOPATH}/src/Transcoder
protoc -I../code/src/ transcode_rpc.proto --go_out=plugins=grpc:${GOPATH}/src/Transcoder
rm -rf bin
mkdir -p bin
cd bin
go build ../client.go
