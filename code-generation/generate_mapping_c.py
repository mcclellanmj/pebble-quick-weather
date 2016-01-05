import csv

with open('condition_codes.csv', 'r') as codes_file, open('condition_codes.template.c', 'r') as template_file, open('condition_codes.c', 'w') as output_file:
    condition_codes_csv = csv.DictReader(codes_file)
    icon_codes = ["RESOURCE_ID_WEATHER_ICON_" + row["icon code"] for row in condition_codes_csv]
    number_of_condition_codes = len(icon_codes)

    for line in template_file:
        modified_line = line.replace('%CONDITION_CODE_TO_ICON_TABLE%', "{" + ",".join(icon_codes) + "}")
        modified_line = modified_line.replace('%NUM_CONDITION_CODES%', str(number_of_condition_codes))
        output_file.write(modified_line) 