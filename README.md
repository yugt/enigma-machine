# Enigma

This project implements a toy version of Enigma machine simulator in Python.
The Enigma machine was a cipher device used primarily by Nazi Germany during World War II to protect military communication.
This simulator allows you to encrypt and decrypt messages using a simplified version of the Enigma machine.

## Features

- Encrypt messages with a specified wheel order and key.
- Decrypt messages with a specified wheel order and key.
- Handle spaces and non-alphabetic characters in messages.
- Command-line interface with history support.

## Usage

### Command-Line Interface

The CLI supports the following commands:

- `a <cyphertext>`: Decrypts the cyphertext without any hints (hardest mode).
- `b <wheel> <cyphertext>`: Decrypts the cyphertext with the wheel position given (hard mode).
- `c <key> <cyphertext>`: Decrypts the cyphertext with the key given (easy mode).
- `d <wheel> <key> <cyphertext>`: Decrypts the cyphertext with the wheel position and key given.
- `e <wheel> <key> <plaintext>`: Encrypts the plaintext with the wheel position and key given.
- `h`: Prints the help message.
- `q`: Quits the program.

### Example

```bash
$ python3 script/main.py
Enigma interactive mode
usage: 
a: cyphertext (decrypt 6 * 37^2 times)
b: wheel cyphertext (decrypt 37^2 times)
c: key cyphertext (decrypt 6 times)
d: decrypt, wheel key cyphertext
e: encrypt, wheel key plaintext
h: print this help message
q: quit

Enigma> e abc key hello world
YI5A7 C7457

Enigma> d abc key yi5a7 c7457
HELLO WORLD
```

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request on GitHub.

## Acknowledgements
This project is inspired by the historical Enigma machine used during World War II.