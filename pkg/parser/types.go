package parser

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
	kind TokenKind
	text TokenText
}

type ParseMode = int

const (
	Normal     ParseMode = iota
	Apostrophe           // '
	Quotation            // "
)

var Operators = map[string]TokenKind{
	"|":  Pipe,
	"&":  Detach,
	"&&": And,
	"||": Or,
	";":  Semicolon,
}
