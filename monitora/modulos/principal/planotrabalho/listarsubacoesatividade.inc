<?
/*
 * Carregando o código da unidade
 */
if($_SESSION['monitora_var']['entid']) {
	
	$sql = "SELECT entunicod, entnome FROM entidade.entidade WHERE entid = '".$_SESSION['monitora_var']['entid']."'";
	$dados = $db->pegaLinha($sql);
	$unicod = $dados['entunicod'];
	$entnome = $dados['entnome'];
	
} else {
	echo "<script>alert('Unidade não encontrada.');window.close();</script>";exit;
}

if(!$unicod) {
	echo "<script>alert('Unidade não encontrada');window.close();</script>";exit;
}


switch($_REQUEST['evento']) {
	case 'I':
		// verificar se existem filhos com subacões
		$_atinumero = $db->pegaUm("SELECT _atinumero FROM pde.atividade	WHERE atiid='".$_REQUEST['atiid']."'");
		$filhossba = $db->carregar("SELECT sat.sbaid FROM pde.atividade ati
									INNER JOIN monitora.subacaoatividade sat ON sat.atiid=ati.atiid 
									WHERE _atinumero LIKE '".$_atinumero.".%'");

		if($filhossba[0]) {
			foreach($filhossba as $s) {
				if($s['sbaid'] != $_REQUEST['sbaid']) {
					die("<script>
							alert('Filhos possuem subações diferentes.');
							window.location='monitora.php?modulo=principal/planotrabalho/listarsubacoesatividade&acao=A&atiid=".$_REQUEST['sbaid']."';
						 </script>");
				}
			}
		}
		
		$filhosati = $db->carregar("SELECT * FROM pde.atividade	WHERE _atinumero LIKE '".$_atinumero.".%' AND _atiprojeto='".$_SESSION["projeto"]."'");
		
		if($filhosati[0]) {
			foreach($filhosati as $atichild) {
				$db->executar("DELETE FROM monitora.subacaoatividade WHERE sbaid='".$_REQUEST['sbaid']."' AND atiid='".$atichild['atiid']."'");
				$sql = "INSERT INTO monitora.subacaoatividade(sbaid, atiid) VALUES ('".$_REQUEST['sbaid']."', '".$atichild['atiid']."');";
				$db->executar($sql);
			}
		}
		
		$sql = "INSERT INTO monitora.subacaoatividade(sbaid, atiid) VALUES ('".$_REQUEST['sbaid']."', '".$_REQUEST['atiid']."');";
		$db->executar($sql);
		$db->commit();
		
		die("<script>alert('Subação inserida com sucesso');opener.history.go(0);window.close();</script>");
		
		break;
	case 'U':
		
		$_atinumero = $db->pegaUm("SELECT _atinumero FROM pde.atividade	WHERE atiid='".$_REQUEST['atiid']."'");
		$filhosati = $db->carregar("SELECT * FROM pde.atividade	WHERE _atinumero LIKE '".$_atinumero.".%' AND _atiprojeto='".$_SESSION["projeto"]."'");

		if($filhosati[0]) {
			foreach($filhosati as $atichild) {
				$atividadefi[] = $atichild['atiid'];
			}
			$sql = "SELECT * FROM monitora.planointernoatividade WHERE atiid IN('".implode("','",$atividadefi)."')";
			$pis = $db->carregar($sql);
			if($pis[0]) {
				echo "<pre>";
				print_r($pis);
				exit;
			}
		}
		
		if($filhosati[0]) {
			foreach($filhosati as $atichild) {
				$sql = "UPDATE monitora.subacaoatividade SET sbaid='".$_REQUEST['sbaid']."' WHERE atiid='".$atichild['atiid']."'";
				$db->executar($sql);
			}
		}
		
		$sql = "UPDATE monitora.subacaoatividade SET sbaid='".$_REQUEST['sbaid']."' WHERE atiid='".$_REQUEST['atiid']."'";
		$db->executar($sql);
		$db->commit();
		die("<script>alert('Subação gravada com sucesso');opener.history.go(0);window.close();</script>");
		break;
}

$sbaid = $db->pegaUm("SELECT sbaid FROM monitora.subacaoatividade WHERE atiid='".$_REQUEST['atiid']."'");

?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php monta_titulo($entnome, 'Vinculação da subação na unidade'); ?>
<form action="" method="post" name="formulario">
<input type="hidden" name="atiid" value="<? echo $_REQUEST['atiid']; ?>">
<input type="hidden" name="evento" value="<? echo (($sbaid)?"U":"I"); ?>">
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" align="right">Atividade:</td>
	<td>
	<?
	$sql = "SELECT atidescricao FROM pde.atividade	WHERE atiid='".$_REQUEST['atiid']."'";
	echo $db->pegaUm($sql);
	?>
	</td>
</tr>	
<tr>
	<td class="SubTituloDireita" align="right">Subação:</td>
	<td>
	<?
	$sql = "SELECT sbt.sbaid FROM pde.atividade at 
			INNER JOIN pde.atividade at2 ON at.atiidpai=at2.atiid 
			INNER JOIN monitora.subacaoatividade sbt ON sbt.atiid= at2.atiid  
			WHERE at.atiid='".$_REQUEST['atiid']."'";
	
	$paicomsba = $db->pegaUm($sql);
	
	$_atinumero = $db->pegaUm("SELECT _atinumero FROM pde.atividade	WHERE atiid='".$_REQUEST['atiid']."'");
	
	$filhosba  = $db->carregar("SELECT sub.sbaid, sub.sbatitulo FROM pde.atividade ati
							  INNER JOIN monitora.subacaoatividade sat ON sat.atiid=ati.atiid 
							  INNER JOIN monitora.subacao sub ON sub.sbaid=sat.sbaid 
							  WHERE _atinumero LIKE '".$_atinumero.".%' 
							  GROUP BY sub.sbaid, sub.sbatitulo");
	
	
	$sql = "SELECT sba.sbaid as codigo, sba.sbatitulo as descricao  FROM monitora.subacao sba 
	   		LEFT JOIN monitora.subacaounidade ppe ON ppe.sbaid = sba.sbaid 
	   		WHERE sbastatus='A' and ppe.entid='".$_SESSION['monitora_var']['entid']."'";
	
	
	if($filhosba[0]) {
		
		if(count($filhosba) > 1) {
			echo "<font color=red><b>* Atividade possui filho(s) com diversas subações.</b></font>";
			unset($paicomsba);
		} else {
			$filhosba = $filhosba[0];
			echo "<input type=hidden name=sbaid id=sbaid value='".$filhosba['sbaid']."'>".$filhosba['sbatitulo']." <font color=red><b>* Atividade possui filho(s) com a subação descrita.</b></font>";
		}
		
	} else {
		$db->monta_combo('sbaid', $sql, ((!$paicomsba)?'S':'N'),'Selecione','','','',400);
	}
	
	if($paicomsba) echo " <font color=red><b>* Atividade pai possui uma subação anexada.</b></font>";
	 
	?>
	</td>
</tr>	
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
	<td>
	<?
	if(!$paicomsba) {
	?>
	<input type="submit" name="botao" value="Salvar"/>
	<input type="button" name="botao1" value="Inserir nova subação" onclick="window.location='monitora.php?modulo=principal/planotrabalho/programainstitucionalunidade&acao=A&atiid=<? echo $_REQUEST['atiid']; ?>';"/>
	<?
	}
	?>
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>
</form>
</body>
</html>

