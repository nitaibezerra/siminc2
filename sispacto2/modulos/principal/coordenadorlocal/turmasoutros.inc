<?

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurmaOutros() {
	ajaxatualizar('requisicao=carregarAlunosTurmaOutros&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function abrirAlunosTurma(turid) {
	window.open('sispacto2.php?modulo=principal/coordenadorlocal/inseriralunosturmas&acao=A&turid='+turid,'Turmas','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmasoutros&turid='+turid;
}

function carregarListaTurmasOutros() {
	ajaxatualizar('requisicao=carregarTurmasOutros&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','td_turmas');
}

function inserirTurma() {

	if(jQuery('#turdesc').val()=='') {
		alert('Nome da turma em branco');
		return false;
	}
	
	if(jQuery('#iusd').val()=='') {
		alert('Formador responsável em branco');
		return false;
	}
	
	jQuery('#inserirturmas').attr('disabled','disabled');
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao='+jQuery('#requisicao').val()+'&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>&turdesc='+jQuery('#turdesc').val()+'&iusd='+jQuery('#iusd').val()+'&turid='+jQuery('#turid').val(),
   		async: false,
   		success: function(html){alert(html);}
	});
	carregarListaTurmasOutros();
	jQuery('#modalTurma').dialog('close');
}

function abrirCadastroTurma(turid) {

	divCarregando();
	
	if(turid!='') {
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: 'requisicao=carregarDadosTurma&return=json&turid='+turid,
	   		async: false,
	   		success: function(html){
	   		var myObject = eval('(' + html + ')');
	   		jQuery('#requisicao').val('atualizarTurma');
			jQuery('#turdesc').val(myObject.turdesc);
	   		jQuery('#iusd').val(myObject.iusd);
			jQuery('#turid').val(myObject.turid);
			jQuery('#estuf_endereco').val(myObject.estuf);
			jQuery('#estuf_endereco').change();
			jQuery('#muncod_endereco').val(myObject.muncod);
	   		}
		});
		

	} else {
		jQuery('#turid').val('');
		jQuery('#turdesc').val('');
		jQuery('#iusd').val('');
   		jQuery('#requisicao').val('inserirTurmaOutros');
	}
	
	jQuery("#modalTurma").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
	                    
	divCarregado();
}

function excluirAlunoTurmaOutros(iusd) {
	var conf = confirm("Deseja realmente excluir o aluno da turma?");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirAlunoTurmaOutros&turid=<?=$_REQUEST['turid'] ?>&iusd='+iusd,'');
		carregarAlunosTurmaOutros();
	}
}

function excluirTurmaOutros(turid) {
	conf = confirm("Deseja realmente excluir esta turma? Caso sim, todos alunos cadastrados nesta turma serão removidos.");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirTurmaOutros&turid='+turid,'');
		carregarListaTurmasOutros();
	}
}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='inserirturma']").remove();
});
<? endif; ?>

</script>

<div id="modalTurma" style="display:none;">
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="inserirTurmaOutros" id="requisicao">
<input type="hidden" name="turid" value="" id="turid">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Turma</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"><b>Nome da turma:</b></td>
	<td><?=campo_texto('turdesc', "S", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="turdesc"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"><b>Responsável:</b></td>
	<td>
	<?
	$sql = "SELECT i.iusd as codigo, UPPER(p.pfldsc)||' - '||i.iusnome|| CASE WHEN tu.turid IS NOT NULL THEN ' (Possui turma)' ELSE ' (Não Possui turma)' END as descricao 
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON i.iusd = t.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto2.turmas tu ON tu.iusd = i.iusd 
			WHERE t.pflcod IN('".PFL_SUPERVISORIES."','".PFL_COORDENADORLOCAL."') AND i.picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'  
			ORDER BY p.pfldsc, i.iusnome";
	
	$db->monta_combo('iusd', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'iusd', '');
	 
	?>
	</td>
</tr>
<tr>	
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Inserir" onclick="inserirTurma();" id="inserirturmas"></td>
</tr>

</table>
</form>
</div>



<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmasoutros"); ?></td>
	</tr>
	<? if($_REQUEST['turid']) : ?>
	<? $turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid'])); ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><a href="sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmasoutros">Lista de turmas</a> >> <?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da turma</td>
		<td><?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Responsável</td>
		<td><?=$turma['iusnome'] ?></td>
	</tr>
	<tr>
		<td colspan="2"><input type="button" name="inserirequipe" id="inserirequipe" value="Inserir Equipe" onclick="abrirAlunosTurma('<?=$turma['turid'] ?>');"></td>
	</tr>
	<tr>
		<td colspan="2" id="td_alunosturmas">
		<?=carregarAlunosTurmaOutros(array("turid"=>$turma['turid'])) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Voltar para lista" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmasoutros';">
		</td>
	</tr>
	<? else : ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">
		<? if($db->testa_superuser()) :?>
		<input type="button" name="inserirturma" id="inserirturma" value="Inserir Turma" onclick="abrirCadastroTurma('');">
		<? endif; ?>
		</td>
		<!-- 
		<td rowspan="3" valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sispacto2']['universidade']['docidturma'], array('picid' => false, 'uncid'=> $_SESSION['sispacto2']['universidade']['uncid']) );
		?>
		</td>
		 -->
	</tr>
	<tr>
		<td colspan="2" id="td_turmas">
		<?=carregarTurmasOutros(array("consulta"=>$consulta,"picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"pflcod" => PFL_COORDENADORLOCAL)) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td colspan="2">
		<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?>
		</td>
	</tr>
	<? endif; ?>
</table>
