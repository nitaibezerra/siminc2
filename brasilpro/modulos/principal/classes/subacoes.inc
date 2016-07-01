<?php
//$solicito = array("qtdCronograma","beneficiarios"); ou $solicito = "qtdCronograma";
//$subacoes = array(1639980,1640410); ou $subacoes = 1639980;
//$ano = 2008; ou $ano = array(2008,2009);
//$dimensao = array(1,2) ou $dimensao = 1;
//$area = 2; ou $area = array(2,3);
//$indicador = 1; ou $indicador = array(1,2);
//$formaExecucao = 3; ou $formaExecucao = array(2,3);

/*******************************
* 	CLASSE: dbSubacoes 
* 	17/11/2008
*   CRIADA POR: Thiago Tasca
*   DESCRI플O:
*	Classe de persistencia para recuperar os dados do BD.
* 
*******************************/

class dbSubacoes extends cls_banco{
	
	/*******************************
	* 	FUNCTION: recuperaQuantitativosDoCronograma() 
	* 	17/11/2008
	*   CRIADA POR: Thiago Tasca
	*   DESCRI플O:
	*	Recupera os quantitativos dos cronogramas da forma solicitada.
	* 
	*******************************/
	
	function recuperaQuantitativosDoCronograma($dimensao, $area, $indicador, $subacao, $anos, $formaExecucao){
		$inner = "";
		$where = "";
		if($subacao != NULL){
			$selectInterno = "s.sbaid,";
			$selectExterno = "sbaid,";
			$where .= " AND s.sbaid in (".$subacao.")";
			$groupbyInterno = ",s.sbaid";
			$groupbyExterno = ",sbaid";
		}
		if($anos != NULL){
			$inner .= " AND sptano in(".$anos.")";
		}
		if($dimensao != NULL){
			$where .= " AND d.dimcod in(".$dimensao.")";
		}
		if($area != NULL){
			$where .= " AND ad.ardcod in(".$area.")";
		}
		if($indicador != NULL){
			$where .= " AND indcod in (".$indicador.")";
		}
		if($formaExecucao != NULL){
			$where .= " AND iu.itrid in (".$formaExecucao.")";
		}
		
		$sql="SELECT 	
			".$selectExterno."
			dimcod,
			ardcod,
			indcod,
			sum(quantidadePorEscola + quantidadeGlobal) as soma
		FROM (
			SELECT	
				".$selectInterno."
				d.dimcod,
				ad.ardcod,
				i.indcod,
				0 as quantidadeGlobal,
				sum(coalesce(qf.qfaqtd,0)) as quantidadePorEscola
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
				INNER JOIN cte.qtdfisicoano qf ON qf.sbaid = s.sbaid 
				LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid AND qf.qfaano = spt.sptano ".$inner."
			WHERE
				 iu.inuid = ".$_SESSION['inuid']."		
				 AND s.sbaporescola = true -- por escola.
				 ".$where."
			GROUP BY
				 d.dimcod,
				 ad.ardcod,
				 i.indcod
				 ".$groupbyInterno."
			UNION ALL
			-- GLOBAL --
			SELECT	
				".$selectInterno."
				d.dimcod,
				ad.ardcod,
				i.indcod,
				sum(coalesce(spt.sptunt,0)) as quantidadeGlobal,
				0 as quantidadePorEscola
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
				LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid ".$inner."
			WHERE
				 iu.inuid = ".$_SESSION['inuid']."
				 AND s.sbaporescola = false -- global.
				 ".$where."
			GROUP BY
				 d.dimcod,
				 ad.ardcod,
				 i.indcod
				 ".$groupbyInterno."
		) AS foo
		GROUP BY 	
			dimcod,
			ardcod,
			indcod
			".$groupbyExterno."
		ORDER BY 
			dimcod,
			ardcod,
			indcod
			";
		$dados = $this->carregar($sql);
		return $dados;
	}
	
	/*******************************
	* 	FUNCTION: recuperaBeneficiarios() 
	* 	18/11/2008
	*   CRIADA POR: Thiago Tasca
	*   DESCRI플O:
	*	Recupera os beneficiarios das suba寤es da forma solicitada.
	* 
	*******************************/
	function recuperaBeneficiarios($dimensao, $area, $indicador, $subacao, $anos, $formaExecucao){
		$where = "";
		if($subacao != NULL){
			$where .= " sb.sbaid in (".$subacao.")";
		}
		if($anos != NULL){
			$where .= " AND sb.sabano in(".$anos.")";
		}
		$sql= "SELECT  b.benidfnde  AS codigobeneficiario,
					   b.bendsc     AS descricaobeneficiario,
				       sb.vlrurbano AS quantidadezonarural ,
				       sb.vlrrural  AS quantidadezonaurbana
				FROM cte.subacaobeneficiario sb
				INNER JOIN cte.beneficiario b ON b.benid = sb.benid
				WHERE ".$where."
				ORDER BY sb.benid";
		$dados = $this->carregar($sql);
		return $dados;
	}
	
}

/*******************************
* 	CLASSE: subacoes
* 	17/11/2008
*   CRIADA POR: Thiago Tasca
*   DESCRI플O:
*	Classe de controle
* 
*******************************/
class subacoes extends dbSubacoes{
	
	var $dimensao 		= NULL; 
	var $area 			= NULL; 
	var $indicador 		= NULL; 
	var $subacoes 		= NULL; 
	var $anos 			= NULL; 
	var $formaExecucao 	= NULL;
	
	/*******************************
	* 	FUNCTION: construct
	* 	17/11/2008
	*   CRIADA POR: Thiago Tasca
	*   DESCRI플O:
	*	Controle para montar um array com o que e solicitado.
	* 
	* 	@PARAM  $solicitado 	-   integer ou Array - OBS: Este valor e utilizado para passar o que o usuario deseja 
	* 			$dimensao		- 	integer ou Array
	* 			$area			- 	integer ou Array
	* 			$indicador  	- 	integer ou Array
	* 			$subacoes		-   integer ou Array
	* 			$anos			-   integer ou Array
	* 			$formaExecucao 	-   integer ou Array
	* 
	*******************************/
	function  construct($solicitado, $dimensao, $area, $indicador, $subacoes, $anos, $formaExecucao){
		
		$this->dimensao = $dimensao;
		$this->area = $area;
		$this->indicador = $indicador;
		$this->subacoes = $subacoes;
		$this->anos = $anos;
		$this->formaExecucao = $formaExecucao;
		
		if(is_array($this->subacoes)){
			$this->subacoes = implode(",", $this->subacoes);
		}
		if(is_array($this->anos)){
			$this->anos = implode(",", $this->anos);
		}
		if(is_array($this->dimensao)){
			$this->dimensao = implode(",", $this->dimensao);
		}
		if(is_array($this->area)){
				$this->area = implode(",", $this->area);
		}
		if(is_array($this->indicador)){
				$this->indicador = implode(",", $this->indicador);
		}
		if(is_array($this->formaExecucao)){
				$this->formaExecucao = implode(",", $this->formaExecucao);
		}
		
		if(is_array($solicitado)){
			$total = count($solicitado);
			for($cont = 0; $cont < $total; $cont++){
				$executar = $solicitado[$cont];
				switch($executar) {
					case 'qtdCronograma':
						$dados["qtdCronograma"] = $this->quantitativosDoCronograma($this->dimensao, $this->area, $this->indicador, $this->subacoes, $this->anos, $this->formaExecucao);
						break;
					case 'beneficiarios':
						$dados["beneficiarios"] = $this->beneficiarios($this->dimensao, $this->area, $this->indicador, $this->subacoes, $this->anos, $this->formaExecucao);
						break;  
				}
			}
		}else{
			switch($solicitado) {
				case 'qtdCronograma':
					$dados["qtdCronograma"] = $this->quantitativosDoCronograma($this->dimensao, $this->area, $this->indicador, $this->subacoes, $this->anos, $this->formaExecucao);
					break;
				case 'beneficiarios':
					$dados["beneficiarios"] = $this->beneficiarios($this->dimensao, $this->area, $this->indicador, $this->subacoes, $this->anos, $this->formaExecucao);
					break; 
			}
		}
		return $dados;
		
	}
	
	function quantitativosDoCronograma($dimensao, $area, $indicador, $subacoes, $anos, $formaExecucao){
		$dados = $this->recuperaQuantitativosDoCronograma($dimensao, $area, $indicador, $subacoes, $anos, $formaExecucao);
		return $dados;
	}

	function beneficiarios($dimensao, $area, $indicador, $subacoes, $anos, $formaExecucao){
		$dados = $this->recuperaBeneficiarios($dimensao, $area, $indicador, $subacoes, $anos, $formaExecucao);
		return $dados;		
	}
	
	function quantitativosDosItensComposicao(){
		
	}
	
	function valorDosItensComposicao(){
		
	}
	
	
}
?>