var minifier = require('minifier');
/*var input = ['js/jscrollpane/jquery.jscrollpane.js', 'js/jscrollpane/jquery.mousewheel.js', 'js/jscrollpane/mwheelintent.js'];
var options = {
  output: 'js/jscrollpane.min.js'
};*/
var input = ['js/selectable.js'];
var options = null;

minifier.on('error', function(err) {
  // handle any potential error
})

minifier.minify(input, options);
