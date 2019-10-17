#!/usr/bin/env python
from argparse import ArgumentParser
from commands import getstatusoutput
from subprocess import call
import sys

parser = ArgumentParser();
parser.add_argument('find_key', type=str);

def print_files(file, index):
	print (file+'    '+'('+str(index)+')')     


def find(find_key=''):
	
	#first search
	file_list = getstatusoutput('git ls-files `git rev-parse --show-toplevel`')[1].split('\n')
	file_list = [file for file in file_list if find_key.lower() in file.lower()]
	file_list_size=0
	for file in file_list:
		file_list_size=file_list_size+1
	
	
	#if there is no file, exit
	if file_list_size==0:
		print ("there is no file\n")
		return 0
	
	order=-1
	#additional search
	while (file_list_size>1):
		
		#print all
		for index in range (0,file_list_size):
			if(index==10):
				break
			print_files(file_list[index], index+1)

		#get new
		new_find_key=raw_input("input new find key : ")
		
		#check exit order
		if(len(new_find_key)==0):
			print("exit program\n")
			return 0

		#check if find_key is order
		order=1
		for index in range(0, len(new_find_key)):
			if (ord(new_find_key[index])<ord('0') or ord(new_find_key[index])>ord('9')):
				order=-1
				break
		if(order==1):
			order=int(new_find_key)

		#case : order
		if(order != -1 and order<=file_list_size and order<=10):
			break
		
		
		#case : find_key
		order=-1
		new_file_list=[file for file in file_list if new_find_key.lower() in file.lower()]
		file_list_size=0
		for file in new_file_list:
			file_list_size=file_list_size+1
		file_list=new_file_list

	
	#vim file
	if (file_list_size==0 or order==0):
		print("no such file\n")
		return 0
	elif order==-1:
		file=file_list[0]
	else:
		file=file_list[order-1]
	call("vim "+file, shell=True)






if __name__ == '__main__':
	if len(sys.argv)!=2:
       		print("wrong arguments\n")
	else:	
		find(parser.parse_args().find_key)
	






