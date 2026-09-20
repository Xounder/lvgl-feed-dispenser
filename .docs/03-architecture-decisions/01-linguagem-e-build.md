## 01. Linguagem e build

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: utilizar C

**Decisão**

A aplicação principal será desenvolvida em:

```text
C
```

**Motivos**

O projeto possui como destino final um microcontrolador:

```text
ESP32-S3
```

e possui requisitos de integração direta com:

* GPIO;
* PWM;
* sensores;
* atuadores;
* display;
* touch;
* interfaces de comunicação;
* drivers de hardware.

C é uma linguagem naturalmente adequada a esse contexto.

Além disso, o próprio LVGL possui uma base fortemente orientada a C, o que torna a integração direta e natural.

**Por que não começar com C++?**

C++ seria uma alternativa tecnicamente válida para o ESP32-S3.

Entretanto, não havia necessidade inicial de introduzir:

* classes;
* herança;
* templates;
* RAII;
* STL;
* abstrações adicionais da linguagem.

O projeto precisava principalmente de:

```text
interfaces simples
+
estruturas de dados
+
funções
+
ponteiros para funções
```

O modelo de `struct + function pointers` utilizado nas abstrações atuais é suficiente para representar os contratos de hardware.

**Benefício adicional**

Utilizar C no simulador significa que grande parte do código que futuramente será executado no ESP32-S3 já pode ser desenvolvida e validada no PC.

Isso reduz a diferença entre:

```text
código do simulador
```

e:

```text
código do dispositivo
```

**Consequência**

A escolha por C exige maior disciplina em relação a:

* gerenciamento de memória;
* ownership;
* ciclo de vida;
* interfaces;
* encapsulamento;
* validação de ponteiros.

Essa complexidade deve ser tratada explicitamente na arquitetura e não escondida atrás de abstrações excessivas.

Status: **Atual**.

---

### 2. Decisão: utilizar CMake

**Decisão**

O projeto utiliza:

```text
CMake
```

como sistema de build.

**Motivos**

O projeto precisa eventualmente ser compilado em ambientes diferentes:

```text
Windows / PC
ESP32-S3
```

CMake fornece uma forma estruturada de descrever:

* fontes;
* includes;
* bibliotecas;
* opções de compilação;
* configurações de plataforma.

Também evita depender exclusivamente da configuração manual do IDE.

Status: **Atual**.

---

### 3. Decisão: utilizar vcpkg para SDL2 no PC

**Decisão**

O SDL2 utilizado pelo simulador é obtido através do:

```text
vcpkg
```

**Motivo**

O gerenciamento manual de bibliotecas no Windows poderia exigir:

* download manual;
* configuração de include paths;
* configuração de library paths;
* cópia de DLLs;
* manutenção de versões.

O vcpkg centraliza parte desse processo.

No projeto atual, o SDL2 é instalado para:

```text
x64-windows
```

e integrado ao CMake através do toolchain do vcpkg.

Status: **Atual**.

Detalhes do ambiente: [09-pc-development-environment.md](../09-pc-development-environment.md).