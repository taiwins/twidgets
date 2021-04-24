if (TESTS)

  if (NKCAIRO)
    add_executable(test_nk_cairo test_nk_cairo.c)
    target_link_libraries(test_nk_cairo twidgets::nkcairo)
  endif()

  if (NKEGL)
    add_executable(test_nk_egl test_nk_egl.c)
    target_link_libraries(test_nk_egl twidgets::nkegl)
  endif()

endif()
