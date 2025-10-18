#!/bin/bash

# Define the sudoers file
SUDOERS_FILE="/etc/sudoers"

# Check if the sudoers file exists
if [ ! -f "$SUDOERS_FILE" ]; then
    echo "Error: Sudoers file not found at $SUDOERS_FILE"
    exit 1
fi

# Exclude comments and check for root or privileged groups
# The regex pattern matches lines that start with a word boundary, followed by root, %wheel, or %sudo
grep -E "^[^#]*\b(root|%wheel|%sudo)\b" "$SUDOERS_FILE" > /tmp/sudoers_privileged_rules.txt

# check if the file is empty
if [ -s /tmp/sudoers_privileged_rules.txt ]; then
    echo "KO: Found rules targeting privileged users or groups:"
    cat /tmp/sudoers_privileged_rules.txt
else
    echo "OK: No rules targeting privileged users or groups were found."
fi

# Cleanup temporary file
rm -f /tmp/sudoers_privileged_rules.txt
