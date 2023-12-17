#ifndef MANIPULACAO_ESTRUTURAS_H
#define MANIPULACAO_ESTRUTURAS_H

#include "estrutura_ext.h"  // Inclui o arquivo de cabeçalho que contém as estruturas

// Declaração da função para imprimir diretório

struct ext4_group_desc lerDescritorGrupo(FILE *arquivo_entrada, struct ext4_super_block *superbloco, int grupo_idx);

void informacao_imagem(FILE *arquivo_entrada, int block_size);
void list(FILE *arquivo_entrada, struct ext4_super_block *superbloco);
unsigned int calculaTamanhoBloco(struct ext4_super_block *superbloco);
void lerSuperbloco(FILE *arquivo_entrada, struct ext4_super_block *superbloco);
void imprimirSuperbloco(struct ext4_super_block *superbloco);
void imprimirCaracteristicas(uint64_t features);
unsigned long long contarBlocosTotal(struct ext4_super_block *superbloco);
void imprimirDescritoresGrupo(struct ext4_group_desc *descritores, int numGrupos);
struct inode_t encontrarInodePorNome(FILE *arquivo_entrada, const char *nome_arquivo, struct ext4_super_block *superbloco, struct ext4_group_desc *descritores, int numGrupos);
unsigned int calcularNumeroGrupos(struct ext4_super_block *superbloco);
void calcularDistribuicaoInodes(struct ext4_super_block *superbloco);

// Adicione outras declarações de funções conforme necessário

#endif // MANIPULACAO_ESTRUTURAS_H
