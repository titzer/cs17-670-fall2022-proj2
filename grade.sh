#!/bin/bash

if [ $# = 0 ]; then
    WEERUN='./weerun'
else
    WEERUN=$1
    shift
fi

T=${OUT:=/tmp/$USER/cs17670/$STUDENT/proj2-test/}
mkdir -p $T


if [ ! -x "$WEERUN" ]; then
    printf "##+weerun\n"
    printf "##-fail, weerun not found: %s\n" $WEERUN
    exit 1
fi

if [ $# = 0 ]; then
    TESTS=$(ls tests/*.wasm)
else
    TESTS=$@
fi

# substitute for the "timeout" command on MacOS
function xtimeout {
  timeout=$1
  shift
  command=("$@")
  (
    "${command[@]}" &
    runnerpid=$!
    trap -- '' SIGTERM
    ( # killer job
      sleep $timeout
      if ps -p $runnerpid > /dev/null; then
	  echo "Timed out, killing"
          kill -SIGKILL $runnerpid 2>/dev/null
      fi
    ) &
    killerpid=$!
    wait $runnerpid
    kill -SIGKILL $killerpid
  )
}

for f in $TESTS; do
    # replace slashes with _
    t=${f//\//_}
    # replace .wasm with .expect
    e=${f/%.wasm/.expect}
    args=${f/%.wasm/.args}
    
    printf "##+%s\n" $t

    if [ -f $args ]; then
	ARGS=$(cat $args)
    else
	ARGS=""
    fi
    
    OUT=$T/$t.out
    ERR=$T/$t.err
    EXP=$T/$t.expect
    DIFF=$T/$t.diff

    echo $WEERUN $f
    xtimeout 5s $WEERUN $f $ARGS > $OUT 2>$ERR
    cp $e $EXP
    echo diff $OUT $EXP
    # ignore case
    diff -i $OUT $EXP > $DIFF
    OK=$?
    if [ ! -s $DIFF ]; then
	# remove empty diffs
	rm $DIFF
    else
	cat $DIFF
    fi
    
    if [ $OK = 0 ]; then
	printf "##-ok\n"
    else
	printf "##-fail\n"
    fi
done
