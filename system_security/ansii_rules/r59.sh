# !/bin/bash

# Combine all repository URLs into one output
REPOS=$(grep -E '^\s*baseurl\s*=' /etc/yum.repos.d/*.repo 2>/dev/null | awk -F '=' '{print $2}' | tr -d ' ')

# Search for non-official repositories (basic check excluding common official domains)
# The regex pattern matches lines that start with http:// or https://, but exclude common official domains
echo "$REPOS" | grep -E -i 'http://|https://' | grep -Ev '(redhat\.com|centos\.org|fedora\.project|local)' > /tmp/non_official_repos.txt

if [ -s /tmp/non_official_repos.txt ]; then
    echo "KO: Found non-official repositories:"
    cat /tmp/non_official_repos.txt
else
    echo "OK: All repositories appear to be official or internal."
fi

# Cleanup
rm -f /tmp/non_official_repos.txt
