## 09. Informação desconhecida e dependências

[voltar ao índice](../13-agent-guide.md)

---

## 74. Como lidar com informação desconhecida

Se algo não estiver definido:

```text
não inventar.
```

Registrar como:

```text
TODO
PENDÊNCIA
DECISÃO FUTURA
```

Exemplo:

```text
O controlador exato do touch ainda não foi confirmado.
```

é melhor do que assumir:

```text
O touch utiliza controlador X.
```

sem evidência.

---

## 75. Como lidar com hardware ainda não disponível

Não escrever uma implementação física baseada em suposições.

Enquanto o hardware não estiver disponível:

```text
simular
documentar
isolar
preparar interfaces
```

Quando o componente estiver disponível:

```text
medir
testar
implementar
calibrar
```

---

## 76. Regra para novas dependências

Antes de adicionar uma biblioteca, perguntar:

```text
É realmente necessária?
Pode ser resolvido com o que já existe?
Aumenta o tamanho do firmware?
Afeta a portabilidade?
Funciona no ESP32?
É necessária no PC ou no hardware?
```

Uma dependência usada apenas para facilitar uma tarefa pequena pode ser inadequada para o firmware.

---

## 77. Regra para novas abstrações

Antes de criar:

```text
interface
factory
manager
service
adapter
wrapper
```

verificar se existe um problema concreto que justifique isso.

A arquitetura deve permanecer compreensível.

---

## 78. Regra para comentários

Comentários devem explicar:

```text
por que
```

quando o motivo não for óbvio.

Evitar comentários que apenas repetem:

```c
// incrementa peso
weight += 2;
```

Prefira explicar uma decisão arquitetural ou comportamento de simulação quando necessário.