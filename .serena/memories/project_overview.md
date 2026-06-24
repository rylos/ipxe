# iPXE Boot Menu - Overview

## Scopo
Menu boot di rete personalizzato per recovery e diagnostica. Produce `nas.efi`, un
bootloader UEFI x86_64 con menu iPXE embedded.

## IMPORTANTE: nas.efi è ipxe.efi FULL-DRIVER (NON snponly.efi)
`build.sh` compila il target `ipxe.efi` con driver NIC nativi inclusi, NON
`snponly.efi`. Conseguenza: `nas.efi` boota sia via **UEFI PXE** (dal router) sia
**da disco locale** (systemd-boot, `/boot/nas.efi`), perché ha i propri driver di
rete e non dipende dall'SNP del firmware. snponly.efi NON è più usato.

## Tastiera USB: USB_KEYBOARD ABILITATO (NON disabilitato)
`src/config/local/usb.h` definisce `USB_KEYBOARD` per riabilitare il driver
tastiera USB nativo di iPXE (driver HCD nativi XHCI/EHCI/UHCI). Necessario perché
l'upstream commit ce6f574a9 (2026-01) disabilita USB_KEYBOARD su EFI delegando al
firmware, ma su Intel 14th gen + AMI la tastiera USB muore quando iPXE prende XHCI.
NB: USB_HCD_USBIO + USB_KEYBOARD NON funziona su questo firmware → usare HCD nativi.

## Architettura
```
Client UEFI → DHCP (router OpenWrt) → TFTP nas.efi (router) → HTTP immagini (NAS)
            └─ oppure → systemd-boot → /boot/nas.efi (stesso menu, da disco)
```
- Router OpenWrt (`firewall.ziliani.net`, SSH 44222) → TFTP server, ospita `nas.efi` in `/tftp/`
- NAS Synology (`192.168.1.1`, SSH `ssh -p 2222 nas.lan`) → HTTP server, immagini in `/volume1/web/`
- Il NAS NON ospita `nas.efi`
- Boot locale: `/boot/nas.efi` su pc-casa, voce systemd-boot `nas-ipxe.conf`
  ("NAS iPXE (network boot)"). Va riallineato a mano dopo ogni build:
  `sudo cp nas.efi /boot/nas.efi` (sudo NOPASSWD ok su pc-casa).
- Strelec WinPE: usa `:boot_strelec` che inietta `network.cmd` via wimboot

## Tool disponibili nel menu
| Categoria | Tool |
|-----------|------|
| Backup/Recovery | Clonezilla (default), Rescuezilla, Macrium, Synology Recovery |
| System Tools | System Rescue CD, Hiren's, Strelec 10/11, MiniTool, EasyUEFI |
| Network Boot | Arch ISO local (archiso da NAS), netboot.xyz |

Timeout: 30s, default: Clonezilla. Vedi `mem:tool_archiso_local` per la voce archiso.
NB: la vecchia voce "Arch Linux netboot (ipxe-arch.efi online)" è stata RIMOSSA
(2026-06-24): il netboot Arch online si fa già da netboot.xyz. La cartella NAS
`/volume1/web/archlinux/` non è più referenziata dal menu.

## Variabili globali menu
```ipxe
set server-ip 192.168.1.1
set nas-ip ${server-ip}
set wimboot-url http://${server-ip}/wimboot
```
