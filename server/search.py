from pyvirtualdisplay import Display
from selenium import webdriver
import urllib


display = Display(visible=0, size=(1024,768))
display.start()
driver = webdriver.Firefox()



def search(query):
    params = urllib.urlencode({'q':query})
    url = 'https://www.google.com/search?' + params
    driver.get(url)
    results = [];
    list =  driver.find_elements_by_css_selector('._uX')
    if not len(list) == 0:
        for el in list:
            results.append(el.text)
        return results
    else:
        return "No result"



