<?php

class Par3_Controller_ConfiguracaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	}

	public function recuperarIntrumentosGuia()
	{
		$oInstrumento = new Par3_Model_Instrumento();
		return $oInstrumento->recuperarIntrumentosGuia();
	}

	public function recuperarDimensoesGuia($itrid)
	{
		$oDimensao = new Par3_Model_Dimensao();
		return $oDimensao->recuperarDimensoesGuia($itrid);
	}

	public function recuperarAreasGuia($dimid)
	{
		$oArea = new Par3_Model_Area();
		return $oArea->recuperarAreasGuia($dimid);

	}

	public function recuperarIndicadoresGuia($areid)
	{
		$oIndicador = new Par3_Model_Indicador();
		return $oIndicador->recuperarIndicadoresGuia($areid);
	}

	public function recuperarCriteriosGuia($indid)
	{
		$oCriterio = new Par3_Model_Criterio();
		return $oCriterio->recuperarCriteriosGuia($indid);
	}

	public function recuperaDadosFormGuiaDimensao($codigo, &$boDimensao, &$stTitulo, &$itrdsc, &$itrid, &$dimid = null, &$dimdsc = null, &$ordcod, &$arCampos)
	{
		$oInstrumento 	= new Par3_Model_Instrumento();
		$oDimensao 		= new Par3_Model_Dimensao();

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

		$boDimensao	= '';
		$stTitulo 	= 'da dimensão';
		$itrdsc 	= $oInstrumento->itrdsc;
		$itrid 		= $oInstrumento->itrid;
		$arCampos 	= array("instrumento", "dimensao");
	}

	public function recuperaDadosFormGuiaArea($codigo, &$boArea, &$stTitulo, &$itrdsc, &$dimdsc, &$dimid, &$areid = null, &$aredsc = null, &$ordcod, &$arCampos)
	{
		$oInstrumento 	= new Par3_Model_Instrumento();
		$oDimensao 		= new Par3_Model_Dimensao();
		$oArea 			= new Par3_Model_Area();

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

		$boArea 	= '';
		$stTitulo 	= 'da área';
		$itrdsc 	= $oInstrumento->itrdsc;
		$dimdsc 	= $oDimensao->dimdsc;
		$dimid 		= $oDimensao->dimid;

		$arCampos 	= array("instrumento", "dimensao", "area");
	}

	public function recuperaDadosFormGuiaIndicador($codigo, &$boIndicador, &$stTitulo, &$itrdsc, &$dimdsc, &$aredsc, &$areid, &$indid = null, &$inddsc = null, &$indajuda = null, &$metas = null, &$ordcod, &$arCampos)
	{
		$oInstrumento 	= new Par3_Model_Instrumento();
		$oDimensao 		= new Par3_Model_Dimensao();
		$oArea 			= new Par3_Model_Area();
		$oIndicador 	= new Par3_Model_Indicador();
		$oIndicadorMeta	= new Par3_Model_IndicadorMeta();

		if($_REQUEST['acaoGuia'] == 'editar'){
			$oIndicador->carregarPorId($codigo);
			$oArea->carregarPorId($oIndicador->areid);
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$metas = $oIndicadorMeta->recuperarDados($codigo);
			$indid	 = $oIndicador->indid;
			$inddsc	 = $oIndicador->inddsc;
			$indajuda = $oIndicador->indajuda;
			$ordcod	 = $oIndicador->indcod;
		}else{
			$oIndicador->recuperarOrdemIndicadorPorAreid($codigo);
			$oArea->carregarPorId($codigo);
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$ordcod	= $oIndicador->stOrdem;
		}

		$boIndicador	= '';
		$stTitulo 		= 'do indicador';
		$itrdsc 		= $oInstrumento->itrdsc;
		$dimdsc 		= $oDimensao->dimdsc;
		$aredsc 		= $oArea->aredsc;
		$areid	 		= $oArea->areid;
		$arCampos 		= array("instrumento", "dimensao", "area", "indicador");
	}

	public function recuperaDadosFormGuiaCriterio($codigo, &$boCriterio, &$stTitulo, &$itrdsc, &$dimdsc, &$aredsc, &$inddsc, &$indajuda, &$metas, &$indid, &$crtid = null, &$crtdsc = null, &$ordcod =null, &$arCampos)
	{
		$oInstrumento 	= new Par3_Model_Instrumento();
		$oDimensao 		= new Par3_Model_Dimensao();
		$oArea 			= new Par3_Model_Area();
		$oIndicador 	= new Par3_Model_Indicador();
		$oCriterio		= new Par3_Model_Criterio();
		$oIndicadorMeta	= new Par3_Model_IndicadorMeta();

		if($_REQUEST['acaoGuia'] == 'editar'){
			$oCriterio->carregarPorId($codigo);
			$oIndicador->carregarPorId($oCriterio->indid);
			$oArea->carregarPorId($oIndicador->areid);
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$crtid = $oCriterio->crtid;
			$crtdsc = $oCriterio->crtdsc;
			$ordcod	= $oCriterio->crtcod;
		}else{
			$oIndicador->carregarPorId($codigo);
			$oArea->carregarPorId($oIndicador->areid);
			$oDimensao->carregarPorId($oArea->dimid);
			$oInstrumento->carregarPorId($oDimensao->itrid);
			$oCriterio->recuperarOrdemCriterioPorIndid($codigo);
			$ordcod	= $oCriterio->stOrdem;
		}

		$boCriterio 	= '';
		$stTitulo 		= 'dos componentes do indicador';
		$itrdsc 		= $oInstrumento->itrdsc;
		$dimdsc 		= $oDimensao->dimdsc;
		$aredsc 		= $oArea->aredsc;
		$inddsc			= $oIndicador->inddsc;
		$indajuda		= $oIndicador->indajuda;
		$indid			= $oIndicador->indid;
		$metas          = $oIndicadorMeta->recuperarDados($indid);
		$arCampos		= array("instrumento", "dimensao", "area", "indicador", "criterio");
	}

}