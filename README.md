# Shell-CS345
An implementation of a basic shell (Assignment 1 of CS345 at CSD @ UoC)

**To compile everything type:** make (shell)

#### command.c

This file contains a basic **struct for a command** *object* as well as the corresponding functions that go with it. Such functions include:
- **initializing** a command
- **parsing** a command
- **executing** a command
- **freeing** a command

#### stack.c

This file contains a basic **struct for a stack of command structs** mentioned above. It contains all the **basic functions for operating on a stack**.

#### tools.c

This file contains useful **functions that operate on strings** that are used in more than one file and thus worth of a separate file. Such functions include:
- **clearing leading and trailing whitespace**
- **deleting quotes**
- **replacing $\<var\>** with the corresponding environmental var

#### shell.c

This file is where the **main()** is contained. It initializes and operates on the stack of commands, asking for input when the stack is empty else executing the next command of the stack.

###Known issues

The shell **doesn't** handle Signals such as `CTRL-C`, `CTRL-S`

### Future improvements

The implementation of a stack allows for the addition of a **command history**.
This would allow:
- Executing the previous command
- Loops
- Macros

**Signal Handling** would be a plus but unlikely to be added