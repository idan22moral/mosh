package parser

import (
	"errors"

	objects "github.com/idan22moral/v2/pkg/parser/objects"
)

type AST interface {
	Execute() (int, error)
	String() string
	Seal()
	Sealed() bool
}

func BuildAST(tokens []Token) ([]AST, error) {
	var ASTList []AST
	var currentToken Token
	var cmd *objects.MoshCommand
	var pipe *objects.MoshPipe

	for _, v := range tokens {
		currentToken = v

		switch currentToken.Kind {
		case Argument:
			if cmd != nil {
				cmd.AddArgument(string(currentToken.Text))
			} else {
				return nil, errors.New("cannot add argument to a command that was not allocated")
			}

		case Command:
			cmd = objects.NewMoshCommand(string(currentToken.Text), nil)
			if pipe != nil && pipe.Right == nil {
				pipe.Right = cmd
			} else {
				ASTList = append(ASTList, cmd)
			}

		case Pipe:
			// make sure there's something to pipe to
			if len(ASTList) > 0 {
				if !ASTList[len(ASTList)-1].Sealed() {
					mc, ok := ASTList[len(ASTList)-1].(*objects.MoshCommand)
					if !ok {
						return nil, errors.New("pipe child must be a *MoshCommand")
					}
					pipe = objects.NewMoshPipe(mc, nil)
					ASTList[len(ASTList)-1] = pipe
				} else {
					return nil, errors.New("command was not specified before a pipe")
				}
			} else {
				return nil, errors.New("command was not specified before a pipe")
			}

		case Semicolon: // end of tree
			if len(ASTList) > 0 {
				ASTList[len(ASTList)-1].Seal()
			} else {
				return nil, errors.New("unexpected token `;`")
			}

		case Or, Detach, And:
			return nil, errors.New("operator \"" + string(currentToken.Text) + "\" is supported yet")

		case Undefined:
			return nil, errors.New("type of \"" + string(currentToken.Text) + "\" is undefined")

		default:
			return nil, errors.New("unknown operator \"" + string(currentToken.Text) + "\" while creating AST")
		}
	}
	return ASTList, nil
}
