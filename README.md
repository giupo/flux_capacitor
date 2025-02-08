# Flux Capacitor
Questo progetto contiene il codice di controllo del flusso canalizzatore di quel frocio di Davide

## Come compilare
### Requisiti
1. Avere python installato (https://www.python.org/). Io ho usato 3.12.3, ma credo qualunque versione vada bene se compatibile con PlatformIO
2. PlatformIO (https://platformio.org/), che installeremo in un virtualenv per non imputtanarsi il sistema.

### Setup ambientre di sviluppo
1. Aprire il prompt dei comandi/shell e dare:

``` sh
python -m virtualenv .venv
```

Questo crea un ambiente virtuale di Python dove installare Pio. Attivare il virtual env con:

#### Linux
``` sh
source .venv/bin/activate
```

#### Windows

``` sh
.venv\Scripts\activate
```

Ora diamo:

``` sh
pip install platformio
```
