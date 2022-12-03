package parser

import (
	"errors"
	"fmt"
	"io"
	"os/exec"
)

type MoshCommand struct {
	sealed          bool
	commandText     string
	originalCommand string
	args            []string
	builtin         bool
	Command         *exec.Cmd
}

func (c *MoshCommand) Execute() (int, error) {
	return 0, c.Command.Start()
}

func (c *MoshCommand) Wait() (int, error) {
	if c.Command == nil {
		return -1, errors.New("command not executed")
	}

	err := c.Command.Wait()
	if err != nil {
		return -1, err
	}

	return c.Command.ProcessState.ExitCode(), nil
}

func (c *MoshCommand) String() string {
	return fmt.Sprintf("%#v", c)
}

func (c *MoshCommand) Seal() {
	c.sealed = true
}

func (c *MoshCommand) Sealed() bool {
	return c.sealed
}

func (c *MoshCommand) Builtin() bool {
	return c.builtin
}

func (c *MoshCommand) OriginalCommand() string {
	return c.originalCommand
}

func (c *MoshCommand) AddArgument(arg string) {
	c.args = append(c.args, arg)
}

func (c *MoshCommand) StdinPipe() (io.WriteCloser, error) {
	wc, err := c.Command.StdinPipe()
	return wc, err
}

func (c *MoshCommand) StdoutPipe() (io.ReadCloser, error) {
	rc, err := c.Command.StdoutPipe()
	return rc, err
}

func (c *MoshCommand) StderrPipe() (io.ReadCloser, error) {
	rc, err := c.Command.StderrPipe()
	return rc, err
}

func (c *MoshCommand) SetCommand(command string) {
	c.commandText = command
}

func (c *MoshCommand) Resolve() {
	// not implemented yet
}

func NewMoshCommand(command string, args []string) *MoshCommand {
	var _args []string
	copy(args, _args)
	mc := &MoshCommand{
		commandText:     command,
		args:            _args,
		sealed:          false,
		originalCommand: command,
		builtin:         false,
		Command:         exec.Command(command, _args...),
	}

	mc.Resolve()

	return mc
}
