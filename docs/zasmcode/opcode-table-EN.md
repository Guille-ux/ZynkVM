| **Name**      | **Opcode** | **Operands**                | **Stack Effect**                       | **Description**                                                                                   |
|---------------|:----------:|-----------------------------|----------------------------------------|---------------------------------------------------------------------------------------------------|
| OP_CONSTANT   | 0x00       | sizeof(size_t) (index)      | `[ ... ] → [ ..., constant ]`          | Pushes a constant from the chunk's constant pool onto the stack.                                  |
| OP_RETURN     | 0x01       | None                        | `[ ..., value ] → [ ]`                 | Ends execution and returns the value at the top of the stack.                                     |
| OP_NEGATE     | 0x02       | None                        | `[ ..., value ] → [ ..., -value ]`     | Negates the numeric or boolean value at the top of the stack.                                     |
| OP_ADD        | 0x03       | None                        | `[ ..., a, b ] → [ ..., a + b ]`       | Pops two values, adds them (if numbers), and pushes the result.                                   |
| OP_SUBSTRACT  | 0x04       | None                        | `[ ..., a, b ] → [ ..., a - b ]`       | Pops two values, subtracts the second from the first, and pushes the result.                      |
| OP_MULTIPLY   | 0x05       | None                        | `[ ..., a, b ] → [ ..., a * b ]`       | Pops two values, multiplies them, and pushes the result.                                          |
| OP_DIVIDE     | 0x06       | None                        | `[ ..., a, b ] → [ ..., a / b ]`       | Pops two values, divides the first by the second, and pushes the result.                          |
| OP_TRUE       | 0x07       | None                        | `[ ... ] → [ ..., true ]`              | Pushes the boolean value `true` onto the stack.                                                   |
| OP_FALSE      | 0x08       | None                        | `[ ... ] → [ ..., false ]`             | Pushes the boolean value `false` onto the stack.                                                  |
| OP_NULL       | 0x09       | None                        | `[ ... ] → [ ..., null ]`              | Pushes the special `null` value onto the stack.                                                   |
| OP_EQUAL      | 0x0A       | None                        | `[ ..., a, b ] → [ ..., a == b ]`      | Pops two values, compares them for equality, and pushes the boolean result.                       |
| OP_GREATER    | 0x0B       | None                        | `[ ..., a, b ] → [ ..., a > b ]`       | Pops two values, compares if the first is greater than the second, and pushes the boolean result. |
| OP_LESS       | 0x0C       | None                        | `[ ..., a, b ] → [ ..., a < b ]`       | Pops two values, compares if the first is less than the second, and pushes the boolean result.    |
| OP_BYTE       | 0x0D       | 1 byte (value)              | `[ ... ] → [ ..., byte ]`              | Pushes a single byte value onto the stack.                                                        |
| OP_STRING     | 0x0E       | Variable (length + bytes)   | `[ ..., length ] → [ ..., string ]`    | Creates a string from the next `length` bytes and pushes it onto the stack. Length is popped.     |
| OP_DUPE       | 0x0F       | None                        | `[ ..., value ] → [ ..., value, value ]`| Duplicates the value at the top of the stack.                                                     |
| OP_DEL        | 0x10       | None                        | `[ ..., value ] → [ ... ]`             | Removes (pops) the value at the top of the stack.                                                 |
| OP_BACKDEL    | 0x11       | None                        | `[ ..., a, b ] → [ ..., b ]`           | Removes the second-to-top value from the stack, leaving the top value.                            |

**Notes:**
- All arithmetic and comparison instructions require operands of the same type (usually numbers).
- Type errors or stack underflows should be handled as runtime errors by the VM.
- For the full list of opcodes and their numeric values, see `opcodes.h`.