# ToxInput 1.0 by pants64

This program makes it possible to play SM64DS with "360 degree" joystick controls. Only compatible with European ROMs. By default, the only supported emulator is the latest 64 bit X432R version of DeSmuME (0.9.12), but other emulators can be made to work as well [(see below)](#compatibility-with-different-emulators). It works with hacked ROMs too, unless they have changed code in certain places.

## How to use

1. Make sure an Xbox controller is connected
2. Start ToxInput
3. Select the right emulator window. If you can't it means that the game or the emulator isn't compatible.
4. Choose the settings you want by clicking the checkboxes. You can't change the first two after leaving the file select screen if you're using JIT mode.

## Compatibility with different emulators

By default, ToxInput only works with a specific build of the X432R version of DeSmuME. This is because it modifies the emulator's memory, and the address of the NDS main RAM within the emulator varies between different builds. For the emulator supported by default, this address is `0x1462c3840`. Other emulators can be made to work by specifying the address using the `--address` command line option when starting the emulator. For example, if that address was `0x14a91a640` for a given emulator, ToxInput would need to be started like this:
```
ToxInput.exe --address=0x14a91a640
```

In practice, I'd recommend making a shortcut for ToxInput.exe and then adding the option `--address=0x14a91a640` at the end of its "target" path in its properties.

Here's how to find that address for a given emulator:

1. Write a unique string of bytes at 0x02000000 in the NDS address space (either by using a RAM viewer in the emulator, or a ROM if the former isn't available).
2. Use a tool like [Cheat Engine](https://www.cheatengine.org/) to find the address of that byte string in the emulator's virtual memory.
3. Close the emulator and repeat a few times to make sure that the address is static, i.e. it doesn't change between different instances or runs. If it does change, you might unfortunately have to change the address every time you open the emulator.
