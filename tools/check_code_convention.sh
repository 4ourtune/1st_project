#!/bin/bash
echo "[Code Convention Check - Linux]"

# Define report path
REPORT_FILE="tools/code-convention-report.txt"

# Clear previous report
rm -f "$REPORT_FILE"

# Define source directories
SRC_DIRS=()
[[ -d "src" ]] && SRC_DIRS+=("src")
[[ -d "include" ]] && SRC_DIRS+=("include")
[[ -d "tools/test_code" ]] && SRC_DIRS+=("tools/test_code")

# If no source folders found, skip check
if [ ${#SRC_DIRS[@]} -eq 0 ]; then
  echo "No source directories found. Skipping checks." >> "$REPORT_FILE"
  cat "$REPORT_FILE"
  exit 0
fi

# Step 1: clang-format
echo "[Step 1] clang-format check" >> "$REPORT_FILE"
FORMAT_FAIL=0
for DIR in "${SRC_DIRS[@]}"; do
  for FILE in "$DIR"/*.[ch]; do
    [ -e "$FILE" ] || continue
    clang-format --dry-run --Werror "$FILE" 2>> "$REPORT_FILE"
    if [ $? -ne 0 ]; then
      echo "Formatting issue: $FILE" >> "$REPORT_FILE"
      FORMAT_FAIL=1
    else
      echo "OK: $FILE" >> "$REPORT_FILE"
    fi
  done
done

# Step 2: cppcheck
echo "" >> "$REPORT_FILE"
echo "[Step 2] cppcheck" >> "$REPORT_FILE"

INCLUDE_OPTION=""
[[ -d "include" ]] && INCLUDE_OPTION="-I include"

cppcheck --enable=all --std=c99 --quiet $INCLUDE_OPTION "${SRC_DIRS[@]}" 2>> "$REPORT_FILE"

# Show result
cat "$REPORT_FILE"

# Check for cppcheck errors
grep "error:" "$REPORT_FILE" > /dev/null
CPPCHECK_FAIL=$?

# Summary
if [ $FORMAT_FAIL -ne 0 ]; then
  echo "clang-format check failed." >> "$REPORT_FILE"
fi
if [ $CPPCHECK_FAIL -eq 0 ]; then
  echo "cppcheck found errors." >> "$REPORT_FILE"
fi

if [ $FORMAT_FAIL -ne 0 ] || [ $CPPCHECK_FAIL -eq 0 ]; then
  exit 1
fi

echo "All checks passed." >> "$REPORT_FILE"
exit 0
