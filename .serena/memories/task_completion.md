# Cosa fare al completamento di un task

## Dopo modifiche a src/menu.ipxe
1. `./build.sh` — ricompila nas.efi
2. Deploy su router (da LAN):
   - `scp nas.efi root@192.168.1.254:/tftp/nas.efi` — UEFI
   - `scp src/menu.ipxe root@192.168.1.254:/tftp/menu.ipxe` — BIOS
3. Testare il boot da un client PXE

## Dopo modifiche a src/config/general.h
1. `./build.sh` — ricompila nas.efi
2. Deploy come sopra

## Non c'è linting/formatting automatico
Il progetto non ha test automatici né linter. La verifica è manuale tramite boot reale.

## Aggiornamento documentazione
Aggiornare `SETUP.md` se si aggiungono nuovi tool o si cambiano procedure.
