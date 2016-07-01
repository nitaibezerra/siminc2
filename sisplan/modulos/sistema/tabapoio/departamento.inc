<?

$modulo = $_REQUEST['modulo'] ;

$saida = "";

 
switch( $_REQUEST['evento'] ){
	case 'inserir':
		// fazer inserção na base de dados.
		$depstatus_item = $_REQUEST['depstatus'] ? 'A' : 'I';
		
		$sql = "INSERT INTO sisplan.departamento
					( depsigla, depdsc, depstatus ) 
				VALUES 
					('".$_REQUEST['depsigla']."', '".$_REQUEST['depdsc']."', '".$depstatus_item."' ) RETURNING depid ";

		$return = $db->executar( $sql );
		$obj    = pg_fetch_object( $return );
		
		if ( $_REQUEST['acao_dept'][0] != "" )
		{
		
			$sql = "DELETE FROM sisplan.depacao WHERE acacod IN ('".implode("','", $_REQUEST['acao_dept'])."') AND exercicio = '".$_SESSION['exercicio']."'";
			$db->executar( $sql );
		
			foreach ( $_REQUEST['acao_dept'] as $acacod )
			{
				//$sql = "UPDATE monitora.acao SET depid = ". $obj->depid ." WHERE acaid = '". $acaid ."'";
				$sql = "INSERT INTO sisplan.depacao(
				            depid, acacod, exercicio)
				    	VALUES (". $obj->depid .", '". $acacod ."', '".$_SESSION['exercicio']."')";
				
				$db->executar( $sql );
			}
		}
		
				
		$db->commit();
		
		$db->sucesso( $modulo );

		$com = "I";

	break;

	case 'excluir':
		$sql = "UPDATE 
					sisplan.departamento
				SET 
					depstatus = 'I'
				WHERE 
					depid = '".$_REQUEST['depid']."'
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
					depsigla, depdsc, depstatus
				FROM 
					sisplan.departamento d
				WHERE
					d.depid = '".$_REQUEST['depid']."'
				";

		$dados = $db->pegaLinha( $sql );

		$depsigla 		= $dados['depsigla'];
		$depdsc 		= $dados['depdsc'];
		$depstatus_item = $dados['depstatus'];

		$com = "A";

	break;

	case 'alterar':
		$depstatus_item = $_REQUEST['depstatus'] ? 'A' : 'I';
		
		$sql = "UPDATE 
					sisplan.departamento
				SET 
					depsigla = '".$_REQUEST['depsigla']."',
					depdsc = '".$_REQUEST['depdsc']."',
					depstatus = '" . $depstatus_item . "'
				WHERE 
					depid = '".$_REQUEST['depid']."'
				";

		$db->executar( $sql );
		
		if ( $_REQUEST['acao_dept'][0] != "" )
		{
		
			$sql = "DELETE FROM sisplan.depacao WHERE acacod IN ('".implode("','", $_REQUEST['acao_dept'])."') AND exercicio = '".$_SESSION['exercicio']."'";
			$db->executar( $sql );
		
			$sql = "DELETE FROM sisplan.depacao WHERE depid = ".$_REQUEST['depid']." AND exercicio = '".$_SESSION['exercicio']."'";
			$db->executar( $sql );
			
			foreach ( $_REQUEST['acao_dept'] as $acacod )
			{
				//$sql = "UPDATE monitora.acao SET depid = ". $_REQUEST['depid'] ." WHERE acaid = '". $acaid ."'";
				$sql = "INSERT INTO sisplan.depacao(
				            depid, acacod, exercicio)
				    	VALUES (". $_REQUEST['depid'] .", '". $acacod ."', '".$_SESSION['exercicio']."')";
				$db->executar( $sql );
			}
		}
		

		$db->commit();

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
			<input type='hidden' name='depid' value=<?=$_REQUEST['depid']?>>
			<tr>
				<td align='right'  class="SubTituloDireita">Sigla:</td>
				<td><?=campo_texto('depsigla','S','S','',18,15,'',''); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Descrição:</td>
				<td><?=campo_texto('depdsc','S','S','',60,80,'',''); ?></td>
			</tr>		
			<tr>
				<td align='right'  class="SubTituloDireita">Ação PPA:</td>
				<td>
				<?php
					if( $_REQUEST['depid'] )
					{
						$sql = "SELECT 
							distinct a.acacod as codigo,
							a.acacod || ' - ' || a.acadsc as descricao
						FROM 
							sisplan.depacao da
						INNER JOIN
							monitora.acao a ON da.acacod = a.acacod
						INNER JOIN
							sisplan.departamento d ON da.depid = d.depid
						WHERE
							da.depid = '".$_REQUEST['depid']."'
						AND a.prgano = '". $_SESSION['exercicio']."'
						AND a.unicod = '" . UNIDADE_IPHAN . "'
						AND a.acasnrap = false
						";
						
						$acao_dept = $db->carregar( $sql );
						$acao_dept = $acao_dept ? $acao_dept : array();
					}
					
					$sql_combo = "
						SELECT
							distinct a.acacod as codigo,
							a.acacod || ' - ' || a.acadsc as descricao
						FROM 
							monitora.acao a
						WHERE
							a.prgano = '". $_SESSION['exercicio']."'
						AND a.unicod = '" . UNIDADE_IPHAN . "'
						AND a.acasnrap = false
						ORDER BY
							a.acacod"; 
					combo_popup(
						'acao_dept',
						$sql_combo,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						0,
						array(),
						'',
						'S',
						true,
						false
					);
				?>	
				</td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Status:</td>
				<td>
					<input type="radio" name="depstatus" value="1" id="depstatus_ativo" <?= $depstatus_item != 'I' ? 'checked="checked"' : '' ?>/> <label for="depstatus_ativo">Ativo</label>
					<input type="radio" name="depstatus" value="0" id="depstatus_inativo" <?= $depstatus_item == 'I' ? 'checked="checked"' : '' ?>/> <label for="depstatus_inativo">Inativo</label>
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
			window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&depid='+ cod;
		}
	}


	function alterar_item_tela( cod ){
		window.location = '?modulo=<?=$modulo?>&acao=<?= $_REQUEST['acao'] ?>&evento=selalterar&depid='+ cod;
	}


    function validar_cadastro( cod ) 
    {

		if (!validaBranco(document.formulario.depsigla, 'Sigla')) 
			return;	
			
		if (!validaBranco(document.formulario.depdsc, 'Descrição')) 
			return;	


		var acoes_validas = new Array();
		<?
			$sql_combo = "
			SELECT
				acacod
			FROM 
				sisplan.depacao da
			WHERE
				da.exercicio = '". $_SESSION['exercicio']."'
			AND da.depid IN ( select depid from sisplan.departamento where depstatus = 'A' )";
			if ( $_REQUEST['depid'] )
				$sql_combo .= " AND da.depid <> ".$_REQUEST['depid'];

			$dados_temp = $db->carregar( $sql_combo );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>
					acoes_validas['<?= $dados_temp_valor['acacod'] ?>'] = true;
					<?
				}
			}
		?>
		
		var qtd = document.formulario.acao_dept.options.length, i;
		var msg = '';
		for ( i=0; i<qtd; i++ )
		{
			if ( acoes_validas[document.formulario.acao_dept.options[i].value] )
				msg += ' ' + document.formulario.acao_dept.options[i].value;
		}
		if ( msg != '' )
		{
			if (!confirm( 'A(s) ação(ões)' + msg  + ' está(ão) atribuída(s) em outro(s) departamento(s).\nDeseja continuar e substituir o vínculo existente dessas ações?' ))
				return false;
		}

		selectAllOptions( formulario.acao_dept );


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

$cabecalho = array( 'Ações', 'Sigla', 'Descrição', 'Ano', 'Ação PPA', 'Status' );

$sql = "
SELECT 
	'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Item  \" onclick=\"alterar_item_tela(' || '\'' || d.depid || '\'' || ')\" >&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Item  \" onclick=\"excluir_item_tela(' || '\'' || d.depid || '\'' || ')\" >' AS acao, 
	d.depsigla, 
	d.depdsc, 
	'".$_SESSION['exercicio']."&nbsp;' exercicio,
	ARRAY_TO_STRING(ARRAY(SELECT t1.acacod FROM sisplan.depacao t1 WHERE t1.depid = d.depid AND t1.exercicio = '".$_SESSION['exercicio']."'), ', ')||'&nbsp;' acacod,
	CASE d.depstatus
		WHEN 'A' THEN 'Ativo' 
		ELSE 'Inativo' 
	END AS depstatus
FROM 
	sisplan.departamento d
ORDER BY
	d.depdsc ASC;
		";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );

?>
