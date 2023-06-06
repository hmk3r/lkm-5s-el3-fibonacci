# Kernel module demonstrating EL3

## Building

```sh
make KERNEL=/path/to/linux-apple CROSS=/prefix/to/aarch64-compiler-
```

Default values:

- `KERNEL = ../linux-apple`
- `CROSS = aarch64-none-linux-gnu-`
