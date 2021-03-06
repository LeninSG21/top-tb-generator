# Python Testbench Generator

## Usage

To execute the TB generator, run the main.py file in the console and provide the name of the file you want to open.

For example:

```sh
$ python3 .\Python\main.py <Path to the .sv file> [OPTIONS]
```

### Options

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

With no option, the user will be prompted to select the value to assign for each variable, the number of iterations will be set to 10, the clock signal is expected to be named _"clk"_ and the reset signal is expected to be _"rst"_ and **active high**, and the timescale is set to 1ns/1ps.

### Example

In this example, the file **PC.sv** is in the folder _Downloads_ and the testbench file will be generated on the same path where the .sv file is located, with the name _PC_tb.sv_.

```sh
$ python3 .\Python\main.py C:\Users\...\Downloads\PC.sv -r
```

In this case, stimuli generated are set to _random_ for all input signals. Due to no other flag were use, some values will be generated by default, such as the number of iterations and the timescale.

As soon as you hit **Enter**, the terminal will show you something like this:

```sh
Welcome to the testbench generator!
Done
```
