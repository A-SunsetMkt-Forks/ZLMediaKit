![logo](https://raw.githubusercontent.com/xia-chu/ZLMediaKit/master/www/logo.png)

[english readme](https://github.com/xia-chu/ZLMediaKit/blob/master/README_en.md)

# 一个基于C++11的高性能运营级流媒体服务框架

[![license](http://img.shields.io/badge/license-MIT-green.svg)](https://github.com/xia-chu/ZLMediaKit/blob/master/LICENSE)
[![C++](https://img.shields.io/badge/language-c++-red.svg)](https://en.cppreference.com/)
[![platform](https://img.shields.io/badge/platform-linux%20|%20macos%20|%20windows-blue.svg)](https://github.com/xia-chu/ZLMediaKit)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-yellow.svg)](https://github.com/xia-chu/ZLMediaKit/pulls)
[![Build Status](https://travis-ci.org/xia-chu/ZLMediaKit.svg?branch=master)](https://travis-ci.org/xia-chu/ZLMediaKit)

## 项目特点

- 基于C++11开发，避免使用裸指针，代码稳定可靠，性能优越。
- 支持多种协议(RTSP/RTMP/HLS/HTTP-FLV/Websocket-FLV/GB28181/MP4),支持协议互转。
- 使用多路复用/多线程/异步网络IO模式开发，并发性能优越，支持海量客户端连接。
- 代码经过长期大量的稳定性、性能测试，已经在线上商用验证已久。
- 支持linux、macos、ios、android、windows全平台。
- 支持画面秒开、极低延时([500毫秒内，最低可达100毫秒](https://github.com/xia-chu/ZLMediaKit/wiki/%E5%BB%B6%E6%97%B6%E6%B5%8B%E8%AF%95))。
- 提供完善的标准[C API](https://github.com/xia-chu/ZLMediaKit/tree/master/api/include),可以作SDK用，或供其他语言调用。
- 提供完整的[MediaServer](https://github.com/xia-chu/ZLMediaKit/tree/master/server)服务器，可以免开发直接部署为商用服务器。
- 提供完善的[restful api](https://github.com/xia-chu/ZLMediaKit/wiki/MediaServer%E6%94%AF%E6%8C%81%E7%9A%84HTTP-API)以及[web hook](https://github.com/xia-chu/ZLMediaKit/wiki/MediaServer%E6%94%AF%E6%8C%81%E7%9A%84HTTP-HOOK-API)，支持丰富的业务逻辑。
- 打通了视频监控协议栈与直播协议栈，对RTSP/RTMP支持都很完善。
- 全面支持H265/H264/AAC/G711/OPUS。

## 项目定位

- 移动嵌入式跨平台流媒体解决方案。
- 商用级流媒体服务器。
- 网络编程二次开发SDK。


## 功能清单

- RTSP[S]
  - RTSP[S] 服务器，支持RTMP/MP4/HLS转RTSP[S],支持亚马逊echo show这样的设备
  - RTSP[S] 播放器，支持RTSP代理，支持生成静音音频
  - RTSP[S] 推流客户端与服务器
  - 支持 `rtp over udp` `rtp over tcp` `rtp over http` `rtp组播`  四种RTP传输方式 
  - 服务器/客户端完整支持Basic/Digest方式的登录鉴权，全异步可配置化的鉴权接口
  - 支持H265编码
  - 服务器支持RTSP推流(包括`rtp over udp` `rtp over tcp`方式)
  - 支持H264/H265/AAC/G711/OPUS编码，其他编码能转发但不能转协议

- RTMP[S]
  - RTMP[S] 播放服务器，支持RTSP/MP4/HLS转RTMP
  - RTMP[S] 发布服务器，支持录制发布流
  - RTMP[S] 播放器，支持RTMP代理，支持生成静音音频
  - RTMP[S] 推流客户端
  - 支持http[s]-flv直播
  - 支持websocket-flv直播
  - 支持H264/H265/AAC/G711/OPUS编码，其他编码能转发但不能转协议
  - 支持[RTMP-H265](https://github.com/ksvc/FFmpeg/wiki)
  - 支持[RTMP-OPUS](https://github.com/xia-chu/ZLMediaKit/wiki/RTMP%E5%AF%B9H265%E5%92%8COPUS%E7%9A%84%E6%94%AF%E6%8C%81)

- HLS
  - 支持HLS文件生成，自带HTTP文件服务器
  - 通过cookie追踪技术，可以模拟HLS播放为长连接，可以实现HLS按需拉流、播放统计等业务
  - 支持HLS播发器，支持拉流HLS转rtsp/rtmp/mp4
  - 支持H264/H265/AAC/G711/OPUS编码

- HTTP[S]与WebSocket
  - 服务器支持`目录索引生成`,`文件下载`,`表单提交请求`
  - 客户端提供`文件下载器(支持断点续传)`,`接口请求器`,`文件上传器`
  - 完整HTTP API服务器，可以作为web后台开发框架
  - 支持跨域访问
  - 支持http客户端、服务器cookie
  - 支持WebSocket服务器和客户端
  - 支持http文件访问鉴权

- GB28181与RTP推流
  - 支持UDP/TCP国标RTP(PS或TS)推流服务器，可以转换成RTSP/RTMP/HLS等协议
  - 支持RTSP/RTMP/HLS转国标推流客户端，支持TCP/UDP模式，提供相应restful api
  - 支持H264/H265/AAC/G711/OPUS编码

- MP4点播与录制
  - 支持录制为FLV/HLS/MP4
  - RTSP/RTMP/HTTP-FLV/WS-FLV支持MP4文件点播，支持seek
  - 支持H264/H265/AAC/G711/OPUS编码
  
- 其他
  - 支持丰富的restful api以及web hook事件 
  - 支持简单的telnet调试
  - 支持配置文件热加载
  - 支持流量统计、推拉流鉴权等事件
  - 支持虚拟主机,可以隔离不同域名
  - 支持按需拉流，无人观看自动关断拉流
  - 支持先拉流后推流，提高及时推流画面打开率
  - 提供c api sdk
  - 支持FFmpeg拉流代理任意格式的流
  - 支持http api生成并返回实时截图
  - 支持按需解复用、转协议，当有人观看时才开启转协议
  
## 更新日志
  - 2020/5/17 新增支持hls播发器，支持hls拉流代理


## 编译以及测试
**编译前务必仔细参考wiki:[快速开始](https://github.com/xia-chu/ZLMediaKit/wiki/%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)操作!!!**

## 怎么使用

 你有三种方法使用ZLMediaKit，分别是：

 - 1、使用c api，作为sdk使用，请参考[这里](https://github.com/xia-chu/ZLMediaKit/tree/master/api/include).
 - 2、作为独立的流媒体服务器使用，不想做c/c++开发的，可以参考[restful api](https://github.com/xia-chu/ZLMediaKit/wiki/MediaServer%E6%94%AF%E6%8C%81%E7%9A%84HTTP-API)和[web hook](https://github.com/xia-chu/ZLMediaKit/wiki/MediaServer%E6%94%AF%E6%8C%81%E7%9A%84HTTP-HOOK-API).
 - 3、如果想做c/c++开发，添加业务逻辑增加功能，可以参考这里的[测试程序](https://github.com/xia-chu/ZLMediaKit/tree/master/tests).

## Docker 镜像

你可以从Docker Hub下载已经编译好的镜像并启动它：

```bash
docker run -id -p 1935:1935 -p 8080:80 -p 8554:554 -p 10000:10000 -p 10000:10000/udp panjjo/zlmediakit
```

你也可以根据Dockerfile编译镜像：

```bash
bash build_docker_images.sh
```

## 参考案例

 - [IOS摄像头实时录制,生成rtsp/rtmp/hls/http-flv](https://gitee.com/xia-chu/IOSMedia)
 - [IOS rtmp/rtsp播放器，视频推流器](https://gitee.com/xia-chu/IOSPlayer)
 - [支持linux、windows、mac的rtmp/rtsp播放器](https://github.com/xia-chu/ZLMediaPlayer)
 - [基于ZLMediaKit分支的管理WEB网站](https://github.com/chenxiaolei/ZLMediaKit_NVR_UI)
 - [基于ZLMediaKit主线的管理WEB网站](https://gitee.com/kkkkk5G/MediaServerUI)
 - [DotNetCore的RESTful客户端](https://github.com/MingZhuLiu/ZLMediaKit.DotNetCore.Sdk)
 - [C#版本的Http API与Hook](https://github.com/chengxiaosheng/ZLMediaKit.HttpApi)
 - [GB28181-2016网络视频平台](https://github.com/swwheihei/wvp)
 - [node-js版本的GB28181平台](https://gitee.com/hfwudao/GB28181_Node_Http)
 - [基于C SDK实现的推流客户端](https://github.com/hctym1995/ZLM_ApiDemo)
 - [Go实现的海康ehome服务器](https://github.com/tsingeye/FreeEhome)
 

## 授权协议

本项目自有代码使用宽松的MIT协议，在保留版权信息的情况下可以自由应用于各自商用、非商业的项目。
但是本项目也零碎的使用了一些其他的开源代码，在商用的情况下请自行替代或剔除；
由于使用本项目而产生的商业纠纷或侵权行为一概与本项项目及开发者无关，请自行承担法律风险。

## 联系方式

 - 邮箱：<1213642868@qq.com>(本项目相关或流媒体相关问题请走issue流程，否则恕不邮件答复)
 - QQ群：542509000

## 怎么提问？

如果要对项目有相关疑问，建议您这么做：

 - 1、仔细看下readme、wiki，如果有必要可以查看下issue.
 - 2、如果您的问题还没解决，可以提issue.
 - 3、有些问题，如果不具备参考性的，无需在issue提的，可以在qq群提.
 - 4、QQ私聊一般不接受无偿技术咨询和支持([为什么不提倡QQ私聊](https://github.com/xia-chu/ZLMediaKit/wiki/%E4%B8%BA%E4%BB%80%E4%B9%88%E4%B8%8D%E5%BB%BA%E8%AE%AEQQ%E7%A7%81%E8%81%8A%E5%92%A8%E8%AF%A2%E9%97%AE%E9%A2%98%EF%BC%9F)).

## 致谢

感谢以下各位对本项目包括但不限于代码贡献、问题反馈、资金捐赠等各种方式的支持！以下排名不分先后：

[老陈](https://github.com/ireader)
[Gemfield](https://github.com/gemfield)
[南冠彤](https://github.com/nanguantong2)
[凹凸慢](https://github.com/tsingeye)
[chenxiaolei](https://github.com/chenxiaolei)
[史前小虫](https://github.com/zqsong)
[清涩绿茶](https://github.com/baiyfcu)
[3503207480](https://github.com/3503207480)
[DroidChow](https://github.com/DroidChow)
[阿塞](https://github.com/HuoQiShuai)
[火宣](https://github.com/ChinaCCF)
[γ瑞γミ](https://github.com/JerryLinGd)
[linkingvision](https://www.linkingvision.com/)
[茄子](https://github.com/taotaobujue2008)
[好心情](<409257224@qq.com>)
[浮沉](https://github.com/MingZhuLiu)

## 捐赠

欢迎捐赠以便更好的推动项目的发展，谢谢您的支持!

[支付宝](https://gitee.com/xia-chu/other/raw/master/IMG_3919.JPG)

[微信](https://gitee.com/xia-chu/other/raw/master/IMG_3920.JPG)
