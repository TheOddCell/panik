# panik
Cause a (linux) kernel panic with a custom message located at /tmp/panik

## How to use

### Build
Just run `make`! To use a diffrent kernel, set the KERN enviroment variable

### Usage
***DO NOT load this automaticly.***

To panik, just put the message in /tmp/panik, then load the module. Instant kernel panic, with synced filesystems!

