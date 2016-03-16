<?php

class ConfiguracaoControle extends Controle 
{
	
	public function __construct()
	{
		parent::__construct();
	}
	
	public function recuperarIntrumentosGuia()
	{
		$oInstrumento = new Instrumento();
		return $oInstrumento->recuperarIntrumentosGuia();
	}

	public function recuperarDimensoesGuia($itrid)
	{
		$oDimensao = new Dimensao();
		return $oDimensao->recuperarDimensoesGuia($itrid);
	}
	
	public function recuperarAreasGuia($dimid)
	{
		$oArea = new Area();
		return $oArea->recuperarAreasGuia($dimid);
	}
	
	public function recuperarIndicadoresGuia($areid)
	{
		$oIndicador = new Indicador();
		return $oIndicador->recuperarIndicadoresGuia($areid);
	}
	
	public function recuperarSubacaoGuia($indid)
	{
		$oPropostaSubacao = new PropostaSubacao();
		return $oPropostaSubacao->recuperarSubacaoGuia($indid);
	}
	
	public function recuperarCriteriosAcoesGuia($indid)
	{
		$oCriterio = new Criterio();
		return $oCriterio->recuperarCriteriosAcoesGuia($indid);
	}
	
	public function recuperarPropostaAcoesCriterioGuia($crtid)
	{
		$oPropostaAcao = new PropostaAcao();
		return $oPropostaAcao->recuperarPropostaAcoesCriterioGuia($crtid);
	}
	
	public function recuperarCriterioSubacoesPorIndicador($indid)
	{
		$oCriterio = new Criterio();
		return $oCriterio->recuperarCriterioSubacoesPorIndicador($indid);
	} 
	
	public function recuperarPropostaAcoesGuia($indid)
	{
		$oPropostaAcao = new PropostaAcao();
		return $oPropostaAcao->recuperarPropostaAcoesGuia($indid);
	}
	
	public function recuperarSubacoesPorCriterio($crtid)
	{
		$oPropostaSubacao = new PropostaSubacao();
		return $oPropostaSubacao->recuperarSubacoesPorCriterio($crtid);
	}
	
	public function recuperaDadosFormGuiaDimensao($codigo, $boDimensao, $stTitulo, $itrdsc, $itrid, $dimid = null, $dimdsc = null, $ordcod, $arCampos)	
	{
		$oInstrumento 	= new Instrumento();
		$oDimensao 		= new Dimensao();
		
		if($_REQUEST['acaoGuia'] == 'editar'){
			$oDimensao->carregarPorId($codigo);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$dimid	= $oDimensao->dimid;
			$dimdsc = $oDimensao->dimdsc;
			$ordcod = $oDimensao->dimcod;			
		}else{
			$oInstrumento->carregarPorId($codigo);				
			$oDimensao->recuperarOrdemDimensaoPorItrid($codigo);
			$ordcod 	= $oDimensao->stOrdem;
		}		
		
		$boDimensao	= 'S';
		$stTitulo 	= 'da dimensão';
		$itrdsc 	= $oInstrumento->itrdsc;
		$itrid 		= $oInstrumento->itrid;
		$arCampos 	= array("instrumento", "dimensao");	
	}
	
	public function recuperaDadosFormGuiaArea($codigo, $boArea, $stTitulo, $itrdsc, $dimdsc, $dimid, $areid = null, $aredsc = null, $ordcod, $arCampos)
	{
		$oInstrumento 	= new Instrumento();
		$oDimensao 		= new Dimensao();
		$oArea 			= new Area();
		
		if($_REQUEST['acaoGuia'] == 'editar'){
			$oArea->carregarPorId($codigo);
			$oDimensao->carregarPorId($oArea->dimid);		
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$areid = $oArea->areid;
			$aredsc = $oArea->aredsc; 
			$ordcod	= $oArea->arecod;
		}else{
			$oArea->recuperarOrdemAreaPorDimid($codigo);
			$oDimensao->carregarPorId($codigo);		
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$ordcod	= $oArea->stOrdem;	
		}
		
		$boArea 	= 'S';
		$stTitulo 	= 'da área';
		$itrdsc 	= $oInstrumento->itrdsc;		
		$dimdsc 	= $oDimensao->dimdsc;
		$dimid 		= $oDimensao->dimid;		
		
		$arCampos 	= array("instrumento", "dimensao", "area");
	}
	
	public function recuperaDadosFormGuiaIndicador($codigo, $boIndicador, $stTitulo, $itrdsc, $dimdsc, $aredsc, $areid, $indid = null, $inddsc = null, $ordcod, $arCampos)
	{
		$oInstrumento 	= new Instrumento();
		$oDimensao 		= new Dimensao();
		$oArea 			= new Area();
		$oIndicador 	= new Indicador();
		
		if($_REQUEST['acaoGuia'] == 'editar'){
			$oIndicador->carregarPorId($codigo);		
			$oArea->carregarPorId($oIndicador->areid);		
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$indid	= $oIndicador->indid;
			$inddsc	= $oIndicador->inddsc;
			$ordcod	= $oIndicador->indcod;
		}else{
			$oIndicador->recuperarOrdemIndicadorPorAreid($codigo);		
			$oArea->carregarPorId($codigo);		
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$ordcod	= $oIndicador->stOrdem;
		}
				
		$boIndicador	= 'S';
		$stTitulo 		= 'do indicador';
		$itrdsc 		= $oInstrumento->itrdsc;
		$dimdsc 		= $oDimensao->dimdsc;		
		$aredsc 		= $oArea->aredsc;
		$areid	 		= $oArea->areid;		
		$arCampos 		= array("instrumento", "dimensao", "area", "indicador");	
	}
	
	public function recuperaDadosFormGuiaCriterio($codigo, $boCriterio, $stTitulo, $itrdsc, $dimdsc, $aredsc, $inddsc, $indid, $crtid = null, $crtdsc = null, $crtpontuacao =null, $crtpeso = null, $arCampos)
	{
		$oInstrumento 	= new Instrumento();
		$oDimensao 		= new Dimensao();
		$oArea 			= new Area();
		$oIndicador 	= new Indicador();	
		$oCriterio		= new Criterio();	
		
		if($_REQUEST['acaoGuia'] == 'editar'){
			$oCriterio->carregarPorId($codigo);
			$oIndicador->carregarPorId($oCriterio->indid);
			$oArea->carregarPorId($oIndicador->areid);		
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$crtid = $oCriterio->crtid;
			$crtdsc = $oCriterio->crtdsc;
			$crtpontuacao = $oCriterio->crtpontuacao;
			$crtpeso = $oCriterio->crtpeso;
		}else{
			$oIndicador->carregarPorId($codigo);
			$oArea->carregarPorId($oIndicador->areid);		
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);	
		}
		
		$boCriterio 	= 'S';
		$stTitulo 		= 'do critério';
		$itrdsc 		= $oInstrumento->itrdsc;
		$dimdsc 		= $oDimensao->dimdsc;
		$aredsc 		= $oArea->aredsc;
		$inddsc			= $oIndicador->inddsc;
		$indid			= $oIndicador->indid;
		$arCampos		= array("instrumento", "dimensao", "area", "indicador", "criterio");
	}
	
	public function recuperaDadosFormGuiaAcao($codigo, $boAcao, $stTitulo, $itrdsc, $dimdsc, $aredsc, $inddsc, $crtdsc, $crtid, $ppaid = null, $ppadsc = null, $arCampos)
	{		
		$oInstrumento 	= new Instrumento();
		$oDimensao 		= new Dimensao();
		$oArea 			= new Area();
		$oIndicador 	= new Indicador();		
		$oCriterio		= new Criterio();
		$oPropostaAcao  = new PropostaAcao();
		
		if($_REQUEST['acaoGuia'] == 'editar'){
			$oPropostaAcao->carregarPorId($codigo);
			$oCriterio->carregarPorId($oPropostaAcao->crtid);
			$oIndicador->carregarPorId($oCriterio->indid);
			$oArea->carregarPorId($oIndicador->areid);		
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$ppaid = $oPropostaAcao->ppaid;
			$ppadsc = $oPropostaAcao->ppadsc; 
		}else{
			$oCriterio->carregarPorId($codigo);
			$oIndicador->carregarPorId($oCriterio->indid);
			$oArea->carregarPorId($oIndicador->areid);		
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);	
		}
		
		$boAcao 	= 'S';
		$stTitulo 	= 'da ação';
		$itrdsc 	= $oInstrumento->itrdsc;
		$dimdsc 	= $oDimensao->dimdsc;
		$aredsc 	= $oArea->aredsc;
		$inddsc		= $oIndicador->inddsc;		
		$crtdsc		= $oCriterio->crtdsc;
		$crtid		= $oCriterio->crtid;
		$arCampos	= array("instrumento", "dimensao", "area", "indicador", "acao");
	}
	
	public function recuperaDadosFormGuiaSubacao($indid)
	{
		$oPropostaSubacao = new PropostaSubacao();
		return $oPropostaSubacao->recuperaDadosFormGuiaSubacao($indid);
	}
	
	public function filtraGuiaFormSubacao($frmid, $ppsid = null)
	{
		
		$this->visao->ppsid = $ppsid;
		$this->visao->frmid = ($frmid) ? $frmid : $_POST['frmid'];
		//$this->visao->visualizar('formaExecucao');
		
		$ppsid = $ppsid;
		$frmid = ($frmid) ? $frmid : $_POST['frmid'];
		$db = !$db ? new cls_banco() : $db;
		$arquivo = APPRAIZ . $_SESSION['sisdiretorio'] . '/modulos/principal/configuracao/visao/formaExecucao.php';
		include_once($arquivo);
	}
	
	public function filtraGuiaAbasSubacao($frmid)
	{
		$this->visao->frmid = ($frmid) ? $frmid : $_POST['frmid'];
		$this->visao->visualizar('formaExecucao');
	}
	
}