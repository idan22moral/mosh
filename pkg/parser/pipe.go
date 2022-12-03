package parser

import (
	"fmt"
	"io"
	"os"
)

type PipeableAST interface {
	AST
	StdinPipe() (io.WriteCloser, error)
	StdoutPipe() (io.ReadCloser, error)
	StderrPipe() (io.ReadCloser, error)
	Wait() (int, error)
}

type MoshPipe struct {
	sealed bool
	Right  PipeableAST
	Left   PipeableAST
}

func (p *MoshPipe) Execute() (int, error) {
	/*
		Writer's stdout (leftStdout) should be piped to reader's stdin (rightStdin).

					writer | reader
				.--------^   ^    ^---------------.
			leftStdout	     |         rightStdout,rightStderr
					     rightStdin
	*/

	leftStdout, err := p.Left.StdoutPipe()
	if err != nil {
		return -1, err
	}
	rightStdin, err := p.Right.StdinPipe()
	if err != nil {
		return -1, err
	}

	// Redirect rightStdout and rightStderr to the parent's corresponding streams to get the output
	rightStdout, err := p.Right.StdoutPipe()
	if err != nil {
		return -1, err
	}
	rightStderr, err := p.Right.StderrPipe()
	if err != nil {
		return -1, err
	}

	p.Left.Execute()
	p.Right.Execute()

	io.Copy(rightStdin, leftStdout)
	io.Copy(os.Stdout, rightStdout)
	io.Copy(os.Stderr, rightStderr)

	return p.Wait()
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

func (p *MoshPipe) Wait() (int, error) {
	_, err := p.Left.Wait()
	if err != nil {
		return -1, err
	}

	exitCodeRight, err := p.Right.Wait()
	if err != nil {
		return -1, err
	}

	return exitCodeRight, nil
}

func (c *MoshPipe) StdinPipe() (io.WriteCloser, error) {
	wc, err := c.Left.StdinPipe()
	return wc, err
}

func (c *MoshPipe) StdoutPipe() (io.ReadCloser, error) {
	rc, err := c.Right.StdoutPipe()
	return rc, err
}

func (c *MoshPipe) StderrPipe() (io.ReadCloser, error) {
	rc, err := c.Right.StderrPipe()
	return rc, err
}

func NewMoshPipe(left PipeableAST, right PipeableAST) *MoshPipe {
	return &MoshPipe{
		Right:  right,
		Left:   left,
		sealed: false,
	}
}
