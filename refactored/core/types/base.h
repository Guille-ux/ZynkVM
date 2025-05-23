struct Value;
struct Obj;
#ifndef ZYNK_BASE_TYPES_H
#define ZYNK_BASE_TYPES_H

// Forward declarations
struct Obj;
struct ObjString;
struct Value;
struct Chunk;
struct ZynkVM;
struct Table;
struct ValueArray;
struct Entry;
struct ArenaManager;

typedef struct Obj Obj;
typedef struct ObjString ObjString;
typedef struct Value Value;
typedef struct ValueArray ValueArray;
typedef struct Chunk Chunk;
typedef struct ZynkVM ZynkVM;
typedef struct Table Table;
typedef struct Entry Entry;
typedef struct ArenaManager ArenaManager;

// Enums básicos
typedef enum {
    ZYNK_OBJ_STRING,
} ObjType;

typedef enum {
    ZYNK_BOOL,
    ZYNK_NUM,
    ZYNK_NULL,
    ZYNK_BYTE,
    ZYNK_OBJ,
    ZYNK_VAR,
} ZynkType;

typedef enum {
    ZYNK_OK,
    ZYNK_COMPILE_ERROR,
    ZYNK_RUNTIME_ERROR,
} ZynkResult;

#define STACK_MAX 1024*1024

#endif