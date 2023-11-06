#ifndef CPP3_S21_SMARTCALC_SRC_S21_VIEW_DEPOSIT_H_
#define CPP3_S21_SMARTCALC_SRC_S21_VIEW_DEPOSIT_H_

#include <QDialog>

#include "./controller/s21_controller_deposit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ViewDeposit;
}
QT_END_NAMESPACE

namespace s21 {

class ViewDeposit : public QDialog {
  Q_OBJECT

 public:
  explicit ViewDeposit(QWidget *parent = nullptr);
  ~ViewDeposit();

 private slots:
  void CalculateDeposit();

 private:
  Ui::ViewDeposit *ui_;
  s21::ControllerDeposit controller_deposit_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_VIEW_DEPOSIT_H_
