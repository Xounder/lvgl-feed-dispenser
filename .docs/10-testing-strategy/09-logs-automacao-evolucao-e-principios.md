# Logs, automação, evolução e princípios

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 68. Logs durante testes

Quando necessário, o controller e as implementações de hardware podem registrar informações como:

```text
estado atual
peso atual
meta
dispenser ativo
tempo de operação
última alteração de peso
erro detectado
```

Exemplo conceitual:

```text
[DOSING] target=100 weight=42 dispenser=ON
[DOSING] target=100 weight=44 dispenser=ON
[DOSING] target=100 weight=46 dispenser=ON
```

Isso facilita a investigação de comportamentos inesperados.

---

## 69. Testes determinísticos

Sempre que possível, o simulador deve permitir reproduzir o mesmo cenário.

Por exemplo:

```text
target = 100
incremento = 2
intervalo = 300 ms
```

Isso permite reproduzir um bug.

Quanto mais determinístico for o simulador, mais fácil será comparar:

```text
antes
```

e:

```text
depois
```

de uma alteração.

---

## 70. Testes automatizados futuros

O projeto pode evoluir para testes automatizados principalmente no domínio.

Exemplo conceitual:

```text
DosingConfig
      ↓
DosingController
      ↓
Mock/Simulated Hardware
      ↓
assert
```

A UI não precisa necessariamente estar aberta para testar regras de negócio.

Isso permite testar rapidamente:

```text
start()
update()
cancel()
timeout()
completion()
```

---

## 71. Prioridade dos testes

Nem todos os testes possuem a mesma prioridade durante toda a evolução.

A ordem conceitual é:

```text
1. Segurança
2. Estado correto
3. Parada do dispenser
4. Precisão da dosagem
5. Tratamento de erros
6. Interface
7. Otimizações
```

No hardware físico, impedir uma operação insegura é mais importante do que atingir uma interface visual perfeita.

---

## 72. O que caracteriza uma implementação pronta

Uma funcionalidade não deve ser considerada pronta apenas porque:

```text
"funcionou uma vez"
```

Ela deve, no mínimo:

```text
compilar
executar
funcionar no caminho normal
ser cancelável quando aplicável
não deixar atuador ativo indevidamente
suportar repetição
não quebrar fluxos existentes
```

Para funcionalidades físicas:

```text
também deve ser testada em condições reais.
```

---

## 73. Estratégia para agentes futuros

Um agente que modificar o projeto deve seguir este ciclo:

```text
1. Entender o estado atual
2. Alterar a menor parte necessária
3. Compilar
4. Executar
5. Testar o cenário alterado
6. Executar regressão dos fluxos principais
7. Registrar limitações quando necessário
```

Não assumir que uma alteração está correta apenas porque o código compila.

---

## 74. Não confundir teste de UI com teste de domínio

Um teste como:

```text
"clicar em Continuar funciona"
```

não substitui:

```text
"o controller inicia corretamente"
```

Da mesma forma:

```text
"Completed apareceu"
```

não garante sozinho que:

```text
dispenser.stop()
```

foi executado.

Os testes devem verificar o comportamento por trás da interface.

---

## 75. Estado atual versus estado futuro

### Atualmente já implementado

```text
✓ dosagem normal
✓ aumento simulado do peso
✓ parada ao atingir meta
✓ cancelamento
✓ reset
✓ conclusão
✓ nova dosagem
```

### Ainda planejado

```text
○ timeout
○ sensor sem progresso
○ sensor inválido
○ excesso de peso
○ tolerância de overshoot
○ estado ERROR
○ validações de domínio mais completas
○ testes automatizados
```

Os itens planejados não devem ser documentados como se já existissem.

---

## 76. Evolução esperada

A estratégia de testes deve evoluir juntamente com a implementação:

```text
Fase 1
Fluxo normal
    ↓
Fase 2
Cancelamento/reset
    ↓
Fase 3
Falhas simuladas
    ↓
Fase 4
Testes automatizados do domínio
    ↓
Fase 5
Hardware real
    ↓
Fase 6
Testes físicos e calibração
    ↓
Fase 7
Testes de segurança e repetibilidade
```

---

## 77. Princípio final

O sistema deve ser testado não apenas para provar que o caminho feliz funciona, mas para descobrir como ele se comporta quando as coisas dão errado.

A ideia central é:

> **Toda operação de dosagem deve possuir um caminho normal para conclusão e um caminho seguro para interrupção.**

No simulador:

```text
cenário normal
        ↓
validar comportamento
```

Nos cenários de falha:

```text
falha
 ↓
detecção
 ↓
parada segura
 ↓
ERROR
```

No hardware:

```text
problema físico
 ↓
detecção quando possível
 ↓
atuador em estado seguro
 ↓
informação ao usuário
```

O objetivo final da estratégia de testes é garantir que a evolução do projeto não dependa apenas de demonstrações manuais bem-sucedidas, mas que o comportamento normal, os limites e as falhas previsíveis sejam deliberadamente exercitados antes e depois da migração para o hardware físico.