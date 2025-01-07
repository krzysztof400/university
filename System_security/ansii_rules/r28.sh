recommended_partitions=(
    "/boot"
    "/home"
    "/tmp"
    "/var"
    "/var/tmp"
    "/var/log"
    "/opt"
    "/usr"
)

cd /
for partition in "${recommended_partitions[@]}"; do
    if [ -d "$partition" ]; then
        echo OK: "$partition is a separate partition"
    else
        echo KO:"$partition is not a separate partition"
    fi
done