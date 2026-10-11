## 04. Protocolo e mensagens

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Modelo de comunicação

A comunicação é **bidirecional e orientada a mensagens** sobre UART. A tela é o
**cliente** (inicia comandos e recebe telemetria); o externo é o **servidor**
(aplica os comandos no domínio e envia estado).

```text
Tela (cliente) ────────►  Externo (servidor)
    COMANDO                  aplica no DosingController
Tela (cliente) ◄────────  Externo (servidor)
    estado/peso/meta          telemetria periódica
```

---

### 2. Tipos de mensagem

#### 2.1 Comandos (tela → externo)

| Mensagem | Direção | Descrição |
| -------- | ------- | --------- |
| `START` | tela → externo | Iniciar dosagem automática |
| `CANCEL` | tela → externo | Interromper dosagem (origem UI) |
| `NEW_DOSING` | tela → externo | Resetar e voltar a IDLE |
| `CONFIG_SET` | tela → externo | Enviar config (modo, gramas/valor, preço) |
| `MANUAL_RELEASE_START` | tela → externo | Iniciar liberação manual |
| `MANUAL_RELEASE_STOP` | tela → externo | Parar liberação manual |
| `PING` | tela → externo | Sondar presença/health |

#### 2.2 Telemetria (externo → tela)

| Mensagem | Direção | Descrição |
| -------- | ------- | --------- |
| `STATE` | externo → tela | Estado (`IDLE/DOSING/COMPLETED/INTERRUPTED`) + fase |
| `WEIGHT` | externo → tela | Peso atual (g) |
| `TARGET` | externo → tela | Meta efetiva (g) |
| `MANUAL_RELEASE_ACTIVE` | externo → tela | Estado da liberação manual |
| `ERROR` | externo → tela | Falha/condição insegura |
| `ACK` | externo → tela | Confirmação de comando recebido |
| `PONG` | externo → tela | Resposta ao `PING` |

---

### 3. Serialização (formato do frame)

A comunicação é **mantida simples**, sem JSON — frames binários com checksum:

```text
┌─────────┬─────────┬─────────┬─────────────┬─────────┐
│ 0xAA    │ len     │ msg_id  │ payload     │ CRC8    │
│ header  │ (1 byte)│ (1 byte)│ (n bytes)   │ (1 byte)│
└─────────┴─────────┴─────────┴─────────────┴─────────┘
```

- `header` = `0xAA` (permite re-sincronizar o stream após ruído).
- `len` = tamanho do payload.
- `msg_id` = código da mensagem (START, STATE, ...).
- `CRC8` sobre header+len+msg_id+payload.

Payloads:
- `CONFIG_SET` → `mode(1) + target_grams(2) + target_money_cents(2) + price_per_kg_cents(2)`.
- `WEIGHT` → `grams(2)`.
- `STATE` → `state(1) + phase(1)`.
- Demais: payload vazio.

---

### 4. Handshake e telemetria periódica

1. Ao iniciar, a tela envia `PING` até receber `PONG` (ou após timeout).
2. A tela envia `CONFIG_SET` com a config atual.
3. O externo responde `ACK` e passa a enviar telemetria periódica
   (`STATE` + `WEIGHT` + `TARGET`), ex. a cada **200–300 ms** (alinhado ao
   timer de UI de ~300 ms da tela de dosagem).
4. No simulador PC, esse padrão é o timer que chama `dosing_controller_update()`
   e relê peso — comportamento idêntico a partir do espelho na tela.

---

### 5. Regras de tempo e erro

- **Timeout**: se a tela não receber telemetria por `LINK_TIMEOUT_MS`
  (ex. `500 ms`), exibir "SEM COMUNICACAO" e **não** permitir novos comandos
  exceto `CANCEL` (a emergência física permanece independente, no externo).
- **Perda de sincronização (CRC)**: descartar o frame e re-sincronizar no
  próximo `0xAA`; nunca interpretar dados parcialmente.
- **`ERROR`**: o externo envia `ERROR` + estado seguro; a tela reflete a
  mensagem e bloqueia ação até novo estado.

---

[voltar ao índice](../14-external-esp32-communication.md)