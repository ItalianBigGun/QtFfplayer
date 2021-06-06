

### 应用使用教程

暂时没有，因为界面太low，界面日后要重构。所以不提供演示，跑起来就完了。

### 当前API教程

1. int ffplay(const char *argv)

- 位置：ffplay_src.c
- 功能说明：
    - 用于播放视频，是ffplay功能的主入口。

- 函数说明：
    - 函数名：ffplay
    - 参数：
        - argv    文件名
    - 返回值：
        - 0    成功
        - -1    失败

- 描述：
    - 该函数由原来的ffplay源码中的main函数修改而来。原型为：
    - int main(int argc, const char **argv);

### 原代码API教程

### ffplay框架教程

- 快捷键对应功能图

![输入图片说明](https://images.gitee.com/uploads/images/2021/0606/125947_ae09d68a_7445095.png "屏幕截图.png")

 _说明：由于资料较旧，和最新版本可能存在差异。_ 

- 流程图

![输入图片说明](https://images.gitee.com/uploads/images/2021/0606/125930_b6fea5c6_7445095.png "屏幕截图.png")

 _说明：由于资料较旧，和最新版本可能存在差异。_ 

- 参考笔记

[输入链接说明](http://note.youdao.com/noteshare?id=36e52d4ff9c43042ed24ed24fa0fb07a&sub=3B11A1AD47CE4F50801E8E8F8ABF2295)

 _ :bangbang: 注意：API教程仅供参考，请勿全信。_ 
