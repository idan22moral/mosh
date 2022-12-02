package interactive

import (
	"bufio"
	"fmt"
	"os"

	"github.com/idan22moral/v2/internal"
	"github.com/idan22moral/v2/pkg/parser"
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
	stdin := bufio.NewReader(os.Stdin)

	for {
		PrintPrompt()

		currentInput, err := stdin.ReadBytes('\n')

		if err != nil {
			fmt.Println(err)
			return
		}

		currentInput = currentInput[:len(currentInput)-1]
		_, err = parser.Parse(currentInput)
		if err != nil {
			fmt.Println(err)
		}
	}
}
