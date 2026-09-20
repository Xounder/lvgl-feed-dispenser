# Configuração do LVGL no build

[voltar ao índice](../09-pc-development-environment.md)

---

## 43. LVGL e `lv_conf.h`

O projeto possui uma configuração própria do LVGL através de:

```text
lv_conf.h
```

Essa configuração determina quais recursos do LVGL ficam habilitados.

Durante a configuração do simulador, houve uma dependência importante relacionada a gráficos vetoriais.

---

## 44. ThorVG

A configuração do projeto utiliza:

```text
LV_USE_VECTOR_GRAPHIC = 1
```

Com essa opção habilitada, o LVGL exigia:

```text
ThorVG
```

Por isso:

```text
LV_USE_THORVG
```

foi alterado de:

```text
0
```

para:

```text
1
```

A mudança permitiu que a configuração atual do LVGL fosse compilada corretamente.

---

## 45. Avisos do LVGL

Ao iniciar o simulador, podem aparecer avisos semelhantes a:

```text
[Warn] lv_init: Memory integrity checks are enabled...
[Warn] lv_init: Object sanity checks are enabled...
[Warn] lv_init: Style sanity checks are enabled...
```

Essas mensagens são avisos relacionados às verificações de diagnóstico do LVGL.

No estado atual do projeto, elas não representam falhas de compilação nem impediram a execução do simulador.

---

## 46. Demo padrão do LVGL

O projeto possui acesso aos exemplos/demos do LVGL:

```c
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
```

Porém, o demo padrão de widgets está desabilitado:

```c
// lv_demo_widgets();
```

Isso é intencional.

A aplicação utiliza suas próprias telas.

---

## 47. Por que não usar o demo como aplicação

Os demos do LVGL servem para:

* validar instalação;
* explorar widgets;
* testar recursos;
* aprender APIs.

Mas o produto possui um fluxo próprio:

```text
Home
 ↓
Mode
 ↓
Config
 ↓
Dosing
 ↓
Completed
```

Portanto, o código da aplicação não deve depender da estrutura dos demos.

O fluxo de telas e sua navegação são detalhados em [07-ui-and-navigation.md](../07-ui-and-navigation.md).