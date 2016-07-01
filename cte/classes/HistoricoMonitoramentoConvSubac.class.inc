<?php
include_once(APPRAIZ .'cte/classes/HistoricoConvItemComposicao.class.inc');
class HistoricoMonitoramentoConvSubac extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicomonitoramentoconvsubac";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hmsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hmsid' => null, 
									  	'hmcid' => null, 
									  	'sbaid' => null, 
									  	'ssuid' => null,
    									'hmsjustificativa' => null,
    									'hmsano' => null,
    									'hmstravado' => null,
    									'pssid' => null
									  );
									  
	public function recuperaHmsid($prsid, $sbaid, $ano, $hmcid){
		$sql = "SELECT hmsid 
				FROM cte.historicomonitoramentoconvsubac 
				WHERE sbaid =".$sbaid."
				and hmsano = '".$ano."'
				AND hmcid = ".$hmcid;
		
		return $this->pegaUm($sql);
		
	}
	
	public function carregarPssid( $sbaid, $ano, $hmcid){
		
		$sql = "SELECT ps.pssid 
				from cte.projetosapesubacao ps
				inner join cte.projetosape p ON p.prsid = ps.prsid
				inner join cte.historicomonitoramentoconvenio c ON c.prsid = p.prsid
				left join cte.historicomonitoramentoconvsubac s on s.sbaid =  ps.sbaid and s.hmsano = ps.pssano and s.hmcid = c.hmcid
				WHERE ps.sbaid =".$sbaid."
				and ps.pssano = '".$ano."'";
		return $this->pegaUm($sql);
		
	}
	
	
	public function salvarItens($hmsid, $hmsidAnterior, $ano, $statusSub = NULL, $hmsjustificativa = NULL){
		$sqlSub = '';
		if($statusSub == EMEXECUCAO){
			$sqlSub = " AND scsid IN ( ".CANCELADAITEMCOMP.",".EXECUTADOITEMCOMP.")";
		}
			$sql = "SELECT  hmsquantidadeempenhada, hmsquantidadeliquidada, hmsquantidadepaga, hmsvalorunitario, 
							hmsvalortotalempenhado, hmsvalortotalliquidado, hmsvalortotalpago, scsid, micid, hmsobs, cosid, hieid 
					FROM cte.historicoconvitemcomposicao  i
					INNER JOIN cte.historicomonitoramentoconvsubac s ON i.hmsid = s.hmsid
					WHERE s.hmsid = ".$hmsidAnterior.$sqlSub;
			$dadosAnteriores = $this->carregar($sql);
			if(is_array($dadosAnteriores) ){
				foreach($dadosAnteriores as $dados){
					if($dados['scsid'] != NAOINICIADAITEMCOMP){
						$obHistItem	= new HistoricoConvItemComposicao();
						$obHistItem->hmsid = $hmsid;
						$obHistItem->scsid = $dados['scsid'];
						$obHistItem->micid = $dados['micid'];
						$obHistItem->cosid = $dados['cosid'];
						$obHistItem->hieid = $dados['hieid'];
						$obHistItem->hmsano = $ano;
						$obHistItem->hcitravado = 'f';
						if($dados['scsid'] == EXECUTADOITEMCOMP){
							$obHistItem->hmsquantidadeempenhada = 0; //$dados['hmsquantidadeempenhada'];
							$obHistItem->hmsquantidadeliquidada = 0; //$dados['hmsquantidadeliquidada'];
							$obHistItem->hmsquantidadepaga 		= 0; //$dados['hmsquantidadepaga'];
							$obHistItem->hmsvalorunitario 		= 0; //$dados['hmsvalorunitario'];
							$obHistItem->hmsvalortotalempenhado = 0; //$dados['hmsvalortotalempenhado'];
							$obHistItem->hmsvalortotalliquidado = 0; //$dados['hmsvalortotalliquidado'];
							$obHistItem->hmsvalortotalpago 		= 0; //$dados['hmsvalortotalpago'];
							$obHistItem->hcitravado = 't';
						}else if($dados['scsid'] == CANCELADAITEMCOMP){
							$obHistItem->hcitravado = 't';
							if($hmsjustificativa){
								$obHistItem->hmsobs 	= $hmsjustificativa;
							}else{
								$obHistItem->hmsobs 	= $dados['hmsobs'];
							}
						}
						if($obHistItem->salvar()){
							$obHistItem->commit();
						}else{
							$obHistItem->rollback();
						}
					}
				}	
			}else{
				return true;	
			}
	}
	
	function carregaSubacaoAnteriorPorID($sbaid, $ano, $hmcid){
		$sql = "SELECT hmcid, hmsid, hmstravado  
				FROM cte.historicomonitoramentoconvsubac 
				WHERE  sbaid = ".$sbaid." and hmsano =  ".$ano."
				-- and hmsid not in (	SELECT max(hmsid) FROM cte.historicomonitoramentoconvsubac WHERE  sbaid = ".$sbaid." and hmsano = ".$ano."  )
				order by hmsid desc limit 1";
		$monitoraAtual = $this->pegaLinha($sql);
		if($hmcid == $monitoraAtual['hmcid'] ){
			$sql = "SELECT hmcid, hmsid, hmstravado  
				FROM cte.historicomonitoramentoconvsubac 
				WHERE  sbaid = ".$sbaid." and hmsano =  ".$ano."
				 and hmsid not in (	SELECT max(hmsid) FROM cte.historicomonitoramentoconvsubac WHERE  sbaid = ".$sbaid." and hmsano = ".$ano."  )
				order by hmsid desc limit 1";
			return $this->pegaLinha($sql);
		}else{
			return $this->pegaLinha($sql);
		}
		
		
	}
	
	/**
	 * function carregaSubacoes($prsid, $ano, $ssuid);
	 * @desc   : Faz a busca no banco e carrega a lista de subações. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $ssuid (id do status da subação)
	 * @param  : numeric $ano (ano de referencia)
	 * @param  : numeric $prsid (id do convênio)
	 * @return : string $conteudo (lista de subacao)
	 * @since 17/03/2009
	 */
	function carregaSubacoes($prsid, $hmcid){
		$sql = "	SELECT  si.sbaid, 
							si.sbadsc,
							hs.ssuid,
							coalesce( hs.hmsid, 0) as hmsid,
							hc.hmcstatus,
							ss.ssudescricao,
							ps.pssano,
							ps.prsid,
							hc.hmcid,
							to_char(pssdata, 'MM') as pssmes,
							ps.pssid
					FROM cte.projetosape 		  p
					INNER JOIN cte.projetosapesubacao ps ON ps.prsid = p.prsid
					INNER JOIN cte.subacaoindicador   si ON si.sbaid = ps.sbaid
					INNER JOIN cte.historicomonitoramentoconvenio 	hc  ON hc.prsid  = p.prsid 
					LEFT JOIN cte.historicomonitoramentoconvsubac 	hs ON hs.hmcid = hc.hmcid AND hs.sbaid = ps.sbaid AND ps.pssano = hs.hmsano
					LEFT JOIN cte.statussubacao ss ON ss.ssutipostatus = 'P' and ss.ssuid = hs.ssuid
					WHERE p.prsid = ".$prsid." 
					AND hc.hmcid = ".$hmcid."
					AND to_char(ps.pssdata, 'YYYY-MM') <= to_char(hc.hmcdatamonitoramento, 'YYYY-MM') 
					ORDER BY ps.pssdata, si.sbadsc ASC
				";
		//dbg($sql,1);
		$subacoes = $this->carregar($sql);
		$conteudo = $this->getSubacoes($subacoes);
		
		return $conteudo; 
	}
	
	/**
	 * function carregaSubacoesResumo($prsid, $ssuid);
	 * @desc   : Faz a busca no banco e carrega a lista de subações. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $ssuid (id do status da subação)
	 * @param  : numeric $prsid (id do convênio)
	 * @return : string $conteudo (lista de subacao)
	 * @since 17/03/2009
	 */
	function carregaSubacoesResumo($prsid, $hmcid){
		$sql = "	SELECT  si.sbaid, 
							si.sbadsc,
							hs.ssuid,
							coalesce( hs.hmsid, 0) as hmsid,
							hc.hmcstatus,
							ss.ssudescricao,
							ps.pssano,
							ps.prsid,
							hc.hmcid,
							to_char(pssdata, 'MM') as pssmes,
							ps.pssid
					FROM cte.projetosape 		  p
					INNER JOIN cte.projetosapesubacao ps ON ps.prsid = p.prsid
					INNER JOIN cte.subacaoindicador   si ON si.sbaid = ps.sbaid
					INNER JOIN cte.historicomonitoramentoconvenio 	hc  ON hc.prsid  = p.prsid 
					LEFT JOIN cte.historicomonitoramentoconvsubac 	hs ON hs.hmcid = hc.hmcid AND hs.sbaid = ps.sbaid AND ps.pssano = hs.hmsano
					LEFT JOIN cte.statussubacao ss ON ss.ssutipostatus = 'P' and ss.ssuid = hs.ssuid
					WHERE p.prsid = ".$prsid." 
					AND hc.hmcid = ".$hmcid."
					AND to_char(ps.pssdata, 'YYYY-MM') <= to_char(hc.hmcdatamonitoramento, 'YYYY-MM') 
					ORDER BY ps.pssdata, si.sbadsc ASC
				";
		//dbg($sql,1);
		$subacoes = $this->carregar($sql);
		$subacoes = $subacoes ? $subacoes : array();
		
		$arSubacao = array();
		foreach( $subacoes as $subacao ){
			
			$dadosMonitoramentos = new HistoricoMonitoramentoConvenio();
			$dadosMonitoramentos->carregarPorId($subacao['hmcid']);
			
			$sql="  SELECT 	sum( coalesce(cs.cosqtd, 0) * cs.cosvlruni ) as totalprogramado
							, sum( coalesce(hic.tot_hmsvalortotalpago, 0) ) AS totalacumulado
					FROM cte.composicaosubacao cs 
					-- INNER JOIN cte.unidademedidadetalhamento umd ON umd.unddid = cs.unddid 
					LEFT JOIN cte.historicomonitoramentoconvsubac h ON h.sbaid = cs.sbaid AND h.hmsid = ".$subacao['hmsid']."
					LEFT JOIN cte.historicoconvitemcomposicao hi ON hi.hmsid = h.hmsid AND hi.cosid = cs.cosid AND h.hmsid = ".$subacao['hmsid']."
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
									i.hmsano = " . $subacao['pssano'] . "
									and  hc.hmcdatamonitoramento <= '".$dadosMonitoramentos->hmcdatamonitoramento."'
							    GROUP BY
									i.cosid
						  	  ) hic ON hic.cosid = cs.cosid
					LEFT JOIN cte.statuscomposicaosubacao scs ON scs.scsid = hi.scsid
					LEFT JOIN cte.historicomonitoramentoconvenio hmc ON hmc.hmcid = h. hmcid
					WHERE cs.sbaid = ".$subacao['sbaid']." AND cs.cosano = ".$subacao['pssano'];

			$itens = $this->pegaLinha($sql);
			$arSubacao[] = $subacao + $itens;
		}
		
		return $arSubacao; 
	}
	
	/**
	 * function getSubacoes($subacoes, $ssuid, $prsid);
	 * @desc   : retorna o HTML da lista de subações. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : array $subacoes (lista de subação)
	 * @param  : numeric $ssuid (id do status da subação)
	 * @param  : numeric $prsid (id do convênio)
	 * @return : string $conteudo (HTML com a lista de subações)
	 * @since 17/03/2009
	 */
	function getSubacoes($subacoes){
		global $db;
		
		if(is_array($subacoes) ){
			$conteudo = '<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
						<tr>
							<th>Descrição da Subações</th>
							<th>Mês / Ano de repasse</th>
							<th colspan="2" >Status da <br> subação</th>
							<th >Qtd. Itens pendentes</th>
						</tr>
					';
			$indicePai = 1;
			foreach($subacoes as $subacao){
				$dadosItens 		= $this->itensComPendencia($subacao["sbaid"], $subacao['pssano'], $subacao['hmcid']);
				$itensFaltam 		= $dadosItens['faltam'];
				$itensTotal 		= $dadosItens['total'];
				$itensMonitorados 	= $dadosItens['monitorados'];
				if($subacao["hmcstatus"] == 'F' ){
					$imagem 			= "../../imagens/check_p.gif";
					$descricaoStatusSub = $subacao['ssudescricao'];
					$podeEditar 		= 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['hmcid'].','.$indicePai.');"';	
					$onclickBtn 		= 'onclick = "erro(5);"';
				}else{
					// se tiver status já informado ele mostra caso não e pendente
					if($subacao["ssuid"] == STATUSNAOSELECIONADO){
						$imagem =  "../../imagens/atencao.png";
						$descricaoStatusSub = "Pendente";
						$subacao["ssuid"] = 0;
					}else{
						$imagem = "../../imagens/check_p.gif";
						$descricaoStatusSub = $subacao['ssudescricao'];
					}
		
					$onclickBtn = 'onclick="carregaTelaStatusSubAcao('.$subacao["sbaid"].','.$subacao["ssuid"].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['pssano'].','.$subacao['hmcid'].')"';
					if( ($subacao['ssuid'] == EXECUTADA) || ($subacao['ssuid'] == EMEXECUCAO)){
						$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['hmcid'].','.$indicePai.');"';	
					}else if($subacao['ssuid'] == STATUSNAOSELECIONADO){
							$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['hmcid'].','.$indicePai.');"';	
					}else{
						$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['hmcid'].','.$indicePai.');"';	
					}
				}
				/*
				if(cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO))){
					$onclickBtn = 'onclick="carregaTelaStatusSubAcao('.$subacao["sbaid"].','.$subacao["ssuid"].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['pssano'].','.$subacao['hmcid'].')"';
					$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$subacao['prsid'].','.$subacao['hmsid'].','.$subacao['hmcid'].');"';	
				}
				*/
				$conteudo .=	'<tr>
									<td class="SubTituloEsquerda" >
										<div class="div_imagens" id="img_subacoes_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'">	
											<img src="../../imagens/mais.gif" id="mais_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" name="mais_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" '.$podeEditar.' /> 
											<img src="../../imagens/menos.gif" style="display:none;" id="menos_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" name="menos_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" onclick="disableItensComposicao('.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].','.$_SESSION['ano'].');"/>
										</div><a style="cursor:pointer;" id="editarSub_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" '.$onclickBtn.'>'.$indicePai.' - '.$subacao["sbadsc"].'</a>
									</td>
									<td class="SubTituloEsquerda" width="8%" id="ano_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'">'.$subacao['pssmes']."/".$subacao['pssano'].'</td>
									<td class="SubTituloEsquerda" width="8%" id="subStatusDesc_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'">'.$descricaoStatusSub.'</td>
									<td class="SubTituloEsquerda" id="subStatus_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" > 
										<a style="cursor:pointer;" id="editarSub_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" '.$onclickBtn.'>
											<img title="Sub-ação não monitorado" id="imgsubStatus_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" align="absmiddle"  src="'.$imagem.'" width="18" height="18">
										</a>
									</td>
									<td id="itensPendentes_'.$subacao["sbaid"].$subacao['pssano'].'" class="SubTituloEsquerda" >'.$itensFaltam.' de '.$itensTotal.'</td>
								</tr>
								 <tr>
								 	<td colspan="5">
								 		<div style="display:none;" id="linha_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'" class="linha">
											<img src="../../includes/dtree/img/line.gif"/><br>
											<img src="../../includes/dtree/img/joinbottom.gif"/>
										</div>
								 		<div class="listaItensComposicao" id="listaItensComposicao_'.$subacao["sbaid"].$subacao['prsid'].$subacao['pssano'].'"></div>
								 	</td>
								 </tr>
								 ';
				$indicePai++;
			}
		}else{
			$conteudo = '<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
						<tr>
							<th>Não existe subaçãoes para ser monitorada neste mês. Entre em contato com o administrador.</th>
						</tr>
						</table>
					';
		}
		$conteudo .='</table>';
		return $conteudo;
	}
	
	function itensComPendencia($sbaid, $ano, $hmcid ){
		$sql = "SELECT (sum(total) - sum(monitorado)) as faltam, sum(total) as total,  sum(monitorado) as monitorados
				FROM (
					SELECT  0 AS monitorado,
						count(cosid) AS total
					FROM cte.composicaosubacao
					WHERE sbaid = ".$sbaid." AND cosano = ".$ano."
				UNION ALL 
					SELECT  count (hciid) AS monitorado,
						0 AS total
					FROM cte.historicoconvitemcomposicao hi
					INNER JOIN cte.historicomonitoramentoconvsubac hs ON hs.hmsid = hi.hmsid 
					WHERE  hs.sbaid = ".$sbaid." AND hmcid= ".$hmcid." and hi.hmsano = ".$ano."
					AND (hi.hmsquantidadeempenhada is not null OR hi.hmsobs <> '')
				UNION ALL 	
					SELECT  0 AS monitorado,
						count(hieid) AS total
					FROM cte.historicoitemexecutado
					WHERE sbaid = ".$sbaid." 				
				      ) AS foo ";
		//dbg($sql,1);
		$dados = $this->pegaLinha($sql); // quantos itens faltam ser monitorados.
		return $dados;
		
	}
	
	/**
	 * function carregaComboStatusSubacao();
	 * @desc   : Carrega o combo com uma lista de todos os status possiveis, 
	 * 			 passando como parametro o id da subação e o id do status, para recuperar o status da Subação.
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $ssuidmonitora (id do status da subação)
	 * @param  : numeric $sbaid (id da subação)
	 * @return : string $select_status_subacao (combo de status de subacao)
	 * @since 17/03/2009
	 */
	function carregaComboStatusSubacao($sbaid ,$ssuidmonitora, $prsid, $historiocoSubacao, $hmcid, $hmstravadoMontAnterior){
		$ssuid = $ssuidmonitora;
		if($hmstravadoMontAnterior){
			$sql = "select ssuid from cte.historicomonitoramentoconvsubac where hmsid = ".$hmstravadoMontAnterior;
			$statusAnterior = $this->pegaUm($sql);
		}
		
		if($statusAnterior == EMEXECUCAO){
			$and = " and ssuid in ( ".EXECUTADA.", ".EMEXECUCAO.")";
		}
		$sqlStatusSubacao = "SELECT ssuid AS codigo, 
									ssudescricao AS descricao 
							 FROM cte.statussubacao 
							 WHERE ssutipostatus = 'P'
							 ".$and."
							 ORDER BY ssudescricao"; 
		$selectStatusSubacao = $this->monta_combo('ssuid',$sqlStatusSubacao,'S', 'Escolha...', 'escolhaStatusSubacao(this.value,'.$hmcid.'); dummies', '', '', '', 'N', 'ssuid', true, $ssuid);
		
		return $selectStatusSubacao;
	}
}