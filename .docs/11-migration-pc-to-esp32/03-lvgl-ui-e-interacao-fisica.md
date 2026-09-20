## 03. LVGL, UI e interação física

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 8. LVGL permanece

O LVGL é uma das principais partes que devem permanecer.

No PC:

```text
Application
   ↓
LVGL
   ↓
SDL2
```

No ESP32:

```text
Application
   ↓
LVGL
   ↓
Display Driver
```

Portanto, a UI desenvolvida para o simulador pode servir como base para a interface física.

O simulador já integra LVGL sobre SDL2; dicas sobre o driver de display para o ESP32 e o papel do LVGL no simulador estão em [09-pc-development-environment.md](../09-pc-development-environment.md). A relação display/LVGL no hardware é detalhada em [08-target-hardware.md](../08-target-hardware.md).

---

### 9. UI durante a migração

Os conceitos das telas devem permanecer (detalhes das telas em [07-ui-and-navigation.md](../07-ui-and-navigation.md)):

```text
Home
Mode
Config
Dosing
Completed
```

O fluxo:

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

também deve permanecer.

O que poderá mudar são detalhes relacionados ao hardware:

* tamanho dos elementos;
* fontes;
* feedback visual;
* touch;
* botões físicos;
* desempenho;
* resolução;
* orientação da tela.

---

### 10. Resolução da interface

O display alvo atual é:

```text
800 × 480
```

O simulador já utiliza essa resolução por referência (detalhes em [08-target-hardware.md](../08-target-hardware.md)).

Isso é útil porque a interface já pode ser desenvolvida considerando a resolução física esperada.

Mesmo assim, a equivalência não deve ser assumida automaticamente: o display real pode apresentar diferenças de área útil, controlador, orientação, touch, escala, densidade e rotação, e esses detalhes deverão ser validados no hardware.

A descrição do display físico (4,3" 800×480, capacitivo) está em [08-target-hardware.md](../08-target-hardware.md).

---

### 11. Touch

No PC, a interação ocorre através do SDL2 e dos dispositivos de entrada disponíveis.

No hardware:

```text
Touch controller
      ↓
driver
      ↓
LVGL input device
```

A lógica da tela não deve precisar saber qual controlador físico está sendo utilizado.

O ideal é manter a fronteira:

```text
hardware input
      ↓
LVGL input
      ↓
UI
```

A arquitetura de touch do hardware (controller, driver e calibração) é tratada em [08-target-hardware.md](../08-target-hardware.md).

---

### 12. Mouse versus touch

O simulador utiliza mouse como aproximação da interação touch.

Isso permite validar:

* posição dos botões;
* navegação;
* tamanho dos controles;
* sequência de telas.

Porém, o teste físico deve verificar:

* precisão do toque;
* área clicável;
* gestos, se utilizados;
* resposta do controlador;
* calibração;
* comportamento com dedos reais.

---

### 13. Botões físicos

O projeto também possui dois botões físicos planejados (detalhes do hardware em [08-target-hardware.md](../08-target-hardware.md)).

Eles podem ser utilizados para funções que não dependam exclusivamente da tela.

A arquitetura deve tratar esses botões como entradas da plataforma:

```text
Botão físico
      ↓
GPIO
      ↓
driver/HAL
      ↓
aplicação
```

O botão físico não deve exigir que o domínio conheça diretamente o número do GPIO.

---

### 14. Switch

O switch físico também pertence à camada de hardware.

Conceitualmente:

```text
Switch
 ↓
GPIO
 ↓
HAL/driver
 ↓
aplicação
```

A função exata do switch ainda deve ser definida conforme o produto final.

O hardware da chave liga/desliga é detalhado em [08-target-hardware.md](../08-target-hardware.md).

---

### 15. LED/indicador

O indicador físico segue a mesma ideia.

A aplicação pode eventualmente solicitar:

```text
indicador ligado
indicador desligado
indicador piscando
```

Enquanto a implementação física conhece:

```text
GPIO
```

Isso mantém o domínio separado dos detalhes elétricos.

O indicador/LED no hardware é detalhado em [08-target-hardware.md](../08-target-hardware.md).