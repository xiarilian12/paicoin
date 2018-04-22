#include "paperkeycompletionpage.h"
#include "ui_paperkeycompletionpage.h"

#include <set>
#include <random>

PaperKeyCompletionPage::PaperKeyCompletionPage(QStringList words, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaperKeyCompletionPage),
    paperKeyList(words),
    firstEquality(false),
    secondEquality(false)
{
    ui->setupUi(this);

    SelectRandomKeys();
    EnableSubmitIfEqual();

    ui->lineEditWordFirst->setPlaceholderText(QString(tr("Word #%1")).arg(QString::number(firstIndex)));
    ui->lineEditWordSecond->setPlaceholderText(QString(tr("Word #%1")).arg(QString::number(secondIndex)));
    ui->lineEditWordFirst->setStyleSheet(Ui::LineEditStyleSheetFormat.arg(Ui::IconNameFail));
    ui->lineEditWordSecond->setStyleSheet(Ui::LineEditStyleSheetFormat.arg(Ui::IconNameFail));

    connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(onBackClicked()));
    connect(ui->pushButtonSubmit, SIGNAL(clicked()), this, SLOT(onSubmitClicked()));
    connect(ui->lineEditWordFirst, SIGNAL(textChanged(QString)), this, SLOT(textChangedFirst(QString)));
    connect(ui->lineEditWordSecond, SIGNAL(textChanged(QString)), this, SLOT(textChangedSecond(QString)));
}

PaperKeyCompletionPage::~PaperKeyCompletionPage()
{
    delete ui;
}

void PaperKeyCompletionPage::SelectRandomKeys()
{
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> uniformDistribution(1, PAPER_KEY_WORD_COUNT);

    std::set<int> verificationIndexes;
    while (verificationIndexes.size() <= 2)
        verificationIndexes.insert(uniformDistribution(generator));

    auto it = verificationIndexes.begin();

    std::advance(it, 1);
    firstIndex = *it;
    firstWord = paperKeyList.at(firstIndex - 1);

    std::advance(it, 1);
    secondIndex = *it;
    secondWord = paperKeyList.at(secondIndex - 1);
}

void PaperKeyCompletionPage::EnableSubmitIfEqual()
{
    if (firstEquality && secondEquality)
    {
        ui->pushButtonSubmit->setEnabled(true);
        ui->pushButtonSubmit->setStyleSheet(Ui::ButtonEnabledStyleSheet);
    }
    else
    {
        ui->pushButtonSubmit->setEnabled(false);
        ui->pushButtonSubmit->setStyleSheet(Ui::ButtonDisabledStyleSheet);
    }
}

void PaperKeyCompletionPage::SetLineEditState(QLineEdit *lineEdit, bool match)
{
    lineEdit->setStyleSheet(Ui::LineEditStyleSheetFormat.arg(match ? Ui::IconNamePass : Ui::IconNameFail)); // NOTE: pass/fail are image names!
}

void PaperKeyCompletionPage::onSubmitClicked()
{
    if (firstEquality && secondEquality)
        Q_EMIT paperKeyProven();
}

void PaperKeyCompletionPage::onBackClicked()
{
    Q_EMIT backToPreviousPage();
}

void PaperKeyCompletionPage::textChangedFirst(QString text)
{
    firstEquality = QString(paperKeyList.at(firstIndex - 1)).compare(text) == 0;
    SetLineEditState(ui->lineEditWordFirst, firstEquality);
    EnableSubmitIfEqual();
}

void PaperKeyCompletionPage::textChangedSecond(QString text)
{
    secondEquality = QString(paperKeyList.at(secondIndex - 1)).compare(text) == 0;
    SetLineEditState(ui->lineEditWordSecond, secondEquality);
    EnableSubmitIfEqual();
}