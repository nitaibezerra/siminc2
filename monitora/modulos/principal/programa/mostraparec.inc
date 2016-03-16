<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:motraparec.inc
   finalidade: permitir a visualização em tela cheia do parecer
   */
 // achar os atributo de acao que interessam
     $sql= "select avptexto as texto from avaliacaoparecer where avpid= ".md5_decrypt($_REQUEST['cod'],'');
     
	 $RS = $db->record_set($sql);
     $res = $db->carrega_registro($RS,0);
	 print '<font face="Verdana" size="1">'.$res['texto'].'</font>';
?>
