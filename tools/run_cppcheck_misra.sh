#!/bin/bash
set -e

echo "[🔍 Running cppcheck with MISRA addon]"

# 경로 설정
CPPCHECK_EXE="tools/cppcheck/bin/debug/cppcheck.exe"
MISRA_PATH="$(pwd)/tools/cppcheck/addons/misra.py"
SRC_DIRS=("src" "include" "tools/test_code")
REPORT="tools/cppcheck_report.txt"

# 리포트 초기화
> "$REPORT"

# cppcheck 실행
"$CPPCHECK_EXE" \
  --enable=all \
  --std=c99 \
  --addon="$MISRA_PATH" \
  -Iinclude "${SRC_DIRS[@]}" 2> "$REPORT"

echo "[cppcheck completed]"

# 결과 분석 후 위반 있으면 실패 처리
if grep -q "^\[.*\]:" "$REPORT"; then
  echo "[cppcheck found issues]"
  cat "$REPORT"
  exit 1
else
  echo "[No issues found]"
  exit 0
fi
