from building import *

cwd = GetCurrentDir()

src = Split('''
jsmn-1.0.0/jsmn.c
util/jsmn_util.c
''')

CPPPATH  = [cwd + '/jsmn-1.0.0']
CPPPATH += [cwd + '/util']

CPPDEFINES = ['JSMN_PARENT_LINKS']

group = DefineGroup('agile_jsmn', src, depend = ['PKG_USING_AGILE_JSMN'], CPPPATH = CPPPATH, CPPDEFINES = CPPDEFINES)

Return('group')
