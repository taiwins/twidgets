add_library(nuklear INTERFACE)
target_include_directories(nuklear INTERFACE
  "${CMAKE_CURRENT_LIST_DIR}/twidgets")
target_compile_options(nuklear INTERFACE
  "-Wno-type-limits"
  "-Wno-unused-but-set-variable"
  "-Wno-unused-function"
  "-Wno-maybe-uninitialized"
  )

add_library(nklua INTERFACE)
target_include_directories(nklua INTERFACE
  "${CMAKE_CURRENT_LIST_DIR}/nklua/src")
target_compile_options(nklua INTERFACE
  "-Wno-type-limits"
  "-Wno-unused-but-set-variable"
  "-Wno-unused-function"
  "-Wno-maybe-uninitialized"
  )
