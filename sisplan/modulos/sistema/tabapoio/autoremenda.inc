<?

$modulo = $_REQUEST['modulo'] ;


$saida = "";

switch( $_REQUEST['evento'] ){
	case 'inserir':
		// fazer inserção na base de dados.
		$status_item = $_REQUEST['status'] ? 'A' : 'I';
		$sql = "INSERT INTO planointerno.autoremenda 
					(auedsc,ppoid,tauid,auestatus) 
				VALUES 
					('".$_REQUEST['auedsc']."', '".$_REQUEST['ppoid']."', '".$_REQUEST['tauid']."', '" . $status_item . "') ";

		$db->executar( $sql );
		$db->commit();
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'excluir':
		$sql = sprintf(
			"DELETE FROM 
				planointerno.autoremenda 
			WHERE 
				aueid = %d",
			$_REQUEST['aueid']
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
					aueid, ppoid, tauid, auedsc, auestatus
				FROM 
					planointerno.autoremenda m
				WHERE
					m.aueid = ".$_REQUEST['aueid']."
				";

		$dados = $db->pegaLinha( $sql );

		$auedsc = $dados['auedsc'];
		$ppoid = $dados['ppoid'];
		$tauid = $dados['tauid'];
		$status_item = $dados['auestatus'];

		$com = "A";

	break;

	case 'alterar':
		$status_item = $_REQUEST['status'] ? 'A' : 'I';
		$sql = "UPDATE 
					planointerno.autoremenda 
				SET 
					auedsc = '".$_REQUEST['auedsc']."',
					ppoid = '".$_REQUEST['ppoid']."',
					tauid = '".$_REQUEST['tauid']."',
					auestatus = '" . $status_item . "'
				WHERE 
					aueid = ".$_REQUEST['aueid']."
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
				<td align='right'  class="SubTituloDireita">Tipo de autor:</td>
				<td><?php
					$sql = "SELECT tauid AS codigo, taudsc AS descricao FROM planointerno.tipoautor WHERE taustatus = 'A' ORDER BY 2";
					$db->monta_combo("tauid",$sql,'S','&nbsp','','','','','','tauid');
					echo obrigatorio(); 
				?></td>
			</tr>

			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Partido:</td>
				<td><?php
					$sql = "SELECT ppoid AS codigo, pposigla || ' - ' || ppodsc AS descricao FROM planointerno.partidopolitico WHERE ppostatus = 'A' ORDER BY 2";
					$db->monta_combo("ppoid",$sql,'S','&nbsp','','','','','','ppoid');
					//echo  obrigatorio(); 
				?></td>
			</tr>

			<tr>
				<td align='right'  class="SubTituloDireita">Descrição:</td>
				<td><?=campo_texto('auedsc','S','S','',60,80,'',''); ?></td>
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
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&aueid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&aueid='+ cod;
	}


    function validar_cadastro( cod ) {    	
		if (!validaBranco(document.formulario.tauid, 'Tipo de Autor')) return;
		if (!validaBranco(document.formulario.ppoid, 'Partido Pol&iacute;tico')) return;
		if (!validaBranco(document.formulario.auedsc, 'Nome')) return;	


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

$cabecalho = array( 'Ações', 'Tipo', 'Partido', 'Nome', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || m.aueid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || m.aueid || '\'' || ')\" >' AS acao, t.taudsc, p.pposigla || ' - ' || p.ppodsc, m.auedsc, CASE m.auestatus WHEN 'A' THEN 'Ativo' ELSE 'Inativo' END AS statusitem
FROM 
	planointerno.autoremenda m
JOIN planointerno.partidopolitico p
ON p.ppoid = m.ppoid
JOIN planointerno.tipoautor t
ON t.tauid = m.tauid
ORDER BY
	m.auedsc ASC
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>