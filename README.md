# iPXE Boot Menu Personalizzato

Questo progetto contiene una configurazione personalizzata di iPXE con un menu di boot per strumenti di recovery e diagnostica.

## Descrizione

iPXE è un bootloader di rete open source che permette di avviare sistemi operativi e strumenti direttamente dalla rete. Questo repository include un menu personalizzato (`src/menu.ipxe`) con una collezione di strumenti di recovery e diagnostica.

## Menu Disponibili

### Recovery Tools
- **Clonezilla Live** - Clonazione e backup di dischi
- **Synology Recovery Media** - Media di recovery per NAS Synology
- **Macrium Recovery Media** - Strumento di backup e recovery
- **Rescuezilla** - Alternativa open source a Clonezilla
- **MiniTool Partition Wizard** - Gestione partizioni
- **System Rescue CD** - Distribuzione Linux per recovery
- **Sergei Strelec WinPE 10/11** - Ambiente Windows PE per diagnostica
- **EasyUEFI Enterprise** - Gestione boot UEFI

### Netboot
- **netboot.xyz** - Menu di boot di rete con molte distribuzioni

## Caratteristiche

- Menu con timeout di 30 secondi
- Rilevamento automatico architettura (x86/x64)
- Gestione errori robusta con fallback
- Supporto per boot UEFI e BIOS
- Shell iPXE integrata per debugging

## Compilazione

```bash
cd src
make
```

## Configurazione

Il menu principale è definito in `src/menu.ipxe`. Modifica questo file per personalizzare le opzioni di boot e gli URL dei server.

## Requisiti

- Server HTTP per ospitare le immagini di boot
- Rete configurata con DHCP
- iPXE compilato e configurato sul server TFTP/HTTP

Per maggiori dettagli su iPXE: http://ipxe.org
