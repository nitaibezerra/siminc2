<?php



ini_set( "display_error", 1 );
error_reporting( E_ALL ^ E_NOTICE );

//dump( ini_get( "memory_limit" ), true );

ini_set( "memory_limit", "1024M" ); // ...

$condicao = array();


// instrumento

//array_push( $condicao, " ps.ppsrel = true   ");
array_push( $condicao, " p.ptostatus = 'A'  ");
//array_push( $condicao, " iu. =  " . );

// demanda
// sempre é M
// array_push( $condicao, "/ " );


//ideb
/*
if ( count( $_REQUEST['ideb'] ) && $_REQUEST['ideb'][0] )
{
	array_push( $condicao, " mt.tpmid in ( '" . implode( "','", $_REQUEST['ideb'] ) . "' ) " );
}
*/
// uf
if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] )
{
	array_push( $condicao, " m.estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
}

// municipio
/*
if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] )
{
	array_push( $condicao, " iu.muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
}
*/
// unidade de medida
if ( count( $_REQUEST['undid'] ) && $_REQUEST['undid'][0] )
{
	array_push( $condicao, " u.undid in ( '" . implode( "','", $_REQUEST['undid'] ) . "' ) " );
}
// programa
if ( count( $_REQUEST['prgid'] ) && $_REQUEST['prgid'][0] )
{
	array_push( $condicao, " s.prgid in ( '" . implode( "','", $_REQUEST['prgid'] ) . "' ) " );
}
// Formacao
if ( count( $_REQUEST['sbastgmpl'] ) && $_REQUEST['sbastgmpl'][0] )
{
	array_push( $condicao, " s.sbastgmpl in ( '" . implode( "','", $_REQUEST['sbastgmpl'] ) . "' ) " );
}
// FORMA DE EXECUCAO
if ( count( $_REQUEST['frmid'] ) && $_REQUEST['frmid'][0] )
{
	array_push( $condicao, " s.frmid in ( '" . implode( "','", $_REQUEST['frmid'] ) . "' ) " );
}
// grandes cidades
/*
if ( count( $_REQUEST['grandescidades'] ) )
{
	array_push( $condicao, " mt.tpmid = " . ( (integer) $_REQUEST['grandescidades'] ) );
}
*/
$condicao = count( $condicao ) ? " where " . implode( " and ", $condicao ) : "";
//$condicao = count( $condicao ) ? " where " . implode( " and ", $condicao ) : "";

$sql = "	
select
	trim(m.estuf) as Estado,
	trim(s.sbastgmpl) as Formacao,
	trim(pr.prgdsc) as Programa,
	trim(u.unddsc) as \"unidade\",
	trim(ad.arddsc) as \"area\",
	trim(f.frmdsc) as \"forma\",
	coalesce(sum(s.sba1ano),0) as \"2008\",
	coalesce(sum(s.sba2ano),0) as \"2009\",
	coalesce(sum(s.sba3ano),0) as \"2010\",
	coalesce(sum(s.sba4ano),0) as \"2011\"
from 

	cte.subacaoindicador as s 	
	inner join cte.acaoindicador a 		  ON a.aciid 	= s.aciid 
	inner join cte.pontuacao p 		  ON p.ptoid 	= a.ptoid
	inner join cte.indicador i 		  ON i.indid 	= p.indid
	inner join cte.areadimensao ad 		  ON ad.ardid 	= i.ardid
	inner join cte.instrumentounidade iu 	  ON iu.inuid 	= p.inuid 
	inner join territorios.estado m 	  ON m.estuf 	= iu.estuf 
	inner join cte.programa pr 	   	  ON pr.prgid 	= s.prgid 
	inner join cte.unidademedida u 	   	  ON u.undid 	= s.undid
	inner join cte.formaexecucao f 	   	  ON f.frmid 	= s.frmid
$condicao
group by 
	m.estuf,
	s.sbastgmpl,
	pr.prgdsc,
	u.unddsc,
	ad.arddsc,
	f.frmdsc
order by						
	m.estuf,
	s.sbastgmpl,
	pr.prgdsc,
	u.unddsc,
	ad.arddsc,
	f.frmdsc

";
//dump($sql,true);
/*
if(isset( $_REQUEST['xls']))
{

		$sqlQddXls = "	
				select
					trim(m.estuf) as \"Estado\",
					--trim(m.mundescricao) as \"Municipio\",
					trim(s.sbastgmpl) as \"Formação\",
					trim(pr.prgdsc) as \"Programa\",
					trim(u.unddsc) as \"Unidade de Medida\",
					coalesce(sum(s.sba1ano),0) as \"2008\",
					coalesce(sum(s.sba2ano),0) as \"2009\",
					coalesce(sum(s.sba3ano),0) as \"2010\",
					coalesce(sum(s.sba4ano),0) as \"2011\"
				from 
					cte.subacaoindicador as s 	
					inner join cte.acaoindicador a 		  ON a.aciid 	= s.aciid 
					inner join cte.pontuacao p 		  ON p.ptoid 	= a.ptoid 
					inner join cte.instrumentounidade iu 	  ON iu.inuid 	= p.inuid 
					inner join territorios.estado m 	  ON m.estuf 	= iu.estuf 
					inner join cte.programa pr 	   	  ON pr.prgid 	= s.prgid 
					inner join cte.unidademedida u 	   	  ON u.undid 	= s.undid
					--left join territorios.muntipomunicipio  mt on mt.muncod = m.muncod and mt.estuf = m.estuf 
				$condicao
				group by 
					m.estuf,
					--m.mundescricao,
					s.sbastgmpl,
					pr.prgdsc,
					u.unddsc					
				order by						
					m.estuf,
					--m.mundescricao,
					s.sbastgmpl,
					pr.prgdsc,
					u.unddsc
				";
		
	echo"<pre>";
	print_r($_REQUEST);
	echo"</pre>";
	dump($sqlQddXls,true);
	
	die();
				
	//$db->sql_to_excel( $sqlQddXls, "relatorioXls", 'Relatório Xml', array('s','s','s','s','s','n','n','n','n') );
	$db->sql_to_excel( $sqlQddXls, "relatorioXls", 'Relatório Xml', array('s','s','s','s','n','n','n','n') );
	die();
}
*/

$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();

//dump($dados,true);
//$agrupadores = array( 'estado', 'dimensao' );
$agrupador = (array) $_REQUEST['agrupador'];
//array_unshift( $agrupador, "pais" );



$dados = cte_agruparDadosRelatorio_2( $agrupador, $dados );
//dump($dados,true);
//dump( memory_get_peak_usage(), true );
function cte_agruparDadosRelatorio_2( array $agrupadores, array $itens )
{
	if ( count( $agrupadores ) == 0 || count( $itens ) == 0 )
	{
		return array();
	}
	
	// captura agrupador atual
	$agrupadorAtual = array_shift( $agrupadores );
	
	// inicia variavel resultante
	$resultado = array();
	
	// percorre itens (realiza agrupamento)
	foreach ( $itens as $item )
	{
		
		$chave = $item[$agrupadorAtual];
		if ( !array_key_exists( $chave, $resultado ) )
		{
			$resultado[$chave] = $item;
			
			 $resultado[$chave]['2008']  = 0;
			 $resultado[$chave]['2009']  = 0;
			 $resultado[$chave]['2010']  = 0;
			 $resultado[$chave]['2011']  = 0;
			
			// filhos
			$resultado[$chave]['sub'] = array();
		}
		// adiciona valores do item ao agrupador
		
		$resultado[$chave]['2008'] += $item['2008'];
		$resultado[$chave]['2009'] += $item['2009'];
		$resultado[$chave]['2010'] += $item['2010'];
		$resultado[$chave]['2011'] += $item['2011'];
		//$resultado[$chave]['fin_sol'][4] += $item['fin_4_' . $campoSolicitacao];

		if ( count( $agrupadores ) > 0 )
		{
			array_push( $resultado[$chave]['sub'], $item );
		}
		
	}
	
	// agrupa filhos dos filhos do agrupador atual caso haja mais agrupadores
	
	reset( $agrupadores );
	if ( count( $agrupadores ) > 0 )
	{
		reset( $resultado );
		foreach ( $resultado as &$item )
		{
			$item['sub'] = cte_agruparDadosRelatorio_2( $agrupadores, $item['sub'] );
		}
	}
	
	ksort( $resultado );
	reset( $resultado );
	return $resultado;
}

function cte_desenhaRelatorio2( array $itens, $profundidade = 0 )
{
	if ( count( $itens ) == 0 )
	{
		return;
	}
	
	
	$padding = $profundidade * 25;
	
	$corSim = "#d9d9d9;";
	$corNao = "";
	$cor = "#d9d9d9;";
	foreach ( $itens as $agrupador => $item )
	{
		if($cor == $corNao){
			$cor = $corSim;
		}
		else
		{
			$cor = "";
		}
		
		
		?>
		<tr style="background-color:#e0e0e0" >
			<td style="padding-left: <?= $padding ?>px;" colspan="7">
				<?php if ( $profundidade > 0 ) : ?>
					<img src="/imagens/seta_filho.gif" align="absmiddle"/>
				<?php endif; ?>
				<b><?= $agrupador ?></b>
			</td>
		</tr>
		<tr style="background-color:#f9f9f9" >
			
			<td width=700px">&nbsp;</td>
			<td>				
				<?= $item['2008'] ?>
			</td>
			<td>				
				<?= $item['2009'] ?>
			</td>
			<td>				
				<?= $item['2010'] ?>
			</td>
			<td>				
				<?= $item['2011'] ?>
			</td>
			
		</tr>
		
		<?php
		cte_desenhaRelatorio2( $item['sub'], $profundidade + 1 );
	}
}
/*

echo "<pre>";
print_r($dados);
echo "</pre>";

*/
if(isset( $_REQUEST['xls']))
{
	preparaXLS($dados, $agrupador);
}
else
{
	desenhaTelaHtml($dados);
}

//die();

function desenhaTelaHtml($dados)
{
	?>
	<html>
		<head>
			<title>Relatório Geral CTE</title>
			<script type="text/javascript" src="../../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
			<style type="text/css">
				body{ margin: 0; padding: 0; }
			</style>
			<script type="text/javascript">
				top.window.focus();
			</script>
		</head>
		<body>
			<?php if ( count( $dados ) ) : ?>
				<table class="tabela" style="width:100%; background-color: #fefefe;" id= "relatorioPar" border="0" cellpadding="2" cellspacing="1" align="center">
					<tr style="background-color: #d9d9d9;">
						<td >&nbsp;</td>
						
						<td align="center" >2008</td>
						<td align="center" >2009</td>
						<td align="center" >2010</td>
						<td align="center" >2011</td>
						
					</tr>
					<?php
						cte_desenhaRelatorio2($dados);
					?>
				</table>
			<?php else : ?>
				<p align="center" style="color: #903030;">
					<br/><br/>
					Nenhum resultado encontrado para o filtro preenchido.
				</p>
			<?php endif; ?>
		</body>
	</html>
	<?
}

function cte_populaXML(array $itens, $agrupamento)
{
	if ( count( $itens ) == 0 )
	{
		return;
	}
	
	foreach ( $itens as $agrupador => $item )
	{
		for($i=0; $i< count($agrupamento); $i++ )
			echo $item[$agrupamento[$i]]."\t";
		
		echo  $item['2008'] ."\t".$item['2009'] ."\t".$item['2010'] ."\t".$item['2011']."\n";
		
		cte_populaXML( $item['sub'], $agrupamento);
	}
}

function preparaXLS($dados,$agrupador)
{
	
	header('Content-type: application/x-msdownload');
	//header('Content-type:  text/plain');
	header('Content-Disposition: attachment; filename=SIMEC_Relatorio_PAR.xls');
	header('Pragma: no-cache');
	header('Expires: 0');
	/**/
	echo "Relatório Geral \n\n\n";
	
	
	for ($x = 0; $x < count($agrupador); $x++)
	{
		echo $agrupador[$x] ."\t";
	}

	echo "2008\t2009\t2010\t2011\t \n";

	if ( count( $dados ) ) 
	{

		cte_populaXML($dados,$agrupador);
	}
	else
	{
		echo "\n\n\n\nNenhum resultado encontrado para o filtro preenchido\n\n\n\n";
	}
	
	die();
}

?>





