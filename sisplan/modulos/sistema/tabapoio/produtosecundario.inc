<?

$modulo = $_REQUEST['modulo'] ;


$saida = "";

switch( $_REQUEST['evento'] ){
	case 'inserir':
		// fazer inserção na base de dados.
		$status_item = $_REQUEST['status'] ? 'A' : 'I';
		$sql = "INSERT INTO sisplan.produtosecundario 
					(pdudsc,pdustatus) 
				VALUES 
					('".$_REQUEST['pdudsc']."', '" . $status_item . "') ";

		$db->executar( $sql );
		$db->commit();
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'excluir':
		$sql = sprintf(
			"DELETE FROM 
				sisplan.produtosecundario 
			WHERE 
				pduid = %d",
			$_REQUEST['pduid']
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
					pduid, pdudsc, pdustatus
				FROM 
					sisplan.produtosecundario m
				WHERE
					m.pduid = ".$_REQUEST['pduid']."
				";

		$dados = $db->pegaLinha( $sql );

		$pdudsc = $dados['pdudsc'];
		$status_item = $dados['pdustatus'];

		$com = "A";

	break;

	case 'alterar':
		$status_item = $_REQUEST['status'] ? 'A' : 'I';
		$sql = "UPDATE 
					sisplan.produtosecundario 
				SET 
					pdudsc = '".$_REQUEST['pdudsc']."',
					pdustatus = '" . $status_item . "'
				WHERE 
					pduid = ".$_REQUEST['pduid']."
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
				<td align='right'  class="SubTituloDireita">Descrição:</td>
				<td><?=campo_texto('pdudsc','S','S','',60,80,'',''); ?></td>
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
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&pduid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&pduid='+ cod;
	}


    function validar_cadastro( cod ) {    	
		if (!validaBranco(document.formulario.pdudsc, 'Descrição')) return;	

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

$cabecalho = array( 'Ações', 'Descrição', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || m.pduid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || m.pduid || '\'' || ')\" >' AS acao, m.pdudsc, CASE m.pdustatus WHEN 'A' THEN 'Ativo' ELSE 'Inativo' END AS statusitem
FROM 
	sisplan.produtosecundario m
ORDER BY
	m.pdudsc ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>