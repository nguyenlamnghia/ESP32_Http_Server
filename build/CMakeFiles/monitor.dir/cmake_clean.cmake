file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "http_server.bin"
  "http_server.map"
  "project_elf_src_esp32.c"
  "CMakeFiles/monitor"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/monitor.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
