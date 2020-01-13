
# EX3
# Name: Sahar Ithzak, Ron Efraim
# ID: 308389485, 204661425

import re


def is_insert(line):
   return 'INSERT INTO' in line or False

def is_create(line):
    return 'CREATE TABLE' in line or False


def get_values(line):
    line1 = line.partition(' VALUES ')[2]
    line1 = line1.replace('),','\n')
    line1 = line1.replace(');', ' ')
    line1 = line1.replace(')', ' ')
    line1 = line1.replace('(',' ')
    return line1


def get_table_name(line):
    match = re.search('`([0-9_a-zA-Z]+)`', line)
    if match:
        return match.group(1)
    else:
        print(line)

def get_columns(line):
    line1 = line.partition(' VALUES ')[2]
    match = re.search('INSERT INTO `.*` VALUES ()', line)
    if match:
        return list(map(lambda x: x.replace('`', '').strip(), line1.group(1).split(',')))


def read_next_column(line):
    match = re.search('\s\s`([0-9_a-zA-Z]+)`', line)
    if match:
        return match.group(1)
    else:
        return " "


def main():
    with open("demo.sql", 'rb') as f:
        found_table = False
        for line in f.readlines():
            try:
                line = line.decode("utf-8")
            except UnicodeDecodeError:
                line = str(line)
            if is_create(line):
                found_table = True
                table_name = get_table_name(line)
                open(table_name + '.csv', 'w')
            else:
                if found_table:
                    with open(table_name + '.csv', 'a') as outcsv:
                        if not is_insert(line):
                            result = read_next_column(line)
                            if result != ' ':
                                outcsv.write(result + ",")
                        else:
                            outcsv.write("\n")
                            values = get_values(line)
                            outcsv.write(values)

if __name__ == '__main__':
    main()
