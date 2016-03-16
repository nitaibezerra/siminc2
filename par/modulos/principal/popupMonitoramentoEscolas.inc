

<?php
	
	$inuid = $_REQUEST['inuid'] ? $_REQUEST['inuid'] : $_SESSION['par']['inuid'];
	

function salvarEscolas(){
	global $db;
	// Caso existam escolas
	if(is_array($_POST['escid']))
	{
		//Zera contador
		$totalItem = 0;
		$icoId 		= $_REQUEST['icoid'];
		
		// Monta array com variáveis
		foreach ( $_POST['escid'] as $k => $escid )
		{
			if($escid)
			{
				$arrEscolas[] = array(
					'icoid' 	  => $icoId,
					'escid' 	  => $escid,
					'qtdRecebida' => $_POST['qtdrecebida'][$k]
				);
			}
			
			$totalItem = $totalItem + $_POST['qtdrecebida'][$k];
		}
		
		// Salvando
		foreach ( $arrEscolas as $key => $escola )
		{
			
			$escolaId 	= $escola['escid'];
			$valor		= $escola['qtdRecebida'];
			if(! $escolaId)
			{
				print_r($escola);die();
			}
			// Verfifica se existe registro, caso exista da o update, senão o insert
			$sqlCheck = "
				SELECT 
					mieid 
				FROM 
					par.monitoramentoitensescolas
				WHERE
					icoid = {$icoId}
				AND 
					escid = {$escolaId}
			"; 
			$result = $db->pegaLinha($sqlCheck);
			
			$valor = is_numeric($valor) ? $valor : 'null';
			
			// Caso exista
			if(is_array($result) && count($result))
			{
				$mieid = $result['mieid'];
				
				$sqlMod= "
						UPDATE 
							par.monitoramentoitensescolas 
						SET
							mieqtdrecebida = {$valor}
						WHERE
							mieid = {$mieid}
					";
				
				$db->executar($sqlMod);
			}
			else 
			{
				// Insere novo registro
				$sqlMod = "
					INSERT into par.monitoramentoitensescolas 
						(icoid, escid, mieqtdrecebida)
					VALUES
						({$icoId}, {$escolaId} , {$valor})
				";
				$db->executar($sqlMod);
				
			}
			
		}
		
		// Insere o total dos itens
		// Caso o valor tenha sido deixado em branco ele coloca o valor de 0 para o insert/update 
			$valorItens = ( $totalItem ) ? $totalItem : 0;
			// Verifica o valor do termo para o item
			$sqlItem = " SELECT  DISTINCT
							(CASE WHEN sbacronograma = 1 THEN
						                               ( SELECT DISTINCT
						                                               CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
						                               FROM par.subacaoitenscomposicao sic 
						                               WHERE sic.sbaid = s.sbaid
						                               AND sic.icoano = sd.sbdano
						                               and sic.icoid = si.icoid
						                               GROUP BY sic.sbaid, sic.icovalidatecnico )
						                ELSE 
						                               ( SELECT DISTINCT
						                                                               CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
						                                                                              THEN -- escolas sem itens
						                                                                                              sum(coalesce(se.sesquantidadetecnico,0))
						                                                                              ELSE -- escolas com itens
						                                                                                              CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
						                                                                                                              sum(coalesce(ssi.seiqtdtecnico,0))
						                                                                                              END
						                                                                              END
						                                                               as vlrsubacao
						                                               FROM entidade.entidade t
						                                               inner join entidade.funcaoentidade f on f.entid = t.entid
						                                               left join entidade.entidadedetalhe ed on t.entid = ed.entid
						                                               inner join entidade.endereco d on t.entid = d.entid
						                                               left join territorios.municipio m on m.muncod = d.muncod
						                                               left join par.escolas e on e.entid = t.entid
						                                               INNER JOIN par.subacaoescolas se ON se.escid = e.escid
						                                               INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						                                               LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						                                               WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
						
						                                               and sic.icoid = si.icoid
						                                               and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
						                                               GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
						                END ) as quantidade
																																						  
							from par.subacao s
							INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
							INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
							INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
							INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
							INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
							LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
							WHERE 
							si.icoid = {$icoId}
							";
			
			$qtd = $db->carregar($sqlItem);
			
			$qtd = $qtd[0]['quantidade'];
	
			// Caso a quantidade informada seja maior que a do termo não aceita
			if( $valorItens > $qtd )
			{
				//
				echo '<script> alert("O quantidade informado nas escolas é superior a quantidade informada no Termo")</script>';
				echo '<script> window.close();</script>';
				$db->rollback();
				die();
				
			}
			else
			{
                            
                            #Selecionar Item composicao
                            $sql = "INSERT INTO par.historicosubacaoitenscomposicao(
                                        icoid, icoano, icodescricao, icodatamonitoramento, icousucpfmonitoramento, 
                                        icoquantidade, icoquantidadetecnico, icoquantidaderecebida, icovalor
                                    )
                                    SELECT 
                                        icoid, icoano, icodescricao, icodatamonitoramento, icousucpfmonitoramento, 
                                        (select coalesce(SUM(seiqtd), 0) from par.subescolas_subitenscomposicao where icoid = {$icoId}), 
                                        (select coalesce(SUM(seiqtdtecnico), 0) from par.subescolas_subitenscomposicao where icoid = {$icoId}), icoquantidaderecebida, icovalor
                                    FROM par.subacaoitenscomposicao
                                    WHERE icoid = {$icoId};";
                            $db->executar($sql);
                            
                            // Caso tenha atribuido novamente valor para o item reativa ele.
                            $ativa = ( $valor > 0 ) ? ",icomonitoramentocancelado = false" : '';  

                            $valorItens = ($valorItens) ? $valorItens : 'null';
                            // Atualiza as informações do item na tabela par.subacaoitenscomposicao
                            $sqUpdate = "

                                    UPDATE	
                                            par.subacaoitenscomposicao
                                    SET
                                            icoquantidaderecebida	= {$valorItens}
                                            ,icousucpfmonitoramento	= '{$_SESSION['usucpf']}'
                                            ,icodatamonitoramento	= 'now()'
                                            $ativa

                                    WHERE
                                            icoid 			= {$icoId}
                            ";

                            $db->executar($sqUpdate);	
			}
			$db->commit();
			// Sucesso
			echo '<script> alert("Quantidade recebida salva com sucesso!")</script>';
			echo '<script> window.close();</script>';
			die();
	}
	else 
	{
			
			echo '<script> alert("Não existem escolas para serem salvas")</script>';
			echo '<script> window.close();</script>';
			die();
	}
	
	
	
	
}
$sbaid = $_REQUEST['sbaid'];
if(!$sbaid){
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
}

if($_POST['requisicao']){
	
	$_POST['requisicao']();
}



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
	    <script>

			$(window).unload( function () { window.parent.opener.location.reload() } );
		
		function preencheConformeInformado()
		{
			$('input[type=hidden]').each(function() {
				
				var idHidden = this.id;
					if(idHidden.toLowerCase().indexOf("qtdinformada_") >= 0)
					{

						var idRelacionado = 'qtdrecebida_' + idHidden.replace("qtdinformada_","");
						var valorInformado = $('#'+idHidden).val();
						
						if(valorInformado)
						{
							$('#'+idRelacionado).val(valorInformado);
						}
						else
						{
							$('#'+idRelacionado).val('');
						}
						
					}
					
				})
		}
		</script>
	</head>
	<body>
		<script type="text/javascript">
			function salvarEscolas()
			{
				
				$("#form_subacao").submit();
				
			}
		  
		</script>
		<form name="form_subacao" id="form_subacao" method="post" action="" >
			<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
			<input type="hidden" name="requisicao" value="salvarEscolas" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Escolas</strong></td>
				</tr>
				<tr>
					<td colspan="2">
					<?php if($_SESSION['par']['itrid'] == 1):
						$cols = 4;
						 $arrTamTd = array(25,25,20,15,15);
											
						$cabecalho = array("Município", "Escola","Código INEP","Quantidade","Quantidade recebida");
						$sql = "	select 
										m.mundescricao as municicpio,
										t.entnome as nome_escola,
										
										t.entcodent as cod_inep,
										(
											SELECT 
												sum(seiqtdtecnico)
											FROM 
												par.subacaoescolas ses
											inner join par.subescolas_subitenscomposicao ssc ON ssc.sesid = ses.sesid 
											WHERE
												sbaid = '{$_GET['sbaid']}'
											AND
												ses.escid = e.escid
											and 
								            	ses.sesstatus = 'A'
								            and
									        	ses.sesano = '{$_GET['ano']}'
									        and
									        	ses.sbaid = '{$_GET['sbaid']}'
									        AND 
									        	ssc.icoid = {$_GET['icoid']}
									        
									
										) as quantidade,
											(	SELECT 
												mieqtdrecebida
											from 
												par.monitoramentoitensescolas 
											where 
												escid = e.escid
											AND
											icoid = {$_GET['icoid']} 
										)	as valor,
										e.escid
									from
										entidade.entidade t
									inner join 
										entidade.funcaoentidade f on f.entid = t.entid
									left join
									entidade.entidadedetalhe ed on t.entid = ed.entid
									and (
									     entdreg_infantil_creche = '1' or
									     entdreg_infantil_preescola = '1' or
									     entdreg_fund_8_anos = '1' or
									     entdreg_fund_9_anos = '1'
											)
									inner join
									entidade.endereco d on t.entid = d.entid
									inner join territorios.municipio m on d.muncod = m.muncod
									left join 
									par.escolas e on e.entid = t.entid
									WHERE
										(t.entescolanova = false or t.entescolanova is null)
									AND 
										t.entstatus = 'A'
									and 
									f.funid = 3
									AND
							            t.tpcid = 1
							        and
							        	m.estuf = '{$_SESSION['par']['estuf']}'
							        order by
							        	m.mundescricao,
							        	t.entnome"; 

						

						?>
					<?php else: ?>
						<?php 
						$arrTamTd = array(25,25,20,15,15);
						$cols = 4; 
						$cabecalho = array("Município", "Escola","Código INEP","Quantidade","Quantidade recebida");
						$tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3;
						
						
						$sql = "
							select 
								m.mundescricao as municicpio,
								t.entnome as nome_escola,
								t.entcodent as cod_inep,
								(
									SELECT 
										sum(seiqtdtecnico)
									FROM 
										par.subacaoescolas ses
									inner join par.subescolas_subitenscomposicao ssc ON ssc.sesid = ses.sesid 
									WHERE
										sbaid = '{$_GET['sbaid']}'
									AND
										ses.escid = e.escid
									and 
						            	ses.sesstatus = 'A'
						            and
							        	ses.sesano = '{$_GET['ano']}'
							        and
							        	ses.sbaid = '{$_GET['sbaid']}'
							        AND 
							        	ssc.icoid = {$_GET['icoid']}
							        
							
								) as quantidade,
								(	SELECT 
										mieqtdrecebida
									from 
										par.monitoramentoitensescolas 
									where 
										escid = e.escid
									AND
									icoid = {$_GET['icoid']} 
								)	as valor,
								e.escid
							from
								entidade.entidade t
							inner join 
								entidade.funcaoentidade f on f.entid = t.entid
							left join
							entidade.entidadedetalhe ed on t.entid = ed.entid
							and (
							     entdreg_infantil_creche = '1' or
							     entdreg_infantil_preescola = '1' or
							     entdreg_fund_8_anos = '1' or
							     entdreg_fund_9_anos = '1'
									)
							inner join
							entidade.endereco d on t.entid = d.entid
							inner join territorios.municipio m on d.muncod = m.muncod
							left join 
							par.escolas e on e.entid = t.entid
							WHERE
								(t.entescolanova = false or t.entescolanova is null)
							AND 
								t.entstatus = 'A'
							and 
							f.funid = 3
							AND
							t.tpcid = {$tcpid}
							AND
							d.muncod = '{$_SESSION['par']['muncod']}'
						";
						
					
					endif;
					
					$habilitado = 'S';
					
					?>
					<?php $arrEscolas = $db->carregar($sql) ?>
					<?php if($arrEscolas): ?>
						<table class="listagem" width="100%" >
							<thead>
								<tr class="SubTituloTabela">
									<?php foreach($cabecalho as $chave => $cab): ?>
										<td width="<?php echo $arrTamTd[$chave] ?>%" class="bold" ><?php echo $cab ?></td>
									<?php endforeach; ?>
								</tr>
							</thead>
							</table>
							<div id="tbl_escolas" >
								<table class="listagem" width="100%" >
									
									<tr bgcolor="#f7f7f7">
										<td colspan="<?=$cols; ?>"></td>
										<td width="<?php echo $arrTamTd[$escNum] ?>%" align="center" >
											<input type="button" onclick="preencheConformeInformado()" value="Bens/Serviços conforme termo" id="btnconformetermo">
										</td>
									</tr>										
									<?php $num = 0; ?>
									<?php foreach($arrEscolas as $escola): ?>
									
										<?php $cor = $num%2 == 1 ? "#f7f7f7" : "#FFFFFF"?>
										<tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
										<?php $escNum = 0; ?>
										<?php foreach($escola as $chave => $esc): ?>
											<?php if($chave == "valor"){continue;}?>
											
											<?php if($chave == 'escid'){ ?>
												<input type="hidden" name="escid[]" id="escid[]" value="<?=$escola['escid'];?>">
											<?php } else { ?>
											
											<?php if($chave == "quantidade"){?>
											
											 
												<td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" >
													<?php echo  $esc?>
													<input type="hidden" value="<?php echo $esc;?>" name="qtdinformada_<?php echo $escola['escid'];?>" id="qtdinformada_<?php echo $escola['escid'];?>" >
												</td>
												
												<td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" >
													<?php echo" <input maxlength='8' type='text' name='qtdrecebida[]' id='qtdrecebida_{$escola['escid']}' value='{$escola['valor']}'> ";?>
													
												</td>
												
											<?php }
												else
												{
											?>
												<td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" > <?php echo  $esc?> </td>
											<?php 
												}?>
											
											
											<?php } ?>
											<?php $escNum++; ?>
										<?php endforeach; ?>
										</tr>
										<?php $num++; ?>
									<?php endforeach; ?>
									<?php echo "<input type=\"hidden\" value=\"{$totalAntes}\" id='total_antes' name='total_antes'>"; ?>
								</table>
							</div>
					<?php endif; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita"></td>
					<td class="SubTituloEsquerda" >
					<?php
				
					 if($habilitado == 'S'){
					?>
					<input type="button" value="Salvar" name="btn_salvar" onclick="salvarEscolas()" />
					<?php }?>
					<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>