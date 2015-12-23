var minifier = require('minifier');
/*var input = ['js/jscrollpane/jquery.jscrollpane.js', 'js/jscrollpane/jquery.mousewheel.js', 'js/jscrollpane/mwheelintent.js'];
var options = {
  output: 'js/jscrollpane.min.js'
};*/
var input = ['js/InGameHud.js']; // set to current working file, listing multiple files will compile them into one output.js
var options = null;

minifier.on('error', function(err) {
  console.log(err);
})

minifier.minify(input, options);
