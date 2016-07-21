add reg reg
mul reg reg
min reg reg 
div reg reg
and reg reg
or reg reg
not reg reg
load reg
store reg
setlow imm
sethigh imm
call
ret
jump
compare reg reg
branch reg

Immediates: 0x0 for hex
Registers:
  10 gp
  rs - result
  sp
  fp
  pc
  sn - signed arithmetic
  me - memory address

Encoding:
Each instruction is 64 bits
32bit opcode
32bit operands

Register table
0x00 r0
0x01 r1
0x02 r2
0x03 r3
0x04 r4
0x05 r5
0x06 r6
0x07 r7
0x08 r8
0x09 r9
0x0a r10
0x0b rs
0x0c sp
0x0d fp
0x0e pc
0x0f sn
0x10 me

Opcode table
0x00 add
0x01 sub
0x02 mul
0x03 div
0x04 and
0x05 or
0x06 not
0x07 load
0x08 store
0x09 setlow
0x0a sethigh
0x0b call
0x0c ret
0x0d jump
0x0e compare
0x0f branch

