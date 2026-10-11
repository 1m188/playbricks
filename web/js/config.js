'use strict';
/**
 * 全局设置类：对应 src/game/Config.h / Config.cpp
 * 与 Qt 版一致，配置字段名保持 HighestScore / HighestScorePlayer /
 * ThemeResourceUrl / Fps，区别是持久化位置从 config.ini 换成 localStorage。
 * Qt 版在程序退出时统一保存（uninit），网页版改为每次修改立即保存，
 * 避免页面意外关闭丢失设置。
 */
const Config = (function () {
  const STORAGE_KEY = 'playbricks-config';

  function Config() {
    // 初始化默认设置（与 Config::init 一致）
    this.highestScore = 0;           // 最高分
    this.highestScorePlayer = 'None'; // 最高分保持者
    this.theme = 'default';          // 当前主题：default | black | gray | yellow
    this.fps = 30;                   // 帧数
  }

  // 初始化所有的设置：读取存档，不存在则使用默认值
  Config.prototype.init = function () {
    this.highestScore = 0;
    this.highestScorePlayer = 'None';
    this.theme = 'default';
    this.fps = 30;

    try {
      const raw = window.localStorage.getItem(STORAGE_KEY);
      if (raw !== null) {
        // 解析 json
        const json = JSON.parse(raw);
        if (typeof json.HighestScore === 'number') {
          this.highestScore = json.HighestScore; // 读取最高分
        }
        if (typeof json.HighestScorePlayer === 'string') {
          this.highestScorePlayer = json.HighestScorePlayer; // 读取最高分保持者
        }
        if (typeof json.ThemeResourceUrl === 'string') {
          // 读取主题资源样式表保存路径，如 :/theme/Resources/theme/black.qss
          const match = json.ThemeResourceUrl.match(/([a-zA-Z]+)\.qss$/);
          if (match) {
            this.theme = match[1];
          }
        }
        if (typeof json.Fps === 'number') {
          this.fps = json.Fps; // 读取帧数
        }
      }
    } catch (err) {
      // 存档损坏时按默认设置继续游戏
    }

    // 控制读取进来的 fps 范围
    if (this.fps > 60) {
      this.fps = 60;
    } else if (this.fps < 30) {
      this.fps = 30;
    }

    // 开始的时候设置当前主题
    this.applyTheme();
  };

  // 保存已有设置（对应 Config::uninit）
  Config.prototype.uninit = function () {
    this.save();
  };

  // json 格式写入存档
  Config.prototype.save = function () {
    try {
      window.localStorage.setItem(STORAGE_KEY, JSON.stringify({
        HighestScore: this.highestScore,             // 写入最高分
        HighestScorePlayer: this.highestScorePlayer, // 写入最高分保持者
        ThemeResourceUrl: ':/theme/Resources/theme/' + this.theme + '.qss', // 写入当前主题资源url
        Fps: this.fps,                               // 写入当前FPS值
      }));
    } catch (err) {
      // 隐私模式下 localStorage 可能不可用，不影响游戏进行
    }
  };

  // 把主题应用到页面（对应 qApp->setStyleSheet）
  Config.prototype.applyTheme = function () {
    document.body.className = 'theme-' + this.theme;
  };

  // 设置当前主题
  Config.prototype.setTheme = function (theme) {
    this.theme = theme;
    this.applyTheme();
    this.save();
  };

  Config.prototype.setHighestScore = function (highestScore) {
    this.highestScore = highestScore;
    this.save();
  };

  Config.prototype.getHighestScore = function () {
    return this.highestScore;
  };

  Config.prototype.setHighestScorePlayer = function (highestScorePlayer) {
    this.highestScorePlayer = highestScorePlayer;
    this.save();
  };

  Config.prototype.getHighestScorePlayer = function () {
    return this.highestScorePlayer;
  };

  Config.prototype.setFps = function (fps) {
    this.fps = fps;
    this.save();
  };

  Config.prototype.getFps = function () {
    return this.fps;
  };

  let initialized = false;
  const instance = new Config();

  return {
    // 获取单例（对应 Config::getInstance；首次调用时读取存档，对应 main 中的 Config::init）
    getInstance: function () {
      if (!initialized) {
        initialized = true;
        instance.init();
      }
      return instance;
    },
  };
})();

// 供 Node 冒烟测试加载（浏览器中无副作用）
if (typeof module !== 'undefined' && module.exports) {
  module.exports = { Config };
}
