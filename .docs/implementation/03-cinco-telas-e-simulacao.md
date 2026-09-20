## 03. As cinco telas, a simulacao e a ordem de implementacao

[voltar ao índice](../implementation.md)

---

# 5. Para as primeiras telas

Eu faria **5 telas**, mas inicialmente sem hardware:

### Home

```text
┌───────────────────────────────────────────────┐
│              DISPENSADOR                      │
│                                               │
│                                               │
│             [  INICIAR  ]                    │
│                                               │
│                                               │
│              Sistema pronto                  │
└───────────────────────────────────────────────┘
```

### Seleção de modo

```text
┌───────────────────────────────────────────────┐
│ ←  SELECIONE O MODO                           │
│                                               │
│       [ QUANTIDADE ]                          │
│                                               │
│       [ PORÇÕES ]                             │
│                                               │
│       [ MANUAL ]                              │
└───────────────────────────────────────────────┘
```

### Configuração

Por exemplo:

```text
┌───────────────────────────────────────────────┐
│ ←  QUANTIDADE                                 │
│                                               │
│                 150 g                         │
│                                               │
│          [ − ]       [ + ]                    │
│                                               │
│              [ CONFIRMAR ]                    │
└───────────────────────────────────────────────┘
```

### Dosagem

Aqui entra a parte interessante:

```text
┌───────────────────────────────────────────────┐
│              DISPENSANDO                      │
│                                               │
│                 87 g                          │
│              / 150 g                          │
│                                               │
│        ███████████░░░░░                       │
│                                               │
│               [ PARAR ]                       │
└───────────────────────────────────────────────┘
```

### Concluído

```text
┌───────────────────────────────────────────────┐
│                                               │
│                 ✓                             │
│                                               │
│           DOSAGEM CONCLUÍDA                   │
│                                               │
│                 151 g                         │
│                                               │
│             [ NOVA DOSAGEM ]                  │
└───────────────────────────────────────────────┘
```

---

## 6. E depois entra a simulação

Quando as telas estiverem funcionando, aí fazemos:

```text
                    Aplicação
                       │
              ┌────────┴────────┐
              │                 │
        WeightSensor        Dispenser
              │                 │
              ▼                 ▼
        ┌───────────┐     ┌─────────────┐
        │ Simulado  │     │  Simulado   │
        └───────────┘     └─────────────┘
```

Por exemplo, o peso simulado poderia subir:

```text
0g → 12g → 27g → 43g → 68g → 91g → 120g → 150g
```

Isso permitirá testar **toda a lógica da dosagem no PC** antes de termos o HX711.

Depois:

```text
SimulatedWeightSensor
        ↓
      HX711
```

sem precisar reescrever a aplicação.

---

### Eu faria a implementação nesta ordem

```text
1. ui/
2. ScreenManager
3. Home
4. Mode
5. Config
6. Dosing
7. Completed
8. Eventos/navegação
9. App layer
10. Estados da aplicação
11. Sensor simulado
12. Dispenser simulado
13. Dosagem simulada
14. Só depois hardware real
```

**Próximo passo:** criar `src/ui/` e o primeiro `ScreenManager`, mantendo o `main.c` funcionando. A partir daí podemos montar a **Home de 800×480** com LVGL, em vez de tentar construir todas as telas de uma vez.
