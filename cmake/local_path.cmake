# cmake/local_path.cmake
# if(WIN32)
# Windows
#     set(VCPKG_ROOT_PATH "C:/path/to/vcpkg")
# else()
# Linux
#     set(VCPKG_ROOT_PATH "/home/user/path/to/vcpkg")
# endif()

if(WIN32)
    set(VCPKG_ROOT_PATH "C:/vcpkg")
else()
    set(VCPKG_ROOT_PATH "/home/royya/project-coding/vcpkg")
endif()