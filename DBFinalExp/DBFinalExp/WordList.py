# Database Final Exp. Li, Zhen, June 10th, 2014.
import ctypes
from ctypes import cdll

lib = cdll.LoadLibrary('.\WordList.so')

class WordList(object):     
    def __init__(self):         
        self.obj = lib.getWordList()
        
    def createIndex(self):
        ret = lib.wordListCreateIndex(self.obj)
        return ret
        
    def search(self, query):
        #q = ctypes.create_string_buffer(query)
        print('Python:%s' % query)
        ret = lib.wordListSearch(self.obj, query)
        retWord = ctypes.string_at(ret, -1)
        return retWord
