import json
import os
from module import crawler
from bs4 import BeautifulSoup
from dotenv import load_dotenv

#request target url
size = crawler.get_size()
unit = crawler.get_unit()
articles_list = crawler.get_data(unit, int(size))


output_file = "./" + unit + ".json"
with open(output_file, 'w', encoding='utf-8') as f:
  json.dump(articles_list, f, ensure_ascii=False, indent=4)
print("Data has been saved to:", output_file)

