import csv
import json

with open('condition_codes.csv') as codes_file:
   condition_codes_csv = csv.DictReader(codes_file)
   
   parts = {row["open weathermap code"]: row["short code"] for row in condition_codes_csv}
   print(json.dumps(parts))