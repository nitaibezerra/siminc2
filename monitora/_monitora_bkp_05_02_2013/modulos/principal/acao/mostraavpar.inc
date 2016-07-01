<?
 // achar os atributo de acao que interessam
// dbg($_REQUEST['cod']);
// dbg(md5_decrypt($_REQUEST['cod'],''),1);
     $sql= "select trim(avptexto) from avaliacaoparecer where avpid= ".md5_decrypt($_REQUEST['cod'],'');   
	 $avptexto=$db->pegaUm($sql);
	 print '<font face="Verdana" size="1">'.$avptexto.'</font>';
?>