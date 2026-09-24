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
| Backup/Recovery | Clonezilla (default), **Clonezilla jumbo**, Rescuezilla, Macrium, Synology Recovery |
| System Tools | System Rescue CD, Hiren's, Strelec 10/11, MiniTool, EasyUEFI |
| Network Boot | Arch ISO local (archiso da NAS), netboot.xyz |

Vedi `mem:tool_netboot_xyz` per il version check di netboot.xyz (`set version 3.x`).

Timeout: 30s, default: Clonezilla. Vedi `mem:tool_archiso_local` per la voce archiso.

## Clonezilla: due voci, standard e jumbo (2026-08-03)
`:clonezilla` (tasto k) monta la destinazione via **NFS**, non SMB:
`ocs_prerun="mount -t nfs 192.168.1.1:/volume2/backup /home/partimag"`. MTU 1500
(il DHCP OpenWrt non manda l'option 26).

`:clonezilla_jumbo` (tasto j) e' identica ma alza la MTU a 9000 via `ocs_prerun`,
spostando il mount su `ocs_prerun1` (eseguiti in ordine numerico).

**Entrambe** hanno `net.ifnames=0` (dal 2026-08-03) -> interfaccia `eth0`
deterministica su qualsiasi macchina. E' anche il default ufficiale Clonezilla live
e lo usa gia' `:archiso`. Nessuna funzionalita' dipende dal nome (il mount NFS usa
l'IP): serve per prevedibilita' quando si diagnostica da `Enter_shell`.
Verificato empiricamente: senza il flag l'interfaccia su pc-casa era `enp4s0`.
Guadagno atteso ~5% (efficienza 94% -> 99%); il link 2.5G e' gia' saturo a 1500.

✅ **Verificata funzionante su pc-casa il 2026-08-03**: MTU 9000 confermata a runtime
dentro Clonezilla. `net.ifnames=0` fa il suo lavoro (interfaccia = `eth0`).

⚠️ La voce jumbo va usata SOLO dove i jumbo sono verificati end-to-end. Il menu e'
condiviso con il laptop: se la NIC accetta 9000 ma lo switch non li inoltra, il mount
NFS si pianta. Verifica da shell Clonezilla (`Enter_shell`), tutto in RAM:
```bash
ip link                                   # nome interfaccia + MTU reale
sudo ip link set dev NOME mtu 9000
ping -c3 -M do -s 8972 192.168.1.1        # se risponde, i jumbo passano
```
Perf misurata pc-casa <-> NAS (2026-08-03): iperf3 2448/2477 Mbit/s, SMB 301/308 MB/s
= line rate 2.5G. Collo di bottiglia del backup NON e' la rete (zstd -z9p multi-thread
e NVMe sono piu' veloci del link).
NB: la vecchia voce "Arch Linux netboot (ipxe-arch.efi online)" è stata RIMOSSA
(2026-06-24): il netboot Arch online si fa già da netboot.xyz. La cartella NAS
`/volume1/web/archlinux/` non è più referenziata dal menu.

## Variabili globali menu
```ipxe
set server-ip 192.168.1.1
set nas-ip ${server-ip}
set wimboot-url http://${server-ip}/wimboot
console --x 1024 --y 768 ||    # 128x48 caratteri invece di 80x25
```

## Console 1024x768: perche' (2026-08-03)
Il menu ha 21 righe (17 voci + 4 separatori) + titolo + indicatore di scroll = 23,
mentre la console EFI di default (80x25) ne lascia ~20 -> `Exit iPXE` finiva in
seconda pagina. `console --x 1024 --y 768` porta a 128x48.

Il `||` finale e' essenziale ed e' best-effort per DUE motivi:
1. se il firmware rifiuta la modalita' video, si resta in 80x25 come prima;
2. sui client **BIOS legacy** il comando `console` NON ESISTE: `config/general.h`
   fa `#undef CONSOLE_CMD` per `PLATFORM_pcbios`. Senza `||` lo script si
   fermerebbe li'. `console_cmd.o` e' linkato solo nel build EFI.
Richiede `CONSOLE_FRAMEBUFFER`, gia' attivo in `config/console.h`.

Se in futuro si aggiungono voci: a 48 righe c'e' margine per ~25 in piu'.

## Merge upstream 2026-09-24: AGENTS.md -> FORK.md
Upstream (0f4a37bc3) ha aggiunto AGENTS.md, CLAUDE.md e .claude/skills/ipxe-security-review.
Le istruzioni del fork sono state spostate in `FORK.md`; `AGENTS.md` resta quello upstream
con una sola riga in cima che rimanda a FORK.md (per ridurre i conflitti nei merge futuri).
Nei merge futuri: se AGENTS.md va in conflitto, prendere la versione upstream e rimettere la riga.
Remote `upstream` = https://github.com/ipxe/ipxe.git.
