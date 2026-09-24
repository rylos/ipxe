# netboot.xyz — build nostra, NON il binario ufficiale

`netboot.xyz.efi` (root progetto, deploy in `/boot/netboot.xyz.efi`, voce systemd-boot
`netboot-xyz.conf` → `efi /netboot.xyz.efi`) è la NOSTRA `ipxe.efi` con embedded
`src/netboot-xyz.ipxe`. Lo script fa `dhcp` e poi `chain https://boot.netboot.xyz/menu.ipxe`:
il menu è quindi sempre quello live, si aggiorna da solo. Del nostro c'è solo la base iPXE
(si aggiorna con merge upstream + `./build.sh`).

Perché non il binario ufficiale: ha USB_KEYBOARD disabilitato → tastiera morta da disco.

## Version check (IL punto da sorvegliare)
Il `menu.ipxe` di netboot.xyz fa `set latest_version 3.x` e `iseq ${version} ${latest_version}`:
se non combacia, fa chain al PROPRIO binario (senza usbkbd) → tastiera morta.
Il nostro script fa `set version 3.x` per passare il check.

Verificato 2026-09-24: live `latest_version 3.x`, `version.ipxe` → `upstream_version 3.x`,
ultima release GitHub 3.0.3 (2026-08-29). Tutto allineato.

Come ricontrollare:
```bash
curl -s https://boot.netboot.xyz/menu.ipxe | grep latest_version
curl -s https://boot.netboot.xyz/version.ipxe
```
Se diventa `4.x`: cambiare `set version` in `src/netboot-xyz.ipxe`, `./build.sh`,
`sudo cp netboot.xyz.efi /boot/netboot.xyz.efi`.

La pagina doc https://netboot.xyz/docs/booting/uefi/ parla solo del boot locale del .efi
(path convenzionale `/EFI/netboot.xyz/`), non dà versioni: non serve per questo controllo.
