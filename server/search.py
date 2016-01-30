from pyvirtualdisplay import Display
from selenium import webdriver
import urllib


display = Display(visible=0, size=(1024,768))
display.start()
driver = webdriver.Chrome()



def search(query):
    params = urllib.urlencode({'q':query})
    url = 'https://www.google.com/search?' + params
    driver.get(url)
    response = {};
    results = [];

    list =  driver.find_elements_by_css_selector('._uX')
    if not len(list) == 0:
        for el in list:
             results.append(el.text)
    else:
        list2 = driver.find_elements_by_css_selector('[data-dobid=dfn]')
        if not len(list2) == 0:
            for el in list2:
                results.append(el.text)
        else:
            try:
                temp = driver.find_element_by_id("wob_tm").text
                print temp
                weather = driver.find_element_by_id("wob_dc").text
                print weather
                loc = driver.find_element_by_id("wob_loc").text
                print loc
                results.append(temp + "\u00B0F\n" + weather + "\n" + loc)
            except:
                try:
                    results.append(driver.find_element_by_id("cwos").text)
                except:
                    results.append("No result")
    ans = results[0];
    for i in range(1,len(results)):
        if len(results[i]) > 0:
            ans = ans + '\n' + results[i]

    response['answer'] = ans
    search_results = []
    slist = driver.find_elements_by_css_selector('.g .r a')
    if not len(slist) == 0:
        for el in slist:
            sres = {"title": el.text}
            link = el.get_attribute('href')
            sres['url'] = link
            search_results.append(sres)
    else:
        search_results = [{"title":"No results found"}]
    response['searchResults'] = search_results
    return response



