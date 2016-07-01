<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$plicod = "";
$plicod = $_REQUEST['plicod'];

if ( $_REQUEST['submete'] == 1 ) {
	$sql = "";
	$sql = "INSERT INTO monitora.planointernohistorico( plicod, pihobs, pihsituacao, pihdata, usucpf )
			VALUES( '{$plicod}', '{$_REQUEST['pioobs']}', '".$_REQUEST['situacaonova']."', now(), '{$_SESSION['usucpf']}' )";

	$db->executar($sql);
	
	$sql = "";
	$sql = "UPDATE
				monitora.planointerno
			SET
				plisituacao = '".$_REQUEST['situacaonova']."'
			WHERE
				plicod in ('{$plicod}')";
	
	$db->executar($sql);
	$db->commit();
	
	enviaEmailStatusPi($plicod);
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.opener.document.pesquisa.submit();
			//window.opener.location.reload();
			//window.parent.opener.location.href = window.opener.location;
			self.close();
		</script>";	
	exit;
}

// Cria o título da tela
$titulo_modulo = "Dados do Plano Interno";
monta_titulo( $titulo_modulo, "Segue abaixo os dados do Plano Interno Selecionado");

/*
 * Aplicando REGRA 0001
 */
if(!$db->testa_superuser())
	$regra_0001 = aplicarregra_0001();
/*
 * FIM Aplicando REGRA 0001
 */
	

?>

<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
		<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
		<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
		<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
		
		<script type="text/javascript">
		
		messageObj = new DHTML_modalMessage();	// We only create one object of this class
		messageObj.setShadowOffset(5);	// Large shadow
		
		
		function displayMessage(url)
		{
			
			messageObj.setSource(url);
			messageObj.setCssClassMessageBox(false);
			messageObj.setSize(400,240);
			messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
			messageObj.display();
		}
		
		function displayStaticMessage(messageContent,cssClass)
		{
			messageObj.setHtmlContent(messageContent);
			messageObj.setSize(400,250);
			messageObj.setCssClassMessageBox(cssClass);
			messageObj.setSource(false);	// no html source since we want to use a static message here.
			messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
			messageObj.display();
			
			
		}
		
		function closeMessage()
		{
			messageObj.close();	
		}
		
		</script>

		<script type="text/javascript">
			function altera( situacao, pi ){
				
				
				
				if ( situacao == 'R' ){
					//window.open('?modulo=principal/planointerno/obspirecusado&acao=A&tipo=2&pi=' + pi,'Observação do PI','width=520, height=400, scrollbars=yes');
					displayMessage('?modulo=principal/planointerno/obspirecusado&acao=A&tipo=2&pi=' + pi);
				}else{
						document.pesquisa.situacaonova.value = situacao;
						document.pesquisa.submit();	
				}
			
			}
		</script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
	</head>
	<body>
		<?php 
		
			if ( empty($plicod) ){
				
				echo '<table width="95%;" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>
					  </table>';
				
			}else{

				$sql = "";
				$sql = "SELECT
							CASE WHEN 
								pl.plititulo IS NOT NULL
							THEN 
								pl.plititulo 
							ELSE 
								'Não Preenchido' END as titulo,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							round(sum( coalesce(rofdot_ini,0) ),2) as dotacaoinicial,
							coalesce((SELECT 
										SUM(plivalor) as valor
									  FROM 
									  	monitora.planointerno 
									  WHERE 
									  	pl.pliptres = pliptres AND 
									  	plistatus='A'),0) as detalhamento,
							pl.pliptres, 
							pl.plilivre as codificacao,
							pl.plidsc,
							pl.plivalor,
							pc.cdtcod as enquadramento,
							pc.cdtcod ||' - '|| pc.cdtdsc as enquadramentodsc,
							sub.sbacod ||' - '|| sub.sbatitulo as subacaodsc,
							pc1.cdtcod as nivel,
							pc1.cdtcod ||' - '|| pc1.cdtdsc as niveldsc,
							pc2.cdtcod as apropriacao,
							pc2.cdtcod ||' - '|| pc2.cdtdsc as apropriacaodsc, 
							pc3.cdtcod as modalidade,
							pc3.cdtcod ||' - '|| pc3.cdtdsc as modalidadedsc, 
							pc4.cdtcod ||' - '|| pc4.cdtdsc as executordsc,
							pc5.cdtcod ||' - '|| pc5.cdtdsc as gestordsc,
							
							-- status do PI
							pl.plisituacao as plisituacao,
							-- atividade
							pia.atiid as atividade,
							pl.obrid as obras, 
							UPPER(obr.obrdesc) as obradesc, 
							UPPER(uni.entnome) as nomeunidade,
							UPPER(cam.entnome) as nomecampus 
						FROM monitora.planointerno pl 
						LEFT JOIN obras.obrainfraestrutura obr ON pl.obrid = obr.obrid 
						LEFT JOIN entidade.entidade uni ON uni.entid = obr.entidunidade 
						LEFT JOIN entidade.entidade cam ON cam.entid = obr.entidcampus
						INNER JOIN monitora.acao ac ON ac.acaid = pl.acaid and ac.prgano= '{$_SESSION['exercicio']}'
						LEFT JOIN financeiro.execucao e ON trim(pl.pliptres)=trim(e.ptres) and e.rofano = '{$_SESSION['exercicio']}'
						INNER JOIN public.combodadostabela pc ON pl.eqdid = pc.cdtid
						INNER JOIN public.combodadostabela pc1 ON pl.dpiid = pc1.cdtid
						INNER JOIN public.combodadostabela pc2 ON pl.cpiid = pc2.cdtid
						LEFT JOIN public.combodadostabela pc3 ON pl.modid = pc3.cdtid
						INNER JOIN monitora.subacao sub ON pl.sbaid = sub.sbaid
						INNER JOIN public.combodadostabela pc4 ON sub.exeid = pc4.cdtid
						INNER JOIN public.combodadostabela pc5 ON sub.gstid = pc5.cdtid
						LEFT JOIN monitora.planointernoatividade pia ON pia.pliid = pl.pliid
						WHERE pl.plicod = '{$plicod}' AND
							  ac.acastatus = 'A' AND
							  ac.acasnrap = false AND
							  pl.plistatus='A'
						GROUP BY 
							pl.pliid, pl.plivalor, ac.prgcod, pl.pliptres, ac.acacod, ac.acadsc, 
							ac.loccod, ac.unicod, pl.plititulo, pc.cdtcod, pc.cdtdsc, pl.plidsc, pl.plilivre, pl.pliacao, 
							pc1.cdtcod, pc1.cdtdsc, pc2.cdtcod, pc2.cdtdsc, pc3.cdtcod, pc3.cdtdsc,
							pc4.cdtcod, pc4.cdtdsc, pc5.cdtcod, pc5.cdtdsc,
							sub.sbacod, sub.sbatitulo, pl.plisituacao,pia.atiid, pl.obrid, obrdesc, nomeunidade, nomecampus
						ORDER BY 
							pl.pliid";
				$dadosPI = $db->carregar($sql);
				if($dadosPI[0]) {
					$dadoscomuns = current($dadosPI);
					$plititulo = $dadoscomuns['titulo'];
					$codificacao = $dadoscomuns['codificacao'];
					$plidsc = $dadoscomuns['plidsc'];
					$plivalor = $dadoscomuns['plivalor'];
					$subacaodsc = $dadoscomuns['subacaodsc'];
					$enquadramento = $dadoscomuns['enquadramento'];
					$enquadramentodsc = $dadoscomuns['enquadramentodsc'];
					$nivel = $dadoscomuns['nivel'];
					$niveldsc = $dadoscomuns['niveldsc'];
					$apropriacao = $dadoscomuns['apropriacao'];
					$apropriacaodsc = $dadoscomuns['apropriacaodsc'];
					$modalidade = $dadoscomuns['modalidade'];
					$modalidadedsc = $dadoscomuns['modalidadedsc'];
					$executordsc = $dadoscomuns['executordsc'];
					$gestordsc = $dadoscomuns['gestordsc'];
					$plisituacao = $dadoscomuns['plisituacao'];
					$atividade = $dadoscomuns['atividade'];
					$obras = $dadoscomuns['obras'];
					$obradesc = $dadoscomuns['obradesc'];
					$nomeunidade = $dadoscomuns['nomeunidade'];
					$nomecampus = $dadoscomuns['nomecampus'];  
					
					
				} else {
					//echo "<script>alert('Erro no Plano Interno. Entre em contato com o suporte!');window.close();</script>";
				}
				// se não for ligada a uma atividade (normalmente pertence ao obras)
				if($atividade) {
					$_REQUEST['atiid'] = $atividade;
					$atividade_resumo = atividade_pegar( $atividade );
					extract( $atividade_resumo );
					$_SESSION["projeto"] = $atividade_resumo["_atiprojeto"];
					define( "PROJETO", $atividade_resumo["_atiprojeto"] );
				}
			?>
		<form method="post" name="pesquisa" id="pesquisa">
			<input type="hidden" name="submete" value="1"/>	
			<input type="hidden" name="plicod" value="<?=$_REQUEST['plicod']?>">
			<input type="hidden" name="tipoacao" value="<?=$_REQUEST['tipoacao']?>">
			<input type="hidden" name="situacaonova" value="">
			
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2">
						<?
					if ($nao_mostra_cabecalho==false) {
						if($obras) {
							echo "<table cellpadding=\"3\" border=\"0\" width=\"98%\" align=\"center\"  style=\"BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;\">
									<tr>
										<td class='SubTituloDireita'><strong>UNIDADE :</strong></td>
										<td>".$nomeunidade."</td>
									</tr>
									<tr>
										<td class='SubTituloDireita'><strong>CAMPUS :</strong></td>
										<td>".$nomecampus."</td>
									</tr>
									<tr>
										<td class='SubTituloDireita'><strong>OBRA :</strong></td>
										<td>".$obradesc."</td>
									</tr>
									
								  </table>";
						} else {
							echo montar_resumo_atividade( $atividade_resumo, false, true );
						}
					}
						?>
					</td>
				</tr>
				<tr>
				    <td colspan="2">
				        <table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
						<tr>
						<td style="background-color: #C9C9C9;" colspan="6" align="center"><b>Detalhamento Orçamentário<br/> Valores em Reais (R$)</b></td>
						</tr>
				        <tr>
							<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
							<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
						    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação inicial</b></td>
						    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado em outros PI's</b></td>
						    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Disponível</b></td>
						    <td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
						</tr>
						<? 
						if($dadosPI[0]) {
							$valortotalpi = 0;
							$cor = 0;
							foreach($dadosPI as $acpl) { 
						?>
				        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['pliptres']; ?>">
							<td align="center"><? echo $acpl['pliptres']; ?></td>
							<td align="center"><? echo $acpl['descricao']; ?></td>
						    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
						    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
						    <td align="right"><? echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); ?></td>
						    <td align="center"><input type="text" name="plivalored[<? echo $acpl['pliid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['plivalor'],2,',','.'); ?>" class="disabled"  style="text-align : right; width:25ex;" title='' readonly="readonly" /></td>
						</tr>
						<? 
								$cor++;
								$valortotalpi = $valortotalpi + $acpl['plivalor']; 
							}
						} 
						?>
				        <tr style="height: 30px;">
							<td align="right" valign="top" colspan="5"><b>TOTAL :</b></td>
							<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
						</tr>
				        </table>
				    </td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" nowrap>Enquadramento da Despesa:</td>
				    <td><? echo $enquadramentodsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Subação:</td>
				    <td><? echo $subacaodsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
					<td><?= $executordsc ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
					<td><?= $gestordsc ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Nível/Etapa de Ensino:</td>
				    <td><? echo $niveldsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Categoria de Apropriação:</td>
				    <td><? echo $apropriacaodsc; ?></td>
				</tr>
				<? 
				/*
				 * SOLICITADO PELO HENRIQUE XAVIER (21/05/09)
				 * VERIFICAR CONDIÇÃO ESPECIAL PARA PIS DO OBRAS
				 */
				if(!$obras) { 
				
				?>
				<tr>
					<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
				    <td><?=$codificacao?></td>
				    <input type="hidden" name="codificacao" id="codificacao" value="<?=$codificacao?>" />
				    <?php
				    	//campo_texto('codificacao','N','N','',7,3,'','',null,null,null,'','');
				    ?>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Modalidade de Ensino:</td>
				    <td><? echo $modalidadedsc; ?></td>
				</tr>
				<? } else { ?>
				<tr>
					<td align='right' class="SubTituloDireita">Código sequencial:</td>
				    <td><? echo substr($plicod,-3,3); ?></td>
				</tr>
				<?
				} 
				/*
				 * SOLICITADO PELO HENRIQUE XAVIER (21/05/09)
				 * VERIFICAR CONDIÇÃO ESPECIAL PARA PIS DO OBRAS
				 */
				?>
				<tr>
					<td align='right' class="SubTituloDireita">Título:</td>
				    <td><?=$plititulo?></td>
				    <input type="hidden" name="plititulo" id="plititulo" value="<?=$plititulo?>" />
				   	<?php
				   		//echo campo_texto('plititulo','N','N','',50,45,'','');
				   	?>
				</tr>
				
				<tr>
				    <td align='right' class="SubTituloDireita">Descrição:</td>
				    <td><?=$plidsc?></td>
				    <input type="hidden" name="plidsc" id="plidsc" value="<?=$plidsc?>" />
				    <?php
				    	//campo_textarea( 'plidsc','N','N','',60,2,250);
				    ?>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Situação:</td>
				    <td align="left">
				    <?
				    	switch($plisituacao) {
				    		case 'P':
				    			echo "<font color=\"red\">Pendente</font>";
				    			break;
				    		case 'C':
				    			echo "<font color=\"green\">Aprovado</font>";
				    			break;
				    		case 'H':
				    			echo "<font color=\"blue\">Homologado</font>";
				    			break;
				    		case 'V':
				    			echo "<font color=\"#3F85FF\">Revisado</font>";
				    			break;
				    		case 'S':
				    			echo "<font color=\"#AF7817\">Cadastrado no SIAFI</font>";
				    			break;
				    		case 'R':
				    			echo "<font color=\"#EAC117\">Enviado para Revisão</font>";
				    			break;
				    	}
				    ?>
				    </td>
				</tr>
				<tr>
				    <td align='right' class="SubTituloDireita">Previsão PI:</td>
				    <td align="left">
				    	<table style="background-color: #C9C9C9" cellpadding="" border="0" width="126px" >
			        		<tr>
				        		<td>
				        		<table cellpadding="0" border="0" width="98%" >
					        		<tr>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Enquadramento</b></td>
					        		<td align="center" colspan="3"><b>Subação</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Nível</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Apropriação</b></td>
					        		<? if(!$obras) { ?>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Codificação</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Modalidade</b></td>
					        		<? } else { ?>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center" colspan="2"><b>Código sequencial</b></td>
					        		<? } ?>
					        		</tr>
					        		<tr>
					        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="enquadramento"><? echo $enquadramento; ?></span></td>
						        		<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan=3><span id="gerador1"><? echo substr($subacaodsc,0,1); ?></span><span id="gerador2"><? echo substr($subacaodsc,1,1); ?></span><span id="gerador3"><? echo substr($subacaodsc,2,2); ?></span></td>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="nivel"><? echo $nivel; ?></span></td>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="apropriacao"><? echo $apropriacao; ?></span></td>
										<? if(!$obras) { ?>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo $codificacao; ?></span></td>		        			        			  
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="modalidade"><? echo $modalidade; ?></span></td>
										<? } else { ?>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan="2"><span id="sequencial"><? echo substr($plicod,-3,3); ?></span></td>
										<? } ?>
					        		</tr>
					        		<tr><td style="background-color: #C9C9C9;" align="center" colspan="8">&nbsp;</td></tr>
					        		<tr>
					        		<td style="background-color: #C9C9C9;" align="center" colspan="8"><b>Código do PI/SIAFI:</b> <?=$plicod?></td>
					        		</tr>
					        	</table>	        		
				        		</td>
			        		</tr>
				        </table>
				    </td>
				</tr>
				<tr>
					<td align="center" colspan="6" style="background-color: rgb(201, 201, 201);">
						<b>Histórico do PI</b>
					</td>
				</tr>
			</table>
			<center>
			<div id="valida" style="overflow:auto; width:95%; height:150px; border-bottom:2px solid #cccccc;">
				<?php 
						$sql = "";
						$sql = "SELECT
									to_char(po.pihdata, 'DD/MM/YYYY HH24:MI:SS') as data,
									CASE WHEN 
										po.pihsituacao = 'P' 
										THEN 
											' Pendente '
										WHEN
											po.pihsituacao = 'C' 
										THEN 
											' Aprovado '
										WHEN
											po.pihsituacao = 'S' 
										THEN 
											' Cadastrado no SIAFI '
										WHEN
											po.pihsituacao = 'H' 
										THEN 
											' Homologado '
										WHEN
											po.pihsituacao = 'V' 
										THEN 
											' Revisado '
										WHEN
											po.pihsituacao = 'R' 
										THEN 
											' Enviado para Revisão ' END as situacao,
									po.pihobs,
									su.usunome
								FROM
									monitora.planointernohistorico po
								INNER JOIN
									seguranca.usuario su ON po.usucpf = su.usucpf 
								WHERE
									--po.pihsituacao = 'R'
									--AND
									po.plicod = '{$plicod}'";
						
						$cabecalho = array("Data/Hora", "Situação", "Observação", "Cadastrado Por");
						$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%' );
					?>
			</div>
			</center>
		<?php } ?>
		<?php if( $_REQUEST['tipoacao'] != 'consulta' ){ ?>
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
				<tr bgcolor="#C0C0C0">
					<td align="right">
					<?
					// CPF's do Paulo Rocha, Wesley Washington e Leo Kessel.
					if($_SESSION['usucpf'] == '' || 
					   $_SESSION['usucpf'] == '' || 
					   $_SESSION['usucpf'] == '' || 
					   $_SESSION['usucpf'] == '' || 
					   $_SESSION['usucpf'] == '' || 
					   $_SESSION['usucpf'] == '' ||
					   $_SESSION['usucpf'] == '' ||
					   $_SESSION['usucpf'] == '' ||
					   $_SESSION['usucpf'] == '' ||
					   $_SESSION['usucpf'] == '' ||
					   $_SESSION['usucpf'] == '' ||
					   $_SESSION['usucpf'] == '') {
						echo "<input type=\"button\" value=\"Alterar para Homologado\" onclick=\"altera('H', '".$plicod."');\" style=\"cursor: pointer;\" />
							  <input type=\"button\" value=\"Enviar para Revisão\" onclick=\"altera('R', '".$plicod."');\" style=\"cursor: pointer;\" />
							  <input type=\"button\" value=\"Alterar para Aprovado\" onclick=\"altera('C', '".$plicod."');\" style=\"cursor: pointer;\" />
							  <input type=\"button\" value=\"Alterar para Cadastrado no SIAFI\" onclick=\"altera('S', '".$plicod."');\" style=\"cursor: pointer;\" />";
					}
					else {
						switch($plisituacao) {
							case "P":
								if(!$regra_0001['btn_alterahomologado_disabled'])
									echo "<input type=\"button\" value=\"Alterar para Homologado\" onclick=\"altera('H', '".$plicod."');\" style=\"cursor: pointer;\" />
										  <input type=\"button\" value=\"Enviar para Revisão\" onclick=\"altera('R', '".$plicod."');\" style=\"cursor: pointer;\" />";
								break;
							case "C":
								echo "<input type=\"button\" value=\"Alterar para Cadastrado no SIAFI\" onclick=\"altera('S', '".$plicod."');\" style=\"cursor: pointer;\" />";
								break;
							case "H":
								if($_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '') {
									echo "<input type=\"button\" value=\"Alterar para Aprovado\" onclick=\"altera('C', '".$plicod."');\" style=\"cursor: pointer;\" />";
								}
								if(!$regra_0001['btn_enviadorevisao_disabled'])
									echo "<input type=\"button\" value=\"Enviar para Revisão\" onclick=\"altera('R', '".$plicod."');\" style=\"cursor: pointer;\" />";
								break;
							case "R":
								if($_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' || 
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '' ||
								   $_SESSION['usucpf'] == '') {
									echo "<input type=\"button\" value=\"Alterar para Aprovado\" onclick=\"altera('C', '".$plicod."');\" style=\"cursor: pointer;\" />";
								}
								if(!$regra_0001['btn_alterahomologado_disabled'])
									echo "<input type=\"button\" value=\"Alterar para Homologado\" onclick=\"altera('H', '".$plicod."');\" style=\"cursor: pointer;\" />";
								break;
							case "V":
								echo "&nbsp;";
								break;
							case "S":
								echo "&nbsp;";
								break;
						}
					}
					?>
					</td>
				</tr>
			</table>
		<?php } ?>
		</form>
	</body>
</html>