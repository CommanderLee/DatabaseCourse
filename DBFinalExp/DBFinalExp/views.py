# Database Final Exp. Li, Zhen, June 10th, 2014.

from django.http import HttpResponse
from django.shortcuts import render_to_response
#import pickle
from django.views.decorators.csrf import csrf_exempt
from django.utils import simplejson
import os
import re
import random

def home(request):
	return render_to_response('home.html')

@csrf_exempt
def search(request):
	
	if 'keyWords' in request.POST:
		keyWords = request.POST['keyWords']
		# Get latitude and longitude from Google Latlng format.
		latlng = re.findall(r'[-]?[\d]*\.[\d]*', request.POST['userPos'])
		userLat = float(latlng[0])
		userLng = float(latlng[1])
		rtnMsg = {"Pos":[{"Lat":random.randint(-50, -30), "Lng":random.randint(140, 160)}, 
		{"Lat":random.randint(-50, -30), "Lng":random.randint(140, 160)}, 
		{"Lat":random.randint(-50, -30), "Lng":random.randint(140, 160)}]}
		#rtnMsg = "Lat:" + str(userLat) + " ; Lng:" + str(userLng) + " ; KeyWords:" + keyWords
	else:
		rtnMsg = "Wrong Keywords"
	rtnMsgJSON = simplejson.dumps(rtnMsg, ensure_ascii=False)
	print rtnMsgJSON
	return HttpResponse(rtnMsgJSON)
	#return HttpResponse(rtnMsg)
	# para = str(request.POST['SearchWord']).strip().lower()
	# if para == '':
		# return render_to_response('home.html')
	# findex = open('indexpickle.txt', 'rb')
	# indexfile = pickle.load(findex)
	# findex.close()
	
	# retlist = []
	# num = 0
		
	# paralist = para.split()
	# isOK = True
	# searchlist = []
	
	# for word in paralist:
		# if indexfile.has_key(word):
			# searchlist += [indexfile[word]]
		# else:
			# isOK = False
			# break
		
	# if isOK:
		# def reduceFunc(x, y):
			# return list(set(x) & set(y))

		# searchlist = reduce(reduceFunc, searchlist)
		# num += len(searchlist)
		# for item in searchlist:
			# f3 = open(r'%s\profInfos\%d.txt' % (os.getcwd(), int(item)), 'r')
			# profInfo = f3.readlines()
			# f3.close()
			
			# f4 = open(r'%s\profPlainText\%d.txt' % (os.getcwd(), int(item)), 'r')
			# profText = f4.read()
			# f4.close()
			# pos = profText.find(paralist[0])
			# abstract = ''
			# if pos >= 0:
				# st = max(0, pos - 40)
				# ed = min(pos + 60, len(profText))
				# abstract = '%s<font color=#FF0000>%s</font>%s' % (profText[st:pos], paralist[0], profText[pos+len(paralist[0]):ed])
			
			# retlist += [{'name':profInfo[0].strip(), 'url':profInfo[1].strip(), 'text':abstract}]
			# print profInfo[0].strip(), profInfo[1].strip()
			
	# return render_to_response('search.html', {'keywords':para, 'num':num, 'profDics':retlist})
