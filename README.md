# Testbench Generator

Inside this repo, you can find three different testbench generators:

-   Python TB Generator
-   C++ TB Generator
-   C++ with Config Generator

While the Python and C++ codes are almost equivalent, the C++ with config allows the user to input a config file to the code, so that it generates the stimulus without prompting the user to select them manually (more information on this is given later). Apart from that, the three generators follow the same methodology to create the testbench.

The generator is built to receive a .sv file and to generate a testbench ready to be executed in EDA Playground. It extracts the module name, the parameters and the signals in the top design provided. Afterwards, it instantiates the module, it initializes the variables and it generates the stimulus for each signal.

The testbench generators rely heavily on regex, since it allows the detection of the different components in the text.

## Information extraction

Consider the following module in verilog:

```sv
module ALU(input [7:0] A, B, input logic [2:0] SEL, output [7:0] OUT);

    output reg ZERO;

    /*
        SOME CODE
    */

endmodule
```

From this, it is seen that the module's name is **ALU**. Also, three different inputs and two outputs are defined. The testbench generator receives this information (along with the rest of the code in the given file) and it produces the appropriate testbench.

### Module Name

To obtain the module's name, the following regex was used:

`module\s+([_a-zA-Z]\w*)`

With this, the first capturing group of the match returns the name of the module. In the example above, the match would be **ALU**

With this name, the testbench module is created by adding a **\_tb** after the module's name:

`module <modules-name>_tb;`

### Signal detection

The signal detection regex is in charge of detecting every input, ouput and inout defined in the top module. This requires multiple capturing groups to determine the type of signal, the bus size and the name.

The regex to do it is the following:

`(input|output|inout)(\s+(reg|logic))?(\s*\[[^\]]+\]\s*|\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\w*(,\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\w*)*)`

With this regex, each match returns six capturing groups. However, only four of them are relevant:

-   1st group: signal type (_input_, _output_, _inout_)
-   3rd group: tells if the signal is _reg_ or _logic_
-   4th group: gives the bus size of the signal
-   5th group: returns the variables associated to the signal separated by a comma.

So, for the example above, the matches for all the signals would be the following:

| 1st group | 3rd group | 4th group | 5th group |
| --------- | --------- | --------- | --------- |
| input     |           | [7:0]     | A, B      |
| input     | logic     | [2:0]     | SEL       |
| output    |           | [7:0]     | OUT       |
| output    | reg       |           | ZERO      |

By separating by commas the fifth group, it is possible to obtain all the variables that are associated with the given type and size.

All the generators save this information in dictionaries, one for each signal type, where the key is the name of the variable and the value is a structure that contains the name, size, if it is a reg or a logic and the stimulus to be given to this variable (the stimulus generations is detailed thoroughy in the [Main Sequence section](https://github.com/LeninSG21/top-tb-generator#main-sequence)). The difference between them is the structure, since Python saves them as an array and in C++ a struct is defined.

### Parameter Detection

Suppose some parameters are added in the desing, as follows:

```sv
module ALU
    #(parameter BUS_SIZE = 4'h10, parameter MUX_SIZE = 3)
    (input [BUS_SIZE-1:0] A, B, input logic [MUX_SIZE-1:0] SEL,
    output [BUS_SIZE-1:0] OUT);

    output reg ZERO;

    /*
        SOME CODE
    */

endmodule
```

The regex to detect the parameters is the following:

`(parameter)\s+(\w*)\s*\=\s*((\d+\'(b|h|d))?\w+)`

Six capturing groups are defined. However, for the parameter instantiation, only the full match is relevant, which is given by the first capturing group. In the testbench, each parameter is included as a "copy-paste", just adding a semicolon at the end of each parameter. In the case of the signals that use a parameter in the bus size, the regex is made to detect them and add them exactly the same, so it does not affect the behavior.

## Testbench generation

The template of the testbench is as follows:

```sv

`timescale {scale}

module {moduleName}_tb;

    {parameters}
    {registers}
    {wires}
    {moduleName} UUT(.*);

    initial begin
        {variableInit}

        {mainSequence}

    end
    {clockSignal}
endmodule

```

The values enclosed in curly braces represent string variables that are obtained by the generator according to the signals in the top design and some other flags.

-   Scale: represents the timescale of the testbench
-   ModuleName: the name of the top module, extracted with the regex
-   Parameters: the parameters extracted from the top module, with the appropriate format
-   Registers: all the inputs and inouts of the top are defined as registers, following the syntax **reg {busSize} {signalName};**. This information is contained in the structure of each signal, saved in the input and inout dictionaries.
-   Wires: all the outputs of the top are defined as wires, with the syntax **wire {busSize} {signalName};**. This information is contained in the structure of each signal, saved in the output dictionary.

The variable initialization and main sequence are explained with more detailed in the following sections.

### Variable init

All the input variables found are initialized to zero, with one exception: the reset signal. By default, the reset signal is considered to be named **rst** and to be active high, so if there is a reset in the design, the testbench will initialize the reset to 1 and will send it to 0 after a delay, as follows:

```sv
    rst = 1;
    #3
    rst = 0;
```

### Main Sequence

The main sequence is a for loop that generates the stimulus for each input signal found in the design. There are three types of stimulus for the signals:

-   Random: assigns the **\$urandom()** of Verilog to the signal
-   Ascending: assings the value of the for loop iterator, so that with each iteration the signal is increased by one
-   Descending: assigns the value of the maximum value the for loop iterator reaches minus the iterator itself, so that with every iteration the value is decreased by one.

This signal stimulus is the one associated for each input signal in the variable structure.

Consider the following inputs with the structure defined:

| Signal name | size  | stimulus type |
| ----------- | ----- | ------------- |
| A           | [3:0] | random        |
| B           | [3:0] | descending    |
| SEL         | [2:0] | ascending     |

The main sequence would be:

```sv
for(integer i = 0; i < 16; i++) begin
    #2
    A = $urandom();
    B = 15 - i;
    SEL = i;
end
```

The _i_ iterator is 4 bits long, while the SEL signal is three bits. By default, Verilog truncates the value to the least significant bits, so the SEL signal would go from 0 to 7 two times, instead of displaying 0 to 15.

### Overrides

The testbench generator assumes certain things to create the template, like the reset and clock signal names, that the reset is active high, the timescale, and the number of iterations in the main sequence. The default values are shown in the subsequent table:

| Variable       | Default value    |
| -------------- | ---------------- |
| Clock signal   | clk              |
| Reset signal   | rst, active high |
| For iterations | 10               |
| Scale          | 1ns/1ps          |

Also, to determine the desired stimulus for each signal, the user is prompted to select one by one the value to assing (random, ascending or descending).

However, to give the user more flexibility, some overrides are defined for the testbench and these are given through command line arguments (both for the Python scripts and C++ executables). This overrides allow the user to give the same stimulus to all the inputs and to change the clock name, the reset name and avtive high or low, the timescale and the for iterations desired.

The flags are defined like this:

| Flag   | action                                                                                                    |
| ------ | --------------------------------------------------------------------------------------------------------- |
| -r     | assings the random stimulus to all input signals                                                          |
| -a     | assings the ascending stimulus to all input signals                                                       |
| -d     | assings the descending stimulus to all input signals                                                      |
| -s     | prompts the user to select the name of the reset signal and to specify if it is active high or active low |
| -c     | prompts the user to select the clock's name                                                               |
| -f     | prompts the user to input the desired number of iterations for the main sequence                          |
| -t     | prompts the user to select the desired timescale                                                          |
| --help | provides usage information and does not generate the testbench                                            |

By doing this, the user can either modify most of the testbench assumptions or generate a testbench without having to input anything but the execution command.

For example, with no flags, the user will be prompted to select the stimulus for each input and the default values for the variables are assigned.

If the user selects only the -r, -d, or -a flags, it will not have to input anything for the testbench to work.

Finally, if the user provides all the flags except for the stimulus ones (r, a, and d), it will have to input manually each and every override. This is true for the Python script and C++ executable. The case of the C++ code with config file solves the need of selecting the overrides manually every time the code is executed, but this is explained more thoroughly in the [CPP_Config](https://github.com/LeninSG21/top-tb-generator/tree/main/CPP_Config) documentation.
