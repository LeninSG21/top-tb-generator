# C++ with config Testbench Generator

This generator, unlike the Python and C++ implementations, allows the user to select a configuration file to assign the desired overrides without having to type them manually every time the code is executed.

## Usage

First, the code must be compiled. To do so, run the following command on the terminal with g++ installed.

```sh
$ g++ main.cpp -o tb-generator
```

After doing so, exexcute the code by running

```sh
$ ./tb-generator [FILENAME] [OPTIONS | CONFIG_FILE]
```

It is important to note that for this code, the FILENAME **MUST** be the first argument. Afterwards, the OPTIONS and the CONFI_FILE have no order. However, if a config file is present, the options **will be ignored**.

### Options

Like the C++ normal implementation, the user can select any of these flags to modify the testbench to suits its needs. The flags can be added individually or all at the same time.

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

### Config file

The config file is a simple .txt that contains the flags and the values to override. The following table shows the possible flags and the values each one can have.

| Flag        | Values                                                                                                                                          |
| ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| RST         | string → name of the reset signal to replace the default value of “rst”                                                                         |
| ACTIVE_HIGH | 1 → the reset is active high<br> 0 → the reset is active low                                                                                    |
| CLK         | string → name of the clock signal to replace the default value of “clk”                                                                         |
| FOR_IT      | positive integer, which represents the desired number of iterations                                                                             |
| SCALE       | string to replace the default timescale (1ns/ps)                                                                                                |
| FUNC        | r → random values to all the input signals <br>a → ascending values to all the input signals <br>d → descending values to all the input signals |

The flags do not have to be in the order presented in the table, and all of them are optional. However, if one of the flags is not present in the config file, the default value will be taken. In the case of the FUNC flag to determine the stimulus of the signals, if the flag is not given, the user will be prompted to select the function type for each input signal. However, if the FUNC flag exists, it will assign said value to every signal.

### Examples

#### Without config file

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

Unlike the equivalent example in the [C++ examples section](https://github.com/LeninSG21/top-tb-generator/tree/main/CPP#examples), this example **will not work** since the first argument is not the input file

```sh
$ ./tb-generator -a -c ../ALU/alu.sv -s

```

#### With config file

##### First Example

Given the config file named _config.txt_

```
RST=reset
CLK=reloj
FOR_IT=32
SCALE=10ns/1ps
```

And the command

```sh
$ ./tb-generator ../ALU/alu.sv config.txt
```

The behaviour would be the following:

1. The code will read the config.txt file that is in the same directory as the executable
2. It will extract the flag information in the file
3. It will set the reset signal name to **reset**, overriding the default rst
4. It will set the clock signal name to **reloj**, overriding the default clk
5. The for iterations will be set to 32
6. The timescale will be set to 10ns/1ps

By omission, the user will be prompted to select the stimulus for each input signal, the reset will be considered active high

##### Second Example

Given the config file named _config.txt_

```
ACTIVE_HIGH=0
FUNC = d
```

And the command

```sh
$ ./tb-generator ../ALU/alu.sv config.txt -rscf
```

The behaviour would be the following:

1. The code will read the config.txt file that is in the same directory as the executable
2. It will extract the flag information in the file
3. It will set the reset signal to **active low**, overriding the default active high
4. It will set the stimulus as descending for all the variables

By omission, the clock signal name is expected to be **clk**, the reset is expected to be named **rst**, the for iterations will be **10** and the timescale is left as **1ns/1ps**.

Although the user gave some command line flags, these are ignored since a config file was detected.
