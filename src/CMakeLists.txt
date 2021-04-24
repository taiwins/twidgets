###############################################################################
# nkegl library
###############################################################################

if (NKEGL)

  add_library(twidgets-nkegl STATIC
    nk_wl_egl.c
    )
  set_target_properties(twidgets-nkegl
    PROPERTIES POSITION_INDEPENDENT_CODE 1
    )
  target_compile_definitions(twidgets-nkegl
    PUBLIC "-D_TW_USE_GL"
    )
  target_link_libraries(twidgets-nkegl PUBLIC
    twidgets::protos
    twclient::twclient
    twclient::icons
    Wayland::Client
    Wayland::EGL
    OpenGL::EGL
    XKBCommon::XKBCommon
    Freetype::Freetype
    Fontconfig::Fontconfig
    m
    )
  target_include_directories(twidgets-nkegl PUBLIC
    ${TWIDGETS_INCLUDE_DIR}
    )
  add_library(twidgets::nkegl ALIAS twidgets-nkegl)

endif()

###############################################################################
# nkcairo library
###############################################################################

if (NKCAIRO)

  add_library(twidgets-nkcairo STATIC
    nk_wl_cairo.c
    )
  set_target_properties(twidgets-nkcairo
    PROPERTIES POSITION_INDEPENDENT_CODE 1
    )
  target_link_libraries(twidgets-nkcairo PUBLIC
    twidgets::protos
    twclient::twclient
    twclient::icons
    Cairo::Cairo
    Wayland::Client
    XKBCommon::XKBCommon
    Freetype::Freetype
    Fontconfig::Fontconfig
    m
    )
  target_include_directories(twidgets-nkcairo PUBLIC
    ${TWIDGETS_INCLUDE_DIR}
    )
  add_library(twidgets::nkcairo ALIAS twidgets-nkcairo)

endif()
