#!/bin/sh
cd src
make bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe
cp bin-x86_64-efi/ipxe.efi ../nas.efi
cd ..
