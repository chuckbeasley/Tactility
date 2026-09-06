import io, os, sys, tarfile, time
import requests

IP = "192.168.1.79"
ROOT = r"C:\Tactility\release\TactilityApps\Apps"
APPS = [
    ("HelloWorld", "tactility.helloworld"), ("TwoEleven", "tactility.twoeleven"),
    ("Brainfuck", "tactility.brainfuck"), ("Breakout", "tactility.breakout"),
    ("Calculator", "tactility.calculator"), ("Diceware", "tactility.diceware"),
    ("GPIO", "tactility.gpio"), ("GraphicsDemo", "tactility.graphicsdemo"),
    ("M5UnitTest", "tactility.m5unittest"), ("Magic8Ball", "tactility.magic8ball"),
    ("MystifyDemo", "tactility.mystifydemo"), ("SerialConsole", "tactility.serialconsole"),
    ("Snake", "tactility.snake"), ("TamaTac", "tactility.tamatac"),
    ("TodoList", "tactility.todolist"), ("MediaKeys", "tactility.mediakeys"),
]
HDR = {"Connection": "close"}

def make_tar(app_dir, app_id):
    elf = os.path.join(app_dir, "build", app_id + ".app.elf")
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w", format=tarfile.USTAR_FORMAT) as tar:
        tar.add(os.path.join(app_dir, "manifest.properties"), arcname="manifest.properties")
        d = tarfile.TarInfo("elf"); d.type = tarfile.DIRTYPE; d.mode = 0o755
        tar.addfile(d)
        tar.add(elf, arcname="elf/esp32c5.elf")
        assets = os.path.join(app_dir, "assets")
        if os.path.isdir(assets):
            tar.add(assets, arcname="assets")
    buf.seek(0)
    return buf

for name, app_id in APPS:
    app_dir = os.path.join(ROOT, name)
    elf = os.path.join(app_dir, "build", app_id + ".app.elf")
    if not os.path.isfile(elf):
        print(f"{name:16s} SKIP (no elf)")
        continue
    tar = make_tar(app_dir, app_id)
    t0 = time.time()
    try:
        r = requests.put(f"http://{IP}/api/apps/install",
                         files={"file": (app_id + ".app", tar, "application/octet-stream")},
                         timeout=180, headers=HDR)
        print(f"{name:16s} {r.status_code} ({round(time.time()-t0,1)}s)")
    except Exception as e:
        print(f"{name:16s} EXC {type(e).__name__}")
    sys.stdout.flush()
