# Zlang - A Lightweight Interpreter in C

Zlang is a simple and lightweight interpreter built in C. It supports a limited set of basic programming constructs such as loops, mathematical integer expressions and an in-built print function. The goal of the project was to implement a limited set of programming languages constructs in order to gain an understanding of how interpreters work. Currenty, all mathematical operations are done with integer arithmetics. Additional constructs and support for different variable types might be added later. 

## Features

- **Custom Language Syntax**: Zlang interprets a custom scripting language with `.zl` files.
- **Control Structures**: Supports constructs like `for` and `while` loops.
- **Expression Evaluation**: Handles basic mathematical and logical expressions.
- **In-built print function**: Used for printing the result of an expression evaluation
- **Error Handling**: Includes error detection for undefined variables and invalid operations.
- **Memory Management**: Utilizes Valgrind to detect memory leaks.

## Project Structure

- **Source Files**:
  - `lexer.c`: Tokenizes the input script.
  - `parser.c`: Parses tokens into an abstract syntax tree (AST).
  - `interpreter.c`: Interprets and executes the AST.
  - `abstract_syntax_tree.c`: Defines the AST structure.
- **Docker Integration**:
  - `Dockerfile`: Sets up a reproducible build environment.
  - `docker-compose.yaml`: Simplifies container build.
- **Example Test files**:
  - Example `.zl` scripts like `test.zl` and `while_for_test_instructions.zl` 
   provide examples of valid instructions and expressions interpretable by the 
   zlang interpreter.

## Getting Started

### Prerequisites

- Docker and Docker Compose installed on your system.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Dan-Popescu/zlang.git
   or 
   git clone git@github.com:Dan-Popescu/zlang.git
   cd zlang
   ```

2. Build valgrind Docker container:
   ```bash
   docker-compose build
   ``` 
3. Run the container
   ```bash 
   docker-compose run valgrind
   ```

### Manual Build

If you prefer to run the project natively:

1. Install CMake and a C compiler (e.g., GCC or Clang).
2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
## Usage

### Running Zlang in console mode

 ```bash
./zlang
```
### Running Zlang by passing a script file containing the instructions

 ```bash
./zlang file_with_instructions.zl
 ```
### Example `.zl` Script

```zl
x = 10;
while (x > 0) {
    print(x);
    x = x - 1;
}
```
## Technical Details

### Lexer
- Tokenizes the input into meaningful symbols.
- Handles keywords, identifiers, numbers, and operators.

### Parser
- Converts tokens into an Abstract Syntax Tree (AST).
- Validates the syntax of the input script.

### Interpreter
- Traverses the AST to evaluate and execute commands.
- Includes support for variable assignment, arithmetic operations, and control 
  flow instruction(limited to for and while loops only at the moment).

### Memory Management
- The project uses Valgrind to ensure memory is allocated and freed correctly.

