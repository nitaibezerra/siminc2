<?php

include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();

if($_REQUEST['montalistaturmas']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaCursoTurmas( $_POST['curid'] );
	exit;
}

//unset($_SESSION['siscap']['turid']);



$sql = "SELECT c.curid, a.areid, a.aredsc, c.curdsc
		FROM siscap.curso c inner join siscap.area a on a.areid = c.areid
        WHERE a.arestatus = 'A'
        ORDER BY a.aredsc, c.curdsc";

$arCurso = $db->carregar( $sql );
$arCurso = $arCurso ? $arCurso : array();
?>	
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<?
	$areid = '';
	foreach ($arCurso as $key => $curso) {
		if( $curso['areid'] != $areid ){
			if( $key != 0 ){?>
				<tr>
					<td style="height: 5px;"></td>
				</tr>
			<?} ?>
			<tr>
				<th colspan="3" class="subtituloesquerda">Área: <? echo $curso['aredsc']; ?></th>				
			</tr>
	<?		$areid = $curso['areid']; 
		}?>
	<tr>
		<td width="15%" class="subtitulodireita">Curso:</td>
		<td width="85%"><img src="/imagens/mais.gif" border=0 alt="Ir" id="imagemid_<?=$curso['curid']; ?>"  style="cursor:pointer" onclick="carregaIMGListaTurmas( this.id, <?=$curso['curid']; ?> );"> 
		<? echo $curso['curdsc']; ?>
		</td>
	</tr>
	<tr id="turmas_<?=$curso['curid']; ?>" style="display: none;"></tr>
	<?} ?>
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
function carregaIMGListaTurmas(idImg, curid){
	var img 	 = $( idImg );
	var tr_nome = 'turmas_'+ curid;
	//var td_nome  = 'trV_'+ curid;

	if($(tr_nome).style.display == 'none'){
		//$(tr_nome).innerHTML = 'Carregando...';
		img.src = '../imagens/menos.gif';
		$(tr_nome).style.display = '';
		montaListaTurmas( curid, tr_nome );
	} /*else if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	}*/ else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}
function montaListaTurmas( curid, tr_nome ){	
	var myajax = new Ajax.Request('/siscap/cronogramaCurso.php', {
			        method:     'post',
			        parameters: '&montalistaturmas=true&curid='+curid,
			        asynchronous: false,
			        onComplete: function (res){
						$(tr_nome).update(res.responseText);
						//$('turmas_40').innerHTML = res.responseText;
			        }
			  });
}
</script>