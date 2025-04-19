
option(FFS_DEBUG "Additional ffilesys logs, builds test executable." on)

if(FFS_DEBUG)
    add_definitions(-DFFS_DEBUG)
endif()

set(ffilesys_sources
    "file-io.cpp"
    "error-code.cpp"
)

add_library(ffilesys STATIC ${ffilesys_sources})

if(FFS_DEBUG)
    add_subdirectory(test)
endif()