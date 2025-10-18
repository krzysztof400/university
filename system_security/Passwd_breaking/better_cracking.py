import subprocess

def breaking_password():
    strings = subprocess.run(['strings', 'final'], capture_output=True, text=True)
    strings = strings.stdout.split('\n')
    for string in strings:
        result = subprocess.run(['./final', string], capture_output=True, text=True)
        if "Access granted" in result.stdout:
            print(f"Password found: {string}")
            return
        
if __name__ == "__main__":
    breaking_password()

breaking_password()

# The code snippet above is a better version of the previous one.
# It uses the strings command to extract all the strings from the binary file and then tries each string as a password.
# Because of that type of attack, we have to improve our binary file.