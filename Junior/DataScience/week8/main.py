import time
import pandas as pd

from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import Select

def print_data():
    target = driver.find_element(By.XPATH, '//*[@id="PC_Week_MOD"]/thead/tr/th[1]').text
    temperature = driver.find_element(By.XPATH, '//*[@id="PC_Week_MOD"]/tbody/tr[1]/td[1]/p/span[1]').text
    print(target)
    print(temperature)

# initial
city = input()
url = 'https://www.cwb.gov.tw/V8/E/W/County/County.html'
driver = webdriver.Chrome()
driver.get(url)

# search
select_element = driver.find_element(By.ID, 'CID')
select = Select(select_element)
if city == 'all':
    for idx in range(1, 23):
        select_element = driver.find_element(By.ID, 'CID')
        select = Select(select_element)
        select.select_by_index(idx)

        print_data()

        time.sleep(1)


else:
    select.select_by_visible_text(city)
    print_data()

time.sleep(1)
driver.quit()