###########################################################################
# Copyright (C) 2013 by Artem 'DOOMer' Galichkin *
# doomer3d@gmail.com *
# 
# This program is free software; you can redistribute it and/or modify *
# it under the terms of the GNU General Public License as published by *
# the Free Software Foundation; either version 2 of the License, or *
# (at your option) any later version. *
# 
# This program is distributed in the hope that it will be useful, *
# but WITHOUT ANY WARRANTY; without even the implied warranty of *
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
# GNU General Public License for more details. *
# 
# You should have received a copy of the GNU General Public License *
# along with this program; if not, write to the *
# Free Software Foundation, Inc., *
# 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
###########################################################################

cmake_minimum_required(VERSION 2.8.8)
 
set(PROJECT_NAME qbspinfo)
project(${PROJECT_NAME})
 
set(VERSION "0.5")
add_definitions( -DVERSION="${VERSION}")

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" ) 

set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
find_package(Qt5Widgets REQUIRED)
 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
 
set(QBSPINFOTOOL_SRC
src/main.cpp 
src/core/qbspinfocore.cpp 
src/core/config.cpp 
src/core/entityitem.cpp 
src/core/entitymodel.cpp 
src/ui/mainwindow.cpp
src/ui/aboutdialog.cpp
src/ui/configdialog.cpp 
)
 
set(QBSPINFOTOOL_HDR
src/core/config.h 
src/core/entityitem.h 
src/core/entitymodel.h 
src/core/qbspinfocore.h 
src/ui/mainwindow.h
src/ui/aboutdialog.h
src/ui/configdialog.h 
)
 
set(QBSPINFOTOOL_UI
src/ui/mainwindow.ui
src/ui/aboutdialog.ui
src/ui/configdialog.ui 
)
 
# Wrap files, create moc files.
qt5_wrap_cpp(QBSPINFOTOOL_MOC ${QBSPINFOTOOL_HDR})
qt5_wrap_ui(QBSPINFOTOOL_UI_SRC ${QBSPINFOTOOL_UI})
#qt5_add_resources(QBSPINFOTOOL_QRC ${QBSPINFOTOOL_QRC_SRC})
add_executable(${PROJECT_NAME} ${QBSPINFOTOOL_SRC} ${QBSPINFOTOOL_MOC} ${QBSPINFOTOOL_UI_SRC} )
qt5_use_modules(${PROJECT_NAME} Widgets)