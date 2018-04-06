<?php

include 'config.inc';
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

include APPRAIZ . 'includes/jpgraph/jpgraph.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_pie.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_pie3d.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_bar.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_line.php';
include APPRAIZ . "financeiro/modulos/relatorio/funcoes_consulta_financeiro.inc";
$db = new cls_banco();

switch ( $_REQUEST['tipo'] )
{
	
	case 'usuario_hora':
		$sql = 'select
EXTRACT(HOUR from estdata) as hora,
count(distinct usucpf) as quantidade
from
seguranca.estatistica
group by EXTRACT(HOUR from estdata) order by 1
';
		
		$dados = array();
		$legenda = array();
		foreach ( $db->carregar( $sql ) as $sistema )
		{
			$dados[] = $sistema['quantidade'];
			$legenda[] = $sistema['hora'];
		}
		
		// monta gráfico
		$graph = new Graph( 800, 200, 'auto' );	
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 100, 30, 15, 20 );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->title->Set( 'Usuários por hora' );
		$graph->title->SetColor( 'black' );
		//$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		//$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->yaxis->SetColor( 'black', 'black' );
		//$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		//$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		$graph->xaxis->SetColor( 'black', 'black' );
		//$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetTickLabels( $legenda );
		$graph->legend->setPos( 0.02, 0.36 );
		
		// monta plotagem
		$bplot = new BarPlot( $dados );
		$bplot->SetWidth( 0.3 );
		$tcol = array( 0, 0, 255 );
		$fcol = array( 200, 200, 255 );
		$bplot->SetFillGradient( $fcol, $tcol, GRAD_HOR );
		
		// imprime imagem
		$graph->Add( $bplot );
		$graph->Stroke();
		break;
	
		case 'hits_hora':
		$sql = 'select
EXTRACT(HOUR from estdata) as hora,
count(mnuid) as quantidade
from
seguranca.estatistica
group by EXTRACT(HOUR from estdata) order by 1';
		
		$dados = array();
		$legenda = array();
		foreach ( $db->carregar( $sql ) as $sistema )
		{
			$dados[] = $sistema['quantidade'];
			$legenda[] = $sistema['hora'];
		}
		
		// monta gráfico
		$graph = new Graph( 800, 200, 'auto' );	
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 100, 30, 15, 20 );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->title->Set( 'Page hits por hora' );
		$graph->title->SetColor( 'black' );
		//$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		//$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->yaxis->SetColor( 'black', 'black' );
		//$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		//$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		$graph->xaxis->SetColor( 'black', 'black' );
		//$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetTickLabels( $legenda );
		$graph->legend->setPos( 0.02, 0.36 );
		
		// monta plotagem
		$bplot = new BarPlot( $dados );
		$bplot->SetWidth( 0.3 );
		$tcol = array( 0, 255, 0 );
		$fcol = array( 200, 255, 200 );
		$bplot->SetFillGradient( $fcol, $tcol, GRAD_HOR );
		
		// imprime imagem
		$graph->Add( $bplot );
		$graph->Stroke();
		break;
	
	
	
	case 'hits_mes':
		$sql = 'select
EXTRACT(year from estdata) as ano,
EXTRACT(month from estdata) as mes,
count(mnuid) as quantidade
from
seguranca.estatistica
group by EXTRACT(year from estdata), EXTRACT(month from estdata) order by 1, 2';
		
		$dados = array();
		$legenda = array();
		foreach ( $db->carregar( $sql ) as $sistema )
		{
			$dados[] = $sistema['quantidade'];
			$legenda[] = $sistema['mes'] . '/' . substr( $sistema['ano'], -2 );
		}
		
		// monta gráfico
		$graph = new Graph( 400, 200, 'auto' );	
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 100, 30, 15, 20 );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->title->Set( 'Acesso ao sistema mensal' );
		$graph->title->SetColor( 'black' );
		//$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		//$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->yaxis->SetColor( 'black', 'black' );
		//$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		//$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		$graph->xaxis->SetColor( 'black', 'black' );
		//$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetTickLabels( $legenda );
		$graph->legend->setPos( 0.02, 0.36 );
		
		// monta plotagem
		$bplot = new BarPlot( $dados );
		$bplot->SetWidth( 0.2 );
		$tcol = array( 255, 210, 100 );
		$fcol = array( 255, 245, 220 );
		$bplot->SetFillGradient( $fcol, $tcol, GRAD_HOR );
		
		// imprime imagem
		$graph->Add( $bplot );
		$graph->Stroke();
		break;
	
	
	
	case 'usuario_ano':
$sql = "SELECT EXTRACT( YEAR from usudatainc) as data,
count(usucpf) as usuarios
FROM seguranca.usuario GROUP BY EXTRACT(YEAR from usudatainc)
order by EXTRACT(YEAR from usudatainc)";
		$legenda = array();
		$dados = array();
		foreach ( $db->carregar( $sql ) as $linha )
		{
			$dados[] = $linha['usuarios'];
			$legenda[] = $linha['data'];
		}
		
		// monta gráfico
		$graph = new PieGraph( 500, 250, 'auto' );
		$graph->SetFrameBevel( 0, false );
		$graph->title->Set( 'Inclusão de usuário anual' );
		//$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 ); 
		$graph->title->SetColor( 'black' );
		$graph->subtitle->SetColor( 'darkslategray' );
		$graph->legend->Pos( 0.02, 0.36 );
		$graph->legend->SetShadow( false );
		
		// monta plotagem
		$p1 = new PiePlot3d( $dados );
		$p1->SetSliceColors( array( 'yellow', 'red' ) );
		$p1->SetCenter( 0.32, 0.48 );
		$p1->SetSize( 120 );
		$p1->SetAngle( 40 );
		$p1->SetStartAngle( 0 );
		//$p1->value->SetFont( FF_ARIAL, FS_NORMAL, 9 );
		$p1->value->SetColor( 'darkslategray' );
		//$p1->SetEdge( 'darkslategray' );
		$p1->SetLegends( $legenda );
		
		// imprime imagem
		$graph->Add( $p1 );
		$graph->Stroke();
		break;
	case 'usuario_ano_mes':
		$sql = "SELECT EXTRACT( YEAR from usudatainc) || '/' || EXTRACT( MONTH from usudatainc) as data,
count(usucpf) as usuarios
FROM seguranca.usuario GROUP BY EXTRACT(YEAR from usudatainc), EXTRACT( MONTH from usudatainc)
order by EXTRACT(YEAR from usudatainc), EXTRACT( MONTH from usudatainc)";
		$legenda = array();
		$dados = array();
		$soma = 0;
		foreach ( $db->carregar( $sql ) as $linha )
		{
			$dados[] = $linha['usuarios'] + $soma;
			$legenda[] = $linha['data'];
			$soma += $linha['usuarios'];
		}
		// monta gráfico
		$graph = new Graph( 800, 200, 'auto' );	
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 100, 30, 15, 20 );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->title->Set( 'Usuários cadastrados' );
		$graph->title->SetColor( 'black' );
		//$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		//$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->yaxis->SetColor( 'black', 'black' );
		//$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		//$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		$graph->xaxis->SetColor( 'black', 'black' );
		//$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetTickLabels( $legenda );
		$graph->legend->setPos( 0.02, 0.36 );
		
		// monta plotagem
		$bplot = new BarPlot( $dados );
		$bplot->SetWidth( 0.1 );
		$tcol = array( 255, 0, 0 );
		$fcol = array( 255, 200, 200 );
		$bplot->SetFillGradient( $fcol, $tcol, GRAD_HOR );
		
		// imprime imagem
		$graph->Add( $bplot );
		$graph->Stroke();
		/*
		// Create the graph. These two calls are always required
		$graph  = new Graph(350, 250,"auto");
		$graph->title->Set( 'Inclusão de usuário mensal' );
		$graph->title->SetColor( 'black' );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->xaxis->SetTickLabels( $legenda );
		$graph->SetScale( "textlin");
		// Create the linear plot
		$lineplot = new LinePlot( $dados );
		$lineplot->SetColor("blue");
		// Add the plot to the graph
		$graph->Add( $lineplot );
		// Display the graph
		$graph->Stroke(); 
*/
		break;
	case 'usuario_sistema':
		$sql = 'SELECT (SELECT count(*) FROM seguranca.usuario ) AS total_simec,
sisabrev as sistema,
count(usucpf) as usuarios
FROM seguranca.usuario_sistema join seguranca.sistema using (sisid) group by sisabrev;';
		
		$dados = array();
		$legenda = array();
		$legenda[] = 'Total';
		foreach ( $db->carregar( $sql ) as $sistema )
		{
			$dados['total'] = $sistema['total_simec'];
			$dados[] = $sistema['usuarios'];
			$legenda[] = $sistema['sistema'];
		}
		
		// monta gráfico
		$graph = new Graph( 1200, 300, 'auto' );	
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 100, 30, 15, 20 );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->title->Set( 'Usuários por sistema' );
		$graph->title->SetColor( 'black' );
		//$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		//$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->yaxis->SetColor( 'black', 'black' );
		//$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		//$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		$graph->xaxis->SetColor( 'black', 'black' );
		//$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetTickLabels( $legenda );
		$graph->legend->setPos( 0.02, 0.36 );
		
		// monta plotagem
		$bplot = new BarPlot( $dados );
		$bplot->SetWidth( 0.1 );
		$tcol = array( 255, 210, 100 );
		$fcol = array( 255, 245, 220 );
		$bplot->SetFillGradient( $fcol, $tcol, GRAD_HOR );
		
		// imprime imagem
		$graph->Add( $bplot );
		$graph->Stroke();
		break;
}

