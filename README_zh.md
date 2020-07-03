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

### iOS

虽然ios可以直接使用dlopen，但是审核上会有风险，可以通过自己实现dlopen/dlsym来绕过审核检测（暂时还没实现，之后我会加上支持）

