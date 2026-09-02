<a id="readme-top"></a>

[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<br />
<div align="center">
  <h3 align="center">🦸‍♂️ The Boys</h3>

  <p align="center">
    Um simulador de eventos discretos (SED) em C com temática da série The Boys — trabalho da disciplina Programação 2 (CI1001) na UFPR.
    <br />
    <a href="https://github.com/GiuTP/The-Boys/issues/new?labels=bug">Reportar Bug</a>
    &middot;
    <a href="https://github.com/GiuTP/The-Boys/issues/new?labels=enhancement">Sugerir Melhoria</a>
  </p>
</div>

---

<!-- SUMÁRIO -->
<details>
  <summary>Sumário</summary>
  <ol>
    <li><a href="#-sobre-o-projeto">Sobre o Projeto</a>
      <ul>
        <li><a href="#-construído-com">Construído com</a></li>
      </ul>
    </li>
    <li><a href="#-o-modelo-de-simulação">O Modelo de Simulação</a></li>
    <li><a href="#-entidades">Entidades</a></li>
    <li><a href="#-dinâmica-dos-eventos">Dinâmica dos Eventos</a></li>
    <li><a href="#-estruturas-de-dados-tads">Estruturas de Dados (TADs)</a></li>
    <li><a href="#-estrutura-do-projeto">Estrutura do Projeto</a></li>
    <li>
      <a href="#-instalação">Instalação</a>
      <ul>
        <li><a href="#-pré-requisitos">Pré-requisitos</a></li>
        <li><a href="#-compilação">Compilação</a></li>
        <li><a href="#-comandos-úteis">Comandos Úteis</a></li>
      </ul>
    </li>
    <li><a href="#-dificuldades-e-aprendizados">Dificuldades e Aprendizados</a></li>
    <li><a href="#-licença">Licença</a></li>
    <li><a href="#-contato">Contato</a></li>
    <li><a href="#-agradecimentos">Agradecimentos</a></li>
  </ol>
</details>

---

## 📖 Sobre o Projeto

**The Boys** é um simulador de eventos discretos (SED) desenvolvido em linguagem C para a disciplina **Programação 2 (CI1001)** da **Universidade Federal do Paraná (UFPR)**.

Inspirado no universo satírico da série *The Boys*, o programa simula um mundo plano bidimensional habitado por heróis com diferentes conjuntos de habilidades, bases com capacidades e filas de espera limitadas, e missões de emergência que surgem em locais aleatórios. A simulação percorre um ano completo de tempo virtual (525.600 minutos), calculando deslocamentos, decisões de paciência em filas, união de habilidades em equipes e o índice final de sucesso de cada herói.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

### 🛠 Construído com

* [![C][C-badge]][C-url]
* [![Linux][Linux-badge]][Linux-url]

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## ⏱ O Modelo de Simulação

Diferente de simulações com avanço contínuo ou passos fixos no tempo, em uma **Simulação a Eventos Discretos (SED)** o estado do sistema só muda nos instantes precisos em que ocorrem eventos.

O motor do simulador apoia-se em dois pilares:
* **Relógio Global:** Representa o tempo atual do mundo em minutos (inicia em `0` e vai até `525600`). O relógio não avança segundo a segundo; ele salta diretamente para o instante do próximo evento agendado.
* **Lista de Eventos Futuros (LEF):** Fila de prioridades ordenada cronologicamente. A simulação consome o primeiro evento da fila, atualiza o relógio, executa a rotina correspondente e agenda eventuais novos eventos decorrentes dessa ação.

```
+-------------------------------------------------------------+
|                     Laço do Simulador                       |
|                                                             |
|  1. Retira o evento prioritário da LEF                      |
|  2. relógio_global = tempo(evento)                          |
|  3. Executa o evento e altera o estado das entidades        |
|  4. Agenda novos eventos na LEF (se houver)                 |
|  5. Repete até o evento FIM (minuto 525600)                 |
+-------------------------------------------------------------+
```

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 👥 Entidades

| Entidade | Atributos Principais | Descrição |
|----------|----------------------|-----------|
| **Herói** | ID, Paciência, Velocidade, Experiência, Base Atual, Habilidades | Agente ativo do mundo. Possui entre 1 e 3 habilidades únicas e se desloca entre bases para atender missões. |
| **Base** | ID, Localização (X, Y), Lotação, Fila de Espera, Presentes | Ponto de encontro com capacidade finita. Possui um porteiro virtual que gerencia a admissão e a fila FIFO. |
| **Missão** | ID, Localização (X, Y), Habilidades Necessárias, Tentativas | Ocorrência aleatória que exige de 6 a 10 habilidades conjuntas para ser resolvida. |
| **Mundo** | Dimensões (20.000 x 20.000 m), Relógio, Estatísticas | Agregador que contém todos os heróis, bases, missões e métricas acumuladas. |

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 🔄 Dinâmica dos Eventos

O ciclo de vida da simulação é governado por eventos interligados:

| Evento | Comportamento |
|--------|---------------|
| `CHEGA` | O herói chega a uma base. Se houver vaga e fila vazia, decide esperar; senão, compara sua paciência com o tamanho da fila para decidir entre `ESPERA` ou `DESISTE`. |
| `ESPERA` | O herói entra na fila de espera da base e aciona o evento `AVISA` para alertar o porteiro. |
| `DESISTE` | O herói desiste de entrar na base, sorteia uma nova base de destino e agenda o evento `VIAJA`. |
| `AVISA` | O porteiro da base inspeciona a fila e admite tantos heróis quanto houver vagas, agendando `ENTRA` para cada um. |
| `ENTRA` | O herói entra na base e calcula seu tempo de permanência a partir de sua paciência, agendando o evento `SAI`. |
| `SAI` | O herói deixa a base, agenda `VIAJA` para outro destino e agenda `AVISA` para liberar a vaga a quem aguarda na fila. |
| `VIAJA` | Calcula a distância cartesiana e a duração da viagem com base na velocidade do herói, agendando `CHEGA` no instante de chegada. |
| `MISSAO` | Localiza a base mais próxima cujos heróis presentes possuam, juntos, todas as habilidades requeridas. Se encontrada, concede experiência aos membros; caso contrário, a missão é marcada como impossível e reagendada para 24 horas depois. |
| `MORRE` | Encerra as atividades do herói e remove seu status ativo. |
| `FIM` | Ocorre no minuto 525.600. Emite o relatório de missões cumpridas, experiência dos heróis e estatísticas gerais. |

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 🧩 Estruturas de Dados (TADs)

O projeto exercita a construção e o uso de Tipos Abstratos de Dados clássicos:

* **Fila de Prioridades (`fprio`):** Implementada como lista encadeada ordenada crescentemente por tempo, com desempate por ordem de chegada (FIFO). Atua diretamente como a LEF.
* **Fila (`fila`):** Fila duplamente encadeada com política FIFO estrita, utilizada na gestão de espera das bases.
* **Conjunto (`conjunto`):** TAD baseado em vetor de bits booleanos (fornecido em binário `conjunto.o` pelos professores). Realiza com eficiência operações de pertinência, cópia, união e cardinalidade para habilidades e heróis.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 📁 Estrutura do Projeto

```
The-Boys/
├── bin/                 executável final gerado pelo make (theboys)
├── build/               arquivos-objeto intermediários (*.o) e conjunto.o
├── include/             interfaces e definições dos TADs (*.h)
│   ├── conjunto.h
│   ├── entidades.h
│   ├── eventos.h
│   ├── fila.h
│   └── fprio.h
├── src/                 código-fonte em C (*.c)
│   ├── entidades.c
│   ├── eventos.c
│   ├── fila.c
│   ├── fprio.c
│   └── main.c
├── compile_flags.txt    configuração de flags para o LSP clangd
├── Makefile             automação de compilação e limpeza
└── README.md
```

> **Atenção:** O arquivo `build/conjunto.o` é fornecido previamente compilado pela disciplina e não é removido pelo `make clean`.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 🚀 Instalação

### 📦 Pré-requisitos

É necessário dispor de um compilador C com suporte a C99 e do GNU Make. No Ubuntu/Debian:

```sh
sudo apt update
sudo apt install build-essential valgrind -y
```

### 🔧 Compilação

1. Clone o repositório:
   ```sh
   git clone https://github.com/GiuTP/The-Boys.git
   cd The-Boys
   ```

2. Compile o projeto:
   ```sh
   make
   ```
   O executável será gerado em `bin/theboys`.

3. Execute a simulação:
   ```sh
   make run
   ```

### ⚙ Comandos Úteis

| Comando | Descrição |
|---------|-----------|
| `make` | Compila os arquivos fonte e gera `bin/theboys` |
| `make run` | Compila (se necessário) e executa o simulador |
| `make valgrind` | Executa o programa sob o Valgrind para checagem de vazamentos de memória |
| `make clean` | Remove os arquivos-objeto gerados e o executável (preservando `conjunto.o`) |
| `make tgz` | Empacota os arquivos de entrega no formato padrão da disciplina |

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 📚 Dificuldades e Aprendizados

Este projeto representou um marco importante na minha trajetória acadêmica por ser o primeiro trabalho de grande porte da faculdade. Entre os maiores desafios enfrentados, destacam-se:

- **Mudança de Paradigma (SED)** — entender a dinâmica de uma simulação a eventos discretos, onde o tempo não flui linearmente segundo a segundo, mas sim em saltos coordenados por uma Lista de Eventos Futuros (LEF).
- **Lógica das Missões e Conjuntos** — a função de resolução de missões foi a mais trabalhosa de conceber. Foi necessário iterar sobre as bases do mundo, calcular distâncias cartesianas até a missão, computar a união de conjuntos das habilidades dos heróis presentes em cada base e encontrar a base mais próxima apta a cumprir a demanda.
- **Validação de Estatísticas e Depuração** — garantir que cada evento atualizasse corretamente os acumuladores ao longo de mais de 500 mil minutos de simulação, além de investigar inconsistências numéricas e vazamentos de memória com o Valgrind, foi um processo intenso de depuração.

Superar esses pontos consolidou meu domínio prático sobre alocação dinâmica, modularização em C e estruturas de dados fundamentais.

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 📄 Licença

O código-fonte deste projeto está distribuído sob a licença **MIT**. Consulte o arquivo `LICENSE` para mais informações.

O arquivo objeto `conjunto.o` e sua respectiva especificação de interface pertencem ao Departamento de Informática da Universidade Federal do Paraná (DINF/UFPR).

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 📬 Contato

GiuTP — [github.com/GiuTP](https://github.com/GiuTP)

E-mail — giulianotpt@gmail.com

Link do projeto: [https://github.com/GiuTP/The-Boys](https://github.com/GiuTP/The-Boys)

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

## 🙏 Agradecimentos

* [Prof. Carlos Maziero (DINF/UFPR)](https://www.inf.ufpr.br/maziero/) — pela concepção didática da especificação e disponibilização do TAD Conjunto
* [Best-README-Template](https://github.com/othneildrew/Best-README-Template) — template base deste README

<p align="right">(<a href="#readme-top">voltar ao topo</a>)</p>

---

<!-- MARKDOWN LINKS & IMAGES -->
[stars-shield]: https://img.shields.io/github/stars/GiuTP/The-Boys.svg?style=for-the-badge
[stars-url]: https://github.com/GiuTP/The-Boys/stargazers
[issues-shield]: https://img.shields.io/github/issues/GiuTP/The-Boys.svg?style=for-the-badge
[issues-url]: https://github.com/GiuTP/The-Boys/issues
[license-shield]: https://img.shields.io/github/license/GiuTP/The-Boys.svg?style=for-the-badge
[license-url]: https://github.com/GiuTP/The-Boys/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/giuliano-tavares/
[C-badge]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://en.wikipedia.org/wiki/C_(programming_language)
[Linux-badge]: https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black
[Linux-url]: https://www.kernel.org/
