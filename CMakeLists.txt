cmake_minimum_required(VERSION 3.24)
project(Pf_ProjectFinal)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pf_ProjectFinal main.cpp client.cpp server.cpp Base64.cpp FileTransfer.cpp Response.cpp)
target_link_libraries(Pf_ProjectFinal PRIVATE ws2_32)

