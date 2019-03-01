// PDV_SegSem2017_Noite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


void main(void)
{
	int i,										// indice e contador
		nQtde;
	char cOpcao,								// Opção do operador
		cNomeAluno[] = "Renan Torres de Souza", // para ser informado se nome
		cWork[300];								// edições do sprintf_s
	PRODUTO stProduto;							// para conter os dados de um produto
	FILE *fdCadastro;							// file descriptor do cadastro de produto
	SYSTEMTIME stTime;							// para buscar data e hora

	BRASIL;										// acentuação brasileira
	SetConsoleTitle("FATEC-MC - Sistema PDV");

		//Abrir o arquivo em modo r+b (leitura gravação binária) precisa existir
		//Se exitir - deixa como está

	if (fopen_s(&fdCadastro, CAMINHO_CADASTRO, "r+b") != 0)			// não existe arquivo?
	{
		//Se não exitir:
		//	1. Abrir o arquivo em modo w+b - primeira vez que o programa é executado
		if (fopen_s(&fdCadastro, CAMINHO_CADASTRO, "w+b") != 0)			// não existe arquivo?
		{
			//	2. Se não abrir, avisar ao operador e volta ao sistema operacional
			cout << "Erro de abertura do arquivo: " << CAMINHO_CADASTRO << endl;
			PAUSA;
			return;
		}
			//	3. Se abriu:
			//		3.1 - fazer um loop de gravação de QTDE_PRODUTOS sendo que
			//				código 1, 2, 3, ..... e cAtivoInativo = 'I'
			//			  Poderá zerar todas as doubles
		for (i = 0; i < QTDE_PRODUTO; i++)
		{
			stProduto.nCodProduto = i + 1;
			stProduto.cAtivoInativo = 'I';
			stProduto.dEstoque = stProduto.dTotCompras = stProduto.dPrecoUnit = stProduto.dTotVendas = stProduto.dVlrTotalEstoque = 0;
			if (fwrite(&stProduto, sizeof(PRODUTO), 1, fdCadastro) == 0) // erro de gravação?
			{
				fclose(fdCadastro);					// fechar o arquivo
				cout << "Erro de gravação" << endl;	// avisa o operador
				PAUSA;
				return;
			}
		}//for i
			//		3.2 - deixar como está 
	}
	//Loop infinito com opções do menu
	while (true)								// loop infinito
	{
		LIMPAR_TELA;
		GetLocalTime(&stTime);				// busca data e hora do sistema
											// Mostrar o menu de opções para o operador
		sprintf_s(cWork, sizeof(cWork),
			"\n\tFATEC-MC-PDV %02d/%02d/%d %02d:%02d\n\tOperador: %s",
			stTime.wDay, stTime.wMonth, stTime.wYear,
			stTime.wHour, stTime.wMinute, cNomeAluno);
		cout << cWork << endl;
		cout << CADASTRAR_PRODUTO << " - Cadastrar Produto" << endl;
		cout << EXCLUIR_PRODUTO << " - Excluir Produto" << endl;
		cout << VENDER_PRODUTO << " - Vender Produto" << endl;
		cout << MOSTRAR_PRODUTO << " - Mostrar Produto" << endl;
		cout << COMPRAR_PRODUTO << " - Comprar Produto" << endl;
		cout << SAIR_DO_PROGRAMA << " - Sair do programa" << endl;
		cout << "\n\tSelecione: ";
		cin >> cOpcao;					// opção escolhida pelo operador
		cOpcao = toupper(cOpcao);		// converte para upper case a opção escolhida
		switch (cOpcao)					// avaliar a opção que foi escolhida pelo operador
		{
		case CADASTRAR_PRODUTO:
			//	Se for cadastrar:
			//		1. Verificar se o produto a cadastrar já cadastrado

			if (SolicitarCodigoProduto(&stProduto.nCodProduto, "Cadastrar Produto"))
			{
				if (VerificarPosicao(stProduto.nCodProduto, fdCadastro, &stProduto))
				{
					if (stProduto.cAtivoInativo == 'A')
					{
						cout << "Produto já cadastrado!" << endl;
						//		2. Se estiver cadastrado exibir os seus dados, pausa para operador
						sprintf_s(cWork,
							"\n\t       Código: %d \n\t    Descrição: %s \nQuantidade em Estoque: %.2f \n       Preço Unitario: R$ %.2f "
							"\n\tTotal Vendido: R$ %.2f \n\tTotal Compras: R$ %.2f \n     Total em Estoque: R$ %.2f     \nData da última compra: %02d/%02d/%04d",
							stProduto.nCodProduto, stProduto.cDescricao, stProduto.dEstoque, stProduto.dPrecoUnit,
							stProduto.dTotVendas, stProduto.dTotCompras, stProduto.dVlrTotalEstoque, stProduto.stDtUltCompra.nDia,
							stProduto.stDtUltCompra.nMes, stProduto.stDtUltCompra.nAno);
						cout << cWork << endl;
						PAUSA;																	//pausa para o operador visualizar a mensagem
						break;																	//retorna ao menu
					}
					else
					{
						//		3. Se não estiver cadastrado pede sua descrição,
						//			qtde em estoque preço unitário e calcular o valor do estoque.
						//			zerar os demais valores.
						cin.ignore(1, EOF);															//ignora o primeiro enter					
						cout << "\n\tDigite os dados do produto!" << endl; 
						cout << "Descrição: ";														//solicita ao usuário inserir uma descrição
						cin.getline(stProduto.cDescricao, TAM_DESCRICAO + 1, '\n');
						stProduto.cAtivoInativo = 'A';
						do
						{
							cout << "Digite a quantidade em estoque: ";
							cin >> stProduto.dEstoque;
							if (stProduto.dEstoque <= 0)
								cout << "Quantidade de entrada deve ser maior que 0." << endl;
						} while (stProduto.dEstoque <= 0);
						do
						{
							cout << "Digite o valor unitário do produto: R$";
							cin >> stProduto.dPrecoUnit;
							if (stProduto.dPrecoUnit <= 0)
								cout << "Valor inválido." << endl;
						} while (stProduto.dPrecoUnit <= 0);
						stProduto.dVlrTotalEstoque = stProduto.dEstoque * stProduto.dPrecoUnit;
						stProduto.dTotCompras = stProduto.dVlrTotalEstoque;
						stProduto.dTotVendas = 0;
						stProduto.stDtUltCompra.nDia = stTime.wDay;
						stProduto.stDtUltCompra.nMes = stTime.wMonth;
						stProduto.stDtUltCompra.nAno = stTime.wYear;
						//		4. Regravar o produto
						if (!GravarProduto(stProduto.nCodProduto, fdCadastro, &stProduto))		//erro?
						{
							cout << "Erro de gravação!" << endl;
							PAUSA;																	//pausa para o operador visualizar a mensagem
							break;																	//retorna ao menu
						}
						cout << "\n\tProduto cadastrado com sucesso!" << endl;
						PAUSA;																	//pausa para o operador visualizar a mensagem

					}
				}
			}
				break;																	//retorna ao menu
		case EXCLUIR_PRODUTO:
			//	Se for excluir:
			//		1. Verificar se o produto está cadastrado e se não estiver
			//			avisar ao operador e voltar ao menu
			if (SolicitarCodigoProduto(&stProduto.nCodProduto, "Excluir Produto"))
			{
				if (VerificarPosicao(stProduto.nCodProduto, fdCadastro, &stProduto))
				{
					if (stProduto.cAtivoInativo == 'I')
					{
						cout << "\n\tProduto não cadastrado!" << endl;
						PAUSA;																	//pausa para o operador visualizar a mensagem
						break;																	//retorna ao menu

					}
					else
					{
						//		2. Se cadastrado exibir os seus dados e pedir a confirmação para
						//			a exclusão lógica do produto (trocar o A por I e regravar)
						sprintf_s(cWork,
							"\n\t       Código: %d \n\t    Descrição: %s \nQuantidade em Estoque: %.2f \n       Preço Unitario: R$ %.2f "
							"\n\tTotal Vendido: R$ %.2f \n\tTotal Compras: R$ %.2f \n     Total em Estoque: R$ %.2f     \nData da última compra: %02d/%02d/%04d",
							stProduto.nCodProduto, stProduto.cDescricao, stProduto.dEstoque, stProduto.dPrecoUnit,
							stProduto.dTotVendas, stProduto.dTotCompras, stProduto.dVlrTotalEstoque, stProduto.stDtUltCompra.nDia,
							stProduto.stDtUltCompra.nMes, stProduto.stDtUltCompra.nAno);
						cout << cWork;
						cout<<"\n\tDeseja realmente excluir o produto? S - SIM     N - NÃO" << endl;
						cout << "\n\tSelecione: ";
						cin >> cOpcao;
						if (cOpcao == 'N' || cOpcao == 'n')
						{
							cout << "Operação cancelada" << endl;
							PAUSA;
							break;
						}
						else if (cOpcao == 'S' || cOpcao == 's')
						{
							stProduto.cAtivoInativo = 'I';
							stProduto.dPrecoUnit = 0;
							stProduto.dTotCompras = 0;
							stProduto.dTotVendas = 0;
							stProduto.dVlrTotalEstoque = 0;
							stProduto.dEstoque = 0;
							if (!GravarProduto(stProduto.nCodProduto, fdCadastro, &stProduto))		//erro?
							{
								cout << "Erro de gravação!" << endl;
								PAUSA;																	//pausa para o operador visualizar a mensagem
								break;																	//retorna ao menu
							}
							cout << "Produto excluído com sucesso!" << endl;
						}
					}
				}
			}
			PAUSA;
			break;
		case VENDER_PRODUTO:
			//	Se for vender:
			//		1. Verificar se o produto está cadastrado e exibir ao operador os
			//			seus dados
			if (SolicitarCodigoProduto(&stProduto.nCodProduto, "Vender Produto"))
			{
				if (VerificarPosicao(stProduto.nCodProduto, fdCadastro, &stProduto) && stProduto.cAtivoInativo == 'A')
				{
					sprintf_s(cWork,
						"\n\t       Código: %d \n\t    Descrição: %s \nQuantidade em Estoque: %.2f \n       Preço Unitario: R$ %.2f "
						"\n\tTotal Vendido: R$ %.2f \n\tTotal Compras: R$ %.2f \n     Total em Estoque: R$ %.2f     \nData da última compra: %02d/%02d/%04d",
						stProduto.nCodProduto, stProduto.cDescricao, stProduto.dEstoque, stProduto.dPrecoUnit,
						stProduto.dTotVendas, stProduto.dTotCompras, stProduto.dVlrTotalEstoque, stProduto.stDtUltCompra.nDia,
						stProduto.stDtUltCompra.nMes, stProduto.stDtUltCompra.nAno);
					cout << cWork<<endl;
					if (stProduto.dEstoque == 0)											//se não houver estoque
					{
						cout << "Não há estoque deste produto" << endl;							//msg de ausencia de estoque
						PAUSA;																	//pausa para leitura da msg
						break;																	//retorna ao menu
					}
					//		2. Pedir a quantidade vendida
					do
					{
						cout << "\nDigite a quantidade a ser vendida: ";
						cin >> nQtde;
					} while (nQtde < 0);
					//		3. Se a quantidade vendida maior que no estoque avisar ao operador
					//			e voltar ao menu
					if (nQtde > stProduto.dEstoque)
					{
						cout << "\n\t Quantidade indisponível para venda. Total de produtos no estoque: " << stProduto.dEstoque << endl;
						PAUSA;
						break;
					}
					else
					{
						//		4. Calcular a quantidade * preço unitário e exibir ao operador
						cout << "Valor da venda: R$" << nQtde * stProduto.dPrecoUnit << endl;
						//		5. Atualizar o estoque - o vendido e calcular o novo valor do estoque
						//			somar na qtde vendida
						stProduto.dEstoque -= nQtde;
						stProduto.dTotVendas += (nQtde * stProduto.dPrecoUnit);
						stProduto.dVlrTotalEstoque -= nQtde * stProduto.dPrecoUnit;
						//		6. Regravar o produto
						if (!GravarProduto(stProduto.nCodProduto, fdCadastro, &stProduto))		//erro?
						{
							cout << "Erro de gravação!" << endl;
							PAUSA;																	//pausa para o operador visualizar a mensagem
							break;																	//retorna ao menu
						}
						cout << "\n\tProduto vendido com sucesso!" << endl;
						PAUSA;
						break;
					}
				}
				//		7. Se o produto estiver inativo avisar ao operador e voltar ao menu
				else
				{
					cout << "\n\tProduto não cadastrado!" << endl;
					PAUSA;
				}
			}
			break;
		case MOSTRAR_PRODUTO:
			if (SolicitarCodigoProduto(&stProduto.nCodProduto, "Mostrar Produto"))
			{
				if (VerificarPosicao(stProduto.nCodProduto, fdCadastro, &stProduto))
				{
					if (stProduto.cAtivoInativo == 'A')
					{
						sprintf_s(cWork,
							"\n\t       Código: %d \n\t    Descrição: %s \nQuantidade em Estoque: %.2f \n       Preço Unitario: R$ %.2f "
							"\n\tTotal Vendido: R$ %.2f \n\tTotal Compras: R$ %.2f \n     Total em Estoque: R$ %.2f     \nData da última compra: %02d/%02d/%04d",
							stProduto.nCodProduto, stProduto.cDescricao, stProduto.dEstoque, stProduto.dPrecoUnit,
							stProduto.dTotVendas, stProduto.dTotCompras, stProduto.dVlrTotalEstoque, stProduto.stDtUltCompra.nDia,
							stProduto.stDtUltCompra.nMes, stProduto.stDtUltCompra.nAno);
						cout << cWork <<endl;
						PAUSA;
						break;
					}
					else
					{
						cout << "\n\tProduto não cadastrado!" << endl;
						PAUSA;																	//pausa para o operador visualizar a mensagem
						break;																	//retorna ao menu

					}
				}
			}
			PAUSA;
			break;
		case COMPRAR_PRODUTO:
			//	Se for comprar:
			//		1. Verificar se o produto está cadastrado
			if (SolicitarCodigoProduto(&stProduto.nCodProduto, "Comprar Produto"))
			{
				if (VerificarPosicao(stProduto.nCodProduto, fdCadastro, &stProduto))
				{
					if (stProduto.cAtivoInativo == 'A')
					{
						//		2. Informar a quantidade comprada e o valor total.
						cout << "Produto selecionado: " << stProduto.cDescricao << endl << "Quantidade em estoque: " << stProduto.dEstoque << endl << "Valor total do estoque atual: R$" << stProduto.dVlrTotalEstoque << endl;
						do
						{
							cout << "Digite a quantidade comprada: ";
							cin >> nQtde;
							if (i == 0)
							{
								cout << "Operação cancelada retornando ao menu!" << endl;
								break;
							}
						} while (nQtde < 0);
						cout << "Valor Total da compra: R$" << nQtde*stProduto.dPrecoUnit << endl;
						//		3. Somar a qtde no estoque
						stProduto.dEstoque += nQtde;
						//		4. Somar no valor do estoque o valor total da compra.
						stProduto.dVlrTotalEstoque += nQtde*stProduto.dPrecoUnit;
						//		5. Calcular o novo preço unitário vlr total estoque/estoque.
						stProduto.dPrecoUnit = stProduto.dVlrTotalEstoque / stProduto.dEstoque;
						//			Somar na quantidade comprada e informar a data da última compra
						stProduto.dTotCompras += nQtde*stProduto.dPrecoUnit;
						cout << "\tData da compra, digite: ";
						cout << "\nDia: ";
						cin >> stProduto.stDtUltCompra.nDia;
						cout << "Mês: ";
						cin >> stProduto.stDtUltCompra.nMes;
						cout << "Ano: ";
						cin >> stProduto.stDtUltCompra.nAno;
						//		6. Regravar o produto
						if (!GravarProduto(stProduto.nCodProduto, fdCadastro, &stProduto))		//erro?
						{
							cout << "Erro de gravação!" << endl;
							PAUSA;																	//pausa para o operador visualizar a mensagem
							break;																	//retorna ao menu
						}
						cout << "\n\tInformações do produto atualizadas!" << endl;
					}
					else
					{
						cout << "\n\tProduto não cadastrado!" << endl;
					}
					PAUSA;
				}
			}
			break;
		case SAIR_DO_PROGRAMA:
			cout << "Vai sair realmente? (S ou N): ";
			cin >> cOpcao;								// recebe S ou N ou qualquer outra tecla
			if (cOpcao == 'S' || cOpcao == 's')			// sair realmente?
				fclose(fdCadastro);														//fechar o cadastro
				return;									// volta ao sistema operacional
			break;										// para sair do switch e decidir se vai sair realmente
		default:
			// Avisar que foi escolhida uma opção errada
			cout << "\nOpcao errada!!!";
			PAUSA;
		}// switch
	}//while
} // main

