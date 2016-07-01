<?php
/*
 * Verifica qual página sera redirecionado
 */
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";

redirecionaUnidadePerfil('gerenciar');

exit;

$permissoes = verificaPerfilPlanoTrabalho();

if($_REQUEST['entid'] && $_REQUEST['tipo']) {
	if(!$permissoes['naoverunidades']) {
		validaAcessoUnidade($permissoes['verunidades'],$db->pegaUm("SELECT entunicod FROM entidade.entidade WHERE entid='".$_REQUEST['entid']."'"));
		// caso seja validado com sucesso é direcionado para o gerenciamento de subatividades
		$_SESSION['monitora_var']['tipo'] = $_REQUEST['tipo'];
		$_SESSION['monitora_var']['entid'] = $_REQUEST['entid'];
		header("location: ?modulo=principal/planotrabalho/subatividades&acao=A");
		exit; 
	}
}

/*
 * FIM TRECHO QUE TESTA A SEGURANÇA DO SISTEMA
 */


// entidade.funcao, ID das unidades gestoras
define("FUNID_UG", 34);

// ID que não se referem as unidades
$funid_no = array('fen.funid <> 31','fen.funid <> 32','fen.funid <> 34');

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

// carregando as unidades
if(!$permissoes['naoverunidades']) {
	$sql = "SELECT ent.entid, ent.entnome, ent.entunicod FROM entidade.entidade ent
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			WHERE entunicod ilike '26%' AND entunicod IS NOT NULL AND 
				(".implode(" AND ",$funid_no).") " . 
				(($permissoes['verunidades'][0]) ? "AND entunicod IN('".implode("','",$permissoes['verunidades'])."')" : "") . 
		   "
		   GROUP BY ent.entid, ent.entnome, ent.entunicod
		   ORDER BY entunicod";
	$unidades = $db->carregar($sql);
	
	if ( is_array($permissoes['verunidades']) && count($permissoes['verunidades']) == 1 && !is_array($permissoes['verunidadesgestoras']) ){
		die("<script>
//				alert('unicod');
				location.href ='?modulo=principal/planotrabalho/plano&acao=A&tipo=un&entid=" . $unidades[0]['entid'] . "';
			 </script>");
	}elseif($permissoes['verunidades'][0] == '26101' && is_array($permissoes['verunidadesgestoras']) && count($permissoes['verunidadesgestoras']) == 1 ){
		$sql = "SELECT ent.entid FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid  
				WHERE ENTUNGCOD NOT IN ('152004','152005') AND 
					entunicod = '" . $permissoes['verunidades'][0] . "' AND 
					fen.funid='".FUNID_UG."' AND
					entungcod = '" . $permissoes['verunidadesgestoras'][0] . "'
				ORDER BY 
					entungcod";
		//ECHO $sql;
//		dbg($sql,1);
		$entid = $db->pegaUm($sql);
		
		die("<script>
//				alert('ungcod');
				location.href ='?modulo=principal/planotrabalho/plano&acao=A&tipo=ug&entid=" . $entid . "';
			 </script>");		
	}
}

?>
<script>
function controlarUG(img) {
	if(img.title == 'abrir') {
		document.getElementById('tr_unidade'+img.id).style.display = '';
		img.src   = '../imagens/menos.gif';
		img.title = 'fechar';
	} else {
		document.getElementById('tr_unidade'+img.id).style.display = 'none';
		img.src   = '../imagens/mais.gif';
		img.title = 'abrir';
	}
}

function irPlanoTrabalho(entid,tipo) {
	window.location='?modulo=principal/planotrabalho/plano&acao=A&tipo='+tipo+'&entid='+entid;
}
</script>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
<tr>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>&nbsp;</strong></td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidades</strong></td>
</tr>
</thead>
<?
// verifica se existem unidades
if($unidades[0]) {
	$contador=0; // contador das linhas
	foreach($unidades as $unidade) {
		/* verifica se existe unidade gestora, se existir, criar uma arvore para mostra-las como
		 * filhas da unidade
		 */
		$sql = "SELECT '<img src=\'../imagens/seta_filho.gif\'>' as img, '<a href=\'#\' style=\'cursor:pointer\' onclick=\"irPlanoTrabalho(\''|| ent.entid ||'\',\'ug\');\">'||entungcod||' - '||entnome||'</a>' 
				FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				WHERE ENTUNGCOD NOT IN ('152004','152005') " . ( is_array($permissoes['verunidadesgestoras']) ? " AND entungcod IN ('" . implode("','", $permissoes['verunidadesgestoras']) . "')"  : "") . "  AND fea.entid = '".$unidade['entid']."' AND fen.funid='".FUNID_UG."' ORDER BY entungcod";
		//ECHO $sql;
		$unidadesgestoras = $db->carregar($sql);
?>
<tr bgcolor="<? echo ((fmod($contador,2) == 0)?'':'#F7F7F7'); ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<? echo ((fmod($contador,2) == 0)?'':'#F7F7F7'); ?>';">
	<td style="width:60px;text-align:center;"><? echo (($unidadesgestoras[0])?"<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\"  id='".$unidade['unicod']."' onclick=\"controlarUG(this);\" title=\"abrir\">":"<img src='../imagens/seta_filho.gif'>"); ?></td>
	<td>
	<?
	// se não existir ugs, basta clicar na unidade, senão as ugs que deverão ser clicadas
	if($unidadesgestoras[0]) {
		echo $unidade['entunicod']." - ".$unidade['entnome'];
	} else {
		echo "<a href=\"#\" style='cursor:pointer' onclick=\"irPlanoTrabalho('".$unidade['entid']."','un');\">".$unidade['entunicod']." - ".$unidade['entnome']."</a>";	
	}
	?>
	</td>
</tr>
<?
		// caso exista unidades gestoras
		if($unidadesgestoras[0]) {
?>
<tr id="tr_unidade<? echo $unidade['unicod']; ?>" style="display:none">
	<td style="width:60px;text-align:center;">&nbsp;</td>
	<td><? echo $db->monta_lista_simples($unidadesgestoras, array("","Unidades Gestoras"), 50, 10, 'N', '100%', '' ); ?></td>
</tr>
<?
		}
		$contador++; // incrementa contador de unidades
	}
} else {
?>
<tr>
	<td align="center" style="color:#cc0000;" colspan="2">Não existem unidades atribuídas para seu perfil.</td>
</tr>
<?
}
?>
</table>