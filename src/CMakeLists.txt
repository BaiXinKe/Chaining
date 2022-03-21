project(MainLibrary)

set(AUX_SOURCE_FILES 
auxiliary/ErrorMsg.hpp 
auxiliary/InitLogger.hpp 
auxiliary/InitLogger.cpp 
auxiliary/Logger.hpp 
auxiliary/Nocopyable.hpp 
auxiliary/ReadSetting.hpp 
auxiliary/ReadSetting.cpp)

set(MAIN_SOURCE_FILE 
    main/Buffer.hpp main/Buffer.cpp)

add_library(${PROJECT_NAME} ${AUX_SOURCE_FILES} ${MAIN_SOURCE_FILE})
add_library(chaining::lib ALIAS ${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} 
    PUBLIC ${PROJECT_SOURCE_DIR})