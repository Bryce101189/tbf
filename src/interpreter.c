#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

#define DEFAULT_STACK_SIZE 0x400
#define STACK_GROWTH_SIZE 0x100

unsigned char* stack = NULL;
size_t stack_size = DEFAULT_STACK_SIZE;
unsigned int stack_pointer = 0;

bool init_interpreter(void) {
    stack_size = DEFAULT_STACK_SIZE;
    
    if(stack == NULL) {
        stack = calloc(1, stack_size);
        if(stack == NULL) {
            fprintf(stderr, "ERROR: Failed to allocate memory for program stack\n");
            return false;
        }
    }

    stack_pointer = 0;
    return true;
}

void destroy_interpreter(void) {
    free(stack);
}

bool run_program(char* program, size_t length) {
    int line = 0;
    int column = 0;

    for(int i = 0; i < length; ++i) {
        ++column;

        switch(program[i]) {
            case '+':
                ++stack[stack_pointer];
                break;
            
            case '-':
                --stack[stack_pointer];
                break;
            
            case '>':
                ++stack_pointer;
                // Increase the length of our array
                if(stack_pointer == stack_size) {
                    stack_size += STACK_GROWTH_SIZE;
                    stack = realloc(stack, stack_size);
                    if(stack == NULL) {
                        fprintf(stderr, "ERROR: Failed to allocate memory for program stack\n");
                        return false;
                    }
                    
                    stack = memset(stack + stack_pointer, 0, STACK_GROWTH_SIZE);
                    if(stack == NULL) {
                        fprintf(stderr, "ERROR: Failed to clear newly allocated memory in program stack\n");
                        return false;
                    }
                }
                break;
            
            case '<':
                if(stack_pointer == 0) {
                    fprintf(stderr, "ERROR: Stack underflow\n");
                    return false;
                }
                --stack_pointer;
                break;
            
            case ',':
                stack[stack_pointer] = getchar();
                break;
            
            case '.':
                putchar(stack[stack_pointer]);
                break;
            
            case '[':
                if(stack[stack_pointer] == 0) {
                    int x = 1;
                    while(i + 1 < length && x != 0) {
                        ++i;
                        
                        if(program[i] == '[') {
                            ++x;
                        } else if(program[i] == ']') {
                            --x;
                        }
                    }

                    if(i >= length) {
                        fprintf(stderr, "ERROR: Encountered end-of-file while looking for matching closed-bracket for open-bracket at line %u, column %u\n", line, column);
                        return false;
                    }
                }
                break;
            
            case ']':
                if(stack[stack_pointer] != 0) {
                    int x = -1;
                    while(i > 0 && x != 0) {
                        --i;

                        if(program[i] == '[') {
                            ++x;
                        } else if(program[i] == ']') {
                            --x;
                        }
                    }

                    if(i < 0) {
                        fprintf(stderr, "ERROR: Encountered beggining-of-file while looking for matching open-bracket for closed-bracket at line %u, column %u\n", line, column);
                        return false;
                    }
                }
                break;

            case '\n':
                ++line;
                column = 0;
                break;

            default:
                break;
        }
    }

    return true;
}