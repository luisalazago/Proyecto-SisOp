// For license information, see `https://assets.adobedtm.com/c5eace8ae526/e9826b4b25e6/fad3adf4f2c8/RC063d0e2f9eb1494faebeee00e0648390-source.js`.
_satellite.__registerScript('https://assets.adobedtm.com/c5eace8ae526/e9826b4b25e6/fad3adf4f2c8/RC063d0e2f9eb1494faebeee00e0648390-source.min.js', "\"undefined\"!=typeof jQuery&&(window._sdi=window._sdi||{},window._sdi.addEventHandler=window.addEventListener?function(e,i,t){e.addEventListener(i,t,!1)}:function(e,i,t){e.attachEvent(\"on\"+i,t)},window._sdi.addEventHandler(window,\"message\",function(e){var i={},t=0;-1!==e.origin.indexOf(\"loop=1\")?t=1:e.srcElement&&e.srcElement.playerOptions&&1==e.srcElement.playerOptions.loop&&(t=1);var n=0;if(e.srcElement&&e.srcElement.playerOptions&&1==e.srcElement.playerOptions.autoplay&&(n=1),e.origin.match(/youtube\\.com/)&&\"undefined\"!=typeof JSON&&!t&&!n){i=JSON.parse(e.data);var a=window._sdi.yt.videos;if(\"infoDelivery\"==i.event){var d,o={};for(var r in i.info.videoData&&((o=a[d=i.info.videoData.video_id]||{}).info=i.info,o.id=i.id,a[d]=o),window._sdi.yt.videos){(c=window._sdi.yt.videos[r]).id==i.id&&(o=c)}var s=window._sdi.yt.sc||{};o.info&&(1.5<Math.abs(i.info.currentTime-o.info.currentTime)&&!0===s.track&&(s.s().Media.stop(o.info.videoData.title,o.info.currentTime),\"paused\"!=o.state&&s.s().Media.play(o.info.videoData.title,i.info.currentTime)),o.info.currentTime=i.info.currentTime),2==i.info.playerState&&(s.s().Media.stop(o.info.videoData.title,o.info.currentTime),o.state=\"paused\"),1==i.info.playerState&&(o.started?!0===s.track&&s.s().Media.play(o.info.videoData.title,o.info.currentTime):(o.started=!0)===s.track&&(s.s().Media.open(o.info.videoData.title,o.info.duration,\"youtube\"),s.s().Media.play(o.info.videoData.title,o.info.currentTime)),o.state=\"playing\"),3==i.info.playerState&&o.started&&(!0===s.track&&s.s().Media.stop(o.info.videoData.title,o.info.currentTime),o.state=\"paused\"),0===i.info.playerState&&(!0===s.track&&(s.s().Media.stop(o.info.videoData.title,o.info.currentTime),s.s().Media.close(o.info.videoData.title)),o.state=\"completed\")}else if(\"onStateChange\"==i.event){for(var r in window._sdi.yt.videos){var c;(c=window._sdi.yt.videos[r]).id==i.id&&(o=c)}s=window._sdi.yt.sc||{};1==i.info&&(o.started?!0===s.track&&s.s().Media.play(o.info.videoData.title,o.info.currentTime):(o.started=!0)===s.track&&(s.s().Media.open(o.info.videoData.title,o.info.duration,\"youtube\"),\"paused\"!=o.state&&s.s().Media.play(o.info.videoData.title,i.info.currentTime)),o.state=\"playing\"),2==i.info&&(!0===s.track&&s.s().Media.stop(o.info.videoData.title,o.info.currentTime),o.state=\"paused\"),3==i.info&&o.started&&(!0===s.track&&s.s().Media.stop(o.info.videoData.title,o.info.currentTime),o.state=\"paused\"),0===i.info&&(!0===s.track&&(s.s().Media.stop(o.info.videoData.title,o.info.currentTime),s.s().Media.close(o.info.videoData.title)),o.state=\"completed\")}}}),window._sdi.loadScript=function(e,i){var t=document.createElement(\"script\");t.type=\"text/javascript\",i&&\"function\"==typeof i&&(t.readyState?t.onreadystatechange=function(){\"loaded\"!==t.readyState&&\"complete\"!==t.readyState||(t.onreadystatechange=null,i())}:t.onload=function(){i()}),t.src=e,document.getElementsByTagName(\"head\")[0].appendChild(t)},window._sdi.yt={stages:{},players:{},sc:{track:!0,s:function(){return s_gi(_satellite.getVar(\"rsid\"))}},init:function(){for(var e=document.getElementsByTagName(\"script\"),i=!1,t=(window._sdi.yt.videos,0);t<e.length;t++){(e[t].src||\"\").match(/youtube\\.com\\/iframe_api/)&&(i=!0)}!1===i?window._sdi.loadScript(\"//www.youtube.com/iframe_api\",function(){window._sdi.yt.ready(\"api\")}):window._sdi.yt.ready(\"api\")},ready:function(e){window._sdi.yt.stages[e]=!0,window._sdi.yt.stages.dom&&window._sdi.yt.stages.api&&window._sdi.yt.checkIframes()},checkIframes:function(){var o=window._sdi.yt.videos,r=1e3,s=0;jQuery('iframe[src*=\"youtube\"][data-sdiyt!=\"true\"]').each(function(e,i){if(-1!=i.src.indexOf(\"loop=1\"))var t=null;else{var n;t=i.src.match(/h?t?t?p?s?\\:?\\/\\/www\\.youtube(-nocookie)?\\.com\\/embed\\/([\\w-]{11})(?:\\?.*)?/)}if(t){n=2<t.length?t[2]:\"\",i.id||(i.id=n+s,s++);var a=!1;if(!i.src.match(/origin/)&&document.location.protocol,i.src.match(/enablejsapi=1/)||(i.src+=(-1<i.src.indexOf(\"?\")?\"&\":\"?\")+\"enablejsapi=1\",a=!0),\"undefined\"!=typeof jQuery&&a){var d=jQuery(this).clone();d.src=i.src,d.id=i.id,jQuery(this).replaceWith(d)}else\"undefined\"!=typeof YT&&\"undefined\"!=typeof YT.get?(ready=!0,o[n]={},(!YT.get(i.id)||\"undefined\"==typeof YT.get(i.id).v&&\"undefined\"!=typeof YT.get(i.id).v.videoId)&&(o[n].player=new YT.Player(i.id,{events:{onReady:function(){},onStateChange:function(){},videoId:i.id}})),\"true\"!==i.getAttribute(\"data-sdiyt\")&&i.setAttribute(\"data-sdiyt\",\"true\")):r=100}}),setTimeout(function(){window._sdi.yt.checkIframes()},r)},videos:{}},\"undefined\"!=typeof window._sdi&&\"undefined\"!=typeof window._sdi.addEventHandler&&window._sdi.addEventHandler(window,\"load\",function(){1<=jQuery('iframe[src*=\"youtube\"]').length?(window._sdi.yt.init(),window._sdi.yt.ready(\"dom\")):window._sdi.ytinterval=setInterval(function(){1<=jQuery('iframe[src*=\"youtube\"]').length&&(window._sdi.yt.init(),window._sdi.yt.ready(\"dom\"),clearInterval(window._sdi.ytinterval))},500)}));");