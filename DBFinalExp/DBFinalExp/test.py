from WordList import *
from django.utils import simplejson
from multiprocessing.sharedctypes import Value

# wl = WordList()
# ret = wl.createIndex()
# print('Return:%d' % ret)
# retWord = wl.search('WTF')
# print('Return:%s' % retWord)
wl = Value(WordList)
wl.createIndex()
rtnMsg = wl.searchPy(15.123, 100.456, "i am the key words")
print rtnMsg
jsonMsg = simplejson.dumps(rtnMsg, ensure_ascii=False)
print jsonMsg