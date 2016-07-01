<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

ini_set("memory_limit", "1024M");


if($_POST['varaux'] == '1'){

	
	include APPRAIZ ."includes/workflow.php";

	$sql = "select distinct doc.docid, doc.esdid  from obras.preobra pre
			inner join workflow.documento doc on doc.docid = pre.docid and doc.esdid not in (228,229,193)
			inner join territorios.muntipomunicipio mtm on mtm.muncod = pre.muncod and mtm.tpmid in (150)
			where pre.prestatus = 'A' and pre.ptoid in (1,2,3,6,7) order by 1;";
	$dados = (array) $db->carregar($sql);
	
	
	$cmddsc = "Status da obra alterado para (Em cadastramento) para que os municípios possam fazer correções nas solicitações de obras, conforme autorização dada pela Resolução RESOLUÇÃO/CD/FNDE N°39 DE 29 DE DEZEMBRO DE 2010.";
	
	
	for($i=0; $i<=count($dados)-1;$i++)
	{
		$docid = $dados[$i]['docid'];
		$esdid = $dados[$i]['esdid'];
		
		
		switch($esdid) {
			case '215': //em analise p/ cadastramento - esdid=215&aedid=603
				$aedid = 603;
				break;
			case '194': //Aguardando análise - FNDE p/ cadastramento - esdid=194&aedid=602
				$aedid = 602;
				break;
			case '195': //Em diligência p/ cadastramento - esdid=195&aedid=604
				$aedid = 604;
				break;
			case '210': //Validação de diligência p/ cadastramento - esdid=210&aedid=605
				$aedid = 605;
				break;
			case '211': //Validação de indeferimento p/ cadastramento - esdid=211&aedid=606
				$aedid = 606;
				break;
			case '212': //Validação de deferimento p/ cadastramento - esdid=212&aedid=607
				$aedid = 607;
				break;
			case '213': //Obra indeferida p/ cadastramento - esdid=213&aedid=608
				$aedid = 608;
				break;
			case '214': //Obra deferida p/ cadastramento - esdid=214&aedid=609
				$aedid = 609;
				break;
			case '217': //Em revisão de análise p/ cadastramento - esdid=217&aedid=610
				$aedid = 610;
				break;
			case '218': //Em análise / retorno de diligência p/ cadastramento - esdid=218&aedid=611
				$aedid = 611;
				break;
			case '220': //Deferimento condicionado de engenharia p/ cadastramento - esdid=220&aedid=612
				$aedid = 612;
				break;
			case '221': //Obra indeferida - decurso prazo de diligência p/ cadastramento - esdid=221&aedid=613
				$aedid = 613;
				break;
			default:
				$aedid = 0;
		}
		
		
		
		$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";
		
		if($aedid != 0){
			wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
			echo $i ."-". $docid . "<br>";
		}
		
		
	}

	echo "<br>ACABOUUUUUUU Proinfancia";
	
	
	
	$sql = "select distinct doc.docid, doc.esdid from obras.preobra pre
			inner join workflow.documento doc on doc.docid = pre.docid and doc.esdid not in (228,229,193)
			inner join territorios.muntipomunicipio mtm on mtm.muncod = pre.muncod and mtm.tpmid in (150,151)
			where pre.prestatus = 'A' and pre.ptoid in (5) order by 1";
	$dados = (array) $db->carregar($sql);
	
	
	for($i=0; $i<=count($dados)-1;$i++)
	{
		$docid = $dados[$i]['docid'];
		$esdid = $dados[$i]['esdid'];
		
		
		switch($esdid) {
			case '215': //em analise p/ cadastramento - esdid=215&aedid=603
				$aedid = 603;
				break;
			case '194': //Aguardando análise - FNDE p/ cadastramento - esdid=194&aedid=602
				$aedid = 602;
				break;
			case '195': //Em diligência p/ cadastramento - esdid=195&aedid=604
				$aedid = 604;
				break;
			case '210': //Validação de diligência p/ cadastramento - esdid=210&aedid=605
				$aedid = 605;
				break;
			case '211': //Validação de indeferimento p/ cadastramento - esdid=211&aedid=606
				$aedid = 606;
				break;
			case '212': //Validação de deferimento p/ cadastramento - esdid=212&aedid=607
				$aedid = 607;
				break;
			case '213': //Obra indeferida p/ cadastramento - esdid=213&aedid=608
				$aedid = 608;
				break;
			case '214': //Obra deferida p/ cadastramento - esdid=214&aedid=609
				$aedid = 609;
				break;
			case '217': //Em revisão de análise p/ cadastramento - esdid=217&aedid=610
				$aedid = 610;
				break;
			case '218': //Em análise / retorno de diligência p/ cadastramento - esdid=218&aedid=611
				$aedid = 611;
				break;
			case '220': //Deferimento condicionado de engenharia p/ cadastramento - esdid=220&aedid=612
				$aedid = 612;
				break;
			case '221': //Obra indeferida - decurso prazo de diligência p/ cadastramento - esdid=221&aedid=613
				$aedid = 613;
				break;
			default:
				$aedid = 0;
		}
		
		
		$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";
		
		if($aedid != 0){
			wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
			echo $i ."-". $docid . "<br>";
		}
		
		
	}
	
	echo "<br>ACABOUUUUUUU quadras";	

	echo "<script>alert('ACABOUUUUUUU tudo');</script>";

}

?>

<html>
<head>




<script type="text/javascript">
		function executa() {
			document.formulario.varaux.value='1';
			document.formulario.submit();
			
		}
</script>

</head>
<body>
<form method="POST" name="formulario">
<input type="hidden" name="varaux" value="" /> 

		
<br><br><br><br>
<center>
<input style="cursor: pointer;" type="button" name="brne" value="executar" onclick="executa();">
</center>

</form>

</body>
</html>