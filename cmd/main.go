package main

import "github.com/idan22moral/v2/pkg/interactive"

func main() {
	interactive.LoadCommandLineArgs()
	interactive.Loop()
}
