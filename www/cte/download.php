<?php

header('Content-type: text/plain');

header('Content-Disposition: attachment; filename='.$_GET["file"].".txt");

readfile($_GET["file"].".txt");

?>