cmake_minimum_required(VERSION 2.6)

project(moped)

set (moped_srcs
    main.cpp
    collection.cpp
    server.cpp
    clientthread.cpp
    dialog.cpp
)

set (moped_moc_hdrs
    server.h
    collection.h
    clientthread.h
    dialog.h
)

add_definitions(-Wall)
set(QT_USE_QTNETWORK TRUE)
set(QT_USE_PHONON TRUE)
set(QT_USE_QTSQL TRUE)


find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

qt4_wrap_cpp(moped_moc_srcs ${moped_moc_hdrs})

add_executable(moped ${moped_srcs} ${moped_moc_srcs})

target_link_libraries(moped ${QT_LIBRARIES})

