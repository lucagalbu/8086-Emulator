# 8086 Emulator

This code emulates an Intel 8086 CPU.

## Registers

14 registers of 16 bits (2 bytes):

### General purpose registers

These registers can store any kind of temporary data. However, each one of them has a common use case:

- AX (divided in AH + AL): accumulator. Used to store the result of arithmetical and logical operations.
- BX (divided in BH + BL): base. Used to store the value of an offset.
- CX (divided in CH + CL): counter. Used in looping.
- DX (divided in DH + DL): data. Used in multiplication and input/output port addressing.

### Segment registers

The microprocessor divides the external memory into segments. The following registers are used to identify the beginning of each segment.

- CS: code segment.
- DS: data segment.
- SS: stack segment.
- ES: extra segment. By default the data are in the data segment. If more space is needed, the extra segment can be used.

### Pointers and index registers

They points to specific memory locations by storing the offset relative to a specific segment. The locations must be even multiples of 16. The different segments can overlap.

- SP: stack pointer. It contains the offset of the topmost item of the stack. If the stack is empty, it will be 0xFFFE. The offset is relative to the stack segment.
- BP: base pointer. Used to hold the offset address of locations in the stack. The offset is relative to the stack segment.
- SI: source index. Used to store the offset address of data, usually as a source in some string-related operations. The offset is relative to the data or extra segment.
- DI: destinaton index. Used to store the offset address of data, usually as a destination in some string-related operations. The offset is relative to the data or extra segment.
- IP: instruction pointer. Used to store the offset of the next instruction that is to be executed. The offset is relative to the code segment.

## Memory

The microprocessor 8086 has a 20 address lines, meaning that it has 20 bits to address memory. Each bit is used to identify one byte of memory, meaning that the processor is able to address $2^{20}$ bytes or 1 MB memory.

However, the internal registries are 16 bits, so each one of those can address only 64 KB of memory and each segment must be 64 KB. Having 4 segments, the 8086 microprocessor can work only with four 64 KB segment within the 1 MB memory at any given time.

## Instruction format

The instruction format for 8086 can vary from 1 byte to 6 bytes. The general format is

- 8 bits: prefix.
- 8 bits: operation code (OpCode). It defines the type of operation, e.g. 100010 corresponds to MOV.
- 2 bits: word (W). How many bytes are transferred: W=0 only one byte, W=1 a whole word (16 bit).
- 8 bits: Mod/Rm. How memorz is accessed and which registers are used.
- 16 bits: displacement. If the displacement is of 8 bits, then only the cell of low order displacement infilled and if the displacement is of 16 bits, then both the cells od low order and high order are filled. The format of the displacement is determined by the Mod/Rm byte (see below).

### OpCode

The OpCode is given by 8 bits. Many OpCodes uses the last bits to give specific information about the instruction:

- D-bit: indicates that the register specified within the instruction is a source register (D = 0) or destination register (D =1).
- W-bit: some instructions of 8086 can operate on byte or a word. The W-bit in the opcode of such instruction specify whether instruction is a byte instruction (W = 0) or a word instruction (W = 1).
- S-bit: if S=1, an 8-bit 2’s complement number can be extended to a 16-bit 2’s complement number by making all of the bits in the higher-order byte equal the most significant bit in the low order byte. This is known as sign extension. This allows e.g. one 16 bit operation (W=1) to encode the operand in only 8 bit. If W=1, the S-bit is ignored.
- V-bit: decides the number of shifts for rotate and shift instructions. If V = 0, then count = 1; if V = 1, the count is in CL register.
- Z-bit: it is used for string primitives such as REP for comparison with ZF Flag.

### Data memory addressing modes (Mod/Rm)

The Mod/Rm is a byte used to specify how memory is addressed.

- 2 bits: Mod. It is used to distinguish between memory and register addressing, and in the case of memory addressing, specifies how many displacement bytes follow the addressing mode byte.
- 3-bits: Reg. Defines which register will be used in the operation. It is used in conjunction with another bit, the w bit to selects whether an 8- or 16-bit operation is performed.
- 3 bits: r/m. It is used in conjunction with the mod field to specify the addressing mode.

The 2-bit mod encodes how many displacement bytes are following the Mod/Rm:

- 00: No displacement bytes.
- 01: One signed displacement byte.
- 10: Unsigend 2-bytes displacement.
- 11: register addressing mode, where the register is specified by r/m. It is used in conjunction with the w bit to determine if an 8- or 16-bit register is selected.

The 3 bits r/m encodes how to access the memory, in conjunction with mod:

| r/m | memory address                               |
| --- | -------------------------------------------- |
| 000 | BX + SI + displacement                       |
| 001 | BX + DI + displacement                       |
| 010 | BP + SI + displacement                       |
| 011 | BP + DI + displacement                       |
| 100 | SI + displacement                            |
| 101 | DI + displacement                            |
| 110 | BP + displacement or direct access if mod=00 |
| 111 | BX + displacement                            |

direct access means that te offset address is taken directly from the two bytes that follow the addressing mode byte.

When mod = 11, r/m encodes the register, in the same way as the 3-bits reg does:

| r/m <br> reg | w=0 | w=1 |
| ------------ | --- | --- |
| 000          | AL  | AX  |
| 001          | CL  | CX  |
| 010          | DL  | DX  |
| 011          | BL  | BX  |
| 100          | AH  | SP  |
| 101          | CH  | BP  |
| 110          | DH  | SI  |
| 111          | BH  | DI  |

### Prefix

Every addressing mode has a standard default segment register. In most cases you can select an alternative segment register by using a segment override prefix. To use the prefix, place the following byte in front ofthe instruction whose default segment register assignment is to be overriden.

The format must be 001rr110, where
| rr | segment |
| -- | ------- |
| 00 | ES |
| 01 | CS |
| 10 | SS |
| 11 | DS |

### Examples

Let's consider the operation whose OpCode is 0000010w. This instruction doesn't expect a mod/rm byte. Depending on w, it's meaning is:

- w=0: add to AL the content of the next 8 bits.
- w=1: add to AX the content of the next 16 bits.

Let's now consider the OpCode 100000sw, where s and w are as defined earlier. This operation expects a mod/rm byte with reg=000 and its meaning is: add to the location specified by the mod/rm the data in the following 8- or 16-bits.
For example, 1000001 11000010 corresponds to ADD DX, 0xF0F0.

As a final example, let's consider the OpCode 000000dw, which expects a mod/rm byte. This operation moves the content of the memory to a register, or viceversa. The register and memory offset of the segment data are encoded in the mod/rm byte.
For example, 00000001 11001110:

- W=1, i.e. we deal with 16-bits operands.
- D=0 means mod/rm is moved to reg.
- Mod is 11, i.e. rm encodes a register.
- Reg is 001, i.e. CX.
- Rm is 110, i.e. SI.

Therefore, 00000001 11001110 -> ADD SI, CX.

## Flags

- Flags> https://www.geeksforgeeks.org/flag-register-8086-microprocessor/
