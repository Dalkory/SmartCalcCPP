#include "s21_view_deposit.h"

#include <QString>

#include "ui_s21_view_deposit.h"

namespace s21 {

ViewDeposit::ViewDeposit(QWidget *parent)
    : QDialog(parent), ui_(new Ui::ViewDeposit) {
  ui_->setupUi(this);
  setWindowTitle("Deposit");
  connect(ui_->raschet, SIGNAL(clicked()), this, SLOT(CalculateDeposit()));
}

ViewDeposit::~ViewDeposit() { delete ui_; }

void ViewDeposit::CalculateDeposit() {
  QString result_text;
  s21::DepositParams params;
  params.initial_amount = ui_->summa_vklada->value();
  params.procent = ui_->procent->value();
  params.month_time = ui_->srok_mes->value();
  params.nalog = ui_->nalog->value();
  params.capitalization = ui_->kapitalizatsia->isChecked() ? 1 : 0;
  params.compound_frequency =
      ui_->periodic->currentIndex() == 0
          ? 1
          : (ui_->periodic->currentIndex() == 1 ? 12 : 4);
  params.withdrawal_amount = ui_->sum_snyatia->value();
  params.deposit_amount = ui_->sum_popolnenya->value();
  double ostatok = ui_->ostatok->value();
  if (ostatok >= (params.initial_amount - params.deposit_amount)) {
    ui_->error->setText("ERROR: Превышение допустимой суммы снятия");
  } else {
    ui_->error->setText("");
    controller_deposit_.CalculateDeposit(params);

    result_text = QString::number(params.profit, 'f', 2);
    ui_->profit->setText(result_text);

    result_text = QString::number(params.sum_nalog, 'f', 2);
    ui_->sum_nalog->setText(result_text);

    result_text = QString::number(params.final_amount, 'f', 2);
    ui_->sum_v_konce->setText(result_text);
  }
}

}  // namespace s21
