# Tech Stack

## Build

```bash
./build.sh          # compila e copia in nas.efi (root progetto)
```

Internamente: `make bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe` in `src/`.

Dipendenze: GCC, GNU Make, Perl, OpenSSL.

## Deploy

```bash
# Router OpenWrt (TFTP)
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi

# PC casa (boot locale, opzionale)
scp -P 22222 nas.efi marco@home.ziliani.net:/tmp/nas.efi
ssh -t -p 22222 marco@home.ziliani.net "sudo mv /tmp/nas.efi /boot/"
```

Il NAS (192.168.1.1) serve solo immagini via HTTP, non ospita nas.efi.

## Flusso PXE

Client UEFI → DHCP (router) → TFTP nas.efi (router) → HTTP immagini (NAS)

## Config chiave

- `src/menu.ipxe` — menu boot (embedded a compile-time)
- `src/config/general.h` — feature e timeout
- `build.sh` — script build
- `SETUP.md` — documentazione deploy completa
