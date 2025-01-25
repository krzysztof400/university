#!/bin/bash

# Define the sudoers file
SUDOERS_FILE="/etc/sudoers"

# Check if the sudoers file exists
if [ ! -f "$SUDOERS_FILE" ]; then
    echo "Error: Sudoers file not found at $SUDOERS_FILE"
    exit 1
fi

# Exclude comments and look for negations
grep -E "^[^#]*!" "$SUDOERS_FILE" > /tmp/sudoers_negations.txt

if [ -s /tmp/sudoers_negations.txt ]; then
    echo "KO: Negation policies found in the sudoers file."
else
    echo "OK: No negation policies found in the sudoers file."
fi

# Cleanup temporary file
rm -f /tmp/sudoers_negations.txt

