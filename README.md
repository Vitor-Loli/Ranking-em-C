# Sistema de Ranking em Tempo Real — AVL vs Rubro-Negra

Trabalho prático comparando duas estruturas de árvores balanceadas para um sistema de ranking de torneio.

## Estrutura do projeto

```
├── player.h          # Struct Jogador e constante TAM_NOME
├── avl.h / avl.c     # Árvore AVL
├── rbtree.h / rbtree.c  # Árvore Rubro-Negra
├── benchmark.h / benchmark.c  # Benchmark comparativo (10.000 operações)
└── main.c            # Demonstração + benchmark + modo interativo
```

## Funcionalidades

- Inserção, remoção e busca por pontuação em ambas as árvores
- Exibição do ranking top-10 atualizado
- Contagem de rotações por estrutura
- Benchmark com 10.000 inserções e 5.000 remoções aleatórias
- Modo interativo para adicionar/remover jogadores manualmente

## Como compilar

**CLion:** abra o projeto e clique em Run.

**Terminal (gcc):**
```bash
gcc -o ranking main.c avl.c rbtree.c benchmark.c -Wall
./ranking
```

## Resultado esperado do benchmark

| Métrica | AVL | Rubro-Negra |
|---|---|---|
| Rotações totais | ~9000 | ~7900 |
| Tempo (s) | ~0.009 | ~0.005 |

A Rubro-Negra realiza menos rotações e é mais rápida em cargas mistas de inserção e remoção, sendo a estrutura indicada para este tipo de sistema.

## Disciplina

Estruturas de Dados — Sistemas de Informação
