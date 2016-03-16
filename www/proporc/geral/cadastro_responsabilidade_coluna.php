<?php
/**
 * Cadastro de responsabilidades de usuário sobre Colunas. As colunas escolhidas para
 * o usuário serão aquelas que ele deverá preencher.
 * $Id: cadastro_responsabilidade_coluna.php 81735 2014-06-18 14:50:32Z maykelbraz $
 */

/**
 *
 */
require_once "config.inc";
/**
 *
 */
include APPRAIZ . "includes/classes_simec.inc";
/**
 *
 */
include APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();
$esquema = 'altorc';

function gravarResponsabilidade($dados) {
    global $db;
    $sql = <<<DML
UPDATE proporc.usuarioresponsabilidade
  SET rpustatus = 'I'
  WHERE usucpf = '{$dados['usucpf']}'
    AND pflcod = '{$dados['pflcod']}'
DML;
    $db->executar($sql);

    if ($dados['usuresp']) {
        foreach($dados['usuresp'] as $resp) {

            $sql = <<<DML
INSERT INTO proporc.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, mtrid)
  VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$resp}')
DML;
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
<meta http-equiv="Pragma" content="no-cache">
<title>Definição de responsabilidades - Unidade Orçamentária</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>
<style type="text/css">
.tabela{width:100%}
</style>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff" onload="self.focus()">
<script>
function marcar(obj) {
    if (obj.checked) {
        if (!jQuery('#usuresp option[value='+obj.value+']')[0]) {
            jQuery("#usuresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
        }
    } else {
        jQuery('#usuresp option[value='+obj.value+']').remove();
    }
}
</script>
<div style="overflow:auto;width:496px;height:350px;border:2px solid #ececec;background-color:white">
<?php
monta_titulo('Definição de responsabilidades - Unidade Orçamentária', '');

$sql = <<<DML
SELECT '<input type="checkbox" name="mtrid" id="chk_' || mtr.mtrid || '" value="' || mtr.mtrid || '" '
           || 'onclick="marcar(this)"'
           || case WHEN (SELECT count(urp.rpuid)
                           FROM proporc.usuarioresponsabilidade urp
                           WHERE urp.mtrid = mtr.mtrid
                             AND urp.usucpf = '{$usucpf}'
                             AND urp.pflcod = '{$pflcod}'
                             AND rpustatus = 'A') > 0 THEN ' checked' ELSE '' END || '>' AS mtrid,
       gpm.gpmdsc || ': ' || mtr.mtrdsc AS descricao
  FROM elabrev.matriz mtr
    INNER JOIN elabrev.grupomatriz gpm USING(gpmid)
  WHERE mtr.mtrano = '2014'
    AND EXISTS (SELECT 1
                  FROM elabrev.unidadematriz udm
                  WHERE udm.mtrid = mtr.mtrid
                    and udm.unicod = '26101')
  ORDER BY gpm.gpmordem, mtr.mtrdsc
DML;

    $cabecalho = array('', 'Grupo: Coluna');
$db->monta_lista_simples($sql, $cabecalho, 2000, 5, 'N', '100%', 'N');
?>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?php echo $usucpf?>">
<input type="hidden" name="pflcod" value="<?php echo $pflcod?>">
<input type="hidden" name="requisicao" value="gravarResponsabilidade">
<select multiple size="8" name="usuresp[]" id="usuresp" style="width:500px" class="CampoEstilo">
<?
$sql = <<<DML
SELECT rpu.mtrid AS codigo,
       gpm.gpmdsc || ': ' || mtr.mtrdsc AS descricao
  FROM proporc.usuarioresponsabilidade rpu
    INNER JOIN elabrev.matriz mtr USING(mtrid)
    INNER JOIN elabrev.grupomatriz gpm USING(gpmid)
  WHERE rpu.usucpf = '{$usucpf}'
    AND rpu.pflcod = '{$pflcod}'
    AND rpu.rpustatus = 'A'
DML;
$usuarioresponsabilidade = $db->carregar($sql);

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
		<td align="right" style="padding:3px" colspan="3">
			<input type="Button" name="ok" value="OK"
                   onclick="selectAllOptions(document.getElementById('usuresp'));document.formassocia.submit();"
                   id="ok">
		</td>
	</tr>
</table>