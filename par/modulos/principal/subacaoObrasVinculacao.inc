<?php
$sbaid = $_REQUEST['sbaid'];
$ano = $_REQUEST['ano'];

if( $_REQUEST['sbaid'] ){
	
	function salvarObrasVinculadas(){
		global $db;

		$sbaid = $_POST['sbaid'];
		$ano = $_POST['ano'];
		if( $sbaid && $ano ){
			$sql = "DELETE FROM par.subacaoobravinculacao WHERE sbaid = ".$sbaid." AND sovano = ".$ano;
			$db->executar($sql);
			
			$i = 0;
			if( is_array($_POST['obrid']) ){
				foreach($_POST['obrid'] as $obrid){
					
//					$sql = "SELECT preid FROM obr as.obr ainfraestrutura WHERE obrid = $obrid";
					
					$sql = "SELECT preid FROM obras2.obras WHERE obrid = $obrid";
					
					$preid = $db->pegaUm( $sql );
					if( $preid ){
						$d1 = ", preid";
						$d2 = ", ".$preid;
					}
					$sqlInsertObra .= "INSERT INTO par.subacaoobravinculacao (obrid, sbaid, sovano ".$d1.") VALUES (".$obrid.", ".$sbaid.", ".$ano." ".$d2.");";
					$i = 1;
				}
			}

			if( $i == 1 ){
				$db->executar($sqlInsertObra);
			}
			$db->commit();
			echo "<script>
					alert('Operação realizada com sucesso!');
					parent.window.opener.location.href = 'par.php?modulo=principal/subacao&acao=A&sbaid={$_GET['sbaid']}&anoatual={$_GET['ano']}';
					window.close();
				</script>";	
			exit;
		}
	}
			
	if($_POST['requisicao']){
		$_POST['requisicao']();
	}
	
	if($_POST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_POST['requisicaoAjax']();
		die;
	}
	
	
	/** Carrega informações de localização e dados da subação  **/
	$sql = "SELECT
				CASE WHEN iu.itrid = 1 THEN 'E' ELSE 'M' END as esfera,
				CASE WHEN iu.itrid = 1 THEN ' AND ende.estuf = \'' || iu.estuf || '\'' ELSE ' AND ende.muncod = \'' || iu.muncod || '\'' END as local,
				s.ppsid
			FROM par.subacao 	 	 s
			INNER JOIN par.acao 	 			a  ON a.aciid  = s.aciid AND a.acistatus = 'A' 
			INNER JOIN par.pontuacao 			p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
			INNER JOIN par.instrumentounidade 	iu ON iu.inuid = p.inuid
			WHERE s.sbastatus = 'A' AND s.sbaid = $sbaid";
	
	$subacao = $db->pegaLinha($sql);

	?>
	<html>
		<head>
		    <meta http-equiv="Cache-Control" content="no-cache">
		    <meta http-equiv="Pragma" content="no-cache">
		    <meta http-equiv="Connection" content="Keep-Alive">
		    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
		    <title>PAR 2010 - Plano de Metas - Subação</title>
			
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
		    <script type="text/javascript" src="../includes/funcoes.js"></script>
		    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
		<body>
			<script type="text/javascript">
				
				function salvarObraVinculada(){
					$("#form_obrasvinculadas").submit();
				}
				
			</script>
			<form name="form_obrasvinculadas" id="form_obrasvinculadas" method="post" action="" >
				<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
				<input type="hidden" name="requisicao" value="salvarObrasVinculadas" />
				<input type="hidden" name="ano" value="<?php echo $_REQUEST['ano']; ?>" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: #cccccc;">
						<td align="center" colspan="2"><strong>Vincular / Desvincular Obras</strong></td>
					</tr>
					<tr>
						<td align="left" colspan="2">
							<?php 
							
//							$sql = "	SELECT DISTINCT
//											       '<input type=\"checkbox\" name=\"obrid[]\" id=\"obrid[]\" value=\"'||obr.obrid||'\" ' || CASE WHEN sov.obrid IS NOT NULL THEN 'checked' ELSE '' END || '>' as acao,
//													 obr.obrid,
//											       obr.obrdesc as obrdesc , 
//													TO_CHAR(obr.obsdtinclusao,'YYYY') as ano 
//											FROM 
//												obr as.o brainfraestrutura obr
//											INNER JOIN entidade.endereco ende ON ende.endid = obr.endid
//											LEFT JOIN obras.preobra 	 pre  ON pre.preid = obr.preid AND pre.prestatus = 'A'
//											LEFT JOIN par.subacaoobravinculacao sov ON sov.obrid = obr.obrid AND sov.sbaid = {$_REQUEST['sbaid']} AND sov.sovano = {$_REQUEST['ano']}
//											WHERE
//												obr.obsstatus = 'A'
//												AND obr.obrtipoesfera = '{$subacao['esfera']}'
//												{$subacao['local']}
//											";

							if($_SESSION['par']['inuid'] == 1){ // se DF
								$subacao['esfera'] = "M','E";
							}	
							
							$where = '';
							if( in_array($subacao['ppsid'], array( 924, 925 )) || in_array($subacao['ppsid'], array( 906, 914 )) ){ #tipo A e B
								$where = ' and obr.tpoid in (16, 9, 104) ';
							}elseif( in_array($subacao['ppsid'], array( 913, 904 )) ){ #tipo C
								$where .= ' and obr.tpoid in (10, 105) ';
							}
							
							$sql = "SELECT DISTINCT
								       '<input type=\"checkbox\" name=\"obrid[]\" id=\"obrid[]\" value=\"'||obr.obrid||'\" ' || CASE WHEN sov.obrid IS NOT NULL THEN 'checked' ELSE '' END || '>' as acao,
										 obr.obrid,
								       obr.obrnome as obrdesc , 
										TO_CHAR(obr.obrdtinclusao,'YYYY') as ano 
									FROM 
										obras2.obras obr
									INNER JOIN obras2.empreendimento emp ON emp.empid = obr.empid
									INNER JOIN entidade.endereco ende ON ende.endid = obr.endid
									LEFT JOIN obras.preobra 	 pre  ON pre.preid = obr.preid AND pre.prestatus = 'A'
									LEFT JOIN par.subacaoobravinculacao sov ON sov.obrid = obr.obrid AND sov.sbaid = '{$_REQUEST['sbaid']}' AND sov.sovano = '{$_REQUEST['ano']}'
									WHERE
										obr.obrstatus = 'A'
										AND emp.empesfera in ( '{$subacao['esfera']}' )
										$where
										{$subacao['local']}";
												// dbg($sql,1);
							?>
							<?php $arrDados = $db->carregar($sql); ?>
							<table id="tbl_item" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr style="background-color: #cccccc;">
									<td align="left" colspan="5">
										<span style="margin-left:100px;color:red" class="bold" >*Obs: Para confirmar as alterações clique no botão "Salvar".</span>
									</td>
								</tr>
								<tr class="SubTituloTabela bold" >
									<td class="bold" >Ação</td>
									<td class="bold" >ID Obra</td>
									<td class="bold" >Obra</td>
									<td class="bold" >Ano</td>
								</tr>
								<?php if( is_array($arrDados) ){ 
										foreach($arrDados as $dado){?> 
											<tr>
												<td align="center"><?=$dado['acao'] ?></td>
												<td align="center"><?=$dado['obrid'] ?></td>
												<td align="left"><?=$dado['obrdesc'] ?></td>
												<td align="left"><?=$dado['ano'] ?></td>
											</tr>
									<?php }
								} else { ?>
									<tr>
										<td align="center" colspan="4"><font color="red">Não existem Obras para este local.</font></td>
									</tr>
								<?php } ?>
							</table>
						</td>
					</tr>
					<tr style="background-color: #cccccc;">
						<td align="left" colspan="3">
							<span style="margin-left:100px;color:red" class="bold" >*Obs: Para confirmar as alterações clique no botão "Salvar".</span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda" >
							<input type="button" value="Salvar" name="btn_salvar" onclick="salvarObraVinculada()" />
							<input type="button" value="Fechar" name="btn_fechar" onclick="window.opener.document.location.reload(); window.close();" />
						</td>
					</tr>
				</table>
			</form>
		</body>
	</html>
<?php } else {
			echo "<script>alert('Subação não encontrada!');window.close();</script>";
		} 
?>