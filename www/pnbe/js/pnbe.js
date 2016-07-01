/**
 * Formata a cor da fonta da grid, baseado em classes css
 * @returns void
 * @autor: Lucas Gomes <lucass.web@gmail.com>
 */
var formatGridColor = function() {
    var objTrsAdd = $(".add-green").parent().parent()
      , objTrsDel = $(".del-red").parent().parent();

    for (var i in objTrsAdd) {
        if (objTrsAdd.hasOwnProperty(i))
            $(objTrsAdd[i]).addClass('row-add');
    }

    for (var i in objTrsDel) {
        if (objTrsDel.hasOwnProperty(i))
            $(objTrsDel[i]).addClass('row-del');
    }
};
    
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
                    args[1]();
                }
            };
        } else {
            opts = {
                url:url,
                success:function(responseText){
                    $(target).html(responseText);
                }
            };
        }
    }
    return $.ajax(opts);
};

/**
 * Alert message
 * @param string message
 * @returns void(0)
 * @autor: Lucas Gomes <lucass.web@gmail.com>
 */
var notice = function(message) {
    var $container
      , $documentBody = $(document.body);
    
    if (message) {
        $("#flash-notice").remove();
        $container = $('<div id="flash-notice"><center>' + message + "</center></div>").prependTo($documentBody);
    }
    
    $container
        .css({left: $documentBody.width() / 2 - $container.width() / 2})
        .show()
        .animate({top: 70}, 500, "swing")
        .delay(3500)
        .fadeOut(500, "swing", function() {});
};