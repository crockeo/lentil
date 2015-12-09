# lentil

*lentil* is an asset management system written in C for use with OpenGL. It
handles the loading of textures, shaders, models, and materials. For a game
engine look at [curry](http://github.com/crockeo/curry) instead, which uses this
for its asset loading.

### Installation

CMake version required `>= 2.9`.

```bash
$ git clone https://github.com/crockeo/lentil.git
$ cd lentil/
$ cmake .
$ make install
```

### Uninstallation

To uninstall, simply run the `uninstall.sh` file, which reads from
`install_manifest.txt` to rmove all installed files.
