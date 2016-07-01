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

		$verid = 999;
		
		// grava o arquivo
		$caminho = '/tmp/'. $verid;
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}

		$contador=1;
		foreach (CsvToArray::open($caminho) as $c)
		 {

			if ( trim($c[0]) != '' && trim($c[1]) != '' && trim($c[2]) != '' && trim($c[3]) != '' && trim($c[4]) != '' && trim($c[5]) != '' )
			{
				$qtd = $db->pegaUm("SELECT count(1) FROM public.naturezadespesa WHERE ndpcod = '".str_pad(trim($c[5]),8, '0', STR_PAD_RIGHT)."'");
				
				if ( (int)$qtd == 1 )
				{
					$sql = sprintf(
						"insert into sisplan.financeiro (   exercicio, prgcod, acacod, unicod, loccod, natureza ) 
							values ( '%s', '%s', '%s',
							 	 '%s', '%s', '%s'
							 ) ",
								trim($c[0]), trim($c[1]), trim($c[2]), 
								trim($c[3]), trim($c[4]), trim($c[5]), 'A'
					);
					$db->executar( $sql ); 
				
					$contador = $contador + 1; 

					echo $contador.'<br>';
				}
			} else {
				$db->rollback();
				$db->insucesso("Existem campos vazios no arquivo!", '', $_REQUEST['modulo'] );
				//redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
				break;
			}
		 }

					$db->commit();
					$db->sucesso( $_REQUEST['modulo'] );

		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'inserir':

		$qtd = $db->pegaUm("SELECT count(1) FROM public.naturezadespesa WHERE ndpcod = '".str_pad($_REQUEST['natureza'],8, '0', STR_PAD_RIGHT)."'");

		if ( (int)$qtd == 1 )
		{
			$sql = "INSERT INTO sisplan.financeiro
						( exercicio, prgcod, acacod, unicod, loccod, natureza ) 
					VALUES 
						('".$_REQUEST['ano']."', '".$_REQUEST['prgcod']."', '".$_REQUEST['acacod']."', '".$_REQUEST['unicod']."', '".$_REQUEST['loccod']."', '".$_REQUEST['natureza']."' ) ";

			$db->executar( $sql ); 		
					
			$db->commit();
			
			$db->sucesso( $modulo );

			$com = "I";
		}
		else
		{
			$script = '
			alert("A Natureza de Despesa informada não existe no sistema. Contate o administrador do sistema.");
			window.location = "?modulo=sistema/tabapoio/importafinanceiro&acao=A"
			';
			$com = "I";
		}
	break;
	case 'excluir':
		$sql = "UPDATE 
					sisplan.financeiro
				SET 
					finstatus = 'I'
				WHERE 
					finid = '".$_REQUEST['finid']."'
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
					exercicio, prgcod, acacod, unicod, loccod, natureza
				FROM 
					sisplan.financeiro 
				WHERE
					finid = '".$_REQUEST['finid']."'
				";

		$dados = $db->pegaLinha( $sql );

		$prgcod 		= $dados['prgcod'];
		$acacod 		= $dados['acacod'];
		$unicod			= $dados['unicod'];
		$loccod			= $dados['loccod'];
		$natureza		= $dados['natureza'];
		$ano			= $dados['exercicio'];

		$com = "A";

	break;


	case 'alterar':
		
		$sql = "UPDATE 
					sisplan.financeiro
				SET 
					prgcod = '".$_REQUEST['prgcod']."',
					acacod = '".$_REQUEST['acacod']."',
					unicod = '".$_REQUEST['unicod']."',
					loccod = '".$_REQUEST['loccod']."',
					natureza = '".$_REQUEST['natureza']."',
					exercicio = '".$_REQUEST['ano']."'
				WHERE 
					finid = '".$_REQUEST['finid']."'
				";

		$db->executar( $sql );
		
		if ( $_REQUEST['acao_dept'][0] != "" )
		{
			$sql = "DELETE FROM sisplan.depacao WHERE depid = ".$_REQUEST['depid'];
			$db->executar( $sql );
			
			foreach ( $_REQUEST['acao_dept'] as $acaid )
			{
				//$sql = "UPDATE monitora.acao SET depid = ". $_REQUEST['depid'] ." WHERE acaid = '". $acaid ."'";
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
monta_titulo('Inclusão de dados Financeiros','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<script language="javascript" type="text/javascript">
	
	<?=$script?>
	
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

		if (!validaBranco(document.formulario.ano, 'Exercício')) 
			return;	
			
		if (!validaBranco(document.formulario.prgcod, 'Programa')) 
			return;	
			
		if (!validaBranco(document.formulario.acacod, 'Ação')) 
			return;	

		if (!validaBranco(document.formulario.unicod, 'Unidade')) 
			return;	

		if (!validaBranco(document.formulario.loccod, 'Localizador')) 
			return;	

		if (!validaBranco(document.formulario.natureza, 'Natureza de Despesa')) 
			return;	

			
	   	if (cod == 'I') 
			document.formulario.evento.value = 'inserir'; 
		else 
			document.formulario.evento.value = 'alterar';

   	   	document.formulario.submit();
     }   		    

	function excluir_item_tela( cod ){
		if ( confirm( 'Deseja excluir o item?' ) ) {
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&finid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&finid='+ cod;
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
							<?
							$ano = $ano ? $ano : $_SESSION['exercicio'];
							?>
							<td><?=campo_texto('ano','S','S','',18,4,'',''); ?></td>
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
							<td align='right'  class="SubTituloDireita">Natureza:</td>
							<td><?=campo_texto('natureza','S','S','',30,6,'',''); ?></td>
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
							<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Importar dados Financeiros</td>
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

$cabecalho = array( 'Ações', 'Ano', 'Funcional', 'Natureza', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || finid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || finid || '\'' || ')\" >' AS acao, 
	exercicio, 
	prgcod||'.'||acacod||'.'||unicod||'.'||loccod, 
	natureza,
	finstatus
FROM 
	sisplan.financeiro d
ORDER BY
	prgcod ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>
