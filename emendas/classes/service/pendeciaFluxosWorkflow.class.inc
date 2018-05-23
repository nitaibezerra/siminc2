<?php
class pendeciaFluxosWorkflow{
	
	public $db;
	private $msg;
	public $ptrid;
	
	public function __construct(&$db = null, &$ptrid = null){
		if($db){
			$this->db = $db;			
		} else {
			$this->db = new cls_banco(); 
		}
		$this->ptrid = $ptrid;
		$this->msg = array();
	}
	
	public function validaIdentificacaoProcesso( $esdiddestino ){
		
		switch ($esdiddestino) {
			case 52: //Enviar para a correção do PTA
				$this->validaEnvioCorrecaoPTA();
			break;
			case 53: //Retornar para Vinculação das Unidades Gestoras
				$this->validaRetornoVinculacaoUnidadeGestora();
			break;
			case 54: //Retornar para Análise de Mérito
				$this->validaEnvioAnaliseMerito();
			break;
			case 56: //Encaminhar para a análise técnica
				$this->validaEnvioAnaliseTecnica();
			break;
			case 57: //Encaminhar para empenho
				$this->validaEnvioEmpenho();
			break;
			
			default:
				;
			break;
		}
		//$msg = implode( '<br>', $this->msg);
		$msg = $this->msg;
		$this->msg = array();
		return $msg;
	}
	
	private function validaEnvioCorrecaoPTA(){
		
	}
	private function validaRetornoVinculacaoUnidadeGestora(){
		
	}
	private function validaEnvioAnaliseMerito(){
		if( $_SESSION['exercicio'] == '2009' ) $this->msg[] = "Este fluxo so está habilitado no exercicio 2010";
	}
	private function validaEnvioAnaliseTecnica(){
		if( $_SESSION['exercicio'] == '2009' ){
			$this->msg[] = "Este fluxo so está habilitado no exercicio 2010";
		} else {
			$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $this->ptrid ";						  
			$arDados = $this->db->pegaLinha($sql);			
			
			if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ){
				if( empty($arDados['ptrnumprocessoempenho']) ) $this->msg[] = "É necessário cadastrar Nº do Processo do Empenho.";
				if( empty($arDados['ptrnumdocumenta']) ) $this->msg[] = "É necessário cadastrar Nº do Documento.";
			} else {
				$arTipos = pegaTipoReformulacao( $this->ptrid );		
				$arCodigo = array();
				foreach ($arTipos as $v) {
					array_push( $arCodigo, $v['codigo'] );
				}
				if( !in_array( 1, $arCodigo ) ){
					$existeExecucao = $this->db->pegaUm("select 1 from emenda.execucaofinanceira where ptrid = $this->ptrid and exfstatus = 'A'");
					if( !$existeExecucao ) $this->msg[] = "É necessário cadastrar os PI para empenhar.";
				}
			}
			$sql = "SELECT anaid FROM emenda.analise WHERE ptrid = $this->ptrid and anatipo = 'T' and anastatus = 'A'";
			if(!$this->db->carregar($sql)) $this->msg[] = "É necessário cadastrar as Unidades que realizarão a Análise Técnica do PTA.";
		}
	}
	private function validaEnvioEmpenho(){
		if($_SESSION['exercicio'] == '2009'){
			$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $this->ptrid";			  
			$arDados = $this->db->pegaLinha($sql);
			
			if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ){
				if( empty($arDados['ptrnumprocessoempenho']) ) $this->msg[] = "É necessário cadastrar Nº do Processo do Empenho.";
				if( empty($arDados['ptrnumdocumenta']) ) $this->msg[] = "É necessário cadastrar Nº do Documento.";
			}
		}
		
		$sql = "select uniid, anasituacaoparecer, anadataconclusao from emenda.analise 
				where ptrid = $this->ptrid and anatipo = 'T' and anastatus = 'A' 
					and analote = (select max(analote) from emenda.analise where ptrid = {$this->ptrid} and anastatus = 'A' and anatipo = 'T')";
					
		$arDados = $this->db->carregar($sql);
		$retorno = true;
		if( $_SESSION['exercicio'] == '2010'){
			if(is_array($arDados)){
				foreach ($arDados as $v) {
					if( (empty($v['anasituacaoparecer']) && empty($v['anadataconclusao']) ) || $v['anasituacaoparecer'] == 'D' ){
						$retorno = false;
					}
				}
			}
		}
		
		if( empty($arDados) || !$retorno ) $this->msg[] = "É necessário atualizar o parecer técnico na análise técnica.";
	}
}
?>