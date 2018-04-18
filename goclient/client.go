package main

import (
	"log"
	"time"

	pb "Transcoder"

	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

const (
	address     = "localhost:50053"
	defaultName = "world"
)

func main() {
	// Set up a connection to the server.
	conn, err := grpc.Dial(address, grpc.WithInsecure())
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	defer conn.Close()
	c := pb.NewTranscodeClient(conn)

	// Contact the server and print out its response.

	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()
	r, err := c.Setchannel(ctx, &pb.PIDS{Video: 256, Audio: 257, Filename: "test"})
	if err != nil {
		log.Fatalf("could not greet: %v", err)
	}
	log.Printf("Greeting: %s", r.List)
	y, err := c.Avio(ctx, &pb.AvioType{Inputtype: 1, File: "/home/johnny/tiral_go.ts"})
	if err != nil {
		log.Fatalf("could not greet: %v", err)
	}
	log.Printf("State: %s", y.Reply)
	x, err := c.Process(ctx, &pb.Empty{})
	if err != nil {
		log.Fatalf("could not greet: %v", err)
	}
	log.Printf("Greeting: %s", x.Reply)

}
