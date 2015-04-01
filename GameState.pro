TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    CastingState.cc \
    DeadState.cc \
    IdleState.cc \
    IState.cc \
    main.cc \
    main.cpp \
    MoveState.cc \
    Player.cc \
    ReadyState.cc \
    StateMachine.cc

SUBDIRS += \
    object.pro

OTHER_FILES += \
    GameState.pro.user \
    makefile \
    object.pro.user

HEADERS += \
    CastingState.h \
    DeadState.h \
    IdleState.h \
    IState.h \
    MoveState.h \
    Player.h \
    ReadyState.h \
    StateMachine.h \
    Utility.h

