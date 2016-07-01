<?
class PDF extends FPDF {

var $header;

function PDF($C='',$I='',$N='',$T='',$orientation='P',$unit='mm',$format='A4') {
	$this->FPDF($orientation,$unit,$format);
	$this->AliasNbPages();
}

function WriteTable($data, $w, $a, $ct)
{
	$this->SetLineWidth(.3);
	foreach($data as $row)
	{
		$nb=0;

		if ($ct == 0) { // Município
			$this->SetFont('Arial', 'B', 10);
			$this->SetFillColor(200);
		}
		elseif ($ct == 1) { // Curso
			$this->SetFont('Arial', 'B', 8);
			$this->SetFillColor(230);
		}
		elseif ($ct == 2) { // Totais
			$this->SetFont('Arial', 'B', 8);
			$this->SetFillColor(200);
		}
		elseif ($ct == 3) { // Dados
			$this->SetFont('Arial', '', 8);
			$this->SetFillColor(255);
		}

		$ct = $ct +1;

		for($i=0;$i<count($row);$i++)
			$nb=max($nb,$this->NbLines($w[$i], trim($row[$i])));

		$h = 5 * $nb;

		$this->CheckPageBreak($h);

		for($i=0;$i<count($row);$i++) {
			$x=$this->GetX();
			$y=$this->GetY();
			if ($ct==0) {$align = 'C';} else {$align = $a[$i];}
			if (trim($row[$i])!='') {
				$this->MultiCell($w[$i], 5, trim($row[$i]), 0, $align, 1);
				$this->Rect($x, $y, $w[$i], 5);
			}
			$this->SetXY($x+$w[$i], $y);
		}

		$this->Ln(5);
	}
}

function WriteTable3($data, $w, $a, $ct, $altura, $offsetx=0, $offsety=0)
{
	$this->SetLineWidth(.3);
	foreach($data as $row)
	{
		$nb=0;

		if ($ct == 0) { // Município
			$this->SetFont('Arial', 'B', 8);
			$this->SetFillColor(200);
		}
		elseif ($ct == 1) { // Curso
			$this->SetFont('Arial', 'B', 6);
			$this->SetFillColor(230);
		}
		elseif ($ct == 2) { // Totais
			$this->SetFont('Arial', 'B', 6);
			$this->SetFillColor(200);
		}
		elseif ($ct == 3) { // Dados
			$this->SetFont('Arial', '', 6);
			$this->SetFillColor(255);
		}

		$ct = $ct +1;

		for($i=0;$i<count($row);$i++)
			$nb=max($nb,$this->NbLines($w[$i], trim($row[$i])));

		$h = 4 * $nb;

		$this->CheckPageBreak($h);

		for($i=0;$i<count($row);$i++) {
			$x=$this->GetX();
			$y=$this->GetY();
			if ($i==0) {
				$x = $x+$offsetx;
				$y = $y+$offsety;
				$this->SetXY($x,$y);
			}
			if ($ct==0) {$align = 'C';} else {$align = $a[$i];}
			if (trim($row[$i])!='') {
				$this->MultiCell($w[$i], 4*$altura, trim($row[$i]), 0, $align, 1);
				$this->Rect($x, $y, $w[$i], 4*$altura);
			}
			$this->SetXY($x+$w[$i], $y);
		}

		$this->Ln(4*$altura);
	}
}

function WriteTable2($data, $w, $a, $ct)
{
	$this->SetLineWidth(.3);
	foreach($data as $row)
	{
		$nb=0;

		if ($ct == 0) { // Município
			$this->SetFont('Arial', 'B', 9);
			$this->SetFillColor(200);
		}
		elseif ($ct == 1) { // Curso
			$this->SetFont('Arial', 'B', 7);
			$this->SetFillColor(230);
		}
		elseif ($ct == 2) { // Totais
			$this->SetFont('Arial', 'B', 7);
			$this->SetFillColor(200);
		}
		elseif ($ct == 3) { // Dados
			$this->SetFont('Arial', '', 7);
			$this->SetFillColor(255);
		}

		$ct = $ct +1;

		for($i=0;$i<count($row);$i++)
			$nb=max($nb,$this->NbLines($w[$i], trim($row[$i])));

		$h = 4 * $nb;

		$this->CheckPageBreak($h);

		for($i=0;$i<count($row);$i++) {
			$x=$this->GetX();
			$y=$this->GetY();
			if ($ct==0) {$align = 'C';} else {$align = $a[$i];}
			if (trim($row[$i])!='') {
				$this->MultiCell($w[$i], 4, trim($row[$i]), 0, $align, 1);
				$this->Rect($x, $y, $w[$i], 4);
			}
			$this->SetXY($x+$w[$i], $y);
		}

		$this->Ln(4);
	}
}

function NbLines($w,$txt)
{
	//Computes the number of lines a MultiCell of width w will take
	$cw=&$this->CurrentFont['cw'];
	if($w==0)
		$w=$this->w-$this->rMargin-$this->x;
	$wmax=($w-2*$this->cMargin)*1000/$this->FontSize;
	$s=str_replace("\r",'',$txt);
	$nb=strlen($s);
	if($nb>0 and $s[$nb-1]=="\n")
		$nb--;
	$sep=-1;
	$i=0;
	$j=0;
	$l=0;
	$nl=1;
	while($i<$nb)
	{
		$c=$s[$i];
		if($c=="\n")
		{
			$i++;
			$sep=-1;
			$j=$i;
			$l=0;
			$nl++;
			continue;
		}
		if($c==' ')
			$sep=$i;
		$l+=$cw[$c];
		if($l>$wmax)
		{
			if($sep==-1)
			{
				if($i==$j)
					$i++;
			}
			else
				$i=$sep+1;
			$sep=-1;
			$j=$i;
			$l=0;
			$nl++;
		}
		else
			$i++;
	}
	return $nl;
}

function CheckPageBreak($h)
{
	if($this->GetY()+$h>$this->PageBreakTrigger)
		$this->AddPage($this->CurOrientation);
}

  function Header()
  {
     $this->SetTextColor(60);
     $this->SetFont('Arial', 'B', 12);
	 $this->Cell(0,5,utf8_decode('Ministério da Educação - SESu/DEDES/REUNI'), 0, 0,'C');
	 $this->Cell(0,5,$this->CO_IFES, 0, 1,'R');
	 $this->SetFont('Arial', 'B', 10);
	 $this->SetTextColor(100);
  	 $this->Cell(0,4,utf8_decode('Planilhas Síntese Projeto REUNI'), 0, 1,'C');
	 $this->Cell(0,5, $this->IFES, 0, 1,'L');
	 $this->Cell(0,2,'',0,1,'L');
  }

  function Footer()
  {
	$this->SetFont('Arial', 'B', 10);
  	$this->SetY(-15);
  	$this->Cell(0,0,utf8_decode('Página '.$this->PageNo().'/{nb} Gerada em '.date('j/n/Y')), 0, 1,'C');
  }

  function TituloN1($Titulo){
    $this->SetFont('Arial','B',14);
    $this->SetFillColor(200);
    $this->MultiCell(0,16,$Titulo,1,'C',1);
    $this->Ln(8);
  }

  function TituloN2($Titulo){
    $this->SetFont('Arial','B',12);
    $this->MultiCell(0,14,$Titulo,1,'L');
    $this->Ln(6);
  }

  function TituloN3($Titulo){
    $this->SetFont('Arial','U',12);
    $this->MultiCell(0,14,$Titulo,0,'L');
    $this->Ln(14);
  }

  function Separador($size=4){
    $this->Ln($size);
  }

}

?>
