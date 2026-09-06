import io, os, tarfile, requests
app_dir = r"C:\Tactility\release\TactilityApps\Apps\TwoEleven"
app_id = "tactility.twoeleven"
elf = os.path.join(app_dir, "build", "twoeleven.app.elf")
buf = io.BytesIO()
with tarfile.open(fileobj=buf, mode="w", format=tarfile.USTAR_FORMAT) as tar:
    tar.add(os.path.join(app_dir, "manifest.properties"), arcname="manifest.properties")
    d = tarfile.TarInfo("elf"); d.type = tarfile.DIRTYPE; d.mode = 0o755
    tar.addfile(d)
    tar.add(elf, arcname="elf/esp32c5.elf")
buf.seek(0)
r = requests.put("http://192.168.1.79/api/apps/install", files={"file": (app_id+".app", buf, "application/octet-stream")}, timeout=180, headers={"Connection":"close"})
print("TwoEleven install:", r.status_code, r.text[:60])