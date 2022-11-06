# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/mailm/esp/esp-idf/components/bootloader/subproject"
  "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader"
  "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader-prefix"
  "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader-prefix/tmp"
  "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader-prefix/src"
  "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Projekte/Projekte_2022/Anthracide/Anthracide/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()