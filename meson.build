project('twidgets', 'c',
	version : '0.1',
	default_options: [
	  'warning_level=3',
	  'c_std=gnu11',
	  'werror=true'
	],
	meson_version: '>= 0.47',
	license: 'LGPLv2.1+')

#### compile flags
cc = meson.get_compiler('c')

global_args = []
global_args_maybe = [
	'-Wno-unused-parameter',
	'-Wno-missing-field-initializers',
	'-Wno-pedantic',
	'-Wno-unused-result',
]
foreach a : global_args_maybe
  if cc.has_argument(a)
    global_args += a
  endif
endforeach

if get_option('buildtype') == 'release' and cc.has_argument('-fvisibility=hidden')
  global_args += '-fvisibility=hidden' #this hides our bindings
endif

#find twclients if possible
pj_twclient = subproject('twclient', required: false, version: '>=0.2.1')
if pj_twclient.found()
  dep_twclient = pj_twclient.get_variable('dep_twclient')
  dep_twclient_icons = pj_twclient.get_variable('dep_twclient_icons')
else
  dep_twclient = dependency('twclient', version: '>=0.2.1')
  dep_twclient_icons = dependency('twclient_icons', version: '>=0.2.1')
endif

inc_twidgets = include_directories('include')

subdir('include')
subdir('src')
subdir('tests')
