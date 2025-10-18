# Check if the umask value is 0077
actual_value="$(umask 2>/dev/null)"
if [[ "$actual_value" == "0077" ]]; then
  echo "OK: umask value is 0077 (expected value 0077)"
else
  echo "KO: umask value is $actual_value (expected value 0077)"
fi