<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase4_sidor.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 4 quantitativa
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

/*
print 'acao orçamento<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPAAcao_SIDOR.xml", "elabrev.ppaacao_orcamento2",'ACA','ppa');
$sql='update elabrev.ppaacao_orcamento2 set prgid=p.prgid from elabrev.ppaprograma_orcamento p where p.prgcod=ppaacao_orcamento2.prgcod and p.prgano=ppaacao_orcamento2.prgano and ppaacao_orcamento2.prgid is null';
$saida = $db->executar($sql);
$db -> commit();

// tratando os localizadores

print 'Localizadores da Ação<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPALocalizador_SIDOR.xml", "elabrev.ppalocalizador_orcamento2",'LOC','ppa');
// abro a tabela de localizaores_orcamento e trato registro a registro
// ordeno a tabela pelo acacodppa
 

$sql = "update elabrev.ppalocalizador_orcamento2  set acaid=a.acaid from elabrev.ppaacao_orcamento2 a where elabrev.ppalocalizador_orcamento2.acacodppa=a.acacodppa and elabrev.ppalocalizador_orcamento2.prgcod=a.prgcod and elabrev.ppalocalizador_orcamento2.prgano=a.prgano and elabrev.ppalocalizador_orcamento2.acaid is null";
$saida = $db->executar($sql);
$db -> commit();
*/
/*
$sql = "select l.locppaid,l.regcod,l.loccod,l.acacodppa,l.prgcod,l.prgano,l.loccodppa from elabrev.ppalocalizador_orcamento2 l order by l.acacodppa";
 
$rs = @$db->carregar( $sql );
if (  $rs && count($rs) > 0 )
{
	$i=0;
	foreach ( $rs as $linha )
	{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 {
		 	// aqui busco cada ação para ver se já existe o campo atualizado
		 	$sql = "select a.regcod,a.loccod from elabrev.ppaacao_orcamento2 a where a.acacodppa='$acacodppa' and a.prgcod='$prgcod' and a.prgano='".$_SESSION['exercicio']."' ";
		 	if (! $db->pegaUm($sql))
		 	{
		 		$sql = "update elabrev.ppaacao_orcamento2 set acactrl='A',regcod='$regcod',loccod='$loccod' where acacodppa='$acacodppa' and prgcod='$prgcod' and prgano='".$_SESSION['exercicio']."' ";
		 		$sql2 = "update elabrev.ppalocalizador_orcamento2 set locctrl='A' where locppaid=$locppaid ";
		 		$saida = $db->executar($sql);
		 		$saida = $db->executar($sql2);
		 	}
		 }
  		 $i++;
	}
	$db -> commit();
}
*/

// fase 3
// criação das ações

$sql = "select l.locppaid,l.regcod as regcod2,l.loccod as loccod2,l.acacodppa as acacodppa2,l.prgcod ,l.prgano,l.loccodppa from elabrev.ppalocalizador_orcamento2 l where locctrl is null order by l.acacodppa";
 
$rs = @$db->carregar( $sql );
if (  $rs && count($rs) > 0 )
{
	$i=0;
	foreach ( $rs as $linha )
	{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 {
		 	// aqui busco cada ação para ver se já existe o campo atualizado
		 	$sql = "select * from elabrev.ppaacao_orcamento2 where acacodppa=$acacodppa limit 1";
		 	$rs2 = @$db->carregar( $sql );
		 	foreach ( $rs2 as $linha2 ){
		 		foreach($linha2 as $k=>$v) {if (! $v) $v='' ; 
		 		
		 		${$k}=$v;}
		 		
		 	}
		 	$sql = "insert into elabrev.ppaacao_orcamento2 (acacod,saccod,loccod,esfcod ,unicod,
  unitpocod,
  tincod,
  funcod ,
  unmcod ,
  regcod ,
  taccod ,
  osicod ,
  procod ,
  prgano ,
  prgcod ,
  prgid ,
  acacodppa,
  sitcodestagio,
  sitcodandamento ,
  sitcodcronograma ,
  acapercexecucao ,
  acadsc ,
  sacdsc ,
  acadsccomentarios ,
  acanomecoordenador ,
  acadscunresp ,
  acadscunexecutora ,
  acasnmedireta,
  acasnmedesc ,
  acasnmelincred ,
  acasnmetanaocumulativa ,
  acamesinicio ,
  acaanoinicio ,
  acamestermino ,
  acaanotermino ,
  acavlrrealateanoanterior ,
  acadsccomentsituacao,
  acadscsituacaoatual ,
  acadscresultadosespobt ,
  acamesprevisaoconclusao ,
  acaanoprevisaoconclusao ,
  acadsccomentexecfisica,
  acadsccomentexecfinanceira ,
  acadsccomentexecfisicabgu ,
  acadsccomentexecfinanceirabgu ,
  acasnrap  ,
  acadsccomentexecucao ,
  acadsccomentexecucaorap ,
  acasnfiscalseguridade ,
  acasninvestatais ,
  acasnoutrasfontes ,
  cod_referencia ,
  acadscproduto ,
  acafinalidade ,
  acadescricao ,
  acabaselegal ,
  acarepercfinanceira ,
  acasnpadronizada ,
  acasnsetpadronizada ,
  acasntransfobrigatoria ,
  acasntransfvoluntaria ,
  acasntransfoutras ,
  acasndespesaobrigatoria ,
  acasnbloqueioprogramacao ,
  acadetalhamento ,
  acavlrcustototal ,
  acavlrcustoateanoanterior ,
  acaqtdprevistoanocorrente ,
  acaordemprioridade ,
  acaobs ,
  acacodsof ,
  acaqtdcustototal ,
  acacodreferenciasof ,
  acavlrrepercfinanceira ,
  sfucod ,
  acastatus  ,
  acasnemenda  ,
  acasnestrategica  ,
  acaqtdateanoanterior ,
  acavlrcustoprevistoanocorrente ,
  acasnbgu  ,
  acaptres ,
  ungcod ,
  justificativa ,
  acaqtdefisico ,
  acaqtdefinanceiro , acactrl )
  
  values (
  '$acacod','$saccod2','$loccod2','$esfcod' ,'$unicod',
  '$unitpocod',
  '$tincod',
  '$funcod' ,
  '$unmcod' ,
  '$regcod2' ,
  '$taccod' ,
  '$osicod' ,
  '$procod' ,
  '$prgano' ,
  '$prgcod' ,
  '$prgid' ,
  '$acacodppa',
  '$sitcodestagio',
  '$sitcodandamento' ,
  '$sitcodcronograma' ,
  '$acapercexecucao' ,
  '$acadsc' ,
  '$sacdsc' ,
  '$acadsccomentarios' ,
  '$acanomecoordenador' ,
  '$acadscunresp' ,
  '$acadscunexecutora' ,
  '$acasnmedireta',
  '$acasnmedesc' ,
  '$acasnmelincred' ,
  '$acasnmetanaocumulativa' ,
  '$acamesinicio' ,
  '$acaanoinicio' ,
  '$acamestermino' ,
  '$acaanotermino' ,
  '$acavlrrealateanoanterior' ,
  '$acadsccomentsituacao',
  '$acadscsituacaoatual' ,
  '$acadscresultadosespobt' ,
  '$acamesprevisaoconclusao' ,
  '$acaanoprevisaoconclusao' ,
  '$acadsccomentexecfisica',
  '$acadsccomentexecfinanceira' ,
  '$acadsccomentexecfisicabgu' ,
  '$acadsccomentexecfinanceirabgu' ,
  'f'  ,
  '$acadsccomentexecucao' ,
  '$acadsccomentexecucaorap' ,
  '$acasnfiscalseguridade' ,
  '$acasninvestatais' ,
  '$acasnoutrasfontes' ,
  '$cod_referencia' ,
  '$acadscproduto' ,
  '$acafinalidade' ,
  '$acadescricao' ,
  '$acabaselegal' ,
  '$acarepercfinanceira' ,
  '$acasnpadronizada' ,
  '$acasnsetpadronizada' ,
  '$acasntransfobrigatoria' ,
  '$acasntransfvoluntaria' ,
  '$acasntransfoutras' ,
  '$acasndespesaobrigatoria' ,
  '$acasnbloqueioprogramacao' ,
  '$acadetalhamento' ,
  '$acavlrcustototal' ,
  '$acavlrcustoateanoanterior' ,
  '$acaqtdprevistoanocorrente' ,
  '$acaordemprioridade' ,
  '$acaobs' ,
  '$acacodsof' ,
  '$acaqtdcustototal' ,
  '$acacodreferenciasof' ,
  '$acavlrrepercfinanceira' ,
  '$sfucod' ,
  '$acastatus'  ,
  '$acasnemenda'  ,
  '$acasnestrategica'  ,
  '$acaqtdateanoanterior' ,
  '$acavlrcustoprevistoanocorrente' ,
  '$acasnbgu'  ,
  '$acaptres' ,
  '$ungcod' ,
  '$justificativa' ,
  '$acaqtdefisico' ,  '$acaqtdefinanceiro' ,  'I' )";
		 		print $sql;
	 		 		
		 		$saida = $db->executar($sql);

		 	
		 }
  		 $i++;
	}
	$db -> commit();
}

	


?>
