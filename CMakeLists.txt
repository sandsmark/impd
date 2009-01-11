cmake_minimum_required(VERSION 2.6)

project(impd)

set (impd_srcs
    main.cpp
    collection.cpp
    server.cpp
    clientthread.cpp
    dialog.cpp
    player.cpp
)

set (impd_moc_hdrs
    server.h
    collection.h
    clientthread.h
    dialog.h
    player.h
)

add_definitions(-Wall)
set(QT_USE_QTNETWORK TRUE)
set(QT_USE_PHONON TRUE)
set(QT_USE_QTSQL TRUE)


find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

qt4_wrap_cpp(impd_moc_srcs ${impd_moc_hdrs})

add_executable(impd ${impd_srcs} ${impd_moc_srcs})

target_link_libraries(impd ${QT_LIBRARIES})

