#include "mf_uid_parameterdialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include "ui_mf_uid_parameterdialog.h"

MF_UID_parameterDialog::MF_UID_parameterDialog(const QString& uid, const QString& atqa, const QString& sak, const QString& block0, const QVariantMap& config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MF_UID_parameterDialog)
{
    ui->setupUi(this);
    ui->UIDLineEdit->setText(uid);
    ui->ATQALineEdit->setText(atqa);
    ui->SAKLineEdit->setText(sak);
    this->config = config;
    this->block0 = block0;
}

MF_UID_parameterDialog::~MF_UID_parameterDialog()
{
    delete ui;
}

void MF_UID_parameterDialog::on_buttonBox_accepted()
{
    QString cmd = config["cmd"].toString();
    QString uid = ui->UIDLineEdit->text().remove(" ").toUpper();
    QString atqa = ui->ATQALineEdit->text().remove(" ").toUpper();
    QString sak = ui->SAKLineEdit->text().remove(" ").toUpper();

    if(cmd.contains("<block0>"))
    {
        if(!QRegularExpression("^[0-9A-F]{8}$").match(uid).hasMatch() ||
                !QRegularExpression("^[0-9A-F]{4}$").match(atqa).hasMatch() ||
                !QRegularExpression("^[0-9A-F]{2}$").match(sak).hasMatch())
        {
            QMessageBox::information(this, tr("Info"), tr("UID/ATQA/SAK format is invalid."));
            return;
        }

        quint8 bcc = 0;
        for(int i = 0; i < 4; i++)
            bcc ^= uid.mid(i * 2, 2).toUInt(nullptr, 16);

        QString tail = QRegularExpression("^[0-9A-F]{32}$").match(block0).hasMatch() ? block0.mid(16, 16) : "0000000000000000";
        QString data = uid + QString("%1").arg(bcc, 2, 16, QLatin1Char('0')).toUpper() + sak + atqa.mid(2, 2) + atqa.mid(0, 2) + tail;
        cmd.replace("<block0>", data);
    }

    cmd.replace("<uid>", uid);
    cmd.replace("<atqa>", atqa);
    cmd.replace("<sak>", sak);
    emit sendCMD(cmd);
    accept();
}
