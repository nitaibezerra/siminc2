<?php
// VERIFICAR NA SUB = AND ps.pssano = hs.hmsano 

// ************************** FOI ******************************************************************************
/**
 * function carregaListaDeConvenio($ano, $instrumentoUnidade);
 * @desc   : Recupera a lista de convênios. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $instrumentoUnidade (Instrumento unidade)
 * @param  : numeric $ano (ano de referencia)
 * @return : array $convenios (array contendo os convênios)
 * @since 17/03/2009
 */
function carregaListaDeConvenio($ano, $instrumentoUnidade){
	global $db;
	$sql = "SELECT p.prsid, p.prsano, p.prsnumconvsape
			FROM cte.projetosape p
			WHERE p.prsano =".$ano." 
			AND p.inuid = ".$instrumentoUnidade;

	$convenios 	= $db->carregar($sql);
	return $convenios;
}

/**
 * function carregaConvenio($ano, $instrumentoUnidade, $prsid);
 * @desc   : Recupera o convênio apos ele finalizado. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $instrumentoUnidade (Instrumento unidade)
 * @param  : numeric $ano (ano de referencia)
 * @return : array $prsid (id do convênio)
 * @since 17/03/2009
 */
function carregaConvenio($ano, $instrumentoUnidade, $prsid){
	global $db;
	$sql = "SELECT p.prsid, p.prsano, p.prsnumconvsape
			FROM cte.projetosape p
			WHERE p.prsano =".$ano." 
			AND p.inuid = ".$instrumentoUnidade."AND p.prsid = ".$prsid;
	$convenios 	= $db->carregar($sql);
	return $convenios;
}

// ************************** FOI ******************************************************************************
/**
 * function carregaMonitoramentosParaSession($prsid, $hmcid);
 * @desc   : Carrega os monitoramentos abertos para session. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $prsid (id do convênio)
 * @param  : numeric $hmcid (id do monitoramento)
 * @return : boleano true
 * @since 01/04/2009
 */
function carregaMonitoramentosParaSession($prsid,$hmcid){
		$_SESSION['hmc'][$prsid] = $hmcid;
	return true;
}

function retiraMonitoramentosDaSession($prsid,$hmcid ){
	unset($_SESSION['hmc'][$prsid]);
	return true;
	
}

// ************************** FOI ******************************************************************************
/**
 * function recuperaDadosDoMonitoramento($idConvenio);
 * @desc   : Carrega dados de monitoramento por convênio. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $idConvenio (id do convênio)
 * @return : array $dadosConvenios
 * @since 01/04/2009
 */
function recuperaDadosDoMonitoramento($idConvenio, $mes = NULL){
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

// ************************** FOI ******************************************************************************
function validaSePodeInserirMonitoramento($prsid, $mesTela, $anoTela = NULL, $return = NULL){
	$obdata = new Data();
	$obHistMonitoraConvenio = new HistoricoMonitoramentoConvenio();
	
	$sql = "SELECT hmcid
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = '".$prsid."' ORDER BY hmcid DESC";
	$hmcidC = $obHistMonitoraConvenio->pegaUm($sql);

	if($hmcidC){
		if(!valorTotalConvenio($hmcidC, $_SESSION['ano'])){
			if($return){
				return false;
			}
			return "O monitoramento deste mês não pode ser criado, \n pois o monitoramento do mês anterior já chegou ao teto máximo de valor do convênio.";
		}
	}
	
	// REGRA 1
	//dbg($mesTela);
	//if($mesTela !=10 || $mesTela !=11 || $mesTela !=12){
	//	$mesTela = '0'.$mesTela;
	//}

	$sql = "SELECT hmcstatus
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid." AND to_char(hmcdatamonitoramento, 'MM') = '".$mesTela."' 
			ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
	
	$status = $obHistMonitoraConvenio->pegaUm($sql);
	
	if($status == "F"){
		if($return){
				return false;
		}
		return "O monitoramento deste mês já foi finalizado.";
	}
	
	$sql = "SELECT hmcstatus
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid."
			ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
	
	$status2 = $obHistMonitoraConvenio->pegaUm($sql);
	//dbg($status2,1);
	if($status2 == "I"){
		if($return){
				return false;
		}
		return "O monitoramento do mês anterior não foi finalizado.";
	}
	
	
	
	// REGRA 2
	if(!existeDadosFinanceirosCadastrados($prsid)){
		if($return){
			return false;
		}
		return "O monitoramento não pode ser iniciado. \n Não existe dados Financeiros do Convênio cadastrado para o mês atual. ";
	}
	
	
	// REGRA 3
	$sql = "select  to_char(prsdata, 'YYYY') AS anoconvenio, 
					to_char(prsdata, 'MM')   AS mesconvenio,
					to_char(prsdata, 'YYYY-MM-DD') as data
			from cte.projetosape 
			where prsid = ".$prsid." limit 1";
	$dataConvenio = $obHistMonitoraConvenio->pegaLinha($sql);
	$podeIniciar = podeIniciarConvenio($dataConvenio);
	
	if($podeIniciar === "erroBanco"){
		$erro = "A data que firmou o convênio não está cadastrado na base de dados. \n Entre em contato com o Administrador do sistema.";
		if($return){
			return false;
		}
		return $erro;
	}
	if($podeIniciar == false){
		if($return){
			return false;
		}
		return "O monitoramento não pode ser iniciado antes do dia 15 do próximo mês do convênio.";
	}
	
	// REGRA 4
	if(!monitoramentoAnteriorEncerrado($prsid) && !$mesTela ){
		if($return){
			return false;
		}
		return "O monitoramento não pode ser iniciado. \n Finalize o monitoramento do mês anterior. ";
	}
	
	// REGRA 5
	$sql = "SELECT to_char(hmcdatamonitoramento, 'MM') AS mes 
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid." ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
	$mes = $obHistMonitoraConvenio->pegaUm($sql);
	
	// PEGA MES
	$sql = "select  to_char(hmcdatamonitoramento, 'MM') AS hmdmes , to_char(hmcdatamonitoramento, 'YYYY') AS hmdano
		from cte.historicomonitoramentoconvenio 
		where prsid = ".$prsid." AND hmcstatus = 'F' ORDER BY hmcdatamonitoramento desc limit 1";
	$mesDB = $obHistMonitoraConvenio->pegaLinha($sql);
	//dbg($sql,1);
	if($mes){ // se já existe monitoramneto.
		$mes 	= $mes != 12 ? $mes += 01 : 01;
		$data 	= date('Y-').$mes.date('-d');
		if(date('d') < 15 && $mes == date('m') ){
			if($return){
				return false;
			}
			return "O monitoramento só pode ser iniciado após o dia 15 do mês.";
		}
		// REGRA 6
		/*
		if($mesTela != $mes ){
			if($return){
				return false;
			}
			return "É obrigatório monitorar os meses consecultivamente,\n desde a data de início do convênio firmado.";
		}
		*/
		
		$proximoMes 	= $mesDB['hmdmes'] != 12 ? $mesDB['hmdmes'] += 01 : '01';
		//$proximoMes = $mesDB['hmdmes'] + 1;
		
		if($mesTela != $proximoMes){
			$msnMes = $obdata->mesTextual($proximoMes);
			if($return){
				return false;
			}
			return "Este mês não pode ser monitorado. \n Inicie o monitoramento do mês ".$msnMes.".";
		}else{
			return true;
		}
		if($anoTela != NULL){
			if($mesDB['hmdano'] != $anoTela){
				$msnMes = $obdata->mesTextual($proximoMes);
				if($return){
					return false;
				}
				return "Este mês não pode ser monitorado. \n Inicie o monitoramento do mês ".$msnMes.".";
			}
		}
		
		
	}else{ // SE NÃO EXISTE MONITORAMENTO CRIADO.
		// PEGA MES
		$sql = "select  hmdmes, hmdano
			from cte.historicomonitoramentodadosbancarios 
			where prsid = ".$prsid." AND hmdano >= ".$_SESSION['ano']." ORDER BY hmdano, hmdmes ASC limit 1";
		$mesDB2 = $obHistMonitoraConvenio->pegaLinha($sql);
		
		// REGRA 7
		$data 			= $dataConvenio['data']; 
		$mesConvenio 	= $mesTela;
		$mesVerificacao = $mesTela; //$mesConvenio != 12 ? $mesConvenio += 01 : 01;
		$anoConvenio 	= $dataConvenio['anoconvenio']; 
		$data 			= date('Y-').$mesTela.date('-d');
		if(date('m') <= $mesConvenio && $anoConvenio == date('Y')  ){
			if($return){
				return false;
			}
			return "O monitoramento só pode ser iniciado no mês seguinte ao convênio.";
		}else if(date('d') < 15 && $mesVerificacao == date('m') ){
			if($return){
				return false;
			}
			return "O monitoramento só pode ser iniciado após o dia 15 do mês.";
		}
		
		//REGRA 8
		if($mesTela != $mesDB2['hmdmes']){
			$msnMes = $obdata->mesTextual($mesDB2['hmdmes']);
			if($return){
				return false;
			}
			return "Este mês não pode ser monitorado. \n Inicie o monitoramento do mês ".$msnMes.".";
		}
		
		if($anoTela != NULL){
			if($mesDB2['hmdano'] != $anoTela){
				$msnMes = $obdata->mesTextual($mesDB2['hmdmes']);
				if($return){
					return false;
				}
				return "Este mês não pode ser monitorado. \n Inicie o monitoramento do mês ".$msnMes.".";
			}
		}

		
		if($anoTela != $mesDB2['hmdano'] && $mesTela == $mesDB2['hmdmes']){
			$msnMes = $obdata->mesTextual($mesDB2['hmdmes']);
			if($return){
				return true;
			}
			return "Este mês não pode ser monitorado. \n Inicie o monitoramento do mês2 ".$msnMes.".";
		}
	}
	return true;
}

/**
 * function insereMonitoramento($subacoesItensMonitorados);
 * @desc   : Verifica se as subçãoes e os itens estão finalizados para poder encerrar o monitoramento. 
 * @author : Thiago Tasca Barbosa
 * @param  : array $subacoesItensMonitorados (dados das subações e itens)
 * @return : retorna true se ok se não retorna string com o erro.
 * @since 02/04/2009
 */
function insereMonitoramento($prsid, $mesTela, $ano, $idMes = NULL){
	$data = new Data();
	$obHistMonitoraConvenio 	= new HistoricoMonitoramentoConvenio();
	
	// REGRA 1
	if($mesTela != 10 && $mesTela != 11 && $mesTela != 12){
		$mesTela = '0'.$mesTela;
	}
	$sql = "SELECT hmcstatus
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid." AND to_char(hmcdatamonitoramento, 'MM') = '".$mesTela."' ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.

	$status = $obHistMonitoraConvenio->pegaUm($sql);
	
	// REGRA 2
	$sql = "SELECT to_char(hmcdatamonitoramento, 'MM') AS mes 
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid." ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
	$mes = $obHistMonitoraConvenio->pegaUm($sql);
	if($mes){ // se já existe monitoramneto.
		$mesAnterior = $mes;
		$mes 	= $mes != 12 ? $mes += 01 : 01;
		if($mes != 10 && $mes != 11 && $mes != 12){
			$mes = '0'.$mes;
		}
		$mesSessao 		= $mes;
		$anoSolicitado	= $ano;
		$data 	= $anoSolicitado.'-'.$mes.date('-d');
		
		$mesAnterior;
		$sql = "SELECT to_char(hmcdatamonitoramento, 'MM') AS mes 
				FROM cte.historicomonitoramentoconvenio 
				WHERE prsid = ".$prsid." ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
		$mes = $obHistMonitoraConvenio->pegaUm($sql);
		//select ssuid from cte.statussubacao where ssutipostatus = 'P' -- 8 executada -- 11 cancelada
	}else{ // SE NÃO EXISTE MONITORAMENTO CRIADO.
		// REGRA 3
		$data 			= $dataConvenio['data'];
		//$mesConvenio 	= $dataConvenio['mesconvenio'];
		$mesConvenio 	= $mesTela;
		$mesSessao 		= $mesTela;
		//$mesVerificacao = $mesConvenio != 12 ? $mesConvenio += 01 : 01;
		$anoConvenio 	= $dataConvenio['anoconvenio']; 
		//$data 			= date('Y-').$mesVerificacao.date('-d');
		$anoSolicitado	= $ano;
		$data 			= $anoSolicitado.'-'.$mesConvenio.date('-d');

	}
	/*
		$sql = "SELECT s.ssuid, s.sbaid, s.hmsano, s.hmsjustificativa, s.hmsid, p.pssano
				FROM cte.historicomonitoramentoconvenio h
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmcid = h.hmcid
				LEFT JOIN cte.projetosapesubacao p ON p.sbaid = s.sbaid
				WHERE h.prsid = ".$prsid."
				AND to_char(hmcdatamonitoramento, 'MM') = '".$mes."' 
				AND hmcstatus = 'F'
				 -- AND s.ssuid in (".EXECUTADA.",".CANCELADA.")
				ORDER BY hmcdatamonitoramento DESC";
		$dadosAnteriores = $obHistMonitoraConvenio->carregar($sql);
		dbg($dadosAnteriores,1);
		die();
	*/
	
	// SALVA DADOS - CRIA O MONITORAMENTO
	$obHistMonitoraConvenio->prsid = $prsid;
	$obHistMonitoraConvenio->hmcdatamonitoramento = $data;
	$obHistMonitoraConvenio->hmcdatareferenciamonitoramento = date('Y-m-d');
	$obHistMonitoraConvenio->hmcstatus = "I";
	$obHistMonitoraConvenio->hmdid = $idMes;
	$obHistMonitoraConvenio->salvar();
	if($mes){ // SE O MONITORAMENTO ANTERIOR TIVER ALGUMA SUBAÇÃO COM STATUS EXECUTADA OU CANCELADA O MONITORAMENTO CRIADO OS ITENS E SUB JA ESTARÃO MONITORADO.
		
		$sql = "SELECT s.ssuid, s.sbaid, s.hmsano, s.hmsjustificativa, s.hmsid, p.pssano
				FROM cte.historicomonitoramentoconvenio h
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmcid = h.hmcid
				LEFT JOIN cte.projetosapesubacao p ON p.sbaid = s.sbaid
				WHERE h.prsid = ".$prsid."
				AND to_char(hmcdatamonitoramento, 'MM') = '".$mes."' 
				AND hmcstatus = 'F'
				 -- AND s.ssuid in (".EXECUTADA.",".CANCELADA.")
				ORDER BY hmcdatamonitoramento DESC";
		//dbg($sql,1);
		//die();
		$dadosAnteriores = $obHistMonitoraConvenio->carregar($sql);
		if(is_array($dadosAnteriores)){
			foreach($dadosAnteriores as $dados){ // INSERE SUBAÇÃOES
				if( ($dados['ssuid'] == EXECUTADA) || ($dados['ssuid'] == CANCELADA) ){
					$hmsidAnterior 		 		= $dados['hmsid'];
					$obHistMonitoraConvSubac 	= new HistoricoMonitoramentoConvSubac();
					$obHistMonitoraConvSubac->hmcid 			= $obHistMonitoraConvenio->hmcid;
					$obHistMonitoraConvSubac->ssuid 			= $dados['ssuid'];
					$obHistMonitoraConvSubac->sbaid 			= $dados['sbaid'];
					$obHistMonitoraConvSubac->hmsjustificativa 	= $dados['hmsjustificativa'];
					$obHistMonitoraConvSubac->hmsano 			= $dados['pssano'];
					if($obHistMonitoraConvSubac->salvar()){
						$obHistMonitoraConvSubac->commit();
						$hmsid = $obHistMonitoraConvSubac->hmsid;
						$obHistMonitoraConvSubac->salvarItens($hmsid, $hmsidAnterior, $anoSolicitado); // INSERE ITENS DE COMPOSIÇÃO.
					}else{
						$obHistMonitoraConvSubac->rollback();
						
					}
				}else if($dados['ssuid'] == EMEXECUCAO ){
					$hmsidAnterior 		 		= $dados['hmsid'];
					$obHistMonitoraConvSubac 	= new HistoricoMonitoramentoConvSubac();
					$obHistMonitoraConvSubac->hmcid 			= $obHistMonitoraConvenio->hmcid;
					$obHistMonitoraConvSubac->ssuid 			= STATUSNAOSELECIONADO;
					$obHistMonitoraConvSubac->sbaid 			= $dados['sbaid'];
					$obHistMonitoraConvSubac->hmsjustificativa 	= $dados['hmsjustificativa'];
					$obHistMonitoraConvSubac->hmsano 			= $dados['pssano'];
					if($obHistMonitoraConvSubac->salvar()){
						$obHistMonitoraConvSubac->commit();
						$hmsid = $obHistMonitoraConvSubac->hmsid;
						$obHistMonitoraConvSubac->salvarItens($hmsid, $hmsidAnterior, $anoSolicitado); // INSERE ITENS DE COMPOSIÇÃO.
					}else{
						$obHistMonitoraConvSubac->rollback();
					}
				}
			}
		}
	}
	
	
	if($obHistMonitoraConvenio->commit()){
		$_SESSION['mes'] = $mesSessao;
		$_SESSION['hmc'][$prsid] = $obHistMonitoraConvenio->hmcid;
		return "Monitoramento iniciado com sucesso.";
	}else{
		return "Ocorreu um erro ao tentar iniciar o monitoramento.";;
	}
}

function valorTotalConvenio($hmcid, $ano){
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

function monitoramentoAnteriorEncerrado($prsid){
	global $db;
	$sql = "SELECT --to_char(hmcdatamonitoramento, 'MM') AS mes,
			hmcstatus 
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid." ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
	
	$status = $db->pegaUm($sql);
	if($status == "I"){
		return false;
	}else{
		return true;
	}
}

/**
 * function podeIniciarConvenio($arryDataConvenio);
 * @desc   : Verifica se o convenio pode iniciar.
 * 			 Regra: O monitoramento so pode começar no apartir do dia 15 do proximo mês. 
 * @author : Thiago Tasca Barbosa
 * @param  : array $arryDataConvenio 
 * @return : boleano $retorno (retorna true se ok ou false)
 * @since 20/04/2009
 */
function podeIniciarConvenio($arryDataConvenio){
	if($arryDataConvenio["anoconvenio"] == NULL){
		return "erroBanco";
	}
	$data = new Data();
	if($arryDataConvenio['mesconvenio'] != 12){
		$arryDataConvenio['mesconvenio'] += 01;
	}else{
		$arryDataConvenio['mesconvenio'] = '01';
		$arryDataConvenio['anoconvenio'] += 1; 
	}
	$dataMinima = $arryDataConvenio['anoconvenio'].'-'.$arryDataConvenio['mesconvenio'].'-15';
	$retorno = $data->diferencaEntreDatas(date('Y-m-d'),$dataMinima, 'maiorDataBolean','','YYYY-MM-DD');
	return $retorno;
}

/**
 * function existeDadosFinanceirosCadastrados($prsid);
 * @desc   : Verifica se o convenio pode iniciar.
 * 			 Regra: Para iniciar o monitoramento tem que ter cadastrado os Dados Financeiros para o mes de monitoramento.
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $prsid 
 * @return : boleano true ou false
 * @since 05/06/2009
 */
function existeDadosFinanceirosCadastrados($prsid){
	global $db;
	$sql="SELECT hmdmes, hmdano FROM cte.historicomonitoramentodadosbancarios where prsid = ".$prsid;
	$dados = $db->carregar($sql);
	$existe = 0;
	if($dados){
		foreach($dados as $dados){
			//if( (date('m') >= $dados['hmdmes']) && (date('Y') >= $dados['hmdano']) ){
			if(date('Y') >= $dados['hmdano'] ){
				$existe = 1;
			}
		}
		//dbg($existe,1);
		if($existe == 1){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

/**
 * function finalizarMonitoramento($prsid);
 * @desc   : Finaliza o monitoramento. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $prsid (id do convênio)
 * @return : string um alert informativo.
 * @since 02/04/2009
 */

function finalizarMonitoramento($prsid){
	global $db;
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
			WHERE h.hmcid = ".$_SESSION['hmc'][$prsid]." AND to_char(ps.pssdata, 'YYYY-MM')  <= to_char(h.hmcdatamonitoramento, 'YYYY-MM') ";
//dbg($sql,1);
	$subacoesItensMonitorados 	= $db->carregar($sql);
	
	$dadosFinalizados = verificaSeSubacoesItensForamMonitorados($subacoesItensMonitorados);
	if($dadosFinalizados === true){
		$sql = "INSERT INTO cte.auditoriafinanceiro (usucpf, usunome, hmcid, adfdata) VALUES('".$_SESSION['usucpf']."','".$_SESSION['usunome']."',".$_SESSION['hmc'][$prsid].",now())";
		
		
		$db->executar($sql);
		unset($sql);
		
		$sql = "UPDATE cte.historicomonitoramentoconvenio SET hmcstatus = 'F' 
				WHERE hmcid = ".$_SESSION['hmc'][$prsid];
		if($db->executar($sql)){
			$db->commit();
			unset($_SESSION['hmc'][$prsid]);
			return "Monitoramento encerrado com sucesso.";
		}else{
			$db->rollback();
			return "Ocorreu um erro ao tentar finalizar o monitoramento.";
		}
	}else{
		return $dadosFinalizados;
	}
}

/**
 * function verificaSeSubacoesItensForamMonitorados($subacoesItensMonitorados);
 * @desc   : Verifica se as subçãoes e os itens estão finalizados para poder encerrar o monitoramento. 
 * @author : Thiago Tasca Barbosa
 * @param  : array $subacoesItensMonitorados (dados das subações e itens)
 * @return : retorna true se ok se não retorna string com o erro.
 * @since 02/04/2009
 */
function verificaSeSubacoesItensForamMonitorados($subacoesItensMonitorados){
	if(is_array($subacoesItensMonitorados)){
		foreach($subacoesItensMonitorados as $itens){
			$statusItem 	= $itens['statusitens'];
			$statusSubacao 	= $itens['statussubacao'];
			if($statusSubacao == NULL or $statusSubacao == ''){
				return "Existem subações que não foram monitorados. 
						Finalize o monitoramento de todas subações, para poder encerrar o monitoramento.";
			}
			if($statusItem == NULL or $statusItem == ''){
				return "Existem itens de composição que não foram monitorados. Finalize o monitoramento dos itens, para poder encerrar o monitoramento.";
			}
		}	
	}else{
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
	//$busca = buscaSubacoes(  $cnvid); VERIFICAR BARRA PORCENTAGEM
	return listaHistoricoMonitoramento($_SESSION['inuid'],$_SESSION['ano'], $arrPrsid);
	exit();
}

function arredondar_dois_decimal($valor) {
   $float_arredondar=round($valor * 100) / 100;
   return $float_arredondar;
}

function porcentagensMesesMonitoramento($inuid,$ano, $prsid, $mesAtual, &$porcentagemTotal){
	global $db;
	$convenio = 0;
	$data = new Data();
	if(!$prsid){
		$prsid = 0;
	}
			
	$sql = "SELECT DISTINCT '<a style=\"margin: 0 -20px 0 20px;\" onclick=\"visualizaRelatorioHistoricoMonitoramento('|| ps.prsid ||','||h.hmcid||');\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
							ps.prsnumconvsape,
							to_char(hmcdatamonitoramento, 'MM') as mes,
							to_char(hmcdatamonitoramento, 'DD/MM/YYYY') as hmcdatamonitoramento,   
							CASE WHEN hmcstatus = 'I' THEN 'Em Andamento' ELSE 'Finalizada' END AS status,
							h.hmcid 
					FROM cte.historicomonitoramentoconvenio h
					INNER JOIN cte.projetosape ps ON h.prsid = ps.prsid
					LEFT join cte.historicomonitoramentoconvsubac  hs ON hs.hmcid = h.hmcid
					LEFT JOIN  cte.historicoconvitemcomposicao hi ON hi.hmsid = hs.hmsid AND (hi.scsid = ".EXECUTADOITEMCOMP." OR hi.scsid = ".EMEXECUÇÃOITEMCOMP.")
					LEFT join cte.composicaosubacao c on c.cosid = hi.cosid 
					WHERE ps.inuid= ".$inuid." AND ps.prsano = ".$ano." AND ps.prsid = ".$prsid." AND to_char(hmcdatamonitoramento, 'MM') = '".$mesAtual."'
					GROUP BY ps.prsnumconvsape, hmcdatamonitoramento, ps.prsid, h.hmcid, h.hmcstatus ORDER BY ps.prsnumconvsape, hmcdatamonitoramento  asc "; 

	$dados 		= $db->carregar($sql);
	if(is_array($dados)){
		for($cont = 0; $cont < count($dados); $cont++){
			$analise = barraListaHistoricoMonitoramento( $ano, $dados[$cont]['prsnumconvsape'], $dados[$cont]['hmcid']);
			if($analise['total'] == NULL){
				$analise['total'] = 1;
			}
			
			$porcentagem = ($analise['analisado'] * 100) /  $analise['total'];
			if($analise['prsnumconvsape'] == $convenio ){
			}else{
				$convenio = $dados[$cont]['prsnumconvsape'];
			}

			if($dados[$cont]['hmcid'] ){
				$porcentagem = number_format($porcentagem,0);
				$porcentagem = arredondar_dois_decimal($porcentagem); 
				$porcentagemTotal += $porcentagem;
				if($porcentagemTotal  == 49 || $porcentagemTotal  == 50  || $porcentagemTotal  == 51 ){
					$cor 	= "#FFFFFF";
					$cor1 	= "black";
				}else if($porcentagemTotal  < 48){
					$cor 	= "#cococo";
					$cor1 	= "black";
				}else if ($porcentagemTotal  > 51 ){
					$cor 	= "#FFFFFF";
					$cor1 	= "#FFFFFF";
				}
			
				
				/*
				$barra  = '<center>';
				$barra .= '	<div style="float: left;">
							<div style=" text-align:left; margin-left:5px; padding:0 0 0 0; height: 12px; max-height: 12px; width: 50px; border: 1px solid #565656; background-color:#FFFFFF; width='.$porcentagem.'% "
							<div  style="text-align:center; width: 50px; position: absolute; "><span style="color:'.$cor.'; font-size: 10px; ">'.$porcentagem.'</span><span style="color:'.$cor1.';">%</span></div>
							<img style="border-style: solid none solid solid; border-color: rgb(136, 136, 136) -moz-use-text-color rgb(136, 136, 136); border-width: 1px 1px 1px 1px; height:10px; width: '.$porcentagem.'%;" src="../../imagens/cor1.gif"/>
							</div>
							</div>
							</center>';
				*/
		
				$barra 	= '<center>';
				$barra .= '	<div style="float: left;">
							<div class="barra1">
								<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
									<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$porcentagemTotal.'<span style="color:'.$cor1.'" >%</span></div>
								</div>
							<img class="imgBarra" style="width: '.$porcentagemTotal.'%;" src="../../imagens/cor1.gif"/>
							</div>
							</div>
							</center>';
			}
		}
	}else{
				$barra  = '<center>';
				$barra .= '	<div style="float: left;">
							<div class="barra1" style=" text-align:center;">
							<span style=" color:cococo;font-size: 10px; "> 0 %</span>
							</div>
							</div>
							</center>';
				
			
	}
	
	return $barra;
	
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
function listaHistoricoMonitoramento($inuid,$ano, $arrPrsid=NULL){
	global $db;
	$convenio = 0;
	$data = new Data();

	$sql = "SELECT  '<a style=\"margin: 0 -20px 0 20px;\" onclick=\"visualizaRelatorioHistoricoMonitoramento('|| ps.prsid ||','||h.hmcid||');\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
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

	$dados 		= $db->carregar($sql);
	$porcentagemTotal = 0;
	if(is_array($dados)){
		for($cont = 0; $cont < count($dados); $cont++){
			$analise = barraListaHistoricoMonitoramento( $ano, $dados[$cont]['prsnumconvsape'], $dados[$cont]['hmcid']);

			if(!$analise['total']){
				$analise['total'] = 1;
			}
			$porcentagem = ($analise['analisado'] * 100) /  $analise['total'];
			
			if($analise['prsnumconvsape'] == $convenio ){
			}else{
				$convenio = $dados[$cont]['prsnumconvsape'];
			}
			if($dados[$cont]['hmcid'] ){
				$porcentagem = number_format($porcentagem,0);
				$porcentagem = arredondar_dois_decimal($porcentagem);
				$porcentagemTotal += $porcentagem;
				$barra  = '<center><span style="color:cococo;font-size: 10px;">';
				$barra .= $porcentagemTotal." % ";
				$barra .= '	</span>
							<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcdcff;" title="'.$porcentagemTotal.'%"
							<div style="font-size:4px; width: '.$porcentagemTotal.'%; height:6px; max-height:6px; background-color:#333399;">
							</div></div>
							</center>';
				$dados[$cont]['hmcid'] = $barra;
			}
			
			
			$cabecalho 	= array("Ação", "Nº do Convênio", "Mês", "Data","Status", "Executado %");
			if($dados[$cont]['hmcdatamonitoramento']){
				
				$dados[$cont]['mes'] = $data->formataData("01/".$dados[$cont]['hmcdatamonitoramento'], "mesTextual",'');
			}
		}
	}
	
	if(is_array($dados)){
		$lista = $db->monta_lista_simples( $dados, $cabecalho, 20, 10, 'N', '', '' );
	}
	return $lista;
	
}

function barraListaHistoricoMonitoramento($cosano, $prsnumconvsape, $hmcid){
	global $db;
	$sql = "SELECT 	SUM(total) AS total, 
					SUM(analisado) AS analisado,
					prsnumconvsape
			FROM(
				SELECT  --sum(cs.cosvlruni * cs.cosqtd) AS total,
						prsvalorconvenio as total,
						0 AS analisado,
						p.prsnumconvsape
				FROM cte.projetosape p
				INNER JOIN cte.projetosapesubacao ps ON p.prsid = ps.prsid
				INNER JOIN cte.subacaoindicador si ON si.sbaid = ps.sbaid
				INNER JOIN cte.composicaosubacao cs ON cs.sbaid = si.sbaid
				INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = p.prsid
				WHERE p.prsnumconvsape = ".$prsnumconvsape."  AND cs.cosano = ".$cosano." AND hc.hmcid = ".$hmcid."
				GROUP BY prsnumconvsape, total
				UNION ALL
				SELECT  0 AS total,
						sum(hmsvalortotalempenhado * hmsquantidadeempenhada) AS analisado,
						p.prsnumconvsape
				FROM cte.historicomonitoramentoconvenio hc
				INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
				INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
				INNER JOIN cte.projetosape p ON  hc.prsid = p.prsid
				WHERE hmsvalortotalempenhado IS NOT NULL AND hc.hmcid = ".$hmcid."
				GROUP BY p.prsnumconvsape
			) AS valores GROUP BY prsnumconvsape";
	//dbg($sql,1);
	return $db->pegaLinha($sql);
}

// ************************** FOI ******************************************************************************

/**
 * function carregaMeses($prsid, $ano);
 * @desc   : Faz a busca no banco e retorna a lista de Meses passivel de monitoramento do Convênio. 
 * @author : Thiago Tasca Barbosa
 * @param  : 
 * @return : 
 * @since 17/03/2009
 */
function carregaMeses($prsid, $ano){
	global $db;
	$temMonitoramento = 0;
	$sql = "	SELECT db.hmdmes, db.hmdano, hc.hmcid, to_char(hc.hmcdatamonitoramento, 'MM') AS mes, db.prsid , hc.hmcstatus, db.hmdid
				FROM cte.historicomonitoramentodadosbancarios db
				LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
								and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
								and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
				WHERE db.prsid = ".$prsid." and db.hmdano >= ".$ano." ORDER BY hmdano, hmdmes";
	//dbg($sql,1);
	$meses = $db->carregar($sql);
	if(is_array($meses) ){
		foreach($meses as $dados){
			if($dados['hmcid']){
				$temMonitoramento = 1;
				break;
			}
		}
	}
	
	$sql = "SELECT db.hmdano, count(hmdano) AS ano
			FROM cte.historicomonitoramentodadosbancarios db
			LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
				and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
				and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
			WHERE db.prsid = ".$prsid."  and db.hmdano >= ".$ano."
			GROUP BY db.hmdano
			ORDER BY count(hmdano) desc";
	$totalMesesPorAno = $db->carregar($sql);
	
	$conteudo = getMeses($meses, $prsid, $temMonitoramento, $totalMesesPorAno);
	return $conteudo; 
	
}

// ************************** FOI ******************************************************************************
/**
 * function getMeses;
 * @desc   : Monta a lista de itens de composição. 
 * @author : Thiago Tasca Barbosa
 * @param  : 
 * @return : 
 * @since 17/03/2009
 */
function getMeses($meses, $prsid, $temMonitoramento, $totalMesesPorAno = NULL){
	$conteudo = '
	<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
			<tr>
				<th class="texto" >Selecione um mês do convênio para iniciar o Monitoramento.</th>
			</tr>
	</table>
	<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">';
	if(is_array($meses)){
		
		$arMeses = array();
		foreach( $meses as $arDados ){
			$arMeses[$arDados["hmdano"]][] = $arDados; 
		}
		
		$nrMaior = $totalMesesPorAno[0]["ano"];
		
		foreach( $totalMesesPorAno as $arDados ){
			$arColspan[$arDados["hmdano"]] = $arDados["ano"]; 
		}
		
		global $db;
		$data = new Data();
		
		$porcentagemTotal = 0;
		
		foreach( $arMeses as $ano => $arDados ){
			
			$colspan = ( $arColspan[$ano] != $nrMaior ) ? $nrMaior - $arColspan[$ano] : 1; 
			
			$conteudo .= '<tr>';
			foreach( $arDados as $dados ){
				
				if($dados['hmcid'] == $_SESSION['hmc'][$prsid] && $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
					$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmcid'].');"';
				}else if( $dados['hmcstatus'] == "F"){
					$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmcid'].');"';
				}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'],$dados['hmdano'], 1 ) == true){
					$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].','.$dados['hmdid'].' );"';	
				}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'], $dados['hmdano'],1) == false ){
					$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].','.$dados['hmdid'].');"';
				}
				
				$mes = $data->mesTextual($dados['hmdmes']);
				$mes = substr($mes,0,3);
				$conteudo .='	<td class="SubTituloEsquerda" ><a style="cursor:pointer;" '.$btn.'>'.$mes.'/'.$dados["hmdano"].'</a></td>';
				
			}
			$conteudo .= $colspan != 1 ? '<td class="SubTituloEsquerda" colspan="'.$colspan.'">&nbsp;</td>' : "";
			$conteudo .= '</tr><tr>';
			foreach( $arDados as $dados ){
				
				$imagemStatus =  porcentagensMesesMonitoramento($_SESSION['inuid'],$_SESSION['ano'], $dados['prsid'], $dados['mes'], $porcentagemTotal);
			
				if($dados['hmcid'] == $_SESSION['hmc'][$prsid] && $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
					$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].');"';
					$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/lapis.png"/> </div>';
					
				}else if( $dados['hmcstatus'] == "F"){
					$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].');"';
					$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/check_p.gif"/> </div>';
					
				}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'],$dados['hmdano'], 1 ) == true){
					$imagem = '<div style=" position: relative; "> <img width="18" height="13"  src="../../imagens/cadeadoAberto.png"/> </div>';
					$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].','.$dados['hmdid'].');"';
					
				}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'], $dados['hmdano'],1) == false ){
					$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].', '.$dados['hmdid'].' );"';
					$imagem = '<div style=" position: relative; "><img width="13" height="13"  src="../../imagens/cadiado.png"/> </div>';
					
				}
				$conteudo .='<td style="background-color: #F5F5F5;"><a style="cursor:pointer;" onmouseout="SuperTitleOff( this );" onmousemove="SuperTitleAjax(\'../../brasilpro/monitoraFinanceiro/infoMonitora.php?hmcid='.$dados['hmcid'].'&ano='.$dados['hmdano'].'\')", this)  '.$btn.'>'.$imagemStatus.$imagem.'</a></td>';
				
			}
			$conteudo .= $colspan != 1 ? '<td style="background-color: #F5F5F5;" colspan="'.$colspan.'">&nbsp;</td>' : "";
			$conteudo .= '</tr>';
		}
		/*
		foreach($meses as $dados){
			if($dados['hmcid'] == $_SESSION['hmc'][$prsid] && $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
				$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmcid'].');"';
			}else if( $dados['hmcstatus'] == "F"){
				$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmcid'].');"';
			}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'],$dados['hmdano'], 1 ) == true){
				$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';	
			}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'], $dados['hmdano'],1) == false ){
				$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';
			}
			
			$mes = $data->mesTextual($dados['hmdmes']);
			$mes = substr($mes,0,3);
			$conteudo .='	<td class="SubTituloEsquerda" ><a style="cursor:pointer;" '.$btn.'>'.$mes.'/'.$dados["hmdano"].'</a></td>';
		}
		$conteudo .='</tr>
					 <tr>';
		$porcentagemTotal = 0;
		foreach($meses as $dados){
			$imagemStatus =  porcentagensMesesMonitoramento($_SESSION['inuid'],$_SESSION['ano'], $dados['prsid'], $dados['mes'], $porcentagemTotal);
			
			if($dados['hmcid'] == $_SESSION['hmc'][$prsid] && $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
				$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].');"';
				$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/lapis.png"/> </div>';
				
			}else if( $dados['hmcstatus'] == "F"){
				$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].');"';
				$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/check_p.gif"/> </div>';
				
			}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'],$dados['hmdano'], 1 ) == true){
				$imagem = '<div style=" position: relative; "> <img width="18" height="13"  src="../../imagens/cadeadoAberto.png"/> </div>';
				$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';
				
			}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'], $dados['hmdano'],1) == false ){
				$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';
				$imagem = '<div style=" position: relative; "><img width="13" height="13"  src="../../imagens/cadiado.png"/> </div>';
				
			}
			$conteudo .='<td style="background-color: #F5F5F5;"><a style="cursor:pointer;" onmouseout="SuperTitleOff( this );" onmousemove="SuperTitleAjax(\'../../brasilpro/monitoraFinanceiro/infoMonitora.php?hmcid='.$dados['hmcid'].'&ano='.$dados['hmdano'].'\')", this)  '.$btn.'>'.$imagemStatus.$imagem.'</a></td>';	
		}
		*/
	}else{
		$conteudo.= '<td class="SubTituloEsquerda"  style="padding-top: 10px; padding-bottom: 10px; background-color:#F5F5F5;"> Não existe meses cadastrados para ser monitorado. Entre em contato com o Gestor do Sistema. </td>';
	}
//	$conteudo .='</tr></table>';
	$conteudo .='</table>';
	return $conteudo;
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
function carregaSubacoes($prsid, $ano, $ssuid ){
	global $db;
	
	$hmcid = $_SESSION['hmc'][$prsid] == NULL ? 0 : $_SESSION['hmc'][$prsid];
	if($hmcid == 0 &&  $_SESSION['mes']){
		$sql = "SELECT hmcid
				FROM cte.historicomonitoramentoconvenio
				WHERE prsid = ".$prsid." AND to_char(hmcdatamonitoramento, 'MM') = '".$_SESSION['mes']."'";
		$hmcid =  $db->pegaUm($sql);
		
	}
	//dbg( $_SESSION['mes']);
	$sql = "SELECT  si.sbaid, 
					si.sbadsc,
					p.prsid,
					hs.ssuid,
					h.hmcid,
					hs.hmsid,
					to_char(hmcdatamonitoramento, 'MM') as mes,
					h.hmcstatus,
					ps.pssano
			FROM cte.projetosape p
			INNER JOIN cte.projetosapesubacao 				ps ON ps.prsid = p.prsid
			INNER JOIN cte.subacaoindicador 				si ON si.sbaid = ps.sbaid
			LEFT JOIN cte.historicomonitoramentoconvenio 	h  ON h.prsid  = p.prsid AND h.hmcid = ".$hmcid. "
			LEFT JOIN cte.historicomonitoramentoconvsubac 	hs ON hs.hmcid = h.hmcid AND hs.sbaid = ps.sbaid AND ps.pssano = hs.hmsano
			WHERE p.prsid = ".$prsid." 
			AND to_char(ps.pssdata, 'YYYY-MM')  <= to_char(h.hmcdatamonitoramento, 'YYYY-MM') 
			ORDER BY si.sbaid, si.sbadsc, ps.pssano -- AND p.prsano = ".$ano." AND ps.pssano = ".$ano;
	$subacoes = $db->carregar($sql);
	$conteudo = getSubacoes($subacoes, $ssuid, $prsid);
	
	return $conteudo; 
}

/**
 * function getSubacoes($subacoes, $ssuid, $prsid);
 * @desc   : Monta a lista de subações. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $subacoes (id da subação)
 * @param  : numeric $ssuid (status da subação)
 * @param  : numeric $prsid (id do convênio)
 * @return : string $conteudo (lista de subacao)
 * @since 17/03/2009
 */

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
function getSubacoes($subacoes, $ssuid, $prsid){
	global $db;
	if(is_array($subacoes) ){
		$conteudo = '<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
					<tr>
						<th>Descrição da Subações</th>
						<th>Ano de repasse</th>
						<th colspan="2" >Status</th>
					</tr>
				';
		foreach($subacoes as $subacao){
			if($ssuid != 0){
				$subacao['ssuid'] = $ssuid;
			}else{
				$subacao['ssuid'] = $subacao['ssuid'] == NULL ? '0' : $subacao['ssuid'];
			} 
			
			if($subacao["hmcstatus"] == 'F' ){
				$descricaoStatusSub = "Finalizado.";
				$imagem = "../../imagens/check_p.gif";
			}else{
				if($subacao['ssuid']){
					$sql = "select ssudescricao from cte.statussubacao where ssutipostatus = 'P' and ssuid = ".$subacao['ssuid'];
					$descricaoStatusSub = $db->pegaUm($sql);
				}else{
					$descricaoStatusSub = "Pendente";
				}
				if($subacao["ssuid"] == STATUSNAOSELECIONADO){
					$imagem =  "../../includes/dtree/img/page.gif";
				}else{
					$imagem = "../../imagens/check_p.gif";
				}
			}
	
			$subacao['hmsid'] = $subacao['hmsid'] == NULL ? 0 : $subacao['hmsid'];
			if($subacao["hmcstatus"]  == 'F' ){
				$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$prsid.','.$subacao['hmsid'].');"';	
				$onclickBtn = ' onclick = "erro(5);"';
			}else{
				if($_SESSION['hmc'][$prsid]){
					$onclickBtn = 'onclick="carregaTelaStatusSubAcao('.$subacao["sbaid"].','.$subacao["ssuid"].','.$prsid.','.$subacao['hmsid'].','.$subacao['pssano'].')"';
					if( ($subacao['ssuid'] == EXECUTADA) || ($subacao['ssuid'] == EMEXECUCAO)){
						$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$prsid.','.$subacao['hmsid'].');"';	
					}else if($subacao['ssuid'] == STATUSNAOSELECIONADO){
							//$podeEditar = 'onclick="erro(1);"';
							$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$prsid.','.$subacao['hmsid'].');"';	
					}else{
						//$podeEditar = 'onclick="erro(2);"';
						$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$prsid.','.$subacao['hmsid'].');"';	
					}
				}else{
					$podeEditar = 'onclick="carregaItensComposicao('.$subacao["sbaid"].','.$subacao['pssano'].','.$subacao['ssuid'].','.$prsid.','.$subacao['hmsid'].');"';	
					$onclickBtn = 'onclick = "erro(3);"';
				}
			}
			$conteudo .=	'<tr>
								<td class="SubTituloEsquerda" >
								<div class="div_imagens" id="img_subacoes_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'">	
									<img src="../../imagens/mais.gif" id="mais_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" name="mais_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" '.$podeEditar.' /> 
									<img src="../../imagens/menos.gif" style="display:none;" id="menos_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" name="menos_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" onclick="disableItensComposicao('.$subacao["sbaid"].$prsid.$subacao['pssano'].','.$_SESSION['ano'].');"/>
								</div><a style="cursor:pointer;" id="editarSub_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" '.$onclickBtn.'>'.$subacao["sbadsc"].'</a>
								</td>
								<td class="SubTituloEsquerda" width="8%" id="ano_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'">'.$subacao['pssano'].'</td>
								<td class="SubTituloEsquerda" width="8%" id="subStatusDesc_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'">'.$descricaoStatusSub.'</td>
								<td class="SubTituloEsquerda" id="subStatus_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" > <a style="cursor:pointer;" id="editarSub_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" '.$onclickBtn.'><img title="Sub-ação não monitorado" id="imgsubStatus_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" align="absmiddle"  src="'.$imagem.'" width="18" height="18"></a></td>
							</tr>
							 <tr>
							 	<td colspan="3">
							 		<div style="display:none;" id="linha_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'" class="linha">
										<img src="../../includes/dtree/img/line.gif"/><br>
										<img src="../../includes/dtree/img/joinbottom.gif"/>
									</div>
							 		<div class="listaItensComposicao" id="listaItensComposicao_'.$subacao["sbaid"].$prsid.$subacao['pssano'].'"></div>
							 	</td>
							 </tr>
							 ';
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
function carregaComboStatusSubacao($sbaid ,$ssuidmonitora, $prsid, $historiocoSubacao){
	global $db;
	$ssuid = $ssuidmonitora;
	$sqlStatusSubacao = "SELECT ssuid AS codigo, 
								ssudescricao AS descricao 
						 FROM cte.statussubacao 
						 WHERE ssutipostatus = 'P'
						 ORDER BY ssudescricao";
	$hmcid = $_SESSION['hmc'][$prsid] == NULL ? 0 : $_SESSION['hmc'][$prsid]; 
	$selectStatusSubacao = $db->monta_combo('ssuid',$sqlStatusSubacao,'S', 'Escolha...', 'escolhaStatusSubacao(this.value,'.$hmcid.'); dummies', '', '', '', 'N', 'ssuid', true, $ssuid);
	
	return $selectStatusSubacao;
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
function carregaItensComposicao($sbaid, $ssuid, $prsid, $hmsid, $ano){
	global $db;	
	$sql="  SELECT cs.cosid, cs.cosdsc, cs.cosqtd, cs.cosvlruni, umd.undddsc, cs.sbaid, h.hmsid, hi.hciid, hi.scsid
				   , hmsvalorunitario, hmsquantidadepaga, hmsvalortotalpago 
				   ,  to_char(hmcdatamonitoramento, 'MM') as mes , hmcstatus
			FROM cte.composicaosubacao cs 
			INNER JOIN cte.unidademedidadetalhamento umd ON umd.unddid = cs.unddid 
			LEFT JOIN cte.historicomonitoramentoconvsubac h ON h.sbaid = cs.sbaid AND h.hmsid = ".$hmsid."
			LEFT JOIN cte.historicoconvitemcomposicao hi ON hi.hmsid = h.hmsid AND hi.cosid = cs.cosid AND h.hmsid = ".$hmsid."
			LEFT JOIN cte.historicomonitoramentoconvenio hmc ON hmc.hmcid = h. hmcid
			WHERE cs.sbaid = ".$sbaid." AND cs.cosano = ".$ano;	
	//dbg($sql,1);
	$itens = $db->carregar($sql);
	$conteudo = getItensComposicao($itens, $prsid, $ssuid);
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
function getItensComposicao($itens, $prsid, $ssuid){
	$conteudo = '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tr>
						<th colspan="9" >Itens de composição</th>
					</tr>
					<tr>
						<th>Descrição dos Itens de composição</th>
						<th>Unidade de Medida</th>
						<th>Qtd.</th>
						<th>R$ Unitário</th>
						<th>R$ Total</th>
						<th>Qtd. contratada paga</th>
						<th>R$ Unitário contratado</th>
						<th>R$ Total contratado pago</th>
						<th>Foi monitorado</th>
					</tr>
				';
	$cont = 0;
	if(is_array($itens)){
		foreach($itens as $item){
			$item["hciid"] = $item['hciid'] == NULL ? 0 : $item['hciid'];
			$item["scsid"] = $item["scsid"] == NULL ? 0 : $item["scsid"]; 
			$item['hmsid'] = $item['hmsid'] == NULL ? 0 : $item['hmsid']; 
			
			if($cont == 0){
				$cor = "#e6e6e6";
				$cont= 1;
			}else{
				$cor = "#ffffff";
				$cont= 0;
			}
			
			if( $item["hmcstatus"]  == 'F' ){
				$style = 'style=""';
				$imagem = "../../imagens/check_p.gif";
				$texto = "Item monitorado";
			}else{
				if($item["scsid"] == STATUSNAOSELECIONADOITENS){
					$style = 'style="display:none;"';
					$imagem = "../../includes/dtree/img/page.gif";
					$texto = "Item não monitorado";
				}else{
					$style = 'style=""';
					$imagem = "../../imagens/check_p.gif";
					$texto = "Item monitorado";
				}
			}
			
			// FORMATANDO OS DADOS
			//$comboStatusItensComposicao = carregaComboStatusItensComposicao($item["scsid"], $item["cosid"],$item["sbaid"], $item['hmsid'],$item['hciid'], $cnvid, $item['cosqtd'], $item['cosvlruni'], $ssuid);
			$valorTotalItem 			= $item["cosqtd"] * $item["cosvlruni"];
			$valorTotalItemT 			= $item["cosqtd"] * $item["cosvlruni"];
			$valorTotalItem 			= number_format($valorTotalItem, 2, ',', '.');
			$valorUnitario 				= number_format($item["cosvlruni"], 2, ',', '.');
			$quantidade 				= number_format($item["cosqtd"], 0, '', '');
			$valorUnitarioPago 			= number_format($item["hmsvalorunitario"], 2, ',', '.');
			$valorUnitarioTotalPago 	= number_format($item["hmsvalortotalpago"], 2, ',', '.');
			$quantidadePago 			= number_format($item["hmsquantidadepaga"], 0, '', '');
			
			 
			// FIM FORMAT DADOS
			if( $item["hmcstatus"]  == 'F' ){
				$onclickBtn = ' onclick = "erro(5);"';
			}else{
				if($_SESSION['hmc'][$prsid]){
					if($item['hmsid'] != 0){
						if($ssuid == NAOINICIADA || $ssuid == CANCELADA ){
							$onclickBtn = ' onclick = "erro(2);"';
						}else{
							$onclickBtn = ' onclick = "inicialytebox(\'brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_itensComp&acao=A&cosid='.$item["cosid"].'\&sbaid='.$item["sbaid"].'\&hmsid='.$item["hmsid"].'\&hciid='.$item["hciid"].'\&quant='.$item["cosqtd"].'\&valor='.$item["cosvlruni"].'\' , \'Monitora Itens Composição\',\'800\',\'550\');"';
						}
					}else{
						$onclickBtn = ' onclick = "erro(4);"';
					}
				}else{
					$onclickBtn = ' onclick = "erro(3);"';
				}
			}
			$conteudo .=	'<tr>																																																								
								<td style="background-color:'.$cor.';" ><a style="cursor:pointer;" id="Editar_'.$item["cosid"].'" '.$onclickBtn.' >'.$item["cosdsc"].'</a></td>
								<td style="background-color:'.$cor.';">'.$item["undddsc"].'</td>
								<td style="background-color:'.$cor.';">'.$quantidade.'</td>
								<td style="background-color:'.$cor.';">R$ '.$valorUnitario.'</td>
								<td style="background-color:'.$cor.';">R$ '.$valorTotalItem.'</td>
								<td id="tdPago1_'.$item["cosid"].'" style="background-color:'.$cor.';">'.$quantidadePago.'</td> 
								<td id="tdPago2_'.$item["cosid"].'" style="background-color:'.$cor.';">R$ '.$valorUnitarioPago.'</td>
								<td id="tdPago3_'.$item["cosid"].'" style="background-color:'.$cor.';">R$ '.$valorUnitarioTotalPago.'</td>
								<td id="td_'.$item["cosid"].'" style="background-color:'.$cor.';"> <a style="cursor:pointer;" id="Editar_'.$item["cosid"].'" '.$onclickBtn.' ><img title="'.$texto.'" id="'.$item["cosid"].'" align="absmiddle"  src="'.$imagem.'" width="18" height="18"></a></td>
							</tr>
							 ';
			//$totalQtd 		+=  $quantidade;
			//$totalVlrUni 	+=  $item["cosvlruni"];
			$totalVlrI 		+=  $valorTotalItemT;
			//$totalQtdP 		+=  $quantidadePago;
			//$totalVlrUnP 	+=  $item["hmsvalorunitario"];
			$totalUniTotalP +=  $item["hmsvalortotalpago"];
		}
	}
	$conteudo .='		<tr>
							<th style="text-align: right;" >Total:</th>
							<th> </th>
							<th style="text-align:left;"></th>
							<th style="text-align:left;"></th>
							<th style="text-align:left;"> R$ '.number_format($totalVlrI, 2, ',', '.').'</th>
							<th style="text-align:left;"></th>
							<th style="text-align:left;"></th>
							<th style="text-align:left;"> R$ '.number_format($totalUniTotalP, 2, ',', '.').'</th>
							<th> </th>
						</tr>
				</table>';
	return $conteudo;
	
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
function carregaComboStatusItensComposicao($statusItem, $cosid){
	global $db;
	
	$scsid = $statusItem = $statusItem == NULL ? 0 : $statusItem; 
	$where = verificaMonitoramentoItemMesAnterior($cosid);
	$sqlStatusItensComp = "SELECT scsid   AS codigo, 
						        scsdesc AS descricao 
						 FROM cte.statuscomposicaosubacao 
						 ".$where."
						 ORDER BY scsdesc";
	$selectStatusItensComp = $db->monta_combo('scsid',$sqlStatusItensComp,'S', 'Escolha...', 'carregaEditarItensComposicao(this.value);', '', '', '', '', 'scsid', true, $scsid);
	return $selectStatusItensComp;
}

function verificaMonitoramentoItemMesAnterior($cosid){
	global $db;
	$sql = "SELECT  scsid
			FROM cte.historicoconvitemcomposicao i
			INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
			INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
			WHERE cosid = ".$cosid." AND c.hmcstatus = 'F'
			ORDER BY i.hciid DESC LIMIT 1";
	//dbg($sql,1);
	$id = $db->pegaUm($sql);
	$where = " ";
	if($id == EXECUTADOITEMCOMP ){
		$where = " WHERE scsid not in ('".EMEXECUÇÃOITEMCOMP."', '".NAOINICIADAITEMCOMP."', '".EMPLANEJAMENTOITEMCOMP."') ";
	} 
	return $where;
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
function recuperaDadosPopupItensComp($sbaid, $cosid){
	global $db;
	$sql = "SELECT  d.dimcod,
					d.dimdsc,
					ad.ardcod,
					ad.arddsc,
					i.indcod,
					i.inddsc,
					s.sbadsc,
					cs.cosdsc
		FROM cte.dimensao d
		INNER JOIN cte.areadimensao 	  	ad  ON ad.dimid = d.dimid
		INNER JOIN cte.indicador 	  		i   ON i.ardid  = ad.ardid
		INNER JOIN cte.criterio		  		c   ON c.indid  = i.indid
		INNER JOIN cte.pontuacao 	  		p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
		INNER JOIN cte.instrumentounidade 	iu  ON iu.inuid = p.inuid
		INNER JOIN cte.acaoindicador 	  	a   ON a.ptoid  = p.ptoid
		INNER JOIN cte.subacaoindicador   	s   ON s.aciid  = a.aciid
		INNER JOIN cte.composicaosubacao  	cs  ON cs.sbaid = s.sbaid
		WHERE s.sbaid = ".$sbaid." AND cosid = ".$cosid;
	$acaind 	= $db->pegaLinha($sql);
	return $acaind;
}

/**
 * function recuperaHistoricoMonitoramentoPagItens($cosid);
 * @desc   : carrega lista de historico de monitoramento do item
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $cosid (id do item de composição)
 * @return : array $acaind (resultado da consulta)
 * @since 24/03/2009
 */
function recuperaHistoricoMonitoramentoPagItens($cosid){
	global $db;
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
			WHERE cosid = ".$cosid;
	$listaHistorico = $db->carregar($sql);
	return $listaHistorico;
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
	global $db;
	$micid = $micid;
	$sqlStatusMotivoItensComp = "SELECT micid   		AS codigo, 
							      		micdescricao 	AS descricao 
							 	 FROM cte.motivoitemcomposicao
							 	 ORDER BY micdescricao";
	$selectStatusMotivoItensComp = $db->monta_combo('micid',$sqlStatusMotivoItensComp,'S', 'Escolha...', '', '', '', '', 'S', 'micid', true, $micid);
	return $selectStatusMotivoItensComp;
}
?>