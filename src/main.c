#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "interpreter.h"

#define VERSION_NUMBER "0.0.1"

void display_help(void) {
    printf(
        "tfb - Tiny Brainfuck interpreter, compiler, and REPL [version %s]\n"
        "Suirabu <suirabu.dev@gmail.com>\n"
        "\n"
        "Usage:\n"
        "    Normal usage           tbf [options] <program>\n"
        "    Run REPL               tbf [options]\n"
        "\n"
        "Options:\n"
        "\n"
        "    --help, -h             Displays the help menu, then closes the application\n"
        "    --compile, -c          Compiles the input program into C, the compiles it with $cc\n"
        "    --run, -r              Runs the input program\n"
        "    --output, -o <path>    Specify the name of the outputted C file produced when compiling\n",
        VERSION_NUMBER
    );
}

typedef struct {
    char* program;
    char* output;

    bool help: 1;
    bool compile: 1;
    bool run: 1;
} Options;

int main(int argc, char* argv[]) {
    // Parse arguments
    Options options = {};
    bool parse_error = false;

    for(int i = 1; i < argc; ++i) {
        char* arg = argv[i];

        // Option
        if(arg[0] == '-' && arg[1] != '\0') {
            // Long option
            if(arg[1] == '-') {
                if(strcmp(arg, "--help") == 0) {
                    options.help = true;
                } else if(strcmp(arg, "--compile") == 0) {
                    options.compile = true;
                } else if(strcmp(arg, "--run") == 0) {
                    options.run = true;
                } else if(strcmp(arg, "--output") == 0) {
                    // Check for output path
                    if(i + 1 < argc) {
                        char* next = argv[++i];

                        // Report an error if 'next' is an option
                        if(next[0] == '-') {
                            fprintf(stderr, "ERROR: Expected path after option '--output', found option '%s' instead\n", next);
                            parse_error = true;
                        } else {
                            options.output = next;
                        }
                    } else {
                        fprintf(stderr, "ERROR: Expected path after option '--output', found end-of-line instead\n");
                        parse_error = true;
                    }
                } else {
                    fprintf(stderr, "ERROR: Unknown option '%s'\n", arg);
                    parse_error = true;
                }
            }
            // Short option(s)
            else {
                size_t arg_len = strlen(arg);
                for(int j = 1; j < arg_len; ++j) {
                    switch(arg[j]) {
                    case 'h':
                        options.help = true;
                        break;
                    case 'c':
                        options.compile = true;
                        break;
                    case 'r':
                        options.run = true;
                        break;
                    case 'o':
                        // Check for output path
                        if(i + 1 < argc) {
                            char* next = argv[++i];

                            // Report an error if 'next' is an option
                            if(next[0] == '-') {
                                fprintf(stderr, "ERROR: Expected path after option '--output', found option '%s' instead\n", next);
                                parse_error = true;
                            } else {
                                options.output = next;
                            }
                        } else {
                            fprintf(stderr, "ERROR: Expected path after option '--output', found end-of-line instead\n");
                            parse_error = true;
                        }
                        break;

                    default:
                        fprintf(stderr, "ERROR: Unknown option '-%c'\n", arg[j]);
                        parse_error = true;
                    }
                }
            }
        }
        // Positional
        else {
            // This program only needs to accept one positional argument, the program path.
            // Because of this the first positional argument is assumed to be this path, and
            // all other positional arguments are treated as errors
            if(options.program == NULL) {
                options.program = arg;
            } else {
                fprintf(stderr, "ERROR: Unused positional argument '%s'\n", arg);
                parse_error = true;
            }
        }
    }

    if(parse_error) {
        return 1;
    }

    if(options.help) {
        display_help();
        return 0; 
    }

    // Run REPL
    if(!options.compile && !options.run) {
        // TODO: Implement REPL
        fprintf(stderr, "ERROR: REPL has not yet been implemented\n");
        return 1;
    }
    
    if(options.program == NULL) {
        fprintf(stderr, "ERROR: 'program' positional not provided\n");
        return 1;
    }

    // Attempt to open program file and read it's contents into buffer
    FILE* program = fopen(options.program, "r");
    if(program == NULL) {
        fprintf(stderr, "ERROR: Failed to open file '%s' for reading\n", options.program);
        return 1;
    }

    fseek(program, 0, SEEK_END);
    size_t program_length = ftell(program);
    rewind(program);

    char* program_buffer = malloc(program_length + 1);
    if(program_buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for program buffer\n");
        fclose(program);
        return 1;
    }

    if(fread(program_buffer, 1, program_length, program) != program_length) {
        fprintf(stderr, "ERROR: Failed to read file contents into program buffer\n");
        fclose(program);
        free(program_buffer);
        return 1;
    }
    fclose(program);
    program_buffer[program_length] = '\0';

    // Compile program
    if(options.compile) {
        // TODO: Implement compiler
        fprintf(stderr, "ERROR: Compiler has not yet been implemented\n");
        return 1;
    }
    // Interpret program
    else if(options.run) {
        if(!init_interpreter() || !run_program(program_buffer, program_length)) {
            free(program_buffer);
            return 1;
        }
        destroy_interpreter();
    }

    free(program_buffer);

    return 0;
}

