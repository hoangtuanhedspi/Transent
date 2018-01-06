# Transent
A mini poll blocking socket file transfer software


## How to build
```
$ make clean && make
```
## How to run
Run program
```
# Run server
$ ./server <PORT>

# Run client
$ ./client <IP_SERVER> <PORT>
```

## For client
#### Program command
```
# Signup
SIGNUP user@password

# Login
LOGIN user@password

# Logout
LOGOUT user@password

# Find file
FIND [file name]

#Select client for transfer file
##[0] Client id [id] | File name [file name]
SELECT [client index number]
# Exit
EXIT
```
