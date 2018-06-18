# Julia 2018 Marlin

18 June 2018 - [Marlin](https://github.com/MarlinFirmware/Marlin) 1.1.8

[Wiki](https://github.com/aharshac/FWJulia2018Marlin/wiki)

[G-code](http://marlinfw.org/meta/gcode/)

## Build Variants
- **Julia18_GLCD**
- **Julia18_GLCD_HB**
- **Julia18_RPI**
- **Julia18_RPI_E**

## Development Process
1. Update Marlin if necessary, by `git submodule update` in project root.
2. Copy overriding files to `src\common` and `src\{variant}`.
3. Modify overriding files by referring to wiki. 
4. Generate intermediates using build script.
5. Export hex files using Arduino IDE and build script.
6. Commit source to git.

## Directory Structure
- `src\` - Source root
  * `Marlin\` - [Marlin](https://github.com/MarlinFirmware/Marlin) (git submodule)
  * `common\` - Files common to all variants
  * `Julia18_GLCD\`
  * `Julia18_GLCD_HB\`
  * `Julia18_RPI\`
  * `Julia18_RPI_E\`
- `intermediate\{variant}\` - Marlin + common + variant files
- `output\` - Hex files
- `build.ps1` - Build automation PowerShell script