package parser

import "errors"

var Operators = map[string]TokenKind{
	"|":  Pipe,
	"&":  Detach,
	"&&": And,
	"||": Or,
	";":  Semicolon,
}

func isOperator[T string | []byte | byte](op T) bool {
	_, ok := Operators[string(op)]
	return ok
}

func Label(rawTokens []TokenText) (labeledTokens []Token, err error) {
	var currentToken Token
	operatorKind := Undefined
	prevKind := Undefined

	for _, token := range rawTokens {
		currentToken = Token{Text: token}

		if isOperator(token) {
			operatorKind = Operators[string(token)]
			switch operatorKind {
			case Pipe, Detach, And, Or, Semicolon:
				if prevKind == Command || prevKind == Argument {
					currentToken.Kind = operatorKind
					prevKind = operatorKind
				} else {
					return nil, errors.New("invalid operator location")
				}

			default:
				return nil, errors.New("unknown operator \"" + string(token) + "\" while creating labeling token")
			}
		} else {
			switch prevKind {
			case Command, Argument:
				currentToken.Kind = Argument
				prevKind = Argument

			case Pipe, Detach, And, Or, Semicolon, Undefined:
				// must be a command
				currentToken.Kind = Command
				prevKind = Command
			}
		}

		labeledTokens = append(labeledTokens, currentToken)
	}

	return labeledTokens, nil

}
