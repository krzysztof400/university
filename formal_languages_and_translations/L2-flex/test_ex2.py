#!/usr/bin/env python3

# 1. BASIC COMMENTS
# This entire line should be removed.
x = 10  # This inline comment should be removed.
y = 20  ##### Multiple hashes should still be removed.

# 2. STRINGS WITH HASHES (Should remain)
print("This is a string with a # inside.")
print('This is single quoted with a # inside.')

# 3. QUOTES INSIDE QUOTES (Should remain)
print("It's a beautiful day.")      # Single inside double
print('He said: "Hello world!"')    # Double inside single

# 4. ESCAPED QUOTES (The Tricky Part)
# The lexer must not think the string ends at the escaped quote \"
print("This string has an escaped \" quote inside it.") 
print('This string has an escaped \' quote inside it.')

# 5. ESCAPED BACKSLASHES (The "Devil" Case)
# The lexer must see that \\ is a literal backslash, and the next " ends the string.
path = "C:\\Windows\\System32" # Comment after valid string
tricky = "End with escaped backslash \\" # Comment after

# 6. TRIPLE QUOTES (Multiline Strings)
# Everything inside here must remain, even newlines and hashes.
doc_double = """
    Start of docstring.
    # This is NOT a comment.
    It is part of the string.
    End of docstring.
"""

doc_single = '''
    Another docstring.
    "Quotes" and 'Quotes' work here.
'''

# 7. COMPACT TRIPLE QUOTES
s = """Single line triple quote""" # Comment here

# 8. PREFIXES & EMPTY STRINGS
empty = "" # Empty string
f_string = f"Score: {x} # Not a comment"
r_string = r"Raw string with #"

print("End of test.")