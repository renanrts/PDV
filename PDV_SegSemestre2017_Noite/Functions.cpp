#include "stdafx.h"

//Funcoes

//Funcao para solicitar código do produto
//
// Parametros
	//

bool SolicitarCodigoProduto(int *ptrCodigo, char *ptrAcao)
{
	LIMPAR_TELA;
	cout << "\n\t" << ptrAcao;
	do
	{
		cout << "\nDigite o código do produto de 1 a " << QTDE_PRODUTO << endl
			<< "Ou zero para cancelar a operação: ";
		cin >> *ptrCodigo;						// recebe o código
		if (*ptrCodigo == 0)					// encerrar execução?
		{
			cout << "\n\tCancelado pelo operador!" << endl;
			PAUSA;
			return false;
		}
	} while (*ptrCodigo < 1 || *ptrCodigo > QTDE_PRODUTO);
	return true;
}


//Funcao para verificar se o produto está ativo ou inativo
//
// Parametros
	//

bool VerificarPosicao(int ptrCodigo, FILE *stArquivo, PRODUTO *stProduto)
{
	if (fseek(stArquivo, (ptrCodigo - 1) * sizeof(PRODUTO), SEEK_SET) != 0) // erro de seek?
	{
		fclose(stArquivo);					// fechar o arquivo
		cout << "Erro de seek!" << endl;
		return false;								// volta ao sistema operacional
	}
	if (fread_s(stProduto, sizeof(PRODUTO), sizeof(PRODUTO), 1, stArquivo) == 0) // erro leitura?
	{
		fclose(stArquivo);					// fechar o arquivo
		cout << "Erro de leitura!" << endl;
		return false;							// continua no loop infinito
	}
	if (stProduto->nCodProduto != ptrCodigo)										//não é o mesmo produto
	{
		return false;
	}
	return true;
}

bool GravarProduto(int ptrCodigo, FILE *stArquivo, PRODUTO *stProduto)
{
	if (fseek(stArquivo, (ptrCodigo - 1) * sizeof(PRODUTO), SEEK_SET) != 0) // erro de seek?
	{
		fclose(stArquivo);					// fechar o arquivo
		cout << "Erro de seek!" << endl;
		PAUSA;
		return false;								// volta ao sistema operacional
	}
	if (fwrite(stProduto, sizeof(PRODUTO), 1, stArquivo) == 0)				//erro?
	{//erro irrecuperável
		fclose(stArquivo);										//fecha o cadastro de produtos
		cout << "Erro de gravação!" << endl;
		PAUSA;
		return false;													//volta ao sistema operacional
	}
	return true;
}