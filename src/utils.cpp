/* 
utils.cpp
Implementacao das funcoes utilitarias
*/ 

#include "hpp/utils.hpp"
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

// Mersenne Twister - gerador aleatorio de qualidade
static mt19937 geradorRNG;
static bool rngInicializado = false;


//gerador de numeros aleatorios (RNG)
void inicializarRNG() {
    if (!rngInicializado) {
        geradorRNG.seed(time(nullptr));//usa o tempo atual como referencia
        rngInicializado = true;
    }
}

int rolarDado(int tipoDado) {
    if (!rngInicializado) {
        inicializarRNG();//garante que RNG esta inicializado
    }
    
    if (tipoDado < 4) {
        tipoDado = 4;//dado deve ter pelo menos 4 lados
    }
    
    uniform_int_distribution<int> distribuicao(1, tipoDado);//distribuicao uniforme entre 1 e tipoDado
    
    return distribuicao(geradorRNG);
}


//funcoes de iniciativa
void rolarIniciativaPersonagem(Personagem* p) {
    if (p == nullptr) {
        return;
    }
    
    int resultadoDado = rolarDado(p->dadoBase);//rola o dado do personagem
    
    p->iniciativaAtual = calcularIniciativa(resultadoDado, p->nivel);//calcula iniciativa final
}

int calcularIniciativa(int resultadoDado, int nivel) {
    int resultado = resultadoDado + nivel;
    
    if (resultado < 1) {
        resultado = 1;//iniciativa minima eh 1
    }
    
    return resultado;
}

void rolarIniciativaTodos(Lista* lista) {
    if (lista == nullptr || lista->estaVazia()) {
        return;
    }
    
    No* atual = lista->getInicio();
    
    while (atual != nullptr) {
        rolarIniciativaPersonagem(&(atual->dados));
        atual = atual->proximo;
    }
}

//valida e corrige nivel min e max
int validarNivel(int nivel) {
    if (nivel < 1) {
        return 1;
    }

    if (nivel > 20) {
        return 20;
    }
    return nivel;
}

//carrega personagens de um arquivo
int carregarArquivo(const string& nomeArquivo, Lista* lista) {
    if (lista == nullptr) {
        return -1;
    }
    
    //abre o arquivo
    ifstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cout << "Erro: Nao foi possivel abrir o arquivo '" << nomeArquivo << "'" << endl;
        return -1;
    }
    
    string linha;
    int contadorID = 1;
    int personagensCarregados = 0;
    int linhaAtual = 0;
    
    while (getline(arquivo, linha)) {
        linhaAtual++;
        
        if (linha.empty()) {
            continue;
        }
        
        if (linha[0] == '#') {
            continue;
        }
        
        //remove espacos em branco no inicio e fim
        size_t inicio = linha.find_first_not_of(" \t");
        size_t fim = linha.find_last_not_of(" \t\r\n");
        
        if (inicio == string::npos) {
            continue;
        }
        
        linha = linha.substr(inicio, fim - inicio + 1);
        
        //parse da linha: nome;nivel;tipo
        string campos[3];
        int campoAtual = 0;
        string temp = "";
        
        for (size_t i = 0; i <= linha.length(); i++) {
            if (i == linha.length() || linha[i] == ';') {
                if (campoAtual < 3) {
                    campos[campoAtual] = temp;
                    campoAtual++;
                }
                temp = "";
            } else {
                temp += linha[i];
            }
        }
        
        if (campoAtual < 3) {
            cout << "Aviso: Linha " << linhaAtual << " ignorada (formato invalido)" << endl;
            continue;
        }
        
        //cria o personagem
        Personagem p;
        p.id = contadorID++;
        p.nome = campos[0];
        
        //converte e valida nivel
        try {
            p.nivel = validarNivel(stoi(campos[1]));
        } catch (...) {
            cout << "Aviso: Linha " << linhaAtual << " - nivel invalido, usando 1" << endl;
            p.nivel = 1;
        }
        
        //define tipo (Player ou NPC)
        p.isPlayer = (campos[3][0] == 'P' || campos[3][0] == 'p');
        
        //valores padrao
        p.dadoBase;
        p.iniciativaAtual = 0;
        
        lista->inFim(p);
        personagensCarregados++;
    }
    
    arquivo.close();
    
    return personagensCarregados;
}