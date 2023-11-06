#ifndef CPP3_S21_SMARTCALC_SRC_S21_VIEW_MAIN_H_
#define CPP3_S21_SMARTCALC_SRC_S21_VIEW_MAIN_H_

#include <QMainWindow>

#include "./controller/s21_controller_calc.h"
#include "s21_view_credit.h"
#include "s21_view_deposit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ViewCalc;
}
QT_END_NAMESPACE

namespace s21 {

class ViewCalc : public QMainWindow {
  Q_OBJECT

 public:
  ViewCalc(QWidget *parent = nullptr);
  ~ViewCalc();

 private slots:
  void InputNumbers();
  void Clear();
  void InputFuncs();
  void InputArithmetic();
  void InputOperations();
  void EnabledX();
  void Calculate();
  void MakeGraph();
  void StartCredit();
  void StartView(QDialog *view);
  void StartDeposit();

 private:
  Ui::ViewCalc *ui_;
  s21::ControllerCalc controller_calc_;
  s21::ViewCredit *view_credit_;
  s21::ViewDeposit *view_deposit_;
  int flag_plot_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_VIEW_H_
