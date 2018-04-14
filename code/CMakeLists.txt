cmake_minimum_required (VERSION 2.6)
project (Transcode)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
set(CMAKE_CXX_FLAGS "-pedantic-errors -DTIMER")
set(CMAKE_C_FLAGS "-g")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(Src_file ${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/../include)
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/../lib)
add_executable(Transcode ${Src_file}/main.cpp ${Src_file}/Audio.cpp 
                    ${Src_file}/RPCServer.cpp ${Src_file}/Timer.cpp
                     ${Src_file}/Transcoder.cpp ${Src_file}/Video.cpp
                     ${Src_file}/TransState.cpp  ${Src_file}/CtxWrapper.cpp
                     ${Src_file}/transcode_rpc.grpc.pb.cpp ${Src_file}/transcode_rpc.pb.cpp 
            )
add_executable(Client ${Src_file}/Client.cpp  
            ${Src_file}/transcode_rpc.grpc.pb.cpp ${Src_file}/transcode_rpc.pb.cpp 
)
set(Tran_lib avformat avfilter avcodec swresample swscale avutil avdevice z m pthread x264 fdk-aac postproc va dl vdpau X11 va-drm va-x11 protobuf grpc grpc++)
target_link_libraries(Transcode ${Tran_lib})
target_link_libraries(Transcode pthread)
target_link_libraries(Client  protobuf grpc grpc++ gpr grpc_unsecure grpc++_unsecure)
target_link_libraries(Client pthread)