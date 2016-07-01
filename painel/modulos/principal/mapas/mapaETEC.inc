<?php

if($_REQUEST['requisicao'])
{
	ob_clean();
	$_REQUEST['requisicao']();
	die;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

if(!$_REQUEST['mapid']){
	$arrMapas = pegaMapasCadastrados();
}else{
	$sql = "select mapid from mapa.mapa where mapid = {$_REQUEST['mapid']} and mapstatus = 'A'";
	$mapid = $db->pegaUm($sql);
	if(!$mapid){
		$arrMapas = pegaMapasCadastrados();
	}else{
		$_SESSION['painel_vars']['mapid'] = $mapid;
		pegaMapasCadastrados(null,false);
	}
}

function relatorioETEC() {
	
	global $db;
	if(!$_REQUEST['groupers']) :
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<div id="filtros" style="border-color : #000; border-style : solid; border-width: .2em; display : none; background-color : #F5F5DC; position: absolute; width: 350px; left: 288px; top: 136px; height: 80px">fsdfsdf</div>
	<script>
	
	function fecharFiltros() {
		document.getElementById('filtros').style.display='none';	
	}
	
	function getPosicaoElemento(elemID){
	    var offsetTrail = document.getElementById(elemID);
	    var offsetLeft = 0;
	    var offsetTop = 0;
	    while (offsetTrail) {
	        offsetLeft += offsetTrail.offsetLeft;
	        offsetTop += offsetTrail.offsetTop;
	        offsetTrail = offsetTrail.offsetParent;
	    }
	    if (navigator.userAgent.indexOf("Mac") != -1 && 
	        typeof document.body.leftMargin != "undefined") {
	        offsetLeft += document.body.leftMargin;
	        offsetTop += document.body.topMargin;
	    }
	    return {left:offsetLeft, top:offsetTop};
	}
	
	function agrupacao(grouper,obj) {
		if(obj.title=='mais') {
	    	document.getElementById('filtros').style.left=(getPosicaoElemento(obj.id).left+100);
	    	document.getElementById('filtros').style.top=getPosicaoElemento(obj.id).top;
	    	var html_dados = document.getElementById('id_'+grouper).innerHTML+'<input type=hidden id=filtro value="'+obj.id.substr(4)+'">';
	    	document.getElementById('filtros').innerHTML = html_dados;
			document.getElementById('filtros').style.display='';
		} else {
			obj.title='mais';
			obj.src='../imagens/mais.gif';
			var tabela = obj.parentNode.parentNode.parentNode;
			var linhaIndex = obj.parentNode.parentNode.rowIndex;
			tabela.deleteRow(linhaIndex);

		}
	}
	
	function agrupar() {
		
		divCarregando();
		
		var tabela = document.getElementById('img_'+document.getElementById('filtro').value).parentNode.parentNode.parentNode;
		
		for(var i=0;i<tabela.rows.length;i++) {
			if(tabela.rows[i].cells[0].childNodes[0].title=='menos') {
				agrupacao('',tabela.rows[i].cells[0].childNodes[0]);
			}
		}
		
		document.getElementById('img_'+document.getElementById('filtro').value).title='menos';
		document.getElementById('img_'+document.getElementById('filtro').value).src='../imagens/menos.gif';
		
		var linhaIndex = document.getElementById('img_'+document.getElementById('filtro').value).parentNode.parentNode.rowIndex;
		var nlinha = tabela.insertRow(linhaIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=10;
		
		jQuery.ajax({
	   		type: "POST",
	   		url: "painel.php?modulo=principal/mapas/mapaETEC&acao=A&requisicao=relatorioETEC",
	   		data: "filtro="+document.getElementById('filtro').value+"&groupers="+document.getElementById('select_grouper').value+"&orders="+document.getElementById('select_order').value+"&"+jQuery("[name^='groupers_old[']").serialize()+"&"+jQuery("[name^='filtros_old[']").serialize(),
	   		async: false,
	   		success: function(msg){
				ncol.innerHTML = msg;		   		
	   		}
	 		});
	 		
		document.getElementById('filtros').style.display='none';
		
		divCarregado();

	}
	
	</script>
	<?
	endif;

	include APPRAIZ."painel/modulos/relatorio/etec/geral_resultado_mapa.inc";
}


function pegaMapasCadastrados($usucpf = null,$die = true)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$pflcod = PAINEL_PERFIL_MAPAS;
	
	if($db->testa_superuser()) {
		
		$sql = "SELECT DISTINCT 
					m.mapid AS codigo, 
					m.mapdsc AS descricao
				FROM 
					mapa.mapa m  
				WHERE 
					m.mapstatus = 'A'
				AND
					m.mapid IN (20)
				order by
					m.mapdsc";
		
	} else {
	
		$sql = "SELECT DISTINCT 
					m.mapid AS codigo, 
					m.mapdsc AS descricao
				FROM 
					painel.usuarioresponsabilidade ur 
				INNER JOIN 
					mapa.mapa m ON ur.mapid = m.mapid 
				WHERE 
					ur.rpustatus = 'A' 
				AND 
					ur.usucpf = '$usucpf' 
				AND 
					ur.pflcod = $pflcod 
				AND 
					m.mapstatus = 'A'
				AND
					m.mapid IN (20)
				order by
					m.mapdsc";
	
	}
	
	$arrMapas = $db->carregar($sql);

	if(!$arrMapas){ ?>
		<table class="tabela" style="margin-bottom:20px" align="center" cellpadding="3" cellspacing="1" width="100%" >
			<tr>
				<td style="font-weight:bold;color:#000000;text-align:center" >Não existem mapas atribuídos ao seu perfil.</td>
			</tr>
		</table>
		<script>document.getElementById('aguarde').style.display='none'</script>
	<?php
	exit;
	}
	if(count($arrMapas) > 1){?>
		<table class="tabela" style="margin-bottom:20px" align="center" cellpadding="3" cellspacing="1" width="100%" >
			<tr>
				<td width="30%" class="SubtituloDireita" >Selecione o Mapa:</td>
				<td><?php $db->monta_combo("mapid",$arrMapas,"S","Selecione..","selecionaMapaUR","","","","","","",$_SESSION['painel_vars']['mapid']) ?></td>
			</tr>
		</table>
		<script>
			document.getElementById('aguarde').style.display='none';
			function selecionaMapaUR(mapid)
			{
				if(!mapid)
				{
					alert('Favor selecionar o Mapa!');
					return false;
				}else{
					var url = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=' + mapid;
					window.location.href=url;
				}
			}
		</script>
	<?php
	if($die){
		die;
	}
	}else{
		$_SESSION['painel_vars']['mapid'] = $arrMapas[0]['codigo'];
		?>
		<table class="tabela" style="margin-bottom:20px" align="center" cellpadding="3" cellspacing="1" width="100%" >
			<tr>
				<td style="font-weight:bold;color:#000000;text-align:center" ><?php echo $arrMapas[0]['descricao'] ?></td>
			</tr>
		</table>
		<?php
	}
	
}

function gerarXml() {
	global $db;
	
	$_arrSQL['xml']=true;
	
	$sql = montaSQL($_arrSQL);
	
	$dados = $db->carregar($sql);
	
	if($dados):
		
		$conteudo .= "<markers> "; // inicia o XML
		
		foreach($dados as $d):
										
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "info=\"". $d['info'] ."\" ";
			$conteudo .= "lat=\"". $d['lat'] ."\" ";
			$conteudo .= "lng=\"". $d['lng'] ."\" ";
			$conteudo .= "icon=\"". $d['icon'] ."\" ";
			$conteudo .= "/> ";
		
		endforeach;
		
		$conteudo .= "</markers> ";
		print $conteudo;
		
	endif;
}


function montaBalao() {
	global $db;
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$municipio = $db->pegaUm("SELECT mundescricao||' / '||estuf FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>".$municipio."</td></tr>";
	echo "</table>";
	
	$sql = "SELECT  CASE WHEN mpcdsc IS NOT NULL THEN dtidsc||' ('|| mpcdsc ||')'
					ELSE 
						dtidsc
					END as dtidsc,
					CASE WHEN vlivalor IS NOT NULL THEN ROUND(vlivalor,2)::text
						 WHEN vliqtd IS NOT NULL THEN ROUND(vliqtd)::text
						 WHEN vliboleano IS NOT NULL THEN CASE WHEN vliboleano=TRUE THEN '<center>Sim</center>' ELSE '<center>Não</center>' END
					END as valor
			FROM mapa.valorindicador vi 
			INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
			LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
			INNER JOIN mapa.indicador ind ON ind.indid = di.indid
			WHERE vi.muncod='".$_REQUEST['muncod']."' AND ind.mapid='".$_SESSION['painel_vars']['mapid']."' 
			ORDER BY di.dtiordem";
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'")."</td></tr>";
	echo "</table>";
	$cabecalho = array("Detalhe","&nbsp;");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Dados Gerais</td></tr>";
	echo "</table>";
	
	$sql = "SELECT coepolo, coeofertante, coenomecurso, coeqtdpolo, coeqtdaluno
			FROM painel.consultaofertasetec
			WHERE muncod='".$_REQUEST['muncod']."'";

	$cabecalho = array("Polo","Ofertante","Curso","Qtd Polo","Qtd Aluno");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
	$sql = "SELECT tmadsc,
					CASE WHEN tmdvalor IS NOT NULL THEN ROUND(tmdvalor,2)::text
					WHEN tmdboleano IS NOT NULL THEN CASE WHEN tmdboleano=TRUE THEN '<center>Sim</center>' ELSE '<center>Não</center>' END 
					WHEN tmdtexto IS NOT NULL THEN tmdtexto 
					END as valor  
			FROM mapa.temadado td 
			INNER JOIN mapa.tema tm ON tm.tmaid = td.tmaid 
			WHERE muncod='".$_REQUEST['muncod']."'";
	$cabecalho = array("Tema","&nbsp;");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

}

function montaSQL($dados = false) {
	global $db;
	
	$filtro[] = "m.munlatlong IS NOT NULL";
	
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "m.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "m.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	if( $_REQUEST['icone'][0] ){
		$filtro[] = "ind.indid in ('".implode("','",$_REQUEST['icone'])."') ";
		
		$filtrob = array();
		if($_REQUEST['cond']) {
			foreach($_REQUEST['cond'] as $indid => $cond) {
				
				$nomecoluna = $db->pegaUm("SELECT t.tpdcampo||'_'||i.dtiid as nomecoluna FROM mapa.indicador i 
										   INNER JOIN mapa.detalheindicador d ON d.dtiid = i.dtiid 
							 			   INNER JOIN mapa.tipodado t ON t.tpdid = d.tpdid WHERE i.indid='".$indid."'");
				
				if($cond=="com") {
					$filtrob[] = $nomecoluna.">0";
				} elseif($cond=="sem") {
					$filtrob[] = $nomecoluna."=0";
				}
			}
			
			$filtro2[] = "( ".implode(" ".$_REQUEST['contexto']." ", $filtrob)." )";
			
		}
	}
	
	if( $_REQUEST['cenario'] ){
		$filtro[] = "(q.mpcid='".$_REQUEST['cenario']."' OR q.mpcid IS NULL)";
	}
	
	if($_REQUEST['filt_populacao']) {
		
		if($_REQUEST['filt_populacao'] == "entre") {
			$filtro[] = "m.munpopulacao BETWEEN ".$_REQUEST['ent_populacao1']." AND ".$_REQUEST['ent_populacao2'];
		} else {
			$filtro[] = "m.munpopulacao ".$_REQUEST['filt_populacao']." ".$_REQUEST['qtd_populacao'];
		}
		
	}
	
	if(in_array('grupo',$_REQUEST['agrupador'])) {
		$inner_ter = "INNER JOIN territoriosgeo.muntipomunicipio mtm ON mtm.muncod = m.muncod 
                      INNER JOIN territoriosgeo.tipomunicipio tmc ON tmc.tpmid = mtm.tpmid AND tmc.gtmid=".$_REQUEST['gtmid'];
		$colum_alias_ter = ", grupo"; 
		$colum_ter = "tpmdsc as grupo,";
		
	}
	
	if($dados['xml']) {
		$colum_xml = ", icon";
	}
	
	$filtro[] = "ind.indstatus='A'";
	
$sql = "SELECT municipio, regiao, uf, lng, lat, info {$colum_xml} {$colum_alias_ter} ".(($dados['d2'])?", ".implode(",",$dados['d2']):"")." FROM (

                        SELECT  '<span style=cursor:pointer onclick=localizaMapa2(\''||m.muncod||'\',\''||ST_X(m.munlatlong)||'\',\''||ST_Y(m.munlatlong)||'\')>'||m.mundescricao||'</span>' as municipio,
                                    '<a style=cursor:pointer; onclick=linkPoloReg(\''||UPPER(r.regdescricao)||'\');>'||r.regdescricao||'</a>' as regiao,
                                    '<a style=cursor:pointer; onclick=linkPoloEst(\''||e.estuf||'\');>'||e.estuf||'</a>' as uf,
                                    ST_X(m.munlatlong) as lng,
                                    ST_Y(m.munlatlong) as lat,
									{$colum_ter}
                                    '&lt;iframe style=padding:10px; src=painel.php?modulo=principal/mapas/mapaETECEEacao=AEErequisicao=montaBalaoEEmuncod='||m.muncod||'EEmapid=".$_SESSION['painel_vars']['mapid']." frameborder=0 scrolling=auto height=300px width=500px &gt;&lt;/iframe&gt;' as info,
                                    indicone as icon
                                    ".(($dados['d1'])?", ".implode(",",$dados['d1']):"")."
                        FROM mapa.valorindicador q
                        INNER JOIN mapa.detalheindicador di ON di.dtiid = q.dtiid
                        INNER JOIN mapa.indicador ind ON ind.indid = di.indid
                        INNER JOIN territoriosgeo.municipio m on m.muncod = q.muncod
                        {$inner_ter} 
                        INNER JOIN territoriosgeo.estado e on e.estuf = m.estuf
                        INNER JOIN territoriosgeo.regiao r on r.regcod = e.regcod
                        ".(($filtro)?"WHERE ".implode(" AND ",$filtro):"").") foo 
                        GROUP BY municipio, regiao, uf, lng, lat, info {$colum_xml} {$colum_alias_ter}
                        ORDER BY ".implode(",",$_REQUEST['agrupador']);

	return $sql;
}


function agp_relatorio($_agp_relatorio){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => $_agp_relatorio,
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "regiao",
															"label" => "Região"
														  ),
													array(
															"campo" => "uf",
															"label" => "UF"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "grupo",
															"label" => "Grupo"
														  )
														  
														  
												)	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "grupo":
				array_push($agp['agrupador'], array(
													"campo" => "grupo",
											  		"label" => "Grupo")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}

function carregarParametroMapa()
{
	global $db;
	
	if($_GET['estuf']){
		$arrEstados = explode(",",str_replace("estuf","",$_GET['estuf']));
		$arrWhere[] = "mun.estuf in ('".implode("','",$arrEstados)."')";
	}
	
	$sql = "select
				tpdcampotema
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = {$_GET['parametro']}";
	$tpdcampotema = $db->pegaUm($sql);
	
	if($_GET['parametro'] == 1){
		$tpdcampotema = "munpopulacao";
	}
	
	switch($tpdcampotema)
	{
		case "tmdboleano":
			$coalesce = "false";
			$whereCampo = "";
		break;
		case "tmdtexto":
			$coalesce = "'N/A'";
			$whereCampo = "";
		break;
		default:
			$coalesce = "0";
			$whereCampo = " and $tpdcampotema >= 0 ";
	}
	
	$sql = "select 
				mun.muncod,
				COALESCE($tpdcampotema,$coalesce) as parametro,
				COALESCE(tmacor,(SELECT tmacor from mapa.tema where tmaid = {$_GET['parametro']}) ) as itemcor,
				'{$_GET['parametro']}' as tipoparametro 
			from 
				territorios.municipio mun 
			left join
				mapa.temadado  tem ON tem.muncod = mun.muncod and tmaid = {$_GET['parametro']}
			left join
				mapa.tema tma ON tma.tmaid = tem.tmaid
			where
				1=1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")." 
			order by
				$tpdcampotema";
	
	$dados = $db->carregar($sql);
	echo JSON_encode($dados);
}


function gerarRelatorio() {
	global $db;
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$coluna = array();
	

	if($_REQUEST['icone']) {
		
		foreach($_REQUEST['icone'] as $ic) {
			
			$sql = "SELECT tpd.tpdtipo, tpd.tpdcampo, dli.dtiid, dli.dtidsc FROM mapa.detalheindicador dli 
					INNER JOIN mapa.tipodado tpd ON tpd.tpdid = dli.tpdid 
					INNER JOIN mapa.indicador ind ON ind.indid = dli.indid
					WHERE ind.indid='".$ic."' ORDER BY dli.dtiordem";
			
			$tipos = $db->carregar($sql);
			
			if($tipos[0]) {
				
				foreach($tipos as $tp) {
					
					$nomecoluna = $tp['tpdcampo']."_".$tp['dtiid'];
					
					$_arrSQL['d1'][] = "CASE WHEN di.dtiid=".$tp['dtiid']." THEN COALESCE({$tp['tpdcampo']},0) ELSE 0 END as {$nomecoluna}";
					$_arrSQL['d2'][] = "SUM({$nomecoluna}) as {$nomecoluna}";
					$_arrSQL['d3'][] = $nomecoluna;
					$_agp_relatorio[] = $nomecoluna;
					array_push( $coluna, array("campo" 	  => $nomecoluna,
			   								   "label" 	  => $tp['dtidsc'],
											   "type"	  => $tp['tpdtipo']
			   		   						   ) );
				}
			}
		}
	}

	$sql = montaSQL($_arrSQL);
	
	$agrupador = agp_relatorio($_agp_relatorio);
	
	$dados = $db->carregar( $sql );
	
	$rel = new montaRelatorio();
	$rel->setColuna($coluna);
	$rel->setTolizadorLinha(true);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setTotalizador(true);
	$rel->setAgrupador($agrupador, $dados);
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
}



function carregaParametro()
{
	global $db;
	
	$sql = "select
				tpdcampotema
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = {$_GET['parametro']}";
	$tpdcampotema = $db->pegaUm($sql);
	
	if($_GET['parametro'] == 1){
		$tpdcampotema = "munpopulacao";
	}
	
	switch($tpdcampotema)
	{
		case "tmdboleano":
			$coalesce = "false";
			$whereCampo = "";
		break;
		case "tmdtexto":
			$coalesce = "'N/A'";
			$whereCampo = "";
		break;
		default:
			$coalesce = "0";
			$whereCampo = " and $tpdcampotema >= 0 ";
	}
	
	$sql = "select 
				parametro,
				count(1) as qtde
				
			from (
					select 
						mun.muncod,
						COALESCE($tpdcampotema,$coalesce) as parametro
					from 
						territorios.municipio mun 
					left join
						mapa.temadado pro ON mun.muncod = pro.muncod $whereCampo and pro.tmaid = {$_GET['parametro']}
				  ) as foo 
			group by 
				parametro
				
			order by 
				1";
	//dbg($sql);
	$arrDados = $db->carregar($sql);
	
	$sqlChave = "select sum(qtde) - 10 from ( 
											select 
												parametro,
												count(1) as qtde
											from (
													select 
														mun.muncod,
														COALESCE($tpdcampotema,$coalesce) as parametro
														
													from 
														territorios.municipio mun 
													left join
														mapa.temadado pro ON mun.muncod = pro.muncod $whereCampo and pro.tmaid = {$_GET['parametro']}
												  ) as foo 
											group by 
												parametro
												
											order by 
												1 ) as foo2";
	
	if($arrDados){
		$numLegenda = 5;
		$valorChave = 5565;
		//$valorChave = 1113;
		(float)$num = 0; //incia o número de soma acumulada igual a zero
		$arrSteps = array();
		foreach($arrDados as $linha => $dado){
			if(count($arrSteps) < ($valorChave/$numLegenda)){//apenas 5 legendas
				if($dado['qtde'] >= $valorChave){//se a qtde for maior que a chave
					$arrSteps[] = array("inicio" => $dado['parametro'], "fim" => $dado['parametro']);
					(float)$valorChave = $db->pegaUm(str_replace("{valorChave}",$dado['qtde'],$sqlChave)); //toral sem o número
				}else{ //se não for maior que a chave
					$arrLeg[] = $dado['parametro']; //grava o parametro
					$num+=(float)$dado['qtde']; //soma o numero de qtde
					if($numLegenda-count($arrSteps)> 0 && $num >= ($valorChave/($numLegenda-count($arrSteps))) ){ //se o numero form maior que a chave
						$arrSteps[] = array("inicio" => $arrLeg[0], "fim" => $arrLeg[count($arrLeg)-1]); //atribui o inicio e o fim da legenda
						unset($arrLeg); //reinicia o array de legendas
					}	
				}
			}
		}
	}
	
	if($tpdcampotema == "tmdboleano"){
		
	}
	
	
	$arrD = $db->pegaLinha("select tpdid,tmacor from mapa.tema where tmaid = {$_GET['parametro']}");
	$arrCores[$_GET['parametro']] = $arrD['tmacor'];
	$tpdid = $arrD['tpdid'];
	
	//percent
	$sql = "select tpdid from mapa.tema where tmaid = {$_GET['parametro']} ";
	$tpdid = $db->pegaUm($sql);
	if($tpdid == 4){
		$percent = true;
	}else{
		$percent = false;
	}
		
	$i = 1;
	if($arrSteps){
		$arrJS[] = "arrIncio['{$_GET['parametro']}'] = false; arrIncio['{$_GET['parametro']}'] = new Array(); ";
		$arrJS[] = "arrFim['{$_GET['parametro']}'] = false; arrFim['{$_GET['parametro']}'] = new Array(); ";
		foreach($arrSteps as $i => $range){
			if($range['inicio'] >= 0 && $range['fim'] >= 0){
				if($range['inicio'] == $range['fim']){
					if($tpdcampotema == "tmdboleano"){
						echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".($range['inicio'] == "t" ? "Sim" : "Não")."</div> </div> ";
					}else{
						echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".($percent ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))."</div> </div> ";
					}
				}else{
					echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".($percent ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))." - ".($percent ?number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.'))."</div> </div> ";	
				}
				$arrJS[] = "var cor = retornarCor('#dddddd', '".$arrCores[$_GET['parametro']]."', 5 , ".($i+1)." ); document.getElementById('div_range_".($i+1)."').style.background=cor; document.getElementById('div_range_".($i+1)."').setAttribute(\"class\",cor);";
				$arrJS[] = "arrIncio['{$_GET['parametro']}'][$i] = ".(strstr($_GET['parametro'],"percent") ? $range['inicio'] : round($range['inicio'],0) ).";";
				$arrJS[] = "arrFim['{$_GET['parametro']}'][$i] = ".(strstr($_GET['parametro'],"percent") ? $range['fim'] : round($range['fim'],0) )."; ";
			}
		}
	}
?>
	<input type="button" onclick="carregarParametroMapa('<?php echo $_GET['parametro'] ?>');carregarParametroGrafico('<?php echo $_GET['parametro'] ?>')" name="btn_parametro" value="Carregar" />
	<script type="text/javascript">
	<?php if($arrJS): ?>
		<?php foreach($arrJS as $js): ?>
			<?php echo $js; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	</script> <?php
}



function mapaFiltros() {
	global $db;
?>
<style>
select.CampoEstilo {
    width: 150px;
}
</style>
<script>

//function linkPoloReg(reg) {
//	window.open('http://www.uab.capes.gov.br/sisuab/PesquisasUAB/gestao/ConsultaPolos.action?regiao='+reg,'UAB','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
//}

//function linkPoloEst(est) {
//	window.open('http://www.uab.capes.gov.br/sisuab/PesquisasUAB/gestao/ConsultaPolos.action?uf='+est,'UAB','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
//}

var arrIncio = new Array();
var arrFim = new Array();

jQuery(function() {
	 jQuery('[name="combo_parametro"]').change(function() {
	 	var parametro = jQuery(this).val();
	 	if(parametro)
	 	{
	 		exibeParametroMapa(parametro);
	 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
	 	}else{
	 		jQuery("#span_parametro").html("");
	 		jQuery("#div_parametro").html("");
	 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
	 	}
	 });
});

function exibeParametroMapa(parametro)
{
	jQuery("#span_parametro").html(jQuery('[name="combo_parametro"] option:selected').text());
 	var url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=carregaParametro&parametro=" + parametro;
 	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#div_parametro").html(response);
			}
		});
}

html_municipio = "<div style=\"padding:5px\" ><iframe src=\"painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalao&muncod={muncod}&mapid=<?=$_SESSION['painel_vars']['mapid'] ?>\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"500px\" ></iframe></div>";

function mostrar_painel(painel) {
	if(document.getElementById(painel).style.display == "none") {
		document.getElementById("img_"+painel).src="../imagens/menos.gif";
		document.getElementById(painel).style.display = "";
	} else {
		document.getElementById("img_"+painel).src="../imagens/mais.gif";
		document.getElementById(painel).style.display = "none";
	}
}

function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
}

function carregarParametroMapa(parametro)
{
	
	divCarregando();
	
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	var arrCores = new Array();
	
	var url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=carregarParametroMapa&parametro=" + parametro + "&estuf=" + arrUF;	
	jQuery.ajax({
		type: "POST",
		url: url,
		async: false,
		dataType:'JSON',
		success: function(response){
			response = jQuery.parseJSON(response);
			jQuery.each(response,function(index,item){
			   	if(nomePoli[item.muncod]){
			   		
			   		if(item.tipoparametro != 3){
				   		var posicao = retornaPosicaoMunicipio(item.parametro,item.tipoparametro);
				   		var cor = '';
						if(posicao)
						{
							cor = '' + retornarCor('#dddddd', item.itemcor , 5 , posicao );
						}
				   		f_mudacor(item.muncod,cor);
				   	}else{
				   		if(item.parametro == "f"){
				   			cor = '' + retornarCor('#dddddd', item.itemcor , 5 , 1 );
				   		}else{
				   			cor = '' + retornarCor('#dddddd', item.itemcor , 5 , 2 );
				   		}
				   		f_mudacor(item.muncod,cor);
				   	}
			   	}
			});
			divCarregado();
		}
	});
}


function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
    }

<?
$sql = "SELECT * FROM mapa.indicador WHERE mapid='".$_SESSION['painel_vars']['mapid']."' ORDER BY indid";
$indicadores = $db->carregar($sql);
?>

function carregarMapasPontos() {
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );
	
	var filtros='';

	filtros += '&gtmid='+jQuery('#gtmid').val();
	filtros += '&'+jQuery('#estuf').serialize();
	filtros += '&'+jQuery('#muncod').serialize();
	filtros += '&'+jQuery('#agrupador').serialize();
	
	if ( !document.getElementById( 'agrupador' ).options.length ){
		alert( 'Favor selecionar ao menos um agrupador!' );
		return false;
	}

	
	if(jQuery("[name^='icone']:checked").length>0) {
		filtros += '&'+jQuery("[name^='icone']").serialize();
	} else {
		alert('Selecione um filtro');
		return false;
	}
	
	if(jQuery("[name^='cond[']").length > 0) {
		filtros += '&contexto='+jQuery('#contexto').val();
		filtros += '&'+jQuery("[name^='cond[']").serialize();
	}
	
	if(jQuery('#filt_populacao').val()) {
		if(jQuery('#filt_populacao').val()=='entre') {
			if(jQuery('#ent_populacao1').val()=='' || jQuery('#ent_populacao2').val()=='') {
				alert('Digite o intervalo da população');
				return false;
			}
		} else if(jQuery('#qtd_populacao').val()=='') {
			alert('Digite a população');
			return false;
		}
		filtros += '&filt_populacao='+jQuery('#filt_populacao').val();
		filtros += '&qtd_populacao='+jQuery('#qtd_populacao').val();
		filtros += '&ent_populacao1='+jQuery('#ent_populacao1').val()+'&ent_populacao2='+jQuery('#ent_populacao2').val();
	}
	
	if(jQuery('#cenario').val()) {
		filtros += '&cenario='+jQuery('#cenario').val();
	}
	
	divCarregando();
	
	<?
	$sql = "SELECT estuf FROM territorios.estado";
	$estufs = $db->carregarColuna($sql);
	?>
	
	<? if($estufs) : ?>
	<? foreach($estufs as $estuf) : ?>
	montarPoligonoEstado('<?=$estuf ?>');
	<? endforeach; ?>
	<? endif; ?>
	
	
	<? if($indicadores[0]): ?>
	<? foreach($indicadores as $indicador) : ?>
	if(pontoMarcadores['<?=$indicador['indicone'] ?>']) {
	deleteOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);
	}
	<? endforeach; ?>
	<? endif; ?>
	
	preencherRelatorio("painel.php?modulo=principal/mapas/mapaETEC&acao=A&requisicao=gerarRelatorio"+filtros);
	marcarPontos("painel.php?modulo=principal/mapas/mapaETEC&acao=A&requisicao=gerarXml"+filtros);
	divCarregado();

}

function selecionaPopulacao(pop) {
	if(pop == 'entre') {
		document.getElementById('qtd_populacao').value='';
		document.getElementById('div_qtd_populacao').style.display='none';
		document.getElementById('div_ent_populacao').style.display='';
	} else {
		document.getElementById('ent_populacao1').value='';
		document.getElementById('ent_populacao2').value='';
		document.getElementById('div_ent_populacao').style.display='none';
		document.getElementById('div_qtd_populacao').style.display='';
	}
}


function e(estuf) {
	centraliza(6,centroPoliEstado[estuf]);
}

function mm(lat,lng,muncod) {
	var myLatLng = new google.maps.LatLng(lat, lng);
	var contentString = replaceAll(html_municipio,"{muncod}",muncod);
  	infowindow = new google.maps.InfoWindow();
  	infowindow.setContent(contentString);
  	infowindow.setPosition(myLatLng);
  	infowindow.open(map);
}

function relatorio_etec() {
	window.open('painel.php?modulo=principal/mapas/mapaETEC&acao=A&requisicao=relatorioETEC','ETEC','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

//function relatorio_docentes() {
//	window.open('painel.php?modulo=principal/mapas/mapaETEC&acao=A&requisicao=relatorioDocentes','Docentes','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
//}

</script>

<?
$sql = "SELECT * FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'";
$dadosmapa = $db->pegaLinha($sql);

$sql = "SELECT mpcid as codigo, mpcdsc as descricao FROM mapa.mapacenario WHERE mpcstatus='A' AND mapid='".$_SESSION['painel_vars']['mapid']."' order by 2";
$dadoscenario = $db->carregar($sql);
?>
<!--<p align="center"><a style="cursor:pointer;" onclick="relatorio_etec();"><b>Relatório E-Tec</b></a></p>-->
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<? if($dadoscenario[0]) : ?>
	<tr>
		<td class="SubTituloDireita">Cenário:</td>
		<td colspan="2"><? $db->monta_combo('cenario', $dadoscenario, 'S', '', '', '', '', '', 'N', 'cenario'); ?></td>
	</tr>
	<? endif; ?>
	<?
	if($indicadores[0]) {
		foreach($indicadores as $indicador) {
			?>
			<tr>
				<td><input type="checkbox" name="icone[]" onclick="if(this.checked){showOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);}else{clearOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);}"  id="id_<?=$indicador['indid'] ?>" value="<?=$indicador['indid'] ?>"  /></td>
				<? if($dadosmapa['mapcontexto']=="t") : ?>
				<td><? $db->monta_combo('cond['.$indicador['indid'].']', array(0 => array('codigo'=>'com','descricao'=>'Com'), 1 => array('codigo'=>'sem','descricao'=>'Sem')), 'S', '', '', '', '', '', 'N', ''); ?></td>
				<? endif; ?>
				<td><img src="<?=$indicador['indicone'] ?>"></td>
				<td><font size=1><?=$indicador['inddsc'] ?></font></td>
			</tr>
			<?
		}
	}
	?>
	
	<? if($dadosmapa['mapcontexto']=="t") : ?>
	<tr>
	<td colspan="2" class="SubTituloDireita">Contexto:</td>
	<td colspan="2"><?
	$db->monta_combo('contexto', array(0 => array('codigo'=>'OR','descricao'=>'OU'), 1 => array('codigo'=>'AND','descricao'=>'E')), 'S', '', '', '', '', '', 'N', 'contexto'); 
	?></td>
	</tr>
	<? endif; ?>

</table>
<br>
<br>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_gerais" onclick="mostrar_painel('gerais');" border=0> Outros Filtros
	</td>
</tr>
<tr>
	<td>
		<div id="gerais" style="display:none">
		<table align="center" border="0" class="listagem" cellpadding="0" cellspacing="0" width="100%">
		<tr>
			<td class="SubTituloDireita" width="10%">População:</td>
			<td width="10%">
			<?
			$opts = Array(Array('codigo'=>'>' ,'descricao'=>'Maior que'),
						  Array('codigo'=>'>=','descricao'=>'Maior Igual a'),
						  Array('codigo'=>'=' ,'descricao'=>'Igual a'),
						  Array('codigo'=>'<' ,'descricao'=>'Menor que'),
						  Array('codigo'=>'<=','descricao'=>'Menor igual a'),
						  Array('codigo'=>'entre','descricao'=>'Entre'));	
			
						   
			$db->monta_combo('filtro[populacao]', $opts, 'S', 'Selecione', 'selecionaPopulacao', '', '', '', 'N', 'filt_populacao');
			?>
			</td>
			<td>
			<div id="div_qtd_populacao"><? echo campo_texto('qtd_populacao', 'N', 'S', '', 10, 12, "##########", "", '', '', 0, 'id="qtd_populacao"' ); ?></div>
			<div id="div_ent_populacao" style="display:none;"><? echo campo_texto('ent_populacao1', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao1"' ); ?> à <? echo campo_texto('ent_populacao2', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao2"' ); ?></div>
			</td>
		</tr>
		</table>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_uf" onclick="mostrar_painel('uf');" border=0> UF
	</td>
</tr>
<tr>
	<td>
		<div id="uf" style="display:none">
			<?php
			$sql = "	SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM 
							territorios.estado
						ORDER BY
							estdescricao ";

			combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 240, '', '' );
			?>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_municipio" onclick="mostrar_painel('municipio');" border=0> Município
	</td>
</tr>
<tr>
	<td>
		<div id="municipio" style="display:none">
			<?php
			$sql = " 	SELECT	
							muncod AS codigo,
							mundescricao AS descricao
						FROM 
							territorios.municipio
						ORDER BY
							mundescricao";

			combo_popup( 'muncod', $sql, 'Selecione os Municípios', '400x400', 0, array(), '', 'S', false, false, 5, 240);							?>
		</div>
	</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" ><img style="cursor:pointer" id="img_agrup" onclick="mostrar_painel('agrup');" src="/imagens/mais.gif"> Agrupadores</td>
</tr>
<tr>
	<td>
	<div style="display:none" id="agrup">
	<p>Grupo de municípios: <?
	$gtmid=6;
	$sql = "SELECT gtmid as codigo, gtmdsc as descricao FROM territorios.grupotipomunicipio WHERE gtmstatus='A' ORDER BY gtmdsc"; 
	$db->monta_combo('gtmid', $sql, 'S', '', '', '', '', '200', 'N', 'gtmid','',$gtmid); 
	?></p>

		<?
			// Início dos agrupadores
			$agrupador = new Agrupador('filtros','');
			
			// Dados padrão de destino (nulo)
			$destino = array(
				'regiao' => array(
											'codigo'    => 'regiao',
											'descricao' => 'Região'
				),
				'uf' => array(
											'codigo'    => 'uf',
											'descricao' => 'UF'
				),
				'municipio' => array(
											'codigo'    => 'municipio',
											'descricao' => 'Município'
				)
			);
			
			// Dados padrão de origem
			$origem = array(
				'grupo' => array(
											'codigo'    => 'grupo',
											'descricao' => 'Grupo'
				)
				
			);
			
			// exibe agrupador
			$agrupador->setOrigem( 'naoColunas', null, $origem );
			$agrupador->setDestino( 'agrupador', null, $destino );
			$agrupador->exibir();
	
	?> 
	</div>
	</td>
</tr>
</table>
<p>
<input type="button" name="carregar" value="Carregar" onclick="carregarMapasPontos();">
</p>
<?
}

function mapaTemas()
{
	global $db;
	
	$sql = "select
				tma.tmaid as codigo,
				tma.tmadsc as descricao
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
	
?>
<table class="tabela" >
	<tr>
		<td class="SubtituloDireita" width="25%" >Tema:</td>
		<td><?php $db->monta_combo("combo_parametro",$sql,"S","Selecione...","","","","200px","") ?></td>
	</tr>
</table>
<br />
<b><span id="span_parametro" ></span></b><br /><br />
<div id="div_parametro" ></div>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<div id="grafico_parametro" >
	<div id="grafico_mapa" ></div>
</div>
<?php } ?>
<script type="text/javascript">

function retornaPosicaoMunicipio(deficit,parametro)
{
		
	var posicao = '';
	deficit = deficit*1;
	if(deficit <= 0){
	  		posicao = 1;
	  	}
	  	
	jQuery.each(arrIncio[parametro],function(key,value) { 
	  	if(deficit >= arrIncio[parametro][key] && deficit <= arrFim[parametro][key]){
	  		posicao =  key + 1;
	  	}
	});	
	
	if(!posicao)
	{
		return false;
	}else
	{
		return posicao;
	}
}

function carregarParametroGrafico(parametro)
{
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	var parametroCor = new Array();
	var i = 1
	jQuery.each(jQuery("[id^='div_range']"),function(index,item){
	    var cor = jQuery(this).attr("class");
	    cor = cor.replace("#","");
	    var id = jQuery(this).attr("id");
	    id = id.replace("div_range_","");
	    parametroCor[i] = cor;
	    i++;
	});
	
	var params = ";parametro=" + parametro + ";estuf=" + arrUF + ';cor=' + parametroCor;
	
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_mapa", "320", "240", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipoMapas=mapas" + params,"loading":"Carregando gráfico..."} );
}

function callBack()
{
	
}
</script>
<?php
include APPRAIZ . 'includes/maps/maps.inc';
?>