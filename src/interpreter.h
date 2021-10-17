#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <stdlib.h>
#include <stdbool.h>

bool init_interpreter(void);
void destroy_interpreter(void);
bool run_program(char* program, size_t length);

#endif // INTERPRETER_H_