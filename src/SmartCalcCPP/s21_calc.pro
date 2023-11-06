QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cc \
           controller/s21_controller_calc.cc \
           controller/s21_controller_credit.cc \
           controller/s21_controller_deposit.cc \
           model/s21_model_calc.cc \
           model/s21_model_credit.cc \
           model/s21_model_deposit.cc \
           view/qcustomplot.cc \
           view/s21_view_main.cc \
           view/s21_view_credit.cc \
           view/s21_view_deposit.cc

HEADERS += controller/s21_controller_calc.h \
        controller/s21_controller_credit.h \
        controller/s21_controller_deposit.h \
        model/s21_model_calc.h \
        model/s21_model_credit.h \
        model/s21_model_deposit.h \
        view/qcustomplot.h \
        view/s21_view_main.h \
        view/s21_view_credit.h \
        view/s21_view_deposit.h

FORMS += view/s21_view_main.ui \
         view/s21_view_credit.ui \
         view/s21_view_deposit.ui 

CONFIG += lrelease
CONFIG += embed_translation

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
