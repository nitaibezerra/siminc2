<?php
if($_REQUEST['requisicao'] == "downloadArquivo"){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
	die;
}

if($_REQUEST['requisicao'] == "excluirArquivo"){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos	= array("gpdid" => $_REQUEST['gpdid']);
	$file   = new FilesSimec("arqgrupo", $campos, "obras");
	$file->setRemoveUpload( $_REQUEST['arqid'] );
	echo "<script>alert('Arquivo excluído com sucesso!');window.location.href = 'obras.php?modulo=principal/supervisao/criarLote&acao=A&gpdid={$_REQUEST['gpdid']}';</script>";
	die;
}

if($_REQUEST['requisicao'] == "inserirArquivoGrupo"){
	if($_FILES['arquivo']['size']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("gpdid" => $_POST['gpdid']);
		$file   = new FilesSimec("arqgrupo", $campos, "obras");
		$file->setUpload($_POST['arqdescricao_grupo'],"arquivo");
		echo "<script>alert('Arquivo anexado com sucesso!');window.location.href = 'obras.php?modulo=principal/supervisao/criarLote&acao=A&gpdid={$_POST['gpdid']}';</script>";
	}else{
		echo "<script>alert('Selecione o Arquivo!');window.location.href = 'obras.php?modulo=principal/supervisao/criarLote&acao=A&gpdid={$_POST['gpdid']}';</script>";
	}
	die;
}

if ($_REQUEST['mostrar'] == 'pendentes') {
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo tabelaObrasChecklistNaoAprovado( $_SESSION["obras"]["gpdid"] );
	die;
	
}elseif($_REQUEST['mostrar'] == 'incompletos'){
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo tabelaObrasChecklistaNaoPreenchido( $_SESSION["obras"]["gpdid"] );
	die;
	
}
?>
<?php 

$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "lista":
		
		if( $_REQUEST["estuf"] ){
			
			$filtro = $_REQUEST["estuf"] ? " AND ede.estuf = '{$_REQUEST["estuf"]}'" : "";
			$filtro .= " AND we.esdid IS NULL AND west.esdid IS NULL AND ( total_exec IS NULL OR total_exec BETWEEN 0 AND 110) ";
			$supervisao->obrListaObrasRepositorio( "", $filtro, "lote" );
			
		}else{
			
			print '<font style="font-size: 8pt; color: #dd0000;">
				       <center>Selecione a UF para visualizar as obras.</center>
				   </font>';
			
		}
		
		die;
		
	break;

	case "listapormunicipio":
		
		if( $_REQUEST["muncod"] ){
			
			$filtro = $_REQUEST["muncod"] ? " AND ede.muncod = '{$_REQUEST["muncod"]}'" : "";
			
			$supervisao->obrListaObrasRepositorio( "", $filtro, "lote" );
			
		}else{
			
				if( $_REQUEST["estuf"] ){
					
					$filtro = $_REQUEST["estuf"] ? " AND ed.estuf = '{$_REQUEST["estuf"]}'" : "";
					
					$supervisao->obrListaObrasRepositorio( "", $filtro, "lote" );
					
				}else{
					
					print '<font style="font-size: 8pt; color: #dd0000;">
						       <center>Selecione a UF para visualizar as obras.</center>
						   </font>';
					
				}
		}
		
		die;
		
	break;
	
	case "salvar":
		$supervisao->obrSalvaGrupoSupervisao( $_REQUEST );
	break;
	
	case "novo":
		$_SESSION["obras"]["gpdid"] = "";
	break;
	
}

switch( $_REQUEST["requisicao2"] ){
		
	case "mostramunicipios":

		if( !empty( $_REQUEST["estuf"] ) ){
			
			$sql = "SELECT
						muncod as codigo,
						mundescricao as descricao
					FROM
						territorios.municipio
					WHERE
						estuf = '{$_REQUEST["estuf"]}'
					ORDER BY
						mundescricao";
			
			$db->monta_combo("muncod", $sql, "S", "Selecione...", "obrListaObrasMunicipio( document.getElementById('estuf').value, this.value );", '', '', '', 'S','muncod');
		
		}else{
			print "Selecione um estado...";
		}
		
		die;
		
	break;
	case "carregarempresa":

		if( !empty( $_REQUEST["estuf"] ) ){
			
			$sql = "SELECT ent.entnome, ea.estuf FROM obras.empresacontratada emp
						inner join entidade.entidade ent on ent.entid = emp.entid
					    inner join obras.empresaufatuacao ea on ea.epcid = emp.epcid
					WHERE
						ea.estuf = '{$_REQUEST["estuf"]}'
					    and emp.epcstatus = 'A'";
			
			$arrDados = $db->pegaLinha( $sql );
			$arrDados['entnome'] = utf8_encode( $arrDados['entnome'] );
			
			echo simec_json_encode($arrDados);
			exit();
		
		}else{
			print "Selecione um estado...";
		}
		
		die;
		
	break;
	
}

if( $_SESSION["obras"]["gpdid"] || $_REQUEST["gpdid"] ){

	$_SESSION["obras"]["gpdid"] = $_REQUEST["gpdid"] ? $_REQUEST["gpdid"] : $_SESSION["obras"]["gpdid"];
	
	$dadosGrupo = $supervisao->obrBuscaDadosGrupo( $_SESSION["obras"]["gpdid"] );
	extract( $dadosGrupo );

	$docid = obrCriarDocumento( $_SESSION["obras"]["gpdid"] );
		
	$esdid = obrPegarEstadoAtual( $_SESSION["obras"]["gpdid"] );

	$obrSupUFLeitura = "N";
	
	$obrSupSoLeitura = $esdid == OBRDISTRIBUIDO ? "" : "N";
	$obrSupDisabled  = $esdid == OBRDISTRIBUIDO ? "" : "disabled='disabled;'";
	
}

$obrSupSoLeitura = !empty( $obrSupSoLeitura ) ? $obrSupSoLeitura : "S";
$obrSupUFLeitura = !empty( $obrSupUFLeitura ) ? $obrSupUFLeitura : "S";

#verificando se os campos estarão ou não desabilitados
if($_SESSION["obras"]["gpdid"]){
	$sql = "select gpdid from obras.ordemservico where gpdid = ".$_SESSION["obras"]["gpdid"]. " and orsstatus = 'A'";
	$gpdid = $db->carregar($sql);
}else{
	$gpdid = false;
}

//if( (possuiPerfil(PERFIL_ADMINISTRADOR) || possuiPerfil(PERFIL_SAA)) && (!$gpdid[0]['gpdid']) ){ era assim
//	#echo "campos habilitados!";
//	$obrSupDisabled = '';
//	$obrSupSoLeitura = 'S';
//	$obrSupUFLeitura = 'S';
//
//}else{
//	#echo "campos desabilitados";
//	$obrSupDisabled = "disabled='disabled'";
//	$obrSupSoLeitura = 'N';
//	$obrSupUFLeitura = 'N';
//
//}
if( possuiPerfil(PERFIL_ADMINISTRADOR) || possuiPerfil(PERFIL_SAA) ){ // agora é assim
	#echo "campos habilitados!";
	$obrSupDisabled = '';
	$obrSupSoLeitura = 'S';
	$obrSupUFLeitura = 'S';

}else{
	#echo "campos desabilitados";
	$obrSupDisabled = "disabled='disabled'";
	$obrSupSoLeitura = 'N';
	$obrSupUFLeitura = 'N';

}
	
/* Recupera a Data de Tramitação do Checklist/Parecer do Grupo.*/
if($_SESSION["obras"]["gpdid"] || $_REQUEST["gpdid"] ){
	
	$sql = "SELECT DISTINCT
					to_char(MAX(wh.htddata), 'DD / MM / YYYY') AS datramitacao
				FROM
					obras.grupodistribuicao gd
				INNER JOIN
					workflow.documento wd ON wd.docid = gd.docid
				INNER JOIN
					workflow.historicodocumento wh ON wh.docid = gd.docid
				INNER JOIN
					workflow.estadodocumento we ON we.esdid = wd.esdid
				WHERE
					gpdstatus = 'A' 
					AND gd.gpdid  = ".(($_SESSION["obras"]["gpdid"])? $_SESSION["obras"]["gpdid"] : $_REQUEST["gpdid"]);
	
	$dataTramitacao = $db->pegaUm($sql);
	
}	
// cabecalho padrão do SIMEC
include APPRAIZ . "includes/cabecalho.inc";
// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Grupo de Supervisão", "" );

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script src="../obras/js/obras.js"></script>
<script>$ = jQuery;</script>


<form action="" method="post" name="formulario" id="obrFormLote" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
	<input type="hidden" name="gpdid" id="gpdid" value="<?php print $_SESSION["obras"]["gpdid"]; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="100%" valign="top" style="background: none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
					<tr>
						<td class="subtitulodireita" width="190px">Nº de Controle:</td>  
						<td>
							<b><?php print $_SESSION["obras"]["gpdid"]; ?></b>
						</td>
						<td class="subtitulodireita" width="190px"><b> Data da Tramitação: </b></td>
						<td>
							<b><?php print $dataTramitacao; ?></b>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">UF:</td>
						<td>
							<input type="hidden" name="estuf_h" id="estuf_h" value="<?php print $estuf; ?>"/>
							<div id="estuflote"><?php print $estuf; ?></div>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Empresa:</td>
						<td><div id="empresalote"><?php print $entnome; ?></div>
						</td>
					</tr>
					<tr>
						<td class="subTituloDireita">Data de Início:</td>
						<td>
							<?php print campo_data2( 'gpddtinicio', 'N', $obrSupSoLeitura, '', 'S', '', '' ); ?>
						</td>
					</tr>
					<tr>
						<td class="subTituloDireita">Data de Término:</td>
						<td>
							<?php print campo_data2( 'gpdtermino', 'N', $obrSupSoLeitura, '', 'S', '', '' ); ?>
						</td>
					</tr>
				</table>
			
				<?php if( $esdid == OBRDISTRIBUIDO || $esdid == OBRREDISTRIBUIDO || !$esdid ): ?>
			
				<table width="100%">
					<tr align="center" valign="top">
					
						<!-- Repositório -->
						<td style="width: 100%;">
							<fieldset style="height: 350px; width: 94%;">
								<legend>REPOSITÓRIO</legend>
								<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
									<tr>
										<td class="SubTituloDireita" width="50px">UF:</td>
										<td>
											<?php 
											
												$sql = "SELECT
															estuf as codigo,
															estdescricao as descricao
														FROM
															territorios.estado
														ORDER BY
															estuf";
												
												$db->monta_combo("estuf", $sql, $obrSupUFLeitura, "Selecione...", 'obrListaObras(this.value);obrListaMunicipios', '', '', '150', 'S','estuf');
											
											?>
										</td>
									</tr>
									<tr>
										<td class="subTituloDireita">Município:</td>
										<td>
											<span id="listaMunicipios" style="color: #aaaaaa;">
												Selecione um estado...
											</span>
										</td>
									</tr>
								</table>
								<center>
									<div id="listaObrasRepositorioLote" style="width: 100%; height: 290px; overflow: auto;">
										<font style="font-size: 8pt; color: #dd0000;">
											<center>Selecione a UF para visualizar as obras.</center>
										</font>
									</div>
								</center>
							</fieldset>
						</td>
					</tr>
				</table>
				
				<? endif; ?>
					
				<table width="100%">
					<tr align="center" valign="top">
							
						<!-- Lote -->
						<td style="width: 100%;">
							<fieldset style="height: 350px; width: 94%;">
								<legend>GRUPO DE SUPERVISÃO</legend>
								
								<div style="width: 100%; height: 340px; overflow: auto;">
									<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" id="tblLoteObras">
										<tr>
											<td class="SubTituloCentro" width="20%">Nome da Obra</td>
											<td class="SubTituloCentro" width="11%">Procedimentos</td>
											<td class="SubTituloCentro" width="11%">Prioridade Supervisão</td>
											<td class="SubTituloCentro" width="10%">Área Construída</td>
											<td class="SubTituloCentro" width="10%">Município</td>
											<td class="SubTituloCentro" width="20%">Unidade</td>
											<td class="SubTituloCentro" width="10%">Tipo de Ensino</td>
											<td class="SubTituloCentro" width="9%">Situação da Obra</td>
											<td class="SubTituloCentro" width="5%">% Executado</td>
											<td class="SubTituloCentro" width="10%">Questionário</td>
											<td class="SubTituloCentro" width="10%"> Situação Parecer MEC</td>
											<td class="SubTituloCentro" width="10%">Data último Parecer</td>
											<td class="SubTituloCentro" width="5%">Qtd. de Pareceres</td>
											<td class="SubTituloCentro" width="10%">Situação Tramitação</td>
											<td class="SubTituloCentro" width="10%">Data Tramitação</td>
											<td class="SubTituloCentro" width="10%">Nº dia(s) após a última Tramitação</td>
											<td class="SubTituloCentro" width="10%">Qtd. dia(s) até a última Tramitação</td>
											<td class="SubTituloCentro" width="10%">Histórico da Obra</td>
											<!--<td class="SubTituloCentro" width="10%">Situação do Grupo</td>-->
										</tr>
										
										<?php $supervisao->obrMotaListaGrupo( $_SESSION["obras"]["gpdid"], $obrSupSoLeitura, $esdid ); ?>
										
										<?php if( !$_SESSION["obras"]["gpdid"] ){ ?>
											<tr bgcolor='#ffffff' id='totalGrupo'>
												<td style="border-top: 2px solid rgb(64, 64, 64); border-bottom: 3px solid rgb(223, 223, 223);" colspan="16">
													<b> Total de Registros: <span id="nTotalObrasGrupo">0</span> &nbsp;        
														( Educação Superior: <span id="nTotalObrasSuperior">0</span> |           
														  Educação Profissional: <span id="nTotalObrasProfissional">0</span> |           
														  Educação Básica: <span id="nTotalObrasBasica">0</span> )    
													</b>
												</td>
											</tr>
										<?php } ?>
										
									</table>
								</div>
							</fieldset>
							
							<br/>
							
							<fieldset style=" width: 94%;">
								<legend>Procedimentos</legend>
								<?php 
								
									$sql = " SELECT tppsigla || ': ' || tppdsc  FROM obras.tipoprocedimento";
									$legendaProcedimento = $db->carregarColuna( $sql );
		
									print implode( " | ", $legendaProcedimento );
									
								?>	
							</fieldset>
							
							<br/>
							
							<fieldset style=" width: 94%;">
								<legend>Arquivos</legend>
									<script>
										function anexarArquivoGrupo()
										{
											if( !document.formulario.arqdescricao_grupo.value ){
												alert('Informe a descrição do arquivo!');
											}else{
												document.getElementById("requisicao").value = "inserirArquivoGrupo";
												document.formulario.btn_anexar.value = "carregando...";
												document.formulario.btn_anexar.disabled = "disabled";
												document.formulario.submit();
											}
										}
										
										function downloadArquivo(arqid)
										{
											if(!arqid){
												alert('Arquivo inexistente!');
											}else{
												window.location.href = 'obras.php?modulo=principal/supervisao/criarLote&acao=A&requisicao=downloadArquivo&arqid=' + arqid;
											}
										}
										
										function excluirArquivoGrupo(arqid)
										{
											if(confirm("Deseja realmente excluir o arquivo?")){
												window.location.href = 'obras.php?modulo=principal/supervisao/criarLote&acao=A&requisicao=excluirArquivo&gpdid=<?php echo $_REQUEST['gpdid'] ?>&arqid=' + arqid;
											}
										}
									</script>
										<table class='tabela' width="100%" align="center" >
											<tr>
												<td width="25%" class="subtituloDireita" >Arquivo:</td>
												<td><input type="file" name="arquivo" /> </td>
											</tr>
											<tr>
												<td class="subtituloDireita" >Descrição:</td>
												<td><?php echo campo_texto("arqdescricao_grupo","S","S","Descrição do Arquivo",60,120,"","") ?> </td>
											</tr>
											<tr>
												<td colspan="2" class="subtituloEsquerda" ><input type="button" name="btn_anexar" value="Anexar" onclick="anexarArquivoGrupo()"   /></td>
											</tr>
										</table>
										<?php listaArquivosGrupo($_REQUEST['gpdid']) ?>
								</fieldset>
							
						</td>
					</tr>
				</table>
				
			</td>
			<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
				<?php wf_desenhaBarraNavegacao( $docid , array( 'gpdid' => $_SESSION["obras"]["gpdid"], 'epcid' => $epcid ) ); ?>
				
				
					<?
					/**Alteração Feita dia 13/04/2011 as 19:22 h.
					 * Obs.: Agora as Informações Pendentes referentes ao Questionário, serão Apresentadas na Lista de Grupo de Supervisão das Obras.
					 */ 
 /*condição 		if ( (!valEnviarParaAvaliacaoMec($_SESSION["obras"]["gpdid"]) && $esdid == OBREMAPROVAMEC) || (!valEnviarParaAvaliacaoMec($_SESSION["obras"]["gpdid"]) && $esdid == OBRENVREAVALSUPMEC) || (!valEnviarParaSAA($_SESSION["obras"]["gpdid"]) && $esdid == OBREMAVALIASUPERVMEC) || (!valEnviarParaSAA($_SESSION["obras"]["gpdid"]) && $esdid == OBRREAVSUPVISAO) ):
 agora é essa */			/*if ( ($esdid == OBREMAPROVAMEC) || ($esdid == OBRENVREAVALSUPMEC) || ($esdid == OBREMAVALIASUPERVMEC) || ($esdid == OBRREAVSUPVISAO) ):*/
								/*if( !verificaPreenchQuest( $_SESSION["obras"]["gpdid"] ) ){*/
							?>
								<!-- Caixa dos checklists incompletos -->
									<!--<table cellspacing="0" cellpadding="3" border="0" style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;">
										<tbody>
											<tr style="background-color: rgb(201, 201, 201); text-align: center;">
												<td style="font-size: 7pt; text-align: center;">
													<span title="estado atual">
														<b>Checklist</b>
													</span>
												</td>
											</tr>
																		
											<tr>
												<td onmouseout="this.style.backgroundColor='';" onmouseover="this.style.backgroundColor='#ffffdd';" style="border-top: 2px solid rgb(208, 208, 208); font-size: 7pt; text-align: center;">
													<a onclick="window.open('obras.php?<?php /* echo $_SERVER['QUERY_STRING']; */?>&mostrar=incompletos' , 'Pendentes' , 'width=850px,height=320px,scrollbars=no');" title="Checklists Pendentes" alt="Checklists Pendentes" href="#">Checklists Pendentes</a>
												</td>
											</tr>
										</tbody>
									</table>-->

							<?
								/*}*/ 
							//elseif ( !valEnviarParaSAA($_SESSION["obras"]["gpdid"]) && $esdid == OBREMAVALIASUPERVMEC ):
							?>
								<!-- Caixa dos checklists pendentes
								<table cellspacing="0" cellpadding="3" border="0" style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;">
									<tbody>
										<tr style="background-color: rgb(201, 201, 201); text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<span title="estado atual">
													<b>Checklist</b>
												</span>
											</td>
										</tr>
																	
										<tr>
											<td onmouseout="this.style.backgroundColor='';" onmouseover="this.style.backgroundColor='#ffffdd';" style="border-top: 2px solid rgb(208, 208, 208); font-size: 7pt; text-align: center;">
												<a onclick="window.open('obras.php?<?php //echo $_SERVER['QUERY_STRING']; ?>&mostrar=pendentes' , 'Pendentes' , 'width=850px,height=250px,scrollbars=no');" title="Checklists Pendentes" alt="Checklists Pendentes" href="#">Checklists Pendentes</a>
											</td>
										</tr>
									</tbody>
								</table> -->
							<? 
							/*endif;*/
							?>
				
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2">
				<?php if( !$obrSupDisabled ){ ?> 
					<input type="button" value="Salvar" onclick="obrValidaGrupo();" style="cursor: pointer;"/>
				<?php } ?>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
				<?php if( $_SESSION["obras"]["gpdid"] ){ ?>
				<input type="button" name="obrBtVisualizarObrasMapa" value="Visualizar Obras do Grupo no Mapa" onclick="janela('?modulo=principal/supervisao/mapaGrupo&acao=A', 600, 585, 'mapaGrupo');" style="cursor: pointer;"/>
				<?php } ?>
			</td>
		</tr>
	</table>
</form>
<script>
function popupHistoricoObra(docid){
	  var url = 'http://<?PHP echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
	 }
</script>
<?php 

if( $estuf ){
	print "<script>obrListaObras('{$estuf}');</script>";
}

?>
