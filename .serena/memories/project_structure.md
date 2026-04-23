# Struttura Progetto

## File chiave (in ordine di frequenza di modifica)
1. `src/menu.ipxe` — menu boot iPXE (embedded in nas.efi a compile-time)
2. `SETUP.md` — documentazione tecnica deploy
3. `src/config/general.h` — feature e timeout iPXE
4. `build.sh` — script build

## Root
- `build.sh` — script build
- `nas.efi` — output finale (usare SEMPRE questo, NON `src/bin-x86_64-efi/ipxe.efi`)
- `SETUP.md` — documentazione tecnica deploy

## src/
- `menu.ipxe` — menu boot (file principale da modificare)
- `menu.ipxe.bak` — backup menu
- `config/general.h` — configurazione feature
- `config/defaults/` — default per piattaforma
- `config/local/` — customizzazioni locali
- `core/` — funzionalità core iPXE
- `net/` — protocolli di rete (TCP, UDP, DHCP, etc.)
- `crypto/` — crittografia
- `drivers/net/` — driver schede di rete
- `hci/commands/` — comandi shell iPXE
- `arch/` — codice specifico per architettura

## NAS - /volume1/web/
```
wimboot                  # WinPE bootloader (≥ v2.7.3)
clonezilla/
rescuezilla/
sysresccd/
macrium/
synomedia/
minitool/
easyuefi/
hirens/
strelec/
  pecmdmain.ini          # Override PXE: forza pecmd.ini (profilo locale)
  network_10.cmd         # Override PXE per WinPE 10 (WaitForNetwork + retry mount Z: + logica WIM)
  network_11.cmd         # Override PXE per WinPE 11 (WaitForNetwork + retry mount Z: + logica WIM)
  bcd, boot.sdi, bootmgr64.exe, bootx64.efi  # File di boot wimboot
  SSTR/                  # Contenuto ISO (WIM, MInst, DriverPacks, ecc.)
archlinux/ipxe-arch.efi
```
