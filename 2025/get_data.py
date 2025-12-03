import requests
import sys

outfilename = sys.argv[1]

with open("cookie.txt") as file:
    session = file.read()

s, cookie = session.strip().split("=")

day_number = outfilename.removeprefix("input/day").removesuffix(".txt")

resp = requests.get(f"https://adventofcode.com/2025/day/{day_number}/input", 
                    cookies = {s: cookie})

with open(outfilename, "wb") as file:
    file.write(resp.content)

