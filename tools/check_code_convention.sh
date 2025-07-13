#!/bin/bash

echo "[Code Convention Check - Linux]"

SRC_DIRS=("src" "include" "tools/test_code")
REPORT_FILE="tools/code-convention-report.txt"
SUMMARY_FILE="tools/code-convention-summary.txt"

mkdir -p tools
: > "$REPORT_FILE"
: > "$SUMMARY_FILE"

FORMAT_FAIL=0
CPPCHECK_FAIL=0

# Step 1: clang-format
echo "[Step 1] clang-format check" >> "$REPORT_FILE"
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
cppcheck --enable=all --std=c99 --quiet -Iinclude "${SRC_DIRS[@]}" 2>> "$REPORT_FILE"
if grep -q "error:" "$REPORT_FILE"; then
  CPPCHECK_FAIL=1
fi

# Summary 생성
if [ $FORMAT_FAIL -ne 0 ]; then
  echo "clang-format check failed." >> "$SUMMARY_FILE"
fi
if [ $CPPCHECK_FAIL -ne 0 ]; then
  echo "cppcheck found errors." >> "$SUMMARY_FILE"
fi

if [ $FORMAT_FAIL -eq 0 ] && [ $CPPCHECK_FAIL -eq 0 ]; then
  echo "All checks passed." >> "$SUMMARY_FILE"
fi

# 전체 출력 (for GitHub Actions)
cat "$REPORT_FILE"
cat "$SUMMARY_FILE"

# 종료 코드
if [ $FORMAT_FAIL -ne 0 ] || [ $CPPCHECK_FAIL -ne 0 ]; then
  exit 1
fi

exit 0
