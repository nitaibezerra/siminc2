<?

// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/sisplan/_constantes.php";
$db = new cls_banco();

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/
if(is_array($_POST['ususecresp']) && @count($_POST['ususecresp'])>0) {
	$txtAcoesComCoordenador = "";
	$confirmarAcoes = false;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$acoesConfirmadas = (bool)$_REQUEST["acoesConfirmadas"];

	$sqlSelPerfil = "SELECT pflsncumulativo FROM perfil WHERE pflcod = " . $pflcod;
	$rsPerfil = $db->carregar($sqlSelPerfil);
	$pflsncumulativo = $rsPerfil[0]["pflsncumulativo"] == 't' ? true : false;
	
	$sqlSelResp = "SELECT ur.rpuid, ur.usucpf, ur.rpustatus, a.acacod || ' - ' || a.acadsc as descricao, u.usunome FROM sisplan.usuarioresponsabilidade ur 
		INNER JOIN ( SELECT acacod, max(acadsc) as acadsc FROM monitora.acao a WHERE a.acasnrap = false AND a.unicod = '" . UNIDADE_IPHAN . "' GROUP BY acacod ) a ON a.acacod  = ur.acacod
		INNER JOIN usuario u on ur.usucpf=u.usucpf 
		WHERE ur.rpustatus = 'A' AND ur.usucpf <> '".$usucpf."' AND ur.pflcod = ".$pflcod;

	$sqlSelItem	= "SELECT DISTINCT acacod FROM monitora.acao a WHERE a.unicod = '" . UNIDADE_IPHAN . "' AND a.acacod = '%s' AND a.acasnrap = false";

	$sqlInsRpu = "INSERT INTO sisplan.usuarioresponsabilidade (acacod, usucpf, rpustatus, rpudata_inc, pflcod) VALUES ('%s', '%s', '%s', '%s', '%s')";

	$sqlUpdRpu = "UPDATE sisplan.usuarioresponsabilidade SET rpustatus = 'I' WHERE acacod = '%s' AND pflcod = ".$pflcod;

	$sqlUpdRpuUsu = "UPDATE sisplan.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '".$usucpf."' AND pflcod = ".$pflcod;
	//
	// verificar quais itens possuem outro coordenador ativo
	if(!$pflsncumulativo && $_POST['ususecresp'][0]!="") {
		foreach ($_POST['ususecresp'] as $respcod) {
			$sql = "";
			$arrCodigoAcao = explode(".", $respcod);
			$sql = vsprintf($sqlSelResp, $arrCodigoAcao);
			if ($sql<>"" && ($linhasRpu = $db->carregar($sql))) {
				foreach ($linhasRpu as $rpu) {
					$confirmarAcoes = true;
					$txtAcoesComCoordenador .= $respcod . " - " . $rpu["acadsc"] . " - Nome: ".$rpu['usunome']." - CPF: " . $rpu["usucpf"] . '\n';
				}
			}
		}
	}

	//
	// caso nao existam outros coordenadores, registrar os itens selecionados
	if(!$confirmarAcoes || $acoesConfirmadas) {
		$db->executar($sqlUpdRpuUsu);
		if($_POST['ususecresp'][0]!="") {
			foreach ($_POST['ususecresp'] as $respcod) {
				$sql = sprintf($sqlSelItem, $respcod);
				$linha = $db->carregar($sql);
				if(is_array($linha) && count($linha)>=1) {
					foreach ($linha as $secretaria) {
						$secid = $secretaria["acacod"];
						// no caso de um perfil cumulativo, não desativa os usuarios atuais
						if(!$pflsncumulativo) {
							$sql = sprintf($sqlUpdRpu, $secid);
							$db->executar($sql);
						}
						
						$dados = array($secid, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod); 			
						$sql = vsprintf($sqlInsRpu, $dados);
						$db->executar($sql);
					}
				}
			}
		}
		$concluido = 1;
		//$db->rollback();
		//dbg(1,1);
	}
	//
	// exibir a tela de aviso dos itens que já possuem coordenador e confirmar
	// a substituição pelo usuario que está sendo liberado e/ou alterado
	else {
		$msg = 'Existem usuários ativos com o perfil selecionado para estas ações:\n\n';
		$msg .= $txtAcoesComCoordenador;
		$msg .= '\nDeseja sobrescrevê-los?\n\n';
		$msg .= 'Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.';
		?>
		<body>
		<form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="acoesConfirmadas" value="1">
		<?
			foreach ($_POST['ususecresp'] as $respcod) {
				?><input type="hidden" name="ususecresp[]" value="<?=$respcod?>"><?	
			}
		?>
		</form>
		<script>
			if (confirm("<?=$msg?>")) {
				document.formassocia.submit();
			}
			else
			{
				self.close();			
			}
		</script>
		</body>
		<?
		exit(0);
	}
	
	if ($concluido>0) {
		$db->commit();
		?>
		<script language="javascript">
			alert("Operação realizada com sucesso!");
			opener.location.reload();
			self.close();
		</script>
		<?
		exit(0);
	}
}
/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Atribuir Ações PPA</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" onload="self.focus()">
<div align=center id="aguarde"><img src="../../imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="2"><strong>Selecione a(s) Ação PPA</strong></td>
</tr>
<?php
//$sql = "select s.uexid as codigo, s.uexcod|| ' - ' || s.uexdsc || ' - ' || s.uexsigla as descricao from planointerno.unidadeexecutora s order by 2";
$sql = "
SELECT 
	acacod as codigo, 
	a.acacod || ' - ' || max(acadsc) as descricao
FROM monitora.acao a 
WHERE 
	a.acasnrap = false 
	AND a.unicod = '" . UNIDADE_IPHAN . "' 
GROUP BY acacod
ORDER BY 2";

$dados = $db->carregar($sql);
$i = -1;
foreach ( $dados as $linha ): 
	$corFundo = $i++ % 2 ? '#f4f4f4' : $cor='#e0e0e0';
?>
	<tr bgcolor="<?=$corFundo?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$corFundo?>';">
		<td align="left"><input type="Checkbox" name="codigo" id="cod_<?=$i?>" value="<?=$linha['codigo']?>" onclick="retorna(<?=$i?>);"/><input type="hidden" name="descricao" id="<?=$i?>" value="<?=$linha['descricao']?>"/></td>
		<td><?=$linha['descricao']?></td>
	</tr>
<?php endforeach; ?>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="ususecresp[]" id="ususecresp" style="width:500px;" class="CampoEstilo">
<?
$sql = "
SELECT
	a.acacod as codigo, 
	a.acacod || ' - ' || a.acadsc as descricao
from sisplan.usuarioresponsabilidade u 
JOIN ( SELECT acacod, max(acadsc) as acadsc FROM monitora.acao a WHERE a.acasnrap = false AND a.unicod = '" . UNIDADE_IPHAN . "' GROUP BY acacod ) a ON a.acacod  = u.acacod
where rpustatus='A' and usucpf = '$usucpf' and u.pflcod=$pflcod";
$dados = $db->carregar($sql);
if ( is_array($dados) && count( $dados ) )
{
	foreach( $dados as $linha ):
	?>
		<option value="<?= $linha['codigo'] ?>"><?= $linha['descricao'] ?></option>
	<?php
	endforeach;

}
else
{
	$sql = "
	SELECT
		a.acacod as codigo, 
		a.acacod || ' - ' || a.acadsc as descricao
	from sisplan.usuarioresponsabilidade u 
	JOIN ( SELECT acacod, max(acadsc) as acadsc FROM monitora.acao a WHERE a.acasnrap = false AND a.unicod = '" . UNIDADE_IPHAN . "' GROUP BY acacod ) a ON a.acacod  = u.acacod
	where rpustatus='A' and usucpf = '$usucpf' and u.pflcod=$pflcod";
	$dados = $db->carregar($sql);
	if ( is_array($dados) && count( $dados ) )
	{
		foreach( $dados as $linha ):
		?>
			<option value="<?= $linha['codigo'] ?>"><?= $linha['descricao'] ?></option>
		<?php
		endforeach;
	}
	else
	{
		?>
		<option value="">Selecione a(s) Ação PPA.</option>
		<?php
	}
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";

var campoSelect = document.getElementById("ususecresp");
if (campoSelect.options[0].value != '')
{
	for (var i=0; i<campoSelect.options.length; i++)
		for (var j=0; j<document.formulario.codigo.length; j++ )
			if ( document.formulario.codigo[j].value == campoSelect.options[i].value )
				document.formulario.codigo[j].checked = true;
}


function retorna(objeto)
{
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.getElementById( 'cod_'+objeto ).checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.descricao[objeto].value, document.getElementById( 'cod_'+objeto ).value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.getElementById( 'cod_'+objeto ).value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Selecione a(s) Ação PPA.', '', false, false);}
			sortSelect(campoSelect);
	}
}
</script>