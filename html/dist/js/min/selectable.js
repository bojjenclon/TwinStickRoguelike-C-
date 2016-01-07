"use strict";var Selectable=Selectable||{};Selectable.parent=null,Selectable.marquee={element:null,x:0,y:0,width:0,height:0},Selectable.items=[],Selectable.mode="touch",Selectable.multiSelect=!1,Selectable.init=function(e){Selectable.parent=$("#selectable"),Selectable.mode=e.mode||"touch";for(var t=Selectable.parent.find("*"),l=0;l<t.length;l++){var a=$(t[l]);(a.hasClass("button")||a.is("input"))&&a.bind("mousedown",function(e){e.stopPropagation()})}for(var c=this.parent.find("li"),l=0;l<c.length;l++){var s=c[l];s.addClass("selectable-item"),Selectable.items.push(s)}this.marquee.element=$("<div></div>"),this.marquee.element.attr("id","selection-marquee"),this.parent.append(this.marquee.element),this.marquee.element.hide(),$(window).bind("keydown",function(e){(17===e.which||162===e.which||163===e.which)&&(Selectable.multiSelect=!0)}),$(window).bind("keyup",function(e){(17===e.which||162===e.which||163===e.which)&&(Selectable.multiSelect=!1)});var i=0;this.parent.bind("mousedown",function(e){if(e.preventDefault(),Selectable.multiSelect===!1)for(var t=Selectable.parent.find(".selected"),l=0;l<t.length;l++)$(t[l]).removeClass("selected");i=setTimeout(function(){Selectable.marquee.element.show(),Selectable.marquee.x=e.pageX,Selectable.marquee.y=e.pageY,Selectable.marquee.width=0,Selectable.marquee.height=0,Selectable.marquee.element.css("left",Selectable.marquee.x),Selectable.marquee.element.css("top",Selectable.marquee.y)},100)}),this.parent.bind("mousemove",function(e){if(e.preventDefault(),"none"!==Selectable.marquee.element.css("display")){Selectable.marquee.width=Math.abs(Selectable.marquee.x-e.pageX),Selectable.marquee.height=Math.abs(Selectable.marquee.y-e.pageY),e.pageX<Selectable.marquee.x&&Selectable.marquee.element.css("left",e.pageX),e.pageY<Selectable.marquee.y&&Selectable.marquee.element.css("top",e.pageY),Selectable.marquee.element.css("width",Selectable.marquee.width),Selectable.marquee.element.css("height",Selectable.marquee.height);for(var t=Selectable.marquee.element.offset().left,l=Selectable.marquee.element.offset().top,a=0;a<Selectable.items.length;a++){var c=Selectable.items[a],s=c.offset().left,i=c.offset().top,m=c.outerWidth(),n=c.outerHeight();"touch"===Selectable.mode?s+m>t&&t+Selectable.marquee.width>s&&i+n>l&&l+Selectable.marquee.height>i?c.addClass("selected"):Selectable.multiSelect===!1&&c.hasClass("selected")&&c.removeClass("selected"):s>=t&&i>=l&&s+m<=t+Selectable.marquee.width&&i+n<=l+Selectable.marquee.height?c.addClass("selected"):Selectable.multiSelect===!1&&c.hasClass("selected")&&c.removeClass("selected")}}}),this.parent.bind("mouseup mouseleave",function(e){e.preventDefault(),clearTimeout(i),Selectable.marquee.element.css("left",0),Selectable.marquee.element.css("top",0),Selectable.marquee.element.css("width",0),Selectable.marquee.element.css("height",0),Selectable.marquee.element.hide()})},Selectable.addItem=function(e){e.addClass("selectable-item"),e.click(function(t){t.preventDefault(),e.toggleClass("selected")}),Selectable.items.push(e)},Selectable.removeSelected=function(e){for(var t=$("#selectable").find(".selected"),l=0;l<t.length;l++){var a=$(t[l]);a.fadeOut(250,function(){this.remove()})}setTimeout(e,500)};