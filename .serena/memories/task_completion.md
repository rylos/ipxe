# Cosa fare al completamento di un task

## Dopo modifiche a src/menu.ipxe (o src/config/*)
1. `./build.sh` — ricompila nas.efi (+ netboot.xyz.efi).
   NB: build.sh passa `NO_WERROR=1`, necessario con binutils >= 2.44 / GCC >= 15
   (warning `.note.GNU-stack` nei .S upstream + `ASFLAGS += --fatal-warnings`).
2. Deploy su TUTTE E TRE le destinazioni:
   - Router OpenWrt (UEFI PXE): `scp nas.efi root@192.168.1.254:/tftp/nas.efi`
   - Disco locale (systemd-boot): `sudo cp nas.efi /boot/nas.efi`
   - Router, menu BIOS legacy: `scp src/menu.ipxe root@192.168.1.254:/tftp/menu.ipxe`
   NB: nas.efi è ipxe.efi full-driver → boota da UEFI PXE e da disco. Se aggiorni
   solo il router, la voce systemd-boot locale resta col menu VECCHIO — e se scordi
   `/tftp/menu.ipxe`, i client BIOS legacy restano indietro (build.sh NON lo tocca:
   non è embedded in nessun binario).
3. Verifica allineamento: sha256sum (build vs /tftp/nas.efi vs /boot/nas.efi)
   + md5sum (src/menu.ipxe vs /tftp/menu.ipxe)
4. Testare il boot reale da un client PXE / da systemd-boot

## Non c'è linting/formatting automatico
Nessun test automatico né linter. Verifica solo via boot reale.

## Aggiornamento documentazione/memorie
Aggiornare `SETUP.md` per nuovi tool/procedure. Per la voce archiso vedi
`mem:tool_archiso_local`. nas.efi è full-driver (NON snponly), USB_KEYBOARD attivo.
