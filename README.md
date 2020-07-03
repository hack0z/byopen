<div align="center">
  <h1>byOpen</h1>
  <p>A dlopen library that bypasses mobile system limitation</p>
</div>

## Introduction ([中文](/README_zh.md))

byOpen is a dlopen library that bypasses mobile system limitation.

## Support features

### Android

Support loading Android system library in App

For Android 7 and above, dlopen and System.load are restricted. Although there are libraries such as [Nougat_dlfunctions](https://github.com/avs333/Nougat_dlfunctions) on the Internet, they can bypass the loading restrictions by looking for the so library from maps.

But for the so library in the app that has not been loaded into the maps, this method will not work.

And byOpen not only supports fake dlopen loading from maps, but also can add so library that has not been loaded into maps to bypass the system restrictions and force loading into use, to achieve a more generalized dlopen.

### iOS

Although ios can directly use dlopen, there will be risks in the audit. You can bypass the audit detection by implementing dlopen/dlsym yourself (not yet implemented, I will add support later)
