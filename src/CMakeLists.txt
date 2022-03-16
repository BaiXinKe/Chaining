project(ChainingSRC)


set(BASE_SOURCE_FILE 
        base/Buffer.cpp 
        base/nocopyable.hpp 
        base/Logger.hpp 
        base/Singleton.hpp
        base/Timestamp.hpp
        base/LoggerUtility.hpp
        base/LoggerUtility.cpp)


set(BASE_LIBRARY base_library)
add_library(${BASE_LIBRARY} ${BASE_SOURCE_FILE})
add_library(src::base ALIAS ${BASE_LIBRARY})

target_include_directories(${BASE_LIBRARY}
    PUBLIC   ${PROJECT_SOURCE_DIR})



    
set(NET_SOURCE_FILE
    net/Epoller.hpp
    net/Epoller.cpp
    net/InetAddr.hpp
    net/InetAddr.cpp)

set(NET_LIBRARY net_library)
add_library(${NET_LIBRARY} ${NET_SOURCE_FILE})

target_include_directories(${NET_LIBRARY} PUBLIC ${PROJECT_SOURCE_DIR})


include_directories(base)
include_directories(net)