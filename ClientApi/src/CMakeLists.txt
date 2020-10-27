project(ClientApi LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

qt5_add_resources(RCC_SOURCES resources.qrc)

set(SOURCES
    main.cpp
    ../../ServerApplication/src/authanswer.cpp
    ../../ServerApplication/src/basedata.cpp
    ../../ServerApplication/src/userlist.cpp
    ../../ServerApplication/src/userdata.cpp
    ../../ServerApplication/src/message.cpp
    ../../ServerApplication/src/connectionhandler.cpp
    ../../ServerApplication/src/authanswer.h
    ../../ServerApplication/src/basedata.h
    ../../ServerApplication/src/userlist.h
    ../../ServerApplication/src/userdata.h
    ../../ServerApplication/src/message.h
    ../../ServerApplication/src/connectionhandler.h
    SettingsWrapper.cpp
    chatwidget.cpp
    chatwidget.h
    authmenu.cpp
    netservice.cpp
    styledialog.cpp
    authmenu.h
    netservice.h
    styledialog.h
    authmenu.ui
    chatwidget.ui
    styledialog.ui
)

add_executable(ClientApi ${RCC_SOURCES} ${SOURCES})

set_property(TARGET ClientApi PROPERTY CXX_STANDARD 11)
target_link_libraries(ClientApi Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network)

install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Network> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
