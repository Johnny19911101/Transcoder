package main

import "net/http"

func main() {
	http.Handle("/", http.FileServer(http.Dir("./")))
	error := http.ListenAndServe(":9000", nil)
	if error != nil {
		panic(error)
	}
}
