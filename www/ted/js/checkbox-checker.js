/**
 * Created by LucasGomes on 09/09/14.
 */
var ControleCheckbox = window.ControleCheckbox = {};
ControleCheckbox = function(html, target, dependent) {
    this.el = $(html);
    this.dad = this.el.find(target);
    this.child = this.el.find(dependent);
    this.ControllerCheckbox();
};

ControleCheckbox.prototype.ControllerCheckbox = function() {
    this.dad.on("click", $.proxy(this.SelectDad, this));
    this.child.livequery("click", $.proxy(this.SelectChild, this));
};

ControleCheckbox.prototype.SelectDad = function(e) {
    if (this.dad.prop("checked") === true) {
        this.markUnmark(true);
    } else {
        this.markUnmark(false);
    }
};

ControleCheckbox.prototype.SelectChild = function() {
    if (!$(this.child.selector).is(":checked")) {
        $(this.dad.selector).prop("checked", false);
    } else {
        $(this.dad.selector).prop("checked", true);
    }

    var that = this;
    $(this.child.selector).each(function(i, el) {
        if ($(el).prop("checked") == false) {
            $(that.dad.selector).prop("checked", false);
        }
    });

    this.addButton();
};

ControleCheckbox.prototype.markUnmark = function(flag) {
    $(this.child.selector).each(function(index, element) {
        $(element).prop("checked", flag ? true : false);
    });
    this.addButton();
};

ControleCheckbox.prototype.addButton = function() {
    var $containerButton = $(".container-button");

    if (!$containerButton.length) return false;

    if ($(this.child.selector).is(":checked")) {
        $containerButton.find("#addNc").remove();
        $containerButton.append(
            $("<button/>", {
                type:"button",
                class:"btn btn-info",
                name:"addNc",
                id:"addNc"
            }));
        $containerButton.find("#addNc").html("Cadastrar Nota de Crédito");
    } else {
        $containerButton.find("#addNc").remove();
    }
};
