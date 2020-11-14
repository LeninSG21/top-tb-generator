# C++ with config Testbench Generator

## Usage

First, the code must be compiled. To do so, run the following command on the terminal with g++ installed.

```sh
$ g++ main.cpp -o tb-generator
```

After doing so, exexcute the code by running

```sh
$ ./tb-generator [OPTIONS] [FILENAME]
```

It is important to note that for this code, the FILENAME and the OPTIONS have no order.

### Options

The user can select any of these flags to modify the testbench to suits its needs. The flags can be added individually or all at the same time.

| Flag    | action                                                                                                    |
| ------- | --------------------------------------------------------------------------------------------------------- |
| -r      | assings the random stimulus to all input signals                                                          |
| -a      | assings the ascending stimulus to all input signals                                                       |
| -d      | assings the descending stimulus to all input signals                                                      |
| -s      | prompts the user to select the name of the reset signal and to specify if it is active high or active low |
| -c      | prompts the user to select the clock's name                                                               |
| -f      | prompts the user to input the desired number of iterations for the main sequence                          |
| -t      | prompts the user to select the desired timescale                                                          |
| -- help | display a guide and provides additional information                                                       |

### Examples

A valid execution command with no flags would be the following:

```sh
$ ./tb-generator ../ALU/alu.sv
```

In this case, the input file is in a folder named ALU that is outside the directory of the tb-generator. The output file would be placed inside the ALU directory with the name _alu_tb.sv_.

Another example with all the flags would be

```sh
$ ./tb-generator ../ALU/alu.sv -rcsft
```

In this case, all the possible overrides are given and the stimulus is set as random for all the input signals.

Finally, this example will override the clock and reset and will set all the input signals stimulus to ascending.

```sh
$ ./tb-generator -a -c ../ALU/alu.sv -s
```
