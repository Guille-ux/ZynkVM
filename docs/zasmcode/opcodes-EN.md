# ZynkVM Opcode and Instruction Reference

This section describes each opcode supported by ZynkVM, its byte value, stack effect, operands, and a detailed explanation of its behavior.

---

## **Stack Notation**
- `TOS`: Top of Stack
- `[...]`: Stack before operation
- `→`: Stack after operation

---

### **OP_CONSTANT**  
**Opcode:** `0x00`  
**Operands:** sizeof `size_t` bytes (constant index)  
**Stack Effect:** `[ ... ] → [ ..., constant ]`  
**Description:**  
Pushes a constant from the chunk's constant pool onto the stack. The operand is a `4 or 8`-byte index into the constant pool.

---

### **OP_RETURN**  
**Opcode:** `0x01`  
**Operands:** None  
**Stack Effect:** `[ ..., value ] → [ ]`  
**Description:**  
Ends execution of the current chunk and returns the value at the top of the stack. Typically used to finish a script or function.

---

### **OP_NEGATE**  
**Opcode:** `0x02`  
**Operands:** None  
**Stack Effect:** `[ ..., value ] → [ ..., -value ]`  
**Description:**  
Negates the numeric or boolean value at the top of the stack. For numbers, multiplies by -1. For booleans, inverts the value.

---

### **OP_ADD**  
**Opcode:** `0x03`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a + b ]`  
**Description:**  
Pops the top two values from the stack, adds them (if they are numbers), and pushes the result.

---

### **OP_SUBSTRACT**  
**Opcode:** `0x04`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a - b ]`  
**Description:**  
Pops the top two values from the stack, subtracts the second from the first (if they are numbers), and pushes the result.

---

### **OP_MULTIPLY**  
**Opcode:** `0x05`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a * b ]`  
**Description:**  
Pops the top two values from the stack, multiplies them (if they are numbers), and pushes the result.

---

### **OP_DIVIDE**  
**Opcode:** `0x06`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a / b ]`  
**Description:**  
Pops the top two values from the stack, divides the first by the second (if they are numbers), and pushes the result.

---

### **OP_TRUE**  
**Opcode:** `0x07`  
**Operands:** None  
**Stack Effect:** `[ ... ] → [ ..., true ]`  
**Description:**  
Pushes the boolean value `true` onto the stack.

---

### **OP_FALSE**  
**Opcode:** `0x08`  
**Operands:** None  
**Stack Effect:** `[ ... ] → [ ..., false ]`  
**Description:**  
Pushes the boolean value `false` onto the stack.

---

### **OP_NULL**  
**Opcode:** `0x09`  
**Operands:** None  
**Stack Effect:** `[ ... ] → [ ..., null ]`  
**Description:**  
Pushes the special `null` value onto the stack.

---

### **OP_EQUAL**  
**Opcode:** `0x0A`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a == b ]`  
**Description:**  
Pops the top two values, compares them for equality, and pushes the boolean result.

---

### **OP_GREATER**  
**Opcode:** `0x0B`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a > b ]`  
**Description:**  
Pops the top two values, compares if the first is greater than the second (numbers only), and pushes the boolean result.

---

### **OP_LESS**  
**Opcode:** `0x0C`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., a < b ]`  
**Description:**  
Pops the top two values, compares if the first is less than the second (numbers only), and pushes the boolean result.

---

### **OP_BYTE**  
**Opcode:** `0x0D`  
**Operands:** 1 byte (value)  
**Stack Effect:** `[ ... ] → [ ..., byte ]`  
**Description:**  
Pushes a single byte value onto the stack.

---

### **OP_STRING**  
**Opcode:** `0x0E`  
**Operands:** Variable (length + string bytes)  
**Stack Effect:** `[ ..., length ] → [ ..., string ]`  
**Description:**  
Creates a string object from the next `length` bytes in the bytecode and pushes it onto the stack. The length is popped from the stack.

---

### **OP_DUPE**  
**Opcode:** `0x0F`  
**Operands:** None  
**Stack Effect:** `[ ..., value ] → [ ..., value, value ]`  
**Description:**  
Duplicates the value at the top of the stack.

---

### **OP_DEL**  
**Opcode:** `0x10`  
**Operands:** None  
**Stack Effect:** `[ ..., value ] → [ ... ]`  
**Description:**  
Removes (pops) the value at the top of the stack.

---

### **OP_BACKDEL**  
**Opcode:** `0x11`  
**Operands:** None  
**Stack Effect:** `[ ..., a, b ] → [ ..., b ]`  
**Description:**  
Removes the second-to-top value from the stack, leaving the top value.

---

## **Notes**
- All arithmetic and comparison instructions require operands of the same type (usually numbers).
- Type errors or stack underflows should be handled as runtime errors by the VM.
- For a full list of opcodes and their numeric values, see `opcodes.h`.

---