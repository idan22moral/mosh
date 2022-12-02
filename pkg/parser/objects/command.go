package parser

import (
	"errors"
	"fmt"
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

func (c *MoshCommand) Prepare() error {
	c.Resolve()

	if c.builtin {
		return errors.New("builtins not implemented")
	}

	c.Command = exec.Command(c.commandText, c.args...)

	return nil
}

func (c *MoshCommand) Execute() (int, error) {
	if c.Command == nil {
		err := c.Prepare()

		if err != nil {
			return -1, err
		}
	}

	err := c.Command.Start()

	return 0, err
}

func (c *MoshCommand) Wait() (int, error) {
	if c.Command == nil {
		return -1, errors.New("command not prepared / executed")
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

func (c *MoshCommand) SetCommand(command string) {
	c.commandText = command
}

func (c *MoshCommand) Resolve() {
	// not implemented yet
}

func NewMoshCommand(command string, args []string) *MoshCommand {
	var _args []string
	copy(args, _args)
	return &MoshCommand{
		commandText:     command,
		args:            _args,
		sealed:          false,
		originalCommand: command,
		builtin:         false,
	}
}
