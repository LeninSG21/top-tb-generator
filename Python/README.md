# Python Testbench Generator

## Usage

To execute the TB generator, run the main.py file in the console and provide the name of the file you want to open.

For example:

`python3 main.py <Users/Documents/Test.sv> [OPTIONS]`

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
| -- help | display a guide                                                                                           |

### Examples

#### With no flags

### With some flags
