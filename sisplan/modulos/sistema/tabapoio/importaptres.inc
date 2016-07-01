<?php
include APPRAIZ . 'includes/CsvToArray.Class.php';
$modulo = $_REQUEST['modulo'] ;

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid']
);

switch( $_REQUEST['evento'] ){

	case 'cadastrar_anexo':

		// obtém o arquivo
		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}

        $contador=1;
		foreach (CsvToArray::open($arquivo['tmp_name']) as $c)
		 {

			if ( trim($c[0]) != '' && trim($c[1]) != '' && trim($c[2]) != '' && trim($c[3]) != '' && trim($c[4]) != '' && trim($c[5]) != '' ){

				$qtd = $db->pegaUm("SELECT count(1) FROM sisplan.ptres WHERE exercicio = '".trim($c[0])."' AND ptres = '".trim($c[5])."'");
				if ( $qtd <= 0 )
				{
					$sql = sprintf(
							"insert into sisplan.ptres (   exercicio, prgcod, acacod, unicod, loccod, ptres )
								values ( '%s', '%s', '%s',
									 '%s', '%s', '%s'
								 ) ",
									trim($c[0]), trim($c[1]), trim($c[2]),
									trim($c[3]), trim($c[4]), trim($c[5]), 'A'
						);
					$db->executar( $sql );
				}
				else
				{
					$sql = sprintf(
							"UPDATE
								sisplan.ptres
							SET
								prgcod = '%s',
								acacod = '%s',
								unicod = '%s',
								loccod = '%s'
							WHERE
								ptres = '%s'
							AND exercicio = '%s'",
									trim($c[1]), trim($c[2]), trim($c[3]),
									trim($c[4]), trim($c[5]), trim($c[0])
						);
					$db->executar( $sql );
				}
				$contador = $contador + 1;
				echo $contador.'<br>';
			} else {
				$db->rollback();
				$db->insucesso("Existem campos vazios no arquivo!", '', $_REQUEST['modulo'] );
				break;
			}
		 }

		$db->commit();
		$db->sucesso( $_REQUEST['modulo'] );

		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'inserir':


		$sql = "INSERT INTO sisplan.ptres
					( exercicio, prgcod, acacod, unicod, loccod, ptres )
				VALUES
					('".$_REQUEST['exercicio']."', '".$_REQUEST['prgcod']."', '".$_REQUEST['acacod']."', '".$_REQUEST['unicod']."', '".$_REQUEST['loccod']."', '".$_REQUEST['ptres']."' ) ";

		$db->executar( $sql );

		$db->commit();

		$db->sucesso( $modulo );

		$com = "I";

	case 'excluir':
		$sql = "UPDATE
					sisplan.ptres
				SET
					ptrstatus = 'I'
				WHERE
					ptrid = '".$_REQUEST['ptrid']."'
				";
		try
		{
			$db->executar( $sql );
			$db->commit();
		}
		catch ( Exception $e )
		{
			$db->rollback();
		}
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'selalterar':
		$sql = "SELECT
					exercicio, prgcod, acacod, unicod, loccod, ptres
				FROM
					sisplan.ptres
				WHERE
					ptrid = '".$_REQUEST['ptrid']."'
				";

		$dados = $db->pegaLinha( $sql );

		$prgcod 		= $dados['prgcod'];
		$acacod 		= $dados['acacod'];
		$unicod			= $dados['unicod'];
		$loccod			= $dados['loccod'];
		$ptres			= $dados['ptres'];

		$com = "A";

	break;


	case 'alterar':

		$sql = "UPDATE
					sisplan.ptres
				SET
					prgcod = '".$_REQUEST['prgcod']."',
					acacod = '".$_REQUEST['acacod']."',
					unicod = '".$_REQUEST['unicod']."',
					loccod = '".$_REQUEST['loccod']."',
					ptres  = '".$_REQUEST['ptres']."'
				WHERE
					ptrid = '".$_REQUEST['ptrid']."'
				";

		$db->executar( $sql );

		if ( $_REQUEST['acao_dept'][0] != "" )
		{
			$sql = "DELETE FROM sisplan.depacao WHERE depid = ".$_REQUEST['depid'];
			$db->executar( $sql );

			foreach ( $_REQUEST['acao_dept'] as $acaid )
			{
				$sql = "INSERT INTO sisplan.depacao(
				            depid, acaid)
				    	VALUES (". $_REQUEST['depid'] .", ". $acaid .")";
				$db->executar( $sql );
			}
		}


		$db->commit();

		$db->sucesso( $modulo );

		$com = "A";

	default:

		$com = "I";

		break;

}

$permissao = true;//atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
//título da página
monta_titulo('Inclusão de dados PTRES','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="javascript" type="text/javascript">
	function cadastrar_anexo(){
		if ( validar_formulario_anexo() ) {
			document.anexo.submit();
		}
	}

	function validar_formulario_anexo(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		if ( document.anexo.arquivo.value == '' ) {
			mensagem += '\nArquivo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function excluirAnexo( anexo ){
		if ( confirm( 'Deseja excluir o instrumento?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&evento=excluir_anexo&aneid='+ anexo;
		}
	}

	function cadastrar_versao( formulario ){
		if ( formulario.arquivo.value == '' ) {
			return;
		}
		formulario.submit();
	}

	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}

	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}

	function trim( value ){
		return ltrim(rtrim(value));
	}
</script>
<script language="javascript" type="text/javascript">
	var IE = document.all ? true : false;
	function exibirOcultarIncluirVersao( identificador ){
		var anexo = document.getElementById( 'anexo_' + identificador );
		if ( anexo.style.display == "none" ) {
			if( !IE ) {
				anexo.style.display = "table-row";
			} else {
				anexo.style.display = "block";
			}
		} else {
			anexo.style.display = "none";
		}
		var historico = document.getElementById( 'historico_' + identificador );
		if ( historico.style.display != "none" ) {
			historico.style.display = "none";
		}
	}

	function exibirOcultarHistoricoVersao( identificador ){
		var historico = document.getElementById( 'historico_' + identificador );
		if ( historico.style.display == "none" ) {
			if( !IE ) {
				historico.style.display = "table-row";
			} else {
				historico.style.display = "block";
			}
		} else {
			historico.style.display = "none";
		}
		var anexo = document.getElementById( 'anexo_' + identificador );
		if ( anexo.style.display != "none" ) {
			anexo.style.display = "none";
		}
	}

    function validar_cadastro( cod )
    {

		if (!validaBranco(document.formulario.exercicio, 'Exercício'))
			return;

		if (!validaBranco(document.formulario.prgcod, 'Programa'))
			return;

		if (!validaBranco(document.formulario.acacod, 'Ação'))
			return;

		if (!validaBranco(document.formulario.unicod, 'Unidade'))
			return;

		if (!validaBranco(document.formulario.loccod, 'Localizador'))
			return;

		if (!validaBranco(document.formulario.ptres, 'PTRES'))
			return;


	   	if (cod == 'I')
			document.formulario.evento.value = 'inserir';
		else
			document.formulario.evento.value = 'alterar';

   	   	document.formulario.submit();
     }

	function excluir_item_tela( cod ){
		if ( confirm( 'Deseja excluir o item?' ) ) {
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&ptrid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&ptrid='+ cod;
	}


</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<?php if( $_REQUEST['mensagem'] ) echo $_REQUEST['mensagem']; ?>
			<!-- NOVO ANEXO -->
			<?php if( $permissao ): ?>
				<form method="POST" name="formulario">
					<input type='hidden' name="modulo" value="<?=$modulo?>">
					<input type='hidden' name='acao' value="<?=$_REQUEST['acao']?>">
					<input type='hidden' name='depid' value="<?=$_REQUEST['depid']?>">
					<input type="hidden" name="evento" value="cadastrar_anexo"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Cadastrar dados manualmente</td>
						</tr>

						<tr>
							<td align='right'  class="SubTituloDireita">Exercício:</td>
							<td><?=campo_texto('exercicio','S','S','',18,4,'',''); ?></td>
						</tr>
						<tr>
							<td align='right'  class="SubTituloDireita">Programa:</td>
							<td><?=campo_texto('prgcod','S','S','',18,4,'',''); ?></td>
						</tr>
						<tr>
							<td align='right'  class="SubTituloDireita">Ação:</td>
							<td><?=campo_texto('acacod','S','S','',18,4,'',''); ?></td>
						</tr>
						<tr>
							<td align='right'  class="SubTituloDireita">Unidade:</td>
							<td><?=campo_texto('unicod','S','S','',18,5,'',''); ?></td>
						</tr>
						<tr>
							<td align='right'  class="SubTituloDireita">Localizador:</td>
							<td><?=campo_texto('loccod','S','S','',18,4,'',''); ?></td>
						</tr>
						<tr>
							<td align='right'  class="SubTituloDireita">PTRES:</td>
							<td><?=campo_texto('ptres','S','S','',18,6,'',''); ?></td>
						</tr>
						<tr bgcolor="#CCCCCC">
							<td></td>
							<td>
							<input type="button" name="btgravar" value="Gravar" onclick="validar_cadastro('<?=$com?>')">
							</td>
						</tr>
						</form>

						<form method="post" name="anexo" enctype="multipart/form-data">
							<input type="hidden" name="evento" value="cadastrar_anexo"/>

						<tr>
							<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Importar dados PTRES</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
							<td>
								<input type="file" name="arquivo"/>
								<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
							</td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_anexo();"/></td>
						</tr>
						</form>
					</table>
			<?php endif; ?>
</table>
<!-- LISTA -->
<?php

unset( $sql );

$cabecalho = array( 'Ações', 'Ano', 'Funcional', 'PTRES', 'Status' );

$sql = "
SELECT
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(''' || ptrid || ''')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(''' || ptrid || ''')\" >' AS acao,
	exercicio,
	prgcod||'.'||acacod||'.'||unicod||'.'||loccod,
	ptres,
	ptrstatus
FROM
	sisplan.ptres d
ORDER BY
	prgcod ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>
