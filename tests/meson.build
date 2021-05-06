if get_option('nkcairo').enabled() and get_option('test').enabled()

nk_cairo_test = executable(
  'test-nk-cairo',
  ['test_nk_cairo.c',
   wayland_xdg_shell_client_protocol_h,
   wayland_xdg_shell_client_protocol_c],
  dependencies : [
    dep_wayland_client,
    dep_nkcairo,
  ],
  install : false,
)

endif

if get_option('nkegl').enabled() and get_option('test').enabled()

nk_egl_test = executable(
  'test-nk-egl',
  ['test_nk_egl.c',
   wayland_xdg_shell_client_protocol_h,
   wayland_xdg_shell_client_protocol_c],
  dependencies : [
    dep_wayland_client,
    dep_nkegl,
  ],
  install : false,
)

endif
