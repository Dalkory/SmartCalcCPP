#include "s21_view_credit.h"

#include "ui_s21_view_credit.h"

namespace s21 {

s21::ViewCredit::ViewCredit(QWidget *parent)
    : QDialog(parent), ui_(new Ui::ViewCredit) {
  ui_->setupUi(this);
  setWindowTitle("Credit");
  connect(ui_->raschet, SIGNAL(clicked()), this, SLOT(CalculateCredit()));
}

s21::ViewCredit::~ViewCredit() { delete ui_; }

void s21::ViewCredit::CalculateCredit() {
  QString result_text;
  s21::CreditParams params;

  params.credit_sum = (double)ui_->summa->value();
  params.percent = (double)ui_->percent->value();

  if (ui_->moutch_or_god->currentText() == "Мес.") {
    params.month_time = ui_->spin_skok_moutch_or_god->value();
  } else {
    params.month_time = ui_->spin_skok_moutch_or_god->value() * 12;
  }

  if (ui_->radio_anuitet->isChecked() == true) {
    controller_credit_.CalculateAnnuityPayment(params);
    result_text = QString::number(params.annuity_payment, 'f', 2);
    ui_->every_moutch_pay->setText(result_text);
  } else {
    controller_credit_.CalculateDifferentiatedPayment(params);
    result_text = QString::number(params.max_payment, 'f', 2) + " ... " +
                  QString::number(params.min_payment, 'f', 2);
    ui_->every_moutch_pay->setText(result_text);
  }

  result_text = QString::number(params.overpay, 'f', 2);
  ui_->pereplata->setText(result_text);
  result_text = QString::number(params.total_payment, 'f', 2);
  ui_->result->setText(result_text);
}

}  // namespace s21
