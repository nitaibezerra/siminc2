<?

$modulo = $_REQUEST['modulo'] ;

$saida = "";

switch( $_REQUEST['evento'] ){
	case 'inserir':
		// fazer inserção na base de dados.
		$sunstatus_item = $_REQUEST['sunstatus'] ? 'A' : 'I';
		$sql = "INSERT INTO sisplan.subunidade
					(uexid, suncod, sunsigla, sundsc, sunstatus ) 
				VALUES 
					(".$_REQUEST['uexid'].", '".$_REQUEST['suncod']."', '".$_REQUEST['sunsigla']."', '".$_REQUEST['sundsc']."', '" . $sunstatus_item . "' ) ";

		$db->executar( $sql );
		$db->commit();
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'excluir':
		$sql = sprintf(
			"DELETE FROM 
				sisplan.subunidade 
			WHERE 
				sunid = %d",
			$_REQUEST['sunid']
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
					uexid, suncod, sunsigla, sundsc, sunstatus
				FROM 
					sisplan.subunidade s
				WHERE
					s.sunid = '".$_REQUEST['sunid']."'
				";

		$dados = $db->pegaLinha( $sql );

		$uexid 			= $dados['uexid'];
		$suncod   		= $dados['suncod'];
		$sunsigla 		= $dados['sunsigla'];
		$sundsc 		= $dados['sundsc'];
		$sunstatus_item = $dados['sunstatus'];

		$com = "A";

	break;

	case 'alterar':
		$sunstatus_item = $_REQUEST['sunstatus'] ? 'A' : 'I';
		$sql = "UPDATE 
					sisplan.subunidade
				SET 
					uexid = '".$_REQUEST['uexid']."',
					suncod = '".$_REQUEST['suncod']."',
					sunsigla = '".$_REQUEST['sunsigla']."',
					sundsc = '".$_REQUEST['sundsc']."',
					sunstatus = '" . $sunstatus_item . "'
				WHERE 
					sunid = '".$_REQUEST['sunid']."'
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
			<input type='hidden' name='sunid' value=<?=$_REQUEST['sunid']?>>
			<tr>
				<td align='right'  class="SubTituloDireita">C&oacute;digo:</td>
				<td><?=campo_texto('suncod','S','S','',5,3,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Sigla:</td>
				<td><?=campo_texto('sunsigla','S','S','',18,15,'',''); ?></td>
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
				<td><?=campo_texto('sundsc','S','S','',60,80,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Status:</td>
				<td>
					<input type="radio" name="sunstatus" value="1" id="sunstatus_ativo" <?= $sunstatus_item != 'I' ? 'checked="checked"' : '' ?>/> <label for="sunstatus_ativo">Ativo</label>
					<input type="radio" name="sunstatus" value="0" id="sunstatus_inativo" <?= $sunstatus_item == 'I' ? 'checked="checked"' : '' ?>/> <label for="sunstatus_inativo">Inativo</label>
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
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&uexid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&sunid='+ cod;
	}


    function validar_cadastro( cod ) {    	
		if (!validaBranco(document.formulario.suncod, 'C&oacute;digo')) return;	
		if (!validaBranco(document.formulario.sunsigla, 'Sigla')) return;	
		if (!validaBranco(document.formulario.uexid, 'Unidade Executora')) return;	
		if (!validaBranco(document.formulario.sundsc, 'Descrição')) return;	

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

$cabecalho = array( 'Ações', 'C&oacute;digo', 'Sigla', 'Unidade Executora', 'Descrição', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || s.sunid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || s.sunid || '\'' || ')\" >' AS acao, 
	s.suncod,
	s.sunsigla, 
	u.uexcod||' - '||u.uexdsc||' - '||u.uexsigla AS unidadeexecutora,
	s.sundsc, 
	CASE s.sunstatus
		WHEN 'A' THEN 'Ativo' 
		ELSE 'Inativo' 
	END AS sunstatusitem
FROM 
	sisplan.subunidade s
	JOIN planointerno.unidadeexecutora u
		ON u.uexid = s.uexid
ORDER BY
	s.sundsc ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>