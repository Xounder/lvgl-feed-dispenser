## 03. Fases 4 e 5 - servo simulado, simulacao melhorada e erros

[voltar ao índice](../agent-propose-implementation-plan.md)

---

# Fase 4 — Simular o servo

Mesma ideia.

Em vez de o sistema depender diretamente de:

```
```

```
servo.write(90);
```

Criamos uma abstração:

```
```

```
class IDispenser {
public:

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool isRunning() = 0;
};
```

---

## Simulação

No PC:

```
```

```
class SimulatedDispenser : public IDispenser {

private:

    bool running = false;

public:

    void start() override {

        running = true;

        printf("SERVO INICIADO\n");
    }

    void stop() override {

        running = false;

        printf("SERVO PARADO\n");
    }

    bool isRunning() override {

        return running;
    }
};
```

Na interface você pode mostrar:

```
```

```
┌─────────────────────────────┐
│                             │
│       DOSANDO...            │
│                             │
│   ⚙ DISPENSADOR: ATIVO      │
│                             │
│   Peso: 327 g / 500 g       │
│                             │
│   ███████████░░░░░░         │
│                             │
│       [ CANCELAR ]          │
│                             │
└─────────────────────────────┘
```

Mesmo sem servo.

---

# A simulação pode ser ainda melhor

Podemos fazer o peso aumentar automaticamente conforme o dispensador está ligado.

Por exemplo:

```
```

```
Meta: 500 g

Dispensador desligado
Peso: 0 g


Usuário clica:

INICIAR
      ↓

Dispensador ligado
      ↓

Peso:

0 g
↓
8 g
↓
17 g
↓
31 g
↓
46 g
↓
...
```

Quando chega próximo da meta:

```
```

```
470 g
480 g
490 g
495 g
```

Podemos até simular uma lógica mais realista:

```
```

```
Peso < 450 g

Servo:
VELOCIDADE RÁPIDA


Peso entre 450 e 490 g

Servo:
VELOCIDADE MÉDIA


Peso > 490 g

Servo:
VELOCIDADE LENTA
```

Depois:

```
```

```
500 g

SERVO PARA
```

Assim você consegue testar a lógica da dosagem **antes de existir qualquer circuito físico**.

---

# Fase 5 — Simular erros

Isso é algo que você deveria aproveitar para testar agora.

Podemos criar botões secretos ou um menu de desenvolvedor:

```
```

```
MODO SIMULAÇÃO
```

Com:

```
```

```
Peso atual

[ -10 g ]

[ +10 g ]

[ +50 g ]

[ RESET ]
```

E também:

```
```

```
SIMULAR ERROS

[ HX711 DESCONECTADO ]

[ PESO TRAVADO ]

[ SERVO NÃO RESPONDE ]

[ CANCELAR DOSAGEM ]
```

Isso seria muito bom para validar o projeto.

---

# Por exemplo: HX711 desconectado

Você pode testar:

```
```

```
Usuário inicia dosagem
        ↓
Sistema tenta ler peso
        ↓
Sensor não responde
        ↓
```

Interface:

```
```

```
┌─────────────────────────────┐
│                             │
│          ⚠ ERRO             │
│                             │
│ Não foi possível obter      │
│ o peso da balança.          │
│                             │
│ [ TENTAR NOVAMENTE ]        │
│                             │
│ [ CANCELAR ]                │
│                             │
└─────────────────────────────┘
```

Quando você comprar o HX711, a lógica já estará pronta.

---
