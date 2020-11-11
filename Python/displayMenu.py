# Function to set a timescale
def timescale():
    ts = input("\nSet time unit and time precision for timescale: ")
    return ts

# Function to ask user for a desirable clk signal


def getClk():
    clk = input("\nDefine new name of your clock: ")
    return clk


def getRst():
    rstName = input("\nDefine the new name of your reset: ")
    active = input("\nIs it active high? [y/n]: ").lower()
    while active != 'y' and active != 'n':
        print("Invalid option!\n")
        active = input("\nIs it active high? [y/n]: ").lower()
    return (rstName, active == "y")


# Function to generate stimulus based on User-defined iterator, by default generates up to 10 stimulus


def selectForIterations():
    forIt = input("\nInput loop for iterations (default 10): ")

    return int(forIt) if forIt.isnumeric() else 10


# Function to display a menu to the user and select what kind of smtimulus wants to generate
def displayMenu(varTuple):

    print("\nFor input %s %s, what do you want to do?" %
          (varTuple[1], varTuple[0]))
    print("1 -> Random signal generation\n2 -> Ascending counter\n3 -> Descending counter")
    opt = input("Option: ")
    while opt != "1" and opt != "2" and opt != "3":
        print("Invalid option!\n")
        print(
            "1 -> Random signal generation\n2 -> Ascending counter\n3 -> Descending counter")
        opt = input("Option: ")

    if opt == "1":
        return "random"
    elif opt == "2":
        return "up"
    else:
        return "down"


def printHelp():
    print("""
    python3 main.py [OPTIONS] [FILENAME]

    Options:

    -r --> All variables are assigned a random number $urandom() in every iteration
    -a --> Variables are assigned a number that increases by 1 with each iteration
    -d --> Variables are assigned a number that starts at the for loop limit and decreases by 1 every iteration
    -t --> Override default timescale of 1ns/1ps
    -s --> Override default reset name (rst) and active high
    -c --> Override default clock name (clk)
    -f --> Override default number of iterations (10)

    With no option, the user will be prompted to select the value to assign for each variable, the loop iterations 
    will be set to 10, the clock signal is expected to be named "clk" and the reset signal is expected to be "rst"
    and active high, and the timescale is set to 1ns/1ps.
    
    """)
    return
