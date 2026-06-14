#!/bin/sh
# Build iPXE EFI con menu personalizzato embedded
#
# Output (nella root del progetto):
#   - nas.efi          menu NAS completo (EMBED=menu.ipxe)
#   - netboot.xyz.efi  launcher netboot.xyz (EMBED=netboot-xyz.ipxe)
#
# Entrambi: target ipxe.efi (driver NIC nativi) -> bootano sia via UEFI PXE
# sia da disco locale (systemd-boot). Tastiera USB nativa iPXE
# (config/local/usb.h, USB_KEYBOARD). Il binario UFFICIALE netboot.xyz.efi
# NON va usato da disco: ha USB_KEYBOARD disabilitato -> tastiera morta.
#
# Deployment:
#   - OpenWrt/TFTP:  scp nas.efi root@192.168.1.254:/tftp/nas.efi
#   - Disco locale:  sudo cp nas.efi /boot/nas.efi
#                    sudo cp netboot.xyz.efi /boot/netboot.xyz.efi

set -e

cd src
make -j$(nproc) bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe
cp bin-x86_64-efi/ipxe.efi ../nas.efi

make -j$(nproc) bin-x86_64-efi/ipxe.efi EMBED=netboot-xyz.ipxe
cp bin-x86_64-efi/ipxe.efi ../netboot.xyz.efi
cd ..

echo ""
echo "Build completato: nas.efi + netboot.xyz.efi (ipxe.efi full-driver)"
