<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

include_once APPRAIZ . 'includes/workflow.php';

if ( $_REQUEST['estuf'] && $_REQUEST['evento'] == 'selecionar' ) {
	if( cte_selecionarUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, WF_TIPO_CTE, $_REQUEST['estuf'] ) ) {
		$db->commit();
		header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A" );
		exit();
	}
}   elseif ( $_REQUEST['muncod'] && $_REQUEST['evento'] == 'selecionar' ) {
		if( cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_CTE, $_REQUEST['muncod'] ) ) {
			$db->commit();
			$complemento = $_REQUEST['csFiltroArvore'] ? '&csFiltroArvore='.$_REQUEST['csFiltroArvore'] : "";  
			header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A$complemento" );
			exit();
		}
}
if( $_POST['ajaxdiretriz'] ){
	if( cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_CTE, $_POST['ajaxdiretriz'] ) ) { 
 		$db->commit();
 		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

?>
<?php if ( $_REQUEST["acao"] == "M" ){ 



	?>

<?
?>
<script src="../includes/prototype.js"></script>
	<script type="text/javascript">
		function removerFiltro(){
			document.formulario.filtro.value = "";
			document.formulario.estuf.selectedIndex = 0;
			document.formulario.submit();
		}
	</script>
	<style type="text/css">
  
	.tituloPorcentagem img{
		height: 17px;
		width: 17px;
		margin-right: 1px; 
	}
	
</style>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tbody>
			<tr>
				<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
					<form action="" method="get" name="formulario">
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
									</td>
								</tr>
								<tr>
									<td colspan="2">
										<?php
										$analise = $_REQUEST["analise"];
										?>
										<label style="font-weight: bold;" for="analise_todos">
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
									<td>
										<label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['capitais'] == 1 ? 'checked="checked"' : '' ?>/> Capitais</label>
										<label><input type="checkbox" name="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/> Grandes Cidades</label>
										<br /><br />
										<label><input type="checkbox" name="indigena" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['indigena'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Indígena</label>
										<label><input type="checkbox" name="quilombola" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Quilombola</label>
										<label><input type="checkbox" name="cidadania" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['cidadania'] == 1 ? 'checked="checked"' : '' ?>/> Territórios da Cidadania</label>
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
									</td>
								</tr>
								<tr>
									<td colspan="2">
										<br>
										Monitoramento<br> 
										<input type="radio" name="init" id = "init" value="iniciado" <? if( $_REQUEST['init'] == "iniciado") echo 'checked=checked'; ?> >Iniciado  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										<input type="radio" name="init" id = "init" value="naoiniciado" <? if( $_REQUEST['init'] == "naoiniciado") echo 'checked=checked'; ?> >Não Iniciado &nbsp;&nbsp;&nbsp; 
										<input type="radio" name="init" id = "init" value="todos" <? if( $_REQUEST['init'] == "todos") echo 'checked=checked'; ?> >Todos
									
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
		function abreDiretriz( ){
  
		 	 return window.open('cte.php?modulo=principal/verificarCumprimentoDiretrizes&acao=A',
		                  'verificarCumprimentoDiretrizes',
		                  'height=420,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
		}
		function abreDiretriz( id ){

			var req = new Ajax.Request('cte.php?modulo=lista&acao=M', {
							        method:     'post',
							        parameters: '&ajaxdiretriz=' + id,							         
							        onComplete: function (res) { 
										return window.open('cte.php?modulo=principal/verificarCumprimentoDiretrizes&acao=A&type=popup',
							                  'verificarCumprimentoDiretrizes',
							                  'height=420,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
									}
				});
		} 
		alterarFiltroAnalise();
	</script>
<?php } ?>

<?php

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
	
	$sql = sprintf(
		"select
			'<a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=lista&acao=E&evento=selecionar&estuf='|| e.estuf ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
			e.estuf,
			e.estdescricao,
			coalesce( ed.esddsc,'Não iniciado' ) as esddsc,
			'<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) || '%%</span>
			<div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title=\"' ||  floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%\">
			<div style=\"font-size:4px;width: ' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%; height: 6px; max-height: 6px; background-color:#339933;\">
			</div></div></center>' as porcentagem
		from territorios.estado as e	
		left join cte.instrumentounidade iu ON iu.estuf = e.estuf and iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_ESTADUAL ."
		left join workflow.documento d on d.docid = iu.docid
		left join workflow.estadodocumento ed on ed.esdid = d.esdid
		left join (
			select count(*) as qtd, i.inuid
			from cte.instrumentounidade i
			inner join cte.dimensao d on d.itrid = i.itrid and d.dimstatus = 'A'
			inner join cte.areadimensao a on a.dimid = d.dimid and a.ardstatus = 'A'
			inner join cte.indicador ind on ind.ardid = a.ardid and ind.indstatus = 'A'
			inner join cte.pontuacao p on p.indid = ind.indid and p.inuid=i.inuid and p.ptostatus = 'A'
			inner join cte.criterio c on c.crtid = p.crtid
			where i.itrid=1
			group by i.inuid
		) as b on b.inuid=iu.inuid
		left join (
			select count(*) as qtdTotal, i.inuid
			from cte.instrumentounidade i
			inner join cte.dimensao d on d.itrid = i.itrid and d.dimstatus = 'A'
			inner join cte.areadimensao a on a.dimid = d.dimid and a.ardstatus = 'A'
			inner join cte.indicador ind on ind.ardid = a.ardid and ind.indstatus = 'A'
			where i.itrid=1
			group by i.inuid
		) as a on a.inuid=iu.inuid
		where e.estuf in ( %s ) %s",
		$unidades_sql,
		$filtro_sql
	);

	$cabecalho = array( "Ação", "Código", "Unidade da Federação", "Situação", "Preenchimento" );
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
}


# LISTA OS MUNICÍPIOS ##########################################################


if ( $_REQUEST["acao"] == "M" ) {
	
	$municipios = cte_pegarMunicipiosPermitidos();
	if($municipios != false){
		$municipios_sql = "'" . implode( "','", $municipios ) . "'";
		$temMunicipio = "and  m.muncod in ( ".$municipios_sql." )";
	}else{
		$temMunicipio = '';
	}
	
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
	if ( $_REQUEST['init'] == "iniciado" ) {
		$where .= " and inumonitoratecnico = true ";
	}elseif( $_REQUEST['init'] == "naoiniciado" ){
		$where .= " and inumonitoratecnico = false ";
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
	
	$join_cidadania = "";
	if ( $_REQUEST['cidadania'] ) {
		$join_cidadania = "
			inner join territorios.muntipomunicipio mtq2 on mtq2.muncod = m.muncod and mtq2.estuf = m.estuf and mtq2.tpmid = 139
		";
	}

	$join_ideb = "";
	if ( count( (array) $_REQUEST['ideb'] ) > 0 ) {
		$join_ideb = "inner join territorios.muntipomunicipio mtm2 on mtm2.muncod = m.muncod and mtm2.estuf = m.estuf and mtm2.tpmid in (". implode( ",", $_REQUEST['ideb'] ) .")";
	}
	
	$sql = sprintf(
		"select
			CASE
			  WHEN ed.esdid IN( 11, 14, 15, 13 ) THEN  
			  	'<a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=lista&acao=M&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
				 <a class=\"tituloPorcentagem\" style=\"margin: 0 -20px 0 20px; cursor: pointer;\" onclick=\"abreDiretriz('||m.muncod||');\" ><img src=\"../../imagens/grafico_pizza.png\" border=0 title=\"Diretrizes\"/></a>
				 <a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=principal/monitora_lista&acao=A&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/consultar.gif\" border=0 title=\"Monitoramento\"></a>' 
			  ELSE 
			  	'<a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=lista&acao=M&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
			 	 <a class=\"tituloPorcentagem\" style=\"margin: 0 -20px 0 20px;  cursor: pointer;\" onclick=\"abreDiretriz('||m.muncod||');\" ><img src=\"../../imagens/grafico_pizza.png\" border=0 title=\"Diretrizes\"/></a>' 
			END AS acao,
			m.muncod,
			m.mundescricao,
			m.estuf,			
			coalesce( ed.esddsc,'Não iniciado' ) as esddsc,
			'<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) || '%%</span>
			<div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title=\"' ||  floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%\">
			<div style=\"font-size:4px;width: ' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%; height: 6px; max-height: 6px; background-color:#339933;\">
			</div></div></center>' as porcentagem,
			
			'<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total)) || '%%</span>
			<div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcdcff;\" title=\"' ||  floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total))  || '%%\">
			<div style=\"font-size:4px;width: ' || floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total))  || '%%; height: 6px; max-height: 6px; background-color:#333399;\">
			</div></div></center>' as porcentagemsubacao 
			, 
			CASE
			  WHEN im.inuid is not null THEN   
				'<center><img src=\"/imagens/check.jpg\" border=0 title=\"Iniciado\"></center>' 
			  ELSE 
				'<center><img src=\"/imagens/unchecked.jpg\" border=0 title=\"Não-iniciado\"></center>' 
			END AS monitora
 
		from territorios.municipio as m
		INNER JOIN cte.instrumentounidade iu ON iu.mun_estuf = m.estuf and iu.muncod = m.muncod  and iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ."       
		left join workflow.documento d on d.docid = iu.docid AND d.tpdid 	= 2  
		left join workflow.estadodocumento ed on ed.esdid = d.esdid
		%s
		%s
		%s
		%s
		left join cte.indicadorespreenchidos as b on b.inuid=iu.inuid
		left join cte.indicadorestotais as a on a.itrid = iu.itrid
		left join cte.subacoestotal as subacoes on subacoes.inuid = iu.inuid
		left join cte.subacoesanalisadas as subacoesanalisadas on subacoesanalisadas.inuid = iu.inuid
		left join cte.instrumentomonitorados im on im.inuid = iu.inuid
        $join_quilombola
        $join_indigena
        $join_cidadania
		where m.muncod not in ('2605459', '5300108') $temMunicipio %s %s $where ",
		$join_acoes,
		$join_capitais,
		$join_grandescidades,
		$join_ideb,
		$filtro_sql,
		$estado
	);
//dbg($sql,1);
	$cabecalho = array( "Ação","Código", "Município", "UF","Situação", "Preenchimento", "Análise", "Monitoramento iniciado?");
	$db->monta_lista( $sql, $cabecalho, 20, 10, 'N', '', '' );
}
?>