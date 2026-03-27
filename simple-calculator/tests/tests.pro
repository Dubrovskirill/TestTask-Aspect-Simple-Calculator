QT += testlib core

QT += gui 

CONFIG += qt warn_on console
CONFIG -= app_bundle

TARGET = tst_calculator

VPATH += ../src/model \
         ../src/presenter

INCLUDEPATH += ../src/model \
               ../src/presenter

SOURCES += TestCalculator.cpp \
           ../src/model/ArithmeticFactory.cpp \
           ../src/model/CalculatorModel.cpp \
           ../src/presenter/CalculatorPresenter.cpp

HEADERS += ../src/model/ArithmeticFactory.h \
           ../src/model/ArithmeticStrategies.h \
           ../src/model/CalculatorModel.h \
           ../src/model/IArithmeticStrategy.h \
           ../src/presenter/CalculatorPresenter.h