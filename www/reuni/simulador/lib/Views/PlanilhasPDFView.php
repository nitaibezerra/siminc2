<?php
define('FPDF_FONTPATH', 'fpdf/font/');

require('fpdf/fpdf.php');
require('cl_PDF.php');

class PlanilhasPDFView extends View {
	private $erro;
	private $erros;
	private $pdf;
	private $sg_instituicao;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

	function gerarLinhaOrc($var,$cor=3) {
		$w = array(20, 20, 20, 20, 20, 20);
		$a = array('R', 'R', 'R', 'R', 'R', 'R');
		$tx[] = array(
		number_format($var['2008'],2,',','.'),
		number_format($var['2009'],2,',','.'),
		number_format($var['2010'],2,',','.'),
		number_format($var['2011'],2,',','.'),
		number_format($var['2012'],2,',','.'),
		number_format($var['Total'],2,',','.'));
		$this->pdf->WriteTable3($tx, $w, $a, $cor, 1, 70, 0);
		unset($tx);
		return;
	}

	function gerarLinhaInd($var,$cor=3) {
		$w = array(19, 19, 19, 19, 19, 19, 19);
		$a = array('R', 'R', 'R', 'R', 'R', 'R', 'R');
		$tx[] = array(
		number_format($var['2007'],2,',','.'),
		number_format($var['2008'],2,',','.'),
		number_format($var['2009'],2,',','.'),
		number_format($var['2010'],2,',','.'),
		number_format($var['2011'],2,',','.'),
		number_format($var['2012'],2,',','.'),
		number_format($var['2017'],2,',','.'));
		$this->pdf->WriteTable3($tx, $w, $a, $cor, 1, 57, 0);
		unset($tx);
		return;
	}

	function gerarPDF() {
		$this->model->PegarInstituicao($this->model->instituicao, &$sg_inst, &$no_inst);
		$this->sg_instituicao = $sg_inst;
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


		$this->pdf=new PDF();
		$this->pdf->AddPage();
		$this->pdf->SetFont('Arial','',16);

		$w = array(190);
		$a = array('C');
		$tx[] = array(utf8_decode("QUADRO SÍNTESE DE ORÇAMENTO"));
		$this->pdf->WriteTable($tx, $w, $a, 0);
		unset($tx);

		$w = array(190);
		$a = array('C');
		$tx[] = array(utf8_decode($sg_inst." - ".$no_inst));
		$this->pdf->WriteTable($tx, $w, $a, 1);
		unset($tx);

		$w = array(70, 20, 20, 20, 20, 20, 20);
		$a = array('C', 'C', 'C', 'C', 'C', 'C', 'C');
		$tx[] = array(utf8_decode("Orçamento"), "2008", "2009", "2010", "2011", "2012", "Total");
		$this->pdf->WriteTable($tx, $w, $a, 1);
		unset($tx);

		$x1=$this->pdf->GetX();
		$y1=$this->pdf->GetY();

		$w = array(18);
		$a = array('C');
		$tx[] = array(utf8_decode("Custeio"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 14 );
		unset($tx);

		$w = array(18);
		$a = array('C');
		$tx[] = array(utf8_decode("Investimento"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 6);
		unset($tx);

		$x2=$this->pdf->GetX();
		$y2=$this->pdf->GetY();

		$w = array(12);
		$a = array('C');
		$tx[] = array(utf8_decode("Pessoal"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 4, 18, $y1-$y2);
		unset($tx);

		$w = array(12);
		$a = array('C');
		$tx[] = array(utf8_decode("Bolsa"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 6, 18);
		unset($tx);

		$x3=$this->pdf->GetX();
		$y3=$this->pdf->GetY();

		$w = array(40);
		$a = array('R');
		$tx[] = array(utf8_decode("Professores Equivalentes"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, $y1-$y3);
		unset($tx);

		$tx[] = array(utf8_decode("Servidores de Nível Superior"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Servidores de nível intermediário"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Total"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Assistência Estudantil"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Mestrado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Doutorado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Pós-Doutorado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Professor Visitante"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 30, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Total"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 30, 0);
		unset($tx);

		$w = array(52);
		$tx[] = array(utf8_decode("Unidades Básicas de Custeio"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Total Projetado"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Créditos Autorizados"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Diferenças"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Edificações"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Infra-Estrutura"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Equipamentos"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Total Projetado"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Créditos Autorizados"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Diferenças"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18, 0);
		unset($tx);

		$this->pdf->SetY($y1);

		$this->gerarLinhaOrc($tpeq);
		$this->gerarLinhaOrc($tsns);
		$this->gerarLinhaOrc($tsni);
		$this->gerarLinhaOrc($tpes,2);
		$this->gerarLinhaOrc($tbae);
		$this->gerarLinhaOrc($tbme);
		$this->gerarLinhaOrc($tbdo);
		$this->gerarLinhaOrc($tbpd);
		$this->gerarLinhaOrc($tbpv);
		$this->gerarLinhaOrc($tbol,2);
		$this->gerarLinhaOrc($tubc);
		$this->gerarLinhaOrc($tprc,2);
		$this->gerarLinhaOrc($tcrc,2);
		$this->gerarLinhaOrc($difc,2);
		$this->gerarLinhaOrc($edif);
		$this->gerarLinhaOrc($infe);
		$this->gerarLinhaOrc($eqip);
		$this->gerarLinhaOrc($tpri,2);
		$this->gerarLinhaOrc($tcri,2);
		$this->gerarLinhaOrc($difi,2);

		$this->pdf->Separador(40);

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

		$w = array(190);
		$a = array('C');
		$tx[] = array(utf8_decode("TABELA DE INDICADORES E DADOS GLOBAIS"));
		$this->pdf->WriteTable($tx, $w, $a, 0);
		unset($tx);

		$w = array(190);
		$a = array('C');
		$tx[] = array(utf8_decode($sg_inst." - ".$no_inst));
		$this->pdf->WriteTable($tx, $w, $a, 1);
		unset($tx);

		$w = array(57, 19, 19, 19, 19, 19, 19, 19);
		$a = array('C', 'C', 'C', 'C', 'C', 'C', 'C' ,'C');
		$tx[] = array("Indicadores", "2007", "2008", "2009", "2010", "2011", "2012", "2017");
		$this->pdf->WriteTable($tx, $w, $a, 1);
		unset($tx);

		$x1=$this->pdf->GetX();
		$y1=$this->pdf->GetY();

		$w = array(18);
		$a = array('C');
		$tx[] = array(utf8_decode("Graduacao"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 9 );
		unset($tx);

		$w = array(18);
		$a = array('C');
		$tx[] = array(utf8_decode("Pós-Graduação"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 4);
		unset($tx);

		$x2=$this->pdf->GetX();
		$y2=$this->pdf->GetY();

		$w = array(25);
		$a = array('C');
		$tx[] = array(utf8_decode("Número de Cursos"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 2, 18, $y1-$y2);
		unset($tx);

		$tx[] = array(utf8_decode("Vagas Anuais"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 2, 18);
		unset($tx);

		$tx[] = array(utf8_decode("Matrícula Proj. (MAT)"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 2, 18);
		unset($tx);

		$tx[] = array(utf8_decode("Alunos Dipl. (DIP)"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 2, 18);
		unset($tx);

		$tx[] = array(utf8_decode("Número de Cursos"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 2, 18, 4);
		unset($tx);

		$tx[] = array(utf8_decode("Matrículas"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 2, 18);
		unset($tx);

		$x3=$this->pdf->GetX();
		$y3=$this->pdf->GetY();

		$w = array(14);
		$a = array('R');
		$tx[] = array(utf8_decode("Total"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43, $y1-$y3);
		unset($tx);

		$tx[] = array(utf8_decode("Noturno"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Total"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Noturno"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Total"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Noturno"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Total"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Noturno"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$w = array(39);
		$tx[] = array(utf8_decode("Taxa conclusão graduação (TCG)"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 18);
		unset($tx);

		$w = array(14);
		$tx[] = array(utf8_decode("Mestrado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Doutorado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Mestrado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$tx[] = array(utf8_decode("Doutorado"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 43);
		unset($tx);

		$w = array(57);
		$tx[] = array(utf8_decode("Número de Professores Equivalentes"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Número de Professores com Equivalência DE (DDE)"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Dedução por integração da Pós-Graduação (DPG)"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Corpo Docente Ajustado (DDE-DPG)"));
		$this->pdf->WriteTable3($tx, $w, $a, 1, 1, 0);
		unset($tx);

		$tx[] = array(utf8_decode("Relação de Alunos de Graduação por Professor (RAP)"));
		$this->pdf->WriteTable3($tx, $w, $a, 2, 1, 0);
		unset($tx);

		$this->pdf->SetY($y1);

		$this->gerarLinhaInd($ncgt);
		$this->gerarLinhaInd($ncgn);
		$this->gerarLinhaInd($nvgt);
		$this->gerarLinhaInd($nvgn);
		$this->gerarLinhaInd($mpgt);
		$this->gerarLinhaInd($mpgn);
		$this->gerarLinhaInd($adgt);
		$this->gerarLinhaInd($adgn);
		$this->gerarLinhaInd($ntcg,2);
		$this->gerarLinhaInd($ncpm);
		$this->gerarLinhaInd($ncpd);
		$this->gerarLinhaInd($nvpm);
		$this->gerarLinhaInd($nvpd);
		$this->gerarLinhaInd($npeq);
		$this->gerarLinhaInd($ndde);
		$this->gerarLinhaInd($ndpg);
		$this->gerarLinhaInd($cdaj);
		$this->gerarLinhaInd($ragp,2);
	}

    //function SalvarPDF($path=null) {
    	//$this->gerarPDF();
    	//if (isset($path)) {
    		//$this->pdf->Output($path.'PlanilhasPDF_'.$this->sg_instituicao.'.pdf');
    	//}
    //}

    function Display() {
    	$this->gerarPDF();
    	$this->pdf->Output("Simulador_REUNI_".$this->sg_instituicao.".pdf","D");
    }

}
?>
