<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:prn_gerenaca.inc
   Finalidade: permitir a construção de relatorio padronizado de dados de ação
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
  //  global $i;
    global $res;

	// autor
    $this->SetTextColor(0);
	$this->SetAuthor('Sistema Integrado de Monitoramento do Ministério da Educação');
	$this->SetSubject('Relatório Gerencial de Programa');
	$this->SetTitle($title2);
    //Logo
    $this->Image('imagens/topo_gov.jpg',40,30,525);
    $this->Image('imagens/topo_prn.jpg',40,53,525);
    $this->Image('imagens/livros_mec.jpg',190,380,200);
        $this->Ln(70);
    $this->SetFont('Arial','',8);
    if ($title <> "")  $this->Cell(0,15,$title,0,1,'C');
    if ($title2 <> "") $this->Cell(0,15,$title2,0,1,'C');
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
    //$this->SetFont('Arial','B',8);
    $this->write(12,$tit);
    //$this->SetFont('Arial','',8);
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
    $yp=$this->PrnWrite('Código do Programa: ',$res[0]['prgcod'],150);
    $yp=$this->PrnWrite('Título do Programa: ',$res[0]['prgdsc'],$yp);
    $yp=$this->PrnWrite('Código da Ação: ',$res[0]['acacod'].'.'.$res[0]['unicod'].'.'.$res[0]['loccod'],$yp);
    $yp=$this->PrnWrite('Título da Ação: ',$res[0]['acadsc'],$yp);
    $yp=$this->PrnWrite('Órgão Responsável: ',$res[0]['orgdsc'],$yp);
    $yp=$this->PrnWrite('Unidade Responsável: ',$res[0]['unidsc'],$yp);
    if ($res[0]['fundsc']) $yp=$this->PrnWrite('Função: ',$res[0]['fundsc'],$yp);
    if ($res[0]['sfudsc']) $yp=$this->PrnWrite('Subfunção: ',$res[0]['sfudsc'],$yp);
    if ($res[0]['acafinalidade']) $yp=$this->PrnWrite('Finalidade: ',$res[0]['acafinalidade'],$yp);
    if ($res[0]['acadescricao']) $yp=$this->PrnWrite('Descrição: ',$res[0]['acadescricao'],$yp);
    if ($res[0]['prodsc']) $yp=$this->PrnWrite('Produto: ',$res[0]['prodsc'],$yp);
    if ($res[0]['acadscproduto']) $yp=$this->PrnWrite('Especificação do Produto: ',$res[0]['acadscproduto'],$yp);
    if ($res[0]['acasnrap']=='t') $yp=$this->PrnWrite('Situação: ','RAP',$yp);
    if ($res[0]['acasnemenda']=='t') $yp=$this->PrnWrite('Complemento: ','Emenda',$yp);
    
    $yp=$this->PrnWrite('Meta: ',$res[0]['meta'],$yp);
    $yp=$this->PrnWrite('Unidade de medida: ',$res[0]['unmdsc'],$yp);
    if ($res[0]['acasnoutrasfontes']=='t') $yp=$this->PrnWrite('Tipo: ','Não orçamentária',$yp);
    else
    {
        $yp=$this->PrnWrite('Tipo: ',$res[0]['tacdsc'],$yp);
        if ($res[0]['acasnmedireta']=='t') $yp=$this->PrnWrite('Implementação: ','Direta',$yp);
        if ($res[0]['acasnmedesc']=='t') $yp=$this->PrnWrite('Forma de Implementação: ','Descentralizada',$yp);
        
        if ($res[0]['acasnmelincred']=='t') $yp=$this->PrnWrite('Forma de Implementação: ','Linha de Crédito',$yp);
        if ($res[0]['acadetalhamento']) $yp=$this->PrnWrite('Detalhamento da Forma de Implementação: ',$res[0]['acadetalhamento'],$yp);
        
    }
    if ($res[0]['acasnfiscalseguridade']=='t') $yp=$this->PrnWrite('Tipo de Orçamento: ','Fiscal',$yp);
    if ($res[0]['acasnfiscalseguridade']=='f') $yp=$this->PrnWrite('Tipo de Orçamento: ','Seguridade',$yp);
    if ($res[0]['acasninvestatais']=='t') $yp=$this->PrnWrite('Investimento: ','Estatais',$yp);

    if ($res[0]['taccod']==1)
    {
       $yp=$this->PrnWrite('Evolução da Situação Física: ',"",$yp);
       $yp=$this->PrnWrite('',"Total                            Realizado até ".(strval($_SESSION['exercicio'])-1)."                         Previsto em ".$_SESSION['exercicio'],$yp);

       $yp=$this->PrnWrite('',$res[0]['acaqtdcustototal'].'                                '.$res[0]['acaqtdateanoanterior'].'                                                    '.$res[0]['acaqtdprevistoanocorrente'],$yp);

       $yp=$this->PrnWrite('Evolução da Situação Financeira: ',"",$yp);
       $yp=$this->PrnWrite('',"Total                            Realizado até ".(strval($_SESSION['exercicio'])-1)."                         Previsto em ".$_SESSION['exercicio'],$yp);
       $yp=$this->PrnWrite('',number_format($res[0]['acavlrcustototal'],2,',','.').'              '.number_format($res[0]['acavlrcustoateanoanterior'],2,',','.').'                                                 '.number_format($res[0]['acavlrprevistoanocorrente'],2,',','.'),$yp);


    if ($res[0]['acarepercfinanceira']) $yp=$this->PrnWrite('Justificativa da Repercussão Financeira sobre o Custeio da União: ',$res[0]['acarepercfinanceira'],$yp);

    if ($res[0]['acavlrrepercfinanceira']) $yp=$this->PrnWrite('Valor Estimado da Repercussão Financeira (R$/ano): ',number_format($res[0]['acavlrrepercfinanceira'],2,',','.'),$yp);
    
       $yp=$this->PrnWrite('Data Início/Término: ',$res[0]['acamesinicio'].'/'.$res[0]['acaanoinicio'].' a '.$res[0]['acamestermino'].'/'.$res[0]['acaanotermino'],$yp);
    }
    if ($res[0]['esfdsc']) $yp=$this->PrnWrite('Esfera: ',$res[0]['esfdsc'],$yp);
    if ($res[0]['acabaselegal']) $yp=$this->PrnWrite('Base Legal: ',$res[0]['acabaselegal'],$yp);
    if ($res[0]['acahistorico']) $yp=$this->PrnWrite('Histórico: ',$res[0]['acahistorico'],$yp);
    $yp=$this->PrnWrite($resp[0]['descricao'].':',$resp[0]['usuario'],$yp);
    $yp=$this->PrnWrite($resp[1]['descricao'].':',$resp[1]['usuario'],$yp);
    //$yp=$this->PrnWrite($resp[2]['descricao'].':',$resp[2]['usuario'],$yp);
}

function FisEvo($j,$i,$acao)
{
    global $res;
    $this->SetFont('Arial','',8);
    $yp=$this->PrnWrite('Programa:',$res[$j]['prgcod'].'-'.$res[$j]['prgdsc'],$yp);
    $yp=$this->PrnWrite('Ação:',$res[$j]['acacod'].'.'.$res[$j]['unicod'].'.'.$res[$j]['loccod'].'-'.$res[$j]['acadsc'],$yp);  
    $yp=$this->PrnWrite('Localizador:',$res[$j]['sacdsc'],$yp);
    $yp=$this->PrnWrite('Produto:',$res[$j]['prodsc'],$yp); 
    $yp=$this->PrnWrite('Unidade de Medida:',$res[$j]['unmdsc'],$yp);
    $yp=$this->PrnWrite('Meta:',$res[$j]['previsto'],$yp);
    $yp=$this->PrnWrite('Tipo de meta:',$res[$j]['tipometa'],$yp);         
    $yp=$this->PrnWrite('Realizado no período:',$_SESSION['soma2']['soma'][$i],$yp);         
    $yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);
    $this->SetTextColor(241,14,0);
    $yp=$this->PrnWrite('','ACOMPANHAMENTO FÍSICO ',$yp);
    $this->SetTextColor(0,0,0);
    for ($m=0;$m<count($_SESSION['rspta']);$m++)    
    {
    	for ($n=0;$n<count($_SESSION['rspta'][0]);$n++)
    	{
    		if ($_SESSION['rspta'][$m][$n]['acaid']==$acao)
    		{
         		$yp=$this->PrnWrite('Referência:',$_SESSION['rspta'][$m][$n]['refdsc'],$yp);
         		$yp=$this->PrnWrite('Coordenador de Ação:',$_SESSION['rspta'][$m][$n]['autor'],$yp);  
         		$yp=$this->PrnWrite('Lançado em:',$_SESSION['rspta'][$m][$n]['pardata'],$yp);
         		$yp=$this->PrnWrite('Realizado:',$_SESSION['rspta'][$m][$n]['exprealizado'],$yp); 
         		$yp=$this->PrnWrite('Situação:',$_SESSION['rspta'][$m][$n]['tpsdsc'],$yp);
    			$yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);         		
            }
    	}
    	
    }

}

function Plantrab($j,$i,$acao)
{
    global $res;
    $this->SetFont('Arial','',8);
    $yp=$this->PrnWrite('Programa:',$res[$j]['prgcod'].'-'.$res[$j]['prgdsc'],$yp);
    $yp=$this->PrnWrite('Ação:',$res[$j]['acacod'].'.'.$res[$j]['unicod'].'.'.$res[$j]['loccod'].'-'.$res[$j]['acadsc'],$yp);  
    $yp=$this->PrnWrite('Localizador:',$res[$j]['sacdsc'],$yp);
    $yp=$this->PrnWrite('Produto:',$res[$j]['prodsc'],$yp); 
    $yp=$this->PrnWrite('Unidade de Medida:',$res[$j]['unmdsc'],$yp);
    $yp=$this->PrnWrite('Meta:',$res[$j]['previsto'],$yp);
    $yp=$this->PrnWrite('Tipo de meta:',$res[$j]['tipometa'],$yp);         
    $yp=$this->PrnWrite('Realizado no período:',$_SESSION['soma2t']['soma'][$i],$yp);         
    $yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);
    $this->SetTextColor(241,14,0);
    $yp=$this->PrnWrite('','PLANO DE TRABALHO ',$yp);
    $this->SetTextColor(0,0,0);
    for ($m=0;$m<count($_SESSION['rspto']);$m++)    
    {
    	for ($n=0;$n<count($_SESSION['rspto'][0]);$n++)
    	{
    		if ($_SESSION['rspto'][$m][$n]['acaid']==$acao)
    		{
    			if ($_SESSION['rspto'][$m][$n]['ptotipo']=='F') $tipo = 'Fase';
    			else if ($_SESSION['rspto'][$m][$n]['ptotipo']=='S') $tipo = 'Subação';
    			else $tipo='Etapa';
         		$yp=$this->PrnWrite('Tipo:',$tipo,$yp);
         		$yp=$this->PrnWrite('Código / Descrição:',trim($_SESSION['rspto'][$m][$n]['ptocod']).' / '.$_SESSION['rspto'][$m][$n]['ptodsc'],$yp); 
             	$yp=$this->PrnWrite('Unidade de medida:',$_SESSION['rspto'][$m][$n]['unmdsc'],$yp);
         		$yp=$this->PrnWrite('Previsão:',$_SESSION['rspto'][$m][$n]['previsto'],$yp);
         		$yp=$this->PrnWrite('Realizado:',$_SESSION['rspto'][$m][$n]['totalrealizado'],$yp); 
         		$yp=$this->PrnWrite('Período:',$_SESSION['rspto'][$m][$n]['inicio'].' a '.$_SESSION['rspto'][$m][$n]['fim'],$yp);
    			$yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);         		
            }
    	}
    	
    }

}

function FinEvo($j,$nt)
{
    global $res;
    $this->SetFont('Arial','',8);

    $acao = ereg_replace("<[^>]*>","",$res[$j]['restricao']);
    if ($nt==0) {
         $yp=$this->PrnWrite('Código:',$res[$j]['codigo'],$yp);
         $yp=$this->PrnWrite('Título:',$res[$j]['acadsc'],$yp);
    $yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);
    }
    else
        $yp=$this->PrnWrite("",'',$yp);
    $yp=$this->PrnWrite('Objeto:',$res[$j]['objeto'],$yp);
    $yp=$this->PrnWrite('Dotação Autorizada:',number_format($res[$j]['dotacao_disponivel'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Provisão Concedida:',number_format($res[$j]['provisao_concedida'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Provisão Recebida:',number_format($res[$j]['provisao_recebida'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Destaque Concedido:',number_format($res[$j]['destaque_concedido'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Destaque Recebido:',number_format($res[$j]['destaque_recebido'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Valor Empenhado:',number_format($res[$j]['empenhos_emitidos'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Valor Liquidado:',number_format($res[$j]['empenhos_liquidados'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Valor Pago:',number_format($res[$j]['valor_pago'],2,',','.'),$yp);
    $yp=$this->PrnWrite('---------------------------','-------------------------------------------',$yp);
}

function AvaliacaoParecer($j,$nt)
{
    global $res;
    $this->SetFont('Arial','',8);
    if ($nt==0) {
         $yp=$this->PrnWrite('Código:',$res[$j]['acacod'].'.'.$res[$j]['unicod'].'.'.$res[$j]['loccod'],$yp);
         $yp=$this->PrnWrite('Título:',$res[$j]['acadsc'],$yp);
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
    //if ($nt==0) {
         $yp=$this->PrnWrite('Código:',$res[$j]['codigo'],$yp);
         $yp=$this->PrnWrite('Título:',$res[$j]['acadsc'],$yp);
    //}
   // else
        $yp=$this->PrnWrite("",'',$yp);
    $yp=$this->PrnWrite("-----------------",'----------------------------------------------------',$yp);
    $yp=$this->PrnWrite('Restrição:',$acao,$yp);
    $yp=$this->PrnWrite('Tipo de Restrição:',$res[$j]['trsdsc'],$yp);
    $acao = ereg_replace("<[^>]*>","",$res[$j]['providencia']);
    $yp=$this->PrnWrite('Providência:',$acao,$yp);
    $yp=$this->PrnWrite('Data:',$res[$j]['datainc'],$yp);
    $yp=$this->PrnWrite('Analista:',$res[$j]['usunome'].'/'.$res[$j]['orgao'],$yp);
        $yp=$this->PrnWrite("",'',$yp);
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
    $this->SetTextColor(0);
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

function FisEvoTot($data,$k)
{
    global $res;
    $this->SetFont('Times','',8);
    for($j=0;$j<=$k;$j++)
    {
    $this->SetFont('Arial','',8);
    $yp=$this->PrnWrite("",'',$yp);
    $yp=$this->PrnWrite('Ação:',$res[$j]['codigo'],$yp);
    $yp=$this->PrnWrite('Unidade:',$res[$j]['unidsc'],$yp);
    $yp=$this->PrnWrite('Região:',$res[$j]['regdsc'],$yp);
    $yp=$this->PrnWrite('Produto:',$res[$j]['prodsc'],$yp);
    $yp=$this->PrnWrite('Tipo:',$res[$j]['tipometa'],$yp);
    $yp=$this->PrnWrite('Unid. Med.:',$res[$j]['unmdsc'],$yp);
    $yp=$this->PrnWrite('Realizado:',number_format($res[$j]['executado'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Meta:',number_format($res[$j]['meta'],2,',','.'),$yp);
    $yp=$this->PrnWrite('Percent.:',number_format($res[$j]['percent'],2,',','.'),$yp);
    $yp=$this->PrnWrite('---------------------------','--------------------------------------------------------------------------------------',$yp);  
    }  
}


}
  
//Instanciation of inherited class
$dat = $_SESSION['data'];
$title4 = $_SESSION['title'];
//if (count($_SESSION['rsfisevotot']) == 0)
  $pdf=new PDF('P','pt','A4');
//  else $pdf=new PDF('L','pt','A4');
$pdf->SetFont('Times','',6);
$pdf->Open();
$pdf->AliasNbPages();
// calcula as larguras de cada coluna


$title = 'SISTEMA INTEGRADO DE MONITORAMENTO DO MINISTÉRIO DA EDUCAÇÃO';

if (count($_SESSION['rsdados']) > 0)
{
  // solicitou dados básicos
  $title3 = 'Dados Básicos de Ação';
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
  $title3 = 'Avaliação e Parecer de Ação';
  $acacod='';
  for ($i=0; $i<=count($_SESSION['rsavapar'])-1;$i++)
  {
   $res = $_SESSION['rsavapar'][$i];
   for ($j=0; $j<=count($_SESSION['rsavapar'][$i])-1;$j++)
   {
   if (($res[$j]['avaliacao']<>'' or $res[$j]['parecer']<>'') and ($res[$j]['avlib']=='t' or $res[$j]['parlib']=='t') ){
        $acacod2 = $res[$j]['acacod'].$res[$j]['unicod'].$res[$j]['loccod'];
        if (! $acacod2 == $acacod){
           $pdf->AddPage();
           $acacod=$acacod2;
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
  $title3 = 'Financeiro Evolução de Ação';
  $acacod='';
  for ($i=0; $i<=count($_SESSION['rsfinevo'])-1;$i++)
  {
   $res = $_SESSION['rsfinevo'][$i];
   for ($j=0; $j<=count($_SESSION['rsfinevo'][$i])-1;$j++)
   {
   if ($res[$j]['objeto']<>''){
        $acacod2 = $res[$j]['objeto'];
        if (! $acacod2 == $acacod)
        {
           $pdf->AddPage();
           $acacod=$acacod2;
           $pdf->setxy(120,150);
           $nt = 0;
         }
         else $nt=1;
     $pdf->FinEvo($j,$nt);
   }
   }
  }
}

if (count($_SESSION['rsfisevo']) > 0)
{
	// Físico Evolução de Ação
  $title3 = 'Físico Evolução de Ação';
  for ($i=0; $i<=count($_SESSION['rsfisevo'])-1;$i++)
  {
   	  $res = $_SESSION['rsfisevo'][$i];
   	  for ($j=0; $j<=count($_SESSION['rsfisevo'][$i])-1;$j++)
      {
         if ($res[$j]['acaid']<>'')
         {
           $pdf->AddPage();
           $pdf->setxy(120,150);
           $pdf->FisEvo($j,$i,$res[$j]['acaid']);
         }
      }
  } 
}

if (count($_SESSION['rsfisevotot']) > 0)
{
  // solicitou fisico evolucao totais
  $title3 = 'Evolução Física Totais';
  $res = $_SESSION['rsfisevotot'][0];
  $pdf->AddPage();
  $pdf->FisEvoTot($_SESSION['rsfisevotot'],count($_SESSION['rsfisevotot'][0]));


}

if (count($_SESSION['rsplantrab']) > 0)
{
    // plano de trabalho de Ação
   $title3 = 'Plano de Trabalho da Ação';

  for ($i=0; $i<=count($_SESSION['rsplantrab'])-1;$i++)
  {
   	  $res = $_SESSION['rsplantrab'][$i];
   	  for ($j=0; $j<=count($_SESSION['rsplantrab'][$i])-1;$j++)
      {
         if ($res[$j]['acaid']<>'')
         {
           $pdf->AddPage();
           $pdf->setxy(120,150);
           $pdf->Plantrab($j,$i,$res[$j]['acaid']);
         }
      }
  }   
}

if (count($_SESSION['rsrespro']) > 0)
{
  // solicitou restrição e providência
  $title3 = 'Restrições e Providências de Ação';
  $acacod='';
  for ($i=0; $i<=count($_SESSION['rsrespro'])-1;$i++)
  {
   $res = $_SESSION['rsrespro'][$i];
   for ($j=0; $j<=count($_SESSION['rsrespro'][$i])-1;$j++)
   {
   if ($res[$j]['restricao']<>''){
        $acacod2 = $res[$j]['codigo'];
        if ($acacod2 <> $acacod){
           $pdf->AddPage();
           $acacod=$acacod2;
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

  for ($i=0; $i<=count($_SESSION['rsparalt'])-1;$i++)
  {
   $res = $_SESSION['rsparalt'][$i];
   if ($res[0]['parecer']<>''){
      $pdf->AddPage();
      $pdf->ParAltGest();
   }

  }
}

$pdf->Output();
?>
