import sys
import os
import http.cookiejar
import urllib
import bs4
from bs4 import BeautifulSoup
import logging
import re

logging.basicConfig(level=logging.INFO)


class UrlReader(object):
    """
    open the url with cookies
    """
    def __init__(self):

        values = {'u': 'guest', 'p': 'guest'}
        postdata = urllib.parse.urlencode(values).encode()

        cookie_filename = 'cookie_jar.txt'
        cookie_jar = http.cookiejar.MozillaCookieJar(cookie_filename)
        hander = urllib.request.HTTPCookieProcessor(cookie_jar)
        self.opener = urllib.request.build_opener(hander)

        try:
            login_url = r'http://172.23.145.130:8009/login'

            headers = {
                'User-Agent':
                'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36'
            }

            login_req = urllib.request.Request(login_url, postdata, headers)

            login_resp = self.opener.open(login_req)
        except urllib.error.URLError as e:
            logging.error(e.code, ':', e.reason)

    def open(self, url):
        """
        use cookie to open url, return the beautiful soup
        """
        req = urllib.request.Request(url)
        resp = self.opener.open(req)
        logging.debug("Url Info = %s" % resp.info())

        # for now, just decode utf-8
        data = resp.read().decode('UTF-8')
        soup = BeautifulSoup(data, 'html.parser')
        return soup


reader = UrlReader()
major_url = 'http://172.23.145.130:8009'
soup = reader.open(major_url)

container = soup.body.find('div', ['container'])
script = container.find(type='text/javascript')
script_str = script.string

assert isinstance(script_str, bs4.element.NavigableString)

script_str = script_str.strip()
script_str = script_str.strip('itemList = [')
index = script_str.find('listContainer')
script_str = script_str[0:index]
script_str = script_str.rstrip().rstrip(']')

docs = []
while True:
    s = script_str.find('{')
    e = script_str.find('}')
    if s == -1 or e == -1 or s > e:
        break

    url_name_str = script_str[s+1:e]
    tmp_url, tmp_name = url_name_str.split(',')
    tmp_url = major_url+tmp_url[8:-1]
    tmp_name = tmp_name[10:-1]

    docs.append(dict(name=tmp_name, url=tmp_url))
    script_str = script_str[e+1:]

contents = []
for doc in docs:
    doc_url = doc['url']
    doc_name = doc['name']

    soup = reader.open(doc_url)

    contents_divs = soup.body.find(class_='wy-nav-content')
    for href in contents_divs.find_all(href=re.compile('html')):
        if href['class'] == ['reference', 'internal']:
            internal_name = ''
            if len(href.contents) != 0:
                for href_cont in href.contents:
                    internal_name = internal_name+href_cont.string
            else:
                internal_name = href.string
            logging.debug("Internal Name = %s" % internal_name)

            interal_url = href['href']
            logging.debug("Internal Url = %s" % interal_url)

            external_url = doc_url.rstrip('index.html')+interal_url
            logging.debug("External Url = %s", external_url)

            contents.append(
                dict(doc=doc_name, title=internal_name, url=external_url))
        else:
            # the button's url
            logging.error("HREF = %s" % href)

# all the docs' url
for doc in docs:
    logging.info('Doc = %s, Url = %s'%(doc['name'],doc['url']))
# all the titles' url
for content in contents:
    logging.info('Doc = %s, Title = %s, Url = %s' % (content['doc'],content['title'],content['url']))