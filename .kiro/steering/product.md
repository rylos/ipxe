# Product Overview

iPXE is an open-source network bootloader that enables booting operating systems and tools directly over the network. This repository contains a customized iPXE configuration with a personalized boot menu for recovery and diagnostic tools.

## Key Features

- Network-based booting via PXE/UEFI
- Custom boot menu with 30-second timeout
- Support for both BIOS and UEFI systems
- Collection of recovery and diagnostic tools including:
  - Clonezilla Live (disk cloning/backup)
  - System Rescue CD (Linux recovery environment)
  - Various Windows PE-based tools (Macrium, Synology Recovery, etc.)
  - Network boot options (netboot.xyz, Arch Linux)

## Target Environment

- NAS Synology (192.168.1.1) serve le immagini boot via HTTP in `/volume1/web/`
- Router OpenWrt (firewall.ziliani.net) serve `nas.efi` via TFTP per PXE boot
- Supports both x86 and x64 architectures
- Primarily used for system recovery and diagnostics in home/small office environments

## Main Deliverable

The primary output is `nas.efi` - a UEFI-compatible iPXE bootloader with embedded menu configuration.