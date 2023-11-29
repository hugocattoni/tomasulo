# Tomasulo

## Visão Geral

Simulador do algoritmo de Tomasulo em C++, desenvolvido para o trabalho final de Arquitetura de Computadores 3 | PUC Minas
Prof. Matheus Alcântara Souza
Acesso do código no Replit [aqui](https://replit.com/@HugoCattoni/tomasulocfinal#main.cpp).

## Definição

O Algoritmo de Tomasulo é um método de execução de instruções em pipelines de processadores superescalares, permitindo a execução simultânea de múltiplas instruções independentes. Ele utiliza estações de reserva para gerenciar e coordenar a execução das instruções.

O algoritmo de Tomasulo usa algumas importantes variáveis para coordenar o fluxo de instruções através do pipeline. São elas:

- **Op:** Representa a operação a ser executada;
- **Vj e Vk:** São os valores dos operandos;
- **Qj e Qk:** Indicam se os operandos estão prontos ou ocupados por alguma outra instrução;
- **A:** Registrador que armazena o resultado da última instrução executada.

## Operações permitidas

1. ADD
2. SUB
3. MUL
4. DIV
5. LW
6. SW

## Instruções

O algoritmo espera um arquivo .txt contendo as instruções como entrada, separadas por quebra de linha e espaço entre a operação e o restante da instrução, e espaçamento após vírgulas.

**Exemplos de formatação correta:**

- **LW F6, 30(R2)**
- **LW F2, 4(R3)**
- **MUL F0, F2, F4**
- **SUB F8, F6, F2**
- **DIV F10, F0, F6**
- **ADD F6, F8, F2**

## Detalhes de implementação

### Método _createInstructionQueue_

O método _createInstructionQueue_ serve unicamente para ler o arquivo .txt de entrada e gerar a fila de instruções (do tipo **<Instruction>**) informadas pelo usuário. Por isso, é de extrema importância a formatação correta no arquivo de entrada para a execução do algoritmo.

### Método _issue_

O método _issue_ recebe **inst** (do tipo **<Instruction>**) como parâmetro e é responsável por encontrar uma estação de reserva livre para a instrução, verificar e lidar com possíveis dependências de registradores e marcar as estações de reserva e registradores apropriados para a instrução atual. Isso garante que as instruções sejam executadas de forma ordenada, sem violar as dependências de dados.

- A partir do tipo de operação, o código procura uma estação de reserva disponível/desocupada para a instrução;
- As posições entre 0 e 15 são reservadas para os tipos **ADD** e **SUB**;
- As posições entre 16 e 31 são reservadas para os tipos **MUL** e **DIV**;
- As posições entre 32 e 47 são reservadas para os tipos **LW** e **SW**;
- As dependências são garantidas a partir das condições das instruções.

### Método _simulateExec_

O método _simulateExec_ retorna o resultado da operação, dependendo do tipo de instrução.

### Método _write_

O método _write_ simula o estágio de escrita/execução das instruções, atualizando os valores dos registradores e das estações de reserva, liberando recursos e preparando o ambiente para a próxima instrução no pipeline.

- Verifica as unidades funcionais que estão ocupadas e não estão executando;
- Atualiza os valores dos registradores e das estações de reserva de acordo com os resultados da execução;
- Libera as estações de reserva e unidades funcionais após a conclusão da execução.

### Método _executeInstructions_

O método _executeInstructions_ faz o controle do progresso das instruções em cada unidade funcional, verificando condições para emissão de novas instruções e atualizando os índices para acompanhar as estações de reserva disponíveis. Garante a execução ordenada das instruções.

- Atualiza o contador de execução para cada instrução pendente, indicando a redução do tempo de execução restante;
- Avança o progresso das instruções em cada tipo de unidade funcional;
- Verifica se as instruções nas estações de reserva estão prontas para avançar para a execução;
- Aloca as instruções nas unidades funcionais disponíveis, se as condições forem atendidas;
- Atualiza os índices para verificar as próximas estações de reserva disponíveis para emissão de instruções.

### Método _writeOutput_

O método _writeOutput_ gera a saída em um arquivo .txt, escrevendo o estado atual das estações de reserva e registradores.

## Renomeação

No código, a renomeação permite que as instruções sejam executadas de forma especulativa e fora de ordem, desde que suas dependências estejam resolvidas.
A partir da entrada:

- LW F6, 32(R2)
- LW F2, 44(R3)
- ADD F2, F6, F6

As instruções de carregamento, que originalmente estavam antes da instrução de adição no código, são concluídas após a execução da instrução de adição.
