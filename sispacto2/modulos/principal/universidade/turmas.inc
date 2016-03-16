<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));
verificarValidacaoEstruturaFormacao(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto2']['universidade']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}


?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurma() {
	ajaxatualizar('requisicao=carregarAlunosTurma&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function abrirAlunosTurma(turid) {
	window.open('sispacto2.php?modulo=principal/universidade/inseriralunosturmas&acao=A&turid='+turid,'Turmas','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=turmas&turid='+turid;
}

function carregarListaTurmas() {
	ajaxatualizar('requisicao=carregarTurmas&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','td_turmas');
}

function inserirTurma() {

	if(jQuery('#estuf_endereco').val()=='') {
		alert('Estado em branco');
		return false;
	}
	
	if(!document.getElementById('muncod_endereco')) {
		alert('Município em branco');
		return false;
	}
	
	if(jQuery('#muncod_endereco').val()=='') {
		alert('Município em branco');
		return false;
	}

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
   		data: 'requisicao='+jQuery('#requisicao').val()+'&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>&turdesc='+jQuery('#turdesc').val()+'&iusd='+jQuery('#iusd').val()+'&turid='+jQuery('#turid').val()+'&muncod_endereco='+jQuery('#muncod_endereco').val(),
   		async: false,
   		success: function(html){
   	   		alert(html);
   	   		jQuery('#inserirturmas').attr('disabled','');
   	   		jQuery('#estuf_endereco').val('');
   	   		jQuery('#td_municipio3').html('Selecione uma UF');
   	   		}
	});
	carregarListaTurmas();
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
   		jQuery('#requisicao').val('inserirTurma');
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

function excluirAlunoTurma(iusd) {
	var conf = confirm("Deseja realmente excluir o aluno da turma?");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirAlunoTurma&turid=<?=$_REQUEST['turid'] ?>&iusd='+iusd,'');
		carregarAlunosTurma();
	}
}

function excluirTurma(turid) {
	conf = confirm("Deseja realmente excluir esta turma? Caso sim, todos alunos cadastrados nesta turma serão removidos.");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirTurma&turid='+turid,'');
		carregarListaTurmas();
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
<input type="hidden" name="requisicao" value="inserirTurma" id="requisicao">
<input type="hidden" name="turid" value="" id="turid">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Turma</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"><b>Polo:</b></td>
	<td> 

	<table width="100%">
	<tr>
	<td>Estado:</td><td><? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', ''); 
	?></td>
	<td>Município:</td>
	<td id="td_municipio3">
	<? 
	if($estuf_endereco) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf_nascimento."' ORDER BY mundescricao";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '');
	else :
		echo "Selecione uma UF";
	endif;
	?> 
	</td>
	</tr>
	</table>
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"><b>Nome da turma:</b></td>
	<td><?=campo_texto('turdesc', "S", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="turdesc"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"><b>Formador responsável:</b></td><td><? 
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao 
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON i.iusd = t.iusd 
			WHERE t.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' 
			ORDER BY i.iusnome";
	$db->monta_combo('iusd', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'iusd', ''); 
	?></td>
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
		<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=turmas"); ?></td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto2']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<? if($_REQUEST['turid']) : ?>
	<? $turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid'])); ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><a href="sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=turmas">Lista de turmas</a> >> <?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da turma</td>
		<td><?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Formador responsável</td>
		<td><?=$turma['iusnome'] ?></td>
	</tr>
	<tr>
		<td colspan="2"><input type="button" name="inseriraluno" id="inseriraluno" value="Inserir Orientadores" onclick="abrirAlunosTurma('<?=$turma['turid'] ?>');"></td>
	</tr>
	<tr>
		<td colspan="2" id="td_alunosturmas">
		<?=carregarAlunosTurma(array("turid"=>$turma['turid'])) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Voltar para lista" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=turmas';">
		</td>
	</tr>
	<? else : ?>
	<?
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total));
	$formadoressolicitados = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid'],"pflcod"=>PFL_FORMADORIES,"tpejustificativaformadories"=>true))
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de turmas estimado</td>
		<td><?=$nturmas ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de turmas solicitado</td>
		<td><?=count($formadoressolicitados) ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Total:</td>
		<td><?=(count($formadoressolicitados)+$nturmas) ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type="button" name="inserirturma" id="inserirturma" value="Inserir Turma" onclick="abrirCadastroTurma('');"></td>
	</tr>
	<tr>
		<td colspan="2" id="td_turmas">
		<?=carregarTurmas(array("consulta"=>$consulta,"uncid"=>$_SESSION['sispacto2']['universidade']['uncid'])) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?>
		</td>
	</tr>
	<? endif; ?>
</table>
