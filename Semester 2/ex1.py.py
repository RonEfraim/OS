
# EX1
# Name: Sahar Ithzak, Ron Efraim
# ID: 308389485, 204661425
# Program:
# User enters his choice [1-4]
# else program will exit


while True:
    choice = input("Enter your choice 1-4\n")

    ##################### Program 1 #####################
    # choice #1
    # User enters 2 numbers and program tell how many
    # digits are the same

    if choice == '1':
        str1 = input("Enter first string\n")
        str2 = input("Enter second string\n")
        counter = 0
        for i in str1:
            for j in str2:
                if i == j:
                    counter += 1

        print(counter)

    ##################### Program 2 #####################
    # choice #2
    # User enters a choice [i,e,p]
    # i: user enters a string to stack without the first
    #    char
    # e: pops an element from stack, exits if empty
    # p: prints the stacks items and their indexes
    # else: break

    elif choice == '2':
        my_stack = []
        str = ""
        while True:
            choice = input("Enter choice\n")
            if choice == 'i':
                str = input("Enter a string\n")
                str = str[1:]
                my_stack.append(str)
            elif choice == 'e':
                if len(my_stack) == 0:
                    print("Stack is empty")
                    exit(0)
                my_stack = my_stack[1:]
            elif choice == 'p':
                # index = 0
                for elem in my_stack:
                    for i, j in enumerate(elem):
                        print(i, j)
                        # index += 1
            else:
                break

    ##################### Program 3 #####################
    # choice #3
    # Program tells user who will be the last survivor:
    # king tell 100 his servants to stand in a circle
    # and gives the first servant a sword and to start
    # the round.
    # the servant will kill the next servant (#2) and
    # passes the sword on the the next living servant (#3)
    # and so forth.

    elif choice == '3':
        SIZE = 100
        arr = []
        for i in range(SIZE):
            arr.append(i + 1)

        curr = 1
        while len(arr) != 1:
            if curr == len(arr) + 1:
                curr = 1
            if curr == len(arr):
                curr = 0
            arr.pop(curr)
            curr = (curr % len(arr)) + 1
            print(arr)

    ##################### Program 4 #####################
    # choice #4
    # User enters a string
    # program prints the string in Julius Caesar's code-
    # each letter will be changed to it's ascii value + 13

    elif choice == '4':
        str = input("Enter a string\n")
        for i in str:
            if i.isalpha():
                if i.isupper():
                    i = (chr(ord(i) + 32))
                i = (chr(ord(i) - 84))
                i = (chr(ord(i) % 26))
                i = (chr(ord(i) + 97))
                i = (chr(ord(i)))
                print(i, end="")
            else:
                print(i, end="")
        print()

    ####################### Else #######################

    else:
        print("Option doesn't exit")
        exit(0)
