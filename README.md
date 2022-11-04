# Bugtris

## How to build

You need a compiled Astralbrew library distribution. As the engine's source is not yet ready to be published, I attached a working debug library version to each of the game releases. Extract the `Astralbrew` folder from the zip file to a location on your computer (make sure the path doesn't contain any spaces, otherwise devkitPro's tools would complain about this).

Now you need to put the path to the engine into an environment variable

`set ASTRALBREW=x:/path/to/Astralbrew`

Now you can finally build it.

```
git clone https://github.com/NotImplementedLife/Bugtris
cd Bugtris
make -f Makefile
```

The code compiles with the latest version of devkitPro toolchains, and makes use of C++17 features. If you have any trouble compiling it, please don't hesistate to contact me. I don't guarantee I would be able to detect the issues and solve them in all cases, but at least I could try to help. (I had nightmares myself to make it work on my own PC after I reinstalled Windows so yeah =)) ).

## Credits

- [foopod](https://github.com/foopod) & [exelotl](https://github.com/exelotl) for the GBAJam2022 logo
- [GBAJam2022](https://itch.io/jam/gbajam22) organisers and the whole GBA and other retro consoles dev communities
- [libgba](https://github.com/devkitPro/libgba) for the GBA development tools
