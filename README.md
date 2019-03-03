<h1 align="center"> Keyboard Driver for Linux </h1>

# How can I run this program?

Follow these steps to test the driver:

<b>Compile "chardev_leds.c" file </b>

You can use the makefile I provide you. In the directory where files are located, type:
```
make
```
<b>Insert "chardev_leds.ko" in the Kernel</b>
```
sudo insmod chardev_leds.ko
```
<b>Create a new device file managed by the driver</b>
```
sudo mknod -m 666 /dev/leds c 243 0
```
We can check if we did these steps well, typing:
```
cat /dev/leds
```
This command will print a message

<b>Compile "leds_user.c" file</b>
```
gcc -Wall -g leds_user.c -o leds_user
```
<b>Run it</b>
```
sudo ./leds_user
```
<p>
  <div align="center">
    <img src="https://github.com/arturobp3/Keyboard_Driver/blob/master/img/terminal1.png" width="700" height="400">
    <img src="https://github.com/arturobp3/Keyboard_Driver/blob/master/img/terminal2.png" width="700" height="700">
  </div>      
</p>

# When you don't want to test this driver anymore:

<b>Remove "chardev_leds.ko" from the Kernel</b>
```
sudo rmmod chardev_leds
```

<b>Remove "/dev/leds"</b>
```
sudo rm -r /dev/leds
```
