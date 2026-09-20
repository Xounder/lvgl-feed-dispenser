## 07. Explicacao - evolucao real e avaliacao do plano

[voltar ao índice](../implementation.md)

---

# Então a evolução real ficou assim

O plano original:

```text
UI
 ↓
ScreenManager
 ↓
App
 ↓
Domain
 ↓
Hardware abstraction
 ↓
Simulation
 ↓
Real hardware
```

A implementação atual:

```text
                  main.c
                    │
                    ▼
                 ui_init
                    │
                    ▼
              ScreenManager
                    │
             ┌──────┴──────┐
             ▼             ▼
            UI          DosingConfig
             │             │
             └──────┬──────┘
                    ▼
            DosingController
                    │
             ┌──────┴──────┐
             ▼             ▼
       WeightSensor      Dispenser
             │             │
             ▼             ▼
         Simulado       Simulado
```

E futuramente:

```text
                  main.c
                    │
                    ▼
                 App/UI
                    │
                    ▼
                 Domain
                    │
             ┌──────┴──────┐
             ▼             ▼
       WeightSensor      Dispenser
             │             │
       ┌─────┴─────┐ ┌─────┴─────┐
       ▼           ▼ ▼           ▼
   Simulado      HX711       Simulado   Servo
```

---

## Minha avaliação do plano

**O plano foi bom e a decisão de não segui-lo literalmente foi melhor do que tentar obedecê-lo cegamente.**

Principalmente por três motivos:

1. **Não criamos `app/` só por criar.** A responsabilidade ainda não justificava a camada.
2. **Não criamos componentes genéricos de UI antecipadamente.** `header.c`, `button.c` etc. só fazem sentido quando houver reutilização real.
3. **Integramos a simulação ao fluxo antes de terminar toda a arquitetura "ideal".** Isso permitiu provar que a arquitetura funciona.

Então eu **não voltaria agora para reorganizar o projeto para ficar idêntico ao plano original**.

O estado atual está coerente.

O próximo refinamento natural seria justamente o que já aparece no nosso roadmap: **fortalecer o domínio e a simulação**, adicionando `ERROR`, timeout, sensor sem progresso, validação de configuração, overshoot etc. Só depois, quando formos preparar a migração para o ESP32-S3, faz sentido decidir se a camada `app/` realmente precisa existir e fazer a abstração de dependências ficar mais explícita.

Em outras palavras: **seguimos o plano arquitetural; não seguimos uma receita de pastas de forma dogmática — e isso foi uma boa escolha.**
