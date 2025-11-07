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

## Como testar

```bash
make test
```

# Menu: Switch Case / Jump Table

O menu foi implementado com `switch-case` (geralmente compilado como jump table para casos densos) por apresentar complexidade de tempo O(1) por seleção e suporte nativo na linguagem, reduzindo overhead e riscos de erro. Alternativamente, uma hash table/`Map` permitiria seleção direta por `string` com custo amortizado O(1), melhorando a ergonomia de entrada. Entretanto, dado o número reduzido de opções e por não ser foco do projeto, priorizou-se a solução de menor custo de implementação e boa eficiência prática: `switch-case`.

## BigInt: Base 2^32 e vantagens de performance

Este projeto usa uma representação interna do BigInt em base 2^32 ("limbs" de 32 bits) armazenada em uma lista ligada. Esta escolha arquitetural oferece vantagens significativas de performance tanto em tempo quanto em memória:

### Vantagens de Performance de Tempo

1. **Aritmética nativa eficiente**: Cada limb armazena 32 bits (uint32_t), permitindo que operações aritméticas utilizem instruções nativas da CPU. A soma de dois limbs pode ser feita em 64 bits (uint64_t) para capturar o carry, resultando em menos instruções de máquina e melhor aproveitamento dos registradores.

2. **Redução drástica de iterações**: Comparado a uma representação decimal (base 10), a base 2^32 reduz o número de dígitos em aproximadamente 9.6 vezes (log₁₀(2³²) ≈ 9.63). Por exemplo, um número de 100 dígitos decimais requer apenas ~11 limbs, reduzindo proporcionalmente o número de iterações em loops de operações aritméticas.

3. **Operações de carry otimizadas**: A propagação de carry em somas é feita diretamente em aritmética binária, evitando conversões e operações modulares custosas. O carry é calculado com um simples shift de bits (`carry = sum >> 32`).

4. **Comparações eficientes**: Comparações de magnitude podem ser feitas diretamente comparando limbs inteiros (uint32_t), sem necessidade de conversão ou processamento caractere por caractere.

5. **Acesso sequencial otimizado**: A lista ligada permite inserção e remoção de elementos no final (append) em O(1) amortizado, ideal para operações que constroem resultados progressivamente (como soma e multiplicação).

### Vantagens de Performance de Memória

1. **Alocação dinâmica sob demanda**: A lista ligada aloca memória apenas quando necessário, evitando desperdício de espaço para números pequenos. Números que cabem em um único limb (até 2³²-1) ocupam apenas o espaço de um nó, sem overhead de arrays pré-alocados.

2. **Eficiência de densidade**: Cada limb armazena 32 bits de informação útil, enquanto a estrutura de nó (ListNode) adiciona apenas overhead de ponteiros (8 bytes em sistemas 64-bit). Para números grandes, este overhead é amortizado sobre muitos limbs.

3. **Sem fragmentação de arrays**: Diferente de arrays que podem precisar realocação e cópia completa quando expandem, a lista ligada cresce incrementalmente sem necessidade de mover dados existentes, reduzindo operações de memória custosas.

4. **Remoção eficiente de zeros à esquerda**: A estrutura permite remover zeros à esquerda (limbs mais significativos zerados) sem necessidade de realocar ou mover dados, apenas desalocando nós finais.

5. **Uso otimizado de cache**: Embora listas ligadas tenham acesso menos cache-friendly que arrays, a redução no número de elementos (limbs vs dígitos decimais) compensa parcialmente, e operações sequenciais ainda aproveitam prefetching do processador.

### Comparação Prática

Para um número de 100 dígitos decimais:
- **Representação decimal**: ~100 bytes (1 byte por dígito) + overhead de string
- **Representação base 2^32**: ~44 bytes (11 limbs × 4 bytes) + overhead de nós (~16 bytes por nó) ≈ ~180 bytes total
- **Redução de iterações**: 100 iterações → 11 iterações (redução de ~89%)

A escolha da lista ligada é particularmente vantajosa para operações que constroem resultados incrementais e para números de tamanho variável, onde a flexibilidade supera o pequeno custo adicional de overhead de ponteiros.
