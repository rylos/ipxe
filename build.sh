#!/bin/sh
# Build iPXE EFI con menu personalizzato embedded
#
# Output: nas.efi (nella root del progetto)
# Il file è un bootloader EFI x86_64 con il menu iPXE integrato.
#
# Deployment:
#   - OpenWRT: copiare nas.efi nella directory TFTP del router (es. /tftp/)
#   - Arch Linux (systemd-boot): copiare nas.efi nella ESP (es. /boot/)
#     e creare una entry in /boot/loader/entries/

cd src
make bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe
cp bin-x86_64-efi/ipxe.efi ../nas.efi
cd ..

echo ""
echo "Build completato: nas.efi"
