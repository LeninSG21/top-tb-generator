
def displayMenu(varTuple):
    print("For input %s %s, what do you want to do?" %
          (varTuple[1], varTuple[0]))
    print("1 -> Random signal generation\n2 -> Up-counting\n3 -> Descending counter")
    opt = input("Option: ")
    while opt != "1" and opt != "2" and opt != "3":
        print("Invalid option!\n")
        print("1 -> Random signal generation\n2 -> Up-counting\n3 -> Descending counter")
        opt = input("Option: ")

    if opt == "1":
        return "random"
    elif opt == "2":
        return "up"
    else:
        return "down"
