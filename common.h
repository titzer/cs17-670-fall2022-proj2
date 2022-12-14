#pragma once
#include <stdint.h>

typedef uint8_t byte;

// Limit file size to something reasonable.
#define MAX_FILE_SIZE 2000000000

// Decode LEB128 values, enforcing the specified signedness and maximum range.
// The argument {ptr} will be advanced to point after the decoded value.
// If an error occurs (e.g. unterminated LEB, invalid value), {ptr} will be set to
// NULL.
int32_t  decode_i32leb(const byte* ptr, const byte* limit, ssize_t* len);
uint32_t decode_u32leb(const byte* ptr, const byte* limit, ssize_t* len);
int64_t  decode_i64leb(const byte* ptr, const byte* limit, ssize_t* len);
uint64_t decode_u64leb(const byte* ptr, const byte* limit, ssize_t* len);

// Decode fixed-size integer values.
uint32_t decode_u32(const byte* ptr, const byte* limit, ssize_t* len);

// Load a file into memory, initializing pointers to the start and end of
// the memory range containing the data. Returns < 0 on failure.
ssize_t load_file(const char* path, byte** start, byte** end);

// Unload a file previously loaded into memory using {load_file}.
ssize_t unload_file(byte** start, byte** end);

// The global trace flag.
extern int g_trace;
extern int g_disassemble;

// Helper macros to trace and to print errors.
#define TRACE(...) do { if(g_trace) fprintf(stderr, __VA_ARGS__); } while(0)
#define DISASS(...) do { if(g_disassemble) fprintf(stderr, __VA_ARGS__); } while(0)
#define ERR(...) fprintf(stderr, __VA_ARGS__)

// A helper struct that keeps track of the start and the current pointer of
// a buffer.
typedef struct {
  const byte* start;
  const byte* ptr;
  const byte* end;
} buffer_t;

// Read an unsigned 32-bit LEB, advancing the {ptr} in the buffer.
uint32_t read_u32leb(buffer_t* buf);

// Read a signed 32-bit LEB, advancing the {ptr} in the buffer.
int32_t read_i32leb(buffer_t* buf);

// Read an unsigned 8-bit byte, advancing the {ptr} in the buffer.
uint32_t read_u8(buffer_t* buf);
