'''
Created July 21, 2025
By: Salman Umar

Want to convert Drive Data from a CSV file to a C struct list for ease of use in our C code
'''
import csv

input = "drive.csv"
output_c = "drive_data.c" # Where we will send our data...
output_h = "drive_data.h"

struct = 'DriveData'
array = 'driveLog'

# Opening CSV file
with open(input, newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    rows = list(reader)

# Create h file; need to define
with open(output_h, 'w') as hfile:
    hfile.write(f"#ifndef DRIVE_DATA_H\n#define DRIVE_DATA_H\n\n")
    hfile.write(f"#include <stdint.h>\n\n")
    hfile.write(f"typedef struct {{\n")
    hfile.write(f"    float time_ms;\n")
    hfile.write(f"    float speed;\n")
    hfile.write(f"    float rpm;\n")
    hfile.write(f"}} {struct};\n\n")
    hfile.write(f"extern const {struct} {array}[];\n")
    hfile.write(f"extern const int {array}Length;\n\n")
    hfile.write(f"#endif // DRIVE_DATA_H\n")

# Step 3: Create .c file
with open(output_c, 'w') as cfile:
    cfile.write(f"#include \"{output_h}\"\n\n")
    cfile.write(f"const {struct} {array}[] = {{\n")
    for row in rows:
        t = float(row['Time(ms)'])
        s = float(row['Speed(kmh)'])
        r = float(row['RPM'])
        cfile.write(f"    {{{t}, {s}, {r}}},\n")
    cfile.write(f"}};\n\n")
    cfile.write(f"const int {array}Length = sizeof({array}) / sizeof({array}[0]);\n")