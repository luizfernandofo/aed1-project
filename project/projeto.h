#ifndef PROJETO_H
#define PROJETO_H

#ifdef _WIN32 // Includes both 32 bit and 64 bit
    #define CLS "cls"
    	    
#elif __linux__
    #define CLS "clear"

#endif

typedef struct{
	char nome[200];
	int codigo;
	char cargo[20];
	float salario;
}funcionarios;


//Declaração das funções
void menu();

void abrirArquivo();

void cadastrar();

void listar();

void consultar();

void alteraSalario();

void alterarCargo();

void demitir();

#endif
