```
  _____                _ __  __          Author: Hans-Helge Buerger
 |  __ \              | |  \/  |         Version: v1.0  
 | |__) |___  __ _  __| | \  / | ___     Date: 19. Jan 2014  
 |  _  // _ \/ _` |/ _` | |\/| |/ _ \
 | | \ \  __/ (_| | (_| | |  | |  __/
 |_|  \_\___|\__,_|\__,_|_|  |_|\___|

```


kbs-emotion
===========
This is part of a university project for _Knowledge Based Systems_. This project takes measured data of faces and tries to calculate the person's emotion. Therefore it uses _evidences_ and _dempster / shafer rule_.

## Program
The program is written in C and takes a CSV file (see `Files/`) with measured data.

### Arguments

```
	usage: ./kbs [-v] [-a] [-h] -f FILE

		-v		verbose output
		-a		calculate all in one run. If -a is not given the
				user is asked to continue or quit after each calculation.
		-h		this usage dialog
		-f FILE		relative file path to CSV file
```

### Makefile
A simple `makefile` is provided to compile the project and for easy execution with both files.