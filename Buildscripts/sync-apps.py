#!/usr/bin/env python3
"""Populate Data/data/tactility/app/ from the TactilityApps repo build outputs.

The firmware's data partition image (data.bin) is packed from Data/data, and its
pre-installed external apps live under Data/data/tactility/app/<app.id>/. Those
payloads are deliberately gitignored: they are built/copied from the separate
TactilityApps repo instead of being committed here.

This script closes the gap between the two repos:

    python Buildscripts/sync-apps.py                # copy already-built apps
    python Buildscripts/sync-apps.py --build        # build via tactility.py, then copy
    python Buildscripts/sync-apps.py --platform esp32c5

For each app under release/TactilityApps/Apps it copies:
    manifest.properties                 -> Data/data/tactility/app/<id>/manifest.properties
    build/<id>.app.elf (or cmake-build) -> Data/data/tactility/app/<id>/elf/<platform>.elf
    assets/                             -> Data/data/tactility/app/<id>/assets/

Run this before `idf.py build` so the flashed data.bin carries the apps.
"""

import argparse
import os
import shutil
import subprocess
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(SCRIPT_DIR)  # firmware repository root

APPS_ROOT = os.path.join(ROOT, "release", "TactilityApps", "Apps")
DATA_APP_ROOT = os.path.join(ROOT, "Data", "data", "tactility", "app")
TACTILITY_PY = os.path.join(ROOT, "release", "TactilityApps", "tactility.py")

DEFAULT_PLATFORM = "esp32c5"

# Parent dir that tactility.py --local-sdk searches for the released SDK
# (see Buildscripts/release-sdk-current.py). Override with TACTILITY_SDK_PATH.
DEFAULT_SDK_PATH = os.path.join(ROOT, "release", "TactilitySDK")


def read_app_id(app_dir):
    """Return the app.id value from manifest.properties, or None."""
    manifest = os.path.join(app_dir, "manifest.properties")
    if not os.path.isfile(manifest):
        return None
    with open(manifest, encoding="utf-8") as f:
        for line in f:
            if line.startswith("app.id="):
                return line.split("=", 1)[1].strip()
    return None


def discover_apps():
    """Return [(app_dir_name, app_id)] for every app with a manifest."""
    apps = []
    if not os.path.isdir(APPS_ROOT):
        return apps
    for name in sorted(os.listdir(APPS_ROOT)):
        app_dir = os.path.join(APPS_ROOT, name)
        if not os.path.isdir(app_dir):
            continue
        app_id = read_app_id(app_dir)
        if app_id:
            apps.append((name, app_id))
    return apps


def find_elf(app_dir, app_id, platform):
    """Locate the built app ELF, preferring the canonical tactility.py output."""
    candidates = [
        os.path.join(app_dir, "build", f"cmake-build-{platform}", f"{app_id}.app.elf"),
        os.path.join(app_dir, "build", f"{app_id}.app.elf"),
    ]
    for path in candidates:
        if os.path.isfile(path):
            return path
    return None


def build_app(app_dir, platform, sdk_path):
    """Build one app with tactility.py (requires a released local SDK)."""
    env = os.environ.copy()
    env["TACTILITY_SDK_PATH"] = sdk_path
    cmd = [sys.executable, TACTILITY_PY, app_dir, "build", platform, "--local-sdk"]
    print(f"  build: {' '.join(cmd[1:])}", flush=True)
    result = subprocess.run(cmd, env=env)
    return result.returncode == 0


def sync_app(app_dir, app_id, platform):
    """Copy one app's payloads into Data/data; returns True on success."""
    elf = find_elf(app_dir, app_id, platform)
    if elf is None:
        print(f"  SKIP {app_id}: no {app_id}.app.elf built (run with --build)")
        return False

    dest = os.path.join(DATA_APP_ROOT, app_id)
    elf_dir = os.path.join(dest, "elf")
    os.makedirs(elf_dir, exist_ok=True)

    shutil.copy2(os.path.join(app_dir, "manifest.properties"),
                 os.path.join(dest, "manifest.properties"))
    shutil.copy2(elf, os.path.join(elf_dir, f"{platform}.elf"))

    assets = os.path.join(app_dir, "assets")
    dest_assets = os.path.join(dest, "assets")
    if os.path.isdir(assets):
        if os.path.isdir(dest_assets):
            shutil.rmtree(dest_assets)
        shutil.copytree(assets, dest_assets)
    elif os.path.isdir(dest_assets):
        shutil.rmtree(dest_assets)

    print(f"  synced {app_id}", flush=True)
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--platform", default=DEFAULT_PLATFORM,
                        help=f"target platform for the ELF (default: {DEFAULT_PLATFORM})")
    parser.add_argument("--build", action="store_true",
                        help="build each app via tactility.py before copying")
    parser.add_argument("--sdk", default=DEFAULT_SDK_PATH,
                        help="TACTILITY_SDK_PATH for --build (local SDK root)")
    args = parser.parse_args()

    apps = discover_apps()
    if not apps:
        print(f"No apps found under {APPS_ROOT}", file=sys.stderr)
        return 1

    synced = 0
    skipped = 0
    build_failures = 0
    for name, app_id in apps:
        app_dir = os.path.join(APPS_ROOT, name)
        print(f"{name}:", flush=True)
        if args.build:
            if not build_app(app_dir, args.platform, args.sdk):
                print(f"  BUILD FAILED {app_id}", flush=True)
                build_failures += 1
                continue
        if sync_app(app_dir, app_id, args.platform):
            synced += 1
        else:
            skipped += 1

    print(f"\nSynced {synced} app(s), skipped {skipped} (no ELF), "
          f"failed {build_failures} build(s) into {DATA_APP_ROOT}")
    print("Now run `idf.py build` (and flash data.bin) to bake them into the image.")
    # Missing ELFs (e.g. device-specific apps not built for this platform) are
    # informational, not errors. Only a requested-but-failed build is fatal.
    return 1 if build_failures else 0


if __name__ == "__main__":
    sys.exit(main())
