from WordList import *

wl = WordList()
ret = wl.createIndex()
print('Return:%d' % ret)
retWord = wl.search('WTF')
print('Return:%s' % retWord)