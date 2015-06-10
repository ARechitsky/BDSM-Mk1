#pragma once

#define COMMAND_MASK 0xF0
#define RESERVED_MASK 0x08
#define TYPE_MASK 0x06
#define POINTERMODE_MASK 0x01

#define MAX_COMMAND 0x0C

#define TYPE_CHAR 0x00
#define TYPE_BYTE 0x02
#define TYPE_WORD 0x04
#define TYPE_DWORD 0x06

#define POINTERMODE_ON 0x01
#define POINTERMODE_OFF 0x00

#define RESERVED 0x08
