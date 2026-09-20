## 05. Explicacao - a camada app nao criada e ScreenManager vs App

[voltar ao índice](../implementation.md)

---

# 5. A maior diferença: não criamos a camada `app/`

O plano original dizia:

```text
src/
└── app/
    └── app.c
```

E a ideia seria:

```text
UI
 ↓
APP
 ↓
DOMAIN
 ↓
HARDWARE
```

Hoje temos mais diretamente:

```text
UI
 ↓
ScreenManager
 ↓
Domain
 ↓
Hardware
```

Por exemplo:

```text
ConfigScreen
     ↓
screen_manager_show(SCREEN_DOSING)
     ↓
ScreenManager
     ↓
dosing_controller_start()
```

Não existe:

```text
app_start_dosing()
```

intermediando.

### Isso foi errado?

**Não necessariamente.**

Naquele momento, a camada `app/` teria pouca responsabilidade real.

Se tivéssemos criado:

```c
void app_start_dosing(void)
{
    ...
}
```

apenas para chamar:

```c
dosing_controller_start();
```

teríamos criado uma camada sem comportamento significativo.

Isso seria uma abstração prematura.

Por isso acabamos com:

```text
UI
 ↓
ScreenManager
 ↓
DosingController
```

E isso é perfeitamente aceitável para o estágio atual.

---

# 6. Mas existe uma diferença importante entre `ScreenManager` e `App`

Esse ponto vale guardar para o futuro.

O `ScreenManager` é responsável por:

```text
qual tela mostrar
```

Enquanto uma eventual camada `app/` seria responsável por:

```text
qual operação do sistema executar
```

Por exemplo, no futuro poderíamos chegar a:

```text
UI
 │
 ▼
Application
 │
 ├── iniciar dosagem
 ├── cancelar dosagem
 ├── configurar operação
 └── tratar eventos globais
 │
 ▼
Domain
 │
 ▼
Hardware
```

Enquanto:

```text
ScreenManager
```

ficaria concentrado em:

```text
HOME
MODE
CONFIG
DOSING
COMPLETED
ERROR
```

**Mas não há necessidade de criar isso agora só porque estava no plano inicial.**

Quando existir comportamento de aplicação que não pertença claramente ao `ScreenManager` nem ao `DosingController`, aí a camada `app/` passa a ter uma justificativa concreta.

---
