package parser

import (
	"fmt"

	"github.com/idan22moral/v2/internal"
)

func Parse(input []byte) error {
	rawTokens, err := Tokenize(input)

	if err != nil {
		return err
	}

	if internal.DebugMode {
		fmt.Printf("rawTokens: %v\n", rawTokens)
	}

	labeledTokens, err := Label(rawTokens)

	if err != nil {
		return err
	}

	if internal.DebugMode {
		fmt.Printf("labeledTokens: %v\n", labeledTokens)
	}

	return nil
}
