# Introduction

A simple ransomware that recursively encrypts directories with AES, afterwards encrypts the generated AES key with a pre planted RSA public key and writes that encrypted key to the Desktop.
This key encrypts key method makes it inecessary for exfiltration of keys and makes it more securte for the attacker/red teamer.
By default it will attack "Desktop", "Documents", "Pictures" and "Downloads" but this can be easily edited by adding files to the variable "targetDirs" in main.c


# Usage

The key generator will generate 2 files, a key pair (wich you keep to yourself) and the public key
- Input the content of the public key file into the "asymKeyData" variable in the begining of the keyCleaner.h file
- (OPTIONAL) Change directories to attack (reffer to "Introduction", 4th line)
- Compile and send to target

# TO DO:

- Fully test in VM
- Change the name of encrypted files for ease of use (for example adding ".ENCR")
- Decryption tool built into the ransomware that automatically decrypts
