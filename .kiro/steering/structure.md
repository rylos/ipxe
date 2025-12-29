# Project Structure

## Root Directory

- `build.sh` - Main build script for the project
- `nas.efi` - Final UEFI bootloader binary (build output)
- `README.md` - Project documentation (Italian)
- `SETUP.md` - Technical setup and deployment guide
- `COPYING*` - License files (GPL2, UBDL)

## Source Directory (`src/`)

### Core Components
- `src/core/` - Core iPXE functionality (main.c, networking, memory management)
- `src/net/` - Network protocol implementations (TCP, UDP, DHCP, etc.)
- `src/crypto/` - Cryptographic functions and algorithms
- `src/image/` - Image format handlers (ELF, PE, compression)

### Architecture Support
- `src/arch/` - Architecture-specific code
  - `src/arch/x86/`, `src/arch/x86_64/` - x86 implementations
  - `src/arch/arm/`, `src/arch/arm64/` - ARM implementations
  - `src/arch/riscv/`, `src/arch/loong64/` - Other architectures

### Configuration
- `src/config/` - Build-time configuration files
  - `src/config/general.h` - Main configuration options
  - `src/config/defaults/` - Platform-specific defaults
  - `src/config/local/` - Local customizations
- `src/menu.ipxe` - **Custom boot menu script (key file)**
- `src/menu.ipxe.bak` - Backup of menu configuration

### Drivers
- `src/drivers/` - Hardware drivers
  - `src/drivers/net/` - Network card drivers
  - `src/drivers/block/` - Block device drivers
  - `src/drivers/usb/` - USB drivers

### User Interface
- `src/hci/` - Human-Computer Interface
  - `src/hci/commands/` - iPXE shell commands
  - `src/hci/tui/` - Text-based user interface

### Build System
- `src/Makefile*` - Build system files
- `src/scripts/` - Linker scripts
- `src/util/` - Build utilities and tools

### Testing
- `src/tests/` - Unit tests and test utilities

### Output Directories (Generated)
- `src/bin/` - Default build outputs
- `src/bin-x86_64-efi/` - UEFI x64 build outputs
- `src/bin-x86_64-pcbios/` - BIOS x64 build outputs

## Key Files for Customization

1. **`src/menu.ipxe`** - The main boot menu configuration
2. **`src/config/general.h`** - Feature enablement and timeouts
3. **`build.sh`** - Custom build process
4. **`SETUP.md`** - Deployment and server configuration

## Naming Conventions

- C source files: `.c` extension
- Header files: `.h` extension
- iPXE script files: `.ipxe` extension
- Build outputs follow pattern: `bin[-arch-platform]/target.format`
- Configuration files use descriptive names (e.g., `config_*.c`)

## Important Notes

- Always use `nas.efi` from project root, not `src/bin-x86_64-efi/ipxe.efi`
- Menu configuration is embedded at build time via `EMBED=menu.ipxe`
- Architecture-specific code is isolated in `src/arch/` subdirectories
- Network protocol implementations are modular in `src/net/`