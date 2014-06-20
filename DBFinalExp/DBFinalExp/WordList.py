# Database Final Exp. Li, Zhen, June 10th, 2014.
import socket
import json
# import ctypes
# from ctypes import *

# lib = cdll.LoadLibrary('G:/Course/Grade_Three_Spring/Database/DatabaseCourse/DBFinalExp/DBFinalExp/WordList.so')

def searchPy(lat, lng, keyWords):
	SERVER_ADDR = '127.0.0.1'
	SERVER_PORT = 8080
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect((SERVER_ADDR, SERVER_PORT))
	print "Connected."
	
	sendData = {"lat":lat, "lng":lng, "keyWords":keyWords}
	sendJSON = json.dumps(sendData)
	sock.send(sendJSON + "\r") 
	print "Send:" + sendJSON
	recvData = sock.recv(4096)        
	print "Receive: " + recvData
	sock.close()
	print "Closed."
	
	return recvData
	# myKeyWords = ctypes.create_string_buffer(keyWords)
	# print('Python: Lat:%f, Lng:%f, Key:%s' % (lat, lng, myKeyWords))
	# print keyWords
	# resAddr = lib.searchC(c_double(lat), c_double(lng), myKeyWords)
	# resString = ctypes.string_at(resAddr, -1)
	# return resString
	
# class WordList(object):     
	# def __init__(self):         
		# self.obj = lib.getWordList()
		# self.init = 'init'
		# ret = lib.wordListCreateIndex(self.obj)
		# print 'WordList created'
		
	# def createIndex(self):
		# self.index = 'index'
		# ret = lib.wordListCreateIndex(self.obj)
		# print 'WordList Index created'
		# return ret
		
	# def searchPy(self, lat, lng, keyWords):
		# myKeyWords = ctypes.create_string_buffer(keyWords)
		# print('Python: Lat:%f, Lng:%f, Key:%s' % (lat, lng, myKeyWords))
		# print self.init, self.index, keyWords
		# resAddr = lib.wordListSearchC(self.obj, c_double(lat), c_double(lng), myKeyWords)
		# resString = ctypes.string_at(resAddr, -1)
		# return resString
		
