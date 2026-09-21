## 02. Display e touch

[voltar ao índice](../08-target-hardware.md)

---

### 5. Display

O display alvo possui aproximadamente:

```text
4,3"
800 × 480 pixels (RGB)
touch capacitivo
```

O display é usado em **orientação retrato**, de modo que a área útil da
UI é:

```text
480 × 800
```

A área em retrato já é utilizada no simulador:

```c
sdl_hal_init(480, 800);
```

Isso permite desenvolver a interface levando em consideração a resolução real desde o início.

---

### 6. Por que desenvolver na resolução final

Desenvolver a interface em:

```text
480 × 800 (retrato)
```

permite validar antecipadamente:

* tamanho dos botões;
* espaçamento;
* posição dos elementos;
* quantidade de informações;
* tamanho dos textos;
* barra de progresso;
* área útil do display;
* interação por touch.

Assim, a migração para o hardware não precisa começar com uma reconstrução completa da interface.

---

### 7. Display e LVGL

O display será utilizado pelo LVGL.

No simulador:

```text
LVGL
  ↓
SDL2
  ↓
janela 480×800 (retrato)
```

No hardware:

```text
LVGL
  ↓
driver de display
  ↓
display físico
```

A camada de aplicação deve continuar trabalhando com os objetos e APIs do LVGL, enquanto a camada de plataforma/hardware cuida da comunicação física com o display.

---

### 8. Touch capacitivo

O display possui touch capacitivo.

A intenção é utilizar o touch como principal método de interação com a interface gráfica.

O fluxo será:

```text
usuário toca tela
       ↓
touch controller
       ↓
ESP32-S3
       ↓
LVGL input device
       ↓
widget
       ↓
callback
       ↓
ação da aplicação
```

---

### 9. Touch e abstração

Assim como acontece com o sensor de peso, o restante da aplicação não deve precisar conhecer detalhes elétricos do touch.

A UI deve trabalhar conceitualmente com:

```text
pressionado
solto
posição X
posição Y
```

e não com:

```text
GPIO
I²C
SPI
registradores
controlador específico
```

Esses detalhes pertencem à implementação da plataforma.

---

[voltar ao índice](../08-target-hardware.md)