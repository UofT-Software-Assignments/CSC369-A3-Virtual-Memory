import os

# INSTRUCTIONS:
# to run, set OUTPUT_FILE_PATH to the file path you want to write the output to, 
# and make sure to set TRACES_DIR to the directory containing the addr-*.ref files,
# by defualt they are already set to the following constants. then just run python3 analysis.py

TRACES_DIR = "./traces"
OUTPUT_FILE_PATH = "./analysis_tables.txt"

def analyse(file):
    access_counts = {"I": 0, "L": 0, "S": 0, "M": 0}
    instruction_page_counts = {}
    data_page_counts = {}

    for line in file:
        #parse line
        access_type, rest = line.split()
        address = rest.split(',')[0]
        page = address.lstrip('0')[:-3] + "000"

        #handle parsed data
        access_counts[access_type] += 1

        if access_type == "I":
            if page not in instruction_page_counts:
                instruction_page_counts[page] = 0
            instruction_page_counts[page] += 1
        else: 
            if page not in data_page_counts:
                data_page_counts[page] = 0
            data_page_counts[page] += 1
    
    return access_counts, instruction_page_counts, data_page_counts
        

def write_traces(file_path, output_file):
   
    with open(file_path, 'r') as file:
        print(f"Analyzing {file_path}")
        access_counts, instruction_page_counts, data_page_counts = analyse(file)

    output_file.write(f"===== Traces for {file_path} =====\n\n")
    output_file.write(f"Counts:\n  Instructions {access_counts['I']}\n  Loads {access_counts['L']}\n  Stores {access_counts['S']}\n  Modifies {access_counts['M']}\n\n")
    
    output_file.write(f"Instructions:\n")
    for page in sorted(instruction_page_counts.keys()):
        output_file.write(f"0x{page},{instruction_page_counts[page]}\n")
    
    output_file.write("Data:\n")
    for page in sorted(data_page_counts.keys()):
        output_file.write(f"0x{page},{data_page_counts[page]}\n")
    output_file.write("\n\n")

def main():
    with open(OUTPUT_FILE_PATH, 'w') as output_file:
        for filename in os.listdir(TRACES_DIR):
            if filename.startswith("addr"):
                file_path = os.path.join(TRACES_DIR, filename)
                write_traces(file_path, output_file)
    print(f"done all analysis, check {OUTPUT_FILE_PATH} for results")

if __name__ == "__main__":
    main()