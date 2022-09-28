#!/bin/bash

WAT2WASM=$(which wat2wasm)

if [[ ! -x $WAT2WASM ]]; then
   echo Error: wat2asm not found in PATH
   exit 1
fi

if [ $# = 0 ]; then
    FILES="*.wat"
else
    FILES="$@"
fi

for f in $FILES; do
    wat2wasm $f
done
