#!/bin/bash
#
# A script for clang-format-diff validation. Pulls a diff of the current branch
# from git. Suggests clang-format replacements for any violations within the
# diff. A patch is generated for the user to apply.
#
# ${CLANG_FORMAT_DIFF} - Path to clang-format-diff.py on your system.
# ${DIFF_FILTER_LIST} - An optional filetype filter list.
#
# Path Output: clang_format.patch

# Don't run this script with pending changes! It may modify your files.
if [ -n "$(git status --porcelain)" ]; then
	echo "ERROR: Changes reported by git, revert and retry"
	exit 1
fi

# The tool clang-format-diff.py does not have a standard location.
if [ -z ${CLANG_FORMAT_DIFF} ] || [ ! -x ${CLANG_FORMAT_DIFF} ]; then
	echo "ERROR: Please set CLANG_FORMAT_DIFF and retry"

	# You may find the file at one of the following locations:
	#  - /usr/lib/llvm/11/share/clang/clang-format-diff.py
	#  - /usr/share/clang/clang-format-12/clang-format-diff.py
	exit 1
fi

if [ -z ${DIFF_FILTER_LIST} ]; then
	DIFF_FILTER_LIST="*.cpp *.c *.h"
fi

if [ -z ${BRANCH_POINT} ]; then
	BRANCH_POINT=$(git merge-base --fork-point ${BRANCH_POINT})
fi

# Copy `.clang-format`, skipping if one already exists
cp -n contrib/clang-format.txt .clang-format

# Apply all clang-format-diff changes to the working directory
git diff -U0 --no-color ${BRANCH_POINT} -- ${DIFF_FILTER_LIST} | ${CLANG_FORMAT_DIFF} -i -p1

# Create patch file of all clang-format suggested changes
git diff > clang_format.patch

# Remove patch file if empty
[ -s clang_format.patch ] || rm clang_format.patch
