<?php

/**
 * This script reorders the constants defined in the language file alphabetically.
 *
 * @version $Id: reorder.php 4 2005-12-13 01:47:15Z nicolas $
 * @copyright 2005 
 */
$handle = fopen('eng.php', 'r+');
$new_file = fopen('neweng.php', 'x');
$defines = array();
while (!feof($handle)) {
    $line = fgets($handle);
    if (eregi('^define', $line)) {
        $defines[substr($line, 0, 40)] = $line;
    }
}

ksort($defines);
fwrite($new_file, 
"<?php
/**
 * File containing the English Word Constants.
 * 
 * @category Procedural
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com> 
 * @author Nicolas Connault, <nick@connault.com.au> 
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version CVS: \$Id: reorder.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */\n");


foreach ($defines as $define) {
   fwrite($new_file, $define);
}

fwrite($new_file, '?>');
fclose($handle);
fclose($new_file);
unlink('eng.php');
rename('neweng.php', 'eng.php');
?>