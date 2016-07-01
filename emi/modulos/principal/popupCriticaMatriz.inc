<?

if( ! $_REQUEST["mdoid"] ) {
	print "<script>
			  alert('Ocorreu um erro com o item selecionado.');
			  self.close();
		  </script>";
	die;
}

if($_REQUEST["submetido"]) {
	// insert
	if($_REQUEST["crmid"] == "") {
		// observação
		if( $_REQUEST["crmvalidado"] == "sim" )
			$crmobs = ($_REQUEST["crmobs"] == "sim") ? "'t'" : "'f'";
		else
			$crmobs = "NULL";
			
		$sql = "INSERT INTO
						emi.critricamatriz(mdoid,crmdsccritica,crmvalidado,crmobs)
				VALUES
						(".$_REQUEST["mdoid"].", '".pg_escape_string($_REQUEST["crmdsccritica"])."', '".(($_REQUEST["crmvalidado"] == "sim") ? "t" : "f")."', ".$crmobs.")";
		$db->executar($sql);
	}
	// update
	else {
		if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) {
			$resposta = ($_REQUEST["crmobservacao"] && $_REQUEST["crmobservacao"] == "t") ? "crmtextoobs = '".pg_escape_string($_REQUEST["crmdscresposta"])."'" : "crmdscresposta = '".pg_escape_string($_REQUEST["crmdscresposta"])."'";
			
			$sql = "UPDATE
						emi.critricamatriz
					SET
						{$resposta},
						crmenviado = 't'
					WHERE
						crmid = ".$_REQUEST["crmid"]." AND
						mdoid = ".$_REQUEST["mdoid"];
		} else {
			$critica = ($_REQUEST["crmenviado"] != "t") ? "crmdsccritica = '".pg_escape_string($_REQUEST["crmdsccritica"])."'," : "";
			
			// observação
			if( $_REQUEST["crmvalidado"] == "sim" )
				$crmobs = ($_REQUEST["crmobs"] == "sim") ? "'t'" : "'f'";
			else
				$crmobs = "NULL";
			
			$sql = "UPDATE
						emi.critricamatriz
					SET
						{$critica}
						crmvalidado = '".(($_REQUEST["crmvalidado"] == "sim") ? "t" : "f")."',
						crmobs = {$crmobs}
					WHERE
						crmid = ".$_REQUEST["crmid"]." AND
						mdoid = ".$_REQUEST["mdoid"];
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

$dadosCritica = $db->pegaLinha("SELECT * FROM emi.critricamatriz WHERE mdoid = ".$_REQUEST["mdoid"]);
$dadosCritica = ($dadosCritica) ? $dadosCritica : array();
extract($dadosCritica);

monta_titulo("Crítica do Item da Matriz", "");

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
		<input type="hidden" value="<?=$crmid?>" name="crmid" id="crmid" />
		<input type="hidden" value="<?=$_REQUEST["mdoid"]?>" name="mdoid" id="mdoid" />
		<input type="hidden" value="<?=$crmenviado?>" name="crmenviado" id="crmenviado" />
		<input type="hidden" value="<?=$crmobs?>" name="crmobservacao" id="crmobservacao" />
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td class="subtitulodireita"><div id="label_critica_obs"><?=(($crmobs == "t") ? "Observação" : "Crítica")?></div></td>
				<td>
				<?
					if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) {
						$obr = $habil = 'N';
					} else {
						$obr = $habil = ($crmenviado == "t") ? 'N' : 'S';
					}
					echo campo_textarea( 'crmdsccritica', $obr, $habil, '', 100, 12, 2000, '', 0, '' ); 
				?>
				</td>
			</tr>
			
			<? if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) || ( emiPossuiPerfil(EMI_PERFIL_ANALISTACOEM) && $crmenviado == "t" ) ) { ?>
			<tr>
				<td class="subtitulodireita">Resposta</td>
				<td>
				<?
					if( emiPossuiPerfil(EMI_PERFIL_ANALISTACOEM) ) {
						$obr = $habil = 'N';
					} else {
						if($crmobs == "t")
							$obr = $habil = 'S';
						else
							$obr = $habil = ($crmenviado == "t" || $crmvalidado == "t") ? 'N' : 'S';
					}
					
					if($crmobs == "t") $crmdscresposta = $crmtextoobs;
					
					echo campo_textarea( 'crmdscresposta', $obr, $habil, '', 100, 12, 2000, '', 0, '' );
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
					<input type="radio" name="crmvalidado" value="sim" <?=(($crmvalidado == "t") ? 'checked="checked"' : "")?> <?=$dis?> /> Sim
					<input type="radio" name="crmvalidado" value="nao" <?=((!$crmvalidado || $crmvalidado == "f") ? 'checked="checked"' : "")?>  <?=$dis?> /> Não
				</td>
			</tr>
			
			<tr>
				<td class="subtitulodireita">Observação</td>
				<td>
				<?
					$dis = ( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) ? 'disabled="disabled"' : '';
				?>
					<input type="radio" name="crmobs" value="sim" <?=(($crmobs == "t") ? 'checked="checked"' : "")?> <?=$dis?> /> Sim
					<input type="radio" name="crmobs" value="nao" <?=((!$crmobs || $crmobs == "f") ? 'checked="checked"' : "")?>  <?=$dis?> /> Não
				</td>
			</tr>
			
			<tr bgcolor="#C0C0C0">
				<td colspan="2">
					<?
						if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ) {
							if($crmobs == "t")
								$dis = '';
							else
								$dis = ($crmenviado == "t" || $crmvalidado == "t") ? 'disabled="disabled"' : '';
						}
					?>
					<input type="button" value="Salvar" onclick="salvarCriticaMatriz(<?=(emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) ? "'cadastrador'" : "'analistacoem'")?>);" style="cursor: pointer;" <?=$dis?> />
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
	var validacao 	= 	document.getElementsByName('crmvalidado');
	var observacao 	= 	document.getElementsByName('crmobs');

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