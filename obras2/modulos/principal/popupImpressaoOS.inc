<?
ini_set("memory_limit", "64M");
header('Content-type: text/html; charset="iso-8859-1"',true);

$sosid  = ($_GET['sosid'] ? $_GET['sosid'] : $_SESSION['obras2']['sosid']);
$os 	= new Supervisao_Os( $sosid );
extract( $os->getDados() );
$grupoEmpresa = new Supervisao_Grupo_Empresa();
$sgrid 		  = $grupoEmpresa->pegaGrupoIdPorSgeid( $sgeid );

$docid = pegaDocidOs( $os->sosid );
$usuarioAcao = null;

if(wf_acaoFoiExecutada($docid, ESDID_OS_ENVIADA_EMPRESA, ESDID_OS_EXECUCAO)){
    $acao = wf_pegarAcao(ESDID_OS_ENVIADA_EMPRESA, ESDID_OS_EXECUCAO);
    $usuarioAcao = pegaUsuarioWorkflowExecutorAcao($docid, $acao['aedid']);
}

$osObra  		 = new Supervisao_Os_Obra();
$arEmpid 		 = $osObra->listaEmpidPorOs( $sosid );

?>
<html>
	<head>
		<title>O.S.</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	</head>
	<body topmargin="0" leftmargin="0">
		<form method="post" action="" name="form_aditivo" id="form_aditivo" enctype="multipart/form-data">
			<table class="Tabela" align="center" cellPadding="3">
			  	<tr>
			    	<td colspan="3">
					<?php echo monta_cabecalho_relatorio(100); ?>
					</td>
				</tr>
			  	<tr>
			    	<td class="SubTituloDireita">OS Nº</td>
			        <td>
						<b><?=$sosnum ?></b>
			        </td>
			        <td rowspan="7" align="right" class="notprint">
		            <?php
		            // Barra de estado WORKFLOW
		                wf_desenhaBarraNavegacao($docid, array('sosid'=>$sosid,'tpdid'=>WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO));
		            ?>
			        </td>
				</tr>
			    <tr>
			    	<td class="SubTituloDireita">Data Emissão</td>
			        <td>
						<?php echo formata_data($sosdtemissao);?>
			        </td>
				</tr>
				<tr>
			        <td class="SubTituloDireita">Nota de Empenho</td>
					<td id="tdEmpenho">
						<?php
						$empenho2 = new Supervisao_Empenho( $semid );
						echo $empenho2->semnumempenho;	
			            ?>
					</td>
				</tr>
			  	<tr bgcolor="#C0C0C0">
			  		<td colspan="2" align="center" >
			  			<b>Bloco 1 - Dados do Contrato</b>
			  		</td>
			  	</tr>
				<tr>
			        <td class="SubTituloDireita">1.1 - Empresa</td>
					<td id="tdComboEmpresa">
						<?php
						$sql = "SELECT 
							   		replace(to_char(e.entnumcpfcnpj::int8,'00{}000{}000/0000-00'),'{}','.') || ' - ' || e.entnome AS descricao 
							    FROM 
									obras2.supervisao_grupo_empresa sge
								JOIN entidade.entidade e ON e.entid = sge.entid
								WHERE
									sgestatus='A'
									AND sge.sgeid = $sgeid
							    ORDER BY 
									entnome";
						echo $db->pegaUm($sql);
			            ?>
					</td>
				</tr>
			    <tr>
			    	<td class="SubTituloDireita" valign="top">1.2 - Período de Execução do Serviço</td>
			    	<td colspan="1">
			    		<div>
							<?=formata_data( $sosdtinicio )?>
				            <b>até</b> <?=formata_data( $sosdttermino ); ?>
				            -
                                            <?if($sosemergencial == 'f'):?>
                                                  <?= ' '.$sosdiasexecucao.' DIAS';?>
                                            <?else:?>
                                                <?='EMERGENCIAL '.$sosdiasexecucao.' DIAS';?>
                                            <?endif;?>
			            </div>
			        </td>
				</tr>
				<tr>
			        <td class="SubTituloDireita">
			        	1.3 - Valor Total dos Serviços (R$)
			        </td>
					<td id="tdTotalServico" colspan="1">
						<?=($sosvalortotal ? number_format($sosvalortotal, 2, ',', '.') : '-'); ?>
					</td>
				</tr>
				<tr>
			        <td class="SubTituloDireita">
			        	1.4 - Serviço
			        </td>
					<td id="tdTotalServico" colspan="3">
						<p>Supervisão de ações de infraestrutura , tais como construção, reforma, ampliação e instalação, 
						financiadas com recursos federais,  com emissão de Relatório de Supervisão e Laudo Técnico de Supervisão da 
						Obra, conforme especificações e condições previstas no Contrato.</p>
					</td>
				</tr>
			        
			  	<tr bgcolor="#C0C0C0">
			  		<td colspan="3" align="center" >
			  			<b>Bloco 2 - Especificação dos Serviços</b>
			  		</td>
			  	</tr>
				<tr>
			        <td class="SubTituloDireita" colspan="3">
			        	<center>2.1 - Obras</center>
			        </td>
				</tr>
				<tr>
			        <td colspan="3">
						<table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_obra">
				<?php 
				$vlrTotalServico = 0;
				if ( !empty( $sgrid ) ){
					$supMesoregiao = new Supervisao_Grupo_Mesoregiao();
					$arMescod 	   = $supMesoregiao->pegaMescodPorSgrid( $sgrid );
					
					$empreendimento = new Empreendimento();
					$param = array('mescod' => $arMescod,
								   'orgid'	=> $orgid);
					$arDadoEmp = $empreendimento->listaDadosPorEmpid( $arEmpid );

					if ( count($arDadoEmp) ){
				?>
							<thead>
			                	<tr style="background-color: #CDCDCD;">
			                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		Nome da Obra
			                        </th>
			                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		UF
			                        </th>
			                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		Mesorregião
			                        </th>
			                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		Microrregião
			                        </th>
			                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		Município
			                        </th>
			                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		Valor Laudo
			                    		<br>
			                    		(R$)
			                        </th>
								</tr>
							</thead>
				<?php	
						if ( $sgeid ){
							$grupoEmpresa  = new Supervisao_Grupo_Empresa();
							$valorUnitario = $grupoEmpresa->pegaValoroPorSgeid( $sgeid );
                            $valorUnitario = (count($arEmpid) > 10) ? $valorUnitario['sgevalorb'] : $valorUnitario['sgevalora'];
						}
						
						$i				 = 0;
						foreach ( $arDadoEmp as $dadoEmp ){

							if(in_array($dadoEmp['empid'], $arEmpid)){
								
								$color = ($i%2 ? '#FFFFFF' : '#E9E9E9');
								$i++;
								if ( in_array($dadoEmp['empid'], $arEmpid) ){
									$vlrTotalServico += $valorUnitario;	
								}

				?>
							<tbody>
			                	<tr style="background-color: <?=$color ?>;">
			                    	<td valign="middle" align="left" style="padding-left: 5px; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                        <?
                                        $situacao = $osObra->pegaSituacaoSupervisao($sosid, $dadoEmp['empid']);
                                        if($situacao)
                                            $situacao = ($situacao['esdid'] == 1188) ? '<b style="color:red">('. $situacao['esddsc'] . ')</b>' : '';
                                        ?>
			                    		<?='<label for="empid_' . $dadoEmp['empid'] . '">(' . $dadoEmp['obrid'] . ') ' . $dadoEmp['obrnome'] . ' '. $situacao . '</label>' ?>
			                        </td>
			                    	<td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		<?=$dadoEmp['estuf'] ?>
			                        </td>
			                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		<?=$dadoEmp['mesdsc'] ?>
			                        </td>
			                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		<?=$dadoEmp['micdsc'] ?>
			                        </td>
			                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    		<?=$dadoEmp['mundescricao'] ?>
			                        </td>
			                    	<td  valign="middle" align="right" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			                    	<?php echo ($valorUnitario ? number_format($valorUnitario, 2, ',', '.') : '-'); ?>
			                        </td>
								</tr>
                                                                
                                                        <? if(!empty($sosobs)): ?>
                                                        <tr>
                                                            <td colspan="6">
                                                                <p><span  style="font-weight: bold">Observações:</span>
                                                                    <br /><?= ucfirst($sosobs)?>
                                                                </p>
                                                            </td>
                                                        </tr>
                                                        <? endif; ?>
                                                                
							</tbody>
				<?php	
							}		
						}
					}else{
				?>
							<tr style="color: red;">
								<td>
									Nenhuma obra encontrada.
								</td>
							</tr>
				<?php		
					}
				}
				?>
                                                        
						</table>
			        </td>
				</tr>
			  	<tr bgcolor="#C0C0C0">
			  		<td colspan="3" align="center" >
			  			<b>Bloco 3 - Autorização da OS</b>
			  		</td>
			  	</tr>
				<tr>
			        <td colspan="3">
			        	<p>Tendo em vista as informações abaixo, solicito execução da presente OS.</p>
						<p>a) O planejamento dos serviços objeto desta OS foi analisado e aprovado e consta dos registros do <?php echo SIGLA_SISTEMA; ?>.<br> 
						b) Consta disponibilidade orçamentária na unidade gestora, conforme notas de empenho.</p>
			        </td>
			    </tr>
				<tr>
			        <td width="50%">
			        	<br>
				        AUTORIZADO PELA CGIMP/DIGAP/FNDE<br>
						EM <?php echo formata_data($sosdtemissao);?><br>
						<br>
			        	<br>
			        	<?php 
		        		if($docid){
		        			$arrUsu = pegaUsuarioWorkflowExecutor($docid);
		        		}
			        	?>
						<center>
						<?php if($arrUsu): ?>
							Nome: <?php echo $arrUsu['usunome'] ?><br/>
							CPF: <?php echo mascaraglobal($arrUsu['usucpf'],".###.###.###-##");?><br/>
							Data: <?php echo $arrUsu['data'] ?>
						<?php else: ?>
							<!-- O que colocar quando não houver dados? -->
						<?php endif; ?>
						</center>
						
			        </td>
			        <td colspan="2">
			        	<br>
				        ACEITE DA CONTRATADA<br>
						EM 
                            <?if ($usuarioAcao):?>
                                <?=$usuarioAcao['data']?> - <?=$usuarioAcao['usunome']?>
                            <?else:?>
                            ___/___/______<br>
                            <?endif;?>
						<br>
			        	<br>
						<center>
						______________________________________<br>
						Assinatura
						</center>
			        </td>
			    </tr>
			  	<tr bgcolor="#C0C0C0">
			  		<td colspan="3" align="center" class="notprint">
			  			<input type="button" value="Imprimir" onclick="window.print();">
			  		</td>
			  	</tr>
			</table>
		</form>	
	</body>
</html>