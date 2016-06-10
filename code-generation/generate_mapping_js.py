import csv
import json

with open('openweather_condition_codes.csv') as codes_file:
   condition_codes_csv = csv.DictReader(codes_file)
   
   parts = {row["open weathermap code"]: row["short code"] for row in condition_codes_csv}
   print('"OpenWeatherLongToShortMap" : ' + json.dumps(parts) + ",")

with open('weather_underground_codes.csv') as codes_file:
   condition_codes_csv = csv.DictReader(codes_file)
   
   parts = {row["weather underground icon"]: row["short code"] for row in condition_codes_csv}
   print('"WeatherUndergroundLongToShortMap" : ' + json.dumps(parts))