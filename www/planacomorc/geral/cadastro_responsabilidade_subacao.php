<?
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

function gravarResponsabilidadeAcao($dados) {
    global $db;
    $sql = "UPDATE planacomorc.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$dados['usucpf']."' AND pflcod='".$dados['pflcod']."' AND id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}";
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach($dados['usuacaresp'] as $id_subacao) {

            $sql = "INSERT INTO planacomorc.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, id_subacao, id_periodo_referencia)
                       VALUES ('".$dados['pflcod']."', '".$dados['usucpf']."', 'A', NOW(), '" . $id_subacao . "', {$_REQUEST['id_periodo_referencia']});";
            $db->executar($sql);
	}
    }

    $db->commit();

    echo "<script language=\"javascript\">
                alert(\"Operação realizada com sucesso!\");
                opener.location.reload();
                self.close();
          </script>";
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Atribuir Ações</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script src="../js/planacomorc.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>
</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" onload="self.focus()">
<script>

jQuery(document).ready(function(){

	jQuery('#id_periodo_referencia').change(function(){

		url = 'cadastro_responsabilidade_subacao.php?pflcod='+jQuery('[name=pflcod]').val();
		url += '&usucpf='+jQuery('[name=usucpf]').val();
		url += '&id_periodo_referencia='+jQuery(this).val()

		document.location.href = url;
	});

});

function carregarAcoesUnidade(orgcod,obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex+1);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'tr_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarAcoesUnidade&orgcod='+orgcod,ncol.id);
		jQuery("#usuacaresp option").each(function() {
			if ( jQuery("#chk_"+jQuery(this).val()).length ){
				jQuery("#chk_"+jQuery(this).val()).attr('checked',true);
			}
		});
	} else {
		obj.title    = "mais";
		obj.src      = "../../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex+1);
	}

}

function marcarAcao(obj) {

	var periodo = document.getElementById('id_periodo_referencia');

	if(periodo.value == ''){
		obj.checked = false;
		alert('Escolha um Período antes!');
		periodo.focus();
		return false;
	}

    if(obj.checked) {
        if (!jQuery('#usuacaresp option[value='+obj.value+']')[0]) {
            jQuery("#usuacaresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
        }
    } else {
        jQuery('#usuacaresp option[value='+obj.value+']').remove();
    }
}

function enviarFormulario()
{

	var periodo = document.getElementById('id_periodo_referencia');

	if(periodo.value == ''){
		alert('O campo Período é obrigatório!');
		periodo.focus();
		return false;
	}

	selectAllOptions(document.getElementById('usuacaresp'));
	document.formassocia.submit();
}
</script>
<div style="overflow:auto;width:496px;height:350px;border:2px solid #ececec;background-color:white">
<?

$sql = "SELECT
			'<input type=\"checkbox\" name=\"id_subacao\" id=\"chk_'||sac.id_subacao||'\" value=\"'||sac.id_subacao||'\" onclick=\"marcarAcao(this);\"' ||
			case when sac.id_subacao = ur.id_subacao then 'checked=\"checked\"' else '' end ||
			'>' as subacao,
            sba.sbacod || ' - ' || sba.sbatitulo AS descricao
          FROM planacomorc.subacao sac
          INNER JOIN  monitora.pi_subacao sba ON sba.sbacod = sac.codigo
		  LEFT JOIN planacomorc.usuarioresponsabilidade ur on ur.id_subacao = sac.id_subacao and ur.usucpf = '{$usucpf}' and ur.rpustatus = 'A' ".($_GET['id_periodo_referencia'] ? " AND ur.id_periodo_referencia = {$_GET['id_periodo_referencia']} " : " AND ur.id_periodo_referencia IS NULL ")."
          WHERE sac.st_ativo = 'A'
          AND sba.sbastatus = 'A'
          AND sac.id_exercicio = {$_SESSION['exercicio']}
          AND sba.sbaano = '{$_SESSION['exercicio']}'
          ORDER BY sba.sbacod";

$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','N');

?>
</DIV>
<?php

$sql = "
SELECT sac.id_subacao AS codigo,
       sba.sbacod || ' - ' || sba.sbatitulo AS descricao,
       id_periodo_referencia
  FROM planacomorc.usuarioresponsabilidade ur
    INNER JOIN planacomorc.subacao sac using(id_subacao)
    INNER JOIN  monitora.pi_subacao sba ON sba.sbacod = sac.codigo
  WHERE sba.sbastatus = 'A'
    AND sba.sbaano = '{$_SESSION['exercicio']}'
    AND sba.sbaano::numeric = sac.id_exercicio
    AND ur.usucpf = '$usucpf'
	AND ur.pflcod = '$pflcod'
	AND ur.rpustatus = 'A' ".
    ($_GET['id_periodo_referencia'] ? " AND ur.id_periodo_referencia = {$_GET['id_periodo_referencia']} " : " AND ur.id_periodo_referencia IS NULL")."
  ORDER BY sba.sbacod
";

$usuarioresponsabilidade = $db->carregar($sql);
?>
<form name="formassocia" style="margin:0px;" method="POST">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
	<tr>
		<td style="padding:3px;" class="subtituloDireita">
		Período:
		</td>
		<td style="padding:3px;" align="left">
		<?php
		$sql = "select
					id_periodo_referencia as codigo,
					titulo || ' - ' || to_char(inicio_validade, 'DD/MM/YYYY') || ' a ' || to_char(fim_validade, 'DD/MM/YYYY') as descricao
				from planacomorc.periodo_referencia
				order by id_exercicio desc, inicio_validade desc, fim_validade desc";
		$id_periodo_referencia = $_GET['id_periodo_referencia'];
		$db->monta_combo('id_periodo_referencia',$sql, 'S', 'Selecione...', '', '', '', 260, 'N', 'id_periodo_referencia', '', '', '');
		?>
		</td>
	</tr>
</table>
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<input type="hidden" name="requisicao" value="gravarResponsabilidadeAcao">
<select multiple size="8" name="usuacaresp[]" id="usuacaresp" style="width:500px;" class="CampoEstilo">
<?

if($usuarioresponsabilidade[0]) {
	foreach($usuarioresponsabilidade as $ur) {
		echo '<option value="'.$ur['codigo'].'">'.$ur['descricao'].'</option>';
	}
}

?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding:3px;" colspan="3">
			<input type="Button" name="ok" value="OK" onclick="enviarFormulario();" id="ok">
		</td>
	</tr>
</table>