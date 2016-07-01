<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
 
<? if($_REQUEST['pieid']) : ?>

<?

$sql = "SELECT *, (SELECT iusnome FROM smfc.identificacaousuario i INNER JOIN smfc.tipoperfil t ON t.iusid = i.iusid WHERE insid=i.insid AND t.pflcod='".PFL_COORDENADOR_INSTITUCIONAL."') as coordenadorinstitucional FROM smfc.planejamentoinstituicaoexercicio p 
		INNER JOIN smfc.instituicoes i ON i.insid = p.insid 
		WHERE pieid='".$_REQUEST['pieid']."'";

$planejamentoinstituicaoexercicio = $db->pegaLinha($sql);

?>
<script>

function inserirCursoPlanejamento(curid, obj) {
	
	ajaxatualizar('requisicao=inserirCursoPlanejamento&pieid=<?=$_REQUEST['pieid'] ?>&curid='+curid,'');
	ajaxatualizar('requisicao=listaCursosPlanejamento&pieid=<?=$_REQUEST['pieid'] ?>&sis=<?=$sis ?>','td_listacursosplanejamento');
	
	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	tabela.deleteRow(linha.rowIndex-1);
	

}

function exibirListaCursos() {
	
	jQuery("#modalListaCursos").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function exibirAbrangenciaCursos(picid) {
	
	jQuery("#modalAbrangenciaCursos").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}


function analiseSituacao(picid, obj) {
	if(obj.value=='P') {
		jQuery('#dadosrepactua_'+picid).css('display','');
	} else {
		jQuery('#dadosrepactua_'+picid).css('display','none');
	}
}

function salvarPlanejamentoInstituicaoCurso() {

	var vagasestimadas_p = false;

	jQuery("[id^='picvagasestimadas_']").each(function() {
	
		if(jQuery(this).val()=='') {
			vagasestimadas_p = true;
		}
	
	});
	
	if(vagasestimadas_p) {
		alert('Todas as vagas estimadas devem ser preenchidas');
		return false;
	}
	
	var valorprevisto_p = false;
	
	jQuery("[id^='picvalorprevisto_']").each(function() {
	
		if(jQuery(this).val()=='') {
			valorprevisto_p = true;
		}
	
	});
	
	if(valorprevisto_p) {
		alert('Todos os valores previstos devem ser preenchidos');
		return false;
	}
	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "requisicao");
	input.setAttribute("value", "salvarPlanejamentoInstituicaoCurso");
	document.getElementById("formulario").appendChild(input);
	
	jQuery('#formulario').submit();

}

</script>

<div id="modalAbrangenciaCursos" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">UF</td>
	<td>UF</td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td>MM</td>
</tr>
<tr>
	<td colspan="2">[xxxxxxxxxxxxxxxxxxxxxx]</td>
</tr>
</table>

</div>


<div id="modalListaCursos" style="display:none;">

<?

$sql = "SELECT '<img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer\" onclick=\"inserirCursoPlanejamento(' || curid || ', this);\">' as inserir, 
			   curdesc 
		FROM catalogocurso.curso 
		WHERE curstatus='A' AND 
			  curid NOT IN( SELECT curid FROM smfc.planejamentoinstituicaocurso WHERE pieid='".$_REQUEST['pieid']."' AND picstatus='A' ) 
		ORDER BY curdesc";

$cabecalho = array("&nbsp;","Curso");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);

?>

</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td valign="top" width="99%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro" colspan="2">&nbsp;</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Exercício : </td>
		<td><?=$planejamentoinstituicaoexercicio['pieexercicio'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Instituição : </td>
		<td><?=$planejamentoinstituicaoexercicio['inssigla'] ?> / <?=$planejamentoinstituicaoexercicio['insnome'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Coordenador Institucional : </td>
		<td><?=$planejamentoinstituicaoexercicio['coordenadorinstitucional'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type="button" name="inserir" value="Inserir Cursos" onclick="exibirListaCursos();"></td>
	</tr>
	<tr>
		<td colspan="2" id="td_listacursosplanejamento">
		<? listaCursosPlanejamento(array('pieid' => $_REQUEST['pieid'],'sis' => $sis)); ?>	
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td><input type="button" name="salvar" value="Salvar" onclick="salvarPlanejamentoInstituicaoCurso();"></td>
	</tr>
	</table>
	
	</td>
	
	<td valign="top">
	<?
	/* Barra de estado atual e ações e Historico */
	wf_desenhaBarraNavegacao( $planejamentoinstituicaoexercicio['docid'], array() );			
	?>
	</td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro"><input type="button" value="Retornar para lista" onclick="window.location='smfc.php?modulo=principal/mec/meccadastro&acao=A&aba=planejamento';"></td>
</tr>
</table>
<? else : ?>
<script>

function editarPlanejamento(pieid) {

	window.location='<?=$_SERVER['REQUEST_URI'] ?>&pieid='+pieid;
	
}

function exibirInsercaoPlanejamento() {
	
	jQuery("#modalInserirPlanejamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function salvarPlanejamentoInstituicaoExercicio() {

	if(jQuery('#pieexercicio').val()=='') {
		alert('Selecione um Exercício');
		return false;
	}
	
	if(jQuery('#insid').val()=='') {
		alert('Selecione uma instituição');
		return false;
	}
	
	jQuery('#form_planejamento').submit();

}
</script>

<div id="modalInserirPlanejamento" style="display:none;">

<form method="post" name="form_planejamento" id="form_planejamento">
<input type="hidden" name="requisicao" value="salvarPlanejamentoInstituicaoExercicio">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Exercício</td>
	<td>
	<?
	
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos ORDER BY ano";
    $db->monta_combo('pieexercicio', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'pieexercicio',''); 
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Instituição</td>
	<td>
	<?
	
	$sql = "SELECT insid as codigo, inssigla || ' / ' || insnome as descricao FROM smfc.instituicoes WHERE insstatus='A'";
    $db->monta_combo('insid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'insid',''); 
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="inserir" value="Salvar" onclick="salvarPlanejamentoInstituicaoExercicio();"></td>
</tr>
</table>
</form>

</div>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao(""); ?></td>
</tr>
<? if($sis=='mec') : ?>
<tr>
	<td class="SubTituloEsquerda" colspan="2"><input type="button" name="inserir" value="Inserir Planejamento" onclick="exibirInsercaoPlanejamento();"></td>
</tr>
<? endif; ?>
<tr>
	<td colspan="2">
	<?
	
	if($sis=='coordenadorinstitucional') {
		 $where[] = "p.insid='".$_SESSION['smfc']['coordenadorinstitucional']['insid']."'";
	}
	
	$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"editarPlanejamento('||p.pieid||');\">' as acao, 
				   pieexercicio,
				   i.inssigla||' / '||i.insnome as instituicao,
				   e.esddsc,
				   COUNT(pic.picid) as ncursos
			FROM smfc.planejamentoinstituicaoexercicio p 
			INNER JOIN smfc.instituicoes i ON i.insid = p.insid 
			LEFT JOIN smfc.planejamentoinstituicaocurso pic ON pic.pieid = p.pieid AND pic.picstatus='A' 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			".(($where)?"WHERE ".implode(" AND ",$where):"")."
			GROUP BY p.pieid, p.pieexercicio, i.inssigla, i.insnome, e.esddsc 
			ORDER BY 2,3";
	
	$cabecalho = array("&nbsp;","Exercício","Instituição","Situação","Número de cursos");
	$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));
	
	?>	
	</td>
</tr>
</table>
<? endif; ?>