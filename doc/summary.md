# cocos2dx学习计划

## 目标
* 时间（2019年9月~2019年11月）
* 结果：从0到产出apk（目前只考虑Android平台），并且能使用和修改引擎以满足需要
  
## 知识大纲(以Android平台)
* 用
    - cocos2dx目录结构
    - cocos2dx API用法
* 修改
    - cocos2dx 内核
    - lua C api
    - tolua++  
    - jni与cmake和ndk-build
* 扩展
    - gradle
    - opengles

## 详细目标
* cocos2dx目录结构
    - 时间：2019-09-16~2019-09-20之间1个小时
    - 目标群：无
    - 结果：可以使用API开发项目
    - 主要内容：
        - 完整创建Demo
        - 引擎目录介绍
        - base Demo
        - 打包运行

* cocos2dx Lua API用法
    - 2019-09-23~2019-09-27之间1个小时
    - 目标群：需要可以使用lua
    - 结果：可以使用API开发项目
    - 主要内容：
        - lua 用法
        - UI Demo
        - 多媒体 Demo
        - local storage Demo
        - NetWork Demo
        - 创建cocos2dx lua Demo
        - 热更原理
        - 打包运行包括加密工具和luajit编译工具
     
* cocos2dx 内核
    - 时间：2019-10-08~2019-10-12之间1个小时
    - 目标群：需要可以创建demo并且能够使用api开发
    - 结果：了解引擎的原理，从0到加载起来，最后结束
    - 主要内容：
        - 内存管理
        - ui、扩展
        - 多媒体，fileUtils
        - 网络
        - 第三方库

* LUA C API 
    - 时间：2019-10-14~2019-10-18之间1个小时
    - 目标群：使用过lua并且对C有了解
    - 结果：自定义绑定接口或者扩展lua,lua源码大体了解
    - 主要内容：
        - 可以查看lua实现源码
        - 可以扩展lua
        - 可以导入库或者修改lua满足需求

* tolua++
    - 时间:2019-10-21~2019-10-25之间1个小时
    - 目标群：了解C/C++且使用过lua
    - 结果：可以自己创建类并且能够使用tolua++工具编译并且导出到lua
    - 主要内容：
        - 能够编写tolua++脚本
        - 了解原理
  
* jni与cmake和ndk-build
    - 时间：2019-10-28~2019-11-1之间1个小时
    - 目标群：了解Android开发，写过java代码。
    - 结果：了解jni语法和jni如何使用，可以写Android.mk或者CMakeList.txt脚本
    - 主要内容：
        - jni api使用，jni性能优化，jni 加载卸载
        - 可以使用ndk-build编译项目
        - 可以使用cmake编译项目

* gradle
    - 时间：2019-11-4~2019-11-8之间1个小时
    - 目标群：需要了解Java语法
    - 结果：可以手动管理多项目，可以查找如何定义ApplicationVariant任务
    - 主要内容：
        - 可以打包
        - 可以配置
        - 可以自定义任务

* opengles
    - 时间：2019-11-11~2019-11-15之间1个小时
    - 目标群：
    - 结果：可以自定义Shader,可以在引擎中自定义Shader
    - 主要内容：
        - 可以做特效
        - 可以在引擎中自定义特效