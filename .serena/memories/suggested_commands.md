# Comandi suggeriti

## Build
```bash
./build.sh          # compila nas.efi + netboot.xyz.efi (root progetto)
# Internamente (NB: target ipxe.efi FULL-DRIVER, NON snponly):
#   cd src && make -j$(nproc) bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe && cp ... ../nas.efi
#   make -j$(nproc) bin-x86_64-efi/ipxe.efi EMBED=netboot-xyz.ipxe && cp ... ../netboot.xyz.efi
# Output finale: nas.efi nella root (NON src/bin-x86_64-efi/...)
# config/local/usb.h: USB_KEYBOARD ABILITATO (tastiera USB nativa iPXE)
# ipxe.efi full-driver → boota sia via UEFI PXE sia da disco locale (systemd-boot)
```
Dipendenze build: GCC, GNU Make, Perl, OpenSSL.

## Deploy — DUE destinazioni (allinearle sempre entrambe!)
```bash
# 1) Router OpenWrt (TFTP) — boot via UEFI PXE. Da LAN:
scp nas.efi root@192.168.1.254:/tftp/nas.efi
# Da remoto:
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi

# 2) Disco locale pc-casa (systemd-boot) — boot da /boot. sudo NOPASSWD ok:
sudo cp nas.efi /boot/nas.efi
sudo cp netboot.xyz.efi /boot/netboot.xyz.efi   # se cambiato
# Voce systemd-boot: /boot/loader/entries/nas-ipxe.conf (efi /nas.efi)

# 3) (verifica) confronto hash tra build, router e /boot:
sha256sum nas.efi; ssh root@192.168.1.254 'sha256sum /tftp/nas.efi'; sudo sha256sum /boot/nas.efi
```

## NAS Synology — immagini in /volume1/web/
```bash
ssh -p 2222 nas.lan
# Cartelle servite via HTTP, una per tool (clonezilla, rescuezilla, macrium,
# synomedia, sysresccd, hirens, strelec, minitool, easyuefi, archiso, wimboot).
# Arch ISO local (archiso): vedi mem:tool_archiso_local
# NB: la cartella archlinux/ (ipxe-arch.efi netboot online) è stata RIMOSSA
# (2026-06-24) — il netboot Arch online si fa da netboot.xyz.
```

## Workflow tipico
1. Modificare `src/menu.ipxe`
2. `./build.sh`
3. Deploy su ENTRAMBE le destinazioni: `scp` sul router + `sudo cp` su /boot
   (anche solo per un commento: il menu è embedded, il binario cambia)
4. Verifica hash + test boot reale
