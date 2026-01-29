# VC370 Assembly Compiler & Emulator

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg" alt="C++20"/>
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey.svg" alt="Windows"/>
  <img src="https://img.shields.io/badge/IDE-Visual%20Studio-purple.svg" alt="Visual Studio"/>
  <img src="https://img.shields.io/badge/License-MIT-green.svg" alt="MIT License"/>
</p>

A **two-pass assembler** and **emulator** for the VC370, a hypothetical computer architecture. This project demonstrates fundamental compiler design concepts, including lexical analysis, symbol table management, error handling, and machine code generation.

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Architecture](#-architecture)
- [Instruction Set](#-instruction-set)
- [Project Structure](#-project-structure)
- [Building the Project](#-building-the-project)
- [Usage](#-usage)
- [Example Program](#-example-program)
- [Technical Implementation](#-technical-implementation)
- [Error Handling](#-error-handling)

---

## 🔍 Overview

The VC370 Assembly Compiler is a complete assembler that translates assembly language programs into machine code and executes them on a built-in emulator. The project implements a classic **two-pass assembly** process:

1. **Pass I**: Scans the source code to build a symbol table with label addresses
2. **Pass II**: Generates machine code using the symbol table and reports errors

The emulator then executes the generated machine code, simulating the VC370's behavior with a 10,000-word memory and an accumulator-based architecture.

---

## ✨ Features

- **Two-Pass Assembly**: Robust label resolution with forward reference support
- **Comprehensive Error Detection**: 14+ distinct error types with location reporting
- **Symbol Table Management**: O(1) average lookup using `unordered_map`
- **Full Instruction Set**: 13 machine instructions + 4 assembler directives
- **Built-in Emulator**: Execute assembled programs immediately
- **Modern C++20**: Leverages `std::format`, `std::ranges`, `std::string_view`, and more
- **Clean Architecture**: Modular design with separation of concerns

---

## 🏗️ Architecture

### VC370 Machine Specifications

| Component | Specification |
|-----------|---------------|
| Memory | 10,000 words (addresses 0-9999) |
| Word Size | 6 decimal digits |
| Accumulator | Single accumulator for arithmetic |
| Instruction Format | XXYYYY (XX = opcode, YYYY = operand/address) |

### System Components

```
┌─────────────────────────────────────────────────────────────┐
│                        Assembler                            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────┐   │
│  │  FileAccess  │──│  Instruction │──│   SymbolTable    │   │
│  │   (Parser)   │  │   (Lexer)    │  │ (Label Storage)  │   │
│  └──────────────┘  └──────────────┘  └──────────────────┘   │
│          │                │                   │             │
│          └────────────────┼───────────────────┘             │
│                           ▼                                 │
│                    ┌──────────────┐                         │
│                    │   Emulator   │                         │
│                    │  (Executor)  │                         │
│                    └──────────────┘                         │
└─────────────────────────────────────────────────────────────┘
```

---

## 📖 Instruction Set

### Machine Language Instructions

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 01 | `ADD` | Add memory contents to accumulator |
| 02 | `SUB` | Subtract memory contents from accumulator |
| 03 | `MULT` | Multiply accumulator by memory contents |
| 04 | `DIV` | Divide accumulator by memory contents |
| 05 | `LOAD` | Load memory contents into accumulator |
| 06 | `STORE` | Store accumulator contents to memory |
| 07 | `READ` | Read input into memory location |
| 08 | `WRITE` | Write memory contents to output |
| 09 | `B` | Unconditional branch |
| 10 | `BM` | Branch if accumulator is negative |
| 11 | `BZ` | Branch if accumulator is zero |
| 12 | `BP` | Branch if accumulator is positive |
| 13 | `HALT` | Terminate program execution |

### Assembler Directives

| Directive | Description |
|-----------|-------------|
| `DC` | Define Constant - allocate and initialize memory |
| `DS` | Define Storage - reserve memory locations |
| `ORG` | Origin - set the location counter |
| `END` | End of program marker |

---

## 📁 Project Structure

```
VC370-AssemblyCompiler/
├── Assembler.cpp        # Main assembler logic (Pass I & Pass II)
├── Assembler.h          # Assembler class definition
├── AssemblerTest.cpp    # Main entry point
├── Emulator.cpp         # VC370 machine emulator
├── Emulator.h           # Emulator class definition
├── Error.cpp            # Error reporting system
├── Error.h              # Error codes and messages
├── FileAccess.cpp       # Source file reader
├── FileAccess.h         # File access interface
├── Instruction.cpp      # Instruction parser/lexer
├── Instruction.h        # Instruction class definition
├── SymbolTable.cpp      # Symbol table implementation
├── SymbolTable.h        # Symbol table interface
└── stdafx.h             # Precompiled header
```

---

## 🔧 Building the Project

### Prerequisites

- **Visual Studio 2022** (or later) with C++20 support
- **Windows SDK**

### Build Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/icyDenev/VC370-AssemblyCompiler.git
   ```

2. Open the solution in Visual Studio

3. Build the solution (F7 or Build → Build Solution)

---

## 🚀 Usage

### Command Line

```bash
VC370-AssemblyCompiler.exe <source_file.asm>
```

### Output

The assembler produces:
1. **Symbol Table**: All labels and their memory addresses
2. **Translation Listing**: Machine code with original source
3. **Error Report**: Any errors encountered during assembly
4. **Program Output**: Results from emulator execution

---

## 📝 Example Program

### Sample Assembly Code

```asm
; Simple program to add two numbers
        ORG     100         ; Start at location 100
        READ    NUM1        ; Read first number
        READ    NUM2        ; Read second number
        LOAD    NUM1        ; Load first number
        ADD     NUM2        ; Add second number
        STORE   RESULT      ; Store the result
        WRITE   RESULT      ; Output the result
        HALT                ; End program
        
; Data section
NUM1    DS      1           ; Reserve space for NUM1
NUM2    DS      1           ; Reserve space for NUM2
RESULT  DS      1           ; Reserve space for RESULT
        END
```

### Sample Output

```
Symbol Table:
Symbol #    Symbol    Location
 0          NUM1      108       
 1          NUM2      109       
 2          RESULT    110       
____________________________________________

Translation of Program:
Location  Contents       Original Statement
                         ; Simple program to add two numbers
100       070108         READ    NUM1
101       070109         READ    NUM2
102       050108         LOAD    NUM1
103       010109         ADD     NUM2
104       060110         STORE   RESULT
105       080110         WRITE   RESULT
106       130000         HALT
____________________________________________
```

---

## 🔬 Technical Implementation

### Modern C++20 Features Used

| Feature | Usage |
|---------|-------|
| `std::format` | Formatted output for tables and listings |
| `std::string_view` | Efficient string parameter passing |
| `std::ranges` | Modern algorithms for searching and transforming |
| `std::array` | Fixed-size arrays for instruction sets |
| `[[nodiscard]]` | Enforced return value checking |
| `constexpr` | Compile-time constants and functions |
| Structured bindings | Clean iteration over maps |
| Init-statements in `if` | Cleaner conditional blocks |

### Key Design Decisions

1. **Hash-based Symbol Table**: Uses `std::unordered_map` for O(1) average lookup time
2. **Immutable String Views**: Uses `std::string_view` to avoid unnecessary copies
3. **Static Error Reporting**: Global error collection for centralized error management
4. **Modular Architecture**: Each component has a single responsibility

---

## ⚠️ Error Handling

The assembler detects and reports the following errors:

| Error Code | Description |
|------------|-------------|
| `ERR_INVALID_OPCODE` | Unrecognized instruction |
| `ERR_UNDEFINED_LABEL` | Reference to undefined label |
| `ERR_DUPLICATE_LABEL` | Label defined multiple times |
| `ERR_INVALID_LABEL` | Label exceeds 10 characters |
| `ERR_SYNTAX_ERROR` | General syntax error |
| `ERR_EXTRA_ELEMENTS` | Unexpected elements on line |
| `ERR_MISSING_OPERAND` | Required operand not provided |
| `ERR_MISSING_END_STATEMENT` | No END directive found |
| `ERR_END_STATEMENT_NOT_LAST` | Code after END directive |
| `ERR_OPERAND_OVERFLOW` | Operand value too large |
| `ERR_INVALID_OPERAND` | Invalid operand format |
| `ERR_MEMORY_OVERFLOW` | Address exceeds memory bounds |
| `ERR_MACHINE_CODE_AFTER_HALT` | Instructions after HALT |
| `ERR_ASSEMBLY_CODE_BEFORE_HALT` | Data definitions in code section |

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👤 Author

**Hristo Denev**

- GitHub: [@icyDenev](https://github.com/icyDenev)

---

## 🙏 Acknowledgments

- Inspired by classic assembler design principles
- Built as part of the Software Design Course (CMPS361)
