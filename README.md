# Sistema de Ranking em Tempo Real — Torneio com Árvores AVL e Rubro-Negra
 
## O que faz
 
Este projeto implementa um sistema de ranking para torneios esportivos usando duas estruturas de dados de árvores balanceadas: **AVL** e **Rubro-Negra**. O sistema suporta inserção, remoção e busca eficientes de jogadores por pontuação, mantendo o top-10 sempre atualizado.
 
---
 
## Estruturas implementadas
 
### Árvore AVL
- Mantém o balanceamento pela diferença de altura entre subárvores (fator de balanceamento).
- Realiza **rotações simples** (esquerda/direita) e **rotações duplas** (esquerda-direita / direita-esquerda) quando necessário.
- Suporta inserção, remoção e busca por chave (pontuação).
### Árvore Rubro-Negra
- Mantém o balanceamento pelas propriedades de coloração dos nós (vermelho/preto).
- Garante as propriedades da árvore após cada inserção e remoção via recoloração e rotações.
- Suporta as mesmas operações da AVL.
---
 
## Comparação de desempenho
 
O projeto gera **10.000 inserções e remoções aleatórias** e mede:
 
| Métrica              | AVL | Rubro-Negra |
|----------------------|-----|-------------|
| Número de rotações   | —   | —           |
| Tempo de execução    | —   | —           |
 
> Os valores são preenchidos ao executar o benchmark.
 
---
