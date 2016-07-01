<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 1
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print 'Ações SIDOR2<br>';

$sql="select * from elabrev.plan2";
$rs = $db->carregar( $sql );
$i=0;
if (  $rs && count($rs) > 0 )	
{  	
	foreach ( $rs as $linha )
	{
	   foreach($linha as $k=>$v) ${$k}=$v;
	$sql = "select acaid from elabrev.ppaacao_orcamento where acacod='".trim($acacod)."' and prgcod='".trim($prgcod)."' and unicod = '".trim($unicod)."' and loccod='".trim(substr($loccod,4,4))."'";
	$pegou = $db->pegaLinha($sql);
	if (! $pegou)
	{
	   print $sql.'<br>';
	   $sql="select acadsc from elabrev.ppaacao_orcamento where  acacod='".trim($acacod)."' and prgcod='".trim($prgcod)."' limit 1" ;
	   $dados = $db->pegaLinha($sql);
	   foreach($dados as $k=>$v) ${$k}=$v;
	   $sql = "insert into elabrev.ppaacao_orcamento (acacod, unicod, prgcod,acadsc, acastatus, loccod) values ('$acacod','$unicod','$prgcod','$acadsc','S','".trim(substr($loccod,4,4))."')";
	   print $sql.'<br>';
	   $db->executar($sql);
	   
	}
	$db->commit();

	}
}

// comparando ao contrario


$sql="select * from elabrev.ppaacao_orcamento";
$rs = $db->carregar( $sql );
$i=0;
if (  $rs && count($rs) > 0 )	
{  	
	foreach ( $rs as $linha )
	{
	   foreach($linha as $k=>$v) ${$k}=$v;
	$sql = "select acacod from elabrev.plan2 where trim(acacod)='".trim($acacod)."' and trim(prgcod)='".trim($prgcod)."' and trim(unicod) = '".trim($unicod)."' and substr(loccod,5,4)='".trim($loccod)."'";
	$pegou = $db->pegaLinha($sql);
	if (! $pegou)
	{
	   print "Programa = $prgcod - Ação = $acacod - Unidade= $unicod - Localizador=$loccod<br>";   
	}

	}
}



?>
