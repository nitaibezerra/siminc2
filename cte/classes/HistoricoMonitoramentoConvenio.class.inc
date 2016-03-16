<?php
//include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');
class HistoricoMonitoramentoConvenio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicomonitoramentoconvenio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hmcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hmcid' => null, 
									  	'prsid' => null, 
									  	'hmcdatamonitoramento' => null, 
									  	'hmcstatus' => null, 
    									'hmcdatareferenciamonitoramento' => null,
    									'hmdid' => null
									  );
									  
	public function carregaUltimoMonitoramento($prsid, $ano){
		global $db;

		$sql = "SELECT 	  hmcid AS codigo, 
					      db.hmdmes||'/'||db.hmdano AS descricao,
					      case length(cast(db.hmdmes as character(2)))
					  		when 1 then db.hmdano||'/'||'0'||db.hmdmes
					  		when 2 then db.hmdano||'/'||db.hmdmes end as data,
					      case length(cast(db.hmdmes as character(2)))
					  		when 1 then '0'||db.hmdmes||'/'||db.hmdano
					  		when 2 then db.hmdmes||'/'||db.hmdano end as dataformatada
					FROM cte.historicomonitoramentodadosbancarios db
					INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
					            and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
					            and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
					WHERE db.prsid = ".$prsid." and db.hmdano >= ".$ano."
					ORDER BY 3 desc
		";
		$mes = $db->pegaLinha($sql);
		return $mes;
	}
	
	/**
	 * function recuperaDadosDoMonitoramento($idConvenio);
	 * @desc   : Carrega dados de monitoramento por convênio. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $idConvenio (id do convênio)
	 * @return : array $dadosConvenios
	 * @since 01/04/2009
	 */
	public function recuperaDadosDoMonitoramento($idConvenio, $mes = NULL){
		global $db;
		if($mes != 0){
			$buscaComMes = " AND to_char(hmcdatamonitoramento, 'MM') = '".$mes."'";
		}else{
			$buscaComMes = '';
		}
		
		$sql = "SELECT hmcid, prsid, hmcstatus
				FROM cte.historicomonitoramentoconvenio
				WHERE prsid = ".$idConvenio.$buscaComMes;
		$dadosConvenios 	= $db->carregar($sql);
		return $dadosConvenios;
	}
	
	/**
	 * function carregaMonitoramentosParaSession($prsid, $hmcid);
	 * @desc   : Carrega os monitoramentos abertos para session. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $prsid (id do convênio)
	 * @param  : numeric $hmcid (id do monitoramento)
	 * @return : boleano true
	 * @since 01/04/2009
	 */
	public function carregaMonitoramentosParaSession($prsid,$hmcid){
			$_SESSION['hmc'][$prsid] = $hmcid;
		return true;
	}
	
	public function monitoramentoAnteriorEncerrado($prsid){
		global $db;
		$sql = "SELECT hmcstatus 
				FROM cte.historicomonitoramentoconvenio 
				WHERE prsid = ".$prsid." ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
		
		$status = $db->pegaUm($sql);
		if($status == "I"){
			return false;
		}else{
			return true;
		}
	}
	
	public function statusMonitoramento($hmcid){
		global $db;
		$sql = "SELECT hmcstatus 
				FROM cte.historicomonitoramentoconvenio 
				WHERE hmcid = ".$hmcid;
		
		$status = $db->pegaUm($sql);
		return $status;
	}
	
	/**
	 * function insereMonitoramento($subacoesItensMonitorados);
	 * @desc   : Verifica se as subçãoes e os itens estão finalizados para poder encerrar o monitoramento. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : array $subacoesItensMonitorados (dados das subações e itens)
	 * @return : retorna true se ok se não retorna string com o erro.
	 * @since 02/04/2009
	 */
	public function insereMonitoramento($prsid, $hmdid){
		
		// Monta Data do monitoramento.
		$data  = new Data();
		$meses = new HistoricoMonitoramentoDadosBancarios();
		$meses = $meses->recuperaMes($hmdid);
		$mes = $meses['hmdmes'];
		$dia = '01'; 
		$ano = $meses['hmdano'];
		$mesMonitoramentoAnterior = $mes -1;
		$anoMonitoramentoAnterior = $ano;
		if($mesMonitoramentoAnterior == 0){
			$mesMonitoramentoAnterior = 12;
			$anoMonitoramentoAnterior = $ano -1;
		}
		
		if($mes != 10 && $mes != 11 && $mes != 12){
			$mes = '0'.$mes;
		}
		if($mesMonitoramentoAnterior != 10 && $mesMonitoramentoAnterior != 11 && $mesMonitoramentoAnterior != 12){
			$mesMonitoramentoAnterior = '0'.$mesMonitoramentoAnterior;
		}
		$dataMonitoramento = $ano."-".$mes."-".$dia;

		// SALVA DADOS - CRIA O MONITORAMENTO
		$this->prsid 							= $prsid;
		$this->hmcdatamonitoramento 			= $dataMonitoramento;
		$this->hmcdatareferenciamonitoramento 	= date('Y-m-d');
		$this->hmcstatus 						= "I";
		$this->hmdid 							= $hmdid;
		$this->salvar();
		$this->commit();
	
		// SE O MONITORAMENTO ANTERIOR TIVER ALGUMA SUBAÇÃO COM STATUS EXECUTADA OU CANCELADA O MONITORAMENTO CRIADO OS ITENS E SUB JA ESTARÃO MONITORADO.
		$sql = "SELECT s.ssuid, s.sbaid, s.hmsano, s.hmsjustificativa, s.hmsid, p.pssano, s.pssid
				FROM cte.historicomonitoramentoconvenio h
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmcid = h.hmcid
				INNER JOIN cte.projetosapesubacao p ON p.sbaid = s.sbaid  and s.hmsano = p.pssano 
				WHERE h.prsid = ".$prsid."
				AND to_char(hmcdatamonitoramento, 'MM') = '".$mesMonitoramentoAnterior."' 
				AND to_char(hmcdatamonitoramento, 'YYYY') = '".$anoMonitoramentoAnterior."' 
				AND hmcstatus = 'F'
				AND ssuid in (".EXECUTADA.", ".CANCELADA.",".EMEXECUCAO.")
				ORDER BY hmcdatamonitoramento DESC";
		//dbg($sql,1);
		$dadosAnteriores = $this->carregar($sql);
		if(is_array($dadosAnteriores)){
			foreach($dadosAnteriores as $dados){ // INSERE SUBAÇÃOES
				
				$salvarSub = false;
				$obHistMonitoraConvSubac->pssid 			= $dados['pssid'];
				if( ($dados['ssuid'] == EXECUTADA) || ($dados['ssuid'] == CANCELADA) ){
					$hmsidAnterior 		 		= $dados['hmsid'];
					$obHistMonitoraConvSubac 	= new HistoricoMonitoramentoConvSubac();
					$obHistMonitoraConvSubac->hmcid 			= $this->hmcid;
					$obHistMonitoraConvSubac->ssuid 			= $dados['ssuid'];
					$obHistMonitoraConvSubac->sbaid 			= $dados['sbaid'];
					$obHistMonitoraConvSubac->hmsjustificativa 	= $dados['hmsjustificativa'];
					$obHistMonitoraConvSubac->hmsano 			= $dados['pssano'];
					$obHistMonitoraConvSubac->hmstravado 		= 't';
					$salvarSub = true;
				}else if($dados['ssuid'] == EMEXECUCAO ){
					$hmsidAnterior 				= $dados['hmsid'];
					$obHistMonitoraConvSubac 					= new HistoricoMonitoramentoConvSubac();
					$obHistMonitoraConvSubac->hmcid 			= $this->hmcid;
					$obHistMonitoraConvSubac->ssuid 			= STATUSNAOSELECIONADO;
					$obHistMonitoraConvSubac->sbaid 			= $dados['sbaid'];
					$obHistMonitoraConvSubac->hmsjustificativa 	= $dados['hmsjustificativa'];
					$obHistMonitoraConvSubac->hmsano 			= $dados['pssano'];
					$obItens 	= new HistoricoConvItemComposicao;
					$itens = $obItens->recuperaDadosItensComposicao($hmsidAnterior);
					foreach($itens as $item){
						if( ($item['scsid'] == CANCELADAITEMCOMP) || ($item['scsid'] == EXECUTADOITEMCOMP) ){
							$salvarSub = true;
							break;
						}
					}
				}

				if($salvarSub){
					if($obHistMonitoraConvSubac->salvar()){
						$obHistMonitoraConvSubac->commit();
						$hmsid = $obHistMonitoraConvSubac->hmsid;
						if($hmsid){
							$obHistMonitoraConvSubac->salvarItens($hmsid, $hmsidAnterior, $dados['pssano'], $dados['ssuid'], $obHistMonitoraConvSubac->hmsjustificativa); // INSERE ITENS DE COMPOSIÇÃO.
						}
					}else{
						$obHistMonitoraConvSubac->rollback();
					}
				}
			}
		}
		if($this->commit()){
			return "Monitoramento iniciado com sucesso.";
		}else{
			return "Ocorreu um erro ao tentar iniciar o monitoramento.";;
		}
	}
	
	public function valorTotalConvenio($hmcid, $ano){
		global $db;
		if($ano == NULL){
			$ano = 0;
		}
			$sql = "SELECT 	SUM(total) AS total, 
							SUM(analisado) AS analisado
					FROM(
						SELECT  sum(cs.cosvlruni * cs.cosqtd) AS total,
								0 AS analisado,
								p.prsnumconvsape
						FROM cte.projetosape p
						INNER JOIN cte.projetosapesubacao ps ON p.prsid = ps.prsid
						INNER JOIN cte.subacaoindicador si ON si.sbaid = ps.sbaid
						INNER JOIN cte.composicaosubacao cs ON cs.sbaid = si.sbaid
						INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = p.prsid
						WHERE  cs.cosano = ".$ano." AND hc.hmcid = ".$hmcid."
						GROUP BY prsnumconvsape
						UNION ALL
						SELECT  0 AS total,
								sum(hmsvalortotalempenhado) AS analisado,
								p.prsnumconvsape
						FROM cte.historicomonitoramentoconvenio hc
						INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
						INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
						INNER JOIN cte.projetosape p ON  hc.prsid = p.prsid
						WHERE hmsvalortotalempenhado IS NOT NULL AND hc.hmcid = ".$hmcid."
						GROUP BY p.prsnumconvsape
					) AS valores GROUP BY prsnumconvsape";
			$valores = $db->pegaLinha($sql);
			if($valores['total'] == $valores['analisado']){
				return false;
			}else{
				return true;
			}
	}
	
	public function recuperaTodosMonitoramentos($prsid){
		$sql = "SELECT hmcid FROM cte.historicomonitoramentoconvenio 
				WHERE prsid = ".$prsid." 
				ORDER BY hmcid DESC";
		$monitoramentos = $this->carregar($sql);
		return $monitoramentos;
	}

	
	
	public function recuperaSubacoesItensDoMonitoramento($hmcid){
		$sql = "SELECT  h.hmcid  AS idhistorico,
						hs.hmsid AS idhistoricosubacao,
						ps.sbaid AS idsubacao,  
						hs.ssuid AS statussubacao,
						hi.hciid AS idhistoriocoitens,
						cs.cosid AS iditens,
						hi.scsid AS statusitens
				FROM cte.projetosape p
				INNER JOIN cte.projetosapesubacao 				ps ON ps.prsid = p.prsid
				INNER JOIN cte.historicomonitoramentoconvenio 	h  ON h.prsid  = p.prsid
				INNER JOIN cte.composicaosubacao 				cs ON cs.sbaid = ps.sbaid AND cs.cosano = ps.pssano
				LEFT JOIN cte.historicomonitoramentoconvsubac 	hs ON h.hmcid  = hs.hmcid AND ps.sbaid = hs.sbaid  AND ps.pssano = hs.hmsano 
				LEFT JOIN cte.historicoconvitemcomposicao 		hi ON hi.hmsid = hs.hmsid AND cs.cosid = hi.cosid  
				WHERE h.hmcid = ".$hmcid." AND to_char(ps.pssdata, 'YYYY-MM')  <= to_char(h.hmcdatamonitoramento, 'YYYY-MM') ";

		$subacoesItensMonitorados 	= $this->carregar($sql);
		return $subacoesItensMonitorados;
	}
	
	/**
	 * function verificaSeSubacoesItensForamMonitorados($subacoesItensMonitorados);
	 * @desc   : Verifica se as subçãoes e os itens estão finalizados para poder encerrar o monitoramento. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : array $subacoesItensMonitorados (dados das subações e itens)
	 * @return : retorna true se ok se não retorna string com o erro.
	 * @since 02/04/2009
	 */
	function verificaSeSubacoesItensForamMonitorados($hmcid, $return = false){
		$subacoesItensMonitorados = $this->recuperaSubacoesItensDoMonitoramento($hmcid);
		if(is_array($subacoesItensMonitorados)){
			foreach($subacoesItensMonitorados as $itens){
				$statusItem 	= $itens['statusitens'];
				$statusSubacao 	= $itens['statussubacao'];
				if($statusSubacao == NULL or $statusSubacao == ''){
					if($return){
						return false;
					}
					return "Existem subações que não foram monitorados.\nFinalize o monitoramento de todas subações, para poder encerrar o monitoramento.";
				}
				if($statusItem == NULL or $statusItem == ''){
					if($return){
						return false;
					}
					return "Existem itens de composição que não foram monitorados. Finalize o monitoramento dos itens, para poder encerrar o monitoramento.";
				}
			}	
		}else{
			if($return){
				return false;
			}
			return "Este convênio está no estado de monitoramento e nenhum item foi monitorado até o momento. Para que seja possível encerrar termine o monitoramento dos itens da subação.";
		}
		return true; // Monitoramento Finalizado. Não existem subações e nem itens com pendências.
	}
	
	/**
	 * function atualizaListaHistoricoMonitoramento($arrPrsid);
	 * @desc   : Atualiza a lista de historico de monitoramento. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : array $arrPrsid
	 * @return : retorna a lista.
	 * @since 20/04/2009
	 */
	function atualizaListaHistoricoMonitoramento($arrPrsid=NULL){
		return $this->listaHistoricoMonitoramento($_SESSION['inuid'],$_SESSION['ano'], $arrPrsid);
		exit();
	}
	
	/**
	 * function listaHistoricoMonitoramento();
	 * @desc   : Atualiza a lista de historico de monitoramento. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $inuid
	 * @param  : numeric $ano
	 * @param  : array $arrPrsid
	 * @return : retorna a lista.
	 * @since 20/04/2009
	 */
	function listaHistoricoMonitoramento($inuid,$ano, $arrPrsid=NULL, $boImpressao = false){
		$convenio = 0;
		$data = new Data();
	
		if( $boImpressao ){
			$nrQtdPorPagina = 200;
			$stSelect = '';
			$cabecalho 	= array("Nº do Convênio", "Mês", "Data","Status", "Executado %");
		}
		else{
			$nrQtdPorPagina = 10;
			$stSelect = " '<a style=\"margin: 0 -20px 0 20px;\" onclick=\"visualizaRelatorioHistoricoMonitoramento('|| ps.prsid ||','||h.hmcid||');\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao, ";
			$cabecalho 	= array("Ação", "Nº do Convênio", "Mês", "Data","Status", "Executado %");
		}
		
		
		
		$sql = "SELECT  		$stSelect
								ps.prsnumconvsape,
								'' as mes,   
								to_char(hmcdatamonitoramento, 'MM/YYYY') as hmcdatamonitoramento,   
								CASE WHEN hmcstatus = 'I' THEN 'Em Andamento' ELSE 'Finalizada' END AS status,
								h.hmcid 
						FROM cte.historicomonitoramentoconvenio h
						INNER JOIN cte.projetosape ps ON h.prsid = ps.prsid
						LEFT join cte.historicomonitoramentoconvsubac  hs ON hs.hmcid = h.hmcid
						LEFT JOIN  cte.historicoconvitemcomposicao hi ON hi.hmsid = hs.hmsid AND (hi.scsid = ".EXECUTADOITEMCOMP." OR hi.scsid = ".EMEXECUÇÃOITEMCOMP.")
						LEFT join cte.composicaosubacao c on c.cosid = hi.cosid 
						WHERE ps.inuid= ".$inuid." AND ps.prsano = ".$ano." AND ps.prsid = ".$arrPrsid."
						GROUP BY ps.prsnumconvsape, hmcdatamonitoramento, ps.prsid, h.hmcid, h.hmcstatus ORDER BY h.hmcdatamonitoramento  asc "; 

		$dados 		= $this->carregar($sql);
		$porcentagemTotal = 0;
		if(is_array($dados)){
			for($cont = 0; $cont < count($dados); $cont++){
				$projeto =  new HistoricoMonitoramentoDadosBancarios();
				$analise = $projeto->barraListaHistoricoMonitoramento( $ano, $dados[$cont]['prsnumconvsape'], $dados[$cont]['hmcid']);
				
				if(!$analise['total']){
					$analise['total'] = 1;
				}
				$porcentagem = ($analise['analisado'] * 100) /  $analise['total'];
			
				if($analise['prsnumconvsape'] == $convenio ){
				}else{
					$convenio = $dados[$cont]['prsnumconvsape'];
				}
				if($dados[$cont]['hmcid'] ){
					//$porcentagem = number_format($porcentagem,0);
					$porcentagem = arredondar_dois_decimal($porcentagem);
					$porcentagemTotal += $porcentagem;
					if($porcentagemTotal > 100){
						$corBarra = 100;
						$cor = "#FF0000";
					}else{
						$corBarra = $porcentagemTotal;
						$cor = "#333399";
					}
				
					
					$barra  = '<center><span style="color:cococo;font-size: 10px;">';
					$barra .= $porcentagemTotal." % ";
					$barra .= '	</span>
								<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcdcff;" title="'.$porcentagemTotal.'%"
								<div style="font-size:4px; width: '.$corBarra.'%; height:6px; max-height:6px; background-color:'.$cor.';">
								</div></div>
								</center>';
					$dados[$cont]['hmcid'] = $barra;
				}
				
				if($dados[$cont]['hmcdatamonitoramento']){
					$dados[$cont]['mes'] = $data->formataData("01/".$dados[$cont]['hmcdatamonitoramento'], "mesTextual",'');
				}
			}
		}
		
		if(is_array($dados)){
			$lista = $this->monta_lista_array($dados,$cabecalho, $nrQtdPorPagina, 10, 'N', '', ''  );
		}
		return $lista;
		
	}
	
	/**
	 * function finalizarMonitoramento($prsid);
	 * @desc   : Finaliza o monitoramento. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $prsid (id do convênio)
	 * @return : string um alert informativo.
	 * @since 02/04/2009
	 */
	
	public function finalizarMonitoramento($hmcid){
		global $db;
		$dadosFinalizados = $this->verificaSeSubacoesItensForamMonitorados($hmcid, true);
				
		if($dadosFinalizados === true){
			$sql = "INSERT INTO cte.auditoriafinanceiro (usucpf, usunome, hmcid, adfdata) VALUES('".$_SESSION['usucpf']."','".$_SESSION['usunome']."',".$hmcid.",now())";
			$db->executar($sql);
			unset($sql);
			
			$sql = "UPDATE cte.historicomonitoramentoconvenio SET hmcstatus = 'F' 
					WHERE hmcid = ".$hmcid;
			if($db->executar($sql)){
				$db->commit();
				return "Monitoramento encerrado com sucesso.";
			}else{
				$db->rollback();
				return "Ocorreu um erro ao tentar finalizar o monitoramento.";
			}
		}else{
			return "Ocorreu um erro ".$dadosFinalizados;
		}
	}
	


}