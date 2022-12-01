package parser

import "fmt"

func Parse(input []byte) error {
	rawTokens, err := Tokenize(input)

	if err != nil {
		return err
	}

	fmt.Printf("rawTokens: %v\n", rawTokens)

	labeledTokens, err := Label(rawTokens)

	if err != nil {
		return err
	}

	fmt.Printf("labeledTokens: %v\n", labeledTokens)

	return nil
}
