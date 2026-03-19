# Product

iPXE custom con menu boot di rete per recovery e diagnostica.

## Output

`nas.efi` — bootloader UEFI x86_64 con menu embedded (`src/menu.ipxe`).

## Infrastruttura

- Router OpenWrt (`firewall.ziliani.net`) → TFTP server, serve `nas.efi` ai client PXE
- NAS Synology (`192.168.1.1`) → HTTP server, serve immagini boot in `/volume1/web/`
- Il NAS NON ospita nas.efi

## Tool disponibili

| Tipo | Tool |
|------|------|
| Backup/Recovery | Clonezilla, Rescuezilla, Macrium, Synology Recovery |
| System Tools | System Rescue CD, Hiren's, Strelec 10/11, MiniTool, EasyUEFI |
| Network Boot | Arch Linux (locale), netboot.xyz (remoto) |

Menu timeout: 30s, default: Clonezilla.
