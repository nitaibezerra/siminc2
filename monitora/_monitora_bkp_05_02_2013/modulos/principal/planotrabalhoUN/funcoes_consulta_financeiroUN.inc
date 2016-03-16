<?php

function cfDesenhaResultado( $itens )
{
	global $cfAgrupadores;
	$soma = array(
		'dotacao_inicial' => 0,
		'autorizado_valor' => 0,
		'valorproposto' => 0,
		'empenhado' => 0,
		'liquidado' => 0,
		'pago' => 0
	);

	$itens = is_array( $itens ) && count( $itens ) ? $itens : array();
	if ( count( $itens ) < 1 )
	{
		print '<br/><br/><p style="color: #ff2020;">Nenhum resultado para os parâmetros indicados.</p>';
		return;
	}

	echo ' <table width="100%" align="center" border="0" cellpadding="2" cellspacing="0"> ';	
	foreach ( $itens as $item )
	{
		$rastro = array( $item['cod'] => $item['dsc'] );
		$rastroCodigo = array();
		$rastroCodigo[$cfAgrupadores[0]] = $item['cod'];
		$valores = cfDesenha( $item, 0, $rastro, $rastroCodigo );
		$soma['dotacao_inicial'] += $valores['dotacao_inicial'];
		$soma['autorizado_valor'] += $valores['autorizado_valor'];
		$soma['valorproposto'] += $valores['valorproposto'];
		$soma['empenhado'] += $valores['empenhado'];
		$soma['liquidado'] += $valores['liquidado'];
		$soma['pago'] += $valores['pago'];
	}
	
	cfDesenhaTotal( 0, 'Geral', '', $soma['dotacao_inicial'], $soma['autorizado_valor'], $soma['valorproposto'], $soma['empenhado'], $soma['liquidado'], $soma['pago'], true );
}

function cfDesenha( $dados, $nivel, $rastro, $rastroCodigo )
{
	global $cfAgrupadores;
	$soma = array(
		'dotacao_inicial' => 0,
		'autorizado_valor' => 0,
		'valorproposto' => 0,
		'empenhado' => 0,
		'liquidado' => 0,
		'pago' => 0
	);
	$rastro[$dados['cod']] = $dados['dsc'];
	if ( count( $dados ) == 3 )
	{
		
		cfDesenhaAgrupador( $dados['cod'], $dados['dsc'], $nivel, $rastro, $rastroCodigo );
		foreach ( $dados['itens'] as $item )
		{
			$subRastroCodigo = $rastroCodigo;
			$subRastroCodigo[$cfAgrupadores[$nivel+1]] = $item['cod'];
			$rastroItem = $rastro;
			$rastroItem[$item['cod']] = $item['dsc'];
			$valores = cfDesenha( $item, $nivel + 1, $rastroItem, $subRastroCodigo );
			$soma['dotacao_inicial'] += $valores['dotacao_inicial'];
			$soma['autorizado_valor'] += $valores['autorizado_valor'];
			$soma['valorproposto'] += $valores['valorproposto'];
			$soma['empenhado'] += $valores['empenhado'];
			$soma['liquidado'] += $valores['liquidado'];
			$soma['pago'] += $valores['pago'];
		}
		global $consulta;
		$titulo = $consulta->pegarTituloAgrupador( $nivel );
		cfDesenhaTotal( $nivel, $titulo, $dados['cod'], $soma['dotacao_inicial'], $soma['autorizado_valor'], $soma['valorproposto'], $soma['empenhado'], $soma['liquidado'], $soma['pago'], false );
	}
	else
	{
		cfDesenhaItem( $dados, $nivel, $rastro, $rastroCodigo );
		$soma['dotacao_inicial'] = $dados['dotacao_inicial'];
		$soma['autorizado_valor'] = $dados['autorizado_valor'];
		$soma['valorproposto'] = $dados['valorproposto'];
		$soma['empenhado'] = $dados['empenhado'];
		$soma['liquidado'] = $dados['liquidado'];
		$soma['pago'] = $dados['pago'];
	}
	return $soma;
}

function cfDesenhaTotal( $nivel, $titulo, $cod, $dotacaoInicial, $autorizado_valor, $valorproposto, $empenhado, $liquidado, $pago, $mostraTable = false )
{
	$autorizado = $dotacaoInicial + $creditoAdicional;
	$porcentagemAutorizado = $autorizado ? ( ( $pago * 100 ) / $autorizado ) : 0 ;
	$cod = $cod != '' ? ' ( ' . $cod . ' ) ' : '';
	if($mostraTable){
		echo '<table class="tabelaDados" width="100%" align="center" border="0" cellpadding="2" cellspacing="0">';
	}
	?>
		<tr>
			<td width="550px" class="alignLeft bold titulolinha" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<b>Total <?= $titulo . $cod ?><b>
			</td>
			<td width="150px" align="right"><b><? cfDesenhaValor( $dotacaoInicial ); ?></b></td>
			<td width="150px" align="right"><b><? cfDesenhaValor( $autorizado_valor ); ?></b></td>
			<td width="150px" align="right"><b><? cfDesenhaValor( $valorproposto ); ?></b></td>
			<td width="150px" align="right"><b><? cfDesenhaValor( $empenhado ); ?></b></td>
			<td width="150px" align="right"><b><? cfDesenhaValor( $liquidado ); ?></b></td>
			<td width="150px" align="right"><b><? cfDesenhaValor( $pago ); ?></b></td>
		</tr>
	<?php 
	if($mostraTable){
		echo '</table>';
	}
}

function cfDesenhaAgrupador( $codigo, $descricao, $nivel, $rastro, $rastroCodigo )
{
	$seta = $nivel > 0 ? '&rsaquo; ' : '' ;
	?>
		<tr>
			<td class="tituloagrup bold alignLeft" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<?= $seta ?>
				<? cfDesenhaCodigo( $nivel, $codigo, $rastroCodigo ); ?>
				<a href="<? cfMontarLinkGrafico( $rastro ); ?>" class="tituloagrup bold"><?= $descricao ?></a>
			</td>
		</tr>
	<?
}

function cfDesenhaCodigo( $nivel, $codigo, $rastroCodigo )
{
	global $cfAgrupadores;
	switch ( $cfAgrupadores[$nivel] )
	{
		case 'acacod':
			//dbg( $rastroCodigo );
			$unicod = trim( $rastroCodigo['uo'] );
			$acacod = trim( $rastroCodigo['acacod'] );
			$loccod = explode( '.', $rastroCodigo['localizador'] );
			$loccod = $loccod[3];
			$saida = sprintf(
				'<a href="#" onclick="window.open( \'%s\', \'Ação\', \'scrollbars=yes,top=50,left=200\' )">%s</a>',
				"http://simec-d/monitora/monitora.php?modulo=relatorio/acao/relatorio_evolucao&acao=C&acacod=$acacod&unicod=$unicod&loccod=$loccod",
				$codigo
			);
			break;
		case 'localizador':
			$a = explode( '.', $codigo );
			$acao = $a[1];
			$programa = $a[0];
			$saida = sprintf(
				'<a href="#" onclick="window.open( \'%s\', \'Localizador\', \'scrollbars=yes,top=50,left=200\' )">%s</a>',
				"http://simec-d/monitora/monitora.php?modulo=principal/acao/monitoraacao&acao=A&refcod=x&acaid=$acao&prgid=$programa",
				$codigo
			);
			break;
		default:
			$saida = $codigo;
			break;
	}
	//var_dump( $rastroCodigo );
	print $saida;
}

function cfDesenhaItem( $item, $nivel, $rastro, $rastroCodigo )
{
	static $cor = '';
	$cor = $cor == '' ? '#f8f8f8' : '';
	$seta = $nivel > 0 ? '&rsaquo; ' : '' ;
	?>
		<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
			<td width="550px" class="alignLeft titulolinha" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<?= $seta ?>
				<? cfDesenhaCodigo( $nivel, $item['cod'], $rastroCodigo ); ?>
				<a href="<? cfMontarLinkGrafico( $rastro ); ?>" class="alignLeft titulolinha"><?= $item['dsc'] ?></a>
			</td>			
			<td width="145px" align="right" style="color: rgb(0, 102, 204);"><? cfDesenhaValor( $item['dotacao_inicial'] ); ?></td>
			<td width="150px" align="right" style="color: rgb(0, 102, 204);"><? cfDesenhaValor( $item['autorizado_valor'] ); ?></td>
			<td width="150px" align="right" style="color: rgb(0, 102, 204);"><? cfDesenhaValor( $item['valorproposto'] ); ?></td>
			<td width="150px" align="right" style="color: rgb(0, 102, 204);"><? cfDesenhaValor( $item['empenhado'] ); ?></td>
			<td width="150px" align="right" style="color: rgb(0, 102, 204);"><? cfDesenhaValor( $item['liquidado'] ); ?></td>
			<td width="150px" align="right" style="color: rgb(0, 102, 204);"><? cfDesenhaValor( $item['pago'] ); ?></td>
			
		</tr>
	<?
}

function cfDesenhaValor( $valor )
{
	$valor = number_format( $valor, 0, ',', '.' );
	print $valor == '-0' ? '-' : $valor ;
}

function cfMontarParametroRastroGrafico( $rastro )
{
	$parametro = '';
	foreach ( $rastro as $cod => $dsc )
	{
		$parametro .= "&rastro[" . $cod . "]=" . urlencode( $dsc );
	}
	$parametro = substr( $parametro, 1 );
	return $parametro;
}

function cfMontarLinkGrafico( $rastro )
{
	print "javascript:mostrarGrafico( '" . cfMontarParametroRastroGrafico( $rastro ) . "' );";
}

// a função nativa do php zera as chaves numéricas
function cfArrayShift( &$array )
{
	reset( $array );
	unset( $array[key($array)] );
}

$cfRastroBusca = array();
function cfBuscarItem( $itens, $rastro )
{
	global $cfRastroBusca;
	// captura o primeiro elemento do array e seus dados
	reset( $rastro );
	$codAtual = key( $rastro );
	$dscAtual = current( $rastro );	
	foreach ( $itens as $item )
	{
		if ( $item['cod'] == $codAtual && $item['dsc'] == $dscAtual )
		{
			$cfRastroBusca[$item['cod']] = $item['dsc'];
			if ( count( $rastro ) > 1 )
			{
				cfArrayShift( $rastro );
				return cfBuscarItem( $item['itens'], $rastro );
			}
			return $item;
		}
	}
	return null;
}

function cfCalcularValorTotal( $itens )
{
	if ( array_key_exists( 'itens', $itens ) == false )
	{
		
		return array(
			'dotacao_inicial' => $itens['dotacao_inicial'],
			'credito_adicional' => $itens['autorizado_valor'],
			'autorizado_valor' => $itens['valorproposto'],
			'empenhado' => $itens['empenhado'],
			'liquidado' => $itens['liquidado'],
			'pago' => $itens['pago']
		);
	}
	$soma = array(
		'dotacao_inicial' => 0,
		'autorizado_valor' => 0,
		'valorproposto' => 0,
		'empenhado' => 0,
		'liquidado' => 0,
		'pago' => 0
	);
	foreach ( $itens['itens'] as $item )
	{
		$valores = cfCalcularValorTotal( $item );
		$soma['dotacao_inicial'] += $valores['dotacao_inicial'];
		$soma['autorizado_valor'] += $valores['autorizado_valor'];
		$soma['valorproposto'] += $valores['valorproposto'];
		$soma['empenhado'] += $valores['empenhado'];
		$soma['liquidado'] += $valores['liquidado'];
		$soma['pago'] += $valores['pago'];
	}
	return $soma;
}

function cfCalculaValorAgrupado( $itens )
{
	if ( array_key_exists( 'itens', $itens ) == false )
	{
		return array( $itens['cod'] => cfCalcularValorTotal( $itens ) );
	}
	$soma = array();
	foreach ( $itens['itens'] as $item )
	{
		$valores = cfCalcularValorTotal( $item );
		$valores['cod'] = $item['cod'];
		$valores['dsc'] = $item['dsc'];
		// o código não é utilizado como chave, pois há casos em que o cod se repete (exemplo: RAP)
		array_push( $soma, $valores );
	}
	return $soma;
}

$cfAgrupadores = array();

?>