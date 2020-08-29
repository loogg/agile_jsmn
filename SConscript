from building import *

cwd      = GetCurrentDir()
src      = Glob('src/*.c')
src     += Glob('util/*.c')

CPPPATH  = [cwd + '/inc']
CPPPATH += [cwd + '/util']

group = DefineGroup('agile_jsmn', src, depend = ['PKG_USING_AGILE_JSMN'], CPPPATH = CPPPATH)

Return('group')
