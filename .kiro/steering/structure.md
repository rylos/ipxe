# Struttura Progetto

## Root

- `build.sh` — script build
- `nas.efi` — output finale (usare sempre questo, NON `src/bin-x86_64-efi/ipxe.efi`)
- `SETUP.md` — documentazione tecnica deploy

## src/

- `menu.ipxe` — menu boot iPXE (file chiave, embedded in nas.efi)
- `menu.ipxe.bak` — backup menu
- `config/general.h` — configurazione feature
- `config/defaults/` — default per piattaforma
- `config/local/` — customizzazioni locali
- `core/` — funzionalità core iPXE
- `net/` — protocolli di rete (TCP, UDP, DHCP, etc.)
- `crypto/` — crittografia
- `drivers/net/` — driver schede di rete
- `hci/commands/` — comandi shell iPXE
- `arch/` — codice specifico per architettura (x86, x86_64, ARM, RISC-V)

## File da modificare (in ordine di frequenza)

1. `src/menu.ipxe` — aggiungere/modificare voci boot
2. `SETUP.md` — aggiornare documentazione
3. `src/config/general.h` — abilitare/disabilitare feature
4. `build.sh` — modificare processo di build
