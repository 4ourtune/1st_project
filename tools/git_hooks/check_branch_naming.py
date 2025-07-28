#!/usr/bin/env python3
"""
Branch naming convention check script
Location: tools/git_hooks/check_branch_naming.py
"""

import subprocess
import sys
import re

def get_current_branch():
    """Retrieve the current Git branch name."""
    try:
        result = subprocess.run(['git', 'rev-parse', '--abbrev-ref', 'HEAD'],
                                capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return None

def check_branch_naming(branch_name):
    """Check whether the branch name follows the convention."""

    # Allowlist for base branches
    if branch_name in ['develop', 'master', 'main']:
        return True, "Allowed base branch."

    # Define branch naming patterns
    patterns = {
        'feature': r'^feature/[A-Z]+-\d+-[a-z0-9A-Z-]+$',
        'release': r'^release/[A-Z]+-\d+-v\d+\.\d+\.\d+$',
        'hotfix':  r'^hotfix/[A-Z]+-\d+-[a-z0-9A-Z-]+$',
        'bugfix':  r'^bugfix/[A-Z]+-\d+-[a-z0-9A-Z-]+$'
    }

    # Check if branch matches any pattern
    for branch_type, pattern in patterns.items():
        if re.match(pattern, branch_name):
            return True, f"Branch name follows the '{branch_type}' naming convention."

    return False, f"Branch name does not follow the naming convention: {branch_name}"

def print_naming_guide():
    """Print the branch naming convention guide."""
    print("\n=== Branch Naming Convention Guide ===")
    print("OK: feature/KEY-001-login-api")
    print("OK: release/KEY-002-v1.0.0")
    print("OK: hotfix/KEY-003-fix-crash")
    print("OK: bugfix/KEY-004-null-check")
    print("OK: develop, master (allowed exceptions)")
    print("\nRules:")
    print("- Jira Key format: [A-Z]+-[number]")
    print("- Use lowercase and hyphens (-) for description")
    print("- Release branches must include version info (v1.0.0)")
    print("==============================================\n")

def main():
    """Main entry point."""
    branch_name = get_current_branch()

    if not branch_name:
        print("ERROR: Unable to determine the current branch.")
        sys.exit(1)

    is_valid, message = check_branch_naming(branch_name)

    if is_valid:
        print(f"OK: {message}")
        sys.exit(0)
    else:
        print(f"ERROR: {message}")
        print_naming_guide()
        sys.exit(1)

if __name__ == "__main__":
    main()
