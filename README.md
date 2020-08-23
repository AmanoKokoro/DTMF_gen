# DTMF_gen
This repository is generate dtmf signal's .wav File.
Enter any number in decimal.

# Note

Use This Page's "wave.c" and "wave.h"

[http://hooktail.org/computer/index.php?Wave%A5%D5%A5%A1%A5%A4%A5%EB%A4%F2%C6%FE%BD%D0%CE%CF%A4%B7%A4%C6%A4%DF%A4%EB](http://hooktail.org/computer/index.php?Wave%A5%D5%A5%A1%A5%A4%A5%EB%A4%F2%C6%FE%BD%D0%CE%CF%A4%B7%A4%C6%A4%DF%A4%EB)

# Usage
```
$ cd DTMF_gen
```

## Use CMake
```
$ mkdir build 
```
```
$ cd build && cmake ..
```
```
$ make
```

## Non CMake
```
$ cd src
```
```
$ gcc main.c dtmf.c wave.c -lm -o dtmf_gen
```

# Exec
```
$ ./dtmf_gen [NUMBER] [OUTPUTFILENAME]