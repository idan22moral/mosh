package parser

import (
	"fmt"
	"io"
	"os"
)

type MoshPipe struct {
	sealed bool
	Right  *MoshCommand
	Left   *MoshCommand
}

func (p *MoshPipe) Execute() (int, error) {
	err := p.Left.Prepare()
	if err != nil {
		return -1, err
	}

	err = p.Right.Prepare()
	if err != nil {
		return -1, err
	}

	/*
		Writer's stdout (leftStdout) should be piped to reader's stdin (rightStdin).

					writer | reader
				.--------^   ^    ^---------------.
			leftStdout	     |         rightStdout,rightStderr
					     rightStdin
	*/

	leftStdout, err := p.Left.Command.StdoutPipe()
	if err != nil {
		return -1, err
	}
	rightStdin, err := p.Right.Command.StdinPipe()
	if err != nil {
		return -1, err
	}

	// Redirect rightStdout and rightStderr to the parent's corresponding streams to get the output
	rightStdout, err := p.Right.Command.StdoutPipe()
	if err != nil {
		return -1, err
	}
	rightStderr, err := p.Right.Command.StderrPipe()
	if err != nil {
		return -1, err
	}

	p.Left.Execute()
	p.Right.Execute()

	io.Copy(rightStdin, leftStdout)
	io.Copy(os.Stdout, rightStdout)
	io.Copy(os.Stderr, rightStderr)

	_, err = p.Left.Wait()
	if err != nil {
		return -1, err
	}

	exitCodeRight, err := p.Right.Wait()
	if err != nil {
		return -1, err
	}

	return exitCodeRight, nil
}

func (p *MoshPipe) String() string {
	return fmt.Sprintf("%#v", p)
}

func (p *MoshPipe) Seal() {
	p.sealed = true
}

func (p *MoshPipe) Sealed() bool {
	return p.sealed
}

func NewMoshPipe(left *MoshCommand, right *MoshCommand) *MoshPipe {
	return &MoshPipe{
		Right:  right,
		Left:   left,
		sealed: false,
	}
}
