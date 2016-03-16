<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:prn_gerenaca.inc
   Finalidade: permitir a construção de relatorio padronizado de dados de ação
    */
/*
session_cache_limiter('private_no_expire');
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-cache");
header("Pragma: no-cache");
session_start(); */
header('Content-type: application/xls');
header('Content-Disposition: attachment; filename="planilha.xls"');
for ($i=0;$i<count($_SESSION['cab']);$i++)
{
	if ($i<count($_SESSION['cab'])-1) print trim(($_SESSION['cab'][$i])). ' ' . "\t";
	else print($_SESSION['cab'][$i]);
}
print chr(13);
//print_r($_SESSION['conteudo']);

for ($i=0;$i<count($_SESSION['conteudo']);$i++)
{
    foreach($_SESSION['conteudo'][$i] as $k=>$v)
    {
       ${$k}=$v;
       if ($v == 't') $v='1';
       else if ($v=='f') $v='0';
       if (! is_int($k) and $v <>'') 
       {
          if (is_numeric($v)) $v=number_format($v,2,',','.');//$v=formata_numero($v);
          //$v=utf8_encode(htmlspecialchars($v));
          print $v."\t";
       }
	}
	print chr(13);
}



?>
