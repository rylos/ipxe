# iPXE Boot Menu - Documentazione Tecnica

## Panoramica
Menu iPXE personalizzato per boot di rete con strumenti di recovery e diagnostica.

## Struttura Server

### Server Web: `192.168.1.1` (NAS Synology)
- **Accesso SSH**: `ssh -p 2222 marco@home.ziliani.net`
- **Path web**: `/volume1/web/`
- **Docker**: `/usr/local/bin/docker` (richiede sudo)

### File Condivisi
```
/volume1/web/
├── wimboot              # WinPE bootloader (v2.7.3+) - condiviso da tutti i progetti WinPE
├── clonezilla/          # Clonezilla Live
├── rescuezilla/         # Rescuezilla
├── sysresccd/           # System Rescue CD
├── macrium/             # Macrium Recovery Media (WinPE)
├── synomedia/           # Synology Recovery Media (WinPE)
├── minitool/            # MiniTool Partition Wizard (WinPE)
├── easyuefi/            # EasyUEFI Enterprise (WinPE)
├── hirens/              # Hiren's BootCD PE (WinPE)
└── strelec/             # Sergei Strelec WinPE 10/11
```

## Configurazione Menu

### File Principale
- **Path**: `src/menu.ipxe`
- **Backup**: `src/menu.ipxe.bak`

### Variabili Globali
```ipxe
set server-ip 192.168.1.1          # IP server web
set nas-ip ${server-ip}            # IP NAS per mount NFS
set wimboot-url http://${server-ip}/wimboot  # WinPE bootloader condiviso
```

### Struttura Menu
1. **Recovery Tools** - Strumenti di recovery e diagnostica
2. **Netboot (UEFI)** - netboot.xyz
3. **Utility** - Config, shell, reboot

## Compilazione e Deploy

### Compilazione
```bash
cd /home/marco/dev/ipxe
./build.sh
```
Questo genera `/home/marco/dev/ipxe/nas.efi`

**IMPORTANTE:** Usare sempre `nas.efi` dalla root del progetto, NON `src/bin-x86_64-efi/ipxe.efi`

### Deploy su Router e PC Casa

**Router (firewall.ziliani.net):**
```bash
scp -P 44222 /home/marco/dev/ipxe/nas.efi root@firewall.ziliani.net:/tftp/nas.efi
scp -P 44222 /home/marco/dev/ipxe/src/menu.ipxe root@firewall.ziliani.net:/tftp/menu.ipxe
```

**PC di casa (home.ziliani.net) - opzionale:**
```bash
scp -P 22222 /home/marco/dev/ipxe/nas.efi marco@home.ziliani.net:/tmp/nas.efi
ssh -t -p 22222 marco@home.ziliani.net "sudo mv /tmp/nas.efi /boot/"
```

## Progetti WinPE

### Struttura Standard
Ogni progetto WinPE richiede:
```
project/
├── boot/
│   ├── bcd              # Boot Configuration Data
│   └── boot.sdi         # System Deployment Image
└── sources/
    └── boot.wim         # Windows PE image
```

### Boot WinPE Generico
Usa la funzione `boot_winpe` per progetti standard (macrium, synomedia):
```ipxe
:project
set base-url http://${server-ip}/project
goto boot_winpe
```

### Boot WinPE Personalizzato
Per progetti con file aggiuntivi (strelec, easyuefi, hirens):
```ipxe
:project
set base-url http://${server-ip}/project
kernel ${wimboot-url}
initrd ${base-url}/boot/bcd         BCD
initrd ${base-url}/boot/boot.sdi    boot.sdi
initrd ${base-url}/bootmgr.efi      bootmgr.efi  # se necessario
initrd ${base-url}/sources/boot.wim boot.wim
boot || goto failed
```

## Progetti Linux

### Clonezilla
- **Kernel**: vmlinuz
- **Initrd**: initrd.img
- **Filesystem**: filesystem.squashfs
- **Mount NFS**: `/volume2/backup` → `/home/partimag`
- **Locale**: italiano (it_IT.UTF-8)

### System Rescue CD
- **Kernel**: vmlinuz
- **Initrd**: intel_ucode.img, amd_ucode.img, sysresccd.img
- **Keyboard**: italiano (setkmap=it)

### Rescuezilla
- **Kernel**: vmlinuz
- **Initrd**: initrd.lz
- **Boot**: casper (Ubuntu-based)
- **Keyboard**: italiano

### Arch Linux Netboot
- **File locale**: `/volume1/web/archlinux/ipxe-arch.efi` (1 MB)
- **Fonte**: https://archlinux.org/static/netboot/ipxe-arch.efi
- **Aggiornamento**: Manuale (raramente necessario, mesi/anni)
- **Motivo file locale**: archlinux.org forza HTTPS, iPXE non supporta HTTPS senza ricompilazione complessa
- **Alternativa non funzionante**: ~~`http://ipxe.archlinux.org/releng/netboot/archlinux.ipxe`~~ (redirect a HTTPS)

**Aggiornamento file:**
```bash
wget https://archlinux.org/static/netboot/ipxe-arch.efi -O /tmp/ipxe-arch.efi
rsync -avP /tmp/ipxe-arch.efi -e "ssh -p 2222" marco@home.ziliani.net:/volume1/web/archlinux/
```

## Aggiungere Nuovo Progetto

### 1. WinPE (es. nuovo tool)
```bash
# Sul server
mkdir -p /volume1/web/newtool/boot /volume1/web/newtool/sources

# Copia file necessari
rsync -avP /path/to/iso/boot/bcd /volume1/web/newtool/boot/
rsync -avP /path/to/iso/boot/boot.sdi /volume1/web/newtool/boot/
rsync -avP /path/to/iso/sources/boot.wim /volume1/web/newtool/sources/
```

Nel menu iPXE:
```ipxe
# Aggiungi voce menu
item --key x newtool     New Tool Name

# Aggiungi sezione boot
:newtool
set base-url http://${server-ip}/newtool
goto boot_winpe
```

### 2. Linux (es. nuova distro)
```bash
# Sul server
mkdir -p /volume1/web/newdistro

# Copia kernel, initrd e filesystem
rsync -avP /path/to/iso/ /volume1/web/newdistro/
```

Nel menu iPXE:
```ipxe
# Aggiungi voce menu
item --key x newdistro   New Distro Name

# Aggiungi sezione boot
:newdistro
set base-url http://${server-ip}/newdistro
kernel ${base-url}/vmlinuz <parametri>
initrd ${base-url}/initrd.img
boot || goto failed
```

## Ottimizzazioni Applicate

### 2025-11-18
- ✅ Centralizzato IP server in variabile `server-ip`
- ✅ Creata funzione `boot_winpe` per ridurre duplicazione
- ✅ Creata funzione `boot_strelec` parametrizzata
- ✅ Rimosso rilevamento architettura inutilizzato
- ✅ Wimboot aggiornato a v2.7.3+ (supporto compressione Hiren's)
- ✅ Wimboot centralizzato in `/volume1/web/wimboot`
- ✅ Aggiunto Hiren's BootCD PE

## Note Tecniche

### Wimboot
- **Versione minima**: 2.7.3 (per Hiren's BootCD PE)
- **Download**: https://github.com/ipxe/wimboot/releases/latest
- **Motivo**: Supporto compressione massima boot.wim

### Case Sensitivity
I nomi file WinPE sono case-sensitive:
- `BCD` vs `bcd` - dipende dal progetto
- `BOOTMGR` vs `bootmgr` - verificare sempre

### Dimensioni
- boot.wim tipico: 1-2 GB
- Tempo download via HTTP: ~8-10 secondi (rete gigabit)

## Gestione Sergei Strelec WinPE

### Struttura Speciale
Strelec richiede una struttura duplicata per funzionare correttamente:

```
strelec/
├── bootia32.efi         # File boot (root)
├── bootmgr32.exe
├── bootmgr64.exe
├── bootx64.efi
├── bcd
├── boot.sdi
├── network.cmd          # Script di inizializzazione rete
└── SSTR/                # Directory principale
    ├── bootia32.efi     # File boot (duplicati identici)
    ├── bootmgr32.exe
    ├── bootmgr64.exe
    ├── bootx64.efi
    ├── bcd
    ├── boot.sdi
    ├── strelec10x64Eng.wim  # WinPE 10 (caricato da iPXE)
    ├── strelec11x64Eng.wim  # WinPE 11 (caricato da iPXE)
    ├── WLANProfile/     # Profili WiFi
    ├── DriverPacks/
    ├── _WIN/
    └── ... (altri file)
```

**IMPORTANTE:** I file `.wim` devono essere **in SSTR**, non nella root. iPXE li carica da `http://192.168.1.1/strelec/SSTR/strelecXXx64Eng.wim`.

### File Boot Duplicati
I file `bootia32.efi`, `bootmgr32.exe`, `bootmgr64.exe`, `bootx64.efi` devono essere:
- ✅ Presenti in **root** (`/volume1/web/strelec/`)
- ✅ Presenti in **SSTR** (`/volume1/web/strelec/SSTR/`)
- ✅ **Identici** (stesso hash MD5)

**Verifica:**
```bash
md5sum /volume1/web/strelec/bootx64.efi /volume1/web/strelec/SSTR/bootx64.efi
# Devono avere lo stesso hash
```

### network.cmd
Script eseguito all'avvio che:
1. Monta share di rete: `\\192.168.1.1\web\strelec`
2. Cerca drive locale con `\SSTR\WLANProfile`
3. Imposta variabile `%strelec%` con lettera drive
4. Configura rete (DHCP, WiFi, firewall)
5. Carica profili WiFi da `%strelec%\SSTR\WLANProfile`
6. Monta share temp: `\\192.168.1.1\temp`

**Credenziali share:**
- User: `web`
- Password: `0jSpjMjZT0a0oG`

### Aggiornamento Versione
Quando si aggiorna Strelec:

1. **Copia nuova versione** in directory temporanea (es. `strelec_new_tofix`)
2. **Verifica file boot** in entrambi i percorsi:
   ```bash
   # Copia dalla vecchia versione
   cp /volume1/web/strelec/boot*.{efi,exe} /volume1/web/strelec_new/
   cp /volume1/web/strelec/SSTR/boot*.{efi,exe} /volume1/web/strelec_new/SSTR/
   ```
3. **Verifica BCD case** - Strelec usa `bcd` minuscolo:
   ```bash
   # Rimuovi BCD maiuscolo se presente
   rm /volume1/web/strelec_new/SSTR/BCD
   # Copia bcd minuscolo dalla vecchia
   cp /volume1/web/strelec/SSTR/bcd /volume1/web/strelec_new/SSTR/
   ```
4. **Verifica hash identici**:
   ```bash
   md5sum /volume1/web/strelec_new/bootx64.efi /volume1/web/strelec_new/SSTR/bootx64.efi
   ```
5. **File TBWinPE opzionali** - I file `TBWinPE*.log` sono log di compilazione, non necessari per il boot
6. **Verifica WIM in SSTR** - I file `.wim` devono essere in SSTR, non in root:
   ```bash
   # Se i WIM sono in root, spostarli in SSTR
   mv /volume1/web/strelec_new/*.wim /volume1/web/strelec_new/SSTR/
   ```
7. **Testa boot** prima di sostituire la vecchia
8. **Backup vecchia versione** prima di sovrascrivere

### Differenze tra Versioni
- **WIM più recenti**: dimensioni leggermente diverse
- **File log**: `TBWinPE*.log`, `TBWinPE_BootWIM*` potrebbero mancare (non critici, sono cache di compilazione)
- **BCD case**: Strelec usa `bcd` minuscolo in SSTR, non `BCD` maiuscolo

## Troubleshooting

### Boot WinPE fallisce
1. Verificare versione wimboot (>= 2.7.3)
2. Controllare case dei file (BCD vs bcd)
3. Verificare presenza tutti i file richiesti
4. Testare download HTTP manuale

### Strelec: errore "non trova \SSTR\xxxx"
1. Verificare file boot duplicati in root e SSTR
2. Controllare hash identici: `md5sum bootx64.efi`
3. Verificare `network.cmd` corretto
4. Controllare share di rete accessibile: `\\192.168.1.1\web\strelec`

### Mount NFS Clonezilla fallisce
1. Verificare export NFS su NAS
2. Controllare firewall
3. Testare mount manuale: `mount -t nfs 192.168.1.1:/volume2/backup /mnt`

### Timeout DHCP
1. Verificare server DHCP attivo
2. Controllare opzioni PXE (66, 67)
3. Testare con `dhcp` manuale in shell iPXE

## Riferimenti
- iPXE: http://ipxe.org
- Wimboot: https://github.com/ipxe/wimboot
- Hiren's BootCD PE: https://www.hirensbootcd.org
- System Rescue: https://www.system-rescue.org
