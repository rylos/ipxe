# Router OpenWrt

## Hardware & Sistema
- OpenWrt 25.12.2 (kernel 6.12.74)
- Target: mediatek/mt7622 (aarch64_cortex-a53)
- Overlay: 54.4M totale, ~43M liberi
- IP LAN: 192.168.1.254/24 (br-lan)
- SSH: porta 22, utente root
- DDNS: firewall.ziliani.net, porta SSH esterna 44222

## TFTP / PXE Boot
Root TFTP: `/tftp/`
```
nas.efi        # iPXE UEFI bootloader con menu embedded (1.1M)
undionly.kpxe  # iPXE BIOS chainloader (68K)
menu.ipxe      # menu standalone per client BIOS legacy — VA TENUTO ALLINEATO
```

⚠️ `/tftp/menu.ipxe` NON è "non più usato": è il menu servito ai client **BIOS
legacy** (`dhcp-boot=tag:bios,tag:ipxe,menu.ipxe`). NON è embedded in nessun
binario, quindi `./build.sh` non lo aggiorna: va copiato a mano da `src/menu.ipxe`
a OGNI modifica del menu, altrimenti i client BIOS restano indietro.

Trovato disallineato il 2026-08-03 (fermo al 22 giugno) con voci **rotte**:
`:archlinux` puntava a `/archlinux/ipxe-arch.efi` (cartella rimossa dal NAS),
`:netboot` caricava il binario .efi invece dello script (tastiera USB morta),
`:strelec` usava `network.cmd` invece di `${strelec-netcmd}`.
Backup del vecchio: `/tftp/menu.ipxe.bak-2026-08-03`.

NB: `undionly.kpxe` è del 2025-03 e potrebbe non avere TLS compilato → la voce
`:netboot` (che ora usa `https://boot.netboot.xyz/menu.ipxe`) può fallire su BIOS.
Le voci EFI-only (wimboot/`bootx64.efi`) non funzionano da BIOS a prescindere.

## Configurazione dnsmasq PXE
```
dhcp-match=set:ipxe,175                          # detect iPXE client
dhcp-boot=tag:bios,tag:!ipxe,undionly.kpxe       # BIOS non-iPXE → chainload
dhcp-boot=tag:!bios,tag:!ipxe,nas.efi            # UEFI non-iPXE → nas.efi (menu embedded)
dhcp-boot=tag:bios,tag:ipxe,menu.ipxe            # BIOS iPXE → menu script
# UEFI iPXE non ha regola → il menu è già embedded in nas.efi
```

TFTP abilitato in `/etc/config/dhcp` con `tftp_root '/tftp'`.

## Deploy da LAN
```bash
scp nas.efi root@192.168.1.254:/tftp/nas.efi
```

## Deploy da remoto
```bash
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi
```
