#!/bin/bash

echo "[Code Convention Check - Linux]"

# 이동: script가 tools/ 안에 있다는 전제
cd "$(dirname "$0")/.."

# 대상 디렉토리 설정
SRC_DIRS=("src" "include" "tools/test_code")
REPORT_FILE="tools/code-convention-report.txt"
SUMMARY_FILE="tools/code-convention-summary.txt"

# 이전 결과 파일 제거
rm -f "$REPORT_FILE" "$SUMMARY_FILE"

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

echo "" >> "$REPORT_FILE"
echo "[Step 2] cppcheck" >> "$REPORT_FILE"

# cppcheck 수행
CPPCHECK_FAIL=0
cppcheck --enable=all --std=c99 --quiet -Iinclude "${SRC_DIRS[@]}" 2>> "$REPORT_FILE"
if grep -q "error:" "$REPORT_FILE"; then
  CPPCHECK_FAIL=1
fi

# 요약 저장 (Slack 메시지 용)
if [ $FORMAT_FAIL -ne 0 ]; then
  echo "clang-format: FAILED" >> "$SUMMARY_FILE"
else
  echo "clang-format: PASSED" >> "$SUMMARY_FILE"
fi

if [ $CPPCHECK_FAIL -ne 0 ]; then
  echo "cppcheck: FAILED" >> "$SUMMARY_FILE"
else
  echo "cppcheck: PASSED" >> "$SUMMARY_FILE"
fi

# 전체 결과 출력
cat "$REPORT_FILE"

# 종료 코드 처리
if [ $FORMAT_FAIL -ne 0 ] || [ $CPPCHECK_FAIL -ne 0 ]; then
  exit 1
else
  echo "All checks passed." >> "$REPORT_FILE"
  exit 0
fi
