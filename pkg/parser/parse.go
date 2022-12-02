package parser

import (
	"fmt"

	"github.com/idan22moral/v2/internal"
)

func Parse(input []byte) ([]AST, error) {
	rawTokens, err := Tokenize(input)

	if err != nil {
		return nil, err
	}

	if internal.DebugMode {
		fmt.Printf("rawTokens: %v\n", rawTokens)
	}

	labeledTokens, err := Label(rawTokens)

	if err != nil {
		return nil, err
	}

	if internal.DebugMode {
		fmt.Printf("labeledTokens: %v\n", labeledTokens)
	}

	ast, err := BuildAST(labeledTokens)

	if err != nil {
		return nil, err
	}

	if internal.DebugMode {
		fmt.Println("AST:")
		for i, v := range ast {
			fmt.Printf("\t%d: %s\n", i, v)
		}
	}

	return ast, nil
}
