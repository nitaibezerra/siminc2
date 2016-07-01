<?php
header('content-type: text/html');

define('ON_WINDOWS', substr(PHP_OS, 0, 3) === 'WIN');
define('HEADER', '<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */
//
// +------------------------------------------------------------------------+
// | G1 - MVC Framework for PHP5                                            |
// | Copyright (c) 2005 The GCoders Group                                   |
// | All Right Reserved                                                     |
// +------------------------------------------------------------------------+
// | The contents of this file are subject to the Mozilla Public License    |
// | Version 1.1 (the "MPL"); you may not use this file except in           |
// | compliance with the License. You may obtain a copy of the License at   |
// | http://www.mozilla.org/MPL/                                            |
// |                                                                        |
// | Software distributed under the License is distributed on an "AS IS"    |
// | basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See    |
// | the License for the specific language governing rights and limitations |
// | under the License.                                                     |
// |                                                                        |
// | The Original Code is The GCoders Group.                                |
// | The Initial Developer of the Original Code is:                         |
// |     Douglas Gontijo <douglas@gcoders.net>                              |
// |                                                                        |
// | Alternatively, the contents of this file may be used under the terms   |
// | of the BSD License (BSD License), in which case the provisions of BSD  |
// | License are applicable instead of those above.                         |
// |                                                                        |
// | If you wish to allow use of your version of this file only under the   |
// | terms of the BSD License and not to allow others to use your version   |
// | of this file under the MPL, indicate your decision by deleting the     |
// | provisions above and replace them with the notice and other provisions |
// | required by the BSD License                                            |
// |                                                                        |
// | If you do not delete the provisions above, a recipient may use your    |
// | version of this file under either the MPL or the BSD License.          |
// +------------------------------------------------------------------------+
//
// $Id$
//


/**
 * @package %s
 * @class %s
 */


/**
 * @class %s
 *
 *
 * @version $Revision$
 * @author  Douglas Gontijo <douglas@gcoders.net>
 */
%s{
    //-------------------------------------------------------------- constants


    //------------------------------------------------------------- properties


    //----------------------------------------------------------------- public


    //-------------------------------------------------------------- protected


    //---------------------------------------------------------------- private


}





');



function createClass(array $data)
{
    $class = '';

    if (array_key_exists('abstract', $data) && $data['isclass'] == 'class')
        $class .= 'abstract ';

    if (array_key_exists('final', $data) && $data['isclass'] == 'class')
        $class .= 'final ';

    $class .= $data['isclass']   . ' ';

    if (trim($data['class']) != '')
        $class .= $data['class'] . ' ';
    else
        throw new Exception('You must specify a class or interface name.');

    if (trim($data['extention']) != '')
        $class .= 'extends '     . $data['extention']       . ' ';

    if (trim($data['implementations']) != '')
        $class .= 'implements '  . $data['implementations'] . ' ';

    $syserr = null;

    if (trim($data['application']) == '') {
        throw new Exception('You must specify the application path.');
    }

    if (!is_dir($data['application'])) {
        if (ON_WINDOWS) {
            mkdir($data['application'], 0777, true);
        } else {
            if (!system("mkdir -m 777 -p {$data['application']}", $syserr) && $syserr != 0)
                throw new Exception("Unable to create the application dir ({$data['application']}). $syserr");
        }
    }

    chdir($data['application']);

    $path = strtolower(str_replace('.', DIRECTORY_SEPARATOR, $data['package']));

    if (!is_dir($path))
        if (ON_WINDOWS) {
            mkdir($path, 0777, true);
        } else {
            if (!system("mkdir -m 777 -p {$path}", $syserr) && $syserr != 0)
                throw new Exception("Unable to create the package dir ({$path}).", $syserr);
        }

    $file = $path . DIRECTORY_SEPARATOR . $data['class'] . '.'
                  . (trim($data['extension'] != '') ? $data['extension'] : 'php');

    if (($fp = fopen($file, 'w+')) === false)
        throw new Exception("Unable to create the class file $file.", $syserr);

    $content = sprintf(HEADER, $data['package'],
                               $data['class'],
                               $data['class'],
                               $class);

    fwrite($fp, $content);
    fclose($fp);
    unset($fp);

    if (!ON_WINDOWS && !system("chmod 777 $file", $syserr) && $syserr != 0)
        throw new Exception("Unable to chmod $file to 777.", $syserr);
}


function throwPHPException($code, $message, $file, $line)
{
    try {
        throw new Exception($message, $code);
    } catch (Exception $e) {
        throwException($e);
    }
}


/**
 * Provides safe access to the information given by Exception::getTrace().
 * The array is non-null, with no null entries.
 *
 * \param Exception
 * \return array an array of stack trace information
 */
function getStackTrace($e)
{
    $bt = $e->getTrace();
    $st = array();

    foreach ($bt as $trace => $entry) {
        if (isset($entry['function']) &&
            $entry['function'] == 'throwPHPException') {
                continue;
        }

        $st[] = array_merge(array('file'     => 'unknown',
                                  'line'     => 0,
                                  'function' => null,
                                  'class'    => null,
                                  'type'     => null), $entry);
    }

    return $st;
}


/**
 * Catch an Exception (set_exception_handler).
 *
 * \param Exception Thrown Exception
 * \return void
 */
function throwException($ex, $file = null)
{
    $message   = $ex->getMessage();
    $code      = $ex->getCode();
    $file      = substr($ex->getFile(), strlen(dirname(__FILE__)) + 1);
    $line      = $ex->getLine();
    $data      = getStackTrace($ex);
    $exception = get_class($ex);
    $div       = 0;

    foreach ($data as $trace) {
        $_file      = str_replace(dirname(__FILE__), '', $trace['file']);
        $id         = $div % 2 == 0 ? 1 : 2;
        $traceDiv[] = "  <div class=\"stackTrace$id\">";
        $traceDiv[] = "    File .......: {$_file}:{$trace['line']}<br />";
        $traceDiv[] = "    Class/Method: {$trace['class']}{$trace['type']}{$trace['function']}";
        $traceDiv[] = "  </div>";

        $div++;
    }

    $trace  = implode("\n", $traceDiv);
    $html = <<<HTML
<?xml version="1.0" charset="ISO-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">
<html>
<head>
<style type="text/css" rel="stylesheet">
div {
  -moz-border-radius: 0.5em;
}
div#header {font-family: verdana;
  font-size: 20px;
  font-weight: bold;
  border: 1px solid #990000;
  color: #990000;
  background-color: #ffcc66;
  padding: 10px;
  margin-bottom: 15px
}
#exceptionDetails {
  border-top: 1px solid #990000;
  display: block;
  font-size: 12px;
  font-weight: bold;
  -moz-border-radius: 0em
}
.stackTrace1, .stackTrace2 {
  padding: 5px;
  font-family: "courier new";
  -moz-border-radius: 0em;
  border: 1px solid black;
  border-top: 0px;
  font-size: 14px;
  color: #000;
  font-weight: normal
}
.stackTrace1 {
  background-color: #fff
}
.stackTrace2 {
  background-color: #DDD
}
h2 {
  font-family: verdana;
  font-size: 18px;
  letter-spacing: 1px;
  font-weight: normal;
  border: 1px solid #990000;
  color: #ffcc66;
  background-color: #990000;
  padding-left: 10px;
  margin: 0
}
span.detail {
  font-weight: normal;
  display: block;
  font-family: "courier new";
  font-size: 14px
}
span.detail span.text {
  font-weight: bold
}
</style>
<title>G1 application has thrown an exception of type '$exception'</title>
</head>
<body>
<div id="header">
$message
<div id="exceptionDetails">
<span class="detail"><span class="text">Exception .:</span> $exception</span>
<span class="detail"><span class="text">Code ......:</span> $code</span>
<span class="detail"><span class="text">File ......:</span> $file</span>
<span class="detail"><span class="text">Line ......:</span> $line</span>
</div>
<p>
<h2>Stack Trace Entries</h2>
<!-- BEGIN stack_trace -->
$trace
<!-- END stack_trace -->
</p>
</div>
</body>
</html>
HTML;
    // jEdit becomes crazy because heredoc, I need this quote to fix it :P"
    header("content-type: text/html");
    echo $html;
    exit;
}

set_error_handler('throwPHPException');
set_exception_handler('throwException');

if (array_key_exists('data', $_POST) && is_array($_POST['data'])) {
    createClass($_REQUEST['data']);
}


?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
  <head>
    <title>Class creator</title>
    <style type="text/css">
    body {
        font-family: verdana;
        font-size: 10pt;
    }

    label.text {
        display: block;
        float: left;
        width: 150px;
        border-bottom: 1px dotted;
    }

    br.clearboth {
    }

    input[radio] {
    }

    input[type=text], select {
        border: 1px solid;
        font-size: 9pt;
    }

    input[type=text]:focus, select:focus {
        background-color: #eee;
    }

    p.buttons {
        border-top: 1px solid;
        text-align: center;
        padding: 5px;
    }
    </style>

    <script type="text/javascript">
      var showingClassItems = false;
      function showClassItems()
      {
          showingClassItems   = !showingClassItems;
          classItemsContainer =  document.getElementById('classItemsContainer');
          if (showingClassItems) {
              classItemsContainer.style.visibility = 'hidden';
              classItemsContainer.style.display = 'none';
              //classItemsContainer.style.display = 'none';
          } else {
              classItemsContainer.style.visibility = 'visible';
              classItemsContainer.style.display = 'block';
              //classItemsContainer.style.display = 'none';
          }
      }
    </script>
  </head>
  <body>
    <div>
      <form action="" method="post">
        <fieldset>
          <legend>Class creator</legend>
          <p>
            <label class="text" for="application">Application</label>
            <input tabindex="1" type="text" name="data[application]" id="application" />
          </p>
          <p>
            <label class="text" for="package">Package</label>
            <input tabindex="2" type="text" name="data[package]" id="package" />
          </p>
          <p>
            <label class="text" for="class">Class/Interface name</label>
            <input tabindex="3" type="text" name="data[class]" id="class" />
            <label for="extension">.</label>
            <input tabindex="0" type="text" style="width: 30px" maxlength="5" name="data[extension]" id="extension" value="php" />
          </p>
          <p>
            <label class="text" for="extention">Extention</label>
            <input tabindex="4" type="text" name="data[extention]" id="extention" />
          </p>
          <p>
            <label class="text" for="implementations">Implementations of</label>
            <input tabindex="5" type="text" name="data[implementations]" id="implementations" />
          </p>
          <p>
            <input type="radio" name="data[isclass]" id="isclass" checked="checked" value="class" onclick="return showClassItems();" />
            <label class="radiolabel" for="isclass">class</label>
            <input type="radio" name="data[isclass]" id="isinterface" value="interface" onclick="return showClassItems();" />
            <label class="radiolabel" for="isinterface">interface</label>
          </p>

          <p id="classItemsContainer">
            <input type="checkbox" name="data[abstract]" id="abstract" />
            <label class="radiolabel" for="abstract">abstract</label>
            <input type="checkbox" name="data[final]" id="final" />
            <label class="radiolabel" for="final">final</label>
          </p>

          <p class="buttons">
            <input tabindex="7" type="reset" name="reset" value="Clear all" />
            <input tabindex="6" type="submit" name="submit" value="Create" />
          </p>

        </fieldset>
      </form>
    </div>
  </body>
</html>

