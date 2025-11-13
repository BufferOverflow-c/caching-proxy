cmake_minimum_required(VERSION 3.20)
project(caching_proxy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#find_package(nlohmann_json 3.12.0 REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/"
#    "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/curlpp-0.8.1/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/curlpp-0.8.1/include/curlpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/curlpp-0.8.1/include/utilspp"
)

target_link_libraries(caching_proxy
    PRIVATE
    "-framework CoreFoundation"
    "-framework Security"
    "-framework CFNetwork"
    /opt/homebrew/lib/libbrotlicommon.dylib
    /opt/homebrew/lib/libbrotlienc.dylib
    /opt/homebrew/lib/libbrotlidec.dylib
    /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/libz.tbd
    /opt/homebrew/lib/libzstd.a
    /opt/homebrew/lib/libcrypto.dylib
    ${OPENSSL_LIBRARIES}
)

#add_subdirectory(src/vendor/curlpp-0.8.1)
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
#    curlpp curl
#    nlohmann_json::nlohmann_json
#)
