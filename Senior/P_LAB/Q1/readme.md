MIPS (Microprocessor without Interlocked Pipelined Stages) is a family of reduced instruction set computer (RISC) instruction set architectures (ISA).

Instructions are divided into three types: R (register), I (immediate), and J (jump). Every instruction starts with a 6-bit opcode. In addition to the opcode, R-type instructions specify three registers, a shift amount field, and a function field; I-type instructions specify two registers and a 16-bit immediate value; J-type instructions follow the opcode with a 26-bit jump target.

| Type | Format(bits)                                        |
|:----:|:--------------------------------------------------- |
|  R   | opcode (6) \| rs (5) \| rt (5)	\| rd (5) \| shamt (5)	\| funct (6) |
|  I   | opcode (6)	\| rs (5) \| rt (5) \| immediate (16)             |
|  J   | opcode (6)	\| address (26)                             | 

<br>

This question will only use the registers $t0~$t7 which the registers are numbered 8~15.