# Comandi suggeriti

## Build
```bash
./build.sh          # compila nas.efi (root progetto)
# Internamente: cd src && make bin-x86_64-efi/snponly.efi EMBED=menu.ipxe && cp ... ../nas.efi
# Override USB: src/config/local/usb.h (tutti i driver USB disabilitati per fix tastiera UEFI)
# NOTA: snponly.efi funziona SOLO via network boot, non da disco locale
```
Dipendenze build: GCC, GNU Make, Perl, OpenSSL.

## Deploy
```bash
# Router OpenWrt (TFTP) — unico deploy (no boot locale)
# Da LAN:
scp nas.efi root@192.168.1.254:/tftp/nas.efi
# Da remoto:
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi
```

## NAS Synology
```bash
# SSH al NAS
ssh -p 2222 nas.lan

# Aggiornare ipxe-arch.efi (Arch Linux netboot)
wget https://archlinux.org/static/netboot/ipxe-arch.efi -O /tmp/ipxe-arch.efi
rsync -avP /tmp/ipxe-arch.efi -e "ssh -p 2222" nas.lan:/volume1/web/archlinux/
```

## Workflow tipico
1. Modificare `src/menu.ipxe`
2. `./build.sh`
3. `scp nas.efi root@192.168.1.254:/tftp/nas.efi`
