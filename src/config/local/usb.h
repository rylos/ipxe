/*
 * Tastiera USB nativa iPXE in modalità EFI.
 *
 * Regressione upstream: il commit ce6f574a9 (2026-01-16, "Canonicalise
 * USB configuration") disabilita USB_KEYBOARD su PLATFORM_efi, delegando
 * la tastiera al firmware (ConIn/CONSOLE_EFI). Ma quando iPXE prende il
 * controllo di XHCI per i driver di rete nativi, il firmware perde la
 * tastiera USB (Intel 14th gen + AMI) -> nessuno la gestisce.
 *
 * Fix: riabilitare il driver tastiera USB nativo di iPXE mantenendo i
 * driver HCD nativi (XHCI/EHCI/UHCI). Cosi' iPXE gestisce direttamente
 * la tastiera, com'era prima del commit. Va buildato come ipxe.efi
 * (driver NIC nativi inclusi) per funzionare sia via PXE sia da disco
 * locale (systemd-boot chainload).
 *
 * NB: la combinazione USB_HCD_USBIO + USB_KEYBOARD (tentata in passato)
 * NON funziona su questo firmware: usare i driver HCD nativi, non USBIO.
 */

#define USB_KEYBOARD	/* driver tastiera USB nativo iPXE */
