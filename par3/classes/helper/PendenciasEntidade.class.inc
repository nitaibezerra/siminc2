<?php

class Par3_Helper_PendenciasEntidade extends Controle
{

	public function __construct()
	{
		parent::__construct();
		$GLOBALS['nome_bd'] = '';
		$GLOBALS['servidor_bd'] = '';
		$GLOBALS['porta_bd'] = '';
		$GLOBALS['usuario_db'] = '';
		$GLOBALS['senha_bd'] = '';
	}

	public function recuperarPendenciasEntidade($inuid)
	{
		
		$oInstrumento = new Par3_Model_Pendencias();
		
		$GLOBALS['nome_bd'] = '';
		$GLOBALS['servidor_bd'] = '';
		$GLOBALS['porta_bd'] = '';
		$GLOBALS['usuario_db'] = '';
		$GLOBALS['senha_bd'] = '';
		
		
		$instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
		$estuf = $instrumentoUnidade->estuf;
		$muncod = $instrumentoUnidade->muncod;
		$itrid = $instrumentoUnidade->itrid;

		$GLOBALS['estuf'] = $estuf;
		$GLOBALS['muncod'] = $muncod;
		
		$arr = $oInstrumento->recuperarPendenciasEntidade($inuid, $estuf, $muncod, $itrid);
		
		$arrAux = array();
		
		if( is_array( $arr) ){
			
			foreach( $arr as $key => $inputAux ){
				
				$arrAux[$inputAux['pendencia']][$inputAux['estuf']][$inputAux['muncod']][$inputAux['obrid']] = $inputAux;
			}
		}
		return $arrAux;
	}
	
	public function recuperarPendenciasLista($inuid)
	{
		$oInstrumento = new Par3_Model_Pendencias();
	
		$arr = $oInstrumento->recuperarPendenciasEntidade($inuid);
	
		return $arr;
	}
	
	public function recuperarTiposPendencia()
	{
		$oInstrumento = new Par3_Model_Pendencias();
	
		$arr = $oInstrumento->recuperarTiposPendencia();

		return $arr;
	}
	
	public function existePendenciaPorTipo($pendencia, $inuid)
	{
		$oInstrumento = new Par3_Model_Pendencias();
	
		$GLOBALS['nome_bd'] = '';
		$GLOBALS['servidor_bd'] = '';
		$GLOBALS['porta_bd'] = '';
		$GLOBALS['usuario_db'] = '';
		$GLOBALS['senha_bd'] = 'o';
		
		
		$instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
		$estuf = $instrumentoUnidade->estuf;
		$muncod = $instrumentoUnidade->muncod;
		$itrid = $instrumentoUnidade->itrid;
		
		$GLOBALS['estuf'] = $estuf;
		$GLOBALS['muncod'] = $muncod;
		
		$exists = $oInstrumento->existePendenciaPorTipo($pendencia, $inuid, $estuf, $muncod, $itrid);

		if( $exists ){
			return true;
		}else{
			return false;
		}
		return $arr;
	}

	/**
	 * Função: controletipoPendencia
	 * -Retorna um array com a descrição e peculiaridades do tipo da pendência para a listagem em tela.
	 * @return array 
	 */
	public function controleTipoPendencia($pendencia, $boExists=false){
		$arAux = array();
		
		switch($pendencia){
			
			case 'par':
				$arAux['description'] = "Pendências de Obras do PAR";
				$arAux['widget'] = "pendenciaPar";
				break;
			case 'recursos':
				$arAux['description'] = "Pendências nos Recursos";
				$arAux['widget'] = "lazur";
				break;
			case 'desembolso':
				$arAux['description'] = "Pendências de Desembolso";
				$arAux['widget'] = "pendenciaDesembolso";
				break;
			case 'preenchimento':
				$arAux['description'] = "Pendências no Preenchimento";
				$arAux['widget'] = "yellow";
				break;
		}
		if( $boExists )
// 			$arAux['thumb'] = "fa-thumbs-down";
			$arAux['thumb'] = "fa-warning";
		else
			$arAux['thumb'] = "fa-thumbs-up";
// 			$arAux['thumb'] = "fa-ok";
		
		return $arAux;
	}
	
	/**
	 * Função: controletipoPendencia
	 * - Retorna um array com a descrição e peculiaridades do tipo da pendência para a listagem em tela.
	 * @return array
	 */
	public function controleTipoPendenciaLista($pendencia){
	
		$arAux = array();
	
		switch($pendencia){
				
			case 'par':
				$arAux['description'] = "Pendências de Obras do PAR";
				$arAux['widget'] = "pendenciaPar";
				$arAux['icon'] = "fa-warning";
				break;
			case 'recursos':
				$arAux['description'] = "Pendências nos Recursos";
				$arAux['widget'] = "lazur";
				$arAux['icon'] = "fa-money";
				break;
			case 'desembolso':
				$arAux['description'] = "Pendências de Desembolso";
				$arAux['widget'] = "pendenciaDesembolso";
				$arAux['icon'] = "fa-book";
				break;
			case 'preenchimento':
				$arAux['description'] = "Pendências no Preenchimento";
				$arAux['widget'] = "yellow";
				$arAux['icon'] = "fa-pencil";
				break;
		}
		return $arAux;
	}
}
