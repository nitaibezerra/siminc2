<?php
class ParametroFiltroConsulta 
{
	private $condicoesWhere;
	private $httpPost;
	
	public function ParametroFiltroConsulta($httpPost) {
		$this->condicoesWhere = array();
		$this->httpPost = $httpPost;
	}

	function addCondicaoWhereCompleta($subClausulaWhere) {
		$this->condicoesWhere["none"] = $subClausulaWhere;
	}
	
	function addCondicaoWhereConformeFiltro($subClausulaWhere, $nomeAtributo) {
		if( $this->estaPreenchido($nomeAtributo) ) {
			$this->addClausulaIn($subClausulaWhere, $nomeAtributo);
		}
	}
	
	function addClausulaIn($subClausulaWhere, $nomeAtributo) {
		$valoresSeparadosPorVirgula = implode( "','", $this->lerParametroHttp($nomeAtributo) );
		$this->condicoesWhere[$nomeAtributo] = $subClausulaWhere . " IN  ('" . $valoresSeparadosPorVirgula . "') ";
	}
	
	function estaPreenchido($nomeAtributo) {
		$campoFlag = $this->httpPost[$nomeAtributo."_campo_flag"];
		$campoFlagAtivo = $campoFlag || $campoFlag=='1' ;
		$campoValoresPreenchidos = $this->httpPost[$nomeAtributo][0];
		
		if( $campoFlagAtivo && $campoValoresPreenchidos ) {
			return true;			
		}
		return false;
	}
	
	function lerParametroHttp($nomeAtributo) {
		return $this->httpPost[$nomeAtributo];
	}		
		
	// TODO Eliminar esta ... ficar com só 'verificar se valor presente ' e 'acumular valores ... ' 
	function addCondicoesWhereConformeArray($subClausulaWhere, $nomeAtributo, $arraySubstituicaoValores) {
		if( $this->estaPreenchido($nomeAtributo) ) {
			$arrayValoresOriginais = $this->lerParametroHttp($nomeAtributo); 
			foreach( $arraySubstituicaoValores as $chave=>$valorSubstituto ) {
				if( in_array($chave,$arrayValoresOriginais) ) {
					$arrayResultado[] = $valorSubstituto;					
				}
			}
			$valoresSeparadosPorVirgula = implode( "','", $arrayResultado );
			$this->condicoesWhere[$nomeAtributo] = $subClausulaWhere . " IN  ('" . $valoresSeparadosPorVirgula . "') ";
		}
	}	
	
	// TODO Eliminar esta ... ficar com só 'verificar se valor presente ' e 'acumular valores ... ' 
	function addCondicoesWhereBaseadoVetorDeChaves($subClausulaWhere, $vetorNomesAtributos = array()) {
		foreach( $vetorNomesAtributos as $nomeAtributo ) {
			if( $this->estaPreenchido($nomeAtributo) ) {
				$haValorAcumulado = true;
				$valoresSeparadosPorVirgula = implode( "','", $this->lerParametroHttp($nomeAtributo) );
				$acumuladorValores .= "'" . $valoresSeparadosPorVirgula . "' ";
			} 
		}
		if( $haValorAcumulado ) {
			$this->condicoesWhere[$nomeAtributo] = $subClausulaWhere . " IN  (" . $acumuladorValores . ") ";
		}
	}	
	
	function getCondicoesWhere() {
		return implode( "  ", $this->condicoesWhere ); 
	}
	
	function getCondicaoWhereConformeNomeAtributo($nomeAtributo) {
		return $this->condicoesWhere[$nomeAtributo];
	}
	
	// mudar ... trabalhar com array e não .... if ($nomeAttributo is array ... ) 
	function verifiqueSeValorPresenteNoAtributo($valorEsperado,$nomeAtributo) {
		if( $this->estaPreenchido($nomeAtributo) ) {
			$valoresPresentes  = $this->lerParametroHttp($nomeAtributo); 
			return in_array($valorEsperado,$valoresPresentes);
		}
		return false;
	}
}
?>