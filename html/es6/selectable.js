let Selectable = Selectable || {};

Selectable.parent = null;
Selectable.marquee = {
  element: null,
  x: 0,
  y: 0,
  width: 0,
  height: 0
};
Selectable.items = [];

/*  
    determines how sensitive items are to the marquee tool
    options:
      "cover" = the item must be completed inside the marquee
      "touch" = the item must simply be touching one part of the marquee
*/
Selectable.mode = 'touch';

Selectable.multiSelect = false;

Selectable.init = function(options) {
  Selectable.parent = $('#selectable');
  Selectable.mode = options.mode || 'touch';

  let children = Selectable.parent.find('*');

  for (let i = 0; i < children.length; i++) {
    let child = $(children[i]);

    if (!child.hasClass('button') && !child.is('input')) {
      continue;
    }

    child.bind('mousedown', function(e) {
      e.stopPropagation();
    });
  }

  let allItems = this.parent.find('li');

  for (let i = 0; i < allItems.length; i++) {
    let item = allItems[i];

    item.addClass('selectable-item');

    Selectable.items.push(item);
  }

  this.marquee.element = $('<div></div>');
  this.marquee.element.attr('id', 'selection-marquee');

  this.parent.append(this.marquee.element);

  this.marquee.element.hide();

  $(window).bind('keydown', function(e) {
    if (e.which === 17 || e.which === 162 || e.which === 163) {
      Selectable.multiSelect = true;
    }
  });

  $(window).bind('keyup', function(e) {
    if (e.which === 17 || e.which === 162 || e.which === 163) {
      Selectable.multiSelect = false;
    }
  });

  // simulate mouse-hold event
  let timeoutId = 0;

  this.parent.bind('mousedown', function(e) {
    e.preventDefault();

    if (Selectable.multiSelect === false) {
      let selected = Selectable.parent.find('.selected');
      for (let i = 0; i < selected.length; i++) {
        $(selected[i]).removeClass('selected');
      }
    }

    // after 100ms, fire mouse-hold function
    timeoutId = setTimeout(function() {
      Selectable.marquee.element.show();

      Selectable.marquee.x = e.pageX;
      Selectable.marquee.y = e.pageY;
      Selectable.marquee.width = 0;
      Selectable.marquee.height = 0;

      Selectable.marquee.element.css('left', Selectable.marquee.x);
      Selectable.marquee.element.css('top', Selectable.marquee.y);
    }, 100);
  });

  this.parent.bind('mousemove', function(e) {
    e.preventDefault();

    if (Selectable.marquee.element.css('display') !== 'none') {
      Selectable.marquee.width = Math.abs(Selectable.marquee.x - e.pageX);
      Selectable.marquee.height = Math.abs(Selectable.marquee.y - e.pageY);

      if (e.pageX < Selectable.marquee.x) {
        Selectable.marquee.element.css('left', e.pageX);
      }
      if (e.pageY < Selectable.marquee.y) {
        Selectable.marquee.element.css('top', e.pageY);
      }

      Selectable.marquee.element.css('width', Selectable.marquee.width);
      Selectable.marquee.element.css('height', Selectable.marquee.height);

      let marqueeLeft = Selectable.marquee.element.offset().left;
      let marqueeTop = Selectable.marquee.element.offset().top;

      for (let i = 0; i < Selectable.items.length; i++) {
        let item = Selectable.items[i];

        let itemLeft = item.offset().left;
        let itemTop = item.offset().top;
        let itemWidth = item.outerWidth();
        let itemHeight = item.outerHeight();

        if (Selectable.mode === 'touch') {
          if (marqueeLeft < itemLeft + itemWidth
              && marqueeLeft + Selectable.marquee.width > itemLeft
              && marqueeTop < itemTop + itemHeight
              && marqueeTop + Selectable.marquee.height > itemTop) {
            item.addClass('selected');
          }
          else if (Selectable.multiSelect === false && item.hasClass('selected')) {
            item.removeClass('selected');
          }
        }
        else {
          if (itemLeft >= marqueeLeft
              && itemTop >= marqueeTop
              && itemLeft + itemWidth <= marqueeLeft + Selectable.marquee.width
              && itemTop + itemHeight <= marqueeTop + Selectable.marquee.height) {
            item.addClass('selected');
          }
          else if (Selectable.multiSelect === false && item.hasClass('selected')) {
            item.removeClass('selected');
          }
        }
      }
    }
  });

  this.parent.bind('mouseup mouseleave', function(e) {
    e.preventDefault();

    // prevent mouse-hold event from firing
    clearTimeout(timeoutId);

    Selectable.marquee.element.css('left', 0);
    Selectable.marquee.element.css('top', 0);
    Selectable.marquee.element.css('width', 0);
    Selectable.marquee.element.css('height', 0);

    Selectable.marquee.element.hide();
  });
};

Selectable.addItem = function(item) {
  item.addClass('selectable-item');

  item.click(function(e) {
    e.preventDefault();

    item.toggleClass('selected');
  });

  Selectable.items.push(item);
};

Selectable.removeSelected = function(callback) {
  let allItems = $('#selectable').find('.selected');

  for (let i = 0; i < allItems.length; i++) {
    let item = $(allItems[i]);

    item.fadeOut(250, function() {
      this.remove();
    });
  }

  setTimeout(callback, 500);
};