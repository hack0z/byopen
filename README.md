<div align="center">
  <h1>dyOpen</h1>
  <p>A dlopen library that bypasses mobile system limitation</p>
</div>

## 简介

byOpen是一个绕过移动端系统限制的增强版dlfunctions库。

## 支持特性

### Android

支持App中加载和使用Android系统库接口（即使maps中还没有被加载也支持）。

Android 7以上dlopen, System.load都是被限制调用的，虽然目前网上有[Nougat_dlfunctions](https://github.com/avs333/Nougat_dlfunctions)等库通过从maps中找so库来绕过加载限制。

不过对于app中还没被加载到maps的so库，这种方式就不行了。

而byOpen不仅支持fake dlopen方式从maps加载，还可以将还没加载到maps的so库绕过系统限制强行加载进来使用，实现更加通用化得dlopen。

注：目前的实现方式理论上还是比较通用的，至少我这Android 10上测试ok，但还没完整详细测试过，是否使用请自行评估。

#### 相关原理

具体实现原理还是比较简单的，主要还是借鉴了[一种绕过Android P对非SDK接口限制的简单方法](http://weishu.me/2018/06/07/free-reflection-above-android-p/)的思想和实现方式。

虽然这篇文章中主要目的是为了绕过hide api，不过它里面使用的将自己假装成系统调用的方式，一样可以用到`System.loadLibrary`上去，让系统以为是系统自身在调用`System.loadLibrary`

从而绕过Android N的classloader-namespace限制，将系统/system/lib中任意so库加载到maps中，然后再通过fake dlopen的方式去dlsym。

#### 增强版fake dlopen

关于fake dlopen的方式实现，网上已有很多实现，比如：

* [Nougat_dlfunctions](https://github.com/avs333/Nougat_dlfunctions)
* [Enhanced_dlfunctions](https://github.com/turing-technician/Enhanced_dlfunctions)

byOpen参考了里面的实现，重新实现了一遍，并且做了一些小改进：

* 不在/proc/self/maps中的系统库，也能绕过限制强行加载进来使用
* 除了从.dynsym中检索符号，还支持从.symtab中检索符号（参考：Enhanced_dlfunctions，顺带修复了里面的一些bug）
* 整个dlopen过程只有一次malloc分配（省去整个符号表的内存分配和copy）

#### Android例子

Android相关测试App例子在：[Android Sample](https://github.com/hack0z/byOpen/tree/master/src/android)

注：目前自带的App测试例子里面的系统库我写死了，有些系统版本上有可能不存在，请先改成用户自己的库和符号名，再编译测试

```java
public class MainActivity extends AppCompatActivity {
    private static final String SYSTEM_LIBRARY = "curl";
    private static final String SYMBOL_NAME = "curl_version";
```

除了Native版本dlopen接口，byOpen额外提供了java版本的[System.loadLibrary](https://github.com/hack0z/byOpen/blob/master/src/android/lib/src/main/java/dyopen/lib/SystemLoader.java)接口在java层直接绕过系统库加载。

关键代码如下：

```java
static public boolean loadLibrary(String libraryName) {
    Method forName = Class.class.getDeclaredMethod("forName", String.class);
    Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
    Class<?> systemClass = (Class<?>) forName.invoke(null, "java.lang.System");
    Method loadLibrary = (Method) getDeclaredMethod.invoke(systemClass, "loadLibrary", new Class[]{String.class});
    loadLibrary.invoke(systemClass, libraryName);
}
```

而native版本的[dlopen_android.c](https://github.com/hack0z/byOpen/blob/master/src/native/byopen_android.c)实现中，我将这段绕过的系统加载的方式，通过jni重新实现了一遍，然后和fake dlopen无缝结合到了一起。

### iOS

虽然ios可以直接使用dlopen，但是审核上会有风险，苹果有可能会对提交AppStore的app扫描相关dlopen/dlsym等调用，来判断是否存在一些敏感的私有调用。

为了在通过调用一些私有接口的时候避免被苹果检测到，byOpen也通过自己实现dlopen/dlsym直接从已经加载进来的images列表里面直接查找对应symbol地址来调用。

当然，为了更加安全，相关调用的库符号硬编码字符串等，用户可以自行做层变换加密，不要直接编译进app。

## 接口用法

相关静态库和接口在：[dlopen.h](https://github.com/hack0z/byOpen/blob/master/src/native/byopen.h)

相关使用方式跟原生dlopen完全相同：

```c
typedef by_char_t const* (*curl_version_t)();
by_pointer_t handle = by_dlopen("libcurl.so", BY_RTLD_LAZY);
if (handle)
{
    by_pointer_t addr = by_dlsym(handle, "curl_version");
    if (addr)
    {
        curl_version_t curl_version = (curl_version_t)addr;
        by_print("curl_version: %s", curl_version());
    }
    by_dlclose(handle);
}
```

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

### iOS

#### 直接编译库

```console
$ xmake f -p iphoneos -a [armv7|arm64]
$ xmake
```

### MacOS

我们也可以在macOS下编译测试，也是支持的：

```console
$ xmake
$ xmake run
```
