# Technology Stack

## Build System

- **Primary Build Tool**: GNU Make
- **Compiler**: GCC with cross-compilation support
- **Architecture**: C-based codebase with assembly components
- **Target Platforms**: x86, x86_64, ARM, RISC-V, LoongArch

## Key Technologies

- **Network Protocols**: IPv4, DHCP, TFTP, HTTP, ARP, UDP, TCP
- **Boot Standards**: PXE, UEFI, BIOS
- **Cryptography**: TLS, various hash algorithms (MD5, SHA1, SHA256, etc.)
- **Image Formats**: ELF, PE, various compression formats
- **File Systems**: Basic support for network-based file access

## Build Commands

### Standard Build
```bash
cd src
make
```

### Custom Build (Project-Specific)
```bash
# Build UEFI binary with embedded menu
cd src
make bin-x86_64-efi/ipxe.efi EMBED=menu.ipxe

# Or use the project build script
./build.sh
```

### Common Build Targets
- `bin/ipxe.iso` - Bootable CD-ROM image
- `bin/ipxe.usb` - Bootable USB image  
- `bin/ipxe.pxe` - PXE chainload image
- `bin-x86_64-efi/ipxe.efi` - UEFI application
- `bin/undionly.kpxe` - UNDI-only PXE image

### Configuration
- Configuration files in `src/config/` directory
- Main config: `src/config/general.h`
- Platform-specific configs in `src/config/defaults/`
- Custom menu: `src/menu.ipxe`

### Dependencies
- GCC toolchain
- GNU Make
- Perl (for build scripts)
- OpenSSL (for crypto features)
- Standard POSIX utilities

## Deployment
The final `nas.efi` binary is deployed to:
- Router TFTP server: `firewall.ziliani.net:/tftp/`
- Local boot partition: `/boot/` (optional)