import re
import string


def printall():
    # read every line in file, remove trainling newlines and append it to lines list
    # with "with" consruct, file will close after use
    lines = []
    with open('CS210_Project_Three_Input_File.txt') as f:
        for line in f:
            lines.append(line.rstrip())

    element_frequency = dict()
    # iterate over lines elements
    for element in lines:
        # add elements to dict as keys (with value 1) and increment value when duplicate
        if element in element_frequency:
            element_frequency[element] += 1
        else:
            element_frequency[element] = 1

    # print items (value) and their frequency (key) from the dictionary
    print("Displaying list of all items purchased/frequency")
    print()
    for item, frequency in element_frequency.items():
        print('{}: {}'.format(item, frequency))
    print()

def getfrequency(v):
    lines = []
    with open('CS210_Project_Three_Input_File.txt') as f:
        for line in f:
            lines.append(line.rstrip())

    element_frequency = dict()
        # Iterate over each element in list
    for element in lines:
        # to make the item name not case sensitive
        element = element.lower()
            # If element exists in dict then increment its value else add it in dict
        if element in element_frequency:
            element_frequency[element] += 1
        else:
            element_frequency[element] = 1

    ''' get the frequency of the parameter (user input) from dictionary
    if user input does not match any of the items in the input file, raise a KeyError exception and return a frequency of 0 '''
    try:
        frequency = element_frequency[v]
    except KeyError:
        frequency = 0

    return frequency

def writefile():
    lines = []
    with open('CS210_Project_Three_Input_File.txt') as f:
        for line in f:
            lines.append(line.rstrip())

    element_frequency = dict()
    # Iterate over each element in list
    for element in lines:
        # If element exists in dict then increment its value else add it in dict
        if element in element_frequency:
            element_frequency[element] += 1
        else:
            element_frequency[element] = 1    

    with open('frequency.dat', 'w+') as f:
        for item, frequency in element_frequency.items():
            f.write(item)
            f.write(' ')
            f.write(str(frequency))
            f.write('\n')