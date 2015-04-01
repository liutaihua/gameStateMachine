TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    IState.cc \
    main.cc \
    Player.cc \
    ReadyState.cc \
    StateMachine.cc \
    IdleState.cc \
    CastingState.cc \
    DeadState.cc \
    MoveState.cc

OTHER_FILES += \
    main.o \
    Player.o \
    ReadyState.o \
    StateMachine.o \
    makefile \
    object.pro.user

HEADERS += \
    IState.h \
    Player.h \
    ReadyState.h \
    StateMachine.h \
    utility.h \
    IdleState.h \
    CastingState.h \
    DeadState.h \
    MoveState.h

