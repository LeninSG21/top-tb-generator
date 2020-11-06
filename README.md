# top-tb-generator

## Usage

To execute the tb generator, run the main.py file in the console and provide the name of the file you want to open.

For example:

`python3 ./Python/main.py <pathToFile>.sv`

When executing the script, it will prompt some inputs to determine the signal generation for the input variables detected. the options are:

| selector | option     | description                                                                                                                                                                                    |
| -------- | ---------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1        | random     | assigns a random value in each iteration of the for loop                                                                                                                                       |
| 2        | ascending  | assigns to the variable the value of the for loop's iterator. It starts at zero and goes up to the value given as a limit minus 1. The value is truncated to the number of bits the signal has |
| 3        | descending | Starts the variable in the value of the for loop minus 1 and it ends when the value is zero. It's also truncated to the bus size of the variable                                               |

The vector generation is done inside a for loop that assigns values to all the input variables detected. The user is prompted to select the number of iterations, which is 10 by default.

## Example

Suppose a module contains three inputs:

-   x
-   [2:0] y
-   [3:0] z

If the user selects the following options for the variables:

-   x --> ascending
-   y --> random
-   z --> descending

Also, it sets the for loop limit to 16. Thus, the for loop would look like this

```sv
    for(integer i = 0; i < 16; i++) begin
        #2
        x = i;
        y = $urandom();
        z = 15 - i;
    end
```

By default, the _i_ value assigned to the _x_ will only be the lsb, since _x_ is only a bit long. The random value given to _y_ will also be in the range 0..7, since _y_ has a bus size of three bits. On the other hand, _z_ will go from 15 to 0 without truncating the bits.
