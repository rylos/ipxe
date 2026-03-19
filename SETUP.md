# iPXE Boot Menu - Setup & Deploy

## Architettura

```
Client PXE (UEFI)
  │
  ├─ DHCP → Router OpenWrt (firewall.ziliani.net)
  │           opzioni PXE: next-server + filename
  │
  ├─ TFTP → Router OpenWrt (/tftp/nas.efi)
  │           bootloader iPXE con menu embedded
  │
  └─ HTTP → NAS Synology (192.168.1.1)
              /volume1/web/{tool}/  ← immagini boot
```

## Build & Deploy

```bash
# Compilazione
./build.sh                # → nas.efi (root progetto)

# Deploy su router OpenWrt
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi

# Deploy su PC casa (opzionale, per boot locale)
scp -P 22222 nas.efi marco@home.ziliani.net:/tmp/nas.efi
ssh -t -p 22222 marco@home.ziliani.net "sudo mv /tmp/nas.efi /boot/"
```

## Server Web - NAS Synology (192.168.1.1)

SSH: `ssh -p 2222 marco@home.ziliani.net`

```
/volume1/web/
├── wimboot                  # WinPE bootloader (≥ v2.7.3)
├── clonezilla/              # Clonezilla Live
├── rescuezilla/             # Rescuezilla
├── sysresccd/               # System Rescue CD
├── macrium/                 # Macrium Recovery (WinPE)
├── synomedia/               # Synology Recovery (WinPE)
├── minitool/                # MiniTool Partition Wizard (WinPE)
├── easyuefi/                # EasyUEFI Enterprise (WinPE)
├── hirens/                  # Hiren's BootCD PE (WinPE)
├── strelec/                 # Sergei Strelec WinPE 10/11
└── archlinux/ipxe-arch.efi  # Arch Linux netboot
```

## Menu iPXE

File: `src/menu.ipxe` (embedded in nas.efi durante la build)

Variabili globali:

```ipxe
set server-ip 192.168.1.1
set nas-ip ${server-ip}
set wimboot-url http://${server-ip}/wimboot
```

Timeout: 30s, default: Clonezilla.

### Voci menu

| Categoria | Tool | Tipo |
|-----------|------|------|
| Backup & Recovery | Clonezilla, Rescuezilla, Macrium, Synology Recovery | Linux / WinPE |
| System Tools | System Rescue CD, Hiren's, Strelec 10/11, MiniTool, EasyUEFI | Linux / WinPE |
| Network Boot | Arch Linux, netboot.xyz | Chain |

## Struttura progetti boot

### WinPE standard (macrium, synomedia)

```
project/
├── boot/
│   ├── BCD (o bcd)
│   └── boot.sdi
└── sources/
    └── boot.wim
```

Usano la funzione `boot_winpe` nel menu. Attenzione al case di BCD/bcd (varia per progetto).

### WinPE custom (hirens, easyuefi, strelec)

Richiedono file aggiuntivi (bootmgr, bootx64.efi, ecc.). Ogni progetto ha la sua sezione dedicata nel menu.

### Linux (clonezilla, sysresccd, rescuezilla)

Kernel + initrd + parametri specifici. Clonezilla monta NFS `${nas-ip}:/volume2/backup` su `/home/partimag`.

## Strelec - Note speciali

Struttura con file duplicati obbligatori:

```
strelec/
├── bootx64.efi, bootmgr64.exe, bcd, boot.sdi, network.cmd
└── SSTR/
    ├── bootx64.efi, bootmgr64.exe, bcd, boot.sdi  ← duplicati identici
    ├── strelec10x64Eng.wim
    └── strelec11x64Eng.wim
```

I file boot in root e SSTR/ devono avere hash identici. Verificare con `md5sum`.

`network.cmd` monta `\\192.168.1.1\web\strelec` (user: `web`, pass: `0jSpjMjZT0a0oG`) e configura rete/WiFi.

### Aggiornamento Strelec

1. Copiare nuova versione in directory temporanea
2. Copiare file boot dalla vecchia versione (root + SSTR/)
3. Verificare case BCD (Strelec usa `bcd` minuscolo)
4. Verificare WIM in SSTR/, non in root
5. Verificare hash identici: `md5sum bootx64.efi` in entrambi i path
6. Testare boot prima di sostituire

## Arch Linux Netboot

File locale: `/volume1/web/archlinux/ipxe-arch.efi`

archlinux.org forza HTTPS, non supportato da iPXE senza ricompilazione. Si usa una copia locale aggiornata manualmente:

```bash
wget https://archlinux.org/static/netboot/ipxe-arch.efi -O /tmp/ipxe-arch.efi
rsync -avP /tmp/ipxe-arch.efi -e "ssh -p 2222" marco@home.ziliani.net:/volume1/web/archlinux/
```

## Aggiungere un nuovo tool

### WinPE

```bash
# Sul NAS
mkdir -p /volume1/web/newtool/{boot,sources}
# Copiare BCD, boot.sdi, boot.wim
```

Nel menu (`src/menu.ipxe`):

```ipxe
item --key x newtool  New Tool Name

:newtool
set base-url http://${server-ip}/newtool
goto boot_winpe
```

### Linux

```bash
mkdir -p /volume1/web/newdistro
# Copiare kernel, initrd, filesystem
```

```ipxe
item --key x newdistro  New Distro Name

:newdistro
set base-url http://${server-ip}/newdistro
kernel ${base-url}/vmlinuz <parametri>
initrd ${base-url}/initrd.img
boot || goto failed
```

Dopo le modifiche: `./build.sh` e deploy su router.

## Troubleshooting

| Problema | Verifica |
|----------|----------|
| WinPE non parte | wimboot ≥ 2.7.3? Case BCD corretto? File tutti presenti? |
| Strelec "non trova \SSTR\..." | File boot duplicati in root e SSTR? Hash identici? network.cmd ok? |
| NFS Clonezilla fallisce | Export NFS attivo? Firewall? `mount -t nfs 192.168.1.1:/volume2/backup /mnt` |
| Timeout DHCP | Server DHCP attivo? Opzioni PXE (66, 67) configurate? |
| Download lento | Verificare rete gigabit. boot.wim ~1-2 GB → ~8-10s su gigabit |

## Riferimenti

- [iPXE](http://ipxe.org)
- [Wimboot](https://github.com/ipxe/wimboot) (≥ v2.7.3)
- [Hiren's BootCD PE](https://www.hirensbootcd.org)
- [System Rescue](https://www.system-rescue.org)
