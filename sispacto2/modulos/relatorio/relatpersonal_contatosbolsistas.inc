<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>

<p align="center" style="font-size:16px;"><b>Contato dos Bolsistas</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

?>
<form method="post" id="formulario" action="sispacto2.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_contatosbolsistas.inc">
<input type="hidden" name="filtros" id="filtros" value="1">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td class="SubTituloDireita">UF :</td>
	<td><?php 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estufatuacao', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF5', '', '', '', 'S', 'estufatuacao', '');
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio5">
	<?
	if($muncodatuacao) {
		carregarMunicipiosPorUF(array('id'=>'muncod_abrangencia','name'=>'muncod_abrangencia','estuf'=>$estufatuacao,'valuecombo'=>$muncodatuacao));
	} else echo "Selecione uma UF";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Perfil</td>
	<td><? 
	
	$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p
		INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = p.pflcod
		WHERE sisid='".SIS_SISPACTO."'";
	$db->monta_combo('pflcod', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'pflcod');
	
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Ver HTML"><input type="button" value="Ver XLS" onclick="jQuery('#filtros').val('2');jQuery('#formulario').submit();"></td>
</tr>
</table>
</form>



<? 
if($_REQUEST['filtros']) : 

if($_REQUEST['estufatuacao']) {
	$wh[] = "m.estuf='".$_REQUEST['estufatuacao']."'";
}

if($_REQUEST['muncodatuacao']) {
	$wh[] = "m.muncod='".$_REQUEST['muncodatuacao']."'";
}

if($_REQUEST['pflcod']) {
	$wh[] = "t.pflcod='".$_REQUEST['pflcod']."'";
}

echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "SELECT iusnome,
			   iusemailprincipal,
			   pe.pfldsc,
			   CASE WHEN p.muncod IS NOT NULL THEN m.estuf||' - '||m.mundescricao||'( Municipal )'
					WHEN p.estuf IS NOT NULL THEN m.estuf||' - '||m.mundescricao||'( Estadual )' 
					ELSE 'Equipe IES' END as rede
		FROM sispacto2.identificacaousuario i
		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod 
		LEFT JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
		WHERE i.iusstatus='A' ".(($wh)?"AND ".implode(" AND ",$wh):"");

$cabecalho = array("Nome","E-mail","Perfil","Rede");

if($_REQUEST['filtros']=='2') {

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
	exit;

} else {
	$db->monta_lista_simples($sql,$cabecalho,100000000,5,5,'N','100%','N');
}

echo '</td>';
echo '</tr>';
echo '</table>';


endif; 
?>