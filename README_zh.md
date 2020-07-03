<div align="center">
  <h1>dyOpen</h1>
  <p>A dlopen library that bypasses mobile system limitation</p>
</div>

## 简介

byOpen是一个绕过移动端系统限制的dlopen库。

## 支持特性

### Android

支持App中加载Android系统库

Android 7以上dlopen, System.load都是被限制调用的，虽然目前网上有[Nougat_dlfunctions](https://github.com/avs333/Nougat_dlfunctions)等库通过从maps中找so库来绕过加载限制。

不过对于app中还没被加载到maps的so库，这种方式就不行了。

而byOpen不仅支持fake dlopen方式从maps加载，还可以加还没加载到maps的so库绕过系统限制强行加载进来使用，实现更加通用化得dlopen。

目前的实现方式理论上还是比较通用的，至少我这Android 10上测试ok，但还完整详细测试过，是否使用请自行评估。

#### 相关原理

具体实现原理还是比较简单的，主要还是借鉴了[一种绕过Android P对非SDK接口限制的简单方法](http://weishu.me/2018/06/07/free-reflection-above-android-p/)的思想和实现方式。

虽然这篇文章中主要目的是为了绕过hide api，不过它里面使用的将自己假装成系统调用的方式，一样可以用到`System.loadLibrary`上去，让系统以为是系统自身在调用`System.loadLibrary`

从而绕过Android N的classloader-namespace限制，将系统/system/lib中任意so库加载到maps中，然后再通过fake dlopen的方式去dlsym。

### iOS

虽然ios可以直接使用dlopen，但是审核上会有风险，可以通过自己实现dlopen/dlsym来绕过审核检测（暂时还没实现，之后我会加上支持）

## 接口

相关静态库和接口在：[Native Dlfunctions](https://github.com/hack0z/byOpen/blob/master/src/native/byopen.h)

## 编译

编译需要先安装：[xmake](https://github.com/xmake-io/xmake)

### Android

#### 直接编译库

```console
$ xmake f -p android --ndk=~/file/android-ndk-r20b
$ xmake
```

#### 通过gradle编译测试Apk

```console
$ cd src/android
$ ./gradlew app:assembleDebug
```

#### 通过xmake直接编译apk

```console
$ xmake apk_build
```

#### 通过xmake直接安装测试apk

```console
$ xmake apk_test
```
