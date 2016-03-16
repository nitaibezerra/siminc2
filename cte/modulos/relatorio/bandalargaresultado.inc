<?php
//dbg($_REQUEST,1);
//include APPRAIZ . 'includes/cabecalho.inc';
// regiao
$condicaoInterno = array();
if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] )
{
	array_push( $condicaoInterno, " regcod in  ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
}

// uf
if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] )
{
	array_push( $condicaoInterno, " estuf in  ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
}
//mesoregiao
if ( count( $_REQUEST['mesdsc'] ) && $_REQUEST['mesdsc'][0] )
{
	array_push( $condicaoInterno, " codigomesoregiao in  ( '" . implode( "','", $_REQUEST['mesdsc'] ) . "' ) " );
}
// municipio
if ( count( $_REQUEST['municipios'] ) && $_REQUEST['municipios'][0] )
{
	array_push( $condicaoInterno, " muncod in  ( '" . implode( "','", $_REQUEST['municipios'] ) . "' ) " );
}
// Categoria prioridade
if ( count( $_REQUEST['tpadsc'] ) && $_REQUEST['tpadsc'][0] )
{
	array_push( $condicaoInterno, " prioridade in  ( '" . implode( "','", $_REQUEST['tpadsc'] ) . "' ) " );
}
// Operadora
if ( count( $_REQUEST['oprdsc'] ) && $_REQUEST['oprdsc'][0] )
{
	array_push( $condicaoInterno, " operadora in  ( '" . implode( "','", $_REQUEST['oprdsc'] ) . "' ) " );
}
// Lab instalado
if($_REQUEST['labinstalado'] != "" ){
	array_push( $condicaoInterno, " possuilaboratorio ='".$_REQUEST['labinstalado']."'");
	$labinstal = $_REQUEST['labinstalado'];
}else{
	$labinstal = "";	
}
// Existe banda larga Instalada
if($_REQUEST['BLInstal'] != "" ){
	array_push( $condicaoInterno, " bdlbanda ='".$_REQUEST['BLInstal']."'");
	$BLInstal = $_REQUEST['BLInstal'];
}else{
	$BLInstal = "";	
}
// Operadora possui area de cobertura
if ( $_REQUEST['Corbetura'] != "" )
{
	array_push( $condicaoInterno, " areaoperadora =  '".$_REQUEST['Corbetura']."'" );
}
// Operadora possui area de cobertura
if ( $_REQUEST['dataprevinsta'] != "" )
{
	array_push( $condicaoInterno, " dataprevbandainst ='".$_REQUEST['dataprevinsta']."'");
}
// Data de instalação da banda larga
if ( $_REQUEST['atidatainicio'] != "" && $_REQUEST['atidatafim'] != "" )
{
	array_push( $condicaoInterno, " datainst between '" . formata_data_sql($_REQUEST['atidatainicio']) . "' and '" . formata_data_sql($_REQUEST['atidatafim']) . "'");
}

$condicaoInterno = count( $condicaoInterno ) ? " where " . implode( " and ", $condicaoInterno ) : "";
$sql = "select * from cte.temp " . $condicaoInterno . "order by estuf, estado";

//dbg($sql,1);

if($_REQUEST['tipo'] == "xls"){
	$cabecalho = array(
		"código", "país","região","estado","município","operadora","laboratorio","possui laboratorio","internet","possui internet","prioridade","codigo prioridade","código da região","sigla da UF"
		,"codigo mesoregião","data da previsão de instalação","data de instalação","area da operadora", "quantidade de escolas","quantidade de aluno","quantidade de professor","quantidade de laboratorio","quantidade da banda larga"
		,"quantidade de banda larga em 2008","quantidade de banda larga em 2009","quantidade de banda larga em 2010"
	);
	/*
	echo("<pre>");
	print_r($cabecalho);
	echo("</pre>");
*/
//	$cabecalho = "";
	$db->sql_to_excel($sql,"BandaLarga",$cabecalho);
	exit;

}
//dbg($sql,1);
//echo($sql);
$dados = $db->carregar( $sql );
//dump($dados);
$dados = $dados ? $dados : array();

//calculando os totais
$total_p = 0;
$total_r = 0;
foreach ( $dados as $item )
	{	
		$total_p   += $item["qtd_2008_4"] +$item["qtd_2009_2011"] ;
		$total_r   += $item["qtd_2008_4_r"] +$item["qtd_2009_2011_r"] ;
	}
	
function agrupar( $lista, $agrupadores )
{
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false )
	{
		return array();
	}
	$campo = array_shift( $agrupadores );
	$novo = array();
	foreach ( $lista as $item )
	{
		$chave = $item[$campo];
		if ( array_key_exists( $chave, $novo ) == false )
		{
			
			$novo[$chave] = array(
				"qtd_escola"      => 0,
				"qtd_aluno"       => 0,
				"qtd_professor"   => 0,
				"qtd_laboratorio" => 0,
				"qtd_bandalarga"  => 0,
				"qtd_2008_1"      => 0,
				"qtd_2008_2"      => 0,
				"qtd_2008_3"      => 0,
				"qtd_2008_4"      => 0,
				"qtd_2009_1_r"    => 0,
				"qtd_2009_2_r"    => 0,
				"qtd_2009_3_r"    => 0,
				"qtd_2009_4_r"    => 0,
				"qtd_2009_2011"   => 0,
				"agrupador"       => $campo,
				"muncod"          => $item["muncod"],
				"sub_itens"       => array()
			);
		}	
		
		$novo[$chave]["qtd_escola"]      += $item["qtd_escola"];
		$novo[$chave]["qtd_aluno"]       += $item["qtd_aluno"];
		$novo[$chave]["qtd_professor"]   += $item["qtd_professor"];
		$novo[$chave]["qtd_laboratorio"] += $item["qtd_laboratorio"];
		$novo[$chave]["qtd_bandalarga"]  += $item["qtd_bandalarga"];		
		$novo[$chave]["qtd_2008_1"]        += $item["qtd_2008_1"];
		$novo[$chave]["qtd_2008_2"]        += $item["qtd_2008_2"];
		$novo[$chave]["qtd_2008_3"]        += $item["qtd_2008_3"];
		$novo[$chave]["qtd_2008_4"]        += $item["qtd_2008_4"];
		$novo[$chave]["qtd_2008_1_r"]        += $item["qtd_2008_1_r"];
		$novo[$chave]["qtd_2008_2_r"]        += $item["qtd_2008_2_r"];
		$novo[$chave]["qtd_2008_3_r"]        += $item["qtd_2008_3_r"];
		$novo[$chave]["qtd_2008_4_r"]        += $item["qtd_2008_4_r"];
		
		$novo[$chave]["qtd_2009_2011"]        += $item["qtd_2009_2011"];		
		
		if ( $existeProximo )
		{
			array_push( $novo[$chave]["sub_itens"], $item );
		}
	}
	if ( $existeProximo )
	{
		foreach ( $novo as $chave => $dados )
		{
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );
		}
	}
	return $novo;
}

function exibir( $lista, $profundidade = 0 )
{
	if ( count( $lista ) == 0 )
	{
		return;
	}	
	
	foreach ( $lista as $chave => $dados )
	{		
		$escola      = (integer) $dados["qtd_escola"];
		$aluno       = (integer) $dados["qtd_aluno"];
		$professor   = (integer) $dados["qtd_professor"];
		$laboratorio = (integer) $dados["qtd_laboratorio"];
		$bandalarga  = (integer) $dados["qtd_bandalarga"];
		$_2008_1       = (integer) $dados["qtd_2008_1"];
		$_2008_2       = (integer) $dados["qtd_2008_2"];
		$_2008_3       = (integer) $dados["qtd_2008_3"];
		$_2008_4	   = (integer) $dados["qtd_2008_4"];
		$_2008_1_r     = (integer) $dados["qtd_2008_1_r"];
		$_2008_2_r     = (integer) $dados["qtd_2008_2_r"];
		$_2008_3_r     = (integer) $dados["qtd_2008_3_r"];
		$_2008_4_r	   = (integer) $dados["qtd_2008_4_r"];
		$_2009_2011	   = (integer) $dados["qtd_2009_2011"];
		$_2009_2011_r  = (integer) $dados["qtd_2009_2011_r"];
		//$total_p	   += $_2008_1 + $_2008_2 + $_2008_3 + $_2008_4 + $_2009_2011;
		//$total_r       += $_2008_1_r + $_2008_2_r + $_2008_3_r + $_2008_4_r + $_2009_2011_r;
		
		$muncod      = $dados["muncod"];
		$agrupador   = $dados["agrupador"];			
		
		
?>

<tr bgcolor="#f9f9f9">
	<td style="padding-left:<?= $profundidade * 20 ?>px;">
		<?php if ( $profundidade > 0 ): ?><img src="../imagens/seta_filho.gif" align="absmiddle"/><?php endif; ?>
		<?php if ( $agrupador == "municipio" ): ?>
			<!-- <img src="../imagens/mais.gif" align="absmiddle" onclick="detalharMunicipio( '<?= $muncod ?>', '<?= $GLOBALS["labinstal"] ?>', '<?= $GLOBALS["BLInstal"] ?>' );" id="img_<?= $muncod ?>" onmouseover="this.style.cursor = 'pointer';"/> -->
			<img src="../imagens/mais.gif" align="absmiddle" onclick="detalharMunicipio( '<?= $muncod ?>', '<?= $GLOBALS["labinstal"] ?>', '<?= $GLOBALS["BLInstal"] ?>', '<?= $_REQUEST[BLpolo] ?>' );" id="img_<?= $muncod ?>" onmouseover="this.style.cursor = 'pointer';"/>

		<?php endif; ?>
		<b><?= $chave ?></b>
	</td>
	<td align="right"><?= number_format( $escola, 0 , ",", "." ); ?></td>
	<td align="right"><?= number_format( $aluno, 0, ",", "." ); ?></td>
	<td align="right"><?= number_format( $professor, 0, ",", "." ); ?></td>
	<td align="right"><?= number_format( $laboratorio, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $laboratorio / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<!-- 
	<td align="right"><?= number_format( $bandalarga, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $bandalarga / $escola ) * 100, 0 ) : 0; ?>%)</font></td>	 
	<td align="right"><?= number_format( $_2008, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2009, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2009 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2010, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2010 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	-->	
	<td align="right"><?= number_format( $_2008_1, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_1 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_1_r, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_1_r / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_2, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_2 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_2_r, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_2_r / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_3, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_3 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_3_r, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_3_r / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_4, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_4 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2008_4_r, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2008_4_r / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	<td align="right"><?= number_format( $_2009_2011, 0, ",", "." ); ?> <font color='#0F55A9'>(<?= $escola > 0 ? number_format( ( $_2009_2011 / $escola ) * 100, 0 ) : 0; ?>%)</font></td>
	
</tr>
<?php if ( $agrupador == "municipio" ): ?>
	<tr style="padding-left:<?= $profundidade * 20 ?>px;display:none;" id="tr_<?= $muncod ?>"><td colspan="14" id="td_<?= $muncod ?>">... carregando</td></tr>
<?php endif; ?>
<?php
		exibir( $dados["sub_itens"], $profundidade + 1 );
	}
}

//$agrupador = arrprint_r($_REQUEST);

$agrupador = $_REQUEST['agrupador'];
//$agrupador = array( "pais", "regiao", "estado" );
//$agrupador = array( "operadora", "municipio" );
$dados = agrupar( $dados, $agrupador );

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
	
	var aberto = new Object();
	
	function detalharMunicipio( muncod, labinstalado, BLInstal, BLpolo  )
	{
		var td = document.getElementById( 'td_' + muncod );
		var tr = document.getElementById( 'tr_' + muncod );
		var img= document.getElementById( 'img_' + muncod );
		if ( tr.style.display != 'none' )
		{
			tr.style.display = 'none';
			img.src = '/imagens/mais.gif';
			return;
		}
		else
		{
			tr.style.display = !!document.all ? 'block' : 'table-row';
			img.src = '/imagens/menos.gif';
		}
		if ( aberto[muncod] )
		{
			return;
		}
		aberto[muncod] = true;
		var requisicao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
		var url = 'http://<?= $_SERVER["SERVER_NAME"] ?>/cte/bandalarga_relat_escolas.php?muncod=' + muncod + '&labinstalado=' + labinstalado + '&BLInstal=' +BLInstal + '&BLpolo=' + BLpolo;
		requisicao.open( 'GET', url, true );
		requisicao.onreadystatechange = function ouvirresposta()
		{
			if ( requisicao.readyState == 4 )
			{
				if ( requisicao.status == 200 && requisicao.responseText != '' )
				{
					td.innerHTML = requisicao.responseText;
				}
				if ( requisicao.dispose )
				{
					requisicao.dispose();
				}
				requisicao = null;
			}
		};
		requisicao.send( null );	
	}
	
</script>
<script type="text/javascript">
    function incluirEscola(entid)
    {
        return windowOpen( 'http://<?php echo $_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/cadastrarescola&acao=A&busca=entnumcpfcnpj&entid=' + entid, 'blank', 'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
</script>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr id="totais">		
		<td colspan="5" align="center" class="SubTituloCentro" id="total_p"> Total Previsto: <?= number_format( $total_p, 0, ",", "." ); ?> </td>
		<td colspan="9" align="center" class="SubTituloCentro" id="total_r"> Total Realizado: <?=number_format( $total_r, 0, ",", "." ); ?> </td>		
	</tr>
	<tr bgcolor="#e0e0e0">
		<td>&nbsp;</td>
		<td width="85" align="center" style="border-bottom: none; margin-bottom: 0; "></td>
		<td width="85" align="center"></td>
		<td width="85" align="center"></td>
		<td width="85" align="center"></td>
		<!-- <td width="85" align="center"><b>N. de Escolas Atendidadas c/ Banda Larga</b></td> -->
		<td width="85" align="center" colspan="2"><b>1º Trimestre 2008</b></td>
		<td width="85" align="center" colspan="2"><b>2º Trimestre 2008</b></td>
		<td width="85" align="center" colspan="2"><b>3º Trimestre 2008</b></td>
		<td width="85" align="center" colspan="2"><b>4º Trimestre 2008</b></td>
		<td width="85" align="center"></td>
	</tr>
	<tr bgcolor="#e0e0e0">
		<td>&nbsp;</td>
		<td width="85" align="center"><b>Qtd de Escolas</b></td>
		<td width="85" align="center"><b>N. de Alunos</b></td>
		<td width="85" align="center"><b>N. de Professores</b></td>
		<td width="85" align="center"><b>N. de Escolas c/ Laboratório</b></td>
		<!-- <td width="85" align="center"><b>N. de Escolas Atendidadas c/ Banda Larga</b></td> -->
		<td width="85" align="center" title="Previsão"><b>Prev.</b></td>
		<td width="85" align="center" title="Realizado"><b>Real.</b></td>
		<td width="85" align="center" title="Previsão"><b>Prev.</b></td>
		<td width="85" align="center" title="Realizado"><b>Real.</b></td>
		<td width="85" align="center" title="Previsão"><b>Prev.</b></td>
		<td width="85" align="center" title="Realizado"><b>Real.</b></td>
		<td width="85" align="center" title="Previsão"><b>Prev.</b></td>
		<td width="85" align="center" title="Realizado"><b>Real.</b></td>
		<td width="85" align="center"><b>Previsão<br> 2009 - 2011</b></td>
	</tr>
	<?php exibir( $dados ); ?>
</table>
