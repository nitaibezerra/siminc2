/**
 * Generic Ajax function
 * @autor: Lucas Gomes <lucass.web@gmail.com>
 */
var callAjax = function(target, url, options) {
   var args = Array.prototype.slice.call(arguments, 0)
     , opts;

   if (arguments.length===1)
       opts = arguments[0] || {};
   else {

       if (typeof args[1] === "function") {
           opts = {
               url:args[0],
               success:function(responseText){
                   args[1](responseText);
               }
           };
       } else {
           opts = {
               url:url,
               success:function(responseText){
                   jQuery(target).html(responseText);
               }
           };
       }
   }
   return jQuery.ajax(opts);
};

/**
 * Alert message
 * @param string message
 * @returns void(0)
 * @autor: Lucas Gomes <lucass.web@gmail.com>
 */
var notice = function(message) {
   var $container
     , $documentBody = jQuery(document.body);

   if (message) {
       jQuery("#flash-notice").remove();
       $container = jQuery('<div id="flash-notice"><center>' + message + "</center></div>").prependTo($documentBody);
   }

   $container
       .css({left: $documentBody.width() / 2 - $container.width() / 2})
       .show()
       .animate({top: 70}, 500, "swing")
       .delay(2000)
       .fadeOut(500, "swing", function() {});
};