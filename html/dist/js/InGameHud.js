'use strict';

var HUD = HUD || {};

$(document).ready(function () {
  HUD.POSSIBLE_OWNERS = ['player', 'enemy'];

  HUD.player = {
    'hud': $('#playerHud'),
    'hpBar': $('#playerHud').find('#hpBar')
  };

  HUD.enemy = {
    'hud': $('#enemyHud'),
    'hpBar': $('#enemyHud').find('#hpBar')
  };

  HUD.resetHP = function () {
    window.cpp.setHP(5);
  };

  HUD.setHP = function (val, owner) {
    owner = owner || 'player';

    if (HUD.POSSIBLE_OWNERS.indexOf(owner) < 0) {
      return;
    }

    var hpMeter = HUD[owner].hpBar.children()[0];
    hpMeter.style.width = val + '%';
  };

  HUD.hideHud = function (owner) {
    if (owner) {
      HUD[owner].hud.hide();
    } else {
      HUD.player.hud.hide();
      HUD.enemy.hud.hide();
    }
  };

  HUD.showHud = function (owner) {
    if (owner) {
      HUD[owner].hud.show();
    } else {
      HUD.player.hud.show();
      HUD.enemy.hud.show();
    }
  };
});