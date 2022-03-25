project(MainLibrary)

set(AUX_SOURCE_FILES 
    auxiliary/BlockQueue.hpp
    auxiliary/ErrorMsg.hpp 
    auxiliary/InitLogger.hpp 
    auxiliary/InitLogger.cpp 
    auxiliary/Logger.hpp 
    auxiliary/Nocopyable.hpp 
    auxiliary/ReadSetting.hpp 
    auxiliary/ReadSetting.cpp
    auxiliary/Timestamp.hpp)

set(MAIN_SOURCE_FILE 
    main/Buffer.hpp 
    main/Buffer.cpp 
    main/Callback.hpp
    main/Channel.hpp
    main/Channel.cpp
    main/Epoller.hpp
    main/Epoller.cpp
    main/Event.hpp
    main/EventLoop.hpp
    main/EventLoop.cpp
    #main/http/http.hpp 
    #main/http/http.cpp
    main/InetAddr.hpp
    main/InetAddr.cpp
    main/ListenSocket.hpp
    main/ListenSocket.cpp
    main/Socket.hpp
    main/Socket.cpp
    main/Timer.hpp
    main/Timer.cpp
    main/TimerId.hpp
    main/TimerId.cpp
    main/TimerQueue.hpp
    main/TimerQueue.cpp
    main/WakeUp.hpp
    main/WakeUp.cpp)

add_library(${PROJECT_NAME} ${AUX_SOURCE_FILES} ${MAIN_SOURCE_FILE})
add_library(chaining::lib ALIAS ${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} 
    PUBLIC ${PROJECT_SOURCE_DIR})