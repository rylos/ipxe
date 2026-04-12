# iPXE Boot Menu - Overview

## Scopo
Menu boot di rete personalizzato per recovery e diagnostica. Produce `nas.efi`, un bootloader UEFI x86_64 con menu iPXE embedded.

## Architettura
```
Client UEFI → DHCP (router OpenWrt) → TFTP nas.efi (router) → HTTP immagini (NAS Synology)
```
- Router OpenWrt (`firewall.ziliani.net`, porta SSH 44222) → TFTP server, ospita `nas.efi` in `/tftp/`
- NAS Synology (`192.168.1.1`, SSH `ssh -p 2222 marco@home.ziliani.net`) → HTTP server, ospita immagini in `/volume1/web/`
- Il NAS NON ospita `nas.efi`

## Tool disponibili nel menu
| Categoria | Tool |
|-----------|------|
| Backup/Recovery | Clonezilla (default), Rescuezilla, Macrium, Synology Recovery |
| System Tools | System Rescue CD, Hiren's, Strelec 10/11, MiniTool, EasyUEFI |
| Network Boot | Arch Linux (locale), netboot.xyz (remoto) |

Timeout: 30s, default: Clonezilla.

## Variabili globali menu
```ipxe
set server-ip 192.168.1.1
set nas-ip ${server-ip}
set wimboot-url http://${server-ip}/wimboot
```
