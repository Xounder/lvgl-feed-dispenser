## 01. Linguagem e build

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: utilizar C++ como linguagem da aplicação

**Decisão**

A aplicação será desenvolvida em:

```text
C++
```

utilizando um subconjunto direto e simples da linguagem (C++ "pragmático"), sem exigir abstrações excessivas.

O domínio atual, escrito em C, permanece válido e pode ser consumido a partir do código C++; a migração progressiva do domínio/UI para C++ é esperada, mas não precisa acontecer de uma só vez.

**Motivos**

O projeto possui como destino final um microcontrolador:

```text
ESP32-S3
```

e os componentes de hardware que o firmware precisará integrar são fortemente orientados ao ecossistema C++/Arduino:

```text
HX711 (balança)
servo (SG90)
display + touch LVGL (exemplos da placa)
```

No Arduino, essas integrações possuem bibliotecas maduras e prontas que reduzem o esforço de desenvolvimento. Por exemplo, o servo:

```cpp
servo.attach(pin);
servo.write(90);
```

e o HX711:

```cpp
scale.begin(DOUT, SCK);
scale.tare();
scale.get_units();
```

Isso é especialmente importante para o display RGB 800×480 da placa alvo, cujo bring-up via exemplo oficial da fabricante é muito mais direto no Arduino.

**Linguagem das bibliotecas**

O uso de C++ não exige que todas as bibliotecas sejam C++:

| Componente         | Linguagem            |
| ------------------ | -------------------- |
| LVGL               | C (consumido de C++) |
| SDL2 (simulador)   | C                    |
| Arduino core       | C/C++                |
| HX711 / servo libs | C++                  |

**Como era antes e por que mudou**

Inicialmente a escolha foi **C**: como o LVGL é orientado a C e todo o domínio podia ser validado no PC, C era suficiente e mantinha a proximidade com o firmware.

A decisão foi revisada ao identificar o hardware alvo (placa integrada com display/touch) e o ecossistema de componentes (HX711, servo): **C++/Arduino acelera a integração física real**, que é a próxima etapa do projeto, sem exigir reescrever o que já foi validado.

**Por que não "C++zão"**

O domínio não precisa ser reescrito com classes, herança, templates e RAII.

Um C++ direto (funções, estruturas, ponteiros, classes pontuais para os adaptadores de hardware e para a aplicação) é suficiente e mantém o projeto simples, testável e claro.

**Consequência**

A linguagem da aplicação passa a ser C++; o código existente em C continua valendo e é consumido de dentro do C++ (via `extern "C"` quando necessário).

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

**Observação**

Esta decisão de CMake se refere ao **simulador PC**. Para o alvo ESP32-S3, o build seguirá **PlatformIO + Arduino framework** — ver [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

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