# Cosa fare al completamento di un task

## Dopo modifiche a src/menu.ipxe (o src/config/*)
1. `./build.sh` — ricompila nas.efi (+ netboot.xyz.efi)
2. Deploy su ENTRAMBE le destinazioni:
   - Router OpenWrt (UEFI PXE): `scp nas.efi root@192.168.1.254:/tftp/nas.efi`
   - Disco locale (systemd-boot): `sudo cp nas.efi /boot/nas.efi`
   NB: nas.efi è ipxe.efi full-driver → boota da entrambe. Se aggiorni solo il
   router, la voce systemd-boot locale resta col menu VECCHIO.
3. Verifica allineamento via sha256sum (build vs /tftp vs /boot)
4. Testare il boot reale da un client PXE / da systemd-boot

## Non c'è linting/formatting automatico
Nessun test automatico né linter. Verifica solo via boot reale.

## Aggiornamento documentazione/memorie
Aggiornare `SETUP.md` per nuovi tool/procedure. Per la voce archiso vedi
`mem:tool_archiso_local`. nas.efi è full-driver (NON snponly), USB_KEYBOARD attivo.
