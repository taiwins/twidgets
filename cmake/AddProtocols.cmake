###############################################################################
# library
###############################################################################
include(Wayland)

#not able to utilse targets
WAYLAND_ADD_PROTOCOL_CLIENT(proto_xdg_shell_client
  "${WLPROTO_PATH}/stable/xdg-shell/xdg-shell.xml"
  xdg-shell)

add_library(twidgets-protos STATIC
  ${proto_xdg_shell_client}
  )

target_include_directories(twidgets-protos PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

#those are alias, but can we actually move those alias into a findpackage file?
add_library(twidgets::protos ALIAS twidgets-protos)
