TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    murmur.c \
    bitmap.cpp \
    countingbloomfilter.cpp \
    scalingbloomfilter.cpp

HEADERS  += \
    murmur.h \
    bitmap.h \
    countingbloomfilter.h \
    scalingbloomfilter.h
include(deployment.pri)
qtcAddDeployment()

