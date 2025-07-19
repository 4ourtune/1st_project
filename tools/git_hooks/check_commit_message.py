#!/usr/bin/env python3
"""
Commit message convention check script
Location: tools/git_hooks/check_commit_message.py
"""

import sys
import re
import os

def read_commit_message():
    """Read the commit message from file."""
    if len(sys.argv) < 2:
        print("Commit message file path not provided.")
        sys.exit(1)

    commit_msg_file = sys.argv[1]

    if not os.path.exists(commit_msg_file):
        print(f"Commit message file not found: {commit_msg_file}")
        sys.exit(1)

    try:
        with open(commit_msg_file, 'r', encoding='utf-8') as f:
            commit_msg = f.read().strip()
        return commit_msg
    except UnicodeDecodeError:
        try:
            with open(commit_msg_file, 'r', encoding='cp949') as f:
                commit_msg = f.read().strip()
            return commit_msg
        except:
            print("Unable to read the commit message file.")
            sys.exit(1)

def check_commit_message(commit_msg):
    """Validate the commit message format."""

    if not commit_msg:
        return False, "The commit message is empty."

    lines = commit_msg.split('\n')
    actual_lines = [line for line in lines if not line.startswith('#')]

    if not actual_lines or not actual_lines[0].strip():
        return False, "No valid commit message found."

    first_line = actual_lines[0].strip()

    # Expected format: [JiraKey] / Description
    pattern = r'^\[([A-Z]+-\d+)\]\s*/\s*(.+)$'
    match = re.match(pattern, first_line)

    if not match:
        return False, f"Commit message format is invalid: {first_line}"

    jira_key = match.group(1)
    work_content = match.group(2)

    if len(work_content) < 3:
        return False, "Commit description is too short. Please use at least 3 characters."

    if len(first_line.encode('utf-8')) > 200:
        return False, f"Commit message is too long ({len(first_line.encode('utf-8'))} bytes). Please keep it within 200 bytes."

    return True, f"Valid commit message format. (Jira Key: {jira_key})"

def print_commit_guide():
    """Print commit message format guide."""
    print("\n=== Commit Message Convention Guide ===")
    print("Format: [JiraKey] / Description")
    print("\nExamples:")
    print("OK: [KEY-001] / Implement login API")
    print("OK: [KEY-002] / Improve error messages")
    print("OK: [KEY-003] / Add user permission check logic")
    print("\nRules:")
    print("- Jira Key format: [A-Z]+-[number]")
    print("- Square brackets [] required")
    print("- Spaces required around slash (/)")
    print("- Description must be at least 3 characters")
    print("- Entire message should be 50 bytes or less (UTF-8)")
    print("===========================================\n")

def main():
    """Main function"""
    commit_msg = read_commit_message()

    is_valid, message = check_commit_message(commit_msg)

    if is_valid:
        print(f"OK: {message}")
        sys.exit(0)
    else:
        print(f"ERROR: {message}")
        print_commit_guide()
        sys.exit(1)

if __name__ == "__main__":
    main()
