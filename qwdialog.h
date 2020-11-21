#ifndef QWDIALOG_H
#define QWDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class QWDialog; }
QT_END_NAMESPACE

class QWDialog : public QDialog
{
    Q_OBJECT

public:
    QWDialog(QWidget *parent = nullptr);
    ~QWDialog();
    void qwinit();
    void Routput();

private slots:
    void on_pushButton_clicked();

    void on_pushButtonADD_clicked();

    void on_pushButtonTab2_clicked();

    void on_pushButton_3LastM_clicked();

    void on_pushButton_2NextM_clicked();

    void on_comboBoxOutputYear_currentIndexChanged(int index);

    void on_comboBoxOutputMonth_currentIndexChanged(int index);

private:
    Ui::QWDialog *ui;
};
#endif // QWDIALOG_H
