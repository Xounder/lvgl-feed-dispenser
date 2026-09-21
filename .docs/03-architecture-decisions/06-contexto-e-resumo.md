## 06. Contexto e resumo

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: preservar o contexto das escolhas

As decisões deste documento não devem ser interpretadas como regras absolutas.

Uma tecnologia pode ser substituída se existir uma razão técnica concreta.

Entretanto, uma alteração deve responder:

1. Qual problema a mudança resolve?
2. O que ela adiciona de complexidade?
3. O que ela remove?
4. O que será perdido?
5. Ela melhora ou prejudica a migração para o ESP32?
6. Ela mantém a separação entre UI, domínio e hardware?
7. Ela torna o simulador mais ou menos útil?

A existência deste documento não tem como objetivo impedir evolução.

Seu objetivo é impedir alterações sem contexto.

Status: **Atual** (princípio permanente).

---

### 2. Resumo das decisões

| Decisão                                   | Motivo principal                                              | Status    |
| ----------------------------------------- | ------------------------------------------------------------- | --------- |
| **C++ (com C no LVGL/SDL2/domínio atual)** | Integração com Arduino/bibliotecas do ESP32 sem abrir mão do validado | Atual |
| **LVGL**                                  | Interface gráfica adequada para sistemas embarcados           | Atual     |
| **SDL2 no PC**                            | Executar/testar LVGL em desktop                               | Atual     |
| **CMake**                                 | Build estruturado do simulador PC                            | Atual     |
| **vcpkg**                                 | Gerenciamento de dependências do simulador                    | Atual     |
| **PlatformIO + Arduino (ESP32)**          | Build/impl. do firmware com bibliotecas prontas               | Planejado |
| **Simular antes do ESP32**                | Reduzir variáveis e acelerar desenvolvimento                  | Atual     |
| **Simulador permanente**                  | Testes, regressão e desenvolvimento sem hardware              | Atual     |
| **Evolução incremental**                  | Reduzir risco e facilitar validação                           | Atual     |
| **WeightSensor**                          | Separar lógica de dosagem do HX711                            | Atual     |
| **Dispenser**                             | Separar lógica de dosagem do servo/atuador                    | Atual     |
| **DosingController**                      | Centralizar as regras do processo                             | Atual     |
| **Estados explícitos**                    | Representar claramente o ciclo da dosagem                     | Atual     |
| **DosingConfig**                          | Centralizar dados de configuração                             | Atual     |
| **UI separada do domínio**                | Evitar regras de negócio nos callbacks                        | Atual     |
| **Hardware separado do domínio**          | Evitar dependência de drivers                                 | Atual     |
| **Abstrações apenas quando justificadas** | Evitar overengineering                                        | Atual     |

Decisões marcadas como **Possível** estão detalhadas nas partes [03-estrategia-de-desenvolvimento.md](03-estrategia-de-desenvolvimento.md), [04-separacao-e-abstracoes.md](04-separacao-e-abstracoes.md) e [05-ui-estados-e-controller.md](05-ui-estados-e-controller.md).

---

### 3. Princípio final

Todas as decisões deste projeto podem ser resumidas por uma ideia:

```text
Desenvolver primeiro o comportamento,
isolar as dependências físicas,
validar no simulador
e substituir as implementações simuladas
pelas reais quando o hardware estiver disponível.
```

Ou, de forma ainda mais direta:

> **O hardware deve implementar as necessidades do domínio; o domínio não deve ser moldado pelos detalhes do hardware.**

Essa é a principal razão para a existência das abstrações, do simulador e da separação entre UI, domínio e hardware.