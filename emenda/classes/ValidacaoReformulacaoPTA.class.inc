<?php

// Classe de validação das informações cadastradas no 
// Plano de Trabalho do sistema de Emendas.
class ValidacaoReformulacaoPTA {
	
	// variável de conexão com o banco
	private	$db;
	
	// chave primária do PTA. Recebida obrigatoriamente por parâmetro
	private $ptrid;
	
	// dados da view 'v_planotrabalho'
	private $entid;
	private $ptrexercicio;
	private $resid;
	private $ptrcontrapartida;
	private $ptrvalorconcedente;
	private $ptrvalorproponente;
	private $ptrvalortotal;
	private $ptivalorconcedente;
	private $ptivalorproponente;
	private $ptivalortotal;
	
	// array com os dados do JOIN entre 'emenda.iniciativa' e 'emenda.v_ptiniciativa'
	private $iniciativas = array();
	
	// array para registro de todos as pendências encontradas
	public $retorno = array();
	
	// método construtor
	public function __construct( $ptrid ) {
		$this->ptrid = $ptrid;
		
		global $db;
		$this->db = ( ($db) ? $db : die("Ocorreu um erro com a conexão do Banco de Dados.") );
		
		$sql = "SELECT * FROM emenda.v_planotrabalho WHERE ptrid = ".$ptrid;
		$dados = $db->carregar($sql);
		
		if($dados) {
			$this->entid				= $dados[0]["entid"];
			$this->ptrexercicio 		= $dados[0]["ptrexercicio"];
			$this->resid 				= $dados[0]["resid"];
			$this->ptrcontrapartida 	= $dados[0]["ptrcontrapartida"];
			$this->ptrvalorconcedente	= $dados[0]["ptrvalorconcedente"];
			$this->ptrvalorproponente 	= $dados[0]["ptrvalorproponente"];
			$this->ptrvalortotal 		= $dados[0]["ptrvalortotal"];
			$this->ptivalorconcedente 	= $dados[0]["ptivalorconcedente"];
			$this->ptivalorproponente 	= $dados[0]["ptivalorproponente"];
			$this->ptivalortotal 		= $dados[0]["ptivalortotal"];
			$this->ptrcontrapartida		= $dados[0]["ptrcontrapartida"];
		}	
		
		$sql = "SELECT
					*
				FROM
					emenda.v_ptiniciativa vpti
				INNER JOIN
					emenda.iniciativa ini ON ini.iniid = vpti.iniid
				WHERE
					vpti.ptrid = ".$ptrid;
		
		$dados = $db->carregar($sql);
		
		if($dados) {
		  for($i=0; $i<count($dados); $i++) {
			$this->iniciativas[$i]['ptiid']						= $dados[$i]['ptiid'];
			$this->iniciativas[$i]['ptrid']						= $dados[$i]['ptrid'];
			$this->iniciativas[$i]['ptidescricao'] 				= $dados[$i]['ptidescricao'];
			$this->iniciativas[$i]['iniid'] 					= $dados[$i]['iniid'];
			$this->iniciativas[$i]['tpeid'] 					= $dados[$i]['tpeid'];
			$this->iniciativas[$i]['ptivalorproponente']		= $dados[$i]['ptivalorproponente'];
			$this->iniciativas[$i]['ptivalorconcedente'] 		= $dados[$i]['ptivalorconcedente'];
			$this->iniciativas[$i]['ptivalortotal'] 			= $dados[$i]['ptivalortotal'];
			$this->iniciativas[$i]['ininome']					= $dados[$i]['ininome'];
			$this->iniciativas[$i]['temid'] 					= $dados[$i]['temid'];
			$this->iniciativas[$i]['resid'] 					= $dados[$i]['resid'];
			$this->iniciativas[$i]['gndcod'] 					= $dados[$i]['gndcod'];
			$this->iniciativas[$i]['inistatus'] 				= $dados[$i]['inistatus'];
			$this->iniciativas[$i]['iniescolasbeneficiasdas'] 	= $dados[$i]['iniescolasbeneficiasdas'];
		  }
		}
	}
	
	public function validacaoPlanoTrabalho() {
		$this->validaIniciativas();
		$this->validaCronograma();
		$this->validaInsereEscolasBeneficiario( true );
				
		return $this->retorno;
	}
	
	private function validaEscolasBeneficiadas() {
		$this->retorno['escolasbeneficiadas'] = array();
		
		$exigeCadastroEscolaBeneficiada = false;
		
		if(!empty($this->iniciativas)) {
			for($i=0; $i<count($this->iniciativas); $i++) {
				if($this->iniciativas[$i]['iniescolasbeneficiasdas'] == 't') {
					$exigeCadastroEscolaBeneficiada = true;
				}
			}
		}
		
		if($exigeCadastroEscolaBeneficiada) {
			$sql = "SELECT
						count(*)
					FROM
						emenda.ptescolasbeneficiadas
					WHERE
						ptrid = ".$this->ptrid;

			$valida = $this->db->pegaUm($sql);
			
			if($valida == 0) array_push($this->retorno['escolasbeneficiadas'], "Deve ser cadastrada pelo menos uma Escola Beneficiada.");
		}
	}
	
	private function validaCronograma() {
		$this->retorno['cronograma'] = array();
		
		if($this->validaParcelasConcedente()) {
			$this->validaSomaParcelasIniciativaConcedente();
		}
		//if( $_SESSION['emenda']['federal'] == 'N' ){
			if($this->validaParcelasProponente()) {
				$this->validaSomaParcelasIniciativaProponente();
			}
		//}
	}
	
	private function validaParcelasConcedente() {
		$sql = "SELECT
					count(*) as parcelas_concedente
				FROM
					emenda.ptparceladesembolso
				WHERE
					ptrid = ".$this->ptrid." and
					prdtipo = 'C' and
					prdminuta = 'P'";
		$valida = $this->db->pegaUm($sql);
		
		if($valida == 0) { 
			array_push($this->retorno['cronograma'], "É necessário informar as parcelas de desembolso do concedente.");
			return false;
		} else {
			return true;
		}
	}
	
	private function validaParcelasProponente() {
		$sql = "SELECT
					count(*) as parcelas_proponente
				FROM
					emenda.ptparceladesembolso
				WHERE
					ptrid = ".$this->ptrid." and
					prdtipo = 'P' and
					prdminuta = 'P'";
		$valida = $this->db->pegaUm($sql);
		
		if($valida == 0) {
			array_push($this->retorno['cronograma'], "É necessário informar as parcelas de desembolso do proponente.");
			return false;
		} else {
			return true;
		}
	}
	
	private function validaSomaParcelasIniciativaConcedente() {
		if(!empty($this->iniciativas)) {
			
			$existePendencia = true;
			
			for($i=0; $i<count($this->iniciativas); $i++) {
				$sql = "SELECT
							sum(pri.privalor) as privalor
						FROM
							emenda.ptparcelainiciativa pri
						INNER JOIN
							emenda.ptparceladesembolso prd ON prd.prdid = pri.prdid
														  AND prd.prdtipo = 'C'
														  AND prdminuta = 'P'
						WHERE
							pri.ptiid = ".$this->iniciativas[$i]["ptiid"];

				$concedente = $this->db->pegaUm($sql);
				
				if($concedente != $this->iniciativas[$i]['ptivalorconcedente']) {
					if($existePendencia) {
						$existePendencia = false;
						array_push($this->retorno['cronograma'], "Pendências encontradas nas parcelas do concedente:");
					}
					
					array_push($this->retorno['cronograma'], "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O total das parcelas da iniciativa '".$this->iniciativas[$i]["ininome"]."' deve ser igual ao valor total da iniciativa.");
				}
			}
		}
	}
	
	private function validaSomaParcelasIniciativaProponente() {
		if(!empty($this->iniciativas)) {
			
			$existePendencia = true;
			
			for($i=0; $i<count($this->iniciativas); $i++) {
				$sql = "SELECT
							sum(pri.privalor) as privalor
						FROM
							emenda.ptparcelainiciativa pri
						INNER JOIN
							emenda.ptparceladesembolso prd ON prd.prdid = pri.prdid
														  AND prd.prdtipo = 'P'
														  AND prdminuta = 'P'
						WHERE
							pri.ptiid = ".$this->iniciativas[$i]["ptiid"];
				
				$proponente = $this->db->pegaUm($sql);
				
				if($proponente != $this->iniciativas[$i]['ptivalorproponente']) {
					if($existePendencia) {
						$existePendencia = false;
						array_push($this->retorno['cronograma'], "Pendências encontradas nas parcelas do proponente:");
					}
					
					array_push($this->retorno['cronograma'], "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O total das parcelas da iniciativa '".$this->iniciativas[$i]["ininome"]."' deve ser igual ao valor total da iniciativa.");
				}
			}
		}
	}
	
	private function validaIniciativas() {
		$this->retorno['iniciativas'] = array();
					
	//	$this->validaBeneficiariosObrigatorios();
		$this->validaCamposObrigatoriosEspecificacoes();
		$this->validaEspecificacoesCadastroKit();
		$this->validaSomaValoresEspecificacaoKit();
		$this->validaSomaRecursosValorConcedente();
		$this->validaSomaRecursosEmendaDetalheEntidade();
	}

	public function validaInsereEscolasBeneficiario( $boFinaliza = false ){
		$this->retorno['escolasbeneficiadas'] = array();
		
		$sql = "SELECT 
					(ptbquantidaderural + ptbquantidadeurbana) as total
				FROM 
					emenda.ptiniciativa pti
				INNER JOIN 
					emenda.iniciativabeneficiario icb ON pti.iniid = icb.iniid
									and icb.icbstatus = 'A'
				INNER JOIN 
					emenda.beneficiario ben ON ben.benid = icb.benid
				INNER JOIN 
					emenda.ptiniciativabeneficiario ptb ON ptb.icbid = icb.icbid
					AND ptb.ptiid = pti.ptiid
				WHERE
					ben.benid = 20
					AND ( ptbquantidaderural <> 0 OR ptbquantidadeurbana <> 0 )
					AND pti.ptrid = ".$this->ptrid;

		$valida = $this->db->pegaUm($sql);

		if( $boFinaliza ){
			if($valida <> 0){
				$escolas = $this->validaEscolasBeneficiarioCadastrada();
				if( $escolas <> $valida ){
					array_push($this->retorno['escolasbeneficiadas'], "Deve(m) ser cadastrada(s) $valida Escola(s) Beneficiada(s). Você tem $escolas escola(s) cadastrada(s).");
				}
			}
		} else {
			return $valida;
		}
	}
	
	private function validaEscolasBeneficiarioCadastrada(){
		$sql = "SELECT
					count(pte.esbid)
				FROM
					emenda.ptescolasbeneficiadas pte
				WHERE
					pte.ptrid = ".$this->ptrid;

		return $this->db->pegaUm($sql);
	}
	
	/*private function validaBeneficiariosObrigatorios() {
		$sql = "SELECT 
					ben.bennome,
					ptb.ptbquantidaderural,
					ptb.ptbquantidadeurbana,
					ini.ininome
				FROM 
					emenda.ptiniciativa pti
				INNER JOIN
					emenda.iniciativa ini ON ini.iniid = pti.iniid
				INNER JOIN 
					emenda.iniciativabeneficiario icb ON pti.iniid = icb.iniid
													 AND icb.icbobrigatorio = true
													 and icb.icbstatus = 'A'
				INNER JOIN 
					emenda.beneficiario ben ON ben.benid = icb.benid
				LEFT JOIN 
					emenda.ptiniciativabeneficiario ptb ON ptb.icbid = icb.icbid
					and ptb.ptiid = pti.ptiid
				WHERE
					pti.ptrid = ".$this->ptrid;
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				$vInformado = false;
				
				if( ($valida[$i]['ptbquantidaderural'] != NULL && $valida[$i]['ptbquantidaderural'] > 0) ||
				    ($valida[$i]['ptbquantidadeurbana'] != NULL && $valida[$i]['ptbquantidadeurbana'] > 0) ){
					$vInformado = true;
				}
					
				if(!$vInformado) array_push($this->retorno['iniciativas'], "É necessário informar a quantidade de '".$valida[$i]['bennome']."' beneficiados(as) da iniciativa '".$valida[$i]['ininome']."'. Para informá-la, vá até a aba 'Reformulação PTA', selecione a iniciativa '".$valida[$i]['ininome']."' e clique na aba 'Beneficiário Reformulação'.");
			}
		}
	}*/
	
	private function validaCamposObrigatoriosEspecificacoes() {
		$sql = "SELECT
					pte.ptequantidade,
					pte.ptevalorunitario,
					pte.ptedatainicio,
					pte.ptedatafim,
					esp.espnome,
					ini.ininome
				FROM
					emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE
					pti.ptrid = ".$this->ptrid."
				ORDER BY 
					pti.ptiid";
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			$texto = "";
			$arrCampos = array();
			
			for($i=0; $i<count($valida); $i++) {
				if(!$valida[$i]['ptequantidade'] || $valida[$i]['ptequantidade'] == 0 || !$valida[$i]['ptevalorunitario'] || 
				   $valida[$i]['ptevalorunitario'] == 0 || !$valida[$i]['ptedatainicio'] || !$valida[$i]['ptedatafim'])
				{
					$texto .= "Na iniciativa '".$valida[$i]['ininome']."', especificação '".$valida[$i]['espnome']."', é necessário informar os seguintes campos: ";
					
					if(!$valida[$i]['ptequantidade'] || $valida[$i]['ptequantidade'] == 0)
						array_push($arrCampos, "'Quantidade'");
					
					if(!$valida[$i]['ptevalorunitario'] || $valida[$i]['ptevalorunitario'] == 0)
						array_push($arrCampos, "'Valor Unitário'");
						
					if(!$valida[$i]['ptedatainicio'])
						array_push($arrCampos, "'Data de Início'");
						
					if(!$valida[$i]['ptedatafim'])
						array_push($arrCampos, "'Data Fim'");
						
					$texto .= implode(',', $arrCampos) . ".";
					array_push($this->retorno['iniciativas'], $texto);
				}
			}
		}
	}
	
	private function validaEspecificacoesCadastroKit() {
		$sql = "SELECT
					esp.espnome,
					ini.ininome
				FROM
					emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid AND esp.espkit = true
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					LEFT JOIN emenda.ptitemkit ptk ON ptk.pteid = pte.pteid
				WHERE
					ptk.ptkid is null
					AND pti.ptrid = ".$this->ptrid."
				ORDER BY pti.ptiid";

		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', é necessário informar o itens que a compõem.");
			}
		}
	}
	
	private function validaSomaValoresEspecificacaoKit() {
		$sql = "SELECT
					sum(vptk.ptkvalortotal) as soma_kit,
					pte.pteid,
					pte.ptequantidade,
					pte.ptevalorunitario,
					esp.espnome,
					ini.ininome
				FROM
					emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid AND esp.espkit = true
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					INNER JOIN emenda.v_ptitemkit vptk ON vptk.pteid = pte.pteid
				WHERE
					pti.ptrid = ".$this->ptrid."
				GROUP BY
					pte.pteid, pte.ptequantidade, pte.ptevalorunitario, esp.espnome,ini.ininome";
		
		$valida = $this->db->carregar($sql);
				
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				if($valida[$i]["ptequantidade"] != 1)
					array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', o campo 'quantidade' deve ser igual a 1");
					
				if($valida[$i]["soma_kit"] != $valida[$i]["ptevalorunitario"])
					array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', o campo 'valor unitário' deve ser igual ao valor total dos itens que compõem a especificação.");
			}
		}
	}

	private function validaSomaRecursosValorConcedente() {
		$sql = "SELECT
					sum(COALESCE(per.pervalor,0)) as pervalorespecificacao,
					pte.pteid,
					ptevalorconcedente,
					espnome,
					ini.ininome
				FROM
					emenda.v_ptiniciativaespecificacao pte
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					INNER JOIN emenda.ptiniciativa pti ON pti.ptiid = pte.ptiid
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.planotrabalho ptr ON pti.ptrid = ptr.ptrid
	 				LEFT JOIN emenda.ptiniesprecurso per ON pte.pteid = per.pteid
				WHERE
					pte.ptestatus = 'A' AND
					ptr.ptrid = ".$this->ptrid."
				GROUP BY
					pte.pteid, pte.ptevalorconcedente, espnome, ini.ininome";

		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				if($valida[$i]["pervalorespecificacao"] != $valida[$i]["ptevalorconcedente"]) {
					array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', o valor debitado das emendas deve ser igual ao valor do concedente da especificação");
				}
			}
		}
	}
		
	private function validaSomaRecursosEmendaDetalheEntidade() {
		$sql = "SELECT
					sum(COALESCE(per.pervalor,0)) as pervaloremenda,
					ped.pedid,
					ped.pedvalor,
					ef.fupdsc
				FROM
					emenda.ptemendadetalheentidade ped
				INNER JOIN
					emenda.emendadetalheentidade ede ON ede.edeid = ped.edeid
				INNER JOIN
					emenda.emendadetalhe emd ON emd.emdid = ede.emdid
				INNER JOIN
					emenda.emenda eme ON eme.emeid = emd.emeid
				INNER JOIN
					emenda.v_funcionalprogramatica ef ON ef.acaid = eme.acaid 
				LEFT JOIN
					emenda.ptiniesprecurso per ON ped.pedid = per.pedid
				WHERE
					ped.ptrid = ".$this->ptrid."
				GROUP BY
					ped.pedid,
					ped.pedvalor,ef.fupdsc";
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				if($valida[$i]["pervaloremenda"] > $valida[$i]["pedvalor"]) {
					array_push($this->retorno['iniciativas'], "O valor utilizado da emenda '".$valida[$i]["fupdsc"]."' no Plano de Trabalho não pode ser superior ao valor da emenda.");
				}
			}
		}
	}
}

?>