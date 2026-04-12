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
menu.ipxe      # menu standalone legacy (5K, non più usato attivamente)
```

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
