#include "./view/s21_view_main.h"

#include "./view/qcustomplot.h"
#include "ui_s21_view_main.h"

namespace s21 {

s21::ViewCalc::ViewCalc(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::ViewCalc) {
  ui_->setupUi(this);
  setWindowTitle("Calculator");
  setFixedWidth(335);
  setFixedHeight(600);

  const QPushButton *numbers_buttons[] = {
      ui_->btn_0, ui_->btn_1, ui_->btn_2, ui_->btn_3, ui_->btn_4,   ui_->btn_5,
      ui_->btn_6, ui_->btn_7, ui_->btn_8, ui_->btn_9, ui_->btn_dot, ui_->btn_x};
  for (const auto *button : numbers_buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(InputNumbers()));
  }

  const QPushButton *arithmetic_buttons[] = {
      ui_->btn_plus, ui_->btn_minus, ui_->btn_mult, ui_->btn_div, ui_->btn_pow};
  for (const auto *button : arithmetic_buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(InputArithmetic()));
  }

  const QPushButton *function_buttons[] = {
      ui_->btn_sin,  ui_->btn_cos,  ui_->btn_tan, ui_->btn_asin, ui_->btn_acos,
      ui_->btn_atan, ui_->btn_sqrt, ui_->btn_ln,  ui_->btn_log,  ui_->btn_mod};
  for (const auto *button : function_buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(InputFuncs()));
  }

  const QPushButton *operation_buttons[] = {
      ui_->btn_open_skob, ui_->btn_close_skob, ui_->btn_plus_minus,
      ui_->btn_percent};
  for (const auto *button : operation_buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(InputOperations()));
  }

  connect(ui_->btn_CE, SIGNAL(clicked()), this, SLOT(Clear()));
  connect(ui_->btn_AC, SIGNAL(clicked()), this, SLOT(Clear()));
  connect(ui_->btn_stantard, SIGNAL(toggled(bool)), this, SLOT(EnabledX()));
  connect(ui_->btn_add_graph, SIGNAL(toggled(bool)), this, SLOT(EnabledX()));
  connect(ui_->btn_equal, SIGNAL(clicked()), this, SLOT(Calculate()));
  connect(ui_->btn_graph, SIGNAL(clicked()), this, SLOT(MakeGraph()));
  connect(ui_->btn_credit_calc, SIGNAL(clicked()), this, SLOT(StartCredit()));
  connect(ui_->btn_depozit_calc, SIGNAL(clicked()), this, SLOT(StartDeposit()));
}

s21::ViewCalc::~ViewCalc() { delete ui_; }

void s21::ViewCalc::InputNumbers() {
  QPushButton *button = (QPushButton *)sender();
  QString result_line = ui_->result_line->text();
  if (result_line == "Ошибка ввода") {
    ui_->result_line->clear();
    result_line = ui_->result_line->text();
  }
  if (button == ui_->btn_x) {
    ui_->result_line->setText(result_line + "x");
  } else if (button == ui_->btn_dot &&
             (!result_line.isEmpty() && !result_line.back().isDigit())) {
    ui_->result_line->setText(result_line + "0.");
  } else {
    ui_->result_line->setText(result_line + button->text());
  }
}

void s21::ViewCalc::InputFuncs() {
  QPushButton *button = (QPushButton *)sender();
  QString result_line = ui_->result_line->text();
  if (result_line == "Ошибка ввода") {
    ui_->result_line->clear();
    result_line = ui_->result_line->text();
  }
  if (button == ui_->btn_sin) {
    result_line += "sin(";
  } else if (button == ui_->btn_cos) {
    result_line += "cos(";
  } else if (button == ui_->btn_tan) {
    result_line += "tan(";
  } else if (button == ui_->btn_asin) {
    result_line += "asin(";
  } else if (button == ui_->btn_acos) {
    result_line += "acos(";
  } else if (button == ui_->btn_atan) {
    result_line += "atan(";
  } else if (button == ui_->btn_sqrt) {
    result_line += "sqrt(";
  } else if (button == ui_->btn_mod) {
    result_line += "mod";
  } else if (button == ui_->btn_ln) {
    result_line += "ln(";
  } else if (button == ui_->btn_log) {
    result_line += "log(";
  }
  ui_->result_line->setText(result_line);
}

void s21::ViewCalc::InputOperations() {
  QPushButton *button = (QPushButton *)sender();
  QString result_line = ui_->result_line->text();
  if (result_line == "Ошибка ввода") {
    ui_->result_line->clear();
    result_line = ui_->result_line->text();
  }
  if (button == ui_->btn_open_skob) {
    result_line += "(";
  } else if (button == ui_->btn_close_skob) {
    result_line += ")";
  } else if (button->text() == "+/-") {
    double all_numbers = result_line.toDouble();
    all_numbers *= -1;
    result_line = QString::number(all_numbers, 'g', 15);
  } else if (button->text() == "%") {
    double all_numbers = result_line.toDouble();
    all_numbers *= 0.01;
    result_line = QString::number(all_numbers, 'g', 15);
  }
  ui_->result_line->setText(result_line);
}

void s21::ViewCalc::InputArithmetic() {
  QPushButton *button = (QPushButton *)sender();
  QString result_line = ui_->result_line->text();
  if (result_line == "Ошибка ввода") {
    ui_->result_line->clear();
    result_line = ui_->result_line->text();
  }
  if (button == ui_->btn_plus) {
    result_line += "+";
  } else if (button == ui_->btn_minus) {
    result_line += "-";
  } else if (button == ui_->btn_mult) {
    result_line += "*";
  } else if (button == ui_->btn_div) {
    result_line += "/";
  } else if (button == ui_->btn_pow) {
    result_line += "^";
  }
  ui_->result_line->setText(result_line);
}

void s21::ViewCalc::Calculate() {
  QString current_text = ui_->result_line->text();
  if (current_text.length() > 255) {
    ui_->result_line->setText("Слишком длинное выражение");
  }
  double result = 0;
  double x = ui_->x_value_line->text().toDouble();
  QByteArray tmp_byte_array = current_text.toLatin1();
  char *expression = tmp_byte_array.data();
  int status = controller_calc_.CalculateExpression(expression, &result, x);
  if (status == 0) {
    ui_->result_line->setText(QString::number(result));
  } else if (status == 1) {
    ui_->result_line->setText("CALCULATION ERROR");
  } else if (status == 3) {
    ui_->result_line->setText("INCORRECT INPUT");
  } else if (status == 2) {
    ui_->result_line->setText("MEMORY ERROR");
  } else if (status == 4) {
    ui_->result_line->setText("nan");
  } else if (status == 5) {
    ui_->result_line->setText("inf");
  }
}

void s21::ViewCalc::MakeGraph() {
  QString current_text = ui_->result_line->text();
  if (current_text.length() > 255) {
    ui_->result_line->setText("Слишком длинное выражение");
  }
  if (flag_plot_) {
    ui_->custom_plot->addGraph();
    ui_->custom_plot->graph(0)->data()->clear();
    ui_->custom_plot->replot();
  }
  QVector<double> vector_x, vector_y;
  double h = 0.1;
  bool x_max_detector, x_min_detector, y_min_detector, y_max_detector = false;
  int x_max = ui_->x_max->text().toInt(&x_max_detector);
  int x_min = ui_->x_min->text().toInt(&x_min_detector);
  int y_max = ui_->y_max->text().toInt(&y_min_detector);
  int y_min = ui_->y_min->text().toInt(&y_max_detector);
  if (x_max_detector && x_min_detector && y_min_detector && y_max_detector &&
      x_min < x_max && y_min < y_max) {
    double x_begin = x_min;
    double x_end = x_max;
    ui_->custom_plot->xAxis->setRange(x_min, x_max);
    ui_->custom_plot->yAxis->setRange(y_min, y_max);
    QByteArray tmp_byte_array = current_text.toLatin1();
    char *expression = tmp_byte_array.data();
    for (double x = x_begin; x <= x_end; x += h) {
      double y;
      int status = controller_calc_.CalculateExpression(expression, &y, x);
      if (status == 0) {
        ui_->result_line->setText(QString::number(y));
      } else if (status == 1) {
        ui_->result_line->setText("CALCULATION ERROR");
      } else if (status == 3) {
        ui_->result_line->setText("INCORRECT INPUT");
      } else if (status == 2) {
        ui_->result_line->setText("MEMORY ERROR");
      } else if (status == 4) {
        ui_->result_line->setText("nan");
      } else if (status == 5) {
        ui_->result_line->setText("inf");
      }
      vector_y.push_back(y);
      vector_x.push_back(x);
    }
    ui_->custom_plot->addGraph();
    ui_->custom_plot->graph(0)->addData(vector_x, vector_y);
    ui_->custom_plot->replot();
    flag_plot_++;
  } else {
    ui_->result_line->setText("Error, please input xmax xmin ymax ymin");
  }
}

void s21::ViewCalc::StartView(QDialog *view) {
  view->setModal(true);  //  окно установлено в режим модальности, это означает,
                         //  что оно блокирует ввод в другие окна
  view->show();
  view->exec();
}

void s21::ViewCalc::StartCredit() {
  view_credit_ = new s21::ViewCredit();
  StartView(view_credit_);
}

void s21::ViewCalc::StartDeposit() {
  view_deposit_ = new s21::ViewDeposit();
  StartView(view_deposit_);
}

void s21::ViewCalc::EnabledX() {
  QRadioButton *radio_button = (QRadioButton *)sender();
  if (radio_button == ui_->btn_add_graph) {
    ui_->x_value_line->setEnabled(false);
    ui_->btn_graph->setEnabled(true);
    setFixedWidth(1005);
    setFixedHeight(600);
  } else {
    ui_->x_value_line->setEnabled(true);
    ui_->btn_graph->setEnabled(false);
    setFixedWidth(335);
    setFixedHeight(600);
  }
}

void s21::ViewCalc::Clear() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button == ui_->btn_AC) {
    ui_->result_line->clear();
  } else {
    QString result_line = ui_->result_line->text();
    result_line.chop(1);
    ui_->result_line->setText(result_line);
  }
}

}  // namespace s21
