# Voce menu: Arch Linux ISO locale (archiso via HTTP)

## Scopo
Far partire l'ISO di Arch (~1,3 GB) dai file estratti sul NAS via HTTP, alternativa
VELOCE al netboot remoto `:archlinux` (che scarica dall'online ed è lento).
Entrambe le voci coesistono nel menu (sezione *Network Boot & Install*):
- `a` archlinux → `ipxe-arch.efi`, online, sempre ultima release
- `i` archiso   → archiso HTTP dal NAS, versione fissa locale

## Voce di menu (src/menu.ipxe) — GENERICA
```ipxe
item --key i archiso     Arch Linux ISO (local, from NAS)

:archiso
set base-url http://${server-ip}/archiso
kernel ${base-url}/arch/boot/x86_64/vmlinuz-linux initrd=initramfs-linux.img archiso_http_srv=${base-url}/ archisobasedir=arch checksum ip=dhcp net.ifnames=0 BOOTIF=01-${net0/mac:hexhyp}
initrd ${base-url}/arch/boot/x86_64/initramfs-linux.img
boot || goto failed
```
È generica perché i nomi file archiso sono FISSI tra le release → per aggiornare
basta sovrascrivere i file sul NAS, NON serve ricompilare nas.efi.
Testato OK in VMware (2026-06-24).

## ⚠️ Parametri di rete OBBLIGATORI (entrambi!)
Servono TUTTI E DUE, derivati dallo script ufficiale Arch (releng/netboot,
extrabootoptions = `ip=dhcp net.ifnames=0 BOOTIF=01-${netX/mac}`):
- `net.ifnames=0` → l'interfaccia resta `eth0` (altrimenti udev la rinomina enpXsY).
- `BOOTIF=01-${net0/mac:hexhyp}` → dice all'hook archiso_pxe_common QUALE
  interfaccia usare, per MAC (formato con trattini: 01-aa-bb-cc-dd-ee-ff;
  `:hexhyp` è il modificatore iPXE che mette i trattini).
SENZA BOOTIF (anche con net.ifnames=0) il boot fallisce: eth0 viene rilevato ma
l'hook non lo mappa → loop infinito `SIOCGIFFLAGS: No such device`, airootfs.sfs
non si scarica mai. Verificato su fisico (Intel i9-14900K) e in VMware.
NB: errori `nouveau [drm] *ERROR*` e `usb ... error -71` nello stesso schermo
sono rumore innocuo (GPU/USB), non c'entrano col fallimento.

## File necessari sul NAS (/volume1/web/archiso/) — SOLO questi 4
- arch/boot/x86_64/vmlinuz-linux        (kernel, caricato da iPXE)
- arch/boot/x86_64/initramfs-linux.img  (initramfs, microcode già incluso)
- arch/x86_64/airootfs.sfs              (rootfs ~1 GB, scaricato da archiso)
- arch/x86_64/airootfs.sha512           (checksum, usato da param `checksum`)
Più: VERSION.txt (versione ISO corrente).
NB: l'archiso recente NON ha file microcode separati (intel/amd-ucode).
Tutto il resto dell'ISO (EFI/, boot/syslinux, loader/, memtest, shell efi,
grubenv, pkglist, version, .cms.sig) serve solo al boot da CD/USB → NON copiarlo.
NON abilitare cms_verify (manca .cms.sig sul NAS); `checksum` invece OK (c'è .sha512).

## AGGIORNARE archiso DA UNA NUOVA ISO (NIENTE ricompila nas.efi)
Le nuove ISO mensili di Arch stanno su Ventoy: /run/media/marco/Ventoy/archlinux-*.iso
I nomi file dentro l'ISO sono fissi → basta sovrascrivere i 4 file sul NAS.
Da pc-casa (NAS via ssh -p 2222 nas.lan). Script completo copia-incolla
(estrae dall'ISO, streamma sul NAS senza staging locale, aggiorna VERSION.txt,
verifica il checksum):
```bash
# 1) imposta il path dell'ISO nuova (unica riga da cambiare)
ISO=/run/media/marco/Ventoy/archlinux-2026.07.01-x86_64.iso

DST=/volume1/web/archiso
VER=$(basename "$ISO" | sed -E 's/archlinux-(.*)-x86_64\.iso/\1/')
# 2) stream dei 4 file dall'interno dell'ISO direttamente sul NAS
for f in arch/boot/x86_64/vmlinuz-linux arch/boot/x86_64/initramfs-linux.img \
         arch/x86_64/airootfs.sfs arch/x86_64/airootfs.sha512; do
  echo ">> $f"
  bsdtar xOf "$ISO" "$f" | ssh -p 2222 nas.lan "cat > $DST/$f"
done
# 3) aggiorna VERSION.txt sul NAS
ssh -p 2222 nas.lan "printf 'Arch Linux ISO: %s\nSource: %s\nUpdated: %s\n' \
  '$VER' '$(basename "$ISO")' '$(date +%F)' > $DST/VERSION.txt"
# 4) verifica integrità (deve dire: airootfs.sfs: OK)
ssh -p 2222 nas.lan "cd $DST/arch/x86_64 && sha512sum -c airootfs.sha512"
```
Dopo: ritestare il boot reale della voce `i` (PXE o systemd-boot). Nessun
rebuild di nas.efi necessario, a meno che cambino i percorsi dei file archiso.

## Storico
- 2026-06-24: prima versione, ISO 2026.06.01. Cartella ripulita da estrazione
  completa vecchia (mar 2025), ridotta ai 4 file.
- 2026-06-24: fix boot #1 — aggiunto net.ifnames=0. NON bastava (loop persisteva).
- 2026-06-24: fix boot #2 — aggiunto BOOTIF=01-${net0/mac:hexhyp} (lo richiede
  l'hook archiso_pxe_common per mappare l'interfaccia). cmdline ora = quella
  ufficiale Arch. Deploy su router + /boot. Testato OK in VMware.
