<?php
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvSubac.class.inc');
class HistoricoConvItemComposicao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicoconvitemcomposicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hciid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hciid' => null, 
									  	'hmsid' => null, 
									  	'hmsquantidadeempenhada' => null, 
									  	'hmsquantidadeliquidada' => null, 
									  	'hmsquantidadepaga' => null, 
									  	'hmsvalorunitario' => null, 
									  	'hmsvalortotalempenhado' => null, 
									  	'hmsvalortotalliquidado' => null, 
									  	'hmsvalortotalpago' => null, 
									  	'scsid' => null, 
									  	'micid' => null, 
									  	'hmsobs' => null, 
									  	'cosid' => null, 
    									'hmsano' => null,
    									'hcitravado' => null,
    									'hieid' => null,
									  );
									  
	public function recuperaDadosItensComposicao($hmsid){
		$sql = "SELECT hciid, scsid, hmsobs, hmsano
				FROM cte.historicoconvitemcomposicao 
				WHERE  hmsid = ".$hmsid; 
		
		return $this->carregar($sql);
	}
	
	/**
	 * function carregaItensComposicao();
	 * @desc   : Faz a busca no banco e carrega a lista de itens de composição de cada Subação. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $ssuid (id do status da subação)
	 * @param  : numeric $sbaid (id da subação)
	 * @return : string $conteudo (lista de itens de composição)
	 * @since 17/03/2009
	 */
	function carregaItensComposicao($sbaid, $ssuid, $prsid, $hmsid, $ano, $hmcid, $indicePai){
		$dadosMonitoramentos = new HistoricoMonitoramentoConvenio();
		$dadosMonitoramentos->carregarPorId($hmcid);
		
		$sql="  SELECT 	cs.cosid,
						null as hieid, 
						cs.cosdsc,
						cs.cosqtd, cs.cosvlruni, umd.undddsc, cs.sbaid, coalesce(h.hmsid,0) as  hmsid,
						coalesce(hi.hciid,0) as hciid,  coalesce(hi.scsid,0) as scsid 
					   , hmsvalorunitario, hmsquantidadepaga, hmsvalortotalpago 
					   ,  to_char(hmcdatamonitoramento, 'MM') as mes , hmcstatus
					   , scs.scsdesc
					   , coalesce(hic.tot_hmsvalortotalpago, 0) AS tot_hmsvalortotalpago
					   , coalesce(hic.tot_hmsquantidadepaga, 0) AS tot_hmsquantidadepaga
				FROM cte.composicaosubacao cs 
				INNER JOIN cte.unidademedidadetalhamento umd ON umd.unddid = cs.unddid 
				LEFT JOIN cte.historicomonitoramentoconvsubac h ON h.sbaid = cs.sbaid AND h.hmsid = ".$hmsid."
				LEFT JOIN cte.historicoconvitemcomposicao hi ON hi.hmsid = h.hmsid AND hi.cosid = cs.cosid AND h.hmsid = ".$hmsid."
				LEFT JOIN ( 
						     SELECT 
						    	SUM( hmsquantidadepaga ) AS tot_hmsquantidadepaga,
								SUM( hmsvalortotalpago ) AS tot_hmsvalortotalpago, 
								cosid
						    FROM
								cte.historicoconvitemcomposicao i
								inner join cte.historicomonitoramentoconvsubac s on s.hmsid = i.hmsid
								inner join cte.historicomonitoramentoconvenio hc on hc.hmcid = s.hmcid
						    WHERE
								i.hmsano = " . $ano . "
								and  hc.hmcdatamonitoramento <= '".$dadosMonitoramentos->hmcdatamonitoramento."'
						    GROUP BY
								i.cosid
					  	  ) hic ON hic.cosid = cs.cosid
				LEFT JOIN cte.statuscomposicaosubacao scs ON scs.scsid = hi.scsid
				LEFT JOIN cte.historicomonitoramentoconvenio hmc ON hmc.hmcid = h. hmcid
				WHERE cs.sbaid = ".$sbaid." 
				AND cs.cosano = ".$ano."
				
				UNION ALL

				SELECT 
					null as cosid,
					hie.hieid, 
					hie.hiedescricao as cosdsc,		
					hie.hieqtd as cosqtd, 
					hie.hievalorunitario as cosvlruni, 
					undddsc, 
					hie.sbaid, 
					hie.hmsid,
					coalesce(hi.hciid,0) as hciid,
					scs.scsid,
					hmsvalorunitario, 
					hmsquantidadepaga, 
					hmsvalortotalpago,
					to_char(hiedata, 'MM') as mes, 
					hmcstatus,
					scsdesc,
					tot_hmsvalortotalpago,
					tot_hmsquantidadepaga
				FROM cte.historicoitemexecutado hie
				INNER JOIN cte.unidademedidadetalhamento umd ON umd.unddid = hie.unddid
				LEFT JOIN cte.historicomonitoramentoconvsubac h ON h.sbaid = hie.sbaid AND h.hmsid = ".$hmsid."
				LEFT JOIN cte.historicoconvitemcomposicao hi ON hi.hmsid = hie.hmsid AND hi.hieid = hie.hieid AND hi.hmsid = ".$hmsid."
				LEFT JOIN ( 
						     SELECT 
						    	SUM( hmsquantidadepaga ) AS tot_hmsquantidadepaga,
								SUM( hmsvalortotalpago ) AS tot_hmsvalortotalpago, 
								hieid
						    FROM
								cte.historicoconvitemcomposicao i
								inner join cte.historicomonitoramentoconvsubac s on s.hmsid = i.hmsid
								inner join cte.historicomonitoramentoconvenio hc on hc.hmcid = s.hmcid
						    WHERE
								i.hmsano = " . $ano . "
								and  hc.hmcdatamonitoramento <= '".$dadosMonitoramentos->hmcdatamonitoramento."'
						    GROUP BY
								i.hieid
					  	  ) hic ON hic.hieid = hie.hieid
				LEFT JOIN cte.statuscomposicaosubacao scs ON scs.scsid = hi.scsid				
				LEFT JOIN cte.historicomonitoramentoconvenio hmc ON hmc.hmcid = h.hmcid				
				WHERE hie.sbaid = ".$sbaid." 
				AND hie.hiedata <= '".$dadosMonitoramentos->hmcdatamonitoramento."'";	
		
		//dbg($sql,1);
		
		$itens = $this->carregar($sql);
		$conteudo = $this->getItensComposicao($itens, $prsid, $ssuid, $hmcid, $indicePai, $ano, $sbaid, $hmsid);
		return $conteudo; 
		
	}
	
	/**
	 * function getItensComposicao();
	 * @desc   : Monta HTML com a lista de itens de composição
	 * @author : Thiago Tasca Barbosa
	 * @param  : array $itens (array de itens de composição)
	 * @return : string $conteudo (HTML contendo a lista de itens de composição)
	 * @since 17/03/2009
	 */
	function getItensComposicao($itens, $prsid, $ssuid, $hmcid, $indicePai, $ano = null, $sbaid = null, $hmsid = null){

		$conteudo = '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<th colspan="13" >Itens de composição</th>
						</tr>
						<tr>
							<th rowspan="2">Descrição dos Itens de composição</th>
							<th rowspan="2">Unidade de Medida</th>
							<th rowspan="2">Intem adicionado <br/> na execução</th>
							<th colspan="3" style=" background-color:#7BA2B6;" >Programado</th>
							<th colspan="3" style=" background-color:#FABF40;" >Executado no mês</th>
							<th colspan="2" style=" background-color:#50A56C;">Executado acumulado</th>
							<th rowspan="2">Status do Item de Composição</th>
							<th rowspan="2">Foi monitorado</th>
						</tr>
						<tr>
							<th style=" background-color:#7BA2B6;" >Qtd.</th>
							<th style=" background-color:#7BA2B6;" >R$ Unitário</th>
							<th style=" background-color:#7BA2B6;" >R$ Total</th>
							<th style=" background-color:#FABF40;" >Qtd. contratada paga</th>
							<th style=" background-color:#FABF40;" >R$ Unitário contratado</th>
							<th style=" background-color:#FABF40;" >R$ Total contratado pago</th>
							<th style=" background-color:#50A56C;" >Qtd. Total</th>
							<th style=" background-color:#50A56C;" >R$ Total</th>
						</tr>
					';
		$cont = 0;
		if(is_array($itens)){
			$countItens  	  = 0;
			$indiceFilho 	  = 1;
			foreach($itens as $item){
				
				if($cont == 0){
					$cor = "#e6e6e6";
					$cor1 = "#617E8D";
					$cor2 = "#E1AB38";
					$cor3 = "#427F56";
					$cont= 1;
				}else{
					$cor = "#ffffff";
					$cor1 = "#7BA2B6";
					$cor2 = "#FABF40";
					$cor3 = "#50A56C";
					$cont= 0;
				}
				
				if( $item["hmcstatus"]  == 'F' ){
					$style = 'style=""';
					$imagem = "../../imagens/check_p.gif";
					$texto = "Item monitorado";
				}else{
					if($item["scsid"] == STATUSNAOSELECIONADOITENS){
						$style = 'style="display:none;"';
						$imagem = "../../imagens/atencao.png";
						$texto = "Item não monitorado";
					}else{
						$style = 'style=""';
						$imagem = "../../imagens/check_p.gif";
						$texto = "Item monitorado";
					}
				}
				
				// FORMATANDO OS DADOS
				$valorTotalItem 			= $item["cosqtd"] * $item["cosvlruni"];
				$valorTotalItemT 			= $item["cosqtd"] * $item["cosvlruni"];
				$valorTotalItem 			= number_format($valorTotalItem, 2, ',', '.');
				$valorUnitario 				= number_format($item["cosvlruni"], 2, ',', '.');
				$quantidade 				= number_format($item["cosqtd"], 0, '', '');
				$valorUnitarioPago 			= number_format($item["hmsvalorunitario"], 2, ',', '.');
				$valorUnitarioTotalPago 	= number_format($item["hmsvalortotalpago"], 2, ',', '.');
				$valorPago 					= number_format($item["tot_hmsvalortotalpago"], 2, ',', '.');
				$quantidadePago 			= number_format($item["hmsquantidadepaga"], 0, '', '');
				// FIM FORMAT DADOS
				
				if( $item["hmcstatus"]  == 'F' ){
					$onclickBtn = ' onclick = "erro(6);"';
				}else{
					if($hmcid){
						if($item['hmsid'] != 0){
							if($ssuid == NAOINICIADA || $ssuid == CANCELADA){
								$onclickBtn = ' onclick = "erro(2);"';
							}else if ( $ssuid == STATUSNAOSELECIONADO ){
								$onclickBtn = ' onclick = "erro(4);"';
							}else{
								if($item["cosid"]){
									$itemId = "cosid={$item["cosid"]}";
								}else{
									$itemId = "hieid={$item["hieid"]}";
								}
								$onclickBtn = ' onclick = "inicialytebox(\'cte.php?modulo=principal/monitoraFinanceiro/monitora_itensComp&acao=A&'.$itemId.'\&sbaid='.$item["sbaid"].'\&hmsid='.$item["hmsid"].'\&hciid='.$item["hciid"].'\&quant='.$item["cosqtd"].'\&valor='.$item["cosvlruni"].'\' , \'Monitora Itens Composição\',\'800\',\'550\');"';
							}
						}else{
							$onclickBtn = ' onclick = "erro(4);"';
						}
					}else{
						$onclickBtn = ' onclick = "erro(3);"';
					}
				}
				
				$conteudo .=	'<tr>																																																								
									<td style="background-color:'.$cor.';" ><a style="cursor:pointer;" id="Editar_'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" '.$onclickBtn.' >'.$indicePai.'.'.$indiceFilho.' - '.$item["cosdsc"].'</a></td>
									<td style="background-color:'.$cor.';">'.$item["undddsc"].'</td>
									<td style="background-color:'.$cor.';" align="center">'.($item['hieid'] ? "<img src=\"../../imagens/check_p.gif\" />" : "").'</td>
									<td style="background-color:'.$cor1.';">'.$quantidade.'</td>
									<td style="background-color:'.$cor1.';">R$ '.$valorUnitario.'</td>
									<td style="background-color:'.$cor1.';">R$ '.$valorTotalItem.'</td>
									<td id="tdPago1_'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" style="background-color:'.$cor2.';">'.$quantidadePago.'</td> 
									<td id="tdPago2_'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" style="background-color:'.$cor2.';">R$ '.$valorUnitarioPago.'</td>
									<td id="tdPago3_'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" style="background-color:'.$cor2.';">R$ '.$valorUnitarioTotalPago.'</td>
									<td id="td_qtd_executado_acumulado_' . ($item["cosid"] ? $item["cosid"] : $item["hieid"]."n") . '" style="background-color:'.$cor3.';">' . $item['tot_hmsquantidadepaga'] . '</td>
									<td id="td_valor_executado_acumulado_' . ($item["cosid"] ? $item["cosid"] : $item["hieid"]."n") . '" style="background-color:'.$cor3.';">' . $valorPago . '</td>
									<td id="td_status_item_' . ($item["cosid"] ? $item["cosid"] : $item["hieid"]."n") . '" style="background-color:'.$cor.';">'.$item['scsdesc'].'</td>
									<td id="td_'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" style="background-color:'.$cor.';"> <a style="cursor:pointer;" id="Editar_'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" '.$onclickBtn.' ><img title="'.$texto.'" id="'.($item["cosid"] ? $item["cosid"] : $item["hieid"]."n").'" align="absmiddle"  src="'.$imagem.'" width="18" height="18"></a></td>
								</tr>
								 ';
				
				// Não soma itens novos para o total previsto
				if(!$item["hieid"]){
					$totalVlrI +=  $valorTotalItemT;
				}
				$totalUniTotalP 	+=  $item["hmsvalortotalpago"];
				$totalExecAcumulado +=  $item["tot_hmsvalortotalpago"];
				
				$indiceFilho++;
				
				// Verifica se existe resto
				if($valorTotalItemT > $item["tot_hmsvalortotalpago"]){
					$boResto = true;
				}
								
				// Pega valores para o calculo do novo item
				if($item['scsdesc'] == 'Cancelada'){
					$sobraCancelada 	 += $item["tot_hmsvalortotalpago"];
				}elseif($item['scsdesc'] == 'Executado' && $boResto){
					$sobraExecutado 	 += $valorTotalItemT-$item["tot_hmsvalortotalpago"];
				}elseif($item['scsdesc'] == 'Em execução' && $boResto){
					$sobraEmExecucao 	 += $valorTotalItemT-$item["tot_hmsvalortotalpago"];
				}elseif($item['scsdesc'] == 'Em planejamento' && $boResto){
					$sobraEmPlanejamento += $valorTotalItemT-$item["tot_hmsvalortotalpago"];
				}elseif($item['scsdesc'] == '' || $item['scsdesc'] == 'Não iniciada'){
					$sobraNaoIniciado 	 += $valorTotalItemT-$item["tot_hmsvalortotalpago"];
				}
				unset($boResto);
				$countItens++;
			}
		}
		
		// Calculo do valor disponível para novo item
		$valorDisponivel = (($totalVlrI-$totalExecAcumulado)-($sobraEmExecucao+$sobraEmPlanejamento))-$sobraNaoIniciado;
		
		$sql = "select 
					hmdmes, 
					hmdano 
				from cte.historicomonitoramentodadosbancarios hmd
				inner join cte.historicomonitoramentoconvenio hmc on hmd.hmdid = hmc.hmdid
				where hmd.prsid = {$prsid}
				and hmc.hmcstatus = 'I'";
		
		$arMesAno = $this->pegaLinha($sql);
		
		$dadosMonitoramentos = new HistoricoMonitoramentoConvenio();
		$dadosMonitoramentos->carregarPorId($hmcid);
		$arDataMonitoramento = explode('-',$dadosMonitoramentos->hmcdatamonitoramento);
		
		// Verifica mês/ano do monitoramento aberto
		if($arMesAno['hmdmes'] == $arDataMonitoramento[1] && $arMesAno['hmdano'] == $arDataMonitoramento[0]){
			
			// Verifica se existe valor disponível para os itens
			if(round($valorDisponivel, 2) > 0){
				$addItem = '<tr>
								<td colspan="12">
									<img src="../imagens/gif_inclui.gif" align="absmiddle" />
									<a onclick = "inicialytebox(\'cte.php?modulo=principal/monitoraFinanceiro/adiciona_itensComp&acao=A&sbaid='.$sbaid.'&hmsid='.$hmsid.'&disponivel='.$valorDisponivel.'&ano='.$ano.'&hmcid='.$hmcid.'\' , \'Adiciona Itens Composição\',\'800\',\'550\');" href="javascript:void(0)">
									<b>Adicionar</b></a> Disponível: R$ <span id="total_disponivel">'.formata_valor($valorDisponivel).'</span>
								</td>
							</tr>';
			}
		}
		
		$conteudo .='		<tr>
								<th style="text-align: right;" >Total:</th>
								<th> </th>
								<th> </th>
								<th style="text-align:left;"></th>
								<th style="text-align:left;"></th>
								<th id="td_total_previsto" style="text-align:left;"> R$ '.number_format($totalVlrI, 2, ',', '.').'</th>
								<th style="text-align:left;"></th>
								<th style="text-align:left;"></th>
								<th style="text-align:left;"> R$ '.number_format($totalUniTotalP, 2, ',', '.').'</th>
								<th>&nbsp;</th>
								<th id="td_total_acumulado" style="text-align:left;">R$ '.number_format($totalExecAcumulado, 2, ',', '.').'</th>
								<th>&nbsp;</th>
								<th>&nbsp;</th>
							</tr>
							'.$addItem.'
					</table>';
		
		return $conteudo;
		
	}
	
	public function recuperaItensComposicao($hmsid, $sbaid, $ano){
			$sql = "SELECT cs.cosid, hi.hciid, hi.scsid, hi.hmsobs, hi.hmsano, hi.hcitravado
					FROM cte.composicaosubacao  cs 
					LEFT JOIN cte.historicoconvitemcomposicao hi on hi.cosid = cs.cosid AND  hi.hmsid = ".$hmsid." AND hmsano = ".$ano."
					WHERE cs.sbaid = ".$sbaid." AND cs.cosano = ".$ano; 
			
			return $this->carregar($sql);
	}
	
	public function recuperaHmcidPorItem($hciid){
		$sql = "select hm.hmcid
				from cte.historicoconvitemcomposicao hi
				inner join cte.historicomonitoramentoconvsubac hs on hi.hmsid = hs.hmsid 
				inner join cte.historicomonitoramentoconvenio hm on hm.hmcid = hs.hmcid
				where hi.hciid = ".$hciid;
		return $this->pegaUm($sql);
	}
	
	/**
	 * function recuperaHistoricoMonitoramentoPagItens($cosid);
	 * @desc   : carrega lista de historico de monitoramento do item
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $cosid (id do item de composição)
	 * @return : array $acaind (resultado da consulta)
	 * @since 24/03/2009
	 */
	function recuperaHistoricoMonitoramentoPagItens($cosid, $ano = NULL){
		if($ano == NULL){
			$ano = 0;
		}
		$sql = "SELECT  c.hmcdatamonitoramento,
					sc.scsdesc,
					i.hmsquantidadeempenhada, 
					i.hmsquantidadeliquidada, 
					i.hmsquantidadepaga, 
					coalesce(i.hmsvalorunitario,0) as hmsvalorunitario, 
					i.hmsvalortotalempenhado,
					i.hmsvalortotalliquidado, 
					i.hmsvalortotalpago
				FROM cte.historicoconvitemcomposicao i
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
				INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
				LEFT JOIN cte.statuscomposicaosubacao sc ON sc.scsid = i.scsid
				WHERE cosid = ".$cosid." and i.hmsano = ".$ano." ORDER BY hciid asc";
	//dbg($sql,1);
		$listaHistorico = $this->carregar($sql);
		return $listaHistorico;
	}
	
	/**
	 * function recuperaValorTotalAcumuladoDoItem($cosid);
	 * @desc   : carrega o valor total acumulado dos meses do item.
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $cosid (id do item de composição)
	 * @return : array $acaind (resultado da consulta)
	 * @since 24/03/2009
	 */
	function recuperaValorTotalAcumuladoDoItem($cosid = null, $hieid = null){
		
		if(!$hieid){
			$stWhere = "WHERE cosid = {$cosid}";
		}else{
			$stWhere = "WHERE hieid = {$hieid}";
		}
		
		$sql = "SELECT  sum(i.hmsvalortotalpago) as total
				FROM cte.historicoconvitemcomposicao i
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
				INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
				LEFT JOIN cte.statuscomposicaosubacao sc ON sc.scsid = i.scsid
				{$stWhere} 
				and i.hciid not in (SELECT  max(i.hciid)
									FROM cte.historicoconvitemcomposicao i
									INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
									INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
									LEFT JOIN cte.statuscomposicaosubacao sc ON sc.scsid = i.scsid
									{$stWhere})";

		$valor = $this->pegaUm($sql);
		return $valor;
	}
	
	function recuperaQtdTotalAcumuladoDoItem($cosid = null, $hieid = null, $hmsano, $hciid){
		
		if(!$hieid){
			$stWhere = " AND cosid = {$cosid} ";
		}else{
			$stWhere = " AND hieid = {$hieid} ";
		}
		
		if(!$hciid){
			return false;	
		}
		$sql = "SELECT					
							sum(i.hmsquantidadeempenhada) as hmsquantidadeempenhada, 
							sum(i.hmsquantidadeliquidada) as hmsquantidadeliquidada, 
							sum(i.hmsquantidadepaga) as hmsquantidadepaga							
						FROM cte.historicoconvitemcomposicao i
						INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
						INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
						LEFT JOIN cte.statuscomposicaosubacao sc ON sc.scsid = i.scsid
						WHERE i.hmsano = ".$hmsano." and hciid != '".$hciid."' {$stWhere}";
				
		$qtdAcumulada = $this->carregar($sql);
		return $qtdAcumulada;
		
	}
	
	/**
	 * function recuperaDadosPopupItensComp($sbaid, $cosid);
	 * @desc   : Recupera a lista de dados do Banco de dados.
	 * 			 Está função recupera os dados para ser mostrado 
	 * 			 na tela de monitoramento de itens de composição 
	 * 			 listando todos os dados des da dimensão até o item de composição.
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $sbaid (id do status)
	 * @param  : numeric $cosid (id do item de composição)
	 * @return : array $acaind (resultado da consulta)
	 * @since 24/03/2009
	 */
	function recuperaDadosPopupItensComp($sbaid, $cosid = null, $hieid = null){
		
		if(!$hieid){
			$stCampo = "cs.cosdsc";
			$stInner = "INNER JOIN cte.composicaosubacao  	cs  ON cs.sbaid = s.sbaid";
			$stWhere = "AND cosid = {$cosid}";
		}else{
			$stCampo = "cs.hiedescricao";
			$stInner = "INNER JOIN cte.historicoitemexecutado  	cs  ON cs.sbaid = s.sbaid";
			$stWhere = "AND hieid = {$hieid}";			
		}
		
		$sql = "SELECT  d.dimcod,
						d.dimdsc,
						ad.ardcod,
						ad.arddsc,
						i.indcod,
						i.inddsc,
						s.sbadsc,
						{$stCampo}
			FROM cte.dimensao d
			INNER JOIN cte.areadimensao 	  	ad  ON ad.dimid = d.dimid
			INNER JOIN cte.indicador 	  		i   ON i.ardid  = ad.ardid
			INNER JOIN cte.criterio		  		c   ON c.indid  = i.indid
			INNER JOIN cte.pontuacao 	  		p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
			INNER JOIN cte.instrumentounidade 	iu  ON iu.inuid = p.inuid
			INNER JOIN cte.acaoindicador 	  	a   ON a.ptoid  = p.ptoid
			INNER JOIN cte.subacaoindicador   	s   ON s.aciid  = a.aciid
			{$stInner}
			WHERE s.sbaid = ".$sbaid." {$stWhere} ";
		$acaind 	= $this->pegaLinha($sql);
		return $acaind;
	}
	
	/**
	 * function statusMotivoItensComp($micid);
	 * @desc   : Retorna o combo com os motivos dos itens de composição.
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $micid (id do status)
	 * @since 24/03/2009
	 * @tutorial : Se existir $micid o monta_combo carrega como selecionado o motivo correto.
	 */
	function statusMotivoItensComp($micid){
		$sqlStatusMotivoItensComp = "SELECT micid   		AS codigo, 
								      		micdescricao 	AS descricao 
								 	 FROM cte.motivoitemcomposicao
								 	 ORDER BY micdescricao";
		$selectStatusMotivoItensComp = $this->monta_combo('micid',$sqlStatusMotivoItensComp,'S', 'Escolha...', '', '', '', '', 'S', 'micid', true, $micid);
		return $selectStatusMotivoItensComp;
	}
	
	/**
	 * function carregaComboStatusItensComposicao();
	 * @desc   : Monta HTML com a lista de itens de composição
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $statusItem (id status do item de composição)
	 * @param  : numeric $iditem (id do item de composição)
	 * @return : string $selectStatusItensComp (HTML com o combo de status do item de composição)
	 * @since 17/03/2009
	 */
	function carregaComboStatusItensComposicao($statusItem, $cosid = null, $ssuid, $statusPassado = NULL, $hieid = null){
		$scsid = $statusItem = $statusItem == NULL ? 0 : $statusItem; 
		$statusPassado = $statusPassado == NULL ? 0 : $statusPassado; 
		if($ssuid == EXECUTADA){// Se for executada o item de composição so pode ter status de executado ou cancelado.
			$where = " WHERE scsid not in ('".EMEXECUÇÃOITEMCOMP."', '".NAOINICIADAITEMCOMP."', '".EMPLANEJAMENTOITEMCOMP."') ";
			if($statusPassado == EMEXECUÇÃOITEMCOMP){
				$where = " WHERE scsid in ('".EXECUTADOITEMCOMP."','".EMEXECUÇÃOITEMCOMP."') ";
			}
		}else{
			$where = $this->verificaMonitoramentoItemMesAnterior($cosid, $hieid);
		}
		
		$sqlStatusItensComp = "SELECT scsid   AS codigo, 
							        scsdesc AS descricao 
							 FROM cte.statuscomposicaosubacao 
							 ".$where."
							 ORDER BY scsdesc";
		$selectStatusItensComp = $this->monta_combo('scsid',$sqlStatusItensComp,'S', 'Escolha...', 'carregaEditarItensComposicao(this.value);', '', '', '', '', 'scsid', true, $scsid);
		return $selectStatusItensComp;
	}
	
	function verificaMonitoramentoItemMesAnterior($cosid = null, $hieid = null){
		if(!$hieid){
			$stWhere = " AND cosid = {$cosid}";
		}else{
			$stWhere = " AND hieid = {$hieid}";
		}
		$sql = "SELECT  scsid
				FROM cte.historicoconvitemcomposicao i
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
				INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
				WHERE c.hmcstatus = 'F' {$stWhere}
				ORDER BY i.hciid DESC LIMIT 1";

		$id = $this->pegaUm($sql);
		$where = " ";
		if($id == EXECUTADOITEMCOMP ){
			$where = " WHERE scsid not in ('".EMEXECUÇÃOITEMCOMP."', '".NAOINICIADAITEMCOMP."', '".EMPLANEJAMENTOITEMCOMP."') ";
		}else if($id == EMEXECUÇÃOITEMCOMP){
			$where = " WHERE scsid not in ('".CANCELADAITEMCOMP."', '".NAOINICIADAITEMCOMP."', '".EMPLANEJAMENTOITEMCOMP."') ";
		}
		return $where;
	}
	
	function carregaItemAnteriorPorID($cosid = null, $ano, $hmsid, $hieid = null){
		
			if($cosid){
				
				$sql = "SELECT hciid, hcitravado , scsid
						FROM cte.historicoconvitemcomposicao 
						WHERE  cosid = ".$cosid." and hmsano =  ".$ano." and hmsid = ".$hmsid."
						order by hmsid desc limit 1";
				
				return $this->pegaLinha($sql);
				
			}elseif($hieid){
				
				$sql = "SELECT hciid, hcitravado , scsid
						FROM cte.historicoconvitemcomposicao 
						WHERE  hieid = ".$hieid." and hmsano =  ".$ano." and hmsid = ".$hmsid."
						order by hmsid desc limit 1";
				
				return $this->pegaLinha($sql);
				
			}else{
				
				return false;
			}
			
	}
}