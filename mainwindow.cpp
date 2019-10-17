#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ag.h"
#include "vector"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCalcular_clicked(){
    int populacao[2*TAMPOPULACAO][TAMCROMO];
    int populacaoFilhos[TAMPOPULACAO][TAMCROMO];
    struct resposta avaliacao[2*TAMPOPULACAO];
    int filho1[TAMCROMO],filho2[TAMCROMO];
    srand(time(NULL));
    iniciaPopulacao(populacao);
    int geracoes = ui->edGeracoes->text().toInt();
    int g = 1;
    int porcentagemCruzamento = ui->edCruzamento->text().toInt();

    while(g <= geracoes){

        ui->progressBar->setValue(100.0/geracoes * g);

        //imprimePopulacao(populacao);
        fazAvaliacao(populacao,avaliacao);
        //imprimeAvaliacao(avaliacao);
        int i=0;
        while(i<TAMPOPULACAO){
            int p1 = torneio(avaliacao,3);
            int p2 = torneio(avaliacao,3);
            if(rand()%100 < porcentagemCruzamento){
                cruzamento(populacao[p1],populacao[p2],filho1,filho2);
            }else{
                mutacao(populacao[p1],filho1);
                mutacao(populacao[p2],filho2);
            }
            copiaCromossomo(populacaoFilhos[i],filho1);
            i++;
            if(i<TAMPOPULACAO){
                copiaCromossomo(populacaoFilhos[i],filho2);
                i++;
            }
        }
        for(i=0;i<TAMPOPULACAO;i++){
            copiaCromossomo(populacao[TAMPOPULACAO+i],populacaoFilhos[i]);
            avaliacao[TAMPOPULACAO+i] = calculaAvaliacao(populacaoFilhos[i]);
        }

        int j;
        vector<int> Sp[2*TAMPOPULACAO];
        int np[2*TAMPOPULACAO];
        vector<vector<int> > fronteiras;
        domina(Sp,avaliacao);
        quantosTeDomina(np,Sp);
        divisaoPorFronteiras(&fronteiras,np,Sp);

        i=0;
        int f =0;
       int populacaoResultados[TAMPOPULACAO][TAMCROMO];
        i=0;
        f =0;
        while(i <= TAMPOPULACAO){
            int qf = fronteiras.at(f).size();
            if((i+qf) <= TAMPOPULACAO){
                for(j=0;j<qf;j++){
                     copiaCromossomo(populacaoResultados[i],
                             populacao[fronteiras.at(f).at(j)]);
                     i++;
             }
            }else
                break;

            f++;
        }
        j=0;
        vector <int> crow = crowDistance(&fronteiras.at(f),avaliacao);
        while(i < TAMPOPULACAO){
            copiaCromossomo(populacaoResultados[i],
                    populacao[crow.at(j)]);
            i++;
            j++;
        }
        for(i=0;i>TAMPOPULACAO;i++)
            copiaCromossomo(populacao[i],populacaoResultados[i]);
        g++;
    }
    fazAvaliacao(populacao,avaliacao);
    int i;
    for(i=0;i<TAMPOPULACAO;i++)
        if((avaliacao[i].multiplicacao==0)&&(avaliacao[i].soma==0))
            break;

    if(i<TAMPOPULACAO){
        for(int j=1;j<=10;j++){
             if(populacao[i][j]==0)
                 ui->lblSoma->setText(ui->lblSoma->text()+" "+ QString::number(j));
             else
                 ui->lblMultiplicacao->setText(ui->lblMultiplicacao->text()+" "+ QString::number(j));
         }}
    else{
         ui->lblSoma->setText("Resposta não encontrada");
         ui->lblMultiplicacao->setText("Resposta não encontrada");
         }
    printf(" ) valor do i e %d",i);








































}

void MainWindow::on_edMutacao_editingFinished()
{
    int x = ui->edMutacao->text().toInt();
    if(( x > 100) || (x < 0)){
        x = 15;
        ui->edMutacao->setText(QString::number(x));
    }

    int r = 100 - x;
    ui->edCruzamento->setText(QString::number(r));
}

void MainWindow::on_edCruzamento_editingFinished()
{
    int x = ui->edCruzamento->text().toInt();
    if(( x > 100) || (x < 0)){
        x = 15;
        ui->edCruzamento->setText(QString::number(x));
    }

    int r = 100 - x;
    ui->edMutacao->setText(QString::number(r));
}
