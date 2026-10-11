'use strict';
/**
 * 精灵类：对应 src/sprite/Sprite.h / Sprite.cpp / Ball / Paddle / Brick
 * 坐标与速度均为整数，保持与 Qt 版一致的逐帧移动语义。
 */

// 精灵类（对应 Sprite）
class Sprite {
  constructor(x, y, width, height, image) {
    // 左上角坐标
    this.x = x;
    this.y = y;
    // 宽高
    this.width = width;
    this.height = height;
    // 图片（HTMLImageElement 或 {width, height}）
    this.image = image;
  }

  getX() { return this.x; }
  setX(x) { this.x = x; }
  getY() { return this.y; }
  setY(y) { this.y = y; }
  getWidth() { return this.width; }
  setWidth(width) { this.width = width; }
  getHeight() { return this.height; }
  setHeight(height) { this.height = height; }

  // 碰撞检测：与 Sprite::isCollided 完全一致
  isCollided(s) {
    return this.x >= s.x - this.width && this.x <= s.x + s.width &&
      this.y >= s.y - this.height && this.y <= s.y + s.height;
  }
}

// 球（对应 Ball）
class Ball extends Sprite {
  constructor(x, y, width, height, image, dx, dy, isMoving) {
    super(x, y, width, height, image);
    this.dx = dx;
    this.dy = dy;
    this.isMoving = isMoving;
  }

  getDx() { return this.dx; }
  setDx(dx) { this.dx = dx; }
  getDy() { return this.dy; }
  setDy(dy) { this.dy = dy; }
  getIsMoving() { return this.isMoving; }
  setIsMoving(isMoving) { this.isMoving = isMoving; }

  // 与 Ball::move 一致：left/right 为左右边界，up 为顶部边界，底部不反弹
  move(left, right, up) {
    this.setX(this.getX() + this.dx);
    this.setY(this.getY() + this.dy);

    // 判断是否达到边界
    if (this.getX() < left) {
      this.setX(left);
      this.dx *= -1;
    } else if (this.getX() + this.getWidth() > right) {
      this.setX(right - this.getWidth());
      this.dx *= -1;
    }
    if (this.getY() < up) {
      this.setY(up);
      this.dy *= -1;
    }
  }
}

// 挡板（对应 Paddle）
class Paddle extends Sprite {
  constructor(x, y, width, height, image, dx, isLefting, isRighting) {
    super(x, y, width, height, image);
    this.dx = dx;
    this.isLefting = isLefting;
    this.isRighting = isRighting;
  }

  getDx() { return this.dx; }
  setDx(dx) { this.dx = dx; }
  getIsLefting() { return this.isLefting; }
  setIsLefting(isLefting) { this.isLefting = isLefting; }
  getIsRighting() { return this.isRighting; }
  setIsRighting(isRighting) { this.isRighting = isRighting; }

  // 与 Paddle::move 一致
  move(left, right) {
    // 判断左右移动
    if (this.isLefting) {
      this.setX(this.getX() - this.dx);
    }
    if (this.isRighting) {
      this.setX(this.getX() + this.dx);
    }

    // 判断是否到达边界
    if (this.getX() < left) {
      this.setX(left);
    } else if (this.getX() + this.getWidth() > right) {
      this.setX(right - this.getWidth());
    }
  }
}

// 砖块（对应 Brick）
class Brick extends Sprite {
  constructor(x, y, width, height, image, isCollided) {
    super(x, y, width, height, image);
    this.isCollided = isCollided;
  }

  getIsCollided() { return this.isCollided; }
  setIsCollided(isCollided) { this.isCollided = isCollided; }
}

// 球速：与 GameScene.init 中 (difficulty * 3 + 5) * 30 / fps 的整数除法一致
function computeBallSpeed(difficulty, fps) {
  return Math.floor((difficulty * 3 + 5) * 30 / fps);
}

// 挡板速：与 GameScene.init 中 (15 - difficulty * 2) * 30 / fps 的整数除法一致
function computePaddleSpeed(difficulty, fps) {
  return Math.floor((15 - difficulty * 2) * 30 / fps);
}

// 碰撞响应：与 GameScene::updateBallDxy 完全一致
// 比较球和物体重叠部分的宽与高：宽更大说明从上/下相撞（翻 dy），
// 高更大说明从左右相撞（翻 dx），相等则完全反射。
function resolveCollision(ball, s) {
  const overWidth = Math.min(ball.getX() + ball.getWidth(), s.getX() + s.getWidth()) - Math.max(ball.getX(), s.getX());
  const overHeight = Math.min(ball.getY() + ball.getHeight(), s.getY() + s.getHeight()) - Math.max(ball.getY(), s.getY());
  if (overWidth > overHeight) {
    ball.setDy(ball.getDy() * -1);
  } else if (overWidth < overHeight) {
    ball.setDx(ball.getDx() * -1);
  } else {
    ball.setDx(ball.getDx() * -1);
    ball.setDy(ball.getDy() * -1);
  }
}

// 供 Node 冒烟测试加载（浏览器中无副作用）
if (typeof module !== 'undefined' && module.exports) {
  module.exports = { Sprite, Ball, Paddle, Brick, computeBallSpeed, computePaddleSpeed, resolveCollision };
}
