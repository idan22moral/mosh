package parser

import (
	"errors"
)

type TokenText = []byte
type TokenKind = int

const (
	Undefined TokenKind = iota
	Command
	Argument
	Pipe      // |
	Detach    // &
	And       // &&
	Or        // ||
	Semicolon // ;
)

type Token struct {
	Kind TokenKind
	Text TokenText
}

type TokenizerMode = int

const (
	Normal     TokenizerMode = iota
	Apostrophe               // '
	Quotation                // "
)

func Tokenize(input []byte) (rawTokens []TokenText, err error) {
	var mode TokenizerMode
	var currentToken []byte

	saveCurrentToken := func() {
		if len(currentToken) == 0 {
			return
		}

		rawTokens = append(rawTokens, currentToken)
		currentToken = nil
	}

	for i := 0; i < len(input); i++ {
		v := input[i]
		switch mode {
		case Normal:
			if v == '\'' { // start of ' literal
				mode = Apostrophe
			} else if v == '"' { // start of " literal
				mode = Quotation
			} else if v == ' ' || v == '\t' { // end of token
				saveCurrentToken()
			} else if isOperator(v) {
				saveCurrentToken()

				// the current and next chars are operators
				if i+1 < len(input) && isOperator(input[i+1]) {
					potentialOperator := input[i : i+2]
					// check if the current and next chars form an operator (e.g. '|' + '|' = "||")
					if isOperator(potentialOperator) {
						rawTokens = append(rawTokens, potentialOperator)
						i++
						break // force switch ending

					} else {
						// invalid operator (e.g. "|&")
						return nil, errors.New("invalid operator - " + string(potentialOperator))
					}
				} else {
					rawTokens = append(rawTokens, []byte{v})
				}
			} else {
				currentToken = append(currentToken, v)
			}

		case Apostrophe:
			if v == '\'' { // end of ' literal
				mode = Normal
			} else {
				currentToken = append(currentToken, v)
			}

		case Quotation:
			if v == '"' { // end of " literal
				mode = Normal
			} else {
				currentToken = append(currentToken, v)
			}
		}
	}

	// Save the token if it's not empty
	if len(currentToken) != 0 {
		saveCurrentToken()
	}

	return rawTokens, nil
}
