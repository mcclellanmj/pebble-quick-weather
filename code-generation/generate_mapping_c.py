import csv

def to_c_array_text(python_list):
  return "{" + ",".join(python_list) + "}"

with open('condition_codes.csv', 'r') as codes_file, open('condition_codes.template.c', 'r') as template_file, open('condition_codes.c', 'w') as output_file:
    condition_codes_csv = csv.DictReader(codes_file)
    csv_extraction = [(row["icon code"], row["description"]) for row in condition_codes_csv]
    icon_codes = ["RESOURCE_ID_WEATHER_ICON_" + icon_code for (icon_code, _) in csv_extraction]
    descriptions = ['"' + description + '"' for (_, description) in csv_extraction]
    description_sizes = [str(len(x)) for x in descriptions]

    number_of_condition_codes = len(icon_codes)

    for line in template_file:
        modified_line = line.replace('%CONDITION_CODE_TO_ICON_TABLE%', "{" + ",".join(icon_codes) + "}")
        modified_line = modified_line.replace('%NUM_CONDITION_CODES%', str(number_of_condition_codes))
        modified_line = modified_line.replace('%CONDITION_CODE_TO_TEXT_TABLE%', "{" + ",".join(descriptions) + "}")
        modified_line = modified_line.replace('%CONDITION_CODE_TO_TEXT_SIZE_TABLE%', "{" + ",".join(description_sizes) + "}")
        output_file.write(modified_line) 