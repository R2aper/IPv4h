# IPv4 for Humans

A simple C library and command-line tool for working with IPv4 addresses.

## Description

This project provides a library and a command-line tool to make working with IPv4 addresses more convenient. It allows you to:

- Parse IPv4 addresses from strings.
- Get information about an IPv4 address, such as its class, network address, broadcast address, and the number of available hosts.
- Represent IPv4 addresses with or without CIDR notation.

## Building

To build the project, you can use the provided `Makefile`.

### Dependencies

- `gcc`
- `make`

### Instructions

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/your-username/ipv4h.git
    cd ipv4h
    ```

2.  **Build the project:**

    ```bash
    make
    ```

    This will create the `ipv4h` executable in the root directory.

3.  **Build with debugging symbols:**

    ```bash
    make debug=1
    ```

4.  **Build with optimizations:**

    ```bash
    make optimize=1
    ```

5.  **Clean the build files:**

    ```bash
    make clean
    ```

## Usage

The `ipv4h` command-line tool allows you to get information about an IPv4 address.

```
Usage: ipv4h [FLAGS]...[Ipv4 address]
Getting info of ipv4 address in human format
Flags:
--no-cidr       Don't use CIDR notation
--network       Print only network address
--broadcast     Print only broadcast address
--class         Print only class of ipv4 address
--hosts         Print only number of available hosts
-h              Print this help message
```

### Example

```bash
./ipv4h 192.168.1.10/24
```

**Output:**

```
IPv4: 192.168.1.10/24
Network address: 192.168.1.0/24
Broadcast address: 192.168.1.255/24
Available hosts: 254
Class: 'C'
```
