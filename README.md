# Kernel module demonstrating the EL3 fibonacci compute service

A simple linux kernel module that provides a `proc_fs` interface to the EL3 fibonacci compute service

## Building

```sh
make KERNEL=/path/to/linux-apple CROSS=/prefix/to/aarch64-compiler-
```

Default values:

- `KERNEL = ../linux-apple`
- `CROSS = aarch64-none-linux-gnu-`

## Usage

```sh
# Load module
insmod el3_demo_fib.ko
# To send a new request for the N-th fibonacci number
echo $N > /proc/el3_fib_svc
# To read the results from the last request
cat /proc/el3_fib_svc
# When done
rmmod el3_demo_fib
```

## Example Output

```text
/home/ssp # insmod el3_demo_fib.ko 
/home/ssp # echo 10 > /proc/el3_fib_svc 
/home/ssp # cat /proc/el3_fib_svc 
F(10) = 55
Param reg check(x1):
    Pre-SMC:  0x000000000000000A
    Post-SMC: 0x000000000000000A
/home/ssp # echo 11 > /proc/el3_fib_svc 
/home/ssp # cat /proc/el3_fib_svc 
F(11) = 89
Param reg check(x1):
    Pre-SMC:  0x000000000000000B
    Post-SMC: 0x000000000000000B
/home/ssp # echo 12 > /proc/el3_fib_svc 
/home/ssp # cat /proc/el3_fib_svc 
F(12) = 144
Param reg check(x1):
    Pre-SMC:  0x000000000000000C
    Post-SMC: 0x000000000000000C
/home/ssp # 
```

More output can be found in the kernel log (`dmesg`).

The `Pre-SMC` and `Post-SMC` values are in place to demonstrate the capability for restoring register states after exiting EL3.

The `N` parameter is passed to the service in register `x1` and this register is modified during the EL3 execution. The original value of `N` is stored in `Pre-SMC`

After the SMC call, the register `x1` is read again, and its value is stored in `Post-SMC`.

We expect the two values to match.
