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

The current implementation is theoretically quite general, at least I tested it on Android 10, but it has been tested in full detail, please use your own evaluation.

#### Related principles

The specific implementation principle is relatively simple, mainly based on [a simple way to bypass the restrictions of Android P on non-SDK interface](http://weishu.me/2018/06/07/free-reflection-above-android -p/) idea and implementation.

Although the main purpose of this article is to bypass the hide api, the way it pretends to be a system call in it can also be used in `System.loadLibrary`, which makes the system think that the system itself is calling System. loadLibrary`

In order to bypass the classloader-namespace limitation of Android N, load any so library in the system /system/lib to the maps, and then go to dlsym by way of fake dlopen.

### iOS

Although ios can directly use dlopen, there will be risks in the audit. You can bypass the audit detection by implementing dlopen/dlsym yourself (not yet implemented, I will add support later)

## Interface

Related static libraries and interfaces are at: [Native Dlfunctions](https://github.com/hack0z/byOpen/blob/master/src/native/byopen.h)

## compile

Compilation needs to be installed first: [xmake](https://github.com/xmake-io/xmake)

### Android

#### Compile the library directly

```console
$ xmake f -p android --ndk=~/file/android-ndk-r20b
$ xmake
```

#### Compile and test Apk through gradle

```console
$ cd src/android
$ ./gradlew app:assembleDebug
```

#### Directly compile apk through xmake

```console
$ xmake apk_build
```

#### Install and test apk directly via xmake

```console
$ xmake apk_test
```
