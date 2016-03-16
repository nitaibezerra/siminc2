<?php
header("Content-Type: text/html; charset=ISO-8859-1",true); 
ini_set( "memory_limit", "512M" ); // ...
set_time_limit("90");

function agrupar( $lista, $agrupadores ) {
	global $db;
	
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false ) {
		return array();
	}
	$campo = array_shift( $agrupadores );
	$novo = array();
	foreach ( $lista as $item )	{
		$chave = $item[$campo];
		
		if($chave) {
		
		if ( array_key_exists( $chave, $novo ) == false ){			
			$novo[$chave] = array(
				"tot_escola"  => 0,
				"tot_curso"   => 0,
				"entid"		  => 0,
				"muncod"	  => 0,
				"agrupador"   => $campo,
				"sub_itens"   => array()
			);
		}	
		
		if ($item[escola] !== $itemEscola || $campo == 'curso')
			$novo[$chave]["tot_escola"]++;
		
		$itemEscola = $item[escola];
		if($item['curso']) {
			$novo[$chave]['tot_curso']++;
		}
		$novo[$chave][entid]  	 = $item[entid];	
		$novo[$chave][muncod]  	 = $item[muncod];	
		
		if ( $existeProximo ) {	
			array_push( $novo[$chave]["sub_itens"], $item );
		}
		
		}
	}
	if ( $existeProximo ) {
		foreach ( $novo as $chave => $dados )
		{
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );
		}
	}
	
	return $novo;
}

function exibirarvore($dadosagrupados, $nivel = 0, $nivelid = 'nivel') {
	global $agrup;
			
	if($nivel) {
		$html .= "<table width='100%' cellpadding='0' cellspacing='0' border='0'>";
		for($i = 0;$i < $nivel; $i++) {
			$espacamento .= '&nbsp;&nbsp;&nbsp;&nbsp;';
		}
	} else {
		$html .= "<table width='98%' cellpadding='0' cellspacing='0' border='0'>";
		$html .="<tr style=\"background-color: #d9d9d9;\">					
					<td class=\"SubTituloEsquerda\" align=\"center\">&nbsp;</td>
					<td class=\"SubTituloEsquerda\" width='100' align=\"center\">Qtd. Escolas</td>
					<td class=\"SubTituloEsquerda\" width='100' align=\"center\">Qtd. Cursos</td>
				</tr>";
	}
	$i = 0;
	
	//$html .= "<table width='750' cellpadding='0' cellspacing='0' border='0'>";
	
	$totalEscola = 0;
	$totalCurso  = 0;

	foreach($dadosagrupados as $key => $dados) {
		if($key) {
			unset($icomais);
			$pref = $nivelid.'_'.$i;
			if(count($dados['sub_itens']) > 0) {
				$icomais = "<img src='/imagens/mais.gif' border='0' title='abrir' name='btnArvore' id='btn_". $pref ."' onclick='abrearvore(\"". $pref ."\");'>";
			}
			$formato = (($i%2)?"bgcolor=\"#F7F7F7\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#F7F7F7';\"":"bgcolor=\"\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='';\"");
			
			$totalEscola += $dados['tot_escola'];
			$totalCurso += $dados['tot_curso'];
			
			$htmlarray[$dados['tot_curso'].'.'.$i] = "<tr ". $formato ." >
					  								  <td>". $espacamento . $icomais ." ". $key ."</td>
					  								  <td width='100'>". $dados['tot_escola'] ."</td>
					  								  <td width='100'>". $dados['tot_curso'] ."</td>
					  								  </tr>";
						
/*			$html .= "<tr ". $formato ." >
					  <td>". $espacamento . $icomais ." ". $key ."</td>
					  <td width='100'>". $dados['tot_escola'] ."</td>
					  <td width='100'>". $dados['tot_curso'] ."</td>
					  </tr>";*/
			if(count($dados['sub_itens']) > 0) {
				$htmlarray[$dados['tot_curso'].'.'.$i] .= "<tr id='". $pref ."' style='display:none'><td colspan='3'>". exibirarvore ($dados['sub_itens'], ($nivel+1), $pref) ."</td></tr>";
			}
			$i++;
		}
		
	}
	krsort($htmlarray);
	if( !$nivel ){
		$html .= "<tr style='background: #eee;'>
						<td style='font-weight: bold; text-align: right; padding-right: 50px;'>Total: </td>
						<td style='font-weight: bold;'>". $totalEscola ."</td>
						<td style='font-weight: bold;'>". $totalCurso  ."</td>
				  </tr>";
	}
	$html .= implode("", $htmlarray);
	if( !$nivel ){
		$html .= "<tr style='background: #eee;'>
						<td style='font-weight: bold; text-align: right; padding-right: 50px;'>Total: </td>
						<td style='font-weight: bold;'>". $totalEscola ."</td>
						<td style='font-weight: bold;'>". $totalCurso  ."</td>
				  </tr>";
	}
	$html .= "</table>";
	return $html;
}

$condicaoExterno = array();
$condicaoInterno = array();
$where			 = array();	
$boSubAcao = false;

// Ano de Referência
if ( $_REQUEST["anoReferencia"] ) {
	$boSubAcao = true;
	array_push($where, " sptano = {$_REQUEST["anoReferencia"]} " );
}
// Status da Subação
if (count($_REQUEST["ssuid"]) && $_REQUEST["ssuid"][0]) {
	$boSubAcao = true;
	array_push($where, " spt.ssuid in ( '" . implode( "','", $_REQUEST[ssuid] ) . "' ) " );
}
// Forma de Execução
if (count($_REQUEST["frmid"]) && $_REQUEST["frmid"][0]) {
	$boSubAcao = true;
	array_push($where, " sai.frmid in ( '" . implode( "','", $_REQUEST[frmid] ) . "' ) " );
}
// Pais
if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ) {
	array_push( $where, " regiao.regcod in  ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
}
// regiao
if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ) {
	array_push( $where, " regiao.regcod in  ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
}
// uf
if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ) {
	array_push( $where, " estado.estuf in  ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
	array_push( $where, " iu.inuid in ( 
											select iud.inuid 
											from cte.instrumentounidade iud 
											where iud.estuf in ( '". implode( "','", $_REQUEST['estuf'] ) ."' ) 
											and iud.itrid = 3 
										   )" );
}
// municipio
if ( count( $_REQUEST[municipios] ) && $_REQUEST[municipios][0] ) {
	array_push( $where, " mu.muncod in  ( '" . implode( "','", $_REQUEST[municipios] ) . "' ) " );
}
// localizacao
if ( count( $_REQUEST['tplid'] ) && $_REQUEST['tplid'][0] ) {
	array_push( $where, " e.tplid in  ( '" . implode( "','", $_REQUEST['tplid'] ) . "' ) " );
}

// mesoregião
if (count($_REQUEST[mescod]) && $_REQUEST[mescod][0]) {
	array_push($where, " mes.mescod in ( '" . implode( "','", $_REQUEST[mescod] ) . "' ) " );
}
// escola
if (count($_REQUEST[escolas]) && $_REQUEST[escolas][0]) {
	array_push($where, " e.entid in ( '" . implode( "','", $_REQUEST[escolas] ) . "' ) " );
}
// curso
if (count($_REQUEST[cursos]) && $_REQUEST[cursos][0]) {
	array_push($where, " ct.crsid in ( '" . implode( "','", $_REQUEST[cursos] ) . "' ) " );
}

// Filtro verifica se a escola possui ept


switch($_REQUEST['filtroept']) {
	case 'sim':
		array_push($where, " edd.entdoferta_ept = true " );
		
		switch( $_REQUEST['statusOferta'] ) {
			case 'F': array_push($where, " edd.entdstatusept = 'F' " ); break;
			case 'P': array_push($where, " edd.entdstatusept = 'P' " ); break;
		}
		break;
	case 'nao':
		array_push($where, " edd.entdoferta_ept = false " );
		break;
}

switch($_REQUEST['filtroppp']) {
	case 'sim':
		// Número de passos para preencher o PPP (completo)		
		$sql2 = "SELECT COUNT(ippid) AS num FROM cte.itensppp WHERE ippconteudo = '1'";
		$nppp = current($db->carregar( $sql2 ));
		$having = "HAVING (COUNT(cpp.cppid) = ".$nppp['num']." OR COUNT(cpp.cppid) = ".($nppp['num']-1).")";
		break;
	case 'nao':
		// Número de passos para preencher o PPP (completo)		
		$sql2 = "SELECT COUNT(ippid) AS num FROM cte.itensppp WHERE ippconteudo = '1'";
		$nppp = current($db->carregar( $sql2 ));
		$having = "HAVING (COUNT(cpp.cppid) < ".($nppp['num']-1)." )";
		break;
}

$compInner = "";
if( $boSubAcao ){
	$compInner = "
   		INNER JOIN cte.pontuacao pn ON pn.inuid = iue.inuid
		INNER JOIN cte.acaoindicador ai ON ai.ptoid = pn.ptoid
		INNER JOIN cte.subacaoindicador sai ON sai.aciid = ai.aciid
		INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sai.sbaid
		INNER JOIN cte.qtdfisicoano qfa ON qfa.sbaid = sai.sbaid and qfa.entid = iue.entid and qfaano = {$_REQUEST["anoReferencia"]}	
	";
}

$sql = "SELECT COUNT(ippid) AS itensppp, 
			   pais.paidescricao AS pais, 
			   regiao.regdescricao AS regiao, 
			   estado.estuf || ' - ' || estado.estdescricao AS estado, 
			   mesdsc AS mesorregiao, 
       		   iue.entid,
       		   e.entnome as escola,
       		   mu.muncod,
       		   mu.mundescricao AS municipio, 
			   ct.crstitulo AS curso,
			   edd.entddataprevisaoept
   		FROM cte.instrumentounidadeescola iue
   		INNER JOIN cte.instrumentounidade iu ON iu.inuid = iue.inuid
		$compInner  		
        INNER JOIN entidade.entidade e ON e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
        INNER JOIN entidade.endereco ende ON ende.entid = e.entid
        INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod
		INNER JOIN territorios.estado ON estado.estuf = mu.estuf 
		INNER JOIN territorios.regiao ON regiao.regcod = estado.regcod
		INNER JOIN territorios.pais ON pais.paiid = regiao.paiid
		INNER JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
        LEFT JOIN entidade.entidadedetalhe edd ON
           e.entid = edd.entid
           AND
           (
           entdreg_infantil_preescola = '1' OR
           entdreg_fund_8_anos        = '1' OR
           entdreg_fund_9_anos        = '1' OR
           entdreg_medio_medio        = '1' OR
           entdreg_medio_medio        = '1' Or
           entdreg_medio_integrado    = '1' OR
           entdreg_medio_normal       = '1' OR
           entdreg_medio_prof         = '1'
           )
        LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid 
		LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid AND cpp.cppid IS NOT NULL 
		LEFT JOIN cte.cursotecnico ct ON cont.crsid = ct.crsid 
   		WHERE
		e.tpcid = 1 AND fe.funid = 3 AND iu.itrid = 3
		". (count($where)?" AND ".implode(' AND ', $where):'') ."
		GROUP BY
       		iue.entid,
       		e.entcodent,
       		escola,
       		mu.muncod,
       		municipio,
			pais,
			regiao,
			estado,
			mesorregiao,
			curso,
			edd.entddataprevisaoept

		". $having ."
   		ORDER BY
       		estado ASC";

$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();
$dadosagrupados = agrupar($dados, $_REQUEST['agrupador']);

$agrupadorr = $_REQUEST['agrupador'];

$arEscolas = array();
foreach( $dados as $arDados ){
	$arEscolas[] = $arDados["entid"];
}
$arEscolas = array_unique( $arEscolas );
$_SESSION["arEscolas"] = $arEscolas; 

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

function fecharnosinternos(idno) {
	if(document.getElementById('link_'+idno+'_0')) {
		alert(document.getElementById('link_'+idno+'_0'));
		document.getElementById('link_'+idno+'_0').click();
	}
}


function toggleResultado( acaoTodos ){

	var arBotoes = document.getElementsByName( 'btnArvore' );
	
	for( i=0; i<arBotoes.length; i++ ){
		var id = arBotoes[i].id;
		pref = id.substr( id.indexOf( "_" ) +1 );
		
		var img = document.getElementById("btn_"+pref);
		if( acaoTodos == 'abrir') {
			img.title = 'fechar';
	    	img.src = '/imagens/menos.gif';
	    	var acao = '';
	    } else {
	    	img.title = 'abrir';
	    	img.src = '/imagens/mais.gif';
	    	var acao = 'none';
	    }
		
		document.getElementById(pref).style.display = acao;
	}
	

}

function abrearvore(pref, total) {
	var img = document.getElementById("btn_"+pref);
	if(img.title == 'abrir') {
		img.title = 'fechar';
    	img.src = '/imagens/menos.gif';
    	var acao = '';
    } else {
    	img.title = 'abrir';
    	img.src = '/imagens/mais.gif';
    	var acao = 'none';
    }
   	document.getElementById(pref).style.display = acao;
}
</script>
<html>
	<head>
		<title>Relatório Geral PPP</title>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style type="text/css">
			body{ margin: 0; padding: 0; }
		</style>
		<script type="text/javascript">
			top.window.focus();
			
			function exibirEnderecosEscolas(){
				var janela = window.open( 'brasilpro.php?modulo=relatorio/exibirEnderecosEscolas&acao=A', 'exibirEnderecosEscolas', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}
		</script>
	</head>
	<body>
	
	   <?php if ( count( $dados ) ) : ?>
			<table class="tabela" style="width:99%; background-color: #fefefe;" id= "relatorioPar" border="0" cellpadding="2" cellspacing="1" align="center">
				<tr style="background-color: #d9d9d9;">					
					<td colspan="3" class="TituloTela" align="center">
					 Relatório PPP
					</TD>
				</TR>
				<tr style="background-color: #d9d9d9;">					
					<td colspan="3" class="SubTituloEsquerda" align="center">
					 Filtros
					</TD>
				</TR>
				<TR>
					<TD class="SubTituloDireita" valign="top" width="30%">
						Agrupadores
					</TD>
					<TD>
						<?php 
						$agrup = array("pais"=>"Pais", "regiao"=>'Região', "estado"=>"Estado", "mesorregiao"=>"Mesorregião", "municipio"=>"Municipio", "escola"=>"Escola", "curso"=>"Curso");
						foreach ($agrupadorr as $valAgrup)
							echo $agrup[$valAgrup]."<br>";
						?>
					</TD>	
				</TR>
				<?
				# Mostra filtro Região
				if($_REQUEST[regcod]):
				if ( current($_REQUEST[regcod]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Regiões
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									regdescricao as descricao
								FROM
									territorios.regiao
								WHERE
									regcod in  ( '" . implode( "','", $_REQUEST[regcod] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif; 
				endif;
				
				# Mostra filtro Unidades da federação
				if($_REQUEST[estuf]):
				if ( current($_REQUEST[estuf]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Unidades da Federação
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									estdescricao as descricao
								FROM
									territorios.estado
								WHERE
									estuf in  ( '" . implode( "','", $_REQUEST[estuf] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif; 
				endif;

				# Mostra filtro Mesorregião
				if($_REQUEST[mescod]):
				if ( current($_REQUEST[mescod]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Mesorregião
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									mesdsc as descricao
								FROM
									territorios.mesoregiao
								WHERE
									mescod in  ( '" . implode( "','", $_REQUEST[mescod] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif; 
				endif;

				# Mostra filtro Município
				if($_REQUEST[municipios]):
				if ( current($_REQUEST[municipios]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Município
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									mundescricao as descricao
								FROM
									territorios.municipio
								WHERE
									muncod in  ( '" . implode( "','", $_REQUEST[municipios] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif;
				endif;
				
				# Mostra filtro Localização
				if($_REQUEST[tplid]):
				if ( current($_REQUEST[tplid]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Localização
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									tpldesc as descricao
								FROM
									entidade.tipolocalizacao
								WHERE
									tplid in  ( '" . implode( "','", $_REQUEST[tplid] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif;
				endif;

				# Mostra filtro Escola
				if($_REQUEST[escolas]):
				if ( current($_REQUEST[escolas]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Escolas
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									entnome as descricao
								FROM
									entidade.entidade
								WHERE
									entid in  ( '" . implode( "','", $_REQUEST[escolas] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif;
				endif;  

				# Mostra filtro Cursos
				if($_REQUEST[cursos]):
				if ( current($_REQUEST[cursos]) ):
				?>
				<TR>
					<TD class="SubTituloDireita" valign="top">
						Escolas
					</TD>
					<TD>
						<?php 
						$sql = "SELECT
									crstitulo as descricao
								FROM
									cte.cursotecnico
								WHERE
									crsid in  ( '" . implode( "','", $_REQUEST[cursos] ) . "' )";
						$pesq = $db->carregar($sql);
						foreach ($pesq as $reg)
							echo $reg[descricao].'<br>';
						unset($sql, $pesq, $reg);						
						?>
					</TD>	
				</TR>
				<?
				endif;
				endif;
				
				# Mostra filtro Ano Referência
				if( $_REQUEST[anoReferencia] ):
					?>
					<tr>
						<td class="SubTituloDireita" valign="top">
							Ano de Referência
						</td>
						<td><?= $_REQUEST[anoReferencia] ?></td>	
					</tr>
					<?
				endif;  				

				# Mostra filtro Status da Subação
				if( $_REQUEST[ssuid] ):
					if ( current($_REQUEST[ssuid]) ):
					?>
					<tr>
						<td class="SubTituloDireita" valign="top">
							Status da Subação
						</td>
						<td>
							<?php 
							$sql = "select ssudescricao as descricao 
									from cte.statussubacao
									where ssuid in  ( '" . implode( "','", $_REQUEST[ssuid] ) . "' )
									order by ssudescricao";
							$pesq = $db->carregar($sql);
							foreach ($pesq as $reg)
								echo $reg[descricao].'<br>';
							unset($sql, $pesq, $reg);						
							?>
						</td>	
					</tr>
					<?
					endif;
				endif;
								
				# Mostra filtro Forma de Execução
				if( $_REQUEST[frmid] ):
					if ( current($_REQUEST[frmid]) ):
					?>
					<tr>
						<td class="SubTituloDireita" valign="top">
							Forma de Execução
						</td>
						<td>
							<?php 
							$sql = "select 
										frmid as codigo,
										frmdsc as descricao
									from cte.formaexecucao
									where frmbrasilpro = true
									and frmid in  ( '" . implode( "','", $_REQUEST[frmid] ) . "' )
									and frmtipo = 'E'";
							$pesq = $db->carregar($sql);
							foreach ($pesq as $reg)
								echo $reg[descricao].'<br>';
							unset($sql, $pesq, $reg);						
							?>
						</td>	
					</tr>
					<?
					endif;
				endif;				
				
				?>
			</table>
			<table style="width:99%; background-color: #fefefe;" id= "relatorioPar" border="0" cellpadding="2" cellspacing="1" align="center">
			<tr>
				<td colspan="3">
					<p>
					<a href="javascript: toggleResultado( 'abrir' );">Abrir Todos</a>
					&nbsp;|&nbsp; <a href="javascript: toggleResultado( 'fechar' );">Fechar Todos</a>
					</p>
				</td>
			</tr>
			<tr>
			<td colspan="3">
			
			<?php 
				if($dadosagrupados) {
					echo exibirarvore($dadosagrupados, $html);	
				}
			?>
			</td>
			</tr>
			<tr>
				<td colspan="3">
					<p>
					<a href="javascript: toggleResultado( 'abrir' );">Abrir Todos</a>
					&nbsp;|&nbsp; <a href="javascript: toggleResultado( 'fechar' );">Fechar Todos</a>
					</p>
				</td>
			</tr>			
			</table>	
			<div style="margin: 20px; text-align: right;">
				<input type="button" onclick="javascript: exibirEnderecosEscolas();" value="Endereço das Escolas" />
			</div>
		<?php else : ?>
			<p align="center" style="color: #903030;">
				<br/><br/>
				Nenhum resultado encontrado para o filtro preenchido.
			</p>
		<?php endif; ?>
</body>
</html>
