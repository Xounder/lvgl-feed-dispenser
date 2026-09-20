## 07. Refatoração e mudanças

[voltar ao índice](../13-agent-guide.md)

---

## 56. Como adicionar uma nova funcionalidade

Antes de implementar:

```text
1. entender o requisito
2. identificar a camada correta
3. verificar documentação
4. procurar código existente relacionado
5. definir menor mudança necessária
6. implementar
7. compilar
8. testar
9. atualizar documentação se necessário
```

---

## 57. Exemplo: adicionar um novo sensor

Não fazer:

```text
UI
 ↓
novo_sensor.c
```

Primeiro perguntar:

```text
Qual capacidade do domínio é necessária?
```

Se for peso:

```text
WeightSensor
```

Se for uma capacidade realmente diferente:

```text
criar nova interface
```

somente se houver necessidade real.

---

## 58. Exemplo: adicionar tela de erro

A divisão esperada seria:

```text
Domain
 ↓
ERROR
 ↓
UI
 ↓
Error Screen
```

A tela não deve decidir que houve erro.

O domínio deve informar o estado.

---

## 59. Exemplo: adicionar timeout

O timeout pertence conceitualmente ao comportamento da operação.

Portanto, evitar implementar:

```text
if (tempo > X)
    mostrar "Erro"
```

somente dentro da tela.

O ideal é que a regra seja representada no domínio e a UI apenas apresente:

```text
ERROR
```

e a informação correspondente.

---

## 60. Quando refatorar

Uma refatoração é justificável quando:

```text
há duplicação relevante
há acoplamento problemático
uma mudança futura fica difícil
uma abstração já possui múltiplas implementações
um bug é consequência da estrutura atual
```

Evitar refatorar apenas por preferência estética.

---

## 61. Quando NÃO refatorar

Não refatorar automaticamente por:

```text
estilo pessoal
preferência por outra arquitetura
preferência por outra linguagem
preferência por outro framework
"clean code" abstrato sem necessidade
```

O projeto possui decisões arquiteturais deliberadas.

---

## 62. Não trocar C sem motivo

O projeto utiliza C intencionalmente por sua proximidade com:

* firmware;
* ESP32;
* controle de memória;
* APIs de hardware;
* LVGL.

Não migrar para C++, Rust, Python ou outra linguagem apenas por preferência.

Uma mudança de linguagem seria uma decisão arquitetural grande e exigiria justificativa específica.

---

## 63. Não trocar LVGL sem motivo

LVGL é parte central da estratégia:

```text
PC
 ↓
SDL2
 ↓
LVGL
```

e posteriormente:

```text
ESP32
 ↓
display/touch
 ↓
LVGL
```

A possibilidade de reutilizar a UI entre plataformas é uma das razões para sua escolha.

---

## 64. Não trocar o sistema de build sem motivo

O projeto utiliza:

```text
CMake
```

e atualmente:

```text
vcpkg
```

para dependências do ambiente PC.

Não substituir o build system para resolver um problema localizado.