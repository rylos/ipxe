# iPXE Boot Menu — custom network boot EFI image

Fork of [ipxe/ipxe](https://github.com/ipxe/ipxe) customized with an embedded boot menu serving recovery/diagnostic tools over HTTP from a Synology NAS.

## Project

- **Upstream**: `ipxe:master` (merged into local `master`)
- **Origin**: `git@github.com:rylos/ipxe.git`
- **Build output**: `nas.efi` (x86_64 EFI, snponly.efi with embedded `menu.ipxe`)
- **Deploy target**: OpenWrt router at `firewall.ziliani.net:/tftp/nas.efi`
- **Architecture**: Client UEFI PXE → DHCP (OpenWrt) → TFTP `nas.efi` → HTTP boot images from NAS (192.168.1.1:`/volume1/web/`)

## Commands

```bash
./build.sh                        # Build nas.efi (make -j$(nproc) bin-x86_64-efi/snponly.efi EMBED=menu.ipxe)
scp -P 44222 nas.efi root@firewall.ziliani.net:/tftp/nas.efi   # Deploy to router
```

## Architecture

| Path | Role |
|------|------|
| `src/menu.ipxe` | **THE custom file** — iPXE script embedded in the EFI binary |
| `src/config/general.h` | Build-time feature toggles (protocols, drivers) |
| `src/config/branding.h` | PRODUCT_NAME / PRODUCT_SHORT_NAME strings |
| `src/net/` | Core network stack (TCP, UDP, IP, DHCP, TFTP, HTTP, TLS) |
| `src/interface/efi/` | UEFI platform layer (SNP, block, console, etc.) |
| `src/arch/x86_64/`, `src/arch/x86/` | CPU architecture support |
| `src/include/ipxe/` | Internal API headers |
| `src/core/` | Core runtime (job control, settings, device model) |
| `src/usr/` | User-facing commands (`autoboot.c`, `imgmgmt.c`, etc.) |
| `src/util/` | Build tools (`elf2efi`, `efirom`, etc.) |
| `SETUP.md` | Full deployment and troubleshooting docs |
| `CHANGELOG.md` | Upstream release notes (v2.0.0 → current) |

## Conventions

- **Don't edit upstream files** — the only local modifications should be `src/menu.ipxe`, `build.sh`, and documentation. Everything else tracks upstream `ipxe:master`.
- **iPXE script syntax** in `menu.ipxe`: labels (`:name`), `goto`, `kernel`/`initrd`/`boot` for chainloading, `set` for variables, `${variable}` for expansion.
- **Boot images** live on the NAS under `/volume1/web/<tool>/` — each tool has its own directory with kernel/initrd or WinPE BCD/boot.sdi/boot.wim.
- **WinPE tools** use `wimboot` (≥2.7.3) from `/volume1/web/wimboot`.
- **Menu variables**: `server-ip` = 192.168.1.1 (NAS), `wimboot-url`, `base-url` per-tool.
- **Testing**: deploy to router and PXE-boot a client. There is no local test suite for the menu itself.
- **Upstream C conventions** (if ever touching C): `FILE_LICENCE(GPL2_OR_LATER_OR_UBDL)` on every file, `__attribute__((packed))` on wire-format structs, custom `__einfo_error` error system, freestanding (no libc), Doxygen `/** @file */` style.

## Notes

- edit_file/write_file works inside this workspace
- Global config.toml updated: sandbox workspace_root = "/" (takes effect next session)
