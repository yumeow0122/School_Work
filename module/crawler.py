import requests
import json
import re
import os
from bs4 import BeautifulSoup
from dotenv import load_dotenv

load_dotenv()
base = os.getenv('BASE_URL')
target = os.getenv('MAIN_URL')
url = base + target

def get_unit():
  print("需爬取的發布單位")
  unit = input()
  while not unit:
    print("輸入單位不可為空\n")
    unit = input()
  return unit

def get_size():
  print("需爬取的文章的數量:")
  size = input()
  while not size or not size.isdigit():
      print("大小需為非0整數\n")
      size = input()
  return size

def get_html_data(url):
  response = requests.get(url)
  data = response.text
  return data

def get_data(unit, size):
    articles_list = []
    for idx in range(0, 1000):
        articles_list = search_page(articles_list, idx, unit, size)
        if len(articles_list) >= size:
            break
    articles_list = articles_list[:size]
    return articles_list

def search_page(articles_list, idx, unit, size):
    cur_url = url + '&page=' + str(idx) + '&PageSize=50'
    data = get_html_data(cur_url)
    soup = BeautifulSoup(data, 'html.parser')
    data_middle_news_div = soup.find('div', class_='data_midlle_news')
    if data_middle_news_div:
        first_div = data_middle_news_div.find('div')
        if first_div:
            articles = first_div.find_all('tr')
            for article_idx, article in enumerate(articles):
                if article_idx == 0:
                    continue 
                article_info = process_article(article, unit, size)
                if article_info:
                    articles_list.append(article_info)
        else:
            print("未找到第一個 div")
    else:
        print("未找到 class=\"data_midlle_news\" 的 div")
    return articles_list

def process_article(article, unit, size):
    article_parts = article.text.strip().split('\n')
    date, title, organization = article_parts[0], article_parts[1], article_parts[2]
    article_link = base + article.find('a').get('href')
    if unit == organization.strip():
        author_info = extract_author_info(article_link)
        if author_info:
            article_info = {
                "date": date,
                "unit": organization,
                "title": title,
                "url": article_link,
                "author": author_info,
            }
            return article_info
    return None

def extract_author_info(article_url):
    data = get_html_data(article_url)
    soup = BeautifulSoup(data, 'html.parser')
    data_midlle_news_box01_div = soup.find('div', class_='data_midlle_news_box01')
    if data_midlle_news_box01_div:
        dd_tags = data_midlle_news_box01_div.find_all('dd')
        for dd in dd_tags:
            dd_text = dd.text.strip().split('\n')[0]
            match = re.search(r'聯絡人：(.*?)\s+電話：(\(?\d+\)?[\d\s-]+)', dd_text)
            if match:
                name = match.group(1).strip()
                tel = match.group(2).strip()
                return {"name": name, "tel": tel}
    else:
        print("未找到 class=\"data_midlle_news_box01\" 的 div")
    return None
