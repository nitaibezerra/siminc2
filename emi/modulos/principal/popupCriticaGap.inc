<?

if( ! $_REQUEST["papid"] ) {
	print "<script>
			  alert('Ocorreu um erro com a Ação/Atividade selecionada.');
			  self.close();
		  </script>";
	die;
}

if($_REQUEST["submetido"]) {
	// insert
	if($_REQUEST["crpid"] == "") {
		// observação
		if( $_REQUEST["crpvalidado"] == "sim" )
			$crpobs = ($_REQUEST["crpobs"] == "sim") ? "'t'" : "'f'";
		else
			$crpobs = "NULL";
		
		$sql = "INSERT INTO
						emi.critricagap(papid,crpdsccritica,crpvalidado,crpobs)
				VALUES
						(".$_REQUEST["papid"].", '".pg_escape_string($_REQUEST["crpdsccritica"])."', '".(($_REQUEST["crpvalidado"] == "sim") ? "t" : "f")."', ".$crpobs.")";
		$db->executar($sql);
	}
	// update
	else {
		if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) {
			$resposta = ($_REQUEST["crpobservacao"] && $_REQUEST["crpobservacao"] == "t") ? "crptextoobs = '".pg_escape_string($_REQUEST["crpdscresposta"])."'" : "crpdscresposta = '".pg_escape_string($_REQUEST["crpdscresposta"])."'";
			
			$sql = "UPDATE
						emi.critricagap
					SET
						{$resposta},
						crpenviado = 't'
					WHERE
						crpid = ".$_REQUEST["crpid"]." AND
						papid = ".$_REQUEST["papid"];
		} else {
			$critica = ($_REQUEST["crpenviado"] != "t") ? "crpdsccritica = '".pg_escape_string($_REQUEST["crpdsccritica"])."'," : "";
			
			// observação
			if( $_REQUEST["crpvalidado"] == "sim" )
				$crpobs = ($_REQUEST["crpobs"] == "sim") ? "'t'" : "'f'";
			else
				$crpobs = "NULL";
			
			$sql = "UPDATE
						emi.critricagap
					SET
						{$critica}
						crpvalidado = '".(($_REQUEST["crpvalidado"] == "sim") ? "t" : "f")."',
						crpobs = {$crpobs}
					WHERE
						crpid = ".$_REQUEST["crpid"]." AND
						papid = ".$_REQUEST["papid"];
		}
		$db->executar($sql);
	}
	
	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.opener.location.reload();
			self.close();
		  </script>";
	die;
}

$dadosCritica = $db->pegaLinha("SELECT * FROM emi.critricagap WHERE papid = ".$_REQUEST["papid"]);
$dadosCritica = ($dadosCritica) ? $dadosCritica : array();
extract($dadosCritica);

$sql = "SELECT trim(papcaoatividade) as atividade FROM emi.emgap WHERE papid = {$_REQUEST["papid"]}";
monta_titulo("<font size='1'>Crítica da Ação/Atividade: <font color=#808080>".$db->pegaUm($sql)."</font></font>", "");

?>

<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="/emi/geral/js/emi.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
	
		<form id="formCritica" name="formCritica" method="post" action="">
		<input type="hidden" value="1" name="submetido" id="submetido" />
		<input type="hidden" value="<?=$crpid?>" name="crpid" id="crpid" />
		<input type="hidden" value="<?=$_REQUEST["papid"]?>" name="papid" id="papid" />
		<input type="hidden" value="<?=$crpenviado?>" name="crpenviado" id="crpenviado" />
		<input type="hidden" value="<?=$crpobs?>" name="crpobservacao" id="crpobservacao" />
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td class="subtitulodireita"><div id="label_critica_obs"><?=(($crpobs == "t") ? "Observação" : "Crítica")?></div></td>
				<td>
				<?
					if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) {
						$obr = $habil = 'N';
					} else {
						$obr = $habil = ($crpenviado && $crpenviado == "t" ? 'N' : 'S');
					}
					
					echo campo_textarea( 'crpdsccritica', $obr, $habil, '', 100, 12, 2000, '', 0, '' ); 
				?>
				</td>
			</tr>
			
			<? if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) || ( emiPossuiPerfil(EMI_PERFIL_ANALISTACOEM) && $crpenviado == "t" ) ) { ?>
			<tr>
				<td class="subtitulodireita">Resposta</td>
				<td>
				<?
					if( emiPossuiPerfil(EMI_PERFIL_ANALISTACOEM) ) {
						$obr = $habil = 'N';
					} else {
						if($crpobs == "t")
							$obr = $habil = 'S';
						else
							$obr = $habil = ($crpenviado == "t" || $crpvalidado == "t") ? 'N' : 'S';
					}
					
					if($crpobs == "t") $crpdscresposta = $crptextoobs;
					
					echo campo_textarea( 'crpdscresposta', $obr, $habil, '', 100, 12, 2000, '', 0, '' );
				?>
				</td>
			</tr>
			<? }?>
			
			<tr>
				<td class="subtitulodireita">Validação</td>
				<td>
				<?
					$dis = ( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) ? 'disabled="disabled"' : '';
				?>
					<input type="radio" name="crpvalidado" value="sim" <?=(($crpvalidado == "t") ? 'checked="checked"' : "")?> <?=$dis?> onclick="habilitaObs();" /> Sim
					<input type="radio" name="crpvalidado" value="nao" <?=((!$crpvalidado || $crpvalidado == "f") ? 'checked="checked"' : "")?>  <?=$dis?> onclick="habilitaObs();" /> Não
				</td>
			</tr>
			
			<tr>
				<td class="subtitulodireita">Observação</td>
				<td>
				<?
					$dis = ( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) ? 'disabled="disabled"' : '';
				?>
					<input type="radio" name="crpobs" value="sim" <?=(($crpobs == "t") ? 'checked="checked"' : "")?> <?=$dis?> /> Sim
					<input type="radio" name="crpobs" value="nao" <?=((!$crpobs || $crpobs == "f") ? 'checked="checked"' : "")?>  <?=$dis?> /> Não
				</td>
			</tr>
			
			<tr bgcolor="#C0C0C0">
				<td colspan="2">
					<?
						if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) {
							if($crpobs == "t")
								$dis = '';
							else
								$dis = ($crpenviado == "t" || $crpvalidado == "t") ? 'disabled="disabled"' : '';
						}
					?>
					<input type="button" value="Salvar" onclick="salvarCriticaPap(<?=(emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ? "'cadastrador'" : "'analistacoem'")?>);" style="cursor: pointer;" <?=$dis?> />
					<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;" />
				</td>
			</tr>
		</table>
		</form>
	</body>
</html>

<script>

habilitaObs();

function habilitaObs() {
	<? if( ! emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) { ?>
	var validacao 	= 	document.getElementsByName('crpvalidado');
	var observacao 	= 	document.getElementsByName('crpobs');

	var criticaObs	=	document.getElementById('label_critica_obs');
	
	if(validacao[0].checked == true) {
		observacao[0].disabled 	=	false;
		observacao[1].disabled 	=	false;
		criticaObs.innerHTML	=	'Observação'; 
	} else {
		observacao[0].disabled	=	true;
		observacao[1].disabled	=	true;
		criticaObs.innerHTML	=	'Crítica';
	}
	<? } ?>
}

</script>