<?php
/*
class Regrasfinanceiro extends Modelo{
	protected $ano 		= '';
	protected $mes 		= '';
	protected $status 	= '';
	
	public function existeDadosBancariosCadastrado(){
		
	}
	
	public function podeIniciarMonitoramento(){
		
	}
///////////////// VERIFICAR ////////////////
	public function validaSePodeInserirMonitoramento($prsid, $mesTela, $anoTela = NULL, $return = NULL){
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
	 * function carregaMonitoramentosParaSession($prsid, $hmcid);
	 * @desc   : Carrega os monitoramentos com status de iniciados para session. 
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
	
	
	
}
*/
?>