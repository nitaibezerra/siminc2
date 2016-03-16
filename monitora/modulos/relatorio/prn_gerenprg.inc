<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:prn_gerenprg.inc
   Finalidade: permitir a construção de relatorio padronizado de dados de programa
    */
/*
session_cache_limiter('private_no_expire');
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-cache");
header("Pragma: no-cache");
session_start(); */

define('FPDF_FONTPATH','includes/fpdf/font/');
require(APPRAIZ.'includes/fpdf/fpdf.php');

class PDF extends FPDF
{
//Page header
function Header()
{
    global $title;
    global $title2;
    global $title3;
  //  global $i;
    global $res;

	// autor
	$this->SetAuthor('Sistema Integrado de Monitoramento do Ministério da Educação');
	$this->SetSubject('Relatório Gerencial de Programa');
	$this->SetTitle($title2);
    //Logo
    $this->Image('/var/www/simec/www/imagens/logo_mec_br.jpg',40,20,525);
    $this->Image('/var/www/simec/www/imagens/top.jpg',40,43,525);
    $this->Image('/var/www/simec/www/imagens/livros_mec.jpg',190,380,200);
        $this->Ln(70);
    $this->SetFont('Arial','',8);
    if ($title <> "")  $this->Cell(0,15,$title,0,1,'C');
    if ($title2 <> "") $this->Cell(0,15,$title2.'   Exercício:'.$_SESSION['exercicio'],0,1,'C');
    $this->Cell(0,15,$title3,0,1,'C');

 
}
function Prnlinha($j,$lin,$dado,$larg)
{
  global $lin;
  $linha = '';
  $piece = explode(" ", $dado);
      //$this->write(10,$dado);
  for ($n=0;$n < count($piece); $n++)
  {
    $linha = $linha.' '.$piece[$n];
    if (strlen($linha) >= $larg or $n==(count($piece)-1))
    {
       $this->Text($j+80,$lin,$linha);
       //$this->write(10,$linha);
       $lin=$lin+10;
       $linha = '';
    }

  }
 // return $lin+10;
}

function PrnWrite($tit,$dado,$y)
{
    $yy = $this->getY();
    if ($y == $yy) $dir=12; else $dir = 0;
    $this->setxy(40,$this->gety()+$dir);
    $this->SetFont('Arial','B',8);
    $this->write(12,$tit);
    $this->SetFont('Arial','',8);
    $this->SetLeftMargin(120);
    $y = $this->getY();
    $this->write(12,$dado);
    $y = $this->getY();
    return $y;
}


function DadosBasicos()
{

    global $res;
    global $resp;
    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    $this->setxy(120,150);
    $yp=$this->PrnWrite('Código:',$res[0]['prgcod'],150);
    $yp=$this->PrnWrite('Título:',$res[0]['prgdsc'],$yp);
    $yp=$this->PrnWrite('Órgão Responsável:',$res[0]['orgdsc'],$yp);
    $yp=$this->PrnWrite('Objetivo:',$res[0]['prgdscobjetivo'],$yp);
    $yp=$this->PrnWrite('Público Alvo:',$res[0]['prgdscpublicoalvo'],$yp);
    $yp=$this->PrnWrite('Justificativa:',$res[0]['prgdscjustificativa'],$yp);
    $yp=$this->PrnWrite('Tipo:',$res[0]['tprnome'],$yp);
    if ($res[0]['prgsntemporario']=='f')
      $yp=$this->PrnWrite('Horizonte temporal:','Contínuo',$yp);
    else      $yp=$this->PrnWrite('Horizonte temporal:','Temporário',$yp);

    if ($res[0]['prgdscestrategia'])
      $yp=$this->PrnWrite('Estratégia:',$res[0]['prgdscestrategia'],$yp);

    if ($res[0]['prgdscobjetivoassoc'])
      $yp=$this->PrnWrite('Obj. Associado:',$res[0]['prgdscobjetivoassoc'],$yp);
   if ($res[0]['prgdscproblema'])
         $yp=$this->PrnWrite('Problema:',$res[0]['prgdscproblema'],$yp);

    $yp=$this->PrnWrite($resp[0]['descricao'].':',$resp[0]['usuario'],$yp);
    $yp=$this->PrnWrite($resp[1]['descricao'].':',$resp[1]['usuario'],$yp);
    $yp=$this->PrnWrite($resp[2]['descricao'].':',$resp[2]['usuario'],$yp);
    $yp=$this->PrnWrite($resp[3]['descricao'].':',$resp[3]['usuario'],$yp);
      
}

function FinEvo($linhas,$prgc,$prgd)
{
     global $res;

    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    //$this->setxy(120,150);
    $yp=150;
    for ($l=0;$l<$linhas;$l++)
    {
        $acao = ereg_replace("<[^>]*>","",$res[$l]['acao']);
        $yp=$this->PrnWrite('Código:',$prgc,$yp);
        $yp=$this->PrnWrite('Título:',$prgd,$yp);
        $yp=$this->PrnWrite('Objeto:',$acao,$yp);
        $yp=$this->PrnWrite('Dotação Autorizada:',number_format($res[$l]['disponivel'],2,',','.'),$yp);
        $yp=$this->PrnWrite('Valor empenhado:',number_format($res[$l]['emitidos'],2,',','.'),$yp);
        $yp=$this->PrnWrite('Valor liquidado:',number_format($res[$l]['liquidados'],2,',','.'),$yp);
        $yp=$this->PrnWrite('Valor pago:',number_format($res[$l]['pago'],2,',','.'),$yp);
        $yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);
    }
}


function AvaliacaoParecer($j,$nt)
{

  /*  global $res;

    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    $this->setxy(120,150);
    if ($res[$j]['avlib']=='t') {
    $acao = ereg_replace("<[^>]*>","",$res[0]['avaliacao']);
    $yp=$this->PrnWrite('Código:',$res[0]['prgcod'],150);
    $yp=$this->PrnWrite('Título:',$res[0]['prgdsc'],$yp);
    $yp=$this->PrnWrite('Avaliação:',$acao,$yp);
    $yp=$this->PrnWrite('Data:',$res[0]['avdata'],$yp);
    $yp=$this->PrnWrite('Situação:',$res[0]['avtpsdsc'],$yp);
    $yp=$this->PrnWrite('Estado:',$res[0]['avcordsc'],$yp);
    }
    $yp=$this->PrnWrite('Avaliador:',$res[0]['avusu'].'/'.$res[0]['orgav'],$yp);
    if ($res[$j]['parlib']=='t') {
    $acao = ereg_replace("<[^>]*>","",$res[0]['parecer']);
    $yp=$this->PrnWrite('Parecer:',$acao,$yp);
    $yp=$this->PrnWrite('Data:',$res[0]['pardata'],$yp);
    $yp=$this->PrnWrite('Situação:',$res[0]['patpsdsc'],$yp);
    $yp=$this->PrnWrite('Estado:',$res[0]['pacordsc'],$yp);
    $yp=$this->PrnWrite('Parecerista:',$res[0]['parusu'].'/'.$res[0]['orgpar'],$yp);
    }
*/

    global $res;
    $this->SetFont('Arial','',8);
    if ($nt==0) {
         $yp=$this->PrnWrite('Código:',$res[$j]['prgcod'],$yp);
         $yp=$this->PrnWrite('Título:',$res[$j]['prgdsc'],$yp);
    }
    else
        $yp=$this->PrnWrite("",'',$yp);
   if ($res[$j]['avlib']=='t') {
    $acao = ereg_replace("<[^>]*>","",$res[$j]['avaliacao']);
    if ($acao <> ''){
        $yp=$this->PrnWrite("-----------------",'----------------------------------------------------',$yp);
        $yp=$this->PrnWrite('Avaliação:',$acao,$yp);
        $yp=$this->PrnWrite('Data:',$res[$j]['avdata'],$yp);
        $yp=$this->PrnWrite('Situação:',$res[$j]['avtpsdsc'],$yp);
        $yp=$this->PrnWrite('Estado:',$res[$j]['avcordsc'],$yp);
        $yp=$this->PrnWrite('Avaliador:',$res[$j]['avusu'].'/'.$res[$j]['orgav'],$yp);
    }
    }
   if ($res[$j]['parlib']=='t') {
    $acao = ereg_replace("<[^>]*>","",$res[$j]['parecer']);
    if ($acao <> ''){
        $yp=$this->PrnWrite('------------------','----------------------------------------------------',$yp);
        $yp=$this->PrnWrite('Parecer:',$acao,$yp);
        $yp=$this->PrnWrite('Data:',$res[$j]['pardata'],$yp);
        $yp=$this->PrnWrite('Situação:',$res[$j]['patpsdsc'],$yp);
        $yp=$this->PrnWrite('Estado:',$res[$j]['pacordsc'],$yp);
        $yp=$this->PrnWrite('Parecerista:',$res[$j]['parusu'].'/'.$res[$j]['orgpar'],$yp);
    }
    }
}

function RestProv($j,$nt)
{
    global $res;
    $this->SetFont('Arial','',8);

    $acao = ereg_replace("<[^>]*>","",$res[$j]['restricao']);
    if ($nt==0) {
         $yp=$this->PrnWrite('Código:',$res[$j]['prgcod'],$yp);
         $yp=$this->PrnWrite('Título:',$res[$j]['prgdsc'],$yp);
    }
    else
        $yp=$this->PrnWrite("",'',$yp);
    $yp=$this->PrnWrite("-----------------",'----------------------------------------------------',$yp);
    $yp=$this->PrnWrite('Restrição:',$acao,$yp);
    $yp=$this->PrnWrite('Tipo de Restrição:',$res[$j]['trsdsc'],$yp);
    $acao = ereg_replace("<[^>]*>","",$res[$j]['providencia']);
    $yp=$this->PrnWrite('Providência:',$acao,$yp);
    $yp=$this->PrnWrite('Data:',$res[$j]['datainc'],$yp);
    $yp=$this->PrnWrite('Analista:',$res[$j]['usunome'].'/'.$res[$j]['orgao'],$yp);
}

function ParAltGest($j,$nt)
{

    global $res;
    $this->SetFont('Arial','',8);

    $acao = ereg_replace("<[^>]*>","",$res[$j]['parecer']);
    if ($nt==0) {
         $yp=$this->PrnWrite('Código:',$res[$j]['prgcod'],$yp);
         $yp=$this->PrnWrite('Título:',$res[$j]['prgdsc'],$yp);
    }
    else
        $yp=$this->PrnWrite("",'',$yp);
    $yp=$this->PrnWrite("-----------------",'----------------------------------------------------',$yp);
    $yp=$this->PrnWrite('Parecer:',$acao,$yp);
    $yp=$this->PrnWrite('Data:',$res[$j]['datainc'],$yp);
    $yp=$this->PrnWrite('Analista:',$res[$j]['usunome'].'/'.$res[$j]['orgao'],$yp);

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
    global $title4;

    //Position at 1.5 cm from bottom
    $this->SetY(-55);
    $this->setX(40);
    $this->SetFont('Arial','I',6);
    $this->write(8,$title4);


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
        for($i=0;$i<2;$i++){
          if (strlen($data[$j][0])> 30) $data[$j][0] = substr($data[$j][0],0,30);
          if (strlen($data[$j][1])> 120) $data[$j][1] = substr($data[$j][1],0,120).'...';
           $this->Cell($w[$i],12,$data[$j][$i],1,0,'L');}
       $this->Ln();
    }
}
}
  
//Instanciation of inherited class
$dat = $_SESSION['data'];
$title4 = $_SESSION['title'];

$pdf=new PDF('P','pt','A4');
$pdf->SetFont('Times','',6);
$pdf->Open();
$pdf->AliasNbPages();
// calcula as larguras de cada coluna


$title = 'SISTEMA INTEGRADO DE MONITORAMENTO DO MINISTÉRIO DA EDUCAÇÃO';

if (count($_SESSION['rsdados']) > 0)
{
  // solicitou dados básicos
  $title3 = 'Dados Básicos de Programas';
  for ($i=0; $i<=count($_SESSION['rsdados'])-1;$i++)
  {
   $res = $_SESSION['rsdados'][$i];
   $resp = $_SESSION['rsdadosresp'][$i];
   $pdf->AddPage();
   $pdf->DadosBasicos();
  }
}
if (count($_SESSION['rsavapar']) > 0)
{
  // solicitou avaliação e parecer
  $title3 = 'Avaliação e Parecer de Programa';
/*
  for ($i=0; $i<=count($_SESSION['rsavapar'])-1;$i++)
  {
   $res = $_SESSION['rsavapar'][$i];
   for ($j=0; $j<=count($_SESSION['rsavapar'][$i])-1;$j++)
   {
   if (($res[$j]['avaliacao']<>'' or $res[$j]['parecer']<>'') and ($res[$j]['avlib']=='t' or $res[$j]['parlib']=='t'))
   {
    $pdf->AddPage();
    $pdf->AvaliacaoParecer();
     }
   }
  }
  */
    $prgcod='';
  for ($i=0; $i<=count($_SESSION['rsavapar'])-1;$i++)
  {
   $res = $_SESSION['rsavapar'][$i];

   for ($j=0; $j<=count($_SESSION['rsavapar'][$i])-1;$j++)
   {
   if (($res[$j]['avaliacao']<>'' or $res[$j]['parecer']<>'') and ($res[$j]['avlib']=='t' or $res[$j]['parlib']=='t') ){
        $prgcod2 = $res[$j]['prgcod'];
        if (! $prgcod2 == $prgcod){
           $pdf->AddPage();
           $prgcod=$prgcod2;
           $pdf->setxy(120,150);
           $nt = 0;
         }
         else $nt=1;
     $pdf->AvaliacaoParecer($j,$nt);

     }
   }
  }
}
if (count($_SESSION['rsfinevo']) > 0)
{
  // solicitou financeiro evolução
  $title3 = 'Financeiro Evolução de Programas';
  $linhas=count($_SESSION['rsfinevo'][0]);
//print_r($_SESSION['rsfinevo']);

  for ($i=0; $i<=count($_SESSION['rsfinevo'])-1;$i++)
  {
   $res = $_SESSION['rsfinevo'][$i];
   if ($res[0]['acao']<>''){
   $pdf->AddPage();
   $pdf->FinEvo($linhas,$_SESSION['prgcod'],$_SESSION['prgdsc']);
   }
  }

}
if (count($_SESSION['rsrespro']) > 0)
{
  // solicitou restrição e providência
  $title3 = 'Restrições e Providências de Programa';
  $prgcod='';
  for ($i=0; $i<=count($_SESSION['rsrespro'])-1;$i++)
  {
   $res = $_SESSION['rsrespro'][$i];
   for ($j=0; $j<=count($_SESSION['rsrespro'][$i])-1;$j++)
   {
   if ($res[$j]['restricao']<>''){
        $prgcod2 = $res[$j]['prgcod'];
        if ($prgcod2 <> $prgcod){
           $pdf->AddPage();
           $prgcod=$prgcod2;
           $pdf->setxy(120,150);
           $nt = 0;
         }
         else $nt=1;
     $pdf->RestProv($j,$nt);
   }
  }
  }
}

if (count($_SESSION['rsparalt']) > 0)
{
  // solicitou financeiro evolução
  $title3 = 'Parecer da Alta Gestão em Programas';
  $prgcod='';
  for ($i=0; $i<=count($_SESSION['rsparalt'])-1;$i++)
  {
   $res = $_SESSION['rsparalt'][$i];
   for ($j=0; $j<=count($_SESSION['rsparalt'][$i])-1;$j++)
   {
   if ($res[$j]['parecer']<>''){
        $prgcod2 = $res[$j]['prgcod'];
        if ($prgcod2 <> $prgcod){
           $pdf->AddPage();
           $prgcod=$prgcod2;
           $pdf->setxy(120,150);
           $nt = 0;
         }
         else $nt=1;
     $pdf->ParAltGest($j,$nt);
   }
  }
  }

}
$pdf->Output();
?>
