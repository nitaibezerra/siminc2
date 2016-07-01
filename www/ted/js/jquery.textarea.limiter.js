/**
 * Created 08/12/14.
 */
(function($){
    $.fn.limit  = function(options) {
        var defaults = {
            limit: 200,
            id_result: false,
            alertClass: false
        }
        var options = $.extend(defaults,  options);
        return this.each(function() {
            var characters = options.limit
              , textLength = $(this).val().length;
            if (options.id_result != false) {
                var characters_tmp = (characters-textLength);
                $("#"+options.id_result).append("Você tem <strong>"+  characters_tmp+"</strong> caracteres restantes");
            }
            $(this).keyup(function() {
                if ($(this).val().length > characters) {
                    $(this).val($(this).val().substr(0, characters));
                }
                if (options.id_result != false) {
                    var remaining =  characters - $(this).val().length;
                    $("#"+options.id_result).html("Você tem <strong>"+  remaining+"</strong> caracteres restantes");
                    if(remaining <= 10) {
                        $("#"+options.id_result).addClass(options.alertClass);
                    } else {
                        $("#"+options.id_result).removeClass(options.alertClass);
                    }
                }
            });
        });
    };
})(jQuery);