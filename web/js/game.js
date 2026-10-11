'use strict';
/**
 * 游戏场景：对应 src/scene/GameScene.h / GameScene.cpp
 *
 * 核心规则放在 createGame(deps) 工厂里（无 DOM 依赖，可在 Node 中测试）；
 * 文件底部的浏览器装配段负责画布、图片、弹窗与键盘事件。
 * 主循环用固定步长（所选 fps）驱动 gameCycle，与原版 QTimer(1000 / fps) 一致，
 * 速度公式按帧计算，因此改帧数只影响更新粒度、不影响实际速度（与 Qt 版相同）。
 */
const Game = (function () {
  // 精灵模块解析：浏览器中 sprites.js 先加载、类在全局作用域；
  // Node 测试环境通过 require 从同目录加载。
  const Sprites = (typeof module !== 'undefined' && module.exports)
    ? require('./sprites.js')
    : { Sprite, Ball, Paddle, Brick, computeBallSpeed, computePaddleSpeed, resolveCollision };

  // deps: {
  //   width, height, scoreTextHeight  —— 与 Window(1000, 940) 和 GameScene 一致
  //   config                          —— Config 单例
  //   images                          —— { ball, paddle, bricks: [绿, 蓝, 红] }
  //   director                        —— { showScene(id) }，场景切换
  //   onGameOver                      —— (payload: {score, isNewRecord}) => void，弹窗由 UI 胶水处理
  //   requestFrame / cancelFrame / now —— 帧调度（浏览器为 rAF / performance.now）
  //   ctx                             —— CanvasRenderingContext2D，测试环境可为 null
  // }
  function createGame(deps) {
    const WIDTH = deps.width;
    const HEIGHT = deps.height;
    const SCORE_TEXT_HEIGHT = deps.scoreTextHeight;

    let paddle = null;
    let ball = null;
    let bricksVector = [];
    let difficulty = 0;
    let nowScore = 0;
    let fps = 30;
    let interval = 1000 / 30;
    let running = false;
    let frameHandle = 0;
    let lastTime = 0;
    let accumulator = 0;

    // 对应 GameScene::init(difficulty)
    function init(difficultyParam) {
      // 初始化难度系数（防止传入的参数超过限制）
      difficulty = difficultyParam % 3;

      // 得分初始化
      nowScore = 0;

      // 帧数初始化
      fps = deps.config.getFps();
      interval = 1000 / fps;

      // 挡板初始化
      const paddleImage = deps.images.paddle;
      paddle = new Sprites.Paddle(
        Math.floor(WIDTH / 2 - paddleImage.width / 2),
        HEIGHT - paddleImage.height - 10,
        paddleImage.width,
        paddleImage.height,
        paddleImage,
        Sprites.computePaddleSpeed(difficulty, fps), // (15 - difficulty * 2) * 30 / fps
        false,
        false
      );

      // 球初始化
      const ballImage = deps.images.ball;
      ball = new Sprites.Ball(
        paddle.getX() + Math.floor(paddle.getWidth() / 2) - Math.floor(ballImage.width / 2),
        paddle.getY() - ballImage.height - 10,
        ballImage.width,
        ballImage.height,
        ballImage,
        Sprites.computeBallSpeed(difficulty, fps), // (difficulty * 3 + 5) * 30 / fps
        Sprites.computeBallSpeed(difficulty, fps),
        false
      );

      // 砖块初始化：difficulty * 4 + 10 行 × 15 列
      // 原版 16 列 × 64px = 1024px，超出 1000px 窗口导致最右一列只显示一半；
      // 网页版改为 15 列（15×64=960px）并水平居中，保证每块砖整块显示。
      const brickImage = deps.images.bricks[difficulty];
      const brickColumns = 15;
      const brickOffsetX = Math.floor((WIDTH - brickColumns * brickImage.width) / 2);
      bricksVector = [];
      for (let i = 0; i < difficulty * 4 + 10; i++) {
        const row = [];
        for (let j = 0; j < brickColumns; j++) {
          row.push(new Sprites.Brick(
            brickOffsetX + j * brickImage.width,
            SCORE_TEXT_HEIGHT + i * brickImage.height,
            brickImage.width,
            brickImage.height,
            brickImage,
            false
          ));
        }
        bricksVector.push(row);
      }

      deps.director.showScene('scene-game');

      // 初始化并启动定时器
      startLoop();
    }

    // 对应 GameScene::keyPressEvent
    function keyDown(code, isRepeat) {
      if (isRepeat) {
        return; // 对应 event.isAutoRepeat() 过滤
      }
      // 挡板向左移动
      if (code === 'ArrowLeft') {
        paddle.setIsLefting(true);
      }
      // 挡板向右移动
      else if (code === 'ArrowRight') {
        paddle.setIsRighting(true);
      }
      // 按住空格键球加速
      else if (code === 'Space') {
        ball.setDx(ball.getDx() * 3);
        ball.setDy(ball.getDy() * 3);
      }
      // 如果球没有移动的话，按任意键后球开始移动
      if (!ball.getIsMoving()) {
        ball.setIsMoving(true);
      }
    }

    // 对应 GameScene::keyReleaseEvent
    function keyUp(code) {
      // 挡板停止向左移动
      if (code === 'ArrowLeft') {
        paddle.setIsLefting(false);
      }
      // 挡板停止向右移动
      else if (code === 'ArrowRight') {
        paddle.setIsRighting(false);
      }
      // 释放空格键后球回到原来的速度
      else if (code === 'Space') {
        ball.setDx(Math.floor(ball.getDx() / 3));
        ball.setDy(Math.floor(ball.getDy() / 3));
      }
    }

    // 对应 GameScene::gameCycle
    function gameCycle() {
      // 计算状态
      // 挡板移动
      paddle.move(0, WIDTH);

      // 球移动
      if (ball.getIsMoving()) {
        ball.move(0, WIDTH, SCORE_TEXT_HEIGHT);
      }

      // 球是否和挡板碰撞
      if (ball.isCollided(paddle)) {
        Sprites.resolveCollision(ball, paddle);
      }

      // 球是否和砖块碰撞（每帧最多处理一块，对应原版 goto outside）
      outer: for (let i = 0; i < bricksVector.length; i++) {
        const row = bricksVector[i];
        for (let j = 0; j < row.length; j++) {
          const brick = row[j];
          if (ball.isCollided(brick) && !brick.getIsCollided()) {
            Sprites.resolveCollision(ball, brick);
            brick.setIsCollided(true);
            nowScore += difficulty + 1;
            break outer;
          }
        }
      }

      // 游戏是否结束
      if (ballLanded()) {
        // 球落地：停止更新画面
        stopLoop();
        handleBallLanded();
      } else if (allBricksCleared()) {
        // 砖块打完：除分数不重置外，其他的重置后继续（分数一直累加）
        resetRound(true);
      }

      // 绘制
      paint();
    }

    // 对应球落地判断：ball.getY() + ball.getHeight() >= height()
    function ballLanded() {
      return ball.getY() + ball.getHeight() >= HEIGHT;
    }

    // 遍历所有砖块，是否全部已被打掉
    function allBricksCleared() {
      for (let i = 0; i < bricksVector.length; i++) {
        const row = bricksVector[i];
        for (let j = 0; j < row.length; j++) {
          if (!row[j].getIsCollided()) {
            return false;
          }
        }
      }
      return true;
    }

    // 球落地结算：破纪录先写入最高分（与原版弹窗前记分一致），弹窗流程交给 UI 胶水
    function handleBallLanded() {
      const isNewRecord = nowScore > deps.config.getHighestScore();
      if (isNewRecord) {
        deps.config.setHighestScore(nowScore);
      }
      deps.onGameOver({ score: nowScore, isNewRecord: isNewRecord });
    }

    // 重新开局（对应 gameCycle 中两处重置代码）
    // keepScore=false：球落地后「再来一局」，分数清零
    // keepScore=true ：砖块打完后续玩，分数保留累加
    function resetRound(keepScore) {
      // 重新设置挡板状态
      paddle.setX(WIDTH / 2 - paddle.getWidth() / 2);
      paddle.setY(HEIGHT - paddle.getHeight() - 10);
      paddle.setIsLefting(false);
      paddle.setIsRighting(false);

      // 重新设置球的状态
      ball.setX(paddle.getX() + Math.floor(paddle.getWidth() / 2) - Math.floor(ball.getWidth() / 2));
      ball.setY(paddle.getY() - ball.getHeight() - 10);
      ball.setDx(Sprites.computeBallSpeed(difficulty, fps));
      ball.setDy(Sprites.computeBallSpeed(difficulty, fps));
      ball.setIsMoving(false);

      // 重新显示所有的砖块
      for (let i = 0; i < bricksVector.length; i++) {
        const row = bricksVector[i];
        for (let j = 0; j < row.length; j++) {
          row[j].setIsCollided(false);
        }
      }

      // 重新初始化分数
      if (!keepScore) {
        nowScore = 0;
      }
    }

    // 对应 GameScene::paintEvent
    function paint() {
      if (!deps.ctx) {
        return; // 测试环境无画布
      }
      const ctx = deps.ctx;

      // 清空画布，露出主题背景色（与 Qt 窗口背景一致）
      ctx.clearRect(0, 0, WIDTH, HEIGHT);

      // 绘制挡板
      ctx.drawImage(paddle.image, paddle.getX(), paddle.getY());

      // 绘制球
      ctx.drawImage(ball.image, ball.getX(), ball.getY());

      // 绘制砖块（没有被打过的才绘制）
      for (let i = 0; i < bricksVector.length; i++) {
        const row = bricksVector[i];
        for (let j = 0; j < row.length; j++) {
          const brick = row[j];
          if (!brick.getIsCollided()) {
            ctx.drawImage(brick.image, brick.getX(), brick.getY());
          }
        }
      }

      // 绘制分数：红色，微软雅黑 12pt，顶部 20px 区域内居中
      ctx.fillStyle = 'red';
      ctx.font = '16px "微软雅黑", "Microsoft YaHei", sans-serif';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText('分数：' + nowScore, WIDTH / 2, SCORE_TEXT_HEIGHT / 2);
    }

    // 启动固定步长主循环（对应 timer->start()）
    function startLoop() {
      running = true;
      lastTime = deps.now();
      accumulator = 0;
      const frame = () => {
        if (!running) {
          return;
        }
        const now = deps.now();
        accumulator += now - lastTime;
        lastTime = now;
        let steps = 0;
        while (accumulator >= interval && steps < 5) {
          accumulator -= interval;
          steps++;
          gameCycle();
          if (!running) {
            break;
          }
        }
        if (steps >= 5) {
          accumulator = 0; // 页面卡顿后不追帧
        }
        if (running) {
          frameHandle = deps.requestFrame(frame);
        }
      };
      frameHandle = deps.requestFrame(frame);
    }

    // 停止主循环（对应 timer->stop()）
    function stopLoop() {
      running = false;
      deps.cancelFrame(frameHandle);
    }

    return {
      init: init,
      gameCycle: gameCycle,
      keyDown: keyDown,
      keyUp: keyUp,
      startLoop: startLoop,
      stopLoop: stopLoop,
      resetRound: resetRound,
      ballLanded: ballLanded,
      allBricksCleared: allBricksCleared,
      paint: paint,
      getBall: () => ball,
      getPaddle: () => paddle,
      getBricks: () => bricksVector,
      getScore: () => nowScore,
      isRunning: () => running,
    };
  }

  return { createGame: createGame };
})();

// ============================================================================
// 浏览器装配（Node 测试环境没有 DOM，自动跳过）
// ============================================================================
(function () {
  if (typeof document === 'undefined' || typeof document.getElementById !== 'function') {
    return;
  }
  const canvas = document.getElementById('game-canvas');
  if (!canvas) {
    return;
  }

  // 图片资源（对应 GameScene.init 中的图片初始化）
  function loadImage(src) {
    const img = new Image();
    img.src = src;
    return img;
  }
  const images = {
    ball: loadImage('assets/ball.png'),
    paddle: loadImage('assets/paddle.png'),
    bricks: [
      loadImage('assets/block_green.png'),
      loadImage('assets/block_blue.png'),
      loadImage('assets/block_red.png'),
    ],
  };

  const dialogHighscore = document.getElementById('dialog-highscore');
  const dialogGameover = document.getElementById('dialog-gameover');
  const highscoreText = document.getElementById('dialog-highscore-text');
  const highscoreInput = document.getElementById('dialog-highscore-input');
  const gameoverText = document.getElementById('dialog-gameover-text');

  let pending = null; // 待处理的落地结算 { score, isNewRecord }

  // 游戏结束弹窗流程：破纪录先输入大名（QInputDialog），再询问是否再来一局（QMessageBox）
  function onGameOver(payload) {
    pending = payload;
    if (payload.isNewRecord) {
      highscoreText.textContent = '你的分数是' + payload.score + '，恭喜你成为最高分，请输入你的尊姓大名。';
      highscoreInput.value = '';
      dialogHighscore.classList.remove('hidden');
      highscoreInput.focus();
    } else {
      showGameoverDialog(payload.score);
    }
  }

  function showGameoverDialog(score) {
    gameoverText.textContent = '游戏结束，你的分数是' + score + '，是否以当前难度再来一局？';
    dialogGameover.classList.remove('hidden');
  }

  // 姓名确认后（确定/取消）进入游戏结束询问；取消时与 Qt 的 getText 一致记为空字符串
  function finishHighscoreDialog(name) {
    dialogHighscore.classList.add('hidden');
    Config.getInstance().setHighestScorePlayer(name);
    showGameoverDialog(pending.score);
  }

  const game = Game.createGame({
    width: canvas.width,
    height: canvas.height,
    scoreTextHeight: 20,
    config: Config.getInstance(),
    images: images,
    director: { showScene: (id) => Director.showScene(id) },
    onGameOver: onGameOver,
    requestFrame: (cb) => requestAnimationFrame(cb),
    cancelFrame: (handle) => cancelAnimationFrame(handle),
    now: () => performance.now(),
    ctx: canvas.getContext('2d'),
  });

  document.getElementById('dialog-highscore-ok').addEventListener('click', () => finishHighscoreDialog(highscoreInput.value));
  document.getElementById('dialog-highscore-cancel').addEventListener('click', () => finishHighscoreDialog(''));
  // 输入框内按回车等同点击确定（对应 QInputDialog 的回车确认）
  highscoreInput.addEventListener('keydown', (event) => {
    if (event.key === 'Enter') {
      event.preventDefault();
      finishHighscoreDialog(highscoreInput.value);
    }
  });
  // 再来一局：以当前难度重开（分数清零）
  document.getElementById('dialog-gameover-yes').addEventListener('click', () => {
    dialogGameover.classList.add('hidden');
    game.resetRound(false);
    game.startLoop();
  });
  // 否：回到开始场景
  document.getElementById('dialog-gameover-no').addEventListener('click', () => {
    dialogGameover.classList.add('hidden');
    Director.showScene('scene-start');
  });

  // 键盘事件：仅游戏进行中响应（对应 Window::keyPressEvent 转发给当前场景）
  window.addEventListener('keydown', (event) => {
    if (Director.nowScene !== 'scene-game' || !game.isRunning()) {
      return;
    }
    if (event.code === 'ArrowLeft' || event.code === 'ArrowRight' || event.code === 'Space') {
      event.preventDefault(); // 防止空格滚动页面、方向键移动焦点
    }
    game.keyDown(event.code, event.repeat);
  });
  window.addEventListener('keyup', (event) => {
    if (Director.nowScene !== 'scene-game' || !game.isRunning()) {
      return;
    }
    game.keyUp(event.code);
  });

  // 暴露给 scenes.js：难度选择按钮 → GameScene.init(difficulty)
  window.PlaybricksGame = {
    start: (difficulty) => game.init(difficulty),
  };
})();

// 供 Node 冒烟测试加载（浏览器中无副作用）
if (typeof module !== 'undefined' && module.exports) {
  module.exports = { createGame: Game.createGame };
}
