#include "qwdialog.h"
#include "ui_qwdialog.h"

QSpinBox* spi[10][6],*Ratime[4],*Rltime[4];
QComboBox* Caldate[30];
int Line=0,Product,Pnum,year,mon,day,period,perN;
int P2R[9][5],Rtime[4],Rtltime[4];
bool stab=false;
struct DateList{
    int R1;
    int R2;
    int R3;
    int R4;
};
DateList Ddate[360*4];
int getmultibitnum(QString::iterator&,QChar);
QWDialog::QWDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWDialog)
{
    ui->setupUi(this);
}

QWDialog::~QWDialog()
{
    delete ui;
}

void QWDialog::on_pushButtonADD_clicked()
{
    if(!stab){
        QString s="Error!Set Stability Rules First!!";
        ui->textEditDyRules->append(s);
        return;
    }
    Product=ui->comboBoxProduct->currentIndex();
    Pnum=ui->spinBoxPnum->text().toInt();
    year=ui->spinBoxyear->text().toInt();
    mon=ui->spinBox_2mon->text().toInt();
    day=ui->spinBox_3day->text().toInt();
    period=ui->spinBox_5period->text().toInt();
    perN=ui->spinBoxPerN->text().toInt();
    Line++;
    QString s;
    s="Rule"+ QString::number(Line)+'\t'+'P'+QString::number(Product+1)+'*'+QString::number(Pnum)+'\t'+QString::number(year)\
            +'.'+QString::number(mon)+'.'+QString::number(day)+'\t'+QString::number(period)+'*'+QString::number(perN);
    ui->textEditDyRules->append(s);
    QString Ls="Rule"+QString::number(Line);
    ui->comboBox_2Select->addItem(Ls,Line);
    int AchieveDate=(year-1)*360+(mon-1)*30+day-1;
    for(;perN;perN--){
        int tmpdate=AchieveDate-Rtime[0];
        Ddate[tmpdate].R1+=Pnum*P2R[0][Product];
        tmpdate=AchieveDate-Rtime[1];
        Ddate[tmpdate].R2+=Pnum*P2R[1][Product];
        tmpdate=AchieveDate-Rtime[2];
        Ddate[tmpdate].R3+=Pnum*P2R[2][Product];
        tmpdate=AchieveDate-Rtime[3];
        Ddate[tmpdate].R4+=Pnum*P2R[3][Product];
        AchieveDate+=period;
        if(AchieveDate>=360*4) break;
    }
    Routput();
}

void QWDialog::on_pushButton_clicked()//this is delete
{
    int index=ui->comboBox_2Select->currentIndex();
    if(index==-1)return;
    int line=ui->comboBox_2Select->currentData().toInt();
    QTextCursor txtcur= ui->textEditDyRules->textCursor();
    txtcur.setPosition(0);
    for(;line;line--){
        txtcur.movePosition(QTextCursor::Down);
    }
    txtcur.select(QTextCursor::LineUnderCursor);
    QString qstr=txtcur.selectedText();
    txtcur.removeSelectedText();
    ui->comboBox_2Select->removeItem(index);
    QString::iterator p=qstr.end()-1;
    perN=getmultibitnum(p,'*');
    --p;
    period=getmultibitnum(p,'\t');
    --p;
    day=getmultibitnum(p,'.');
    --p;
    mon=getmultibitnum(p,'.');
    --p;
    year=getmultibitnum(p,'\t');
    --p;
    Pnum=getmultibitnum(p,'*');
    --p;
    Product=getmultibitnum(p,'P')-1;
    int AchieveDate=(year-1)*360+(mon-1)*30+day-1;
    for(;perN;perN--){
        int tmpdate=AchieveDate-Rtime[0];
        Ddate[tmpdate].R1-=Pnum*P2R[0][Product];
        tmpdate=AchieveDate-Rtime[1];
        Ddate[tmpdate].R2-=Pnum*P2R[1][Product];
        tmpdate=AchieveDate-Rtime[2];
        Ddate[tmpdate].R3-=Pnum*P2R[2][Product];
        tmpdate=AchieveDate-Rtime[3];
        Ddate[tmpdate].R4-=Pnum*P2R[3][Product];
        AchieveDate+=period;
        if(AchieveDate>=360*4) break;
    }
    Routput();
}
void QWDialog::qwinit(){
    ui->tableWidgetStableRules->setRowCount(10);
    ui->tableWidgetStableRules->setColumnCount(6);
    ui->tableWidgetStableRules->verticalHeader()->setVisible(false);
    ui->tableWidgetStableRules->horizontalHeader()->setVisible(false);
    for(int i=1;i<=5;i++){
        QString stri="P"+QString::number(i);
        QTableWidgetItem* tabi=new QTableWidgetItem(stri);
        ui->tableWidgetStableRules->setItem(0,i,tabi);
    }
    for(int i=1;i<=4;i++){
        QString stri="R"+QString::number(i);
        QTableWidgetItem* tabi=new QTableWidgetItem(stri);
        ui->tableWidgetStableRules->setItem(i,0,tabi);
    }
    for(int i=5;i<=9;i++){
        QString stri="P"+QString::number(i-4);
        QTableWidgetItem* tabi=new QTableWidgetItem(stri);
        ui->tableWidgetStableRules->setItem(i,0,tabi);
    }
    for(int i=1;i<=9;i++)
        for(int j=1;j<=5;j++){
            spi[i][j]=new QSpinBox();
            ui->tableWidgetStableRules->setCellWidget(i,j,spi[i][j]);
        }
    ui->tableWidgettab2Rtime->setRowCount(3);
    ui->tableWidgettab2Rtime->setColumnCount(5);
    ui->tableWidgettab2Rtime->verticalHeader()->setVisible(false);
    ui->tableWidgettab2Rtime->horizontalHeader()->setVisible(false);
    for(int i=1;i<=4;i++){
        QString stri="R"+QString::number(i);
        QTableWidgetItem* tabi=new QTableWidgetItem(stri);
        ui->tableWidgettab2Rtime->setItem(0,i,tabi);
    }
    QString stri="achieve time";
    QTableWidgetItem* tabi=new QTableWidgetItem(stri);
    ui->tableWidgettab2Rtime->setItem(1,0,tabi);
    stri="live time";
    tabi=new QTableWidgetItem(stri);
    ui->tableWidgettab2Rtime->setItem(2,0,tabi);
    for(int i=0;i<4;i++){
        Ratime[i]=new QSpinBox();
        ui->tableWidgettab2Rtime->setCellWidget(1,i+1,Ratime[i]);
    }
    for(int i=0;i<4;i++){
        Rltime[i]=new QSpinBox();
        ui->tableWidgettab2Rtime->setCellWidget(2,i+1,Rltime[i]);
    }
    ui->tableCalenderOutput->setRowCount(6);
    ui->tableCalenderOutput->setColumnCount(5);
    ui->tableCalenderOutput->verticalHeader()->setVisible(false);
    ui->tableCalenderOutput->horizontalHeader()->setVisible(false);
    ui->tableCalenderOutput->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int k=0;
    for(int i=0;i<6;i++)
        for(int j=0;j<5;j++){
            Caldate[k]=new QComboBox();
            //Caldate[k]->addItem("None");
            ui->tableCalenderOutput->setCellWidget(i,j,Caldate[k]);
            ++k;
        }
}

void QWDialog::on_pushButtonTab2_clicked()
{
    ui->pushButtonTab2->setEnabled(false);
    ui->tableWidgetStableRules->setEnabled(false);
    ui->tableWidgettab2Rtime->setEnabled(false);
    for(int i=0;i<9;i++)
        for(int j=0;j<5;j++){
            P2R[i][j]=spi[i+1][j+1]->value();
        }
    for(int i=0;i<4;i++){
        Rtime[i]=Ratime[i]->value();
        Rtltime[i]=Rltime[i]->value();
    }
    stab=true;
    ui->textEditDyRules->clear();
    QString s="-----------------Top Line---------------";
    ui->textEditDyRules->append(s);
    Routput();
}

void QWDialog::on_pushButton_3LastM_clicked()
{
    int y=ui->comboBoxOutputYear->currentIndex()+1;
    int m=ui->comboBoxOutputMonth->currentIndex()+1;
    if(y==1&&m==1) return;
    if(m==1){
        --y;
        m=12;
    }
    else --m;
    ui->comboBoxOutputYear->setCurrentIndex(y-1);
    ui->comboBoxOutputMonth->setCurrentIndex(m-1);
    Routput();
}

void QWDialog::on_pushButton_2NextM_clicked()
{
    int y=ui->comboBoxOutputYear->currentIndex()+1;
    int m=ui->comboBoxOutputMonth->currentIndex()+1;
    if(y==4&&m==12) return;
    if(m==12){
        ++y;
        m=1;
    }
    else ++m;
    ui->comboBoxOutputYear->setCurrentIndex(y-1);
    ui->comboBoxOutputMonth->setCurrentIndex(m-1);
    Routput();
}

void QWDialog::on_comboBoxOutputYear_currentIndexChanged(int)
{
    Routput();
}

void QWDialog::on_comboBoxOutputMonth_currentIndexChanged(int)
{
    Routput();
}
void QWDialog::Routput(){
    int sumR[4]={0};
    int y=ui->comboBoxOutputYear->currentIndex();
    int m=ui->comboBoxOutputMonth->currentIndex();
    int stDate=y*360+m*30;
    for(int k=0;k<30;k++) Caldate[k]->clear();
    for(int i=0;i<30;i++){
        bool f=false;
        int r1=Ddate[stDate].R1;
        if(r1){
            sumR[0]+=r1;
            Caldate[i]->clear();
            f=true;
            Caldate[i]->addItem("R1*"+QString::number(r1));
        }
        int r2=Ddate[stDate].R2;
        if(r2){
            sumR[1]+=r2;
            if(!f){Caldate[i]->clear();f=true;}
            Caldate[i]->addItem("R2*"+QString::number(r2));
        }
        int r3=Ddate[stDate].R3;
        if(r3){
            sumR[2]+=r3;
            if(!f){Caldate[i]->clear();f=true;}
            Caldate[i]->addItem("R3*"+QString::number(r3));
        }
        int r4=Ddate[stDate].R4;
        if(r4){
            sumR[3]+=r4;
            if(!f){Caldate[i]->clear();f=true;}
            Caldate[i]->addItem("R4*"+QString::number(r4));
        }
        stDate++;
    }
    ui->textEditOutputList->clear();
    QString s="R1*"+QString::number(sumR[0]);
    ui->textEditOutputList->append(s);
    s="R2*"+QString::number(sumR[1]);
    ui->textEditOutputList->append(s);
    s="R3*"+QString::number(sumR[2]);
    ui->textEditOutputList->append(s);
    s="R4*"+QString::number(sumR[3]);
    ui->textEditOutputList->append(s);
}
int getmultibitnum(QString::iterator &p,QChar ch){
    QString tmp;
    while(*p!=ch)tmp.push_front(*p--);
    return tmp.toInt();
}
