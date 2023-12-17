#ifndef ESTRUTURA_EXT_H
#define ESTRUTURA_EXT_H

#include <stdint.h>
#include <sys/stat.h>

// Defina EXT4_FILE_LEN como 255 bytes
#define EXT4_NAME_LEN 255
#define EXT4_N_BLOCKS 1000000
typedef uint32_t __le32;
typedef uint16_t __le16;
typedef uint64_t __le64;
typedef uint8_t __u8;

struct ext4_group_desc {
    int32_t bg_block_bitmap_lo;       // Endereço do bloco do mapa de bits de bloco (32 bits inferiores)
    int32_t bg_inode_bitmap_lo;       // Endereço do bloco do mapa de bits de inode (32 bits inferiores)
    int32_t bg_inode_table_lo;        // Endereço do bloco da tabela de inodes (32 bits inferiores)
    int16_t bg_free_blocks_count_lo;  // Contagem de blocos livres no grupo (16 bits inferiores)
    int16_t bg_free_inodes_count_lo;  // Contagem de inodes livres no grupo (16 bits inferiores)
    int16_t bg_used_dirs_count_lo;    // Contagem de diretórios usados no grupo (16 bits inferiores)
    int16_t bg_flags;                 // Flags (por exemplo, checksum do mapa de bits de bloco e inode, etc.)
    int32_t bg_exclude_bitmap_lo;    // Mapa de bits de exclusão de blocos (32 bits inferiores)
    int16_t bg_block_bitmap_csum_lo;  // Soma de verificação do mapa de bits de bloco (16 bits inferiores)
    int16_t bg_inode_bitmap_csum_lo;  // Soma de verificação do mapa de bits de inode (16 bits inferiores)
    int16_t bg_itable_unused_lo;      // Entradas não utilizadas na tabela de inodes (16 bits inferiores)
    int16_t bg_checksum;              // Soma de verificação do bloco de descritor de grupo
    int32_t bg_block_bitmap_hi;       // Endereço do bloco do mapa de bits de bloco (32 bits superiores)
    int32_t bg_inode_bitmap_hi;       // Endereço do bloco do mapa de bits de inode (32 bits superiores)
    int32_t bg_inode_table_hi;        // Endereço do bloco da tabela de inodes (32 bits superiores)
    int16_t bg_free_blocks_count_hi;  // Contagem de blocos livres no grupo (16 bits superiores)
    int16_t bg_free_inodes_count_hi;  // Contagem de inodes livres no grupo (16 bits superiores)
    int16_t bg_used_dirs_count_hi;    // Contagem de diretórios usados no grupo (16 bits superiores)
    int16_t bg_itable_unused_hi;      // Entradas não utilizadas na tabela de inodes (16 bits superiores)
    int32_t bg_exclude_bitmap_hi;     // Mapa de bits de exclusão de blocos (32 bits superiores)
    int16_t bg_block_bitmap_csum_hi;  // Soma de verificação do mapa de bits de bloco (16 bits superiores)
    int16_t bg_inode_bitmap_csum_hi;  // Soma de verificação do mapa de bits de inode (16 bits superiores)
    uint32_t bg_reserved;             // Reservado para uso futuro
};

struct dir_entry_2_t
{
    int32_t inode;            /* Número do inode */
    int16_t rec_len;          /* Comprimento do registro de entrada do diretório */
    uint8_t name_len;         /* Comprimento do nome */
    uint8_t file_type;        /* Tipo de arquivo */
    char name[EXT4_NAME_LEN]; /* Nome do arquivo */
};

struct inode_t
{
    int16_t i_mode;        /* Modo do arquivo */
    int16_t i_uid;         /* 16 bits mais baixos do ID do proprietário */
    int32_t i_size_lo;     /* Tamanho em bytes */
    int32_t i_atime;       /* Hora de acesso */
    int32_t i_ctime;       /* Hora de modificação do inode */
    int32_t i_mtime;       /* Hora de modificação */
    int32_t i_dtime;       /* Hora de exclusão */
    int16_t i_gid;         /* 16 bits mais baixos do ID do grupo */
    int16_t i_links_count; /* Contagem de links */
    int32_t i_blocks_lo;   /* Contagem de blocos */
    int32_t i_flags;       /* Flags do arquivo */
    union
    {
        struct
        {
            int32_t l_i_version;
        } linux1;
        struct
        {
            uint32_t h_i_translator;
        } hurd1;
        struct
        {
            uint32_t m_i_reserved1;
        } masix1;
    } osd1;                         /* OS dependente 1 */
    int32_t i_block[EXT4_N_BLOCKS]; /* Ponteiros para blocos */
    int32_t i_generation;           /* Versão do arquivo (para NFS) */
    int32_t i_file_acl_lo;          /* Lista de controle de acesso do arquivo */
    int32_t i_size_high;
    int32_t i_obso_faddr; /* Endereço do fragmento obsoleto */
    union
    {
        struct
        {
            int16_t l_i_blocks_high; /* eram l_i_reserved1 */
            int16_t l_i_file_acl_high;
            int16_t l_i_uid_high;    /* esses 2 campos */
            int16_t l_i_gid_high;    /* eram reserved2[0] */
            int16_t l_i_checksum_lo; /* crc32c(uuid+inum+inode) LE */
            int16_t l_i_reserved;
        } linux2;
        struct
        {
            int16_t h_i_reserved1; /* Número/tamanho do fragmento obsoleto removido no ext4 */
            uint16_t h_i_mode_high;
            uint16_t h_i_uid_high;
            uint16_t h_i_gid_high;
            uint32_t h_i_author;
        } hurd2;
        struct
        {
            int16_t h_i_reserved1; /* Número/tamanho do fragmento obsoleto removido no ext4 */
            int16_t m_i_file_acl_high;
            uint32_t m_i_reserved2[2];
        } masix2;
    } osd2; /* OS dependente 2 */
    int16_t i_extra_isize;
    int16_t i_checksum_hi;  /* crc32c(uuid+inum+inode) BE */
    int32_t i_ctime_extra;  /* Hora extra de modificação (nsec << 2 | epoch) */
    int32_t i_mtime_extra;  /* Hora extra de modificação (nsec << 2 | epoch) */
    int32_t i_atime_extra;  /* Hora extra de acesso (nsec << 2 | epoch) */
    int32_t i_crtime;       /* Hora de criação do arquivo */
    int32_t i_crtime_extra; /* Hora extra de criação do arquivo (nsec << 2 | epoch) */
    int32_t i_version_hi;   /* 32 bits mais altos para a versão de 64 bits */
    int32_t i_projid;       /* ID do projeto */
};

struct extent_t
{
    int32_t ee_block;    /* Primeiro bloco lógico coberto pela extensão */
    int16_t ee_len;      /* Número de blocos cobertos pela extensão */
    int16_t ee_start_hi; /* 16 bits mais altos do bloco físico */
    int32_t ee_start_lo; /* 32 bits mais baixos do bloco físico */
};

struct extent_header_t
{
    int16_t eh_magic;      /* Provavelmente suportará formatos diferentes */
    int16_t eh_entries;    /* Número de entradas válidas */
    int16_t eh_max;        /* Capacidade de armazenamento em entradas */
    int16_t eh_depth;      /* A árvore tem blocos subjacentes reais? */
    int32_t eh_generation; /* Geração da árvore */
};


struct ext4_super_block {
    __le32  s_inodes_count;          /* Contagem de inodes */
    __le32  s_blocks_count_lo;       /* Contagem de blocos (parte baixa) */
    __le32  s_r_blocks_count_lo;     /* Contagem de blocos reservados (parte baixa) */
    __le32  s_free_blocks_count_lo;  /* Contagem de blocos livres (parte baixa) */
    __le32  s_free_inodes_count;     /* Contagem de inodes livres */
    __le32  s_first_data_block;      /* Primeiro bloco de dados */
    __le32  s_log_block_size;        /* Tamanho do bloco (log) */
    __le32  s_log_cluster_size;      /* Tamanho do cluster de alocação (log) */
    __le32  s_blocks_per_group;      /* Número de blocos por grupo */
    __le32  s_clusters_per_group;    /* Número de clusters por grupo */
    __le32  s_inodes_per_group;      /* Número de inodes por grupo */
    __le32  s_mtime;                 /* Tempo de montagem */
    __le32  s_wtime;                 /* Tempo de escrita */
    __le16  s_mnt_count;             /* Contagem de montagem */
    __le16  s_max_mnt_count;         /* Contagem máxima de montagem */
    __le16  s_magic;                 /* Assinatura mágica */
    __le16  s_state;                 /* Estado do sistema de arquivos */
    __le16  s_errors;                /* Comportamento ao detectar erros */
    __le16  s_minor_rev_level;       /* Nível de revisão menor */
    __le32  s_lastcheck;             /* Tempo da última verificação */
    __le32  s_checkinterval;         /* Tempo máximo entre verificações */
    __le32  s_creator_os;            /* SO criador */
    __le32  s_rev_level;             /* Nível de revisão */
    __le16  s_def_resuid;            /* UID padrão para blocos reservados */
    __le16  s_def_resgid;            /* GID padrão para blocos reservados */
    __le32  s_first_ino;             /* Primeiro inode não reservado */
    __le16  s_inode_size;            /* Tamanho da estrutura de inode */
    __le16  s_block_group_nr;        /* Número do grupo de bloco deste Superbloco */
    __le32  s_feature_compat;        /* Conjunto de recursos compatíveis */
    __le32  s_feature_incompat;      /* Conjunto de recursos incompatíveis */
    __le32  s_feature_ro_compat;     /* Conjunto de recursos somente leitura compatíveis */
    __u8    s_uuid[16];              /* UUID de 128 bits para o volume */
    char    s_volume_name[16];       /* Nome do volume */
    char    s_last_mounted[64];      /* Diretório onde foi montado pela última vez */
    __le32  s_algorithm_usage_bitmap; /* Para compressão */
    __u8    s_prealloc_blocks;       /* Número de blocos para tentar pré-alocar */
    __u8    s_prealloc_dir_blocks;   /* Número a pré-alocar para diretórios */
    __le16  s_reserved_gdt_blocks;   /* Blocos por descritor de grupo para crescimento online */
    __u8    s_journal_uuid[16];      /* UUID do Superbloco do journal */
    __le32  s_journal_inum;          /* Número do inode do arquivo journal */
    __le32  s_journal_dev;           /* Número do dispositivo do arquivo journal */
    __le32  s_last_orphan;           /* Início da lista de inodes a serem excluídos */
    __le32  s_hash_seed[4];          /* Semente do hash HTREE */
    __u8    s_def_hash_version;      /* Versão de hash padrão a ser usada */
    __u8    s_jnl_backup_type;
    __le16  s_desc_size;             /* Tamanho do descritor de grupo */
    __le32  s_default_mount_opts;
    __le32  s_first_meta_bg;         /* Primeiro bloco metabloco do grupo */
    __le32  s_mkfs_time;             /* Quando o sistema de arquivos foi criado */
    __le32  s_jnl_blocks[17];        /* Backup do inode do journal */
    __le32  s_blocks_count_hi;       /* Contagem de blocos (parte alta) */
    __le32  s_r_blocks_count_hi;     /* Contagem de blocos reservados (parte alta) */
    __le32  s_free_blocks_count_hi;  /* Contagem de blocos livres (parte alta) */
    __le16  s_min_extra_isize;       /* Todos os inodes têm pelo menos # bytes */
    __le16  s_want_extra_isize;      /* Novos inodes devem reservar # bytes */
    __le32  s_flags;                 /* Diversas flags */
    __le16  s_raid_stride;           /* Passo de RAID */
    __le16  s_mmp_update_interval;   /* # segundos para esperar na verificação MMP */
    __le64  s_mmp_block;             /* Bloco para proteção de multi-montagem */
    __le32  s_raid_stripe_width;     /* Blocos em todos os discos de dados (N*passo) */
    __u8    s_log_groups_per_flex;   /* Tamanho do grupo FLEX_BG */
    __u8    s_checksum_type;         /* Algoritmo de checksum de metadados utilizado */
    __u8    s_encryption_level;      /* Nível de versionamento para criptografia */
    __u8    s_reserved_pad;          /* Preenchimento até o próximo múltiplo de 32 bits */
    __le64  s_kbytes_written;        /* Número de quilobytes escritos durante a vida útil */
    __le32  s_snapshot_inum;         /* Número do inode da snapshot ativa */
    __le32  s_snapshot_id;           /* ID sequencial da snapshot ativa */
    __le64  s_snapshot_r_blocks_count; /* Blocos reservados para uso futuro da snapshot ativa */
    __le32  s_snapshot_list;         /* Número do inode da cabeça da lista de snapshots no disco */
};


#endif // ESTRUTURA_EXT_H