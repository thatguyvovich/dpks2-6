# dpks2-6
## *Basic-2 is currently being developed*

Lab6 of RDCS.

## What is this?
This is a module, which allocates memory according to the custom parameter `hello_param`. On top of that, it allows for some basic debugging.

## How to compile?
Use `make`.

## How to run?
Use `insmod hello_param=[number]`.

## What does it do?
It allocates memory for the custom data structure which encapsulates time stamps before `printk` function is called and after. Upon using `rmmod`, module prints out information on how long `printk` took. It also creates a segmentation fault for third memory allocation, and it 'blacks out' on `hello_param` higher than 10
