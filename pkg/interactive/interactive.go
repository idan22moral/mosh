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

func ReadLine(reader *bufio.Reader) (text []byte, err error) {
	text, err = reader.ReadBytes('\n')

	if err != nil {
		return nil, err
	}

	text = text[:len(text)-1]

	return text, err
}

func Loop() {
	stdin := bufio.NewReader(os.Stdin)

	for {
		PrintPrompt()

		currentInput, err := ReadLine(stdin)

		if err != nil {
			fmt.Println(err)
			continue
		}

		_, err = parser.Parse(currentInput)
		if err != nil {
			fmt.Println(err)
		}
	}
}
