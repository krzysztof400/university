$result = $(sysctl kernel.modules_disabled)

if [ "$result" == "kernel.modules_disabled = 1" ]; then
    echo "OK: kernel.modules_disabled = 1"
else
    echo "KO: kernel.modules_disabled = 0"
fi