## 07. Reservatório e mecânica

[voltar ao índice](../08-target-hardware.md)

---

### 31. Reservatório

O sistema terá um reservatório para armazenar a ração.

A estrutura conceitual é:

```text
┌──────────────────┐
│                  │
│   RESERVATÓRIO   │
│                  │
│     RAÇÃO        │
│                  │
└────────┬─────────┘
         │
      abertura
         │
         ▼
      recipiente
```

O reservatório deve ser projetado considerando:

* fluxo;
* capacidade;
* facilidade de abastecimento;
* possibilidade de obstrução;
* acesso para limpeza;
* estabilidade mecânica.

---

### 32. Recipiente de pesagem

A célula de carga deverá medir o recipiente onde a ração é depositada.

Conceitualmente:

```text
        ração
          ↓
   ┌─────────────┐
   │  recipiente │
   └──────┬──────┘
          │
      estrutura
          │
          ▼
     Load Cell
```

A estrutura precisa permitir que a carga seja transferida de forma consistente para a célula.

---

### 33. Relação entre mecânica e software

A mecânica pode alterar significativamente o comportamento do software.

Por exemplo:

```text
abertura maior
     ↓
fluxo maior
     ↓
overshoot maior
```

ou:

```text
abertura menor
     ↓
fluxo menor
     ↓
dosagem mais lenta
```

Portanto, a integração deve ser iterativa:

```text
software
   ↕
eletrônica
   ↕
mecânica
```

---

[voltar ao índice](../08-target-hardware.md)