var cpp = cpp || {};

cpp.currentHealth = 5;
cpp.maxHealth = 5;

cpp.setHP = function(hp) {
  cpp.currentHealth = hp;

  if (cpp.currentHealth < 0) {
    cpp.currentHealth = 0;
  }
  else if (cpp.currentHealth > cpp.maxHealth) {
    cpp.currentHealth = cpp.maxHealth;
  }
};

function updateUI() {
  var hpPercent = Math.floor((cpp.currentHealth / cpp.maxHealth) * 100);

  setHP(hpPercent);

  requestAnimationFrame(updateUI);
}

$(function() {
  $(document).keypress(function(e) {
    if (e.which === 65 || e.which === 97) { // a pressed
      cpp.currentHealth--;

      if (cpp.currentHealth < 0) {
        cpp.currentHealth = 0;
      }
    }
  });

  requestAnimationFrame(updateUI);
});

window.cpp = cpp;