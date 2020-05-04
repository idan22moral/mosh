# mosh
A simple implementation of an interactive shell.

## Installation
Simply clone the repository and `make` it.
The executable will be at the `build` directory.
```sh
git clone https://github.com/idan22moral/mosh
cd mosh
make
```
## Usage
```sh
cd build
# run regularly
./mosh
# run in debug mode
./mosh -d
```

## Command List
* Every executable on your machine can be run using mosh.
* You can use `boop` to exit mosh.


## Examples
```sh
$ echo "abc" | xxd
00000000: 6162 630a                                abc.
$ cd /home/idan/Desktop
$ cat secret.txt
Keep it between us: mosh = Moral Shell.
$ cd /
$ pwd
/
$ cd -
/home/idan/Desktop
$ boop
```

## Notes
* Redirections to files (like the operator `>`) are not supported.
* Parentheses and curly brackets (subshells, basically) are not supported.
* Comments are not supported.
* Some of those features are left commented in the code for future changes.
