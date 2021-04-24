nk_cflags=cc.get_supported_arguments(
  ['-Wno-unused-but-set-variable',
   '-Wno-unused-function',
   '-Wno-maybe-uninitialized']
)

dep_nuklear = declare_dependency(
  include_directories : include_directories('./twidgets'),
  compile_args :nk_cflags,
)

if get_option('nklua').enabled()
  dep_lua = dependency('lua', version: '>= 5.3', required: false)
  if not dep_lua.found()
    dep_lua = dependency('lua-5.3', version: '>= 5.3', required: true)
  endif

  dep_nklua = declare_dependency(
    include_directories : include_directories('nklua/src'),
    dependencies : dep_lua)

endif
