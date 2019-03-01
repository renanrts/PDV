// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
// TODO: reference additional headers your program requires here
#include <iostream>
#include <Windows.h>
using namespace std;
// defines 
#define LIMPAR_TELA					system("cls")
#define PAUSA						system("pause")
#define BRASIL						setlocale(LC_ALL, "portuguese_brazil")
#define CAMINHO_CADASTRO			"..\\Cadastro.cad"
#define QTDE_PRODUTO				2000
#define TAM_DESCRICAO				40
//
// Opções do menu
//
#define CADASTRAR_PRODUTO			'C'
#define EXCLUIR_PRODUTO				'E'
#define VENDER_PRODUTO				'V'
#define MOSTRAR_PRODUTO				'M'
#define COMPRAR_PRODUTO				'A'
#define SAIR_DO_PROGRAMA			'S'
//
// Struct de data
//
typedef struct tagDATA
{
	int nDia, nMes, nAno;
} DATA;
//
// Struct do produto
//
typedef struct tagPRODUTO
{
	int nCodProduto;							// 1, 2, 3, ......QTDE_PRODUTO
	char cDescricao[TAM_DESCRICAO + 1],			// descrição do produto
		cAtivoInativo;							// 'A' - ativo 'I' - inativo
	DATA stDtUltCompra;							// data da ultima compra
	double dPrecoUnit,							// preço unitário
		dEstoque,								// quantidade em estoque
		dTotCompras,							// total de compras
		dTotVendas,								// total de vendas
		dVlrTotalEstoque;						// valor total do estoque
} PRODUTO;

bool SolicitarCodigoProduto(int *ptrCodigo, char *ptrAcao);
bool VerificarPosicao(int ptrCodigo, FILE *stArquivo, PRODUTO *stProduto);
bool GravarProduto(int ptrCodigo, FILE *stArquivo, PRODUTO *stProduto);

