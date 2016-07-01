<?

if($_POST['salvar']) {
	
	// verificando se os filhos possuem subação
	$sql = "SELECT pla.pliid FROM pde.atividade atv 
			INNER JOIN monitora.pi_planointernoatividade pla ON atv.atiid = pla.atiid 
			WHERE _atiprojeto = (SELECT _atiprojeto FROM pde.atividade WHERE atiid = {$_REQUEST['atiid']} ) AND _atinumero ilike (SELECT _atinumero FROM pde.atividade WHERE atiid=".$_REQUEST['atiid'].")||'%'";
	
	$existe_filhos = $db->pegaUm($sql);
	
	if($existe_filhos) {
		
		die("<script>
				alert('Não foi possivel alterar a subação');
				window.close();
			 </script>");
		
	}
	
	
	$sql = "DELETE FROM monitora.pi_subacaoatividade WHERE atiid IN (SELECT atiid FROM pde.atividade WHERE atiid=".$_REQUEST['atiid'].")";
	$db->executar($sql);
		
	if($_REQUEST['sbaid']) {
		$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid, sbaatividade) VALUES ('".$_REQUEST['sbaid']."', '".$_REQUEST['atiid']."', false);";
		$db->executar($sql);
		
		$db->executar("UPDATE pde.atividade SET atidescricao = (SELECT sbatitulo || ' - (' || sbacod || ')' FROM monitora.pi_subacao WHERE sbaid= '".$_REQUEST['sbaid']."') WHERE atiid = '".$_REQUEST['atiid']."'");
	}

	$sql = "SELECT atiid FROM pde.atividade WHERE _atiprojeto = ".$_SESSION['projeto']." and atistatus='A' AND  _atinumero ilike (SELECT _atinumero FROM pde.atividade WHERE atiid=".$_REQUEST['atiid'].")||'.%'";

	$filhos = $db->carregar($sql);
	
	if($filhos[0]) {
		foreach($filhos as $f) {
			# Existe subação atividade filha diferente da subação atual
			$existe_subacao_atividade_dif = $db->pegaUm("select count(1) from monitora.pi_subacaoatividade where sbaid != {$_REQUEST['sbaid']} and atiid = {$f['atiid']}");
			if($existe_subacao_atividade_dif){
				$db->rollback();
				echo "<script>alert('Existem atividades filhas vinculadas em outra subação.'); window.close();</script>";
				die;			
			}
			
			$existe_subacao_atividade = $db->pegaUm("select count(1) from monitora.pi_subacaoatividade where sbaid = {$_REQUEST['sbaid']} and atiid = {$f['atiid']}");
			if(!$existe_subacao_atividade){
				$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid, sbaatividade) VALUES ('".$_REQUEST['sbaid']."', '".$f['atiid']."', false);";
				$db->executar($sql);
			}
		}
	}

	$db->commit();
	
	die("<script>
			window.opener.location.replace(window.opener.location);
			window.close();
		 </script>");
		
}

if($_GET['des_atiid']){
	$sql = "SELECT atiid FROM pde.atividade WHERE _atiprojeto = ".$_SESSION['projeto']." and atistatus='A' and _atinumero ilike (SELECT _atinumero FROM pde.atividade WHERE atiid=".$_GET['des_atiid'].")||'.%'";
	$filhos = $db->carregar($sql);
	$filhos = ($filhos) ? $filhos : array();
	if($filhos[0]) {
		foreach($filhos as $f) {
			$sql = "DELETE FROM monitora.pi_subacaoatividade WHERE atiid = '".$f['atiid']."'";
			$db->executar($sql);
		}
	}
	
	$sql_D = "DELETE FROM monitora.pi_subacaoatividade WHERE atiid = '".$_GET['des_atiid']."'";
	$db->executar($sql_D);
	$db->commit();
	die("<script>alert('Registro desvinculado com sucesso.');
				 window.opener.location.replace(window.opener.location);
				 window.close();
		</script>");
}


?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?

$unidsc = $db->pegaUm("SELECT unidsc FROM public.unidade WHERE unicod='".$_SESSION['monitora_var']['unicod']."'");
$ati = $db->pegaLinha("SELECT _atiprofundidade, atidescricao FROM pde.atividade	WHERE atiid='".$_REQUEST['atiid']."'");

if($ati) {
	$atidescricao = $ati['atidescricao'];
	$_atiprofundidade = $ati['_atiprofundidade'];	
}


monta_titulo($unidsc, 'Vinculação/Criação da subação na unidade');


// se não tiver opção, selecionar criação
if(!$_REQUEST['opc']) {
	$_REQUEST['opc'] = 'cri';
}

$sbaid = $db->pegaUm("SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$_REQUEST['atiid']."'");

?>
<form <? echo (($_REQUEST['opc']=='cri')?"action=\"./planotrabalhoUN/ajaxUN.php?atividade=".$_REQUEST['atiid']."&evento=arvore_inserir&subatv=true\" onsubmit=\"document.getElementById('atidescricao').value=document.getElementById('sbatitulo').value;\"":"action=\"monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A\""); ?> method="post" name="formulario">

<input type="hidden" name="salvar" id="salvar" value="1">
<input type="hidden" name="atidescricao" id="atidescricao">
<input type="hidden" name="atiid" value="<? echo $_REQUEST['atiid']; ?>">
<input type="hidden" name="atiidpai" value="<? echo $_REQUEST['atiid']; ?>">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<? if($_atiprofundidade > 1 && !$_REQUEST['opcnone']) { ?>
<tr>
	<td class="SubTituloCentro" colspan=2>
	<input type="radio" name="opc" value="vin" <? echo (($_REQUEST['opc']=='vin')?"checked":""); ?> onclick="if(this.checked==true){window.location='monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<? echo $_REQUEST['atiid']; ?>&opc=vin';}" > Vincular 
	<input type="radio" name="opc" value="cri" <? echo (($_REQUEST['opc']=='cri')?"checked":""); ?> onclick="if(this.checked==true){window.location='monitora.php?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&atiid=<? echo $_REQUEST['atiid']; ?>&opc=cri';}" > Criar</td>
</tr>
<? } ?>

<?

switch($_REQUEST['opc']) {
	case 'cri':
		?>
		<tr>
			<td class="SubTituloDireita" align="right">Título:</td>
			<td><? echo campo_texto('sbatitulo', 'S', 'S', 'Título', 67, 150, "", "", '', '', 0, 'id="sbatitulo"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Código:</td>
			<td><? echo campo_texto('sbacod', 'S', 'S', 'Código', 5, 4, "", "", '', '', 0, 'id="sbacod"', "this.value=this.value.toUpperCase();" ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Unidade Orçamentária:</td>
			<td><? echo $unidsc; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Descrição/Funcionalidade:</td>
			<td><? echo campo_textarea( 'sbadsc', 'N', 'S', '', '70', '4', '200'); ?></td>
		</tr>
		<?
		break;
	case 'vin':
		?>
		<tr>
			<td class="SubTituloDireita" align="right">Atividade:</td>
			<td>
			<?
			echo $atidescricao;
			?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" align="right">Subação:</td>
			<td><?
			
			$sql = "SELECT 
						count(1) as count
					FROM monitora.pi_planointerno p 
						INNER JOIN monitora.pi_planointernoatividade pia ON p.pliid = pia.pliid
					WHERE 
						p.plistatus = 'A' AND pia.atiid = {$_REQUEST["atiid"]}
					LIMIT 1
					";
			$boPiVinculado = $db->pegaUm($sql);
			
			$habilitado = 'S';
			if($boPiVinculado){
				$habilitado = 'N';
				$disabled = "disabled='disabled'";
			}
			
			$sql = "SELECT sba.sbaid as codigo, '('||sba.sbacod||') '||sba.sbatitulo as descricao  FROM monitora.pi_subacao sba 
			   		LEFT JOIN monitora.pi_subacaounidade ppe ON ppe.sbaid = sba.sbaid 
			   		WHERE sbasituacao='A' AND sbastatus='A' and ppe.unicod='".$_SESSION['monitora_var']['unicod']."'";
			
			
			$db->monta_combo('sbaid', $sql, $habilitado,'Selecione','','','',400,'','sbaid','');
			 
			?></td>
		</tr>
		<?
		break;
}

?>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
	<td>
	<input type="button" name="gravar" value="Salvar" onclick="enviar_<? echo $_REQUEST['opc']; ?>();" <?php echo $disabled; ?> />
	<input type="button" name="close" value="Fechar" onclick="window.close();">
	<?php 
	if(!$boPiVinculado && $sbaid){
	?>	
	<input type="button" value="Desvincular" onclick="desvincular('<? echo $_REQUEST['atiid']; ?>');">
	<?php 
	}
	?>	
	</td>
</tr>		
</table>
</form>
<script type="text/javascript">
function enviar_cri(){
	if(document.formulario.sbatitulo.value == '') {
		alert("O preenchimento do campo Título é obrigatório.\n");
	}

	var sbacod = document.getElementById('sbacod');
	if(sbacod.value.length < 4){
		alert('O Código deve ter 4 caracteres');
		return false;
	}
	document.formulario.submit();
}

function enviar_vin(){
	if(document.formulario.sbaid.value == '') {
		alert("Selecione a subação\n");
		return false;
	}
	
	document.formulario.submit();
}

function desvincular(atiid){
	var conf = confirm("Você realmente deseja desvincular esta subação?");	
	if(conf) {
		window.location.href = '?modulo=principal/planotrabalhoUN/listarsubacoesatividadeUN&acao=A&des_atiid='+atiid;
	}
}

</script>
</body>
</html>