#ifndef AG_H
#define AG_H
#include<math.h>
#define TAMCROMO 11
#define TAMPOPULACAO 100

using namespace std;

struct resposta{
    int soma;
    int multiplicacao;
};

void iniciaPopulacao(int populacao[TAMPOPULACAO][TAMCROMO]){
    for(int i=0;i<TAMPOPULACAO;i++)
        for(int j=1;j<TAMCROMO;j++)
            populacao[i][j] = rand()%2;
}

void imprimeCromossomo(int *cromo){
    printf("\n");
    for(int i=1;i<TAMCROMO;i++)
        printf(" %d",cromo[i]);
}

void copiaCromossomo(int *dest,int *orig){
    for(int i=0;i<TAMCROMO;i++)
        dest[i]=orig[i];
}

void imprimePopulacao(int populacao[TAMPOPULACAO][TAMCROMO]){
    for(int i=0;i<TAMPOPULACAO;i++)
        imprimeCromossomo(populacao[i]);
}
struct resposta calculaAvaliacao(int *cromo){
    struct resposta r;
    r.soma = 0;
    r.multiplicacao =1;
    for(int i=1;i<TAMCROMO;i++){
        if(cromo[i]==0)
            r.soma = r.soma +i;
        else
            r.multiplicacao = r.multiplicacao * i;
    }
    r.soma = abs(36-r.soma);
    r.multiplicacao = abs(360-r.multiplicacao);
    return(r);
}


void fazAvaliacao(int populacao[TAMPOPULACAO][TAMCROMO],
                  struct resposta *avaliacao){
    for(int i=0;i<TAMPOPULACAO;i++)
        avaliacao[i] = calculaAvaliacao(populacao[i]);

}

void imprimeAvaliacao(struct resposta *avaliacao){
    for(int i=0;i<TAMPOPULACAO;i++)
        printf("\n(%d,%d)",avaliacao[i].soma,avaliacao[i].multiplicacao);
}

int torneio(struct resposta *avaliacao,int quantidade){
    int tipo = rand()%2;
    int pm = rand()%TAMPOPULACAO;
    if(tipo == 0){
        //soma
        for(int i=0;i<quantidade;i++){
            int s = rand()%TAMPOPULACAO;
            if(avaliacao[i].soma < avaliacao[pm].soma)
                pm = s;
        }
    }else{
        //mult
        for(int i=0;i<quantidade;i++){
            int s = rand()%TAMPOPULACAO;
            if(avaliacao[i].multiplicacao < avaliacao[pm].multiplicacao)
                pm = s;
    }
}
    return(pm);
}

void cruzamento(int *pai1, int *pai2,int *filho1,int *filho2){
    int pc = rand()%TAMCROMO;
    //printf("\n ponto de corte %d\n",pc);
    for(int i=0;i<pc;i++){
        filho1[i] = pai1[i];
        filho2[i] = pai2[i];
    }
    for(int i=pc;i<TAMCROMO;i++){
        filho1[i]=pai2[i];
        filho2[i]=pai1[i];
    }
}

void mutacao(int *pai,int *filho){
    for(int i=1;i<TAMCROMO;i++)
        filho[i]=pai[i];
    int pm = rand()%TAMCROMO;
   // printf("\npm %d",pm);
    if(filho[pm]==0)
        filho[pm]=1;
    else
        filho[pm]=0;
}

int domina(int i,int j,struct resposta *avaliacao){
    int r =0;
    if(avaliacao[i].multiplicacao != avaliacao[j].multiplicacao)
        if(avaliacao[i].multiplicacao < avaliacao[j].multiplicacao)
            r++;
        else
            r--;
    if(avaliacao[i].soma != avaliacao[j].soma)
        if(avaliacao[i].soma < avaliacao[j].soma)
            r++;
        else
            r--;

    if(r>0)
        return(1);
    else
        return(0);
 }

int fronteirasConstruidas(int *np){
    for(int i=0;i<2*TAMPOPULACAO;i++){
        if(np[i]!=-1)
            return(1);
    }
    return(0);

}

float distancia(struct resposta x1,struct resposta x2){
    //printf("\nCalculo  %d  %d  ----- %d  %d",x1.multiplicacao,x1.soma,x2.multiplicacao,x2.soma);
    float resposta = (x1.multiplicacao - x2.multiplicacao)* (x1.multiplicacao - x2.multiplicacao);
    resposta += (x1.soma-x2.soma)*(x1.soma-x2.soma);
    resposta = sqrt(resposta);
    return(resposta);

}


float calculoDasDistancias(int x,vector<int> fronteiras,struct resposta *avaliacao){
    float resposta = 0;
    for(int i=0;i<fronteiras.size();i++)
        resposta += distancia(avaliacao[x],avaliacao[fronteiras.at(i)]);

    return(resposta);
}


void ordenacaoSelecao(float *vet,vector <int> *fronteira,int tam){
    int i,j,auxv;
    float aux;

    for(i=0;i<tam-1;i++){

        int Pmaior = i;
        for(j=i+1;j<tam;j++)
            if(vet[j] > vet[Pmaior]){
                Pmaior = j;
            }
            aux = vet[Pmaior];
            vet[Pmaior] = vet[i];
            vet[i] = aux;

            auxv = fronteira->at(Pmaior);
            fronteira->at(Pmaior) = fronteira->at(i);
            fronteira->at(i) = auxv;

    }
}

bool pertence(int n,vector<int> resposta){
    for(int i=0;i<resposta.size();i++){
       if(n == resposta.at(i))
            return(true);
    }
    return(false);
}

vector<int> crowDistance(vector<int> *fronteira,struct resposta *avaliacao){
    vector<int> resposta;
    int menorErroSoma = fronteira->at(0);
    int menorErroMultiplicacao = fronteira->at(0);
    for(int i=1;i<fronteira->size();i++){
        if(avaliacao[fronteira->at(i)].soma < avaliacao[menorErroSoma].soma)
            menorErroSoma = fronteira->at(i);
        if(avaliacao[fronteira->at(i)].multiplicacao < avaliacao[menorErroSoma].multiplicacao)
            menorErroMultiplicacao = fronteira->at(i);
    }
     resposta.push_back(menorErroSoma);
     if(!pertence(menorErroMultiplicacao,resposta))
        resposta.push_back( menorErroMultiplicacao);
     float distancias[fronteira->size()];
     for(int i=0;i<fronteira->size();i++)
         distancias[i] = calculoDasDistancias(fronteira->at(i),*fronteira,avaliacao);

     ordenacaoSelecao(distancias,fronteira,fronteira->size());
     for(int i=0;i<fronteira->size();i++)
        if(!pertence(fronteira->at(i),resposta))
            resposta.push_back(fronteira->at(i));


 return(resposta);

}

void domina(vector<int> Sp[2*TAMPOPULACAO],struct resposta *avaliacao){
    int j=0;
    for(int i =0;i<2*TAMPOPULACAO;i++)
        if(i!=j)
            for(int j=0;j<2*TAMPOPULACAO;j++){
                if(domina(i,j,avaliacao))
                    Sp[i].push_back(j);
    }
}




void divisaoPorFronteiras(vector<vector<int> > *fronteiras,int *np,vector<int>Sp[2*TAMPOPULACAO]){
    while(fronteirasConstruidas(np)){
        vector<int> f;
        for(int w=0;w<2*TAMPOPULACAO;w++)
              if(np[w]==0){
                  f.push_back(w);
                  np[w]=-1;
              }
        fronteiras->push_back(f);
        for(int i=0;i<f.size();i++){
                for(int j=0;j< Sp[f.at(i)].size();j++)
                    np[Sp[f.at(i)].at(j)]--;
        }

  }

}

void quantosTeDomina(int *np,vector<int>Sp[2*TAMPOPULACAO]){
    int cont;
    for(int w=0;w<2*TAMPOPULACAO;w++){
        cont = 0;
        for(int j=0;j<2*TAMPOPULACAO;j++)
             for(int i=0;i<Sp[j].size();i++)
                     if (Sp[j].at(i)==w)
                         cont++;
        np[w]=cont;
    }
}


#endif // AG_H
