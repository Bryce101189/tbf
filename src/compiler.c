#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

bool compile_program(char* program, unsigned int length, char* output_path) {
    // Check bracket count
    int brackets = 0;
    for(int i = 0; i < length; ++i) {
        if(program[i] == '[') {
            ++brackets;
        } else if(program[i] == ']') {
            --brackets;

            if(brackets < 0) {
                fprintf(stderr, "ERROR: Uneven number of brackets\n");
                return false;
            }
        }
    }

    if(brackets != 0) {
        fprintf(stderr, "ERROR: Uneven number of brackets\n");
        return false;
    }

    // Compile program
    char* new_output_path = malloc(strlen(output_path) + 3);
    if(new_output_path == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new output path buffer\n");
        return false;
    }

    sprintf(new_output_path, "%s.c", output_path);

    FILE* output = fopen(new_output_path, "w");
    if(output == NULL) {
        fprintf(stderr, "ERROR: Failed to create file '%s' for writing\n", output_path);
        return 1;
    }

    fprintf(
        output,
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "\n"
        "#define DEFAULT_STACK_SIZE 0x400\n"
        "#define STACK_GROWTH_SIZE 0x100\n"
        "\n"
        "char* stack = NULL;\n"
        "size_t stack_size = DEFAULT_STACK_SIZE;\n"
        "unsigned int stack_pointer = 0;\n"
        "\n"
        "void grow_stack(void) {\n"
        "    stack_size += STACK_GROWTH_SIZE;\n"
        "    stack = realloc(stack, stack_size);\n"
        "    if(stack == NULL) {\n"
        "        fprintf(stderr, \"ERROR: Failed to allocate memory for program stack\\n\");\n"
        "        free(stack);\n"
        "        exit(EXIT_FAILURE);\n"
        "    }\n"
        "\n"
        "    stack = memset(stack + stack_pointer, 0, STACK_GROWTH_SIZE);\n"
        "    if(stack == NULL) {\n"
        "        fprintf(stderr, \"ERROR: Failed to clear newly allocated memory in program stack\\n\");\n"
        "        free(stack);\n"
        "        exit(EXIT_FAILURE);\n"
        "    }\n"
        "}\n"
        "\n"
        "int main(void) {\n"
        "    stack = calloc(1, DEFAULT_STACK_SIZE);\n"
        "    if(stack == NULL) {\n"
        "        fprintf(stderr, \"ERROR: Failed to allocate memory for program stack\\n\");\n"
        "        return 1;\n"
        "    }\n"
        "\n"
    );

    for(int i = 0; i < length; ++i) {
        switch(program[i])
        {
        case '+':
            fprintf(
                output,
                "    ++stack[stack_pointer];\n"
            );
            break;
        
        case '-':
            fprintf(
                output,
                "    --stack[stack_pointer];\n"
            );
            break;

        case '<':
            fprintf(
                output,
                "    if(stack_pointer == 0) {\n"
                "        fprintf(stderr, \"ERROR: Stack underflow\\n\");\n"
                "        free(stack);\n"
                "        return 1;\n"
                "    }\n"
                "    --stack_pointer;\n"
            );
            break;

        case '>':
            fprintf(
                output,
                "    ++stack_pointer;\n"
                "    if(stack_pointer >= stack_size) {\n"
                "        grow_stack();\n"
                "    }\n"
            );
            break;
        
        case ',':
            fprintf(
                output,
                "    stack[stack_pointer] = getchar();\n"
            );
            break;
        
        case '.':
            fprintf(
                output,
                "    putchar(stack[stack_pointer]);\n"
            );
            break;
        
        case '[':
            fprintf(
                output,
                "\n"
                "    while(stack[stack_pointer] != 0)\n"
                "    {\n"
                "\n"
            );
            break;

        case ']':
            fprintf(
                output,
                "\n"
                "    }\n"
                "\n"
            );
            break;

        default:
            break;
        }
    }

    fprintf(
        output,
        "\n"
        "    free(stack);\n"
        "    return 0;\n"
        "}\n"
    );

    fclose(output);

    char command[1024];

#ifdef _WIN32
    sprintf(command, "cc %s -Ofast -o %s.exe", new_output_path, output_path);
#else
    sprintf(command, "cc %s -Ofast -o %s", new_output_path, output_path);
#endif // _WIN32
    
    fprintf(stderr, "%s\n", command);

    if(system(command) != 0) {
        fprintf(stderr, "ERROR: Application returned non-zero status code on execution\n");
        return false;
    }

    return true;
}