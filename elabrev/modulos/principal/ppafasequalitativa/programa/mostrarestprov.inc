<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:motrarestprov.inc
   finalidade: permitir a visualização em tela cheia da restrição ou da providência
   */
 // achar os atributo de acao que interessam
 if ($_REQUEST['tp']==1)
     $sql= "select rspdscrestricao as texto from restricaoprograma where rspnum= ".md5_decrypt($_REQUEST['cod'],'');
     else $sql= "select rspdscprovidencia as texto from restricaoprograma where rspnum= ".md5_decrypt($_REQUEST['cod'],'');
	 $RS = $db->record_set($sql);
     $res = $db->carrega_registro($RS,0);
	 print '<font face="Verdana" size="1">'.$res['texto'].'</font>';
?>
