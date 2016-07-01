<?

$modulo = $_REQUEST['modulo'] ;


$saida = "";

switch( $_REQUEST['evento'] ){
	case 'inserir':
		// fazer inserção na base de dados.
		$status_item = $_REQUEST['status'] ? 'A' : 'I';
		$sql = "INSERT INTO planointerno.partidopolitico 
					(pposigla, pponumero, ppodsc, ppostatus) 
				VALUES 
					('".$_REQUEST['pposigla']."', '".$_REQUEST['pponumero']."', '".$_REQUEST['ppodsc']."', '" . $status_item . "') ";

		$db->executar( $sql );
		$db->commit();
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'excluir':
		$sql = sprintf(
			"DELETE FROM 
				planointerno.partidopolitico 
			WHERE 
				ppoid = %d",
			$_REQUEST['ppoid']
		);
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
					ppoid, pposigla, pponumero, ppodsc, ppostatus
				FROM 
					planointerno.partidopolitico m
				WHERE
					m.ppoid = ".$_REQUEST['ppoid']."
				";

		$dados = $db->pegaLinha( $sql );

		$pposigla = $dados['pposigla'];
		$pponumero = $dados['pponumero'];
		$ppodsc = $dados['ppodsc'];
		$status_item = $dados['ppostatus'];

		$com = "A";

	break;

	case 'alterar':
		$status_item = $_REQUEST['status'] ? 'A' : 'I';
		$sql = "UPDATE 
					planointerno.partidopolitico 
				SET 
					pposigla = '".$_REQUEST['pposigla']."',
					pponumero = '".$_REQUEST['pponumero']."',
					ppodsc = '".$_REQUEST['ppodsc']."',
					ppostatus = '" . $status_item . "'
				WHERE 
					ppoid = ".$_REQUEST['ppoid']."
				";

		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $modulo );

		$com = "A";

	break;

	default:
	
		$com = "I";

	break;
}


include APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?
$parametros = array('','','');
$db->cria_aba($abacod_tela,$url,$parametros);

//título da página
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<form method="POST"  name="formulario">
			<input type='hidden' name="modulo" value="<?=$modulo?>">
			<input type='hidden' name='evento' value=0>
			<input type='hidden' name='acao' value=<?=$_REQUEST['acao']?>>

			<tr>
				<td align='right'  class="SubTituloDireita">Sigla:</td>
				<td><?=campo_texto('pposigla','S','S','',18,15,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">N&uacute;mero:</td>
				<td><?=campo_texto('pponumero','S','S','',4,2,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Descrição:</td>
				<td><?=campo_texto('ppodsc','S','S','',60,80,'',''); ?></td>
			</tr>

			<tr>
				<td align='right'  class="SubTituloDireita">Status:</td>
				<td>
					<input type="radio" name="status" value="1" id="status_ativo" <?= $status_item != 'I' ? 'checked="checked"' : '' ?>/> <label for="status_ativo">Ativo</label>
					<input type="radio" name="status" value="0" id="status_inativo" <?= $status_item == 'I' ? 'checked="checked"' : '' ?>/> <label for="status_inativo">Inativo</label>
				</td>
			</tr>

			<tr bgcolor="#CCCCCC">
				<td></td>
				<td>
				<input type="button" name="btgravar" value="Gravar" onclick="validar_cadastro('<?=$com?>')" class="botao">
				<?php 
				if( $com == "A" ):
				?>
				<input type="button" name="btvoltar" value="Voltar" onclick="voltar()" class="botao">
				<?php endif; ?>
				</td>
			</tr>
		</form>
	</table>

<script>

	function voltar(){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>';
	}

	function excluir_item_tela( cod ){
		if ( confirm( 'Deseja excluir o Item?' ) ) {
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&ppoid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&ppoid='+ cod;
	}


    function validar_cadastro( cod ) {    	
		if (!validaBranco(document.formulario.ppodsc, 'Descrição')) return;	

	   	if (cod == 'I') 
			document.formulario.evento.value = 'inserir'; 
		else 
			document.formulario.evento.value = 'alterar';

   	   	document.formulario.submit();
     }   		    

</script>
<!-- LISTA -->
<?php

unset( $sql );

$cabecalho = array( 'Ações', 'Sigla', 'N&uacute;mero', 'Descrição', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || m.ppoid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || m.ppoid || '\'' || ')\" >' AS acao, m.pposigla, m.pponumero, m.ppodsc, CASE m.ppostatus WHEN 'A' THEN 'Ativo' ELSE 'Inativo' END AS statusitem
FROM 
	planointerno.partidopolitico m
ORDER BY
	m.ppodsc ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>