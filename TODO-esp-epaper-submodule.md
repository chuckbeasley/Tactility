# TODO: Push the esp_epaper submodule change to my fork

## Status
- The `Libraries/esp_epaper` submodule has a local commit `d72e20b`
  ("Add esp32c5 to supported targets") that is **required** for the C5 build —
  the component-manager rejects the transitive `esp_epaper` dependency otherwise
  (`Component "esp_epaper" ... is not compatible with target "esp32c5"`).
- This commit exists only locally. The superproject `backport/bt-events` branch
  (commit `8243da45`) points at it, but a fresh `git clone --recursive` cannot
  fetch it.
- The submodule's current `origin` is `NellowTCS/esp_epaper`, which **denies
  write access** to `chuckbeasley`.

## To do (one-time)
1. Fork the vendor repo to my account: visit `https://github.com/tuanpmt/esp_epaper`
   → Fork → `chuckbeasley/esp_epaper`.
2. Repoint the submodule and push:
   - `git -C Libraries/esp_epaper remote set-url origin https://github.com/chuckbeasley/esp_epaper.git`
   - `git -C Libraries/esp_epaper push origin HEAD:refs/heads/esp32c5`
3. Commit the `.gitmodules` change in the superproject and push:
   - `git add Libraries/esp_epaper .gitmodules && git commit -m "Point esp_epaper submodule at chuckbeasley fork"`
   - `git push origin backport/bt-events`

## Alternative (if I don't want to fork)
- Revert the submodule pointer change and instead exclude `esp-epaper-module` /
  `esp_epaper` from the C5 build so the `esp32c5` target line isn't needed.
