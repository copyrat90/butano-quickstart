#!/usr/bin/env python

from pathlib import Path


def import_module(module_path: Path):
    import importlib.util

    sys.path.insert(0, str(module_path.parent))
    spec = importlib.util.spec_from_file_location(module_path.name, module_path)
    assert spec is not None
    module = importlib.util.module_from_spec(spec)
    assert spec.loader
    spec.loader.exec_module(module)

    return module


if __name__ == "__main__":
    import argparse
    import sys
    import traceback

    butano_ldtk = import_module(Path("libs/butano-ldtk/tools/butano_ldtk.py"))
    butano_fonts_tool = import_module(
        Path("libs/gba-kor-fonts/tools/butano/butano_fonts_tool.py")
    )

    parser = argparse.ArgumentParser(description="python scripts caller.")
    parser.add_argument("--ldtk-project", required=True, help="LDtk project file")
    parser.add_argument("--ldtk-build", required=True, help="LDtk build folder")
    parser.add_argument("--fonts", required=True, help="fonts folders")
    parser.add_argument("--texts", required=False, help="texts forder or files")
    parser.add_argument("--fonts-build", required=True, help="fonts build folder")
    parser.add_argument("--licenses", required=True, help="licenses folder")
    parser.add_argument("--misc-build", required=True, help="misc build folder")

    try:
        args = parser.parse_args()
        ldtk_project = Path(args.ldtk_project)
        ldtk_build = Path(args.ldtk_build)
        fonts_build = Path(args.fonts_build)
        licenses = Path(args.licenses)
        misc_build = Path(args.misc_build)

        if butano_ldtk.process_ldtk(ldtk_project, ldtk_build):
            print(
                f'Successfully converted LDtk project "{ldtk_project}" to "{ldtk_build}"'
            )
        else:
            print(
                f'Skipped converting LDtk project "{ldtk_project}" (not modified since last conversion)'
            )

        fonts_build.mkdir(parents=True, exist_ok=True)

        butano_fonts_tool.process_fonts(
            args.fonts,
            args.fonts_build,
            f"{args.texts} {str(ldtk_build / "src")} {str(ldtk_build / "include")}",
        )

        import misc_writer

        misc_writer.write_miscs(licenses, misc_build)

    except Exception as ex:
        sys.stderr.write(f"Error: {ex}\n")
        traceback.print_exc()
        exit(-1)
