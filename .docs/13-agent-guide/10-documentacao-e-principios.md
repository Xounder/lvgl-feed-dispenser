## 10. Documentação e princípios

[voltar ao índice](../13-agent-guide.md)

---

## 79. Regra para documentação

Quando uma decisão importante mudar, atualizar a documentação correspondente.

Exemplos:

```text
mudança arquitetural
→ 03-architecture-decisions.md

mudança de estado
→ 04-domain-and-state-machine.md

mudança de UI
→ 07-ui-and-navigation.md

mudança de hardware
→ 08-target-hardware.md

mudança de testes
→ 10-testing-strategy.md

mudança de migração
→ 11-migration-pc-to-esp32.md

mudança de etapas
→ 12-roadmap.md
```

---

## 80. Não atualizar documentação com hipóteses

Documentação deve diferenciar:

```text
Atual
Planejado
Possível
Pendente
```

Exemplo:

```text
Atual:
simulação adiciona 2 g a cada ~300 ms.

Planejado:
simular overshoot.

Pendente:
determinar comportamento real do mecanismo.
```

Isso evita que futuros agentes confundam hipótese com implementação.

---

## 81. Prioridade das informações

Quando houver conflito entre documentação e código:

```text
1. comportamento observado/testado
2. código atual
3. documentação
4. arquitetura planejada
```

Porém, a inconsistência deve ser identificada.

Não alterar silenciosamente a documentação para esconder uma divergência.

---

## 82. Quando uma decisão antiga estiver errada

Não preservar uma decisão apenas porque está documentada.

O procedimento deve ser:

```text
problema identificado
 ↓
entender impacto
 ↓
propor mudança
 ↓
implementar
 ↓
atualizar documentação
```

O histórico arquitetural deve continuar compreensível.

---

## 83. O simulador como contrato comportamental

O simulador deve servir como referência para o comportamento da aplicação.

Exemplo:

```text
usuário configura
 ↓
controller recebe
 ↓
operação começa
 ↓
peso muda
 ↓
meta é alcançada
 ↓
dispenser para
 ↓
operação termina
```

Quando o hardware real for integrado, ele deve implementar o mesmo comportamento conceitual.

---

## 84. O que o agente deve preservar

Ao trabalhar neste projeto, preservar:

```text
✓ separação de responsabilidades
✓ desenvolvimento incremental
✓ simulação antes do hardware
✓ hardware substituível
✓ domínio independente de plataforma
✓ testes reproduzíveis
✓ documentação das decisões
✓ possibilidade de migração para ESP32
```

---

## 85. O que o agente deve evitar

Evitar:

```text
✗ colocar regra de negócio na UI
✗ colocar LVGL no domínio
✗ colocar SDL2 no domínio
✗ colocar Windows no domínio
✗ acoplar domínio ao SG90
✗ assumir pinagem
✗ inventar hardware
✗ apagar a simulação quando surgir hardware real
✗ criar abstrações sem necessidade
✗ refatorar tudo para resolver um problema pequeno
✗ introduzir dependências sem necessidade
✗ tratar planejamento como implementação existente
✗ ignorar falhas físicas possíveis
```

---

## 86. Filosofia de desenvolvimento

O projeto segue uma abordagem incremental:

```text
pequena mudança
 ↓
compilar
 ↓
executar
 ↓
testar
 ↓
validar
 ↓
documentar
 ↓
próxima mudança
```

Evitar acumular muitas alterações antes de testar.

---

## 87. Princípio de menor mudança segura

Quando duas soluções funcionarem, preferir inicialmente aquela que:

* altera menos arquivos;
* introduz menos dependências;
* preserva as interfaces existentes;
* mantém o simulador funcionando;
* facilita a futura migração;
* é fácil de testar.

Isso não significa evitar toda refatoração.

Significa evitar complexidade que ainda não possui justificativa.

---

## 88. Como pensar sobre novas funcionalidades

Para qualquer requisito novo, seguir:

```text
Requisito
   ↓
Qual comportamento é necessário?
   ↓
Qual camada possui essa responsabilidade?
   ↓
Existe uma abstração existente?
   ↓
É necessário criar uma nova?
   ↓
Como testar no PC?
   ↓
Como isso funcionará no ESP32?
```

Essa sequência deve orientar a implementação.