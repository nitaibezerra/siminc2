
<?php
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
// Editar o vinculo
if( $_REQUEST['requisicao'] == "editar" )
{
	// Pega o tpsid
	$tpsidQuery = $_REQUEST['tpsid'];
	
	$sqlCarregar = "
		SELECT
			frmid,
			ptsid 
		FROM 
			par.propostatiposubacao
		WHERE
			tpsid = {$tpsidQuery}
		AND
			ptsstatus = 'A'	
	";

	
	$editVincularTipoForma = $db->carregar($sqlCarregar);
	
	$arrEditFormaExecucao = array();
	
	$strJaMarcados = '';
	if( count($editVincularTipoForma) && is_array($editVincularTipoForma) )
	{
		foreach( $editVincularTipoForma as $k => $formaExecucao )
		{
			$arrEditFormaExecucao[$formaExecucao['frmid']] = $formaExecucao['ptsid'];
			$strJaMarcados .=  "{$formaExecucao['frmid']},";		
		}
	}
	
	$tpsid = $_REQUEST['tpsid'];
	$editar = '- Editar';
	
// Esta validação é feita porque a tabela que agora é a associativa( par.propostaitemtiposubacao ) antes era usada diferente e tem relacionamentos que não podem ser excluidos 
$sqlNaoExcluir = " 
	SELECT 
		DISTINCT (ptsid)
	FROM
		(
			-- VERIFICA PROPOSTAITEMTIPOSUBACAO
			SELECT 
				DISTINCT(ptsid) 
			FROM 
				par.propostaitemtiposubacao
			WHERE 
				ptsid IN
				(SELECT
					DISTINCT(pts.ptsid) 
				 FROM 
					par.propostatiposubacao pts
				 WHERE
					tpsid = {$tpsidQuery} )
			-- JUNTO COM
			UNION ALL
			-- VERIFICA PROPOSTASUBACAO
			SELECT 
				DISTINCT(ptsid) 
			FROM 
				par.propostasubacao pts
			
			WHERE 
				ptsid IN
				(SELECT
					DISTINCT(pts.ptsid) 
				 FROM 
					par.propostatiposubacao pts
				 WHERE
					tpsid = {$tpsidQuery} )
			-- JUNTO COM
			UNION ALL
			-- VERIFICA PROPOSTAGRUPOTIPOSUBACAO
			SELECT
				DISTINCT(ptsid) 
			FROM  
				par.propostagrupotiposubacao pts
			
			WHERE
			ptsid IN
				(SELECT
					DISTINCT(pts.ptsid) 
				 FROM 
					par.propostatiposubacao pts
				 WHERE
					tpsid = {$tpsidQuery} )
			-- JUNTO COM
			UNION ALL
			-- VERIFICA SUBACAO
			SELECT 
				DISTINCT(ptsid) 
			FROM 
				par.subacao pts
			
			WHERE
			ptsid IN
				(SELECT
					DISTINCT(pts.ptsid) 
				 FROM 
					par.propostatiposubacao pts
				 WHERE
					tpsid = {$tpsidQuery} )
		) 
		AS foo";
	
	// Carrega os ids que não podem ser excluidos
	$resultNaoExcluir = $db->carregar($sqlNaoExcluir);
	
	
	if( count($resultNaoExcluir) && is_array($resultNaoExcluir) )
	{
		foreach( $resultNaoExcluir as $k => $reg )
		{
			$arrNaoExcluir[] = $reg['ptsid'];		
		}
	}
	
}
else 
{
	$editar = '';
}


// Salvar as modificações
if($_POST["submeter"] == 'salvar' ) 
{
	
	 //
	$arrFormasExec = array();
	
	foreach ( $_POST as $k => $post )
	{	
		if( strstr( $k , "formaExecucao" ) )
		{
			$arrFormasExec[] = $post;
		}
	}
	
	// Caso tivesse antes e não tem mais, deleta, caso continue, não faz nada, caso seja novo adiciona.
	

	$jaMarcados = $_POST['strJaMarcados'];
	$jaMarcados = substr_replace($jaMarcados, '', -1);
	$arrJaMarcados = explode(',', $jaMarcados);
	
/*	
 * 
 * se tiver antes e não tem mais add no $arrDeletar
 * se não tinha antes e tem agora add no $arrAdicionar
 * se tinha e continua ignora.
 * */
 
	//print_r($arrFormasExec);
	// Verifica qual tem que deletar
	foreach( $arrJaMarcados as $k => $itemMarcado )
	{
		if( ! (in_array( $itemMarcado , $arrFormasExec ) ) )
		{
			$arrDeletar[] = $itemMarcado;
		}
	}
	
	foreach($arrFormasExec as $k => $marcadoAgora )
	{
		if( ! (in_array( $marcadoAgora , $arrJaMarcados ) ))
		{
			$arrAdicionar[] = $marcadoAgora;
		} 
	}

	$tipoSubacao = $_POST['tpsid'];
	
	$sqlInsert = 
	"
		INSERT into par.propostatiposubacao
			( ptsstatus, tpsid, frmid )
		VALUES
	";
	
	if(count($arrAdicionar))
	{
		foreach( $arrAdicionar as $k => $frmExec )
		{
			if( $k == 0 )
			{
				$sqlInsert .= "		( 'A', {$tipoSubacao}, {$frmExec})";	
			}
			else
			{
				$sqlInsert .= "		,( 'A', {$tipoSubacao}, {$frmExec})";
			}
		}
		$db->executar($sqlInsert);
	}
	
	if(count($arrDeletar))
	{
		foreach( $arrDeletar as $k => $frmExec )
		{
			$sqlDelete = "
				DELETE FROM 
					par.propostatiposubacao
				WHERE
					tpsid =	{$tipoSubacao}
				AND
					frmid = {$frmExec}
			";
			$db->executar($sqlDelete);
		}
	}
	
	$db->commit();

	echo "<script>alert('Relacionamento salvo com sucesso');</script>";
	echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/vincularTipoForma&acao=A'</script>";
	
} 

$titulo = 'Vincular Tipo de Subação a Forma de Execução' . $editar;
monta_titulo($titulo , '');
?>
<form id="formulario" method="post" action="">
<input type="hidden" id="tpsid" name="tpsid" value="<?=$tpsid?>" />
<input type="hidden" id="strJaMarcados" name="strJaMarcados" value="<?=$strJaMarcados ?>" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<input type="hidden" id="submeter" name="submeter" value="" />

<?php 
if($tpsid)
{?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		
		
		<tr>
		
			<td class="subtituloEsquerda"  colspan="2" >Tipo Subação</td>
		</tr>
		<tr>
			<td style="width: 5%" ></td>
			<td>
	<?php 	
					$sqlTipoSubacao = "
					 	SELECT
		 			 		tpsid, 
		 			 		tpsdescricao
		 			 	FROM
		 			 		par.tiposubacao 
		 				WHERE
		 					tpsid = {$tpsid}
		 			";
		 			
				 	$arrTipoSubacao = $db->carregar($sqlTipoSubacao); 	
					
				 	$tpsdescricao =  $arrTipoSubacao[0]['tpsdescricao']; 
				 	
		?>			 			
						 	<b><?= $tpsdescricao ?></b>
				
				</td>
				
			</tr>
		
		
		<tr>
			<td class="subtituloEsquerda" colspan="2"  >Forma de Execução</td>
		</tr>
		<tr>
			<td style="width: 5%" ></td>
			<td>
	<?php 	
					
					 $sqlFormaExecucao = "
					 	SELECT 
		 			 		frmid, 
		 			 		frmdsc
		 				FROM 
		 					par.formaexecucao
		 				WHERE
		 					frmstatus = 'A'
		 			";
		 			
				 	$arrFormaExecucao = $db->carregar($sqlFormaExecucao); 	
					
				 	if(count($arrFormaExecucao))
				 	{
				 		foreach($arrFormaExecucao as $k => $v)
				 		{
				 			
				 			if( $editar != '')
							{
								$checkedCheck = '';
								$disabledCheck =  '';
								if( is_array( $arrEditFormaExecucao ) )
								{
									if( array_key_exists( $v['frmid'] , $arrEditFormaExecucao))
									{
										$checkedCheck = 'checked';
										
										if( in_array( $arrEditFormaExecucao[$v['frmid']] , $arrNaoExcluir) )
										{
											$disabledCheck =  'disabled';
											echo "<input type='hidden' name='formaExecucao_{$v['frmid']}' value='{$v['frmid']}' >";
										}
										else
										{
											$disabledCheck =  '';
										}
									}
								}
							}
				 			
	?>			 
					 	<input type="checkbox"  value='<?= $v['frmid'] ?>' <?= $disabledCheck  ?> <?= $checkedCheck ?> name="formaExecucao_<?= $v['frmid'] ?>"> <?= $v['frmdsc'] ?> <Br>		
				 			
	<?php				
				 		}
				
					}
					
				?>	
				</td>
		</tr>
		
		
		<tr>
			<td bgcolor="#c0c0c0" align="center" colspan="2">
			
				<input type="button" id="bt_salvar" value="Salvar" onclick="javascript:salvarObjeto();" />
				&nbsp;
				<input type="button" id="bt_cancel" value="Cancelar" onclick="javascript:cancelar();"/>
				
			</td>
			
		</tr>
	</table>
<?php 
}
?>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Tipos de Subação</b></td>
	</tr>
</table>
<script>
	function salvarObjeto()
	{

		var check = 0;
		var radio = 0;
		$('input:checkbox').each(function() 
		{
		   if( this.checked)
		   {
			   check++;
		   }
		});
					
		if( check == 0 )
		{
			alert('Selecione no mínimo uma "Forma de Execução"');
		}
		else 
		{
			document.getElementById('submeter').value = 'salvar';	
			document.getElementById('formulario').submit();
		}
	}
	function alterarVinculo( tpsid )
	{
		document.getElementById('submeter').value = 'salvar';	
		document.getElementById('formulario').submit();
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/vincularTipoForma&acao=A&tpsid='+tpsid+'&requisicao=editar';
		
	}

	function cancelar(){
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/vincularTipoForma&acao=A';
	}
</script>
<?

	$acao = "'<a href=\"#\" onclick=\"alterarVinculo(' || tps.tpsid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
			||
			CASE WHEN 
				tpsstatus = 'A' then 
					--'<a href=\"#\" onclick=\"excluirVinculo(' || tps.tpsid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
					''
				ELSE
					''
			END	
					";
			

	 $sql = "		
					SELECT 
						DISTINCT({$acao}) as acao,
						tps.tpsdescricao  as tpsdescricao,
						 array_to_string(
						 array	(
								SELECT 
									
									frm1.frmdsc
									
								FROM 
									par.propostatiposubacao pts1  
								JOIN par.formaexecucao frm1 on pts1.frmid = frm1.frmid
								JOIN par.tiposubacao tps1 on tps1.tpsid = pts1.tpsid
								WHERE pts1.tpsid = pts.tpsid
								
							),
						 ';<br>'
						) 
							as formas_execucao
					FROM
						par.tiposubacao tps
				
					LEFT JOIN par.propostatiposubacao pts on tps.tpsid = pts.tpsid
					LEFT JOIN par.formaexecucao frm on pts.frmid = frm.frmid
					
					WHERE 
						tps.tpsstatus = 'A'
							
					";
	 $cabecalho = array("ações", "Tipo Subação", "Formas de Execução");
	 $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center');
?>