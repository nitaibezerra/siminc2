<?php
// carrega as funções gerais
include_once "config.inc";
include ("../../includes/funcoes.inc");
include ("../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['requisicao'])
{
	ob_clean();
	$_REQUEST['requisicao']();
	die;
}

function carregaEstado()
{
	global $db;
	$sql = "
		select 
			ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(estpoligono, 2249), 12000),4291),2, 0) as poli,
			REPLACE(REPLACE(st_astext(st_centroid(estpoligono)),'POINT(',''),')','') as centro, 
			estuf, 
			removeacento(estdescricao) as estdescricao 
		from territoriosgeo.estado 
		where estuf='".$_REQUEST['estuf']."' ";
	$dados = $db->carregar($sql,3600); 
	echo JSON_encode($dados);
	
}

function carregaTipoMunicipio() {
	global $db;
	if($_REQUEST['gtmid']) $filtro[] = "gtmid='".$_REQUEST['gtmid']."'";
	if($_REQUEST['tpmid']) $filtro[] = "tpmid='".$_REQUEST['tpmid']."'";
	$sql = "	select 
					ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(tpmpoligono, 2249), 12000),4291),2, 0) as poli,
					REPLACE(REPLACE(st_astext(st_centroid(tpmpoligono)),'POINT(',''),')','') as centro, 
					removeacento(tpmdsc) as tpmdsc, 
					tpmid,
					gtmid,
					'".$_SESSION['painel_vars']['mapid']."' as mapid,
					'&lt;iframe class=iframeBalao src=painel.php?modulo=principal/mapas/mapaPadraoEEacao=AEErequisicao=montaBalaoTipoMunicipioEEtpmid=' || tpmid || 'EEgtmid='||gtmid||'EEmapid=".$_SESSION['painel_vars']['mapid']." &gt;&lt;/iframe&gt;' as info,
					'#f6ead9' as cor 
				from 
					territoriosgeo.tipomunicipio ".(($filtro)?"WHERE ".implode(" AND ", $filtro):"");
	$dados = $db->carregar($sql); 
	echo JSON_encode($dados);
	

}

function carregaMunicipio()
{
	global $db;
	$sql = "SELECT ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(munpoligono, 2249), 12000),4291),2, 0) as poli, muncod, removeacento(mundescricao) as mundescricao, estuf, '#f6ead9' as cor
	from territoriosgeo.municipio mun
	where munpoligono is not null
	".(($_REQUEST['estuf'])?" and mun.estuf in ('".$_REQUEST['estuf']."')":" ")."
	".(($_REQUEST['muncod'])?" and mun.muncod in ('".$_REQUEST['muncod']."') ":" ");
	$dados = $db->carregar($sql,3600); 
	echo JSON_encode($dados);
	
}

function infGrupoMunicipio()
{
	global $db;
	
	extract($_REQUEST);
	
	$sql = "select
				tma.tmaid
			from
				mapa.mapatema mpt
			inner join
				mapa.mapa map ON map.mapid = mpt.mapid
			inner join
				mapa.tema tma ON tma.tmaid = mpt.tmaid
			where
				map.mapid = {$_SESSION['painel_vars']['mapid']}
			order by
				tma.tmadsc";
	$hdn_tmaid = $db->carregarColuna($sql);
	
	$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
	$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
	$arrWhere[] = "tpm.gtmid = $gtmid";
	$arrWhere[] = "tpm.tpmid = $tpmid";
	$campo = "tpm.tpmid";
	$descricao = "tpmdsc";
	$chave = "tpm.tpmid";
	$arrGroupBy[] = "tpm.tpmid";
	$arrGroupBy[] = "tpmdsc";
		
	if($hdn_tmaid){
		foreach($hdn_tmaid as $tmaid){
			if($tmaid != 1){
				$arrInnerJoin[] = "mapa.temadado  tem_{$tmaid} ON tem_{$tmaid}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$tmaid}.tmaid = $tmaid");
				$sql = "select 
							tmadsc,
							tpddsc,
							tpdcampotema 
						from 
							mapa.tipodado tpd
						inner join
							mapa.tema tma ON tma.tpdid = tpd.tpdid
						where
							tma.tmaid = $tmaid";
				$tdm = $db->pegaLinha($sql);
				if($tdm['tpddsc'] == "Boleano"){
					$arrCampos[] = "(CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) as capmpo{$tmaid}";
				}else{
					$arrCampos[] = "sum(tem_{$tmaid}.{$tdm['tpdcampotema']})".($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")." as capmpo{$tmaid}";	
				}
				$arrColunas[] = $tdm['tmadsc'];
			}else{
				$arrCampos[] = "sum(mun.munpopulacao) as campo{$tmaid}";
				$arrColunas[] = "População";
			}
		}
	}
				
	$sql = "select distinct
				$campo as acao,
				$chave as muncod,
				$descricao as descri
				".($arrCampos ? " , ".implode(" , ",$arrCampos) : "")."
			from 
				territoriosgeo.municipio mun
			".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")."
			left join
				mapa.temadado pro ON mun.muncod = pro.muncod 
			where
				1=1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
			order by
				$descricao";
	dbg($sql,1);
	$arrCabecalho = array("Ação","Descrição");

	if($arrColunas){
		foreach($arrColunas as $coluna){
			array_push($arrCabecalho,$coluna);
		}
	}
	
	$dados = $db->carregar($sql);

	if($dados){
	
		foreach($dados as $d){
				$arrMuncod[] = $d['muncod'];
		}
		$arrMunRep = array();
		foreach(array_count_values($arrMuncod) as $mun => $rep){
			if($rep > 1){
				$arrMunRep[] = $mun;
			}
		}
		
		foreach($dados as $d){
			if(in_array($d['muncod'],$arrMunRep) && ( ($tpdcampotema == "tmdboleano" && $d['parametro'] == "f") || ($tpdcampotema != "tmdboleano" && $d['parametro'] <= 0) ) ){
				
			}else{
				unset($d['muncod']);
				$arrDados[] = $d;
			}
		}
		
	}
	
	require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");
	$ajax = new MontaListaAjax($db);
	$ajax->montaLista($arrDados,$arrCabecalho,50,5,"N","center",100);
	
}


function infoMunicipio()
{
	global $db;

	$sql = "select 
				mun.*,
				estdescricao,
				( select sum(distinct munpopulacao) from territorios.municipio m3 where m3.estuf = mun.estuf  ) as populacao_est,
				mun.mescod,
				mesdsc,
				( select sum(distinct munpopulacao) from territorios.municipio m1 where m1.mescod = mun.mescod  ) as populacao_mes,
				mun.miccod,
				mic.micdsc,
				( select sum(distinct munpopulacao) from territorios.municipio m2 where m2.miccod = mun.miccod  ) as populacao_mic,
				( select sum(distinct munpopulacao) from territorios.municipio m4 inner join territorios.municipiosvizinhos mv on m4.muncod = mv.muncodvizinho where mv.muncod = mun.muncod ) as populacao_viz,
				( select sum(distinct munpopulacao) from territorios.municipio m5 where m5.muncod in ( select 
																											distinct muncod 
																										from 
																											territoriosgeo.muntipomunicipio where tpmid = (
																										select 
																											tpm.tpmid
																										from 
																											territoriosgeo.muntipomunicipio mtp
																										inner join
																											territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtp.tpmid
																										where 
																											muncod = '{$_REQUEST['muncod']}'
																										and
																											gtmid = 1 ) ) ) as populacao_saude
			from 
				territorios.municipio mun
			inner join
				territorios.estado e on e.estuf = mun.estuf
			inner join
				 territorios.mesoregiao mes ON mes.mescod = mun.mescod
			inner join
				 territorios.microregiao mic ON mic.miccod = mun.miccod
			where 
				mun.muncod = '{$_REQUEST['muncod']}' ";
	//dbg($sql,1);
	$arrDados = $db->pegaLinha($sql);
	if($arrDados)
		extract($arrDados);
		
		$sql = "select 
					tpmdsc
				from 
					territoriosgeo.muntipomunicipio mtp
				inner join
					territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtp.tpmid
				where 
					muncod = '{$_REQUEST['muncod']}'
				and
					gtmid = 1 ";
		$regiao_saude = $db->pegaUm($sql);
		
	?>
	<script language="JavaScript" src="/includes/funcoes.js"></script>
	<script type="text/javascript" src="/includes/maps/maps.js"></script>
	<script>
		function exibeListaMunicipio(muncod){
		window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	}
	</script>
	<div style="font-size:12px;margin-3px;text-align:center;" >
	<b>Localização:</b> <span style="cursor:pointer" onmouseover="f_mouseover('<?=$estuf.$muncod?>','#F0F','<?=$mundescricao.'/'.$estuf?>');" onmouseout="f_mouseout('<?=$estuf.$muncod?>','');" onclick="window.open('http://www.ibge.gov.br/cidadesat/xtras/perfilwindow.php?nomemun=<?php echo $mundescricao ?>&codmun=<?php echo substr($muncod,0,6) ?>&r=2','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no')" > <?php echo $mundescricao ?> / <?php echo $estuf ?> </span>
	<table style="margin-top:4px" class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" >
		<thead>
			<tr><td>&nbsp;</td><td>Tipo</td><td>População</td><td>% Estado</td></tr>
		</thead>
			<tr><td><input type="checkbox" name="microregiao" onclick="f_mudacores('<?=$muncod ?>',this);"></td><td>Município:</td><td align="right"><?php echo number_format($munpopulacao,0,".",".") ?></td><td align="right"><?php echo round(( $munpopulacao/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="microregiao" onclick="f_mudacores('<?=retornaMunicipiosVizinho($muncod) ?>',this);"></td><td>Municípios Vizinhos:</td><td align="right"><?php echo number_format($populacao_viz,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_viz/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="microregiao" onclick="f_mudacores('<?=retornaMunicipio("miccod",$miccod) ?>',this);"></td><td>Microregião: <?php echo $micdsc ?></td><td align="right"><?php echo number_format($populacao_mic,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_mic/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="mesoregiao" onclick="f_mudacores('<?=retornaMunicipio("mescod",$mescod) ?>',this);"></td><td>Mesoregião: <?php echo $mesdsc ?></td><td align="right"><?php echo number_format($populacao_mes,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_mes/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td><input type="checkbox" name="estado" onclick="f_mudacores('<?=retornaMunicipio("estuf",$estuf) ?>',this);"></td><td>Estado: <?php echo $estdescricao ?></td><td align="right"><?php echo number_format($populacao_est,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_est/$populacao_est )*100,2) ?>%</td></tr>
			<?php  if($_SESSION['painel_vars']['mapid'] == 32): ?>
				<tr><td><input type="checkbox" name="estado" onclick="f_mudacores('<?=retornaMunicipioSaude($muncod) ?>',this);"></td><td>Regiões da Saúde: <?php echo $regiao_saude ?></td><td align="right"><?php echo number_format($populacao_saude,0,".",".") ?></td><td align="right"><?php echo round(( $populacao_saude/$populacao_est )*100,2) ?>%</td></tr>
			<?php  endif; ?>
	</table>
	<br />
	<table class="tabela" cellspacing="0" cellpadding="2" border="0" align="center" >
		<tr><td class="SubtituloEsquerda" ><b>Outras Classificações</b></td></tr>
	</table>
	
	<?php $sql = "select gtmdsc,tpmdsc from 
					territorios.muntipomunicipio  m
				inner join
					territorios.tipomunicipio t ON t.tpmid = m.tpmid
				inner join
					territorios.grupotipomunicipio g ON g.gtmid = t.gtmid
				where 
					muncod = '{$_REQUEST['muncod']}'
				order by
					gtmdsc,tpmdsc";
	$cabecalho = array("Grupo","Tipo");
	$db->monta_lista_simples($sql,$cabecalho,10,10,"N","95%","N");?>
	<br />
	</div><br />
	<?php
}

function retornaMunicipiosVizinho($muncod) 
{
	global $db;
	$sql = "select muncodvizinho from territorios.municipiosvizinhos where muncod='".$muncod."'";
	$arrMuncod = $db->carregarColuna($sql);
	return implode(",", $arrMuncod);
}

function retornaMunicipio($campo, $valor)
{
	global $db;
	$sql = "select muncod as obj from territorios.municipio where ".$campo."='".$valor."'";
	$arrMuncod = $db->carregarColuna($sql);
	return implode(",", $arrMuncod);
}

function retornaMunicipioSaude($muncod)
{
	global $db;
	$sql = "select 
				distinct muncod 
			from 
				territoriosgeo.muntipomunicipio where tpmid = (
			select 
				tpm.tpmid
			from 
				territoriosgeo.muntipomunicipio mtp
			inner join
				territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtp.tpmid
			where 
				muncod = '$muncod'
			and
				gtmid = 1 )";
	$arrMuncod = $db->carregarColuna($sql);
	return implode(",", $arrMuncod);
}

