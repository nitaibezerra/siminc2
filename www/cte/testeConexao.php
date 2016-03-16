<?php
 $sk=fsockopen('172.20.65.115','8201',$errnum,$errstr,TIMEOUT) ;
  if (!is_resource($sk)) {
  $erro_conexao = "Erro de Conexao:".$errnum." ".$errstr;
  }
	
?>