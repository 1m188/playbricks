'use strict';
/**
 * 场景管理：对应 src/game/Director 与 src/scene 下的各菜单场景。
 * 菜单场景用 DOM 实现；游戏场景（GameScene）见 game.js。
 * 各菜单按钮的文案与跳转关系与 Qt 版一致，包括主题/帧数场景里
 * 「返回开始界面」按钮同样是回到开始场景（原版如此）。
 */

// 场景切换器（对应 Director）
const Director = (function () {
  let nowScene = 'scene-start';

  function showScene(id) {
    const scenes = document.querySelectorAll('.scene');
    for (let i = 0; i < scenes.length; i++) {
      if (scenes[i].id === id) {
        scenes[i].classList.remove('hidden');
      } else {
        scenes[i].classList.add('hidden');
      }
    }
    nowScene = id;
  }

  return {
    showScene: showScene,
    get nowScene() { return nowScene; },
  };
})();

(function () {
  const config = Config.getInstance();

  // ---- 开始场景（对应 StartScene）----
  document.getElementById('btn-start-new-game').addEventListener('click', () => {
    Director.showScene('scene-difficulty');
  });
  document.getElementById('btn-start-highest-score').addEventListener('click', () => {
    refreshHighestScore();
    Director.showScene('scene-highest');
  });
  document.getElementById('btn-start-setting').addEventListener('click', () => {
    Director.showScene('scene-settings');
  });
  document.getElementById('btn-start-exit').addEventListener('click', () => {
    // 对应 QApplication::quit；浏览器通常不允许脚本关闭非脚本打开的标签页，给出告别提示
    window.close();
    setTimeout(() => {
      document.getElementById('dialog-exit').classList.remove('hidden');
    }, 150);
  });
  document.getElementById('dialog-exit-back').addEventListener('click', () => {
    document.getElementById('dialog-exit').classList.add('hidden');
  });

  // ---- 难度选择场景（对应 DifficultyChooseScene）----
  const difficultyButtons = document.querySelectorAll('.difficulty-button');
  for (let i = 0; i < difficultyButtons.length; i++) {
    difficultyButtons[i].addEventListener('click', (event) => {
      // 通过各自预先设置的 data-difficulty 可以直接拿到难度系数并用作初始化
      PlaybricksGame.start(parseInt(event.currentTarget.dataset.difficulty, 10));
    });
  }

  // ---- 设置场景（对应 SettingScene）----
  document.getElementById('btn-settings-theme').addEventListener('click', () => {
    Director.showScene('scene-theme');
  });
  document.getElementById('btn-settings-fps').addEventListener('click', () => {
    syncFpsScene();
    Director.showScene('scene-fps');
  });

  // ---- 主题选择场景（对应 ThemeChooseScene）----
  const themeButtons = document.querySelectorAll('.theme-button');
  for (let i = 0; i < themeButtons.length; i++) {
    themeButtons[i].addEventListener('click', (event) => {
      config.setTheme(event.currentTarget.dataset.theme);
    });
  }

  // ---- 帧数选择场景（对应 FpsChooseScene）----
  const fpsSlider = document.getElementById('fps-slider');
  const fpsValue = document.getElementById('fps-value');

  function syncFpsScene() {
    fpsSlider.value = String(config.getFps());
    fpsValue.textContent = String(config.getFps());
  }

  // fps 值显示的 label 上的显示值随 slider 滑动而变化
  fpsSlider.addEventListener('input', () => {
    fpsValue.textContent = fpsSlider.value;
  });
  // 直到用户释放 slider 确定之后才更改设置里的 fps 值（对应 sliderReleased）
  fpsSlider.addEventListener('change', () => {
    config.setFps(parseInt(fpsSlider.value, 10));
  });

  // ---- 最高分场景（对应 HighestScoreScene）----
  function refreshHighestScore() {
    document.getElementById('highest-score-label').textContent =
      config.getHighestScore() + '分     ' + config.getHighestScorePlayer();
  }

  // ---- 各场景的「返回开始界面」按钮 ----
  const backButtons = document.querySelectorAll('[data-target]');
  for (let i = 0; i < backButtons.length; i++) {
    backButtons[i].addEventListener('click', (event) => {
      Director.showScene(event.currentTarget.dataset.target);
    });
  }

  // 显示开始场景（对应 Window::show 放入开始场景）
  Director.showScene('scene-start');
})();
