# CMakeList.txt: ADBS 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable (ADBS "ADBS.cpp" "ADBS.h" "include/BufferManager.h" "BufferManager.cpp" "include/DataStorageManager.h" "DataStorageManager.cpp" "include/Common.h" "LRUReplacer.cpp" "include/LRUReplacer.h"  "LRU2Replacer.cpp" "include/LRU2Replacer.h" "include/Replacer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ADBS PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
