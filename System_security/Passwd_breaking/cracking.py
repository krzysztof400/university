import subprocess
import itertools

# Define character set (alphanumeric only)
charset = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'

def brute_force():
    for length in range(1, 6):  # Password length between 1 and 5
        for guess in itertools.product(charset, repeat=length):
            guess_password = ''.join(guess)
            result = subprocess.run(['./test', guess_password], capture_output=True, text=True)
            if "Access granted" in result.stdout:
                print(f"Password found: {guess_password}")
                return

brute_force()