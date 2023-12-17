#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include "manipulacao_estruturas.h"
#include "estrutura_ext.h"


void lerSuperbloco(FILE *arquivo_entrada, struct ext4_super_block *superbloco)
{
    fseek(arquivo_entrada, 1024, SEEK_SET);
    fread(superbloco, sizeof(struct ext4_super_block), 1, arquivo_entrada);

    unsigned int tamanhoBloco = 1024 << superbloco->s_log_block_size;
    unsigned int posicaoDescritores = tamanhoBloco * (superbloco->s_first_data_block + 1);
    fseek(arquivo_entrada, posicaoDescritores, SEEK_SET);
}

struct ext4_group_desc lerDescritorGrupo(FILE *arquivo_entrada, struct ext4_super_block *superbloco, int grupo_idx)
{
    unsigned int tamanhoBloco = 1024 << superbloco->s_log_block_size;
    off_t posicaoDescritores = tamanhoBloco * (superbloco->s_first_data_block + 1);
    off_t posicaoDescritor = posicaoDescritores + grupo_idx * sizeof(struct ext4_group_desc);
    fseek(arquivo_entrada, posicaoDescritor, SEEK_SET);

    struct ext4_group_desc grupo_desc;
    fread(&grupo_desc, sizeof(struct ext4_group_desc), 1, arquivo_entrada);

    return grupo_desc;
}

// Função para imprimir as informações do superbloco
void imprimirSuperbloco(struct ext4_super_block *superbloco)
{
    printf("Nome do volume do sistema de arquivos:   %.*s\n", (int)sizeof(superbloco->s_volume_name), superbloco->s_volume_name);
    printf("Última montagem em:          %.*s\n", (int)sizeof(superbloco->s_last_mounted), superbloco->s_last_mounted);
    printf("UUID do sistema de arquivos:          ");
    for (size_t i = 0; i < sizeof(superbloco->s_uuid); i++)
    {
        printf("%02x", superbloco->s_uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9)
        {
            printf("-");
        }
    }
    printf("\n");
    printf("Número mágico do sistema de arquivos:  0x%04x\n", superbloco->s_magic);
    printf("Flags do sistema de arquivos signed_directory_hash\n");
    printf("Opção padrão de montagem: user_xattr acl\n");
    printf("Comportamento de erros: Continue\n");
    printf("Sistema operacional dos arquivos: Linux\n");
    printf("Contagem de inodes: %u\n", superbloco->s_inodes_count);

    unsigned long long contagemTotal = contarBlocosTotal(superbloco);

    printf("Contagem de blocos: %llu\n", contagemTotal);
    printf("Contagem de blocos reservados: %u\n", superbloco->s_r_blocks_count_lo);
    printf("Blocos Livres: %u\n", superbloco->s_free_blocks_count_lo);
    printf("Inodes livres: %u\n", superbloco->s_free_inodes_count);

    printf("Primeiro bloco:           %u\n", superbloco->s_first_data_block);
    printf("Tamanho do bloco:         %u\n", 1024 << superbloco->s_log_block_size);
    printf("Tamanho do fragmento:      %u\n", 1024 << superbloco->s_log_cluster_size);
    printf("Tamanho do descritor de grupo: %u\n", superbloco->s_desc_size);

    printf("Blocos GDT reservados:    %u\n", superbloco->s_reserved_gdt_blocks);
    printf("Blocos por grupo:         %u\n", superbloco->s_blocks_per_group);
    printf("Fragments por grupo:      %u\n", superbloco->s_clusters_per_group);
    calcularDistribuicaoInodes(superbloco);

    printf("Tamanho do grupo FLEX_BG:  %u\n", 1 << (superbloco->s_log_groups_per_flex));

    time_t mkfs_time = (time_t)superbloco->s_mkfs_time;
    printf("Sistema de arquivos criado:        %s", ctime(&mkfs_time));

    time_t lastcheck_time = (time_t)superbloco->s_lastcheck;
    printf("Última vez checado:          %s", ctime(&lastcheck_time));

    time_t wtime = (time_t)superbloco->s_wtime;
    printf("Última hora de gravação:     %s", ctime(&wtime));

    printf("Contagem de montagem:              %hu\n", superbloco->s_mnt_count);
    printf("Contagem máxima de montagem:      %hd\n", superbloco->s_max_mnt_count);
    printf("Intervalo de verificação:         %u (<nenhum>)\n", superbloco->s_checkinterval);
    printf("Gravações ao longo da vida:       %lu MB\n", (superbloco->s_kbytes_written / 1024) + 1);
    printf("UID de blocos reservados:         %hu (usuário root)\n", superbloco->s_def_resuid);

    printf("Primeiro inode:           %u\n", superbloco->s_first_ino);
    printf("Tamanho do inode:         %u\n", superbloco->s_inode_size);
    printf("Tamanho extra de inode necessário:  %u\n", superbloco->s_want_extra_isize);
    printf("Tamanho extra de inode desejado:  %u\n", superbloco->s_want_extra_isize);
    printf("Inode do journal:         %u\n", superbloco->s_journal_inum);

    printf("Backup do journal:        %s\n", superbloco->s_jnl_backup_type == 0 ? "inode" : "blocos");
    printf("Tipo de checksum:         crc32c\n");
}

unsigned int calculaTamanhoBloco(struct ext4_super_block *superbloco)
{
    unsigned int tamanhoBloco = 1024 << superbloco->s_log_block_size;
    return tamanhoBloco;
}

void calcularDistribuicaoInodes(struct ext4_super_block *superbloco)
{
    unsigned int tamanhoBloco = 1024 << superbloco->s_log_block_size;
    unsigned int tamanhoInode = superbloco->s_inode_size;

    unsigned int inodesPorBloco = tamanhoBloco / tamanhoInode;
    unsigned int blocosDeInodePorGrupo = superbloco->s_inodes_per_group / inodesPorBloco;

    printf("Inodes por bloco: %u\n", inodesPorBloco);
    printf("Blocos de inode por grupo: %u\n", blocosDeInodePorGrupo);
}

unsigned long long contarBlocosTotal(struct ext4_super_block *superbloco)
{
    unsigned long long contagemTotal = ((unsigned long long)superbloco->s_blocks_count_hi << 32) | superbloco->s_blocks_count_lo;
    return contagemTotal;
}

unsigned int calcularNumeroGrupos(struct ext4_super_block *superbloco)
{
    unsigned int numBlocosGrupo = superbloco->s_blocks_per_group;
    unsigned long long totalBlocos = contarBlocosTotal(superbloco);
    unsigned int numGrupos = (totalBlocos + numBlocosGrupo - 1) / numBlocosGrupo;

    return numGrupos;
}

void imprimirInformacoesGrupo(FILE *arquivo_entrada, struct ext4_super_block *superbloco)
{
    unsigned int numGrupos = calcularNumeroGrupos(superbloco);
    // Imprimir a descrição dos grupos
    for (unsigned int grupo_idx = 0; grupo_idx < numGrupos; grupo_idx++)
    {
        struct ext4_group_desc grupo_desc = lerDescritorGrupo(arquivo_entrada, superbloco, grupo_idx);

        printf(" Group %2u: block bitmap at %d, inode bitmap at %d, inode table at %d\n",
               grupo_idx, grupo_desc.bg_block_bitmap_lo, grupo_desc.bg_inode_bitmap_lo, grupo_desc.bg_inode_table_lo);
        printf("           %d free blocks, %d free inodes, %d used directories, %d unused inodes\n",
               grupo_desc.bg_free_blocks_count_lo, grupo_desc.bg_free_inodes_count_lo,
               grupo_desc.bg_used_dirs_count_lo, superbloco->s_inodes_per_group - grupo_desc.bg_free_inodes_count_lo);
        printf("           [Checksum 0x%x]\n", grupo_desc.bg_checksum);
    }
}

// Operação (1) - Exibir informações da imagem e do sistema de arquivos
void operacaoInfo(FILE *arquivo_entrada, struct ext4_super_block *superbloco)
{
    imprimirSuperbloco(superbloco);
    imprimirInformacoesGrupo(arquivo_entrada, superbloco);
}
void operacaoCat(FILE *arquivo_entrada, struct ext4_super_block *superbloco, const char *caminhoArquivo)
{
    // Lógica para exibir o conteúdo do arquivo
    // Você precisará usar informações do superbloco para encontrar o inode do arquivo
    // e, em seguida, ler os dados do inode para obter o conteúdo do arquivo.
}

int main()
{
    FILE *arquivo_entrada;
    arquivo_entrada = fopen("myext4image1k.img", "rb");

    if (arquivo_entrada == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Ler o superbloco
    struct ext4_super_block superbloco;
    lerSuperbloco(arquivo_entrada, &superbloco);
    // struct ext4_group_desc grupo_desc;

    operacaoInfo(arquivo_entrada, &superbloco);

    fclose(arquivo_entrada);
    return 0;
}