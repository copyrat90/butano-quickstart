# butano-quickstart

This is my personal quickstart template for [Butano](https://github.com/GValiente/butano) projects.

Submodules included:
* [butano](libs/butano/)
* [butano-ldtk](libs/butano-ldtk/)
* [iso-butano](libs/iso-butano/)
* [gba-kor-fonts](libs/gba-kor-fonts/)

If you don't want these submodules and/or my boilerplate included in this repo,\
you can use my [butano-template](https://github.com/copyrat90/butano-template) repo instead.

## Setup

### Setting up build environment

1. Setup dependencies for Butano with [devkitARM](https://gvaliente.github.io/butano/getting_started.html) or [Wonderful Toolchain](https://gvaliente.github.io/butano/getting_started_wt.html).
   * Butano itself is included as a submodule, so you don't need to manually download it.
1. **Recursively** clone this repo.
1. `pip install -r libs/butano-ldtk/requirements.txt`
1. That's it, you can now run `make -j$(nproc)` to build the project.

### Finding include paths with VSCode C/C++ extension

[See this gist.](https://gist.github.com/copyrat90/eee49d92846ca3585a69d5bea001710d)

### Setting up debugger

[See this guide.](https://felixjones.co.uk/mgba_gdb/vscode.html)

### Using stats viewer

Open [`libs/iso-butano/tools/ibn_stats.lua`](libs/iso-butano/tools/ibn_stats.lua) with `Debug > Script Window` in [MesenCE](https://github.com/nesdev-org/MesenCE).


## Checklist

- [ ] Change `BQ` prefix to game's unique one
   - [ ] `namespace bq`
      - [ ] [`misc_writer.py`](tools/misc_writer.py#L6)
   - [ ] [`BQ_DEVBUILD`](Makefile#L78)
   - [ ] [`ROMCODE := 2BQE`](Makefile#L66)
- [ ] Change [`SAVE_MAGIC = "BNQST"`](src/ut/config_save.cpp#L13) to game's unique one
- [ ] Change [`ROMTITLE :=`](Makefile#L65) to game's unique one
- [ ] (Optional) Change filename of [`project.ldtk`](levels/project.ldtk) to your name of the game
   - [ ] Update [`LDTKPROJECT := levels/project.ldtk`](Makefile#L46)
   - [ ] Change folder name of [`levels/project/`](levels/project/)
   - [ ] Update [`"externalRelPath"` in `project.ldtk`](levels/project.ldtk#L324)
   - [ ] Update [LDtk backups path in `.gitignore`](.gitignore#L19)
- [ ] Replace or remove the [`LICENSE`](LICENSE)
- [ ] Replace or remove this [`README.md`](README.md)
