package interactive

import (
	"bufio"
	"fmt"
	"os"

	"github.com/idan22moral/v2/internal"
)

func LoadCommandLineArgs() {
	args := os.Args[1:]
	for _, arg := range args {
		if arg == "-d" {
			internal.DebugMode = true
			return
		}
	}
}

func PrintPrompt() {
	fmt.Print("$ ")
}

func Loop() {
	var inputBuffer []byte
	reader := bufio.NewReader(os.Stdin)
	for {
		PrintPrompt()
		// fmt.Scanf("%c", currentInput)
		os.Stdin.Read()
		if err != nil {
			fmt.Println(err)
			return
		}

		if currentInput == '\t' {
			if internal.DebugMode {
				fmt.Println("tab entered")
			}
		} else if currentInput == '\n' {
			if internal.DebugMode {
				fmt.Println("newline entered")
			}
			fmt.Println("inputBuffer", inputBuffer)
			inputBuffer = nil

		} else {
			inputBuffer = append(inputBuffer, currentInput)
		}
	}
}
