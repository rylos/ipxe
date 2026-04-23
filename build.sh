#!/bin/sh
# Build iPXE EFI con menu personalizzato embedded
#
# Output: nas.efi (nella root del progetto)
# Il file è un bootloader EFI x86_64 con il menu iPXE integrato.
#
# Deployment:\n#   scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi\n#\n# NOTA: snponly.efi funziona solo via network boot (UEFI PXE).\n#       Non può essere avviato da disco locale (systemd-boot).

set -e

cd src
make -j$(nproc) bin-x86_64-efi/snponly.efi EMBED=menu.ipxe
cp bin-x86_64-efi/snponly.efi ../nas.efi
cd ..

echo ""
echo "Build completato: nas.efi"
