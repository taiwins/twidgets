### wayland-protocols #########################################################
dep_scanner = dependency('wayland-scanner', native: true)
prog_scanner = find_program(dep_scanner.get_pkgconfig_variable('wayland_scanner'))
dep_wp = dependency('wayland-protocols', version: '>= 1.18')
dir_wp_base = dep_wp.get_pkgconfig_variable('pkgdatadir')
protocols = [['xdg-shell', 'stable']]

foreach proto: protocols
  xml_path = '@0@/@1@/@2@/@2@.xml'.format(dir_wp_base, proto[1], proto[0])
  base_file = proto[0]
  foreach type : [ 'client-header', 'private-code']
    if type == 'client-header'
      output_file = 'wayland-@0@-client-protocol.h'.format(base_file)
    else
      output_file = 'wayland-@0@-client-protocol.c'.format(base_file)
    endif
    target = custom_target(
      '@0@-@1@'.format(base_file, type),
      command : [ prog_scanner, type, '@INPUT@', '@OUTPUT@'],
      input: files(xml_path),
      output: output_file,
    )
    var_name = output_file.underscorify()
    set_variable(var_name, target)
  endforeach
endforeach

### twclient-nkbackends #######################################################

dep_xkbcommon = dependency('xkbcommon', version: '>= 0.3.0')
dep_wayland_client = dependency('wayland-client', version: '>= 1.17.0')
dep_wayland_egl = dependency('wayland-egl', version: '>= 1.17.0')
dep_wayland_cursor = dependency('wayland-cursor', version: '>= 1.17.0')

dep_threads = dependency('threads')
dep_libm = cc.find_library('m')
dep_cairo = dependency('cairo')
dep_fontconfig = dependency('fontconfig')
dep_freetype = dependency('freetype2')

twidgets_flags = ['-D_GNU_SOURCE'] + global_args

depends = [
  dep_wayland_client,
  dep_xkbcommon,
  dep_fontconfig,
  dep_freetype,
  dep_libm,
  dep_twclient,
  dep_twclient_icons,
  dep_nuklear,
]

if get_option('nklua').enabled()
  depends += dep_nklua
endif

if get_option('nkcairo').enabled()

  lib_nkcairo = static_library(
    'twidgets-cairo',
    ['nk_wl_cairo.c',
     wayland_xdg_shell_client_protocol_h,
     wayland_xdg_shell_client_protocol_c],
    c_args : twidgets_flags,
    include_directories : inc_twidgets,
    dependencies : [dep_cairo] + depends,
    install : false,
  )

  dep_nkcairo = declare_dependency(
    link_with : lib_nkcairo,
    compile_args : twidgets_flags + nk_cflags,
    include_directories : inc_twidgets,
    dependencies : [dep_cairo] + depends,
  )

endif

if get_option('nkegl').enabled()

  lib_nkegl = static_library(
    'twidgets-egl',
    ['nk_wl_egl.c',
     wayland_xdg_shell_client_protocol_h,
     wayland_xdg_shell_client_protocol_c],
    c_args : twidgets_flags,
    include_directories : inc_twidgets,
    dependencies : depends,
    install : false,
  )

  dep_nkegl = declare_dependency(
    link_with : lib_nkegl,
    compile_args : twidgets_flags + nk_cflags,
    include_directories : inc_twidgets,
    dependencies : depends,
  )

endif
