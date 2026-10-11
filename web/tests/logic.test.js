'use strict';
/**
 * playbricks 网页版核心逻辑测试
 *
 * 所有断言的期望值都直接从 C++ 原版推导：
 *   - src/sprite/Sprite.cpp / Ball.cpp / Paddle.cpp
 *   - src/scene/GameScene.cpp
 *   - src/game/Config.cpp
 * 运行：node web/tests/logic.test.js
 */

// ---- 在加载被测模块前准备浏览器全局桩（测试专用的最小桩） ----
function createFakeStorage() {
  const map = new Map();
  return {
    getItem: (k) => (map.has(k) ? map.get(k) : null),
    setItem: (k, v) => map.set(k, String(v)),
    removeItem: (k) => map.delete(k),
    _map: map,
  };
}
global.window = { localStorage: createFakeStorage() };
global.document = { body: { className: '' } };

const assert = require('assert');
const { Sprite, Ball, Paddle, Brick, computeBallSpeed, computePaddleSpeed, resolveCollision } = require('../js/sprites.js');
const { Config } = require('../js/config.js');
const Game = require('../js/game.js');

// ---- 极简测试骨架 ----
const tests = [];
const test = (name, fn) => tests.push({ name, fn });

// ---- 公共桩 ----
const fakeImages = {
  ball: { width: 16, height: 16 },
  paddle: { width: 96, height: 16 },
  bricks: [
    { width: 64, height: 32 }, // 简单：绿
    { width: 64, height: 32 }, // 中等：蓝
    { width: 64, height: 32 }, // 困难：红
  ],
};

const config = Config.getInstance();
function resetConfig() {
  global.window.localStorage._map.clear();
  config.init(); // 回到默认值：0 / None / default / 30fps
}

function makeGame(overrides) {
  const events = { gameOver: [] };
  const directorCalls = [];
  const game = Game.createGame({
    width: 1000, // 与 Window::setFixedSize(1000, 940) 一致
    height: 940,
    scoreTextHeight: 20,
    config,
    images: fakeImages,
    director: { showScene: (id) => directorCalls.push(id) },
    onGameOver: (payload) => events.gameOver.push(payload),
    requestFrame: () => 0,
    cancelFrame: () => {},
    now: () => 0,
    ctx: null,
    ...overrides,
  });
  return { game, events, directorCalls };
}

// ============================================================
// sprites.js：与 C++ 精灵类逐行对应
// ============================================================

test('Ball.move 撞左墙：x 钳到 0，dx 反向', () => {
  const ball = new Ball(3, 100, 16, 16, null, -5, 0, true);
  ball.move(0, 1000, 20);
  assert.strictEqual(ball.getX(), 0);
  assert.strictEqual(ball.getDx(), 5);
  assert.strictEqual(ball.getY(), 100);
});

test('Ball.move 撞右墙：x 钳到 right-width，dx 反向', () => {
  const ball = new Ball(990, 100, 16, 16, null, 5, 0, true);
  ball.move(0, 1000, 20);
  assert.strictEqual(ball.getX(), 984); // 1000 - 16
  assert.strictEqual(ball.getDx(), -5);
});

test('Ball.move 撞顶墙：y 钳到 up，dy 反向；底部不反弹（落地由游戏处理）', () => {
  const ball = new Ball(500, 15, 16, 16, null, 0, -5, true);
  ball.move(0, 1000, 20);
  assert.strictEqual(ball.getY(), 20);
  assert.strictEqual(ball.getDy(), 5);
  // 底部无边界反弹
  const ball2 = new Ball(500, 935, 16, 16, null, 0, 5, true);
  ball2.move(0, 1000, 20);
  assert.strictEqual(ball2.getY(), 940);
  assert.strictEqual(ball2.getDy(), 5);
});

test('Paddle.move 按键移动并被左右边界钳制', () => {
  const paddle = new Paddle(452, 914, 96, 16, null, 15, false, false);
  paddle.setIsLefting(true);
  paddle.move(0, 1000);
  assert.strictEqual(paddle.getX(), 437);
  paddle.setIsLefting(false);
  paddle.setIsRighting(true);
  paddle.move(0, 1000);
  assert.strictEqual(paddle.getX(), 452);
  // 越界钳制：先移动后钳制（与 C++ Paddle::move 一致）
  paddle.setIsRighting(false);
  paddle.setIsLefting(true);
  paddle.setX(5);
  paddle.move(0, 1000); // 5 - 15 = -10 → 钳到 0
  assert.strictEqual(paddle.getX(), 0);
  paddle.setIsLefting(false);
  paddle.setIsRighting(true);
  paddle.setX(995);
  paddle.move(0, 1000); // 995 + 15 = 1010 → 钳到 904
  assert.strictEqual(paddle.getX(), 904); // 1000 - 96
});

test('Sprite.isCollided 与 C++ 角点判定完全一致', () => {
  const a = new Sprite(100, 100, 16, 16, null);
  const b = new Sprite(90, 90, 64, 32, null);
  assert.strictEqual(a.isCollided(b), true); // 100>=74 && 100<=154 && 100>=74 && 100<=122
  const c = new Sprite(200, 100, 16, 16, null);
  assert.strictEqual(c.isCollided(b), false); // 200 > 154
});

test('computeBallSpeed = (难度*3+5)*30/fps 整数除法', () => {
  assert.strictEqual(computeBallSpeed(0, 30), 5);
  assert.strictEqual(computeBallSpeed(0, 60), 2); // 150/60=2.5 → 2
  assert.strictEqual(computeBallSpeed(1, 30), 8);
  assert.strictEqual(computeBallSpeed(1, 60), 4); // 240/60=4
  assert.strictEqual(computeBallSpeed(2, 30), 11);
  assert.strictEqual(computeBallSpeed(2, 60), 5); // 330/60=5.5 → 5
});

test('computePaddleSpeed = (15-难度*2)*30/fps 整数除法', () => {
  assert.strictEqual(computePaddleSpeed(0, 30), 15);
  assert.strictEqual(computePaddleSpeed(0, 60), 7); // 450/60=7.5 → 7
  assert.strictEqual(computePaddleSpeed(1, 30), 13);
  assert.strictEqual(computePaddleSpeed(2, 30), 11);
  assert.strictEqual(computePaddleSpeed(2, 60), 5); // 330/60=5.5 → 5
});

test('resolveCollision：重叠宽 > 重叠高 → 只翻 dy（上下相撞）', () => {
  const ball = new Ball(500, 620, 16, 16, null, 5, 5, true);
  const brick = new Sprite(460, 624, 64, 32, null);
  resolveCollision(ball, brick); // overWidth=16 > overHeight=12
  assert.strictEqual(ball.getDy(), -5);
  assert.strictEqual(ball.getDx(), 5);
});

test('resolveCollision：重叠宽 < 重叠高 → 只翻 dx（左右相撞）', () => {
  const ball = new Ball(452, 630, 16, 16, null, 5, 5, true);
  const brick = new Sprite(460, 624, 64, 32, null);
  resolveCollision(ball, brick); // overWidth=8 < overHeight=16
  assert.strictEqual(ball.getDx(), -5);
  assert.strictEqual(ball.getDy(), 5);
});

test('resolveCollision：重叠相等 → dx dy 同时反向（极端情况完全反射）', () => {
  const ball = new Ball(452, 616, 16, 16, null, 5, 5, true);
  const brick = new Sprite(460, 624, 64, 32, null);
  resolveCollision(ball, brick); // overWidth=8 == overHeight=8
  assert.strictEqual(ball.getDx(), -5);
  assert.strictEqual(ball.getDy(), -5);
});

// ============================================================
// config.js：与 Config.cpp 对应，持久化到 localStorage
// ============================================================

test('Config 无存档时使用默认设置（0 分 / None / default / 30fps）', () => {
  resetConfig();
  assert.strictEqual(config.getHighestScore(), 0);
  assert.strictEqual(config.getHighestScorePlayer(), 'None');
  assert.strictEqual(config.getFps(), 30);
  assert.strictEqual(document.body.className, 'theme-default');
});

test('Config 读取已保存的配置（字段名与 config.ini 一致）', () => {
  global.window.localStorage._map.clear();
  global.window.localStorage.setItem('playbricks-config', JSON.stringify({
    HighestScore: 123,
    HighestScorePlayer: 'Alice',
    ThemeResourceUrl: ':/theme/Resources/theme/black.qss',
    Fps: 45,
  }));
  config.init();
  assert.strictEqual(config.getHighestScore(), 123);
  assert.strictEqual(config.getHighestScorePlayer(), 'Alice');
  assert.strictEqual(config.getFps(), 45);
  assert.strictEqual(document.body.className, 'theme-black');
});

test('Config 读取的 fps 钳制在 30~60', () => {
  global.window.localStorage._map.clear();
  global.window.localStorage.setItem('playbricks-config', JSON.stringify({ Fps: 100 }));
  config.init();
  assert.strictEqual(config.getFps(), 60);
  global.window.localStorage.setItem('playbricks-config', JSON.stringify({ Fps: 10 }));
  config.init();
  assert.strictEqual(config.getFps(), 30);
});

test('Config 损坏的存档按默认设置处理，不崩溃', () => {
  global.window.localStorage._map.clear();
  global.window.localStorage.setItem('playbricks-config', '{不是json');
  config.init();
  assert.strictEqual(config.getHighestScore(), 0);
  assert.strictEqual(config.getFps(), 30);
});

test('setFps / setHighestScore / setHighestScorePlayer 会写回存储', () => {
  resetConfig();
  config.setFps(50);
  config.setHighestScore(77);
  config.setHighestScorePlayer('Bob');
  const saved = JSON.parse(global.window.localStorage.getItem('playbricks-config'));
  assert.deepStrictEqual(saved, {
    HighestScore: 77,
    HighestScorePlayer: 'Bob',
    ThemeResourceUrl: ':/theme/Resources/theme/default.qss',
    Fps: 50,
  });
});

test('setTheme 切换 body 主题类并持久化', () => {
  resetConfig();
  config.setTheme('gray');
  assert.strictEqual(document.body.className, 'theme-gray');
  const saved = JSON.parse(global.window.localStorage.getItem('playbricks-config'));
  assert.strictEqual(saved.ThemeResourceUrl, ':/theme/Resources/theme/gray.qss');
});

// ============================================================
// game.js：createGame 游戏核心（对应 GameScene.cpp）
// ============================================================

test('init(0) 简单难度：挡板/球/砖块初始值与 C++ 公式一致', () => {
  resetConfig();
  const { game, directorCalls } = makeGame();
  game.init(0);
  const paddle = game.getPaddle();
  const ball = game.getBall();
  assert.strictEqual(paddle.getX(), 452); // 1000/2 - 96/2
  assert.strictEqual(paddle.getY(), 914); // 940 - 16 - 10
  assert.strictEqual(paddle.getDx(), 15); // (15-0)*30/30
  assert.strictEqual(ball.getX(), 492); // 452 + 48 - 8
  assert.strictEqual(ball.getY(), 888); // 914 - 16 - 10
  assert.strictEqual(ball.getDx(), 5); // (0*3+5)*30/30
  assert.strictEqual(ball.getDy(), 5);
  assert.strictEqual(ball.getIsMoving(), false);
  const bricks = game.getBricks();
  assert.strictEqual(bricks.length, 10); // 0*4+10 行
  assert.strictEqual(bricks[0].length, 15); // 15 列（原版 16 列×64=1024 超出窗口，最右半块；网页版改 15 列整块显示）
  assert.strictEqual(bricks[0][0].getX(), 20); // (1000 - 15*64) / 2 水平居中
  assert.strictEqual(bricks[0][0].getY(), 20); // scoreTextHeight
  assert.strictEqual(bricks[9][14].getX(), 916); // 20 + 14*64
  assert.strictEqual(bricks[9][14].getY(), 308); // 20 + 9*32
  assert.ok(bricks[0][14].getX() + 64 <= 1000); // 最右一列完整落在窗口内
  assert.strictEqual(bricks[0][0].image, fakeImages.bricks[0]); // 简单=绿
  assert.ok(directorCalls.includes('scene-game'));
  assert.strictEqual(game.isRunning(), true);
});

test('init(1)/init(2)：行数、球速、挡板速、砖块颜色随难度变化', () => {
  resetConfig();
  const g1 = makeGame().game;
  g1.init(1);
  assert.strictEqual(g1.getBricks().length, 14); // 1*4+10
  assert.strictEqual(g1.getBall().getDx(), 8); // (3+5)*30/30
  assert.strictEqual(g1.getPaddle().getDx(), 13); // (15-2)*30/30
  assert.strictEqual(g1.getBricks()[0][0].image, fakeImages.bricks[1]); // 中等=蓝
  assert.strictEqual(g1.getScore(), 0);

  const g2 = makeGame().game;
  g2.init(2);
  assert.strictEqual(g2.getBricks().length, 18); // 2*4+10
  assert.strictEqual(g2.getBall().getDx(), 11); // (6+5)*30/30
  assert.strictEqual(g2.getPaddle().getDx(), 11); // (15-4)*30/30
  assert.strictEqual(g2.getBricks()[0][0].image, fakeImages.bricks[2]); // 困难=红
});

test('难度参数取模：init(5) 等价 init(2)', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(5); // 5 % 3 = 2
  assert.strictEqual(game.getBricks().length, 18);
  assert.strictEqual(game.getBall().getDx(), 11);
});

test('任意键按下后球开始移动；auto-repeat 被过滤', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  game.keyDown('KeyA', false);
  assert.strictEqual(game.getBall().getIsMoving(), true);

  const g2 = makeGame().game;
  g2.init(0);
  g2.keyDown('KeyA', true); // isAutoRepeat
  assert.strictEqual(g2.getBall().getIsMoving(), false);
});

test('左键按住时 gameCycle 内挡板左移，keyUp 后停止', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  game.keyDown('ArrowLeft', false);
  game.gameCycle();
  assert.strictEqual(game.getPaddle().getX(), 437); // 452 - 15
  game.gameCycle();
  assert.strictEqual(game.getPaddle().getX(), 422);
  game.keyUp('ArrowLeft');
  game.gameCycle();
  assert.strictEqual(game.getPaddle().getX(), 422); // 不再移动
  // 原版行为：按任意键（包括左右键）都会发球，球已按 dx=5 移动了 3 帧
  assert.strictEqual(game.getBall().getIsMoving(), true);
  assert.strictEqual(game.getBall().getX(), 507); // 492 + 5*3
});

test('长按左/右键挡板被钳制在 [0, 904]', () => {
  resetConfig();
  const left = makeGame().game;
  left.init(0);
  left.keyDown('ArrowLeft', false);
  for (let i = 0; i < 40; i++) left.gameCycle();
  assert.strictEqual(left.getPaddle().getX(), 0);

  const right = makeGame().game;
  right.init(0);
  right.keyDown('ArrowRight', false);
  for (let i = 0; i < 40; i++) right.gameCycle();
  assert.strictEqual(right.getPaddle().getX(), 904);
});

test('空格按住球速 ×3，松开恢复原速', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  game.keyDown('Space', false);
  assert.strictEqual(game.getBall().getDx(), 15);
  assert.strictEqual(game.getBall().getDy(), 15);
  game.keyUp('Space');
  assert.strictEqual(game.getBall().getDx(), 5);
  assert.strictEqual(game.getBall().getDy(), 5);
});

test('球撞左墙/右墙反弹（gameCycle 内）', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  const ball = game.getBall();
  ball.setX(2); ball.setY(500); ball.setDx(-5); ball.setDy(0); ball.setIsMoving(true);
  game.gameCycle();
  assert.strictEqual(ball.getX(), 0);
  assert.strictEqual(ball.getDx(), 5);

  ball.setX(990); ball.setDx(5);
  game.gameCycle();
  assert.strictEqual(ball.getX(), 984);
  assert.strictEqual(ball.getDx(), -5);
});

test('球落到挡板上反弹（dy 反向，先移动后判定）', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  const ball = game.getBall();
  ball.setX(492); ball.setY(895); ball.setDx(0); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle();
  // 本帧先移动到 900，再与挡板判定反弹
  assert.strictEqual(ball.getY(), 900);
  assert.strictEqual(ball.getDy(), -5);
  assert.strictEqual(game.getScore(), 0);
});

test('球撞砖块：砖块标记、得分 += 难度+1、dy 反向', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  const ball = game.getBall();
  // 本帧先移动到 (28,45)，从下方顶到砖块（砖块区居中，[0][0] 起始于 x=20）
  ball.setX(28); ball.setY(40); ball.setDx(0); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle();
  assert.strictEqual(game.getBricks()[0][0].getIsCollided(), true);
  assert.strictEqual(game.getScore(), 1); // 难度 0 → +1
  assert.strictEqual(ball.getDy(), -5); // overWidth=16 > overHeight=7 → 翻 dy
});

test('每帧最多判定一块砖（对应 goto outside）', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  const ball = game.getBall();
  // 先移动后判定（与 C++ gameCycle 顺序一致）：本帧移动后球位于 (76,45)，同时压住 [0][0] 和 [0][1]
  ball.setX(71); ball.setY(40); ball.setDx(5); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle();
  assert.strictEqual(game.getBricks()[0][0].getIsCollided(), true); // 行优先先判到 [0][0]
  assert.strictEqual(game.getBricks()[0][1].getIsCollided(), false);
  assert.strictEqual(game.getScore(), 1);
  assert.strictEqual(ball.getDy(), -5); // 对 [0][0]：overWidth=8 > overHeight=7 → 翻 dy
  assert.strictEqual(ball.getDx(), 5);  // dx 保持不变
});

test('球落地：停止循环并回调 onGameOver（非纪录）', () => {
  resetConfig();
  const { game, events } = makeGame();
  game.init(0);
  const ball = game.getBall();
  ball.setX(500); ball.setY(930); ball.setDx(0); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle();
  assert.strictEqual(game.isRunning(), false);
  assert.strictEqual(events.gameOver.length, 1);
  assert.deepStrictEqual(events.gameOver[0], { score: 0, isNewRecord: false });
});

test('球落地破纪录：先写入最高分，再回调 onGameOver(isNewRecord=true)', () => {
  resetConfig();
  const { game, events } = makeGame();
  game.init(1); // 每块 +2
  const ball = game.getBall();
  ball.setX(28); ball.setY(40); ball.setDx(0); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle();
  assert.strictEqual(game.getScore(), 2);
  ball.setX(500); ball.setY(930); ball.setDx(0); ball.setDy(5);
  game.gameCycle();
  assert.strictEqual(config.getHighestScore(), 2); // 与 C++ 一致：弹窗前先记分
  assert.strictEqual(events.gameOver.length, 1);
  assert.deepStrictEqual(events.gameOver[0], { score: 2, isNewRecord: true });
});

test('再来一局（resetRound(false)）：分数清零，砖块恢复，球回挡板待发', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  const ball = game.getBall();
  ball.setX(28); ball.setY(40); ball.setDx(0); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle(); // 得 1 分
  assert.strictEqual(game.getScore(), 1);
  game.resetRound(false);
  assert.strictEqual(game.getScore(), 0);
  assert.strictEqual(game.getBricks()[0][0].getIsCollided(), false);
  assert.strictEqual(ball.getX(), 492);
  assert.strictEqual(ball.getY(), 888);
  assert.strictEqual(ball.getDx(), 5);
  assert.strictEqual(ball.getDy(), 5);
  assert.strictEqual(ball.getIsMoving(), false);
  assert.strictEqual(game.getPaddle().getX(), 452);
  assert.strictEqual(game.getPaddle().getY(), 914);
});

test('砖块全部打完：分数保留累加，重置后继续（清关续玩）', () => {
  resetConfig();
  const { game } = makeGame();
  game.init(0);
  const ball = game.getBall();
  ball.setX(28); ball.setY(40); ball.setDx(0); ball.setDy(5); ball.setIsMoving(true);
  game.gameCycle(); // 打掉第一块，得 1 分
  assert.strictEqual(game.getScore(), 1);
  // 手动标记其余砖块为已打
  const bricks = game.getBricks();
  for (let i = 0; i < bricks.length; i++) {
    for (let j = 0; j < bricks[i].length; j++) {
      if (!(i === 0 && j === 0)) bricks[i][j].setIsCollided(true);
    }
  }
  game.gameCycle(); // 检测到全部打完 → resetRound(true)
  assert.strictEqual(game.getScore(), 1); // 分数不清零
  assert.strictEqual(game.getBricks()[0][0].getIsCollided(), false); // 砖块全部恢复
  assert.strictEqual(ball.getX(), 492);
  assert.strictEqual(ball.getY(), 888);
  assert.strictEqual(ball.getIsMoving(), false);
  assert.strictEqual(game.isRunning(), true); // 继续游戏
});

// ---- 运行 ----
(async () => {
  let failed = 0;
  for (const { name, fn } of tests) {
    try {
      await fn();
      console.log('  ✓ ' + name);
    } catch (err) {
      failed++;
      console.error('  ✗ ' + name);
      console.error('      ' + (err && err.message ? err.message : err));
    }
  }
  console.log(failed === 0 ? `\n全部 ${tests.length} 个测试通过` : `\n${failed}/${tests.length} 个测试失败`);
  process.exit(failed === 0 ? 0 : 1);
})();
