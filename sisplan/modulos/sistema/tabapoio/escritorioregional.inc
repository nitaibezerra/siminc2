<?

$modulo = $_REQUEST['modulo'] ;

$saida = "";

 

switch( $_REQUEST['evento'] ){
	case 'inserir':
		// fazer inserção na base de dados.
		$ergstatus_item = $_REQUEST['ergstatus'] ? 'A' : 'I';
		$sql = "INSERT INTO sisplan.escritorioregional
					(uexid, ergsigla, ergdsc, ergstatus ) 
				VALUES 
					(".$_REQUEST['uexid'].", '".$_REQUEST['ergsigla']."', '".$_REQUEST['ergdsc']."', '".$ergstatus_item."' ) ";

		$db->executar( $sql );
		$db->commit();
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'excluir':
		$sql = "UPDATE 
					sisplan.escritorioregional
				SET 
					ergstatus = 'I'
				WHERE 
					ergid = '".$_REQUEST['ergid']."'
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
					uexid, ergsigla, ergdsc, ergstatus
				FROM 
					sisplan.escritorioregional e
				WHERE
					e.ergid = '".$_REQUEST['ergid']."'
				";

		$dados = $db->pegaLinha( $sql );

		$uexid 			= $dados['uexid'];
		$ergsigla 		= $dados['ergsigla'];
		$ergdsc 		= $dados['ergdsc'];
		$ergstatus_item = $dados['ergstatus'];

		$com = "A";

	break;

	case 'alterar':
		$ergstatus_item = $_REQUEST['ergstatus'] ? 'A' : 'I';
		$sql = "UPDATE 
					sisplan.escritorioregional
				SET 
					uexid = '".$_REQUEST['uexid']."',
					ergsigla = '".$_REQUEST['ergsigla']."',
					ergdsc = '".$_REQUEST['ergdsc']."',
					ergstatus = '" . $ergstatus_item . "'
				WHERE 
					ergid = '".$_REQUEST['ergid']."'
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
			<input type='hidden' name='ergid' value=<?=$_REQUEST['ergid']?>>
			<tr>
				<td align='right'  class="SubTituloDireita">Sigla:</td>
				<td><?=campo_texto('ergsigla','S','S','',18,15,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Unidade Executora:</td>
				<td>
				<? 
				$sql = "SELECT * FROM planointerno.unidadeexecutora_uo";
				$db->monta_combo('uexid',$sql,'S','&nbsp','','','','','','uexid');
				echo  obrigatorio(); 
				?>
				</td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Descrição:</td>
				<td><?=campo_texto('ergdsc','S','S','',60,80,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Status:</td>
				<td>
					<input type="radio" name="ergstatus" value="1" id="ergstatus_ativo" <?= $ergstatus_item != 'I' ? 'checked="checked"' : '' ?>/> <label for="ergstatus_ativo">Ativo</label>
					<input type="radio" name="ergstatus" value="0" id="ergstatus_inativo" <?= $ergstatus_item == 'I' ? 'checked="checked"' : '' ?>/> <label for="ergstatus_inativo">Inativo</label>
				</td>
			</tr>

			<tr bgcolor="#CCCCCC">
				<td></td>
				<td>
				<input type="button" name="btgravar" value="Gravar" onclick="validar_cadastro('<?=$com?>')" class="botao">
				</td>
			</tr>
		</form>
	</table>

<script>

	function voltar(){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>';
	}

	function excluir_item_tela( cod ){
		if ( confirm( 'Deseja excluir o item?' ) ) {
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&ergid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&ergid='+ cod;
	}


    function validar_cadastro( cod ) 
    {
		if (!validaBranco(document.formulario.ergsigla, 'Sigla')) 
			return;	
			
		if (!validaBranco(document.formulario.uexid, 'Unidade Executora')) 
			return;	
			
		if (!validaBranco(document.formulario.ergdsc, 'Descrição')) 
			return;	

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

$cabecalho = array( 'Ações', 'Sigla', 'Unidade Executora', 'Descrição', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || e.ergid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || e.ergid || '\'' || ')\" >' AS acao, 
	e.ergsigla, 
	u.uexcod||' - '||u.uexdsc||' - '||u.uexsigla AS unidadeexecutora,
	e.ergdsc, 
	CASE e.ergstatus
		WHEN 'A' THEN 'Ativo' 
		ELSE 'Inativo' 
	END AS ergstatusitem
FROM 
	sisplan.escritorioregional e
	JOIN planointerno.unidadeexecutora u
		ON u.uexid = e.uexid
ORDER BY
	e.ergdsc ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>