<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h1 align="center">🦸‍♂️ The Boys - Simulador de Eventos Discretos</h1>

  <p align="center">
    Simulador de Eventos Discretos (SED) desenvolvido em C para a disciplina de Programação 2 (CI1001) da Universidade Federal do Paraná (UFPR).
    <br />
    <br />
    <a href="https://github.com/GiuTP/The-Boys/issues/new?labels=bug&template=bug-report---.md">Reportar Bug</a>
    &middot;
    <a href="https://github.com/GiuTP/The-Boys/issues/new?labels=enhancement&template=feature-request---.md">Sugerir Melhoria</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Sumário</summary>
  <ol>
    <li>
      <a href="#-sobre-o-projeto">Sobre o Projeto</a>
      <ul>
        <li><a href="#-conceitos-de-simulação-a-eventos-discretos-sed">Conceitos de Simulação a Eventos Discretos (SED)</a></li>
        <li><a href="#-entidades-do-mundo">Entidades do Mundo</a></li>
        <li><a href="#-dinâmica-dos-eventos">Dinâmica dos Eventos</a></li>
        <li><a href="#-estruturas-de-dados-tads">Estruturas de Dados (TADs)</a></li>
        <li><a href="#-principais-desafios-e-aprendizados">Principais Desafios e Aprendizados</a></li>
        <li><a href="#%EF%B8%8F-tecnologias-utilizadas">Tecnologias Utilizadas</a></li>
      </ul>
    </li>
    <li>
      <a href="#-estrutura-do-repositório">Estrutura do Repositório</a>
    </li>
    <li>
      <a href="#-como-começar">Como Começar</a>
      <ul>
        <li><a href="#-pré-requisitos">Pré-requisitos</a></li>
        <li><a href="#-compilação">Compilação</a></li>
      </ul>
    </li>
    <li><a href="#-uso-e-execução">Uso e Execução</a></li>
    <li><a href="#-roteiro-e-melhorias-futuras">Roteiro e Melhorias Futuras</a></li>
    <li><a href="#-contribuição">Contribuição</a></li>
    <li><a href="#-licença">Licença</a></li>
    <li><a href="#-contato">Contato</a></li>
    <li><a href="#%EF%B8%8F-agradecimentos">Agradecimentos</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## 📖 Sobre o Projeto

Este projeto foi desenvolvido como trabalho prático da disciplina **Programação 2 (CI1001)** no curso de Ciência da Computação da **Universidade Federal do Paraná (UFPR)** durante o segundo semestre de 2024, sob orientação do **Prof. Carlos Maziero**.

Inspirado no universo satírico da série *The Boys*, o programa simula a dinâmica de um mundo composto por heróis, bases de apoio e missões de emergência espalhadas por um plano bidimensional. A simulação transcorre ao longo de um ano completo de tempo simulado (525.600 minutos), registrando a movimentação dos heróis entre bases, filas de espera gerenciadas por porteiros, formação de equipes por compatibilidade de habilidades e a resolução ou adiamento de missões de resgate.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

### 🛠 Construído com

* [![C][C-badge]][C-url]
* [![Linux][Linux-badge]][Linux-url]
* [![Valgrind][Valgrind-badge]][Valgrind-url]

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>


### ⏱️ Conceitos de Simulação a Eventos Discretos (SED)

Diferente de simulações orientadas a passos fixos de tempo (*time-stepped*), uma Simulação a Eventos Discretos modela a evolução de um sistema apenas nos instantes exatos em que ocorrem alterações de estado. Entre dois eventos sucessivos, nada muda no mundo, permitindo que o relógio global dê saltos diretos entre eventos futuros.

O núcleo do simulador fundamenta-se em:
* **Relógio Global:** Contador inteiro iniciado em zero que avança monotonicamente até o instante final da simulação (minuto 525.600).
* **Lista de Eventos Futuros (LEF):** Fila de prioridades onde a chave de ordenação é o instante temporal da ocorrência do evento. O laço principal retira sucessivamente o evento prioritário (mais antigo no tempo), atualiza o relógio global para o tempo do evento, despacha o tratamento para a função correspondente e insere na LEF eventuais novos eventos decorrentes daquela ação.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

### 👥 Entidades do Mundo

O estado estático e dinâmico da simulação é representado por quatro entidades principais:

* **Herói:** Possui identificador único, nível de paciência (influencia a tolerância a filas), velocidade de deslocamento (metros por minuto), nível de experiência acumulada (missões concluídas), base atual e um conjunto contendo de 1 a 3 habilidades únicas.
* **Base:** Ponto de encontro com capacidade máxima definida, fila de espera FIFO para admissão, coordenadas cartesianas no mapa e o conjunto de heróis atualmente admitidos no seu interior.
* **Missão:** Demanda que surge em momento e local específicos, exigindo um conjunto de 6 a 10 habilidades para ser atendida.
* **Mundo:** Contêiner agregador com dimensões pré-definidas (20.000 x 20.000 metros), vetores de heróis, bases e missões, relógio global e métricas estatísticas globais.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

### 🔄 Dinâmica dos Eventos

O ciclo de vida das entidades é governado pela cadeia causal de eventos:

* **CHEGA:** O herói chega a uma base e decide se aguarda na fila ou desiste, ponderando sua paciência em relação ao tamanho da fila e ocupação atual.
* **ESPERA:** O herói entra na fila de espera da base e notifica o porteiro através do evento AVISA.
* **DESISTE:** O herói opta por não esperar, sorteia uma nova base de destino e agenda uma viagem.
* **AVISA:** O porteiro da base avalia a fila de espera e admite heróis enquanto houver vagas disponíveis, agendando o evento ENTRA para cada um.
* **ENTRA:** O herói é incorporado aos presentes na base e calcula seu tempo de permanência com base em sua paciência, agendando o evento SAI.
* **SAI:** O herói desocupa a vaga na base, agenda viagem para outra base e notifica novamente o porteiro (AVISA) para admitir o próximo da fila.
* **VIAJA:** Calcula a distância euclidiana até o destino, a duração do deslocamento e agenda um evento CHEGA no instante futuro correspondente.
* **MISSAO:** Disparada em instante aleatório. Varre todas as bases, determina a distância cartesiana até a missão e avalia a união das habilidades dos heróis presentes em cada base. A missão é atribuída à base mais próxima capaz de cumprir todos os requisitos. Caso nenhuma base seja apta, a missão é reagendada para 24 horas após (missão impossível no instante).
* **MORRE:** Evento reservado para o encerramento do ciclo de vida de um herói.
* **FIM:** Disparado no minuto 525.600, encerrando a execução e gerando o relatório consolidado de experiências e métricas da simulação.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

### 🧩 Estruturas de Dados (TADs)

A modularização do projeto baseia-se em Tipos Abstratos de Dados (TADs) implementados com separação estrita entre contrato de interface (`.h`) e implementação (`.c`):

* **Fila de Prioridades Genérica (`fprio`):** Lista encadeada ordenada por prioridade crescente (tempo de ocorrência), armazenando ponteiros genéricos `void *` e tratando empates de tempo com política FIFO.
* **Fila Simples (`fila`):** Fila duplamente encadeada utilizada no controle de espera nas portarias das bases.
* **Conjunto de Inteiros (`conjunto`):** Estrutura fundamentada em vetor de bits booleanos (fornecida pré-compilada em `conjunto.o`), viabilizando operações rápidas de pertinência, união, interseção, diferença e cardinalidade sobre habilidades e agrupamentos de heróis.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

### 📚 Principais Desafios e Aprendizados

Durante a concepção e implementação do projeto, destacaram-se os seguintes aspectos técnicos:

* **Adaptação ao Paradigma de Simulação:** Por ser o primeiro trabalho de grande porte do curso, a transição da programação sequencial tradicional para a modelagem orientada a eventos e estados discretos exigiu profunda compreensão da linha temporal assíncrona gerada pela LEF.
* **Resolução e Otimização de Missões:** A lógica de atendimento de missões foi o componente mais complexo do sistema. Foi necessário iterar por todas as bases candidatas, calcular distâncias euclidianas, realizar a união iterativa dos conjuntos de habilidades de todos os heróis abrigados em cada base e verificar a relação de subconjunto exigida pela missão.
* **Gerenciamento de Memória e Depuração:** Garantir ausência total de vazamentos de memória (*memory leaks*) ao longo de centenas de milhares de eventos instanciados e destruídos dinamicamente, validando a integridade das métricas com auxílio do Valgrind.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- REPOSITORY STRUCTURE -->
## 📁 Estrutura do Repositório

O projeto adota o padrão de organização recomendado para aplicações em C:

```
.
├── bin/                    Executável gerado pela compilação
├── build/                  Arquivos-objeto intermediários (*.o) e conjunto.o pré-compilado
├── include/                Arquivos de cabeçalho (.h)
│   ├── conjunto.h
│   ├── entidades.h
│   ├── eventos.h
│   ├── fila.h
│   └── fprio.h
├── src/                    Arquivos de implementação (.c)
│   ├── entidades.c
│   ├── eventos.c
│   ├── fila.c
│   ├── fprio.c
│   └── main.c
├── .gitignore              Regras de exclusão do controle de versão
├── compile_flags.txt       Diretivas de compilação para o servidor clangd
├── Makefile                Automação de compilação e limpeza
└── README.md
```

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- GETTING STARTED -->
## 🚀 Instalação

Siga os passos abaixo para obter uma cópia local do código, compilar e executar o simulador.

### 📦 Pré-requisitos

* **Compilador C:** GCC com suporte ao padrão C99.
* **Make:** GNU Make.
* **Valgrind (Opcional):** Para verificação de corretude de ponteiros e vazamento de memória.

No ambiente Ubuntu/Debian, as dependências podem ser instaladas via:
```sh
sudo apt update
sudo apt install build-essential valgrind -y
```

### 🔧 Compilação

1. Clone o repositório:
   ```sh
   git clone https://github.com/GiuTP/The-Boys.git
   ```
2. Acesse a pasta do projeto:
   ```sh
   cd The-Boys
   ```
3. Compile o programa:
   ```sh
   make
   ```
   O binário será gerado no caminho `bin/theboys`.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- USAGE EXAMPLES -->
## 💻 Uso e Execução

O `Makefile` disponibiliza regras pré-definidas para execução, testes e sanitização:

* **Compilação padrão:**
  ```sh
  make
  ```
* **Compilar e executar:**
  ```sh
  make run
  ```
* **Executar com análise de memória no Valgrind:**
  ```sh
  make valgrind
  ```
* **Limpar artefatos de compilação:**
  ```sh
  make clean
  ```
  *(Nota: O arquivo `build/conjunto.o` é mantido intacto por ser o objeto pré-compilado oficial da disciplina).*

### 📊 Formato das Mensagens de Saída

Durante o processamento, o simulador emite mensagens padronizadas informando o instante de tempo e o estado dos eventos:

```text
 45844: CHEGA  HEROI  0 BASE 2 ( 7/ 9) ESPERA
 45844: ESPERA HEROI  0 BASE 2 ( 4)
 45844: AVISA  PORTEIRO BASE 2 ( 7/ 9) FILA [ 30 17 04 23  0 ]
 45844: AVISA  PORTEIRO BASE 2 ADMITE 30
 45844: ENTRA  HEROI 30 BASE 2 ( 8/ 9) SAI 46101
 46101: SAI    HEROI 30 BASE 2 ( 7/ 9)
 46101: VIAJA  HEROI 30 BASE 2 BASE 6 DIST 6922 VEL 4763 CHEGA 46102
...
120037: MISSAO 4150 TENT 1 HAB REQ: [ 0 2 3 4 6 7 8 9 ]
120037: MISSAO 4150 BASE 7 DIST 9867 HEROIS [ 16 21 22 23 24 31 33 46 ]
120037: MISSAO 4150 CUMPRIDA BASE 7
```

Ao atingir o tempo final (525.600 minutos), a simulação conclui imprimindo o consolidado dos heróis e estatísticas gerais:

```text
525600: FIM
HEROI  0 PAC  32 VEL 3879 EXP  441 HABS [ 2 9 ]
HEROI  1 PAC  52 VEL 2974 EXP  620 HABS [ 5 ]
...
MISSOES CUMPRIDAS: 5236/5256 (99.62%)
TENTATIVAS/MISSAO: MIN 1, MAX 21, MEDIA 2.08
```

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- LICENSE -->
## 📄 Licença

Distribuído sob a licença MIT. Consulte o arquivo `LICENSE` para mais informações sobre termos de uso e redistribuição.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- CONTACT -->
## 📬 Contato

GiuTP — [github.com/GiuTP](https://github.com/GiuTP)

E-mail — giulianotpt@gmail.com

Link do projeto: [https://github.com/GiuTP/The-Boys](https://github.com/GiuTP/The-Boys)

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## 🙏 Agradecimentos

* [Prof. Carlos Maziero (DINF/UFPR)](https://www.inf.ufpr.br/maziero/) pela especificação didática do projeto.
* [Best-README-Template](https://github.com/othneildrew/Best-README-Template) pela referência de estruturação e documentação.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[stars-shield]: https://img.shields.io/github/stars/GiuTP/The-Boys.svg?style=for-the-badge
[stars-url]: https://github.com/GiuTP/The-Boys/stargazers
[issues-shield]: https://img.shields.io/github/issues/GiuTP/The-Boys.svg?style=for-the-badge
[issues-url]: https://github.com/GiuTP/The-Boys/issues
[license-shield]: https://img.shields.io/github/license/GiuTP/The-Boys.svg?style=for-the-badge
[license-url]: https://github.com/GiuTP/The-Boys/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/giuliano-tavares/
[C-badge]: https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://en.wikipedia.org/wiki/C_(programming_language)
[Linux-badge]: https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black
[Linux-url]: https://www.kernel.org/
[Valgrind-badge]: https://img.shields.io/badge/Valgrind-4B0082?style=for-the-badge&logo=c&logoColor=white
[Valgrind-url]: https://valgrind.org/
