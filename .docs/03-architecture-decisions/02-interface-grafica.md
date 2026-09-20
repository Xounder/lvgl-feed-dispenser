## 02. Interface gráfica: LVGL e SDL2

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: utilizar LVGL

**Decisão**

A interface gráfica será construída utilizando:

```text
LVGL
```

**Motivos**

O produto final precisa de uma interface gráfica em um dispositivo embarcado com recursos limitados.

O LVGL é adequado para esse cenário porque foi projetado especificamente para interfaces gráficas embarcadas.

Ele permite utilizar conceitos como:

* telas;
* labels;
* botões;
* barras de progresso;
* eventos;
* timers;
* layouts;
* touch;
* navegação.

Esses conceitos são diretamente relevantes para o dosador.

**Por que não desenvolver uma UI própria?**

Uma UI desenvolvida manualmente exigiria implementar grande parte de funcionalidades que o LVGL já fornece.

Isso aumentaria o escopo do projeto sem contribuir diretamente para o objetivo principal, que é desenvolver o sistema de dosagem.

Portanto:

```text
UI framework → LVGL
```

é considerado infraestrutura, e não o foco do projeto.

Status: **Atual**.

A organização da UI é detalhada em [07-ui-and-navigation.md](../07-ui-and-navigation.md).

---

### 2. Decisão: utilizar SDL2 no simulador

**Decisão**

No PC, o LVGL é executado utilizando uma integração baseada em:

```text
LVGL
  +
SDL2
```

**Motivo principal**

O objetivo do SDL2 neste projeto não é ser a tecnologia gráfica definitiva do produto.

Seu papel é fornecer uma plataforma desktop para executar e visualizar a interface LVGL.

Assim:

```text
ESP32-S3
    ↓
Display físico
```

é substituído durante o desenvolvimento por:

```text
PC
 ↓
SDL2
 ↓
janela
```

**Benefício**

Isso permite testar no computador:

* layout;
* navegação;
* interação;
* botões;
* configuração;
* telas;
* timers;
* fluxo da aplicação.

Sem precisar gravar o firmware no ESP32 a cada alteração visual.

**Decisão importante**

SDL2 **não faz parte da arquitetura do produto final**.

Ele pertence à infraestrutura do simulador.

Portanto, código de domínio não deve depender de SDL2.

A dependência deve ficar aproximadamente:

```text
HAL / plataforma
       ↓
     SDL2
       ↓
     LVGL
       ↓
      UI
```

e não:

```text
Domain
  ↓
SDL2
```

Status: **Atual**.

---

### 3. Decisão: não misturar dependências do simulador com o domínio

O fato de o PC utilizar SDL2 não significa que o domínio deva depender dele.

O domínio deve continuar sendo código C relativamente independente.

Assim, uma eventual mudança de:

```text
SDL2
```

para outra infraestrutura gráfica não deveria exigir alterações nas regras da dosagem.

Status: **Atual**.