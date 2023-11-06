#ifndef CPP3_S21_SMARTCALC_SRC_S21_VIEW_CREDIT_H_
#define CPP3_S21_SMARTCALC_SRC_S21_VIEW_CREDIT_H_

#include <QDialog>

#include "./controller/s21_controller_credit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ViewCredit;
}
QT_END_NAMESPACE

namespace s21 {

class ViewCredit : public QDialog {
  Q_OBJECT

 public:
  explicit ViewCredit(QWidget *parent = nullptr);
  ~ViewCredit();

 private slots:
  void CalculateCredit();

 private:
  Ui::ViewCredit *ui_;
  s21::ControllerCredit controller_credit_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_VIEW_CREDIT_H_
