
# ========================== Sahar Itzhak + Ron Efraim ========================== #
# This program will get all capital cities names of the USA from the text file
# "largest_cities, and will look for their postal code in the list of the excel
# file name "us_postal_codes" and will print the name of the city and its postal
# code

import re


# put all capital cities in a list
city_names = []
file1 = open("largest_cities.txt", "r")
next(file1)
for line in file1:
    temp_name = re.search(r"[A-Z][a-z]+([\s][A-Z][a-z]+){0,2}", line)
    city = temp_name.group(0)
    city_names.append(city)


# looks for a match in the csv file
file2 = open("us_postal_codes.csv", "r")
next(file2)
index = 0
result_dict = {key: -1 for key in city_names}

for line in file2:
    city = line.split(',')[1]
    # print(city)
    if city in result_dict and result_dict[city] is -1:
        result = re.search(r"[0-9]+", line)
        result_dict[city] = result.group()

for index, (key, val) in enumerate(result_dict.items()):
    # print('{} {}'.format(key,val))
    print(key, val)


file1.close()
file2.close()
