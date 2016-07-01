/**
* @category  Javascript
* @package   Web1 Admin
* @author    Ian Warner <iwarner@triangle-solutions.com>
* @author    Nicolas Connault <nick@connault.com.au>
* @copyright (C) 2001 Triangle Solutions Ltd
* @version   SVN: $Id: admin.js 11 2005-12-15 03:23:20Z nicolas $
* @since     File available since Release 1.1.1.1
* \\||
*/
// TODO
// Tidy up this page like the php standards
// We have two text counter functions please remove one if not needed
// Add all comments to the functions

// CHANGE LOG
// [Nick] 14-12-2005 : Clean up according to standards and added PHPdocs
// [Nick] 14-11-2005 : Added char counting script (check_max_chars())


/**
* ? Not too sure what this is for ?
*
*/
if (self != top) {
    top.location.href = self.location.href;
}


/**
* @todo Document this function
*
*
*/
function jumpMenu(targ, selObj, restore) {
    eval(targ + ".location='" + selObj.options[selObj.selectedIndex].value + "'");
    if (restore) {
        selObj.selectedIndex = 0;
    }
}


/**
* @todo Document this function
*
*/
function deletethis(link) {
    var name = confirm('Before we continue are you sure you want to delete this?');

    if (name == true) {
        window.location = link;
    } else {
        return false;
    }
}


/**
* @todo Document this function
*
*/
function popwindow(popsrc, stuff) {
    if (!stuff) {
        videoWindow = window.open(popsrc, 'Popup', 'width=800, height=600, left=50, top=50, scrollbars=yes, toolbars=yes, menubar=yes, location=yes');
    } else {
        videoWindow = window.open(popsrc, 'Popup', stuff)
    }
}


/**
* @todo Document this function
*
*/
function blocking(nr, type) {
    var browser = navigator.appName

    if (browser == "Microsoft Internet Explorer") {
        type = 'block';
    }

    if (document.layers) {
        current = (document.layers[nr].display == 'none') ? type : 'none';
        document.layers[nr].display = current;
    } else if (document.all) {
        current = (document.all[nr].style.display == 'none') ? type : 'none';
        document.all[nr].style.display = current;
    } else if (document.getElementById) {
        vista = (document.getElementById(nr).style.display == 'none') ? type : 'none';
        document.getElementById(nr).style.display = vista;
    }
}


/**
* Highlights a row when mouseover and converts the onclick behaviour
* to a link based on the first <a> tag found.
*
* @param string xTableId the Table's id attribute
*/
function ConvertRowsToLinks(xTableId) {

    var rows = document.getElementById(xTableId).rows;

    for (i = 0; i < rows.length; i++) {
        var event = '';
        if(rows[i].innerHTML.match("nohighlight") == null) {
            rows[i].onmouseover = new Function("this.className='row_highlight'");
            rows[i].onmouseout = new Function("this.className=''");
        }

        inputs = rows[i].getElementsByTagName('input');
        if (inputs.length > 0 && inputs[0].type == 'checkbox') {
            var checkbox = inputs[0];
            var id = checkbox.id;
        } else if(rows[i].innerHTML.match("nolink")) {
            // If <!-- nohighlight --> is found within the row, do not convert to link
        } else {
            var link = rows[i].getElementsByTagName('a');
            if (link.length != 0) {
                rows[i].onclick = new Function("this.className='row_click'; document.location.href='" + link[0].href + "'");
            } else {
                rows[i].onclick = new Function("this.className='row_click';");
            }
        }
    }
}


/**
* When a row is clicked, inverts any checkbox on that row.
*
* @param event
* @param row
*/
function invertCheckBox(event, row) {
    if (!event) var event = alert(window.event);

    // Determine which element was clicked. If it is a checkbox, do nothing
    var targ;
    if (event.target) {
        targ = event.target;
    } else if (event.srcElement) {
        targ = event.srcElement;
    }
    if (targ.nodeType == 3) { // defeat Safari bug
        targ = targ.parentNode
    }
    var tname;
    tname = targ.tagName;
    if (tname != "INPUT") {
        var inputs = row.getElementsByTagName('input');
        for (i = 0; i < inputs.length; i++) {
            if (inputs[i].type == 'checkbox') {
                var checkbox = inputs[i];
                checkbox.click();
            }
        }
    }
}


/**
*
*
*/
function check_max_chars(max_chars, element, list_id, event) {
    if (element.value.length > max_chars) {
        element.value = element.value.substring(0, max_chars);
        document.getElementById('max_' + list_id).style.display = 'block';
        document.getElementById('show_count_' + list_id).style.display = 'none';
    } else {
        document.getElementById('show_count_' + list_id).style.display = 'inline';
        document.getElementById('max_' + list_id).style.display = 'none';

        // Detect which key was pressed. If delete (unicode 8), use -1
        if (event.keyCode == '8') {
            document.getElementById('count_' + list_id).value = (max_chars - element.value.length + 1);
        } else {
            document.getElementById('count_' + list_id).value = (max_chars - element.value.length - 1);
        }

        if (document.getElementById('count_' + list_id).value < 0) {
            document.getElementById('count_' + list_id).value = 0;
        } else if (document.getElementById('count_' + list_id).value > max_chars) {
            document.getElementById('count_' + list_id).value = max_chars;
        }
    }
}


/**
* In a given html <form>, ticks all the checkboxes.
*/
function check_all(form) {
    for (var c = 0; c < form.elements.length; c++) {
        if (form.elements[c].type == 'checkbox') {
            form.elements[c].checked = true;
        }
    }
}


/**
* In a given html <form>, unticks all the checkboxes.
*/
function uncheck_all(form) {
    for (var c = 0; c < form.elements.length; c++) {
        if (form.elements[c].type == 'checkbox') {
            form.elements[c].checked = false;
        }
    }
}


/**
* In a given html <form>, ticks all the unticked checkboxes and
* unticks all the ticked ones: reverses the selection.
*/
function invert_selection(form) {
    for (var c = 0; c < form.elements.length; c++) {
        if (form.elements[c].type == 'checkbox') {
            if(form.elements[c].checked == true) {
                form.elements[c].checked = false;
            } else {
                form.elements[c].checked = true;
            }
        }
    }
}


/**
* Checks whether any checkbox has been ticked before taking an action
*/
function check_ticks(form, element) {
    for (var c = 0; c < form.elements.length; c++) {
        if (form.elements[c].type == 'checkbox') {
            if(form.elements[c].checked == true) {
                return true;
            }
        }
    }
    window.alert('You need to select at least one ' + element + ' before performing a global action');
    return false;
}


/**
* Checks the state of a checkbox and updates a given input box accordingly
*/
function update_counter(checkbox, input, number) {
    var input = document.getElementById(input);
    var total = parseInt(input.value);
    var number = parseInt(number);
    if (checkbox.checked == true) {
        total += number;
    } else {
        total -= number;
    }
    input.value = total;
}

/**
* This is an equivalent to PHP's in_array, except
* that it takes a third argument, the attribute. This
* way you can send an array of HTML elements and search
* for a matching attribute like 'id' or 'class'
*/
function in_array(needle, haystack, attribute)
{
    for (var i = 0; i < haystack.length; i++) {
        if (eval("haystack[i]." + attribute + ";") == needle) {
            return true;
        }
    }
    return false;
}