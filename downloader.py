import threading
import urllib2
from urllib2 import urlopen
#url = raw_input('enter something')
url="http://rohinibarla.github.io/heros/05JoeEdelman.jpg"
site = urllib2.urlopen(url)
meta = site.info()
file_size = int(meta.getheaders("Content-Length")[0])
threads = []
parts = {}
chunk_size = file_size/4
def download(start):
    req = urllib2.Request(url)
    req.headers['Range'] = 'bytes=%s-%s' % (start, start + chunk_size)
    f = urllib2.urlopen(req)
    parts[start] = f.read()

for i in range(0, 4):
    t = threading.Thread(download(i * chunk_size))
    t.start()
    threads.append(t)
for i in threads:
    i.join()

result = ''
chunk = ''
i = 0
while chunk != None:
    if parts.has_key(i * chunk_size):
        chunk = parts[i * chunk_size]
        result = result + chunk
        i = i + 1
    else:
        break
f = open("selfie", "wb")
f.write(result)
