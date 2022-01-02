#include "ui/GalaxyEditorForm.h"
#include "ui_GalaxyEditorForm.h"

#include <ui/Blackhole.h>

GalaxyEditorForm::GalaxyEditorForm(QWidget *parent, QString galaxyName) :
    QDialog(parent), m_ui(new Ui::GalaxyEditorForm)
{
    m_ui->setupUi(this);
    setWindowTitle(QString::fromStdString(blackholeName) + " - Editing " + galaxyName);

}

GalaxyEditorForm::~GalaxyEditorForm()
{

}
