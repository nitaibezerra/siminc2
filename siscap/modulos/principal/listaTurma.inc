<?php

if( $_POST['requisicao'] == 'excluir' && !empty($_POST['turid']) ){

	$sql = "DELETE FROM siscap.turma_facilitador WHERE turid = ".$_POST['turid'];
	$db->executar( $sql );

	$sql = "DELETE FROM siscap.turma WHERE turid = ".$_POST['turid'];
	$db->executar( $sql );

	if($db->commit()){
		$db->sucesso( 'principal/listaTurma' );
	} else {
		echo "<script>
				alert('alha na exclução da turma');
				window.location.href = 'siscap.php?modulo=principal/listaTurma&acao=A';
			 </script>";
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Lista de Turma(s) Cadastrada(s)', '' );
?>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="turid" id="turid" value="">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">Curso:</td>
			<td colspan="3" width="35%">
				<?
				$curdsc = $_REQUEST['curdsc'];
				/*$sql = "SELECT curid as codigo, curnome as descricao
						FROM siscap.curso
						ORDER BY curnome";
				$db->monta_combo( "curnome", $sql, 'S', 'Selecione...', '', '', '', '265' );*/

				echo campo_texto('curdsc', 'N', 'S', 'Curso', 80, 50, '', '', '', '', 0, 'id=curdsc autocomplete=off' );
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
				<input type="button" id="btPesquisar" value="Pesquisar" onclick="pesquisar();" />
				<input type="button" id="btCadastro" value="Novo" onclick="novaTurma();" />
			</td>
		</tr>
	</table>
</form>
<?

$where.= (!empty($_POST['curdsc']) ? " WHERE cur.curdsc ilike '%".$_POST['curdsc']."%'" : '' );

$cabecalho = array("Ações", "Curso", "Turma", "Ano Inicio/Fim", "Status");
$tamanho = array( '10%', '35%', '20%', '10%', '20%' );
$alinhamento = array( '', 'left', '', 'center', 'center' );

$sql = "SELECT ( '<center><a href=\"siscap.php?modulo=principal/gerenciaTurma&acao=A&turid='|| tur.turid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			CASE WHEN (SELECT count(atuid) FROM siscap.aluno_turma WHERE turid = tur.turid ) > 0 THEN
				'<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\">'
            ELSE
            	'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirTurma('|| tur.turid ||');\" border=0 alt=\"Ir\" title=\"Excluir\">'
            END)
            || ' <a href=\"siscap.php?modulo=principal/gerenciaTurma&acao=A&visualizar=1&turid='|| tur.turid ||'\"><img src=\"/imagens/consultar.gif\" border=0 alt=\"Ir\" title=\"Visualizar\"></a>'
            || ' <a href=\"siscap.php?modulo=relatorio/relatorioTurmas&acao=A&turid='|| tur.turid ||'\"><img src=\"/imagens/report.gif\" height=\"15\" border=0 alt=\"Ir\" title=\"Relatório de Inscrições\"></a></center>' as acao, cur.curdsc,
			     '<center>'||tur.turturma||'</center>', to_char(tur.turdtini, 'YYYY') ||'/'||to_char(tur.turdtfim, 'YYYY') as turdtfim,
				case when tur.turstatus = 'EA' then 'Em andamento'
					 when tur.turstatus = 'IA' then 'Inscrições Abertas'
					 when tur.turstatus = 'IE' then 'Inscrições Encerradas'
					 when tur.turstatus = 'CO' then 'Concluído'
					 when tur.turstatus = 'AD' then 'Adiado'
					 when tur.turstatus = 'CA' then 'Cancelado'
			    end as turperiodo
			FROM siscap.turma tur
				inner join siscap.curso cur on cur.curid = tur.curid
			$where
			order by
				cur.curdsc, tur.turturma";

$db->monta_lista($sql, $cabecalho, 10, 4, 'N','Center','','form', $tamanho, $alinhamento);
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	var form		=	$("formulario");
	var btPesquisar	=	$("btPesquisar");
	var btCadastro	=	$("btCadastro");

	function pesquisar(){
		btPesquisar.disabled = true;
		btCadastro.disabled  = true;

		curdsc	= document.getElementsByName("curdsc")[0];

		if(curdsc.value.length > 0 && curdsc.value.length < 3) {
			alert("Para realizar a busca por nome do curso é necessário informar pelo menos 3 caracteres.");
			curdsc.focus();
			btPesquisar.disabled = false;
			btCadastro.disabled  = false;
			return;
		}
		form.submit();
	}
	function novaTurma(){
		btPesquisar.disabled = true;
		btCadastro.disabled  = true;
		window.location.href = 'siscap.php?modulo=principal/gerenciaTurma&acao=A';
	}
	function excluirTurma( turid ){
		if(confirm("Deseja realmente excluir está Turma ?")) {
			$('requisicao').value = 'excluir';
			$('turid').value = turid;
			form.submit();
		}
	}
</script>