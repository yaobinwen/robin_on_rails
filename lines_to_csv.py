#!/usr/bin/python3

import csv

def convert_to_csv(input_file, output_file, n):
    with open(input_file, 'r') as f:
        lines = f.readlines()

    # m = len(lines) // n
    data = [lines[i:i+n] for i in range(0, len(lines), n)]

    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f, delimiter='|')
        for row in data:
            writer.writerow([column.strip() for column in row])

# Usage example
input_file = 'aaa.md'  # Replace with the path to your input file
output_file = 'output.csv'  # Replace with the desired output file path
n = 16  # Replace with the number of lines per block

convert_to_csv(input_file, output_file, n)
