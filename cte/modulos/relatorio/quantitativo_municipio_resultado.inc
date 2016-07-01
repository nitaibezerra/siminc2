<?php
/******** Para manter sessão ***********/
if ( isset( $_REQUEST['volta'] ) ){
	$_REQUEST = $_SESSION['filtro_quantitativo_municipio'];
}
else{
	$_SESSION['filtro_quantitativo_municipio'] = $_REQUEST;
	unset( $_SESSION['filtro_quantitativo_municipio']['buscar'] );
	unset( $_SESSION['filtro_quantitativo_municipio']['coarvore'] );
	unset( $_SESSION['filtro_quantitativo_municipio']['csarvore'] );
	unset( $_SESSION['filtro_quantitativo_municipio']['PHPSESSID'] );
}

function agruparPorCampo( $campo, $campoId, array $dados ){
	
	global $db;
	$resultado = array();
	foreach ( $dados as $item )
	{
		$chave = $item[$campo];
		if ( !array_key_exists( $chave, $resultado ) ){
			$resultado[$chave] = array(
				'id'    		=> $item[$campoId],
				'tec'   		=> 0,
				'fin'   		=> 0,
				'total' 		=> 0,
				'par'   		=> 0,
				'elabpard'   	=> 0,
				'elapar'   		=> 0,
				'cadasparfin'   => 0,
				'docfinal'   	=> 0,
				'enviofnde'   	=> 0
			);
		}
		$resultado[$chave]['total']++;
		if ( $item['tec'] == 't' )
		{
			$resultado[$chave]['tec']++;
		}
		if ( $item['fin'] == 't' )
		{
			$resultado[$chave]['fin']++;
		}
		if ( $item['par'] == 't' )
		{
			$resultado[$chave]['par']++;
		}
		if ( $item['elabpard'] == 't' )
		{
			$resultado[$chave]['elabpard']++;
		}
		if ( $item['elapar'] == 't' )
		{
			$resultado[$chave]['elapar']++;
		}
		if ( $item['cadasparfin'] == 't' )
		{
			$resultado[$chave]['cadasparfin']++;
		}
		if ( $item['docfinal'] == 't' )
		{
			$resultado[$chave]['docfinal']++;
		}
		if ( $item['enviofnde'] == 't' )
		{
			$resultado[$chave]['enviofnde']++;
		}
	}
	ksort( $resultado );
	return $resultado;
}

$condicao = array();

if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] )
{
	array_push( $condicao, " uf.estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
}

$condicao = count( $condicao ) ? " where " . implode( " and ", $condicao ) : "";

$sqlBase = "
	select
		mi.miccod,
		mi.micdsc,
		me.mescod,
		me.mesdsc,
		uf.estcod,
		uf.estdescricao,
		re.regcod,
		re.regdescricao,
		pa.paiid,
		pa.paidescricao,
		case when par.esdid = " . CTE_ESTADO_DIAGNOSTICO . "  then true else false end as elabPard,
		case when par.esdid = " . CTE_ESTADO_ANALISE . " then true else false end as tec,
		case when par.esdid = " . CTE_ESTADO_ANALISE_FIN . " then true else false end as fin,
		case when par.esdid = " . CTE_ESTADO_PAR . "  then true else false end as elaPar,
		case when par.esdid = " . CTE_ESTADO_PARECER . " then true else false end as cadasparfin,
		case when par.esdid = " . CTE_ESTADO_FINALIZADO . " then true else false end as docfinal,
		case when par.esdid = " . CTE_ESTADO_FNDE . " then true else false end as enviofnde,
		case when par.muncod is null then false else true end as par
		%s
	from territorios.municipio mu
		inner join territorios.microregiao mi on
			mi.miccod = mu.miccod
		inner join territorios.mesoregiao me on
			me.mescod = mu.mescod
		inner join territorios.estado uf on
			uf.estuf = mu.estuf
		inner join territorios.regiao re on
			re.regcod = uf.regcod
		inner join territorios.pais pa on
			pa.paiid = re.paiid
		%s
		left join
		(
			select
				dc.esdid,
				iu.muncod,
				iu.mun_estuf
			from cte.instrumentounidade iu
				left join cte.pontuacao po on
					po.inuid = iu.inuid
				left join cte.acaoindicador ai on
					ai.ptoid = po.ptoid
				left join cte.subacaoindicador si on
					si.aciid = ai.aciid
				left join workflow.documento dc on
					dc.docid = iu.docid
				left join workflow.estadodocumento ed on ed.esdid = dc.esdid

				
			where
				iu.itrid = " . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . " and
				po.ptostatus = 'A'
			group by
				dc.esdid,
				iu.muncod,
				iu.mun_estuf
				
		) par on
			par.muncod = mu.muncod
			and par.mun_estuf = mu.estuf
		" . $condicao . "
";
 

$agrupador = $_REQUEST['agrupador'];

switch ( $agrupador )
{
	case "microregiao":
		$descricaoAgrupador = "Microregião";
		$campoParaAgrupar = "micdsc";
		$campoParaAgruparId = "miccod";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "mesoregiao":
		$descricaoAgrupador = "Mesoregião";
		$campoParaAgrupar = "mesdsc";
		$campoParaAgruparId = "mescod";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "uf":
		$descricaoAgrupador = "Unidade da Federação";
		$campoParaAgrupar = "estdescricao";
		$campoParaAgruparId = "estcod";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "regiao":
		$descricaoAgrupador = "Região";
		$campoParaAgrupar = "regdescricao";
		$campoParaAgruparId = "regcod";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "pais":
		$descricaoAgrupador = "País";;
		$campoParaAgrupar = "paidescricao";
		$campoParaAgruparId = "paiid";
		$campoNovo = "";
		$joinNovo = "";
		break;
	
/*********** GRUPO TIPO MUNICIPIO ************/
	default:
		
		$gtmid = (integer) $agrupador;
		$sqlNomeGrupo = "
			select
				gtmdsc
			from territorios.grupotipomunicipio
			where
				gtmid = " . $gtmid . "
		";
		
		$descricaoAgrupador = (string) $db->pegaUm( $sqlNomeGrupo );
		$campoParaAgrupar = "tpmdsc";
		$campoParaAgruparId = "tpmid";
		$campoNovo = " , tm.tpmdsc, mm.tpmid ";
		$joinNovo = "
			inner join territorios.muntipomunicipio mm on
				mm.muncod = mu.muncod
			inner join territorios.tipomunicipio tm on
				tm.tpmid = mm.tpmid
				and mm.tpmid <> 8 
				and tm.gtmid = " . $gtmid . "
		";
		break;
}
$sql 	= sprintf( $sqlBase, $campoNovo, $joinNovo );
$dados 	= $db->carregar( $sql );
$dados 	= $dados ? $dados : array();
$dados 	= agruparPorCampo( $campoParaAgrupar, $campoParaAgruparId, $dados );

/*********** SQL para IDEB ***************/
if($_REQUEST['estuf'][0] != NULL){
	$condicaoIDEB = "mu.estuf in ( '".$_REQUEST['estuf'][0]."' ) and";	
}

$sqlMenorIDEB = "
						select 
							case when ed.esdid = 1  then true else false end as elabPard,
							case when ed.esdid = 10 then true else false end as tec,
							case when ed.esdid = 13 then true else false end as fin,
							case when ed.esdid = 2  then true else false end as elaPar,
							case when ed.esdid = 14 then true else false end as cadasparfin,
							case when ed.esdid = 11 then true else false end as docfinal,
							case when ed.esdid = 15 then true else false end as enviofnde,
							case when mu.muncod is null then false else true end as par,
							tm.tpmdsc, mm.tpmid
					 	from territorios.municipio mu
						inner join territorios.estado uf on
							uf.estuf = mu.estuf
						inner join territorios.muntipomunicipio mm on
							mm.muncod = mu.muncod
						inner join territorios.tipomunicipio tm on
							tm.tpmid = mm.tpmid
							and mm.tpmid = 8 
						left join cte.instrumentounidade iu on iu.mun_estuf = mu.estuf and iu.muncod = mu.muncod 
						left join workflow.documento dc on
							dc.docid = iu.docid
						left join workflow.estadodocumento ed on ed.esdid = dc.esdid
						where ".$condicaoIDEB."  tm.gtmid = " . $gtmid . "
					";
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
				<thead>
					<tr>
						<th colspan="11" style="font-size: 16pt;">
							<?= $descricaoAgrupador ?>
						</th>
					</tr>
					<tr style="background-color: #d9d9d9;">
						<th>&nbsp;</td>
						<th align="center" width="100">Em Elaboração do PAR <br>(Fase Diagnóstico)</td>
						<th align="center" width="100">Em Elaboração do PAR (PAR)</td>
						<th align="center" width="100">Assistência Técnica</td>
						<th align="center" width="100">Assistência Financeira</td>
						<th align="center" width="100">Cadastramento de Parecer <br> Físico / Financeiro</td>
						<th align="center" width="100">Aguardando envio <br> para o FNDE</td>
						<th align="center" width="100">Documento Finalizado</td>
						<th align="center" width="100">Qtd.<br> Municípios</td>
						<th align="center" width="100">Qtd.<br> com PAR</td>
						<th align="center" width="100">% com PAR</td>
						
					</tr>
				</thead>
				<tbody>
					<?php
					$total_elabpard 	= 0;
					$total_elapar 		= 0;
					$total_tec 			= 0;
					$total_fin 			= 0;
					$total_cadparfintec = 0;
					$total_enviofnde 	= 0;
					$total_docfinal 	= 0;
					$total_tot 			= 0;
					$total_par 			= 0;

					foreach ( $dados as $nomeItemGrupo => $valores ) : 
						$total_tec 			+= $valores['tec'];
						$total_fin 			+= $valores['fin'];
						$total_tot 			+= $valores['total'];
						$total_par 			+= $valores['par'];
						$total_elabpard 	+= $valores['elabpard'];
						$total_elapar 		+= $valores['elapar'];
						$total_cadparfintec += $valores['cadasparfin'];
						$total_enviofnde 	+= $valores['enviofnde'];
						$total_docfinal 	+= $valores['docfinal'];
						
						$cor = $cor == "" ? "#f0f0f0" : "";

						$url =  $_SERVER['HTTP_REFERER'] .
								"&" . http_build_query( $_SESSION['filtro_quantitativo_municipio'] ) .
								"&detalhamento=1" .
								"&campoId=" . $valores['id'];
						?>
						<tr bgcolor="<?= $cor ?>">
							<td>
								<a href="<?= $url ?>"><?= $nomeItemGrupo ?></a>
							</td>
								<td align="center">
								<?= $valores['elabpard'] ?>
							</td>
							<td align="center">
								<?= $valores['elapar'] ?>
							</td>
							<td align="center">
								<?= $valores['tec'] ?>
							</td>
							<td align="center">
								<?= $valores['fin'] ?>
							</td>
							<td align="center">
								<?= $valores['cadasparfin'] ?>
							</td>
							<td align="center">
								<?= $valores['enviofnde'] ?>
							</td>
							<td align="center">
								<?= $valores['docfinal'] ?>
							</td>
							<td align="center">
								<?= $valores['total'] ?>
							</td>
							<td align="center">
								<?= $valores['par'] ?>
							</td>
							<td align="center">
								<?= number_format( ( $valores['par'] / $valores['total'] ) * 100, 2, ",", "." ) ?>
								%
							</td>
							
						</tr>
				<?php 
				if($nomeItemGrupo == "Priorizados"){

					$dadosMenIDEB = $db->carregar( $sqlMenorIDEB );
					$dadosMenIDEB = $dadosMenIDEB ? $dadosMenIDEB : array();
					$dadosMenIDEB = agruparPorCampo( $campoParaAgrupar, $campoParaAgruparId, $dadosMenIDEB );
					if ( count( $dadosMenIDEB ) ) { 
						foreach ( $dadosMenIDEB as $nomeItemGrupo => $valoresIDEB ) {
							$url = $_SERVER['HTTP_REFERER'] .
								   "&" . http_build_query( $_SESSION['filtro_quantitativo_municipio'] ) .
								   "&detalhamento=1" .
								   "&campoId=" . $valoresIDEB['id'];
							$nomeItemGrupo = '<img style="border:none;" src="../includes/dtree/img/joinbottom.gif">1.016 com menor IDEB';
					?>
					<tr>
					<td style="background-color:#66CCFF">
								<a href="<?= $url ?>"><?= $nomeItemGrupo ?></a>
							</td>
								<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['elabpard'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['elapar'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['tec'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['fin'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['cadasparfin'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['enviofnde'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['docfinal'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['total'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= $valoresIDEB['par'] ?>
							</td>
							<td style="background-color:#66CCFF" align="center">
								<?= number_format( ( $valoresIDEB['par'] / $valoresIDEB['total'] ) * 100, 2, ",", "." ) ?>
								%
							</td>					

					</tr>	
					<?php 
								}
							}
						}
					endforeach; ?>
						<tr style="background-color: #d9d9d9;">
						<td>
							<b>TOTAL</b>
						</td>
						
						<td align="center">
							<?= $total_elabpard ?>
						</td>
						<td align="center">
							<?= $total_elapar ?>
						</td>
						<td align="center">
							<?= $total_tec ?>
						</td>
						<td align="center">
							<?= $total_fin ?>
						</td>
						<td align="center">
							<?=$total_cadparfintec ?>	
						</td>
						<td align="center">
							<?=$total_enviofnde?>
						</td>
						<td align="center">
							<?= $total_docfinal ?>
						</td>
						<td align="center">
							<?= $total_tot ?>
						</td>
						<td align="center">
							<?= $total_par ?>
						</td>
						<td align="center">
						<?= number_format( ( $total_par / $total_tot ) * 100, 2, ",", "." ) ?>
							%
						</td>
					</tr>
				</tbody>
			</table>

		<?php else : ?>
			<p align="center" style="color: #903030;">
				<br/><br/>
				Nenhum resultado encontrado para o filtro preenchido.
			</p>
		<?php endif; 
		if($agrupador == "2"){
		?>	
<table width="100%" style=" padding: 5px 5px 5px 5px;" border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td width="50%" align="right">&nbsp;</td>
    <td width="23%" align="right">Legenda:</td>
    <td width="27%" align="right" style="background-color:#66CCFF">* Municípios Contidos nos Priorizados. Não faz parte do somatório.</td>
  </tr>
</table>
<? } ?> 
	</body>
</html>