# **Documentación del Ensamblador y Bytecode de ZynkVM**
*Nota: Tenga en cuenta que los comentarios no existen en el ensamblador, son solo para facilitar la lectura.*
## **Introducción**
**ZynkVM** utiliza un formato de bytecode compacto y eficiente para ejecutar instrucciones en su máquina virtual. Este bytecode es generado por un ensamblador que traduce instrucciones de medio nivel en un formato binario ejecutable por la VM.

---

## **Formato del Bytecode**
El bytecode de **ZynkVM** está compuesto por una secuencia de instrucciones, donde cada instrucción puede incluir:
1. **Opcode**: Un identificador único de 1 byte que representa la operación a realizar.
2. **Valor** (en vez de **Opcode**): Datos.

### **Estructura General**
```
[Opcode/Valor]
```

---

## **Lista de Instrucciones**
A continuación, se describen las instrucciones soportadas por **ZynkVM**:

| **Instrucción** | **Opcode** | **Descripción**                                                                 | **Operandos** |
|------------------|------------|---------------------------------------------------------------------------------|---------------|
| `OP_CONSTANT`    | `0x00`     | Empuja una constante desde el pool de constantes a la pila.                     | Índice (4 bytes) |
| `OP_RETURN`      | `0x01`     | Finaliza la ejecución y devuelve el valor en el tope de la pila.                | Ninguno        |
| `OP_NEGATE`      | `0x02`     | Niega el valor en el tope de la pila.                                           | Ninguno        |
| `OP_ADD`         | `0x03`     | Suma los dos valores superiores de la pila y empuja el resultado.               | Ninguno        |
| `OP_SUBSTRACT`   | `0x04`     | Resta los dos valores superiores de la pila y empuja el resultado.              | Ninguno        |
| `OP_MULTIPLY`    | `0x05`     | Multiplica los dos valores superiores de la pila y empuja el resultado.         | Ninguno        |
| `OP_DIVIDE`      | `0x06`     | Divide los dos valores superiores de la pila y empuja el resultado.             | Ninguno        |

---

## **Ejemplo de Bytecode**
El siguiente ejemplo muestra un programa que suma dos constantes y devuelve el resultado:

### **Código Ensamblador**
```asm
CONSTANT;
3.14;
CONSTANT;
2.71;
ADD;
RETURN;
```

### **Bytecode Generado**
```
0x00 0x00 0x00 0x00 0x00  // OP_CONSTANT (índice 0)
0x00 0x00 0x00 0x00 0x01  // OP_CONSTANT (índice 1)
0x03                      // OP_ADD
0x01                      // OP_RETURN
```

---

## **Formato del Ensamblador**
El ensamblador utiliza una sintaxis simple basada en instrucciones separadas por líneas. Cada instrucción puede incluir un opcode y, opcionalmente, operandos.

### **Reglas del Ensamblador**
1. Cada instrucción debe terminar con un punto y coma (`;`).
2. Los operandos (si los hay) deben estar separados por espacios.
3. Los valores constantes se definen como números en formato decimal.

### **Ejemplo**
```asm
CONSTANT;
42.0;  // Empuja el valor 42.0 a la pila
NEGATE;         // Niega el valor en el tope de la pila
RETURN;         // Finaliza la ejecución
```

---

## **Extensión del Bytecode**
Para agregar nuevas instrucciones al bytecode:
1. Define un nuevo opcode en `opcodes.h`.
2. Implementa la lógica de la instrucción en la función `run` de `vm.h`.
3. Actualiza el ensamblador en `zasm.h` para traducir la nueva instrucción.

### **Ejemplo: Nueva Instrucción `OP_PRINT`**
1. **Definir el Opcode**:
   ```c
   // filepath: src/core/opcodes.h
   typedef enum {
       ...
       OP_PRINT, // Imprime el valor en el tope de la pila
   } OpCode;
   ```

2. **Implementar la Lógica**:
   ```c
   // filepath: src/core/vm.h
   case OP_PRINT:
       printVal(pop(vm));
       break;
   ```

3. **Actualizar el Ensamblador**:
   ```c
   // filepath: src/assembler/zasm.h
   else if (zcmp(line, "PRINT;", ';')) {
       code[*code_index] = OP_PRINT;
   }
   ```

---

## **Pruebas**
### **Prueba de Ejecución**
1. Escribe un programa en ensamblador.
2. Compílalo a bytecode usando el ensamblador.
3. Ejecuta el bytecode en ZynkVM y verifica los resultados.

### **Ejemplo**
**Entrada Ensamblador**:
```asm
CONSTANT;
10;
CONSTANT;
20;
ADD;
PRINT;
RETURN;
```

**Salida Esperada**:
```
30
```

---

## **Referencias**
- Opcodes
- Máquina Virtual
- Ensamblador

---

[Atras](../README.md)