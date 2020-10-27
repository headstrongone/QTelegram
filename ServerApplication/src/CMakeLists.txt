project(ServerApplication LANGUAGES CXX)

find_package(Qt5 COMPONENTS Gui Network REQUIRED)

set(SOURCES
      main.cpp
      userlist.cpp
      settingswrapper.cpp
      userlist.h
      userdata.cpp
      userdata.h
      basedata.cpp
      basedata.h
      message.cpp
      message.h
      connectionhandler.cpp
      connectionhandler.h
      authanswer.cpp
      authanswer.h
)

add_executable(ServerApplication ${SOURCES})

set_property(TARGET ServerApplication PROPERTY CXX_STANDARD 11)
target_link_libraries(ServerApplication Qt5::Gui Qt5::Network)

install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Network> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
