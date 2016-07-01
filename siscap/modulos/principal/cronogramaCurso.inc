<?php
if($_REQUEST['montalistaturmas']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaCursoTurmas( $_POST['curid'] );
	exit;
}

unset($_SESSION['siscap']['turid']);
unset($_SESSION['siscap']['curid']);
unset($_SESSION['siscap']['tipo']);

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Cronograma de Cursos', '' );

$sql = "SELECT c.curid, a.areid, a.aredsc, c.curdsc,
			(select count(t.turid) from siscap.turma t WHERE t.curid = c.curid and t.turstatus = 'IA') as total
		FROM siscap.curso c
			inner join siscap.area a on a.areid = c.areid
        WHERE a.arestatus = 'A'
        ORDER BY a.aredsc, c.curdsc";

$arCurso = $db->carregar( $sql );
$arCurso = $arCurso ? $arCurso : array();
?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<?
	$areid = '';
	foreach ($arCurso as $key => $curso) {
		$incricoes = ($curso['total']>0 ? ' <b>(Inscrições Abertas)</b>' : '');
		//ver($curso['total']);
		if( $curso['areid'] != $areid ){
			if( $key != 0 ){?>
				<tr>
					<td style="height: 5px;"></td>
				</tr>
			<?} ?>
			<tr>
				<th colspan="2" class="subtituloesquerda">Área: <? echo $curso['aredsc']; ?></th>
			</tr>
	<?		$areid = $curso['areid'];
		}?>
	<tr>
		<td width="15%" class="subtitulodireita">Curso:</td>
		<td width="85%"><img src="/imagens/mais.gif" border=0 alt="Ir" id="imagemid_<?=$curso['curid']; ?>"  style="cursor:pointer" onclick="carregaIMGListaTurmas( this.id, <?=$curso['curid']; ?> );">
		<? echo $curso['curdsc'].$incricoes; ?>
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
	var myajax = new Ajax.Request('siscap.php?modulo=principal/cronogramaCurso&acao=A', {
			        method:     'post',
			        parameters: '&montalistaturmas=true&curid='+curid,
			        asynchronous: false,
			        onComplete: function (res){
						$(tr_nome).update(res.responseText);
						//$('turmas_40').innerHTML = res.responseText;
			        }
			  });
}
function imprimir(id){
    window.open('siscap.php?modulo=principal/popupImprimirTurma&acao=A&turid='+id,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}
</script>