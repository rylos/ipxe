# Convenzioni e Stile

## Linguaggio menu iPXE (`src/menu.ipxe`)
- Label con `:nome_tool` (snake_case)
- Variabili con `${nome-variabile}` (kebab-case)
- Sempre `boot || goto failed` dopo ogni boot
- Funzioni riutilizzabili: `:boot_winpe`, `:boot_strelec`
- Shortcut tasto con `item --key X label  Descrizione`
- Sezioni separate da `item --gap --  --- Titolo ---`

## Aggiungere un tool WinPE standard
```ipxe
item --key x newtool  New Tool Name

:newtool
set base-url http://${server-ip}/newtool
goto boot_winpe
```

## Aggiungere un tool Linux
```ipxe
item --key x newdistro  New Distro Name

:newdistro
set base-url http://${server-ip}/newdistro
kernel ${base-url}/vmlinuz <parametri>
initrd ${base-url}/initrd.img
boot || goto failed
```

## Note importanti
- Dopo ogni modifica al menu: `./build.sh` + deploy su router
- Output finale è sempre `nas.efi` nella root, NON `src/bin-x86_64-efi/ipxe.efi`
- Case-sensitive: BCD vs bcd varia per tool (macrium/synomedia usano BCD maiuscolo, strelec/minitool usano bcd minuscolo)
- wimboot deve essere ≥ v2.7.3

## Strelec - Note speciali
- File boot duplicati obbligatori in root e SSTR/ con hash identici
- Verificare: `md5sum bootx64.efi` in entrambi i path
- WIM in SSTR/, non in root
- `network.cmd` monta share SMB `\\192.168.1.1\web\strelec`
