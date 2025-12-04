# pe_calculadora
Trabalho final da disciplina de Programação Estruturada na UFABC.

## Como compilar

```bash
make
```

## Como executar

```bash
./output/pe_calculadora
```

## Menu: Switch Case / Jump Table

O menu foi implementado com `switch-case` (geralmente compilado como jump table para casos densos) por apresentar complexidade de tempo O(1) por seleção e suporte nativo na linguagem, reduzindo overhead e riscos de erro. Alternativamente, uma hash table/`Map` permitiria seleção direta por `string` com custo amortizado O(1), melhorando a ergonomia de entrada. Entretanto, dado o número reduzido de opções e por não ser foco do projeto, priorizou-se a solução de menor custo de implementação e boa eficiência prática: `switch-case`.

## BigInt: Base 2^32 e vantagens de performance

Este projeto usa uma representação interna do BigInt em base 2^32 ("limbs" de 32 bits) armazenada em uma lista ligada. Esta escolha arquitetural oferece vantagens significativas de performance tanto em tempo quanto em memória:

**Nota sobre ordem de armazenamento:** Os dígitos são armazenados em ordem **little-endian** (LSB primeiro). O índice 0 contém o **dígito menos significativo** (LSB), e o último índice contém o **dígito mais significativo** (MSB). Esta ordem facilita operações aritméticas, pois começamos pelos dígitos menos significativos e propagamos o carry para a esquerda.

### Vantagens de Performance de Tempo

1. **Aritmética nativa eficiente**: Cada limb armazena 32 bits (uint32_t), permitindo que operações aritméticas utilizem instruções nativas da CPU. A soma de dois limbs pode ser feita em 64 bits (uint64_t) para capturar o carry, resultando em menos instruções de máquina e melhor aproveitamento dos registradores.

2. **Redução drástica de iterações**: Comparado a uma representação decimal (base 10), a base 2^32 reduz o número de dígitos em aproximadamente 9.6 vezes (log₁₀(2³²) ≈ 9.63). Por exemplo, um número de 100 dígitos decimais requer apenas ~11 limbs, reduzindo proporcionalmente o número de iterações em loops de operações aritméticas.

3. **Operações de carry otimizadas**: A propagação de carry em somas é feita diretamente em aritmética binária, evitando conversões e operações modulares custosas. O carry é calculado com um simples shift de bits (`carry = sum >> 32`).

4. **Comparações eficientes**: Comparações de magnitude podem ser feitas diretamente comparando limbs inteiros (uint32_t), sem necessidade de conversão ou processamento caractere por caractere.

5. **Acesso sequencial otimizado**: A lista ligada permite inserção e remoção de elementos no final (append) em O(1) amortizado, ideal para operações que constroem resultados progressivamente (como soma e multiplicação).

### Vantagens de Performance de Memória

1. **Alocação dinâmica sob demanda**: A lista ligada aloca memória apenas quando necessário, evitando desperdício de espaço para números pequenos. Números que cabem em um único limb (até 2³²-1) ocupam apenas o espaço de um nó, sem overhead de arrays pré-alocados.

2. **Eficiência de densidade**: Cada limb armazena 32 bits (4 bytes) de informação útil. A estrutura de nó (ListNode) tem tamanho total de 16 bytes devido ao alinhamento de memória: 4 bytes para o valor (uint32_t), 4 bytes de padding para alinhamento, e 8 bytes para o ponteiro (struct ListNode*). O overhead real é de 12 bytes por nó (ponteiro + padding), não contando os 4 bytes do valor útil. Para números grandes, este overhead é amortizado sobre muitos limbs.

3. **Sem fragmentação de arrays**: Diferente de arrays que podem precisar realocação e cópia completa quando expandem, a lista ligada cresce incrementalmente sem necessidade de mover dados existentes, reduzindo operações de memória custosas.

4. **Remoção eficiente de zeros à esquerda**: A estrutura permite remover zeros à esquerda (limbs mais significativos zerados) sem necessidade de realocar ou mover dados, apenas desalocando nós finais.

5. **Uso otimizado de cache**: Embora listas ligadas tenham acesso menos cache-friendly que arrays, a redução no número de elementos (limbs vs dígitos decimais) compensa parcialmente, e operações sequenciais ainda aproveitam prefetching do processador.

### Comparação Prática

Comparação de performance entre representação decimal com lista ligada versus array (string):

### Performance de Tempo

**Array (String):**
- Acesso por índice: **O(1)** - acesso direto
- Inserção no início/fim: **O(n)** - precisa mover elementos
- Inserção no meio: **O(n)** - precisa mover elementos subsequentes
- Realocação: **O(n)** - cópia completa de todos os dados
- Cache-friendly: **Sim** - dados contíguos, excelente localidade espacial

**Lista Ligada (1 dígito por nó):**
- Acesso por índice: **O(n)** - precisa percorrer a lista
- Inserção no início/fim: **O(1)** - apenas ajustar ponteiros
- Inserção no meio: **O(n)** - precisa encontrar posição
- Realocação: **Não necessária** - crescimento incremental
- Cache-friendly: **Não** - dados não contíguos, pior localidade espacial

**Lista Ligada (base 2^32 - implementação atual):**
- Acesso por índice: **O(n/9.6)** - ainda O(n), mas muito menos nós (11 vs 100 para 100 dígitos)
- Inserção no início/fim: **O(1)**
- Aritmética nativa: **Sim** - operações com uint32_t/uint64_t, carry via shift de bits
- Redução de iterações: **~89%** menos iterações que representação decimal

### Performance de Memória

Para um número de 100 dígitos decimais:

**Array (String):**
- Dados: 100 bytes (1 byte por dígito)
- Overhead: 1 byte (terminador `'\0'`)
- **Total: 101 bytes**

**Lista Ligada (1 dígito por nó):**
- Dados: 100 bytes (100 nós × 1 byte por dígito)
- Overhead: 100 nós × 16 bytes = 1600 bytes (12 bytes de overhead real por nó)
- **Total: ~1700 bytes** (~16.8x mais que array)

**Lista Ligada (base 2^32 - implementação atual):**
- Dados: ~44 bytes (11 limbs × 4 bytes)
- Overhead: 11 nós × 16 bytes = 176 bytes (12 bytes de overhead real por nó)
- **Total: ~220 bytes** (~2.2x mais que array, mas ~87% menos que decimal com lista ligada)

### Conclusão

**Array é melhor quando:** acesso aleatório frequente, tamanho conhecido, performance de cache crítica.

**Lista ligada é melhor quando:** tamanho varia frequentemente, inserções/remoções no início/fim comuns, acesso sequencial predominante.

**Base 2^32 com lista ligada (implementação atual):** Combina as vantagens de lista ligada (crescimento dinâmico, inserção O(1)) com eficiência de base binária: reduz drasticamente o número de nós (11 vs 100 para 100 dígitos decimais), cada nó armazena 32 bits úteis, operações aritméticas nativas da CPU, e o overhead é amortizado sobre muitos bits de dados. Resulta em apenas ~2.2x mais memória que array decimal, mas com ~89% menos iterações nas operações.
