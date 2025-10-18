#!/bin/bash

# Function to check if the file can be trusted
check_trustworthiness() {
    local file=$1
    local owner=$(stat -c %U "$file")
    local group=$(stat -c %G "$file")
    local perms=$(stat -c %a "$file")

    # Define trusted users (e.g., root)
    local trusted_users=("root")

    # Check if the file is owned by a trusted user
    if [[ ! " ${trusted_users[@]} " =~ " ${owner} " ]]; then
        echo "KO: $file is not owned by a trusted user ($owner)."
    else
        echo "OK: $file is owned by a trusted user ($owner)."
    fi

}

files=$(find / -type f -perm /6000 -ls 2>/dev/null)

# Process each file found
while read -r file; do
    # Extract file path from the ls output (assuming it's the last column)
    filepath=$(echo $file | awk '{print $NF}')
    check_trustworthiness "$filepath"
done <<< "$files"