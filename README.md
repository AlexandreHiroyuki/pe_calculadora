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

Este projeto usa uma representação interna do BigInt em base 2^32 ("limbs" de 32 bits em ordem little-endian). Isso traz benefícios importantes:

- Desempenho de CPU: operações de soma e propagação de carry usam aritmética nativa de 64 bits (acúmulo em 64 bits para dois limbs de 32 bits), reduzindo instruções e branches.
- Menos iterações: para o mesmo valor, há muito menos limbs do que dígitos decimais (cada limb representa 32 bits), diminuindo o custo de laços nas operações.
- Melhor uso de cache: estruturas mais compactas significam menos leituras/escritas de memória por operação.
- Shifts e comparações rápidos: deslocamentos de bits e comparações de magnitude operam diretamente por limbs, evitando conversões.

Conversão para/da forma decimal continua disponível:
- Parsing decimal: feito por multiplicações por 10 e somas do dígito (pequenos inteiros) diretamente nos limbs 2^32.
- Impressão decimal: feita por divisões repetidas por 10^9, gerando blocos decimais eficientes para formatação.
