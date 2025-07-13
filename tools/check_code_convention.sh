#!/bin/bash

echo "[Code Convention Check - Linux]"

REPORT_FILE="tools/code-convention-report.txt"
SUMMARY_FILE="tools/code-convention-summary.txt"
> "$REPORT_FILE"
> "$SUMMARY_FILE"

SRC_DIRS="src include tools/test_code"

FORMAT_FAIL=0
for DIR in $SRC_DIRS; do
  for FILE in "$DIR"/*.c "$DIR"/*.h; do
    if [ -f "$FILE" ]; then
      clang-format --dry-run --Werror "$FILE" 2>> "$REPORT_FILE"
      if [ $? -ne 0 ]; then
        echo "Formatting issue: $FILE" >> "$REPORT_FILE"
        FORMAT_FAIL=1
      fi
    fi
  done
done

if [ $FORMAT_FAIL -ne 0 ]; then
  echo "[clang-format] Formatting issues found." >> "$REPORT_FILE"
else
  echo "[clang-format] Passed." >> "$REPORT_FILE"
fi

cppcheck --enable=all --std=c99 --quiet -Iinclude $SRC_DIRS 2>> "$REPORT_FILE"
if [ $? -ne 0 ]; then
  echo "[cppcheck] Issues found." >> "$REPORT_FILE"
else
  echo "[cppcheck] Passed." >> "$REPORT_FILE"
fi

# create summary for slack
head -n 10 "$REPORT_FILE" > "$SUMMARY_FILE"

# if failed return errorcode
if grep -q "issue" "$REPORT_FILE"; then
  exit 1
else
  exit 0
fi
