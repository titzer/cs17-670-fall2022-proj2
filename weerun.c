#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "common.h"
#include "vm.h"
#include "test.h"
#include "weewasm.h"
#include "illegal.h"
#include "ir.h"

// Disassembles and runs a wasm module.
wasm_value_t run(const byte* start, const byte* end, uint32_t num_args, wasm_value_t* args);

// Main function.
// Parses arguments and either runs the tests or runs a file with arguments.
//  -trace: enable tracing to stderr
//  -test: run internal tests
int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    if (strcmp(arg, "-test") == 0) return run_tests();
    if (strcmp(arg, "-trace") == 0) {
      g_trace = 1;
      continue;
    }
    if (strcmp(arg, "-disassemble") == 0) {
      g_disassemble = 1;
      continue;
    }
    
    byte* start = NULL;
    byte* end = NULL;
    ssize_t r = load_file(arg, &start, &end);
    if (r >= 0) {
      TRACE("loaded %s: %ld bytes\n", arg, r);
      int num_args = argc - i - 1;
      wasm_value_t* args = NULL;
      if (num_args > 0) {
	args = (wasm_value_t*)malloc(sizeof(wasm_value_t)*num_args);
	for (int j = i + 1; j < argc; j++) {
	  int a = j - i - 1;
	  args[a] = parse_wasm_value(argv[j]);
	  TRACE("args[%d] = ", a);
	  trace_wasm_value(args[a]);
	  TRACE("\n");
	}
      }
      wasm_value_t result = run(start, end, num_args, args);
      unload_file(&start, &end);
      print_wasm_value(result);
      printf("\n");
      exit(0);
      return 0;
    } else {
      ERR("failed to load: %s\n", arg);
      return 1;
    }
  }
  return 0;
}

// Returns the string name of a section code.
const char* section_name(byte code) {
  switch (code) {
  case WASM_SECT_TYPE: return "type";
  case WASM_SECT_IMPORT: return "import";
  case WASM_SECT_FUNCTION: return "function";
  case WASM_SECT_TABLE: return "table";
  case WASM_SECT_MEMORY: return "memory";
  case WASM_SECT_GLOBAL: return "global";
  case WASM_SECT_EXPORT: return "export";
  case WASM_SECT_START: return "start";
  case WASM_SECT_ELEMENT: return "element";
  case WASM_SECT_CODE: return "code";
  case WASM_SECT_DATA: return "data";
  case 0: return "custom";
  default:
    return "unknown";
  }
}

// A pair of a buffer and a module into which to parse the bytes.
typedef struct {
  buffer_t* buf;
  wasm_module_t* module;
} mparse_t;

//=== SOLUTION CODE -- YOUR SOLUTION HERE ==================================
int parse(wasm_module_t* module, buffer_t* buf);

wasm_value_t run(const byte* start, const byte* end, uint32_t num_args, wasm_value_t* args) {
  buffer_t onstack_buf = {
    start,
    start,
    end
  };
  wasm_module_t onstack_module;
  memset(&onstack_module, 0, sizeof(onstack_module));
  int result = parse(&onstack_module, &onstack_buf);
  if (result < 0) return wasm_i32_value(-1);
  // TODO: run
  return wasm_i32_value(0);
}

// The main disassembly routine.
int parse(wasm_module_t* module, buffer_t* buf) {
  return -1;
}
