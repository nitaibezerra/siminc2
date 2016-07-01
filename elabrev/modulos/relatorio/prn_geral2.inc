<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:prn_prop_aca.inc
   Finalidade: permitir a construção de relatorio de propostas em ação
    */
/*
session_cache_limiter('private_no_expire');
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-cache");
header("Pragma: no-cache");
session_start(); */

define('FPDF_FONTPATH','includes/fpdf/font/');
require('includes/fpdf/fpdf.php');

class PDF extends FPDF
{
//Page header
function Header()
{
    global $title;
    global $title2;
    global $title3;
    global $title4;
    global $title5;
    global $title6;
    global $title7;
	global $header;
	global $w;
	// autor
	$this->SetAuthor('Sistema Integrado de Monitoramento do Ministério da Educação');
	$this->SetSubject('Relatório Padronizado de Propostas em ação');
	$this->SetTitle($title2);
    //Logo
    $this->Image('imagens/topo_gov.jpg',40,30,525);
    $this->Image('imagens/topo_prn.jpg',40,53,525);
    $this->Image('imagens/livros_mec.jpg',190,380,200);
    $this->Ln(70);
  
    //$this->Ln(20);
    //Arial bold 15
    $this->SetFont('Arial','B',10);
    //Move to the right
   // $this->Cell(80);
    //Title
    $this->SetLeftMargin(40) ;
    if ($title <> "")  $this->Cell(0,30,$title,0,1,'C');
    if ($title2 <> "") $this->Cell(0,15,$title2,0,1,'C');
    if ($title3 <> "") $this->Cell(0,15,$title3,0,1,'C');
    if ($title4 <> "") $this->Cell(0,15,$title4,0,1,'C');
    if ($title5 <> "") $this->Cell(0,15,$title5,0,1,'C');
    if ($title6 <> "") $this->Cell(0,15,$title6,0,1,'C');
    if ($title7 <> "") $this->Cell(0,15,$title7,0,1,'C');
    $this->Ln(20);
	   for($i=0;$i<count($header[0]);$i++)
        $this->Cell($w[$i],15,$header[0][$i],1,0,'C');
    $this->Ln();
}

// function Rotate
        function Rotate($angle,$x=-1,$y=-1) {
                if($x==-1)
                        $x=$this->x;
                if($y==-1)
                        $y=$this->y;
                if($this->angle!=0)
                        $this->_out('Q');
                $this->angle=$angle;
                if($angle!=0) {
                        $angle*=M_PI/180;
                        $c=cos($angle);
                        $s=sin($angle);
                        $cx=$x*$this->k;
                        $cy=($this->h-$y)*$this->k;
                        $this->_out(sprintf('q %.5f %.5f %.5f %.5f %.2f %.2f cm 1 0 0 1 %.2f %.2f cm',$c,$s,-$s,$c,$cx,$cy,-$cx,-$cy));
                }
        }

// function RotatedText
        function RotatedText($x,$y,$txt,$angle) {
                //Text rotated around its origin
                $this->Rotate($angle,$x,$y);
                $this->Text($x,$y,$txt);
                $this->Rotate(0);
        }

//Page footer
function Footer()
{
    global $dat;
    global $title3;

    //Position at 1.5 cm from bottom
    $this->SetY(-55);
    $this->setX(40);
    $this->SetFont('Arial','I',6);
    $this->write(8,$title3);


    //Page number
   // $this->Cell(40);
    $this->SetY(-35);
    $this->setX(40);
    $this->Write(6,'Data: '.$dat);
    $this->setX(300);
    $this->Write(6,'Página '.$this->PageNo(),'',$this->getY()-12);
    //$this->Cell(0,10,'Página '.$this->PageNo().'/{nb}',0,0,'C');
}

//Simple table
function BasicTable($header,$data,$k)
{
    //Column widths
    global $w;
    global $cab;
    global $cab2;
    global $LINHA;

    //Data
    $this->SetFont('Times','',8);
    for($j=0;$j<=$LINHA;$j++)
    {
        for($i=0;$i<3;$i++){
          if (strlen($data[$j][0])> 30) $data[$j][0] = substr($data[$j][0],0,30);
          if (strlen($data[$j][1])> 120) $data[$j][1] = substr($data[$j][1],0,120).'...';
           $this->Cell($w[$i],12,$data[$j][$i],1,0,'L');}
       $this->Ln();
    }
}
}
  
//Instanciation of inherited class


$TIT =  $_SESSION['TIT'];
$LIN =  $_SESSION['LIN'];
$LINHA = $_SESSION['LINHA'];

 //Data loading
$data= $LIN;

 //Column titles
$header=$TIT;
$pdf=new PDF('P','pt','A4');
$pdf->SetFont('Times','',6);
// calcula as larguras de cada coluna
$w[0]=75;
$w[1]=200;
$w[2]=200;

$title = 'SISTEMA INTEGRADO DE MONITORAMENTO DO MINISTÉRIO DA EDUCAÇÃO';
$title2 = $_SESSION['title'];

$titulos = explode("|", $title2);
$title2 = $titulos[0]; // piece
$title3 = $titulos[1]; // piece
$title4 = $titulos[2]; // piece
$title5 = $titulos[3]; // piece
$title6 = $titulos[4]; // piece
$title7 = $titulos[5]; // piece

$dat = $_SESSION['data'];
$pdf->Open();

$pdf->AliasNbPages();

$pdf->AddPage();
   $pdf->BasicTable($TIT,$data,$LINHA);
$pdf->Ln();

$pdf->Output();
?>
