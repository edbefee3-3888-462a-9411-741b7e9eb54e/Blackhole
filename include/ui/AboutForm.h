#pragma once

#include <QDialog>
#include <QScopedPointer>

namespace Ui
{
class AboutForm;
}

/**
 * @todo write docs
 */
class AboutForm : public QDialog
{
    Q_OBJECT

public:
    explicit AboutForm(QWidget *parent);
    ~AboutForm() override;


private:
    QScopedPointer<Ui::AboutForm> m_ui;
};
