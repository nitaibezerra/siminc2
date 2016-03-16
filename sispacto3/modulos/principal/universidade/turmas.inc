<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));
verificarValidacaoEstruturaFormacao(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['universidade']['docidturmas'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

if($_REQUEST['turid']) {
	$turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid']));
}

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurma() {
	ajaxatualizar('requisicao=carregarAlunosTurma&plpabreviacaosub=<?=$turma['plpabreviacaosub'] ?>&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function abrirAlunosTurma(turid) {
	window.open('sispacto3.php?modulo=principal/universidade/inseriralunosturmas&acao=A&turid='+turid,'Turmas','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmas&pflcodturma=<?=$_REQUEST['pflcodturma'] ?>&turid='+turid;
}

function carregarListaTurmas() {
	ajaxatualizar('requisicao=carregarTurmas&uncid=<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>','td_turmas');
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
   		data: 'requisicao='+jQuery('#requisicao').val()+'&uncid=<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>&turdesc='+jQuery('#turdesc').val()+'&iusd='+jQuery('#iusd').val()+'&turid='+jQuery('#turid').val()+'&muncod_endereco='+jQuery('#muncod_endereco').val(),
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

function excluirAlunoTurma(iusd) {
	var conf = confirm("Deseja realmente excluir o aluno da turma?");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirAlunoTurma&plpabreviacao=<?=$turma['plpabreviacaosub'] ?>&turid=<?=$_REQUEST['turid'] ?>&iusd='+iusd,'');
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

function definirTurmas(pflcodturma) {
	window.location='sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmas&pflcodturma='+pflcodturma;
	
}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='inserirturma']").remove();
});
<? endif; ?>

</script>

<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmas"); ?></td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto3']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Definir turmas do perfil:</td>
		<td><? 
		
		$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p
				WHERE p.pflcod IN(".PFL_FORMADORIES.",".PFL_SUPERVISORIES.",".PFL_COORDENADORADJUNTOIES.")";
		
		$db->monta_combo('pflcodturma', $sql, 'S', 'Selecione', 'definirTurmas', '', '', '', 'N', 'pflcodturma', '',$_REQUEST['pflcodturma']);
		
		?></td>
	</tr>
	
	<? if($_REQUEST['pflcodturma']) : ?>
	
	<tr>
	
	<td colspan="2">
	
	<? if($_REQUEST['turid']) : ?>
	<table width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><a href="sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmas&pflcodturma=<?=$_REQUEST['pflcodturma'] ?>">Lista de turmas</a> >> <?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da turma</td>
		<td><?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"><?=$turma['pfldsc'] ?></td>
		<td><?=$turma['iusnome'] ?></td>
	</tr>
	<tr>
		<td colspan="2"><input type="button" name="inseriraluno" id="inseriraluno" value="Inserir <?=$turma['pfldscsub'] ?>" onclick="abrirAlunosTurma('<?=$turma['turid'] ?>');"></td>
	</tr>
	<tr>
		<td colspan="2" id="td_alunosturmas">
		<?=carregarAlunosTurma(array("turid"=>$turma['turid'],"plpabreviacaosub"=>$turma['plpabreviacaosub'])) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Voltar para lista" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmas&pflcodturma=<?=$_REQUEST['pflcodturma'] ?>';">
		</td>
	</tr>
	</table>
	<? else : ?>
	<table width="100%">
	<tr>
		<td colspan="2" id="td_turmas">
		<?=carregarTurmas(array("consulta"=>$consulta,"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],"pflcodturma"=>$_REQUEST['pflcodturma'])) ?>
		</td>
		<td valign="top" width="5%" rowspan="2">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sispacto3']['universidade']['docidturmas'], array('aba' => $_REQUEST['aba']) );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<?=criarBotoesNavegacao(array('url' => '/sispacto3/sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmas')) ?>
		</td>
	</tr>
	</table>
	<? endif; ?>
	
	</td>
	
	</tr>
	
	
	<? endif; ?>
</table>
