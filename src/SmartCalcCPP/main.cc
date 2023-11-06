#include <QApplication>

#include "./view/s21_view_main.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::ViewCalc w;
  w.show();
  return a.exec();
}
