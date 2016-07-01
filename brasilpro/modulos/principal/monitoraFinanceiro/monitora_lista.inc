<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past


include_once APPRAIZ . 'includes/workflow.php';

if ( $_REQUEST['estuf'] && $_REQUEST['evento'] == 'selecionar' ) {
	if( cte_selecionarUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, WF_TIPO_CTE, $_REQUEST['estuf'] ) ) {
		$db->commit();
		header( "Location: ?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A" );
		exit();
	}
} elseif ( $_REQUEST['muncod'] && $_REQUEST['evento'] == 'selecionar' ) {
	if( cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_CTE, $_REQUEST['muncod'] ) ) {
		$db->commit();
		header( "Location: ?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A" );
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

if ( $_REQUEST["acao"] == "M" ){ 
?>
<script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}
</script>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tbody>
			<tr>
				<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
					<form action="" method="post" name="formulario">
						<input type="hidden" name="modulo" value="<?= $_REQUEST['modulo'] ?>"/>
						<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
						<div style="float: left;">
							
							<table border="0" cellpadding="3" cellspacing="0">
								<tr>
									<td valign="bottom">
										Município
										<br/>
										<?php $filtro = simec_htmlentities( $_REQUEST['filtro'] ); ?>
										<?= campo_texto( 'filtro', 'N', 'S', '', 50, 200, '', '' ); ?>
									</td>
									<td valign="bottom">
										Estado
										<br/>
										<?php
										$estuf = $_REQUEST['estuf'];
										$unidades = "'" . implode( "','", cte_pegarUfsPermitidas() ) . "'";
										$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e where e.estuf in ({$unidades}) order by e.estdescricao asc";
										$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
										?>
									</td>
									<td valign="bottom"> 
										<?php 
										$par = $_REQUEST['par'] ? $_REQUEST['par'] : "com";
										?>
										<label><input type="radio" name="par" value="com" <?= $par == "com" ? 'checked="checked"' : '' ?>/>Com PAR</label>
										<label><input type="radio" name="par" value="sem" <?= $par == "sem" ? 'checked="checked"' : '' ?>/>Sem PAR</label>
										&nbsp;&nbsp;|&nbsp;&nbsp;
										<label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['capitais'] == 1 ? 'checked="checked"' : '' ?>/>Capitais</label>
										<label><input type="checkbox" name="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/>Grandes Cidades</label>

										<label><input type="checkbox" name="indigena" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['indigena'] == 1 ? 'checked="checked"' : '' ?>/>Comunidade Indígena</label>
										<label><input type="checkbox" name="quilombola" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/>Comunidade Quilombola</label>
									</td>
								</tr>
								<tr>
									<td colspan="2">
										<?php
										$analise = $_REQUEST["analise"];
										?>
										<label style="font-weight: bold; " for="analise_todos">
								
											<input
												type="radio"
												name="analise"
												value=""
												id="analise_todos"
												<?= $analise == "" ? 'checked="checked"' : '' ?>
												onchange="alterarFiltroAnalise();"
												onclick="alterarFiltroAnalise();"
											/>Todas
										</label>
										<label for="analise_naoanal"><input id="analise_naoanal" type="radio" name="analise" value="naoanalisado" <?= $analise == "naoanalisado" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Não Analisado</label>
										<label for="analise_emanal"><input id="analise_emanal" type="radio" name="analise" value="emanalise" <?= $analise == "emanalise" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Em Análise</label>
										<label for="analise_anal"><input  id="analise_anal" type="radio" name="analise" value="analisado" <?= $analise == "analisado" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Analisado</label>
									</td>
									<td colspan="1">
										&nbsp;
										<div id="div_analise" style="display: none;">
											<?php
											$minimo = $_REQUEST['minimo'] ? (integer) $_REQUEST['minimo'] : 1;
											$maximo = $_REQUEST['maximo'] ? (integer) $_REQUEST['maximo'] : 100;
											?> 
											<input type="text" style="width: 25px;" maxlength="3" name="minimo" value="<?= $minimo ?>"/>%
											até <input type="text" style="width: 25px;" maxlength="3" name="maximo" value="<?= $maximo ?>"/>%
										</div>
									</td>
								</tr>
								<tr>
									<td colspan="2">
										Situação<br/>
										<?php
								
										
										$esdid = $_REQUEST["esdid"];
										$estados = array(
											CTE_ESTADO_DIAGNOSTICO,
											CTE_ESTADO_PAR,
											CTE_ESTADO_ANALISE,
											CTE_ESTADO_ANALISE_FIN,
											CTE_ESTADO_FINALIZADO,
										);
										$estados = implode( ",", $estados );
										$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 2 order by esdordem";
										$res = $db->carregar( $sql );
										$res [] = array( "codigo" => 99,  "descricao" => "Municípios com Termo de Cooperação Técnica" );
										
										$db->monta_combo( "esdid", $res , 'S', 'Todas as Situações', '', '' );
										?>
									</td>
									<td colspan="1">
										&nbsp;
									</td>
								</tr>
							</table>
							
							<div style="height: 10px;"></div>
							<style>
								#ideb_, #ideb { width: 300px; }
							</style>
							<table border="0" cellpadding="3" cellspacing="0">
								<tr>
									<td width="342">
										Classificação IDEB
									</td>
									<td>
										Filtrar por:
									</td>
								</tr>
							</table>
							
							<?php
			
							$agrupador = new Agrupador( "formulario" );
							
							$sql = sprintf( "select tpmid, tpmdsc from territorios.tipomunicipio where gtmid = 2 and tpmstatus = 'A'" );
							$tipos = $db->carregar( $sql );
							
							$origem = array();
							$destino = array();
							if ( $tipos ) {
								foreach ( $tipos as $tipo ) {
									if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
										$destino[] = array(
											'codigo'    => $tipo['tpmid'],
											'descricao' => $tipo['tpmdsc']
										);
									} else {
										$origem[] = array(
											'codigo'    => $tipo['tpmid'],
											'descricao' => $tipo['tpmdsc']
										);
									}
								}
							}
							
							$agrupador->setOrigem( "ideb_", null, $origem );
							$agrupador->setDestino( "ideb", null, $destino );
							$agrupador->exibir();
		
							?>
						</div>
						<div style="float: right;">
							<input type="button" name="" value="Pesquisar" onclick="pesquisar();"/>
						</div>
					</form>
				</td>
			</tr>
		</tbody>
	</table>
	<script type="text/javascript">
		function pesquisar()
		{
			selectAllOptions( document.getElementById( 'ideb' ) );
			formulario.submit();
		}
		
		function alterarFiltroAnalise()
		{
			var checked = document.getElementById('analise_emanal').checked;
			document.getElementById( 'div_analise' ).style.display = checked ? 'block' : 'none';
		}
		
		alterarFiltroAnalise();
	</script>
<?php
}
# LISTA AS UNIDADES DA FEDERAÇÃO ###############################################


if ( $_REQUEST["acao"] == "E" ) {
	$unidades = cte_pegarUfsPermitidas();
	$unidades_sql = "'" . implode( "','", $unidades ) . "'";
	
	$campos = array( "e.estdescricao", "d.docdsc", "ed.esddsc" );
	$palavras = array_unique( explode( " ", $_REQUEST['filtro'] ) );
	$filtro = array();
	foreach ( $palavras as $palavra ) {
		if ( empty( $palavra ) || strlen( $palavra ) < 3 ) {
			continue; # ignora palavras vazias
		}
		$a = array();
		foreach ( $campos as $campo ) {
			array_push( $a, "lower({$campo}) like lower('%{$palavra}%')" );
		}
		array_push( $filtro, " ( " . implode( " OR ", $a ) . " ) " );
	}
	if ( count( $filtro ) > 0 ) {
		$filtro_sql = " AND " . implode( " AND ", $filtro );
	}
	
	$sql = "SELECT  '<a style=\"margin: 0 -20px 0 20px;\" href=\"brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_lista&acao=E&evento=selecionar&estuf='|| e.estuf ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
					e.estuf,
					e.estdescricao
			FROM territorios.estado e
			INNER JOIN cte.usuarioresponsabilidade ur ON ur.estuf = e.estuf and rpustatus = 'A'
        	INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
        	INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod AND pu.usucpf = ur.usucpf
        	WHERE ";
			if(!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))){
				$sql .= "ur.usucpf = '" . $_SESSION['usucpf'] . "' AND ";
			}
			$sql .= "
		        p.sisid = " . $_SESSION['sisid'] . "
		        GROUP BY e.estuf, e.estdescricao
		        ORDER BY e.estdescricao";

	$cabecalho = array( "Ação", "Código", "Unidade da Federação" );
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
}


# LISTA OS MUNICÍPIOS ##########################################################


if ( $_REQUEST["acao"] == "M" ) {
	
	$municipios = cte_pegarMunicipiosPermitidos();
	$municipios_sql = "'" . implode( "','", $municipios ) . "'";
	
	$campos = array(  "m.mundescricao", "m.muncod", "d.docdsc", "ed.esddsc" );
	$palavras = array_unique( explode( " ", $_REQUEST['filtro'] ) );
	$filtro = array();
	foreach ( $palavras as $palavra ) {
		if ( empty( $palavra ) || strlen( $palavra ) < 3 ) {
			continue; # ignora palavras vazias
		}
		$a = array();
		foreach ( $campos as $campo ) {
			array_push( $a, "lower({$campo}) like lower('%{$palavra}%')" );
		}
		array_push( $filtro, " ( " . implode( " OR ", $a ) . " ) " );
	}
	switch ( $_REQUEST["analise"] ) {
		case "naoanalisado":
			array_push( $filtro, " ( floor( ( coalesce( subacoesanalisadas.total, 0 ) * 100 ) / ( subacoes.total ) ) = 0 ) " );
			break;
		case "emanalise":
			$a = array( abs( $_REQUEST["minimo"] ), abs( $_REQUEST["maximo"] ) );
			$minimo = min( $a );
			$maximo = max( $a );
			array_push( $filtro, " floor( ( coalesce( subacoesanalisadas.total, 0 ) * 100 ) / ( subacoes.total ) ) >= {$minimo} " );
			array_push( $filtro, " floor( ( coalesce( subacoesanalisadas.total, 0 ) * 100 ) / ( subacoes.total ) ) <= {$maximo} " );
			break;
		case "analisado":
			array_push( $filtro, " ( subacoes.total > 0 AND subacoesanalisadas.total = subacoes.total ) " );
			break;
		default:
			break;
	}
	
	if ( $par == "com" ) {
		array_push( $filtro, " coalesce(b.qtd,0) > 0 " );
	} else {
		array_push( $filtro, " coalesce(b.qtd,0) = 0 " );
	}
	
	if (  !empty( $_REQUEST["esdid"] ) ) {
		if( $_REQUEST["esdid"] == 99 )
			array_push( $filtro, " ed.esdid in ( 13, 14, 15, 11 )  " );
		else
			array_push( $filtro, " ed.esdid = {$_REQUEST["esdid"]} " );
	}
	
	if ( count( $filtro ) > 0 ) {
		$filtro_sql = " AND " . implode( " AND ", $filtro );
	}
	
	if ( !empty( $_REQUEST['estuf'] ) ) {
		$estado = " and m.estuf = '{$_REQUEST['estuf']}' ";
	}

	$join_capitais = "";
	if ( $_REQUEST['capitais'] ) {
		$join_capitais = "inner join territorios.estado e on e.estuf = m.estuf and e.muncodcapital = m.muncod";
	}

	$join_grandescidades = "";
	if ( $_REQUEST['grandescidades'] ) {
		$join_grandescidades = "
			inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1
		";
	}

	$join_indigena = "";
	if ( $_REQUEST['indigena'] ) {
		$join_indigena = "
			inner join territorios.muntipomunicipio mti on mti.muncod = m.muncod and mti.estuf = m.estuf and mti.tpmid = 16
		";
	}

	$join_quilombola = "";
	if ( $_REQUEST['quilombola'] ) {
		$join_quilombola = "
			inner join territorios.muntipomunicipio mtq on mtq.muncod = m.muncod and mtq.estuf = m.estuf and mtq.tpmid = 17
		";
	}

	$join_ideb = "";
	if ( count( (array) $_REQUEST['ideb'] ) > 0 ) {
		$join_ideb = "inner join territorios.muntipomunicipio mtm2 on mtm2.muncod = m.muncod and mtm2.estuf = m.estuf and mtm2.tpmid in (". implode( ",", $_REQUEST['ideb'] ) .")";
	}

	$sql = sprintf(
		"select
			'<a style=\"margin: 0 -20px 0 20px;\" href=\"brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_lista&acao=M&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
			m.muncod,
			m.mundescricao,
			m.estuf,
			coalesce( ed.esddsc,'Não iniciado' ) as esddsc
		from territorios.municipio as m
		left join cte.instrumentounidade iu ON iu.mun_estuf = m.estuf and iu.muncod = m.muncod  and iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ."
		left join workflow.documento d on d.docid = iu.docid
		left join workflow.estadodocumento ed on ed.esdid = d.esdid
		%s
		%s
		%s
		%s
		left join cte.indicadorespreenchidos as b on b.inuid=iu.inuid
		left join cte.indicadorestotais as a on a.itrid = iu.itrid
		left join cte.subacoestotal as subacoes on subacoes.inuid = iu.inuid
		left join cte.subacoesanalisadas as subacoesanalisadas on subacoesanalisadas.inuid = iu.inuid
        $join_quilombola
        $join_indigena
		where m.muncod != '5300108' and  m.muncod in ( %s ) %s %s",
		$join_acoes,
		$join_capitais,
		$join_grandescidades,
		$join_ideb,
		$municipios_sql,
		$filtro_sql,
		$estado
	);
	
	//$cabecalho = array( "Ação", "Código", "Município", "UF", "Situação", "Preenchimento", "Análise" );
	$cabecalho = array( "Ação","Código", "Município", "UF","Situação");
	$db->monta_lista( $sql, $cabecalho, 20, 10, 'N', '', '' );

}



?>