#!/usr/bin/env bash
# Usage:  ./commit_task.sh 1      (commits Task01)    ... ./commit_task.sh 7     ./commit_task.sh readme
# First time only:
#   git init -b main
#   git remote add origin https://github.com/SKSan2007000/DSA01-192511053-Assignment2.git
set -e
case "$1" in
  readme) git add README.md REPORT.md .gitignore commit_task.sh
          git commit -m "Add README, report and .gitignore" ;;
  [1-7])  T=$(printf "Task%02d" "$1")
          git add "$T" .gitignore
          git commit -m "$T: add source, output, screenshots and notes" ;;
  *) echo "usage: $0 <1-7|readme>"; exit 1 ;;
esac
git push -u origin main
