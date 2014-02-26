#!/bin/sh

OUTPUT_TYPES="xml html json csv text"
TEST_BIN="./outplus_test"
MATCH_DIR="out"
FAILED=0

echo "Running Outplus Tests"

for i in $OUTPUT_TYPES
do
    BIN_RV=0
    DIFF_RV=0

    echo -n "Testing ${i} output"

    TMP=$(mktemp)
    MATCH="${MATCH_DIR}/${i}.out"

    $TEST_BIN $i > $TMP
    BIN_RV=$?

    diff $MATCH $TMP > /dev/null
    DIFF_RV=$?

    if [ "$BIN_RV" -ne "0" -o "$DIFF_RV" -ne "0" ]; then 
        FAILED=$(( $FAILED + 1 ))
        echo "\t\t[FAILED]"
    else
        echo "\t\t[OK]"
    fi
    rm $TMP
done

if [ $FAILED -eq 0 ]; then
    echo "All tests OK"
    exit 0;
else 
    echo "${FAILED} tests failed"
    exit 1;
fi
