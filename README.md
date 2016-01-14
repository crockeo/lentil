# lentil

*lentil* is a graphical toolkit written in C for use with OpenGL. It is *not* a
game engine, but instead a toolkit one can use to ease the development of game
engines.

### Features

#### Assets

* Texture loading (.png)
* Model loading (.obj, .lnt)
* Shader loading (GLSL shaders)
* Material loading (.mtl, .lnt)

...

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

### Credits

The code contained in this project is largely original. Swaths have been copied
and modified from my [clibgame](http://github.com/crockeo/clibgame).

**Credits to other people:**

* [linmath.h](http://github.com/crockeo/linmath.h) was copied and modified for
  project scoping in-line with the project per his "DO WHAT THE FUCK YOU WANT
  PUBLIC LICENSE".
