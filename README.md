# iPXE Boot Menu

Menu iPXE personalizzato per boot di rete con tool di recovery e diagnostica.

## Architettura

```
Client UEFI → DHCP (router OpenWrt) → TFTP nas.efi (router) → HTTP immagini (NAS Synology)
```

## Quick Start

```bash
# Build
./build.sh

# Deploy su router
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi
```

## Tool

| Categoria | Tool |
|-----------|------|
| Backup & Recovery | Clonezilla, Rescuezilla, Macrium, Synology Recovery |
| System Tools | System Rescue CD, Hiren's, Strelec 10/11, MiniTool, EasyUEFI |
| Network Boot | Arch Linux, netboot.xyz |

## File principali

| File | Descrizione |
|------|-------------|
| `src/menu.ipxe` | Menu boot (embedded in nas.efi) |
| `src/config/general.h` | Configurazione feature |
| `build.sh` | Script di compilazione |
| `SETUP.md` | Documentazione tecnica completa |

## Documentazione

Vedi [SETUP.md](SETUP.md) per deploy, configurazione server, aggiunta tool e troubleshooting.
