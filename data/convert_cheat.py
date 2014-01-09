#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
import xml.etree.ElementTree as ET
import re, sys, getopt
from os.path import basename

numbers = (
	'Zero',
	'One',
	'Two',
	'Tree',
	'Four',
	'Five',
	'Six',
	'Seven',
	'Eight',
	'Nine',
	'Ten',
	'Eleven',
	'Twelve',
	'Thirteen',
	'Fourteen',
	'Fifteen',
	'Sixteen',
	'Seventeen',
	'Eighteen',
	'Nineteen' )
def convert(inputfile, outputfile):
	if outputfile == '':
		outputfile = basename(inputfile).split('.')[0] + '.ini'
	tree = ET.parse(inputfile)
	cheats = tree.getroot()
	with open(outputfile, 'w') as file:
		for cheat in cheats:
			setCheat = False
			file.write('cheat "' + cheat.attrib['desc'] + '"\n')
			cheatConntent = 'default 0\n'
			cheatConntent += '0 "Disabled"\n'
			script = cheat.find('script')
			parameter = cheat.find('parameter')
			if script is not None and parameter is not None:
				result = re.match(r'.+?@([0-9A-F]{6})=(.+?)(\||$)', script.find('action').text)
				if result:
					setCheat = True
					if parameter.get('min') is not None and parameter.get('max') is not None and parameter.get('step') is not None:
						for param in range(int(parameter.attrib['min']), int(parameter.attrib['max']), int(parameter.attrib['step'])):
							cheatConntent += str(param) + ' "' + numbers[eval(result.group(2))] + '"'
							cheatConntent += ', 0, 0x' + result.group(1) + ', 0x%02X' % eval(result.group(2)) + '\n'
					else:
						cheatOffset = 1
						for item in parameter:
							param = int(item.attrib['value'], 16)
							cheatConntent += str(cheatOffset) + ' "' + item.text + '"'
							for action in script:
								result = re.match(r'.+?@([0-9A-F]{6})=(.+?)(\||$)', action.text)
								if result:
									if action.get('condition') is None:
										if result.group(2).find('param') != -1:
											cheatConntent += ', 0, 0x' + result.group(1) + ', 0x%02X' % eval(result.group(2))
										else:
											cheatConntent += ', 0, 0x' + result.group(1) + ', 0x' + result.group(2)
									elif action.attrib['condition'].find('param') != -1 and eval(action.attrib['condition']):
										if result.group(2).find('param') != -1:
											cheatConntent += ', 0, 0x' + result.group(1) + ', 0x%02X' % eval(result.group(2))
										else:
											cheatConntent += ', 0, 0x' + result.group(1) + ', 0x' + result.group(2)
							cheatConntent += '\n'
							cheatOffset += 1


			if script is not None and parameter is None and (script.attrib['state'] == "run" or script.attrib['state'] == "on"):
				cheatConntent += '1 "Enabled"'
				for action in script:
					result = re.match(r'.+?@([0-9A-F]{6})=([0-9A-F]{2})', action.text)
					if result:
						setCheat = True
						cheatConntent += ', 0, 0x' + result.group(1) + ', 0x' + result.group(2)
				cheatConntent += '\n'
			if setCheat:
				file.write(cheatConntent)
			file.write('\n')

def usage():
	print 'Usage:'
	print '\t' + sys.argv[0] + ' -i <inputfile> -o [outputfile]'
	print '\t' + sys.argv[0] + ' <inputfile> [outputfile]'

def main():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hi:o", ["help", "input=", "output="])
	except getopt.GetoptError as err:
		usage()
		exit(2)
	inputfile = ''
	outputfile = ''
	if opts == [] and args == []:
		usage()
		exit(2)
	for opt, arg in opts:
		if opt == "-v":
			verbose = True
		elif opt in ("-h", "--help"):
			usage()
			exit(0)
		elif opt in ("-i", "--input"):
			inputfile = arg
		elif opt in ("-o", "--output"):
			output = arg
		else:
			assert False, "unhandled option"
	if len(args) == 1:
		inputfile = args[0]
	elif len(args) == 2:
		inputfile = args[0]
		outputfile = args[1]

	convert(inputfile, outputfile)

if __name__ == "__main__":
	main()