file(REMOVE_RECURSE
  ".bin_timestamp"
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "http_server.bin"
  "http_server.map"
  "project_elf_src_esp32.c"
  "CMakeFiles/gen_project_binary"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/gen_project_binary.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
