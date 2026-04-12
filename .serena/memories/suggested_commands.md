# Comandi suggeriti

## Build
```bash
./build.sh          # compila nas.efi (root progetto)
# Internamente: cd src && make bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe && cp bin-x86_64-efi/ipxe.efi ../nas.efi
```
Dipendenze build: GCC, GNU Make, Perl, OpenSSL.

## Deploy
```bash
# Router OpenWrt (TFTP) — deploy principale
# Da LAN:
scp nas.efi root@192.168.1.254:/tftp/nas.efi
# Da remoto:
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi

# PC casa (boot locale, opzionale)
scp -P 22222 nas.efi marco@home.ziliani.net:/tmp/nas.efi
ssh -t -p 22222 marco@home.ziliani.net "sudo mv /tmp/nas.efi /boot/"
```

## NAS Synology
```bash
# SSH al NAS
ssh -p 2222 marco@home.ziliani.net

# Aggiornare ipxe-arch.efi (Arch Linux netboot)
wget https://archlinux.org/static/netboot/ipxe-arch.efi -O /tmp/ipxe-arch.efi
rsync -avP /tmp/ipxe-arch.efi -e "ssh -p 2222" marco@home.ziliani.net:/volume1/web/archlinux/
```

## Workflow tipico
1. Modificare `src/menu.ipxe`
2. `./build.sh`
3. `scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi`
