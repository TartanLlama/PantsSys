add reg reg
mul reg reg
sub reg reg 
div reg reg
and reg reg
or reg reg
not reg
copy reg reg
load reg
store reg
setlow imm
sethigh imm
jump
compare reg reg
branch reg

Immediates: 0x0 for hex
Registers:
  10 gp
  rs - result
  sp - stack pointer
  fp - frame pointer
  pc - program counter
  sn - signed arithmetic
  me - memory address
  ra - return address

Encoding:
Each instruction is 64 bits
32bit opcode
32bit operands

