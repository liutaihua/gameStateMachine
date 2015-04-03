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
    MoveState.cc \
    Creature.cc \
    ReadyState.cc \
    StateMachine.cc \
    CreatureObserver.cc \
    Creature.cc \
    Pet.cc \
    AI/AI.cpp \
    AI/AIMoving.cpp \
    AI/AIPatrol.cpp \
    AI/AIPet.cpp \
    AI/AIAttacking.cpp \
    AI/AIMovingToTarget.cpp \
    Player.cc

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
    Creature.h \
    ReadyState.h \
    StateMachine.h \
    Utility.h \
    CreatureObserver.h \
    fd/delegate.hpp \
    fd/resolution.hpp \
    fd/delegate/bind.hpp \
    fd/delegate/delegate_base.hpp \
    fd/delegate/delegate_template.hpp \
    fd/delegate/delegate0.hpp \
    fd/delegate/delegate1.hpp \
    fd/delegate/delegate2.hpp \
    fd/delegate/delegate3.hpp \
    fd/delegate/delegate4.hpp \
    fd/delegate/delegate5.hpp \
    fd/delegate/delegate6.hpp \
    fd/delegate/delegate7.hpp \
    fd/delegate/delegate8.hpp \
    fd/delegate/delegate9.hpp \
    fd/delegate/delegate10.hpp \
    fd/delegate/bind/bind_template.hpp \
    fd/delegate/bind/bind0.hpp \
    fd/delegate/bind/bind1.hpp \
    fd/delegate/bind/bind2.hpp \
    fd/delegate/bind/bind3.hpp \
    fd/delegate/bind/bind4.hpp \
    fd/delegate/bind/bind5.hpp \
    fd/delegate/bind/bind6.hpp \
    fd/delegate/bind/bind7.hpp \
    fd/delegate/bind/bind8.hpp \
    fd/delegate/bind/bind9.hpp \
    fd/delegate/bind/bind10.hpp \
    fd/delegate/detail/delegate_cc_impl.hpp \
    fd/delegate/detail/delegate_cc.hpp \
    fd/delegate/detail/delegate_iterate.hpp \
    fd/delegate/detail/delegate_mfn_cc_impl.hpp \
    fd/delegate/detail/delegate_mfn_cc_ps.hpp \
    fd/delegate/detail/delegate_mfn_cc.hpp \
    fd/delegate/detail/maybe_include.hpp \
    fd/delegate/detail/multicast_call_iterator.hpp \
    fd/delegate/detail/prolouge.hpp \
    fd/delegate/bind/detail/bind_iterate.hpp \
    fd/delegate/bind/detail/bind_mfn_cc.hpp \
    fd/delegate/bind/detail/maybe_include.hpp \
    fd/delegate/bind/detail/prolouge.hpp \
    fd/resolution/detail/maybe_include.hpp \
    fd/resolution/detail/prolouge.hpp \
    fd/resolution/detail/resolution_cc_select.hpp \
    fd/resolution/detail/resolution_iterate.hpp \
    fd/resolution/detail/resolution_mfn_cc_select.hpp \
    fd/resolution/resolution_base.hpp \
    fd/resolution/resolution_template.hpp \
    fd/resolution/resolution0.hpp \
    fd/resolution/resolution1.hpp \
    fd/resolution/resolution2.hpp \
    fd/resolution/resolution3.hpp \
    fd/resolution/resolution4.hpp \
    fd/resolution/resolution5.hpp \
    fd/resolution/resolution6.hpp \
    fd/resolution/resolution7.hpp \
    fd/resolution/resolution8.hpp \
    fd/resolution/resolution9.hpp \
    fd/resolution/resolution10.hpp \
    Pet.h \
    Creature.h \
    AI/AI.h \
    AI/AIMoving.h \
    AI/AIPatrol.h \
    AI/AIPet.h \
    AI/AIAttacking.h \
    AI/AIMovingToTarget.h \
    Player.h

