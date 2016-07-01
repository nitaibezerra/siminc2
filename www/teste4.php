<?php
$teste = array();
$teste["a"] = 'teste';
$teste["a"] = $teste;
if (is_null($teste["b"])) print "nulo";
print $teste["a"]["a"];
?>