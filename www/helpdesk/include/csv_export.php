<?php
/**
 * @category  Procedural File
 * @package   Admin
 * @author    Ian Warner <iwarner@triangle-solutions.com>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version   SVN: $Id: csv_export.php 2 2005-12-13 01:08:15Z nicolas $
 * @since     File available since Release 1.1.1.1
 * \\||
 */
session_start();
header ("Content-type: text/csv");
header ("Content-type: application/octet-stream");
header ("Content-Disposition: attachment; filename=" . date('Ymd') . "_data.csv");
header ("Cache-Control: must-revalidate, post-check=0, pre-check=0");
header ("Pragma: no-cache");
header ("Expires: 0");

echo $_SESSION['csv'];
unset($_SESSION['cvs']);

exit();
?>