# tbf - Tiny Brainfuck interpreter, compiler, and REPL

tbf is a tiny [Brainfuck](https://esolangs.org/wiki/Brainfuck) interpreter, compiler, and REPL written in C

#### Important note...

This project was made for fun and not with the intention of being expanded, improved or actively maintained. Taking even a quick glance at the source code, you may notice that some source files look much dirtier than they probably have to be...

If you do happen to find a bug in this program you can report it using the GitHub issue tracker, but please don't expect me to fix it.

### Usage

Building tbf from source using the GNU make utility

```bash
make
```

Installing the program locally to your computer

```bash
make install
```

Running the interactive REPL

```bash
tbf
```

Interpreting a Brainfuck program

```bash
tbf --run /path/to/program.bf
```

Compiling and running a Brainfuck program

```bash
tbf --compile /path/to/program.bf --output program

# On Windows systems '.\program.exe' should be run instead
./program
```

### Dependencies

tbf depends on a C compiler being installed on your system, and assumes that it can be invoked from the command line as `cc`.

### Contributing

PRs are welcome. Please be sure to adhere to the general style I used when writing tbf to maintain consistency and readability across source files.

### License

tbf is MIT licensed. This license can be viewed [here](LICENSE).