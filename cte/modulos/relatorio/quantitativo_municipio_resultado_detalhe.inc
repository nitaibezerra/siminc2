<?php

$filtro = $_SESSION['filtro_quantitativo_municipio'];
$filtro['campoId'] = $_REQUEST['campoId'];


function agruparPorCampo( $campo, $campoId, array $dados )
{
	global $db;
	$com_par = array();
	$sem_par = array();
	foreach ( $dados as $item )
	{
		$chave = $item['estuf'] . " " . $item['mundescricao'];
		$novo = array(
			'ibge'      => $item['muncod'],
			'municipio' => $item['mundescricao'],
			'uf'        => $item['estuf']
		);
		if ( $item['par'] == 't' )
		{
			$com_par[$chave] = $novo;
		}
		else
		{
			$sem_par[$chave] = $novo;
		}
	}
	ksort( $com_par );
	ksort( $sem_par );
	return array(
		'com_par' => $com_par,
		'sem_par' => $sem_par
	);
}

$sqlBase = "
	select
		mu.muncod,
		mu.mundescricao,
		uf.estuf,
		case when par.muncod is null
			then false else true end as par
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
				iu.muncod
			from cte.instrumentounidade iu
				inner join cte.pontuacao po on
					po.inuid = iu.inuid
				inner join cte.acaoindicador ai on
					ai.ptoid = po.ptoid
				inner join cte.subacaoindicador si on
					si.aciid = ai.aciid
			where
				iu.itrid = " . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . " and
				po.ptostatus = 'A'
			group by
				iu.muncod
		) par on
			par.muncod = mu.muncod
	%s
";

$agrupador = $filtro['agrupador'];
switch ( $agrupador )
{
	case "microregiao":
		$descricaoAgrupador = "Microregião";
		$descricaoAgrupador = "Microregião";
		$campoParaAgrupar = "micdsc";
		$campoParaAgruparId = "miccod";
		$campoPrefixoId = "mi";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "mesoregiao":
		$descricaoAgrupador = "Mesoregião";
		$campoParaAgrupar = "mesdsc";
		$campoParaAgruparId = "mescod";
		$campoPrefixoId = "me";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "uf":
		$descricaoAgrupador = "Unidade da Federação";
		$campoParaAgrupar = "estdescricao";
		$campoParaAgruparId = "estcod";
		$campoPrefixoId = "uf";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "regiao":
		$descricaoAgrupador = "Região";
		$campoParaAgrupar = "regdescricao";
		$campoParaAgruparId = "regcod";
		$campoPrefixoId = "re";
		$campoNovo = "";
		$joinNovo = "";
		break;
	case "pais":
		$descricaoAgrupador = "País";
		$campoParaAgrupar = "paidescricao";
		$campoParaAgruparId = "paiid";
		$campoPrefixoId = "pa";
		$campoNovo = "";
		$joinNovo = "";
		break;
	
	// GRUPO TIPO MUNICIPIO
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
		$campoPrefixoId = "mm";
		$campoNovo = " , tpmdsc ";
		$joinNovo = "
			inner join territorios.muntipomunicipio mm on
				mm.muncod = mu.muncod
			inner join territorios.tipomunicipio tm on
				tm.tpmid = mm.tpmid and
				tm.gtmid = " . $gtmid . "
		";
		break;
}


$condicao = array();

// uf
if ( count( $filtro['estuf'] ) && $filtro['estuf'][0] )
{
	array_push( $condicao, " uf.estuf in ( '" . implode( "','", $filtro['estuf'] ) . "' ) " );
}

array_push( $condicao, $campoPrefixoId . "." . $campoParaAgruparId . " =  '" . $filtro['campoId'] . "' " );

$condicao = count( $condicao ) ? " where " . implode( " and ", $condicao ) : "";

$sql = sprintf( $sqlBase, $campoNovo, $joinNovo, $condicao );

$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();
$dados = agruparPorCampo( $campoParaAgrupar, $campoParaAgruparId, $dados );

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
		
		<p align="center">
			<a href="<?= $_SERVER['HTTP_REFERER'] ?>&buscar=1&volta=1" style="color: #303030; text-decoration: none;">
				voltar
			</a>
		</p>
		
		<?php if ( count( $dados['com_par'] ) ) : ?>
			<table class="tabela" style="width:100%; background-color: #fefefe;" id= "relatorioPar" border="0" cellpadding="2" cellspacing="1" align="center">
				<thead>
					<tr>
						<th colspan="4" style="font-size: 16pt;">
							Com PAR
						</th>
					</tr>
					<tr style="background-color: #d9d9d9;">
						<th align="center" width="100">IBGE</td>
						<th>Município</td>
						<th align="center" width="100">UF</td>
					</tr>
				</thead>
				<tbody>
					<?php foreach ( $dados['com_par'] as $item ) : ?>
						<?php $cor = $cor == "" ? "#f0f0f0" : ""; ?>
						<tr bgcolor="<?= $cor ?>">
							<td align="center">
								<?= $item['ibge'] ?>
							</td>
							<td>
								<?= $item['municipio'] ?>
							</td>
							<td align="center">
								<?= $item['uf'] ?>
							</td>
						</tr>
					<?php endforeach; ?>
				</tbody>
			</table>
		<?php else : ?>
			<p align="center" style="color: #903030;">
				Nenhum município com PAR.
			</p>
		<?php endif; ?>
		
		<p align="center">
			<a href="<?= $_SERVER['HTTP_REFERER'] ?>&buscar=1&volta=1" style="color: #303030; text-decoration: none;">
				voltar
			</a>
		</p>
		
		<?php if ( count( $dados['sem_par'] ) ) : ?>
			<table class="tabela" style="width:100%; background-color: #fefefe;" id= "relatorioPar" border="0" cellpadding="2" cellspacing="1" align="center">
				<thead>
					<tr>
						<th colspan="4" style="font-size: 16pt;">
							Sem PAR
						</th>
					</tr>
					<tr style="background-color: #d9d9d9;">
						<th align="center" width="100">IBGE</td>
						<th>Município</td>
						<th align="center" width="100">UF</td>
					</tr>
				</thead>
				<tbody>
					<?php foreach ( $dados['sem_par'] as $item ) : ?>
						<?php $cor = $cor == "" ? "#f0f0f0" : ""; ?>
						<tr bgcolor="<?= $cor ?>">
							<td align="center">
								<?= $item['ibge'] ?>
							</td>
							<td>
								<?= $item['municipio'] ?>
							</td>
							<td align="center">
								<?= $item['uf'] ?>
							</td>
						</tr>
					<?php endforeach; ?>
				</tbody>
			</table>
		<?php else : ?>
			<p align="center" style="color: #903030;">
				Nenhum município sem PAR.
			</p>
		<?php endif; ?>
		
		<p align="center">
			<a href="<?= $_SERVER['HTTP_REFERER'] ?>&buscar=1&volta=1" style="color: #303030; text-decoration: none;">
				voltar
			</a>
		</p>
		
	</body>
</html>