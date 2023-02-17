# PL Lab: Simple MIPS Assembler

MIPS (Microprocessor without Interlocked Pipelined Stages) is a family of reduced instruction set computer (RISC) instruction set architectures (ISA).

Instructions are divided into three types: R (register), I (immediate), and J (jump). Every instruction starts with a 6-bit opcode. In addition to the opcode, R-type instructions specify three registers, a shift amount field, and a function field; I-type instructions specify two registers and a 16-bit immediate value; J-type instructions follow the opcode with a 26-bit jump target.

| Type | Format(bits)                                                   |
|:----:|:-------------------------------------------------------------- |
|  R   | opcode(6)  \| rs(5) \| rt(5) \| rd (5) \| shamt(5)	\| funct(6) |
|  I   | opcode(6)	\| rs(5) \| rt(5) \| immediate (16)                  |
|  J   | opcode(6)	\| address (26)                                      |
       
This question will only use the following registers and commands :  
| Instruction(R-Type) | register1 | register2 | register3 | Explain       |
| ------------------- | --------- | --------- | --------- | ------------- |
| add                 | rd        | rs        | rt        | rd = rs + rt  |
| sub                 | rd        | rs        | rt        | rd = rs - rt  |
| or                  | rd        | rs        | rt        | rd = rs \| rt |
| and                 | rd        | rs        | rt        | rd = rs & rt  |

| Instruction(I-Type) | regiter1 | regiter2 | Immediate | Explain       |
| ------------------- | -------- | -------- | --------- | ------------- |
| addi                | rt       | rs       | imm       | rt = rs + imm |


Please convert the input assembly code to the specified format:
| Register | format |
| -------- | ------ |
| $t0      | 01000  |
| $t1      | 01001  |
| $t2      | 01010  |
| $t3      | 01011  |
| $t4      | 01100  |
| $t5      | 01101  |
| $t6      | 01110  |
| $t7      | 01111  |

| Instruction(R-Type) | opcode(6) | register1(5) | register2(5) | register3(16) | shamt (5) | funct (6) |
| ------------------- |:---------:| ------------ | ------------ | ------------- | --------- | --------- |
| add                 |  000000   | rs           | rt           | rd            | 00000     | 100000    |
| sub                 |  000000   | rs           | rt           | rd            | 00000     | 100010    |
| or                  |  000000   | rs           | rt           | rd            | 00000     | 100101    |
| and                 |  000000   | rs           | rt           | rd            | 00000     | 100100    |

| Instruction(Ｉ-Type) | opcode(6) | register1(5) | register2(5) | immediate (16)     |
| -------------------- |:---------:| ------------ | ------------ | ------------------ |
| addi                 |  001000   | rs           | rt           | imm(binary format) |  


---


For example,
the code "add t0 t1 t2" will be "000000 01001 01010 01000 00000 100000",
and the code "addi t0 t1 3" will be "001000 01001 01000 0000000000000011".