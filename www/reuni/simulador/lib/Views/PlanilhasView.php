<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PlanilhasView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

    function tabOrcamento() {
		$this->model->PegarInstituicao($this->model->instituicao, &$sg_inst, &$no_inst);

		$matriz = $this->model->getMatrizOrc();
    	foreach($matriz as $lin => $valor){
    		switch($lin) {
    			case "edif": $edif = $valor; break;
    			case "infe": $infe = $valor; break;
    			case "eqip": $eqip = $valor; break;
    			case "tpri": $tpri = $valor; break;
    			case "tcri": $tcri = $valor; break;
    			case "difi": $difi = $valor; break;

    			case "tpeq": $tpeq = $valor; break;
    			case "tsns": $tsns = $valor; break;
    			case "tsni": $tsni = $valor; break;
    			case "tpes": $tpes = $valor; break;

    			case "tbae": $tbae = $valor; break;
    			case "tbme": $tbme = $valor; break;
    			case "tbdo": $tbdo = $valor; break;
    			case "tbpd": $tbpd = $valor; break;
    			case "tbpv": $tbpv = $valor; break;
    			case "tbol": $tbol = $valor; break;

    			case "tubc": $tubc = $valor; break;

    			case "tprc": $tprc = $valor; break;
    			case "tcrc": $tcrc = $valor; break;
    			case "difc": $difc = $valor; break;
    		}
    	}

		if(empty($edif)) {$edif = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($infe)) {$infe = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($eqip)) {$eqip = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tpri)) {$tpri = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tcri)) {$tcri = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tpeq)) {$tpeq = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tsns)) {$tsns = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tsni)) {$tsni = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tbae)) {$tbae = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tbme)) {$tbme = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tbdo)) {$tbdo = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tbpd)) {$tbpd = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tbpv)) {$tbpv = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tbol)) {$tbol = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tubc)) {$tubc = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tprc)) {$tprc = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($tcrc)) {$tcrc = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }
		if(empty($difc)) {$difc = array(2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, "Total" => 0); }

    	$this->output .= '<table class="lista" width="800" align="center">';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="10" class="center">QUADRO SÍNTESE DE ORÇAMENTO</th>';
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="10" class="center">'.$sg_inst.' - '.$no_inst.'</th>';
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3" class="center">Orçamento</th>';
    	$this->output .= '	<td class="anos">2008</td>';
    	$this->output .= '	<td class="anos">2009</td>';
    	$this->output .= '	<td class="anos">2010</td>';
    	$this->output .= '	<td class="anos">2011</td>';
    	$this->output .= '	<td class="anos">2012</td>';
    	$this->output .= '	<td class="anos">Total</td>';
    	$this->output .= '</tr>';

    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="14">Custeio</th>';
    	$this->output .= '	<th rowspan="4">Pessoal</th>';
    	$this->output .= '	<th class="right">Professores-Equivalentes</th>';
		foreach($tpeq as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Servidores de nível superior</th>';
		foreach($tsns as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Servidores de nível intermediário</th>';
		foreach($tsni as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right"><strong>Total</strong></th>';
		foreach($tpes as $vlr) {
	    	$this->output .= '	<td class="right"><strong>'.number_format($vlr,2,',','.').'</strong></td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="6">Bolsas</th>';
    	$this->output .= '	<th class="right">Assistência Estudantil</th>';
		foreach($tbae as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Mestrado</th>';
		foreach($tbme as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Doutorado</th>';
		foreach($tbdo as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Pós-Doutorado</th>';
		foreach($tbpd as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Professor Visitante</th>';
		foreach($tbpv as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Total</th>';
		foreach($tbol as $vlr) {
	    	$this->output .= '	<td class="right"><strong>'.number_format($vlr,2,',','.').'</strong></td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right" colspan="2">Unidades Básicas de Custeio</th>';
		foreach($tubc as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="valoresitenstotalr" colspan="2">Total Projetado</th>';
		foreach($tprc as $vlr) {
	    	$this->output .= '	<th class="valoresitenstotalr">'.number_format($vlr,2,',','.').'</th>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="valoresitenstotalr" colspan="2">Créditos Autorizados</th>';
		foreach($tcrc as $vlr) {
	    	$this->output .= '	<th class="valoresitenstotalr">'.number_format($vlr,2,',','.').'</th>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="valoresitenstotalr" colspan="2">Diferenças</th>';
		foreach($difc as $vlr) {
			if($vlr > 0)
	    		$this->output .= '	<th class="valoresitenstotalr">'.number_format($vlr,2,',','.').'</th>';
	    	else
	    		$this->output .= '	<th class="valoresitenstotalred">'.number_format($vlr,2,',','.').'</th>';
		}
    	$this->output .= '</tr>';

    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="6">Investimento</th>';
    	$this->output .= '	<th class="right" colspan="2">Edificações</th>';
		foreach($edif as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right" colspan="2">Infra-Estrutura</th>';
		foreach($infe as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right" colspan="2">Equipamentos</th>';
		foreach($eqip as $vlr) {
	    	$this->output .= '	<td class="right">'.number_format($vlr,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="valoresitenstotalr" colspan="2">Total Projetado</th>';
		$total = 0;
		foreach($tpri as $vlr) {
	    	$this->output .= '	<th class="valoresitenstotalr">'.number_format($vlr,2,',','.').'</th>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="valoresitenstotalr" colspan="2">Créditos Autorizados</th>';
		foreach($tcri as $vlr) {
	    	$this->output .= '	<th class="valoresitenstotalr">'.number_format($vlr,2,',','.').'</th>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="valoresitenstotalr" colspan="2">Diferença</th>';
		foreach($difi as $vlr) {
			if($vlr > 0)
	    		$this->output .= '	<th class="valoresitenstotalr">'.number_format($vlr,2,',','.').'</th>';
	    	else
	    		$this->output .= '	<th class="valoresitenstotalred">'.number_format($vlr,2,',','.').'</th>';
		}
    	$this->output .= '</tr>';

    	$this->output .= '</table>';
    }

    function tabDadosGlobais() {
    	$matriz = $this->model->getMatrizGlob();

    	foreach($matriz as $lin => $valor){
    		switch($lin) {
    			case "ncgt": $ncgt = $valor; break;
    			case "ncgn": $ncgn = $valor; break;
    			case "nvgt": $nvgt = $valor; break;
    			case "nvgn": $nvgn = $valor; break;
    			case "mpgt": $mpgt = $valor; break;
    			case "mpgn": $mpgn = $valor; break;
    			case "adgt": $adgt = $valor; break;
    			case "adgn": $adgn = $valor; break;
    			case "ntcg": $ntcg = $valor; break;
    			case "ncpm": $ncpm = $valor; break;
    			case "ncpd": $ncpd = $valor; break;
    			case "nvpm": $nvpm = $valor; break;
    			case "nvpd": $nvpd = $valor; break;
    			case "npeq": $npeq = $valor; break;
    			case "ndde": $ndde = $valor; break;
    			case "ndpg": $ndpg = $valor; break;
    			case "cdaj": $cdaj = $valor; break;
    			case "ragp": $ragp = $valor; break;
    		}
    	}

		if(empty($ncgt)) {$ncgt = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ncgn)) {$ncgn = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($nvgt)) {$nvgt = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($nvgn)) {$nvgn = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($mpgt)) {$mpgt = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($mpgn)) {$mpgn = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($adgt)) {$adgt = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($adgn)) {$adgn = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ntcg)) {$ntcg = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ncpm)) {$ncpm = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ncpd)) {$ncpd = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($nvpm)) {$nvpm = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($nvpd)) {$nvpd = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($npeq)) {$npeq = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ndde)) {$ndde = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ndpg)) {$ndpg = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }
		if(empty($ragp)) {$ragp = array(2006 => 0, 2008 => 0, 2009 => 0, 2010 => 0, 2011 => 0, 2012 => 0, 2017 => 0); }

    	$this->model->PegarInstituicao($this->model->instituicao, &$sg_inst, &$no_inst);

    	$this->output .= '<p><table class="lista" width="800" align="center">';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="10" class="center">TABELA DE INDICADORES E DADOS GLOBAIS</th>';
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="10" class="center">'.$sg_inst.' - '.$no_inst.'</th>';
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3">&nbsp;</th>';
    	$this->output .= '	<td class="anos">2007</td>';
    	$this->output .= '	<td class="anos">2008</td>';
    	$this->output .= '	<td class="anos">2009</td>';
    	$this->output .= '	<td class="anos">2010</td>';
    	$this->output .= '	<td class="anos">2011</td>';
    	$this->output .= '	<td class="anos">2012</td>';
    	$this->output .= '	<td class="anos">2017</td>';
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="9" class="center">Graduação</th>';
    	$this->output .= '	<th rowspan="2" class="center">Número de Cursos</th>';
    	$this->output .= '	<th class="right">Total</th>';
		foreach($ncgt as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Noturno</th>';
		foreach($ncgn as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="2" class="center">Vagas Anuais</th>';
    	$this->output .= '	<th class="right">Total</th>';
		foreach($nvgt as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Noturno</th>';
		foreach($nvgn as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="2" class="center">Matrícula Projetada<br>(MAT)</th>';
    	$this->output .= '	<th class="right">Total</th>';
		foreach($mpgt as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Noturno</th>';
		foreach($mpgn as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="2" class="center">Alunos Diplomados<br>(DIP)</th>';
    	$this->output .= '	<th class="right">Total</th>';
		foreach($adgt as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Noturno</th>';
		foreach($adgn as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="2" class="right">Taxa de conclusão dos cursos de graduação (TCG)</th>';
		foreach($ntcg as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="4" class="center">Pós-Graduação</th>';
    	$this->output .= '	<th rowspan="2" class="center">Número de Cursos</th>';
    	$this->output .= '	<th class="right">Mestrado</th>';
		foreach($ncpm as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Doutorado</th>';
		foreach($ncpd as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th rowspan="2" class="center">Matrículas</th>';
    	$this->output .= '	<th class="right">Mestrado</th>';
		foreach($nvpm as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th class="right">Doutorado</th>';
		foreach($nvpd as $valor) {
   			$this->output .= '	<td class="right">'.$valor.'</td>';
		}
    	$this->output .= '</tr>';

    	$this->model->PegarRazaoDE();

    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3" class="right">Número de Professores Equivalentes</th>';
		foreach($npeq as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3" class="right">Número de Professores com Equivalência DE (DDE)</th>';
		foreach($ndde as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3" class="right">Dedução por integração da Pós-Graduação (DPG)</th>';
		foreach($ndpg as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3" class="right">Corpo Docente Ajustado (DDE - DPG)</th>';
		foreach($cdaj as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '<tr>';
    	$this->output .= '	<th colspan="3" class="right">Relação de Alunos de Graduação por Professor (RAP)</th>';
		foreach($ragp as $valor) {
   			$this->output .= '	<td class="right">'.number_format($valor,2,',','.').'</td>';
		}
    	$this->output .= '</tr>';
    	$this->output .= '</table>';
    }

    function display() {
        $this->menu='planilhas';
        $this->output .= '<br>';
        $this->tabOrcamento();
        $this->tabDadosGlobais();
        $this->output .= '<br>';
        parent::display();
    }
}

?>
