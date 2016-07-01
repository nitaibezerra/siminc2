<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao'])
{
	ob_clean();
	$_REQUEST['requisicao']();
	die;
}

// monta cabeçalho 
if(!$sem_cabecalho) {
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}


function gerarXml() {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	
	$sql = montaSQL();
	$dados = $db->carregar($sql);
	ob_clean();
		
	if($dados):
		
		$conteudo .= "<markers>"; // inicia o XML
		
		foreach($dados as $d):
										
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "info=\"". $d['info'] ."\" ";
			$conteudo .= "lat=\"". $d['lat'] ."\" ";
			$conteudo .= "lng=\"". $d['lng'] ."\" ";
			$conteudo .= "icon=\"". $d['icon'] ."\" ";
			$conteudo .= "/>";
		
		endforeach;
		
		$conteudo .= "</markers>";
			header ("Content-Type:text/xml");
		print $conteudo;
		
	endif;
}

function montaBalao() {
	global $db;
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$mundados = $db->pegaLinha("SELECT mundescricao||' - '||estuf as cabecalhomun, munpopulacao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
	$dadoproinfancia = $db->pegaLinha("SELECT * FROM painel.dadoproinfancia WHERE trim(muncod)='".$_REQUEST['muncod']."'");
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro colspan=2 style=cursor:pointer; onclick=window.open('?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod=".$_REQUEST['muncod']."','Painel','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');>".$mundados['cabecalhomun']." (Grupo ".$dadoproinfancia['dpitipogrupo'].")</td></tr>";
 
	echo "<tr><td valign=top><table align=center border=0 class=tabela cellpadding=3 cellspacing=1>";
	echo "<tr><td class=SubTituloCentro colspan=2>Dados Demográficos</td></tr>";
	echo "<tr><td class=SubTituloDireita>População Total</td><td align=right>".number_format($mundados['munpopulacao'],0,",",".")."</td></tr>";
	
	echo "<tr><td class=SubTituloDireita>População de 0 a 3</td><td align=right>".number_format($dadoproinfancia['dpipopulacao0_3'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Atendimento de creche</td><td align=right>".number_format($dadoproinfancia['dpimatricula0_3'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Déficit de 0 a 3</td><td align=right>".number_format($dadoproinfancia['dpideficit0_3'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>% Déficit de 0 a 3</td><td align=right>".(($dadoproinfancia['dpipercentualdeficit0_3']>0)?number_format($dadoproinfancia['dpipercentualdeficit0_3'],1,",","."):0)."%</td></tr>";
	
	echo "<tr><td class=SubTituloDireita>População de 4 a 5</td><td align=right>".number_format($dadoproinfancia['dpipopulacao4_5'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Atendimento de pré-escola</td><td align=right>".number_format($dadoproinfancia['dpimatricula4_5'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Déficit de 4 a 5</td><td align=right>".number_format($dadoproinfancia['dpideficit4_5'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>% Déficit de 4 a 5</td><td align=right>".(($dadoproinfancia['dpipercentualdeficit4_5']>0)?number_format($dadoproinfancia['dpipercentualdeficit4_5'],1,",","."):0)."%</td></tr>";
	
	echo "<tr><td class=SubTituloDireita>População de 0 a 5</td><td align=right>".number_format($dadoproinfancia['dpipopulacao0_5'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Atendimento de escola infantil</td><td align=right>".number_format($dadoproinfancia['dpimatricula0_5'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Déficit de 0 a 5</td><td align=right>".number_format($dadoproinfancia['dpideficit0_5'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>% Déficit de 0 a 5</td><td align=right>".(($dadoproinfancia['dpipercentualdeficit0_5']>0)?number_format($dadoproinfancia['dpipercentualdeficit0_5'],1,",","."):0)."%</td></tr>";
			
	echo "</table></td>";
			
	echo "<td valign=top><table align=center border=0 class=tabela cellpadding=3 cellspacing=1>";
	echo "<tr><td class=SubTituloCentro colspan=2>Unidades de creches</td></tr>";
	echo "<tr><td class=SubTituloDireita>Déficit Global de Unidades</td><td align=right>".number_format($dadoproinfancia['dpideficitunidade'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Unidades Total PAC</td><td align=right>".number_format($dadoproinfancia['dpiunidadetotal'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Creches Aprovadas Pré-PAC2</td><td align=right>".number_format($dadoproinfancia['dpiquantidadecrechepre'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Creches Aprovadas PAC2</td><td align=right>".number_format($dadoproinfancia['dpiquantidadecrechepac'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Creches Propostas 2012-2014</td><td align=right>".number_format($dadoproinfancia['dpiquantidadeprevista2012_14'],0,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Creches - MCMV</td><td align=right>".number_format($dadoproinfancia['dpimcmv'],0,",",".")."</td></tr>";	
	echo "</table>";
	
	echo "<table align=center border=0 class=tabela cellpadding=3 cellspacing=1>";
	echo "<tr><td class=SubTituloCentro colspan=2>Dados do município</td></tr>";
	echo "<tr><td class=SubTituloDireita>% Pobreza</td><td align=right>".number_format($dadoproinfancia['dpipercentualpobreza'],1,",",".")."%</td></tr>";
	echo "<tr><td class=SubTituloDireita>IDEB Anos iniciais do EF 2007</td><td align=right>".number_format($dadoproinfancia['dpiideb2007inicial'],1,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>IDEB Anos iniciais do EF 2009</td><td align=right>".number_format($dadoproinfancia['dpiideb2009inicial'],1,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>IDEB Anos finais do EF 2007</td><td align=right>".number_format($dadoproinfancia['dpiideb2007final'],1,",",".")."</td></tr>";
	echo "<tr><td class=SubTituloDireita>IDEB Anos finais do EF 2009</td><td align=right>".number_format($dadoproinfancia['dpiideb2009final'],1,",",".")."</td></tr>";
	
	echo "</table>";
	
	echo "</td></tr>";
	
	echo "<tr><td class=SubTituloEsquerda colspan=2>Creches Aprovadas PAC2</td></tr>";
	
	$sql = "SELECT upper(predescricao) as predescricao, preid FROM obras.preobra p 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE p.muncod='".$_REQUEST['muncod']."' AND d.esdid='228' AND p.ptoid!=5 AND p.tooid=1";
	
	$obras = $db->carregar($sql);
	
	if($obras[0]) {
		foreach($obras as $obra) {
			echo "<tr><td colspan=2>".$obra['predescricao']."</td></tr>";
			$sql = "SELECT '<img title='||p.pofdescricao||' border=1px; id='||p.arqid||' src=../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid='||p.arqid||'&_sisarquivo=obras hspace=10 vspace=3 style=position:relative;z-index:5;float:left;width:70px; height:70px; onmouseover=\"return escape( \''||p.pofdescricao||'\' );\" onclick=\"javascript:window.open(\'../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&preid={$obra['preid']}\',\'imagem\',\'width=850,height=600,resizable=yes\')\">' as fot,
						   to_char(a.arqdata, 'DD/MM/YYYY') as data  
					FROM obras.preobrafotos p
					INNER JOIN public.arquivo a ON a.arqid = p.arqid 
					WHERE p.preid='".$obra['preid']."' ORDER BY a.arqdata DESC LIMIT 5";
			$fotos = $db->carregar($sql);
			if($fotos[0]) {
				echo "<tr><td colspan=2>";
				echo "<table>";
				foreach($fotos as $foto) {
					echo "<td>".$foto['fot']."<br>".$foto['data']."</td>";
				}
				echo "</table>";
				echo "</td></tr>";
			}
		}
	} else {
		echo "<tr><td colspan=2>Não existem obras</td></tr>";
	}
	echo "<tr><td class=SubTituloEsquerda colspan=2>Creches Aprovadas Pré-PAC2</td></tr>";
	
	$sql = "SELECT 
		        oi.obrid as id,
		        upper(oi.obrdesc) as nome 
			FROM obras.obrainfraestrutura AS oi 
			INNER JOIN
			            entidade.entidade AS ee ON oi.entidunidade = ee.entid
			INNER JOIN
			            entidade.endereco AS ed ON oi.endid = ed.endid
			INNER JOIN
			            territorios.municipio AS tm ON ed.muncod = tm.muncod
			INNER JOIN
			            obras.orgao AS oo ON oi.orgid = oo.orgid
			INNER JOIN 
			        obras.situacaoobra  AS st ON oi.stoid = st.stoid
			INNER JOIN 
			        obras.programafonte AS pf ON oi.prfid = pf.prfid
			INNER JOIN
			        obras.tipoobra AS tp ON oi.tobraid = tp.tobaid
			WHERE
			       oi.obsstatus = 'A' 
			       AND ee.entstatus = 'A' 
			       AND pf.prfid = 41 -- Programa Proinfância
			       AND oi.tooid <> 1 -- elimina obras do PAC2
			       AND oi.orgid = 3 -- obras do ensino médio 
			       AND ed.muncod = '".$_REQUEST['muncod']."'
			ORDER BY
			        oi.obrdesc";
	
	
	$obras = $db->carregar($sql);
	
	if($obras[0]) {
		foreach($obras as $obra) {
			echo "<tr><td colspan=2>".$obra['nome']."</td></tr>";
			
			$sql = "SELECT '<img title='||a.arqdescricao||' border=1px; id='||p.arqid||' src=../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid='||p.arqid||'&_sisarquivo=obras hspace=10 vspace=3 style=position:relative;z-index:5;float:left;width:70px; height:70px; onmouseover=\"return escape( \''||a.arqdescricao||'\' );\" onclick=\"javascript:window.open(\'../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obra['id']}\',\'imagem\',\'width=850,height=600,resizable=yes\')\">' as fot,
						   to_char(a.arqdata, 'DD/MM/YYYY') as data  
					FROM obras.arquivosobra p
					INNER JOIN public.arquivo a ON a.arqid = p.arqid 
					WHERE p.obrid='".$obra['id']."' AND p.tpaid=23 
					ORDER BY a.arqdata DESC LIMIT 5";
			$fotos = $db->carregar($sql);
			if($fotos[0]) {
				echo "<tr><td colspan=2>";
				echo "<table><tr>";
				foreach($fotos as $foto) {
					echo "<td>".$foto['fot']."<br>".$foto['data']."</td>";
				}
				echo "</tr></table>";
				echo "</td></tr>";
			}
			
		}
	} else {
		echo "<tr><td colspan=2>Não existem obras</td></tr>";		
	}

	echo "</table>";
}

function montaSQL() {
	
	ob_clean();
	
	$filtro[] = "m.munlatlong IS NOT NULL";
	
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "m.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "m.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	if( $_REQUEST['semcreche'] ){
		$filtroesp[] = "(dpiquantidadecrechepac+dpiquantidadecrechepre+dpiquantidadeprevista2012_14)=0";
	}
	
	if( $_REQUEST['territorioscidadania'] ){
		$filtro[] = "dpiterritoriocidadania=TRUE";
	}
	if( $_REQUEST['grupo'] ){
		$filtro[] = "dpitipogrupo IN('".implode("','",$_REQUEST['grupo'])."')";
	}
	
	if( $_REQUEST['icone'][0] ){
		foreach($_REQUEST['icone'] as $ic) {
			if($_REQUEST['cond'][$ic]=="todos") $cond = ">0";
			else $cond = "=0";
			$filtroesp[] = "d.".$ic.$cond;
		}
	}
	
	if($filtro || $filtroesp) {
		$cl = "WHERE ";
		
		if($filtro) {
			$cls[] = implode($filtro," AND ");
		}
		
		if($filtroesp) {
			$cls[] = "(".implode($filtroesp," ".$_REQUEST['contexto']." ").")";
		}
		
		$cl .= implode(" AND ", $cls);
		
		
	}

    $sql = "select  m.muncod,
                    r.regdescricao as regiao,
                    e.estuf as uf,
                    'Grupo '||dpitipogrupo as grupo,
                    '<span style=cursor:pointer onclick=localizaMapa2(\''||m.muncod||'\',\''||ST_X(m.munlatlong)||'\',\''||ST_Y(m.munlatlong)||'\')>'||m.mundescricao||'</span>' as municipio,
                    dpipopulacao0_5 as popul,
                    dpimatricula0_3 mat0_3,
                    dpimatricula4_5 mat4_5,
                    dpipopulacao0_3,
                    ((dpipopulacao0_3/2) - dpimatricula0_3)::numeric(6) deficitcreche,
                    dpipopulacao4_5,
                    (dpipopulacao4_5) - dpimatricula4_5 deficitpreescola,
                    dpiquantidadeprevista2012_14 + dpiquantidadecrechepac as totalcreches,
                    dpiquantidadecrechepre ,
                    dpiquantidadecrechepac ,
                    dpicrecheconcluida,
                    ST_X(m.munlatlong) as lng,
                    ST_Y(m.munlatlong) as lat,
                    '&lt;iframe class=iframeBalao src=?modulo=principal/mapas/mapaProInfanciaEEacao=AEErequisicao=montaBalaoEEmuncod='||m.muncod||' &gt;&lt;/iframe&gt;' as info,
                    '/imagens/tachinha_b.png' as icon
            FROM painel.dadoproinfancia d
            INNER JOIN territoriosgeo.municipio m on m.muncod = d.muncod
            INNER JOIN territoriosgeo.estado e on e.estuf = m.estuf
            INNER JOIN territoriosgeo.regiao r on r.regcod = e.regcod
            {$cl} ORDER BY ".implode(",",$_REQUEST['agrupador']);
	//dbg($sql,1);
	return $sql;
}


function agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("popul",
										  "mat0_3",
										  "mat4_5",
										  "deficitcreche",
										  "deficitpreescola",
										  "dpiquantidadecrechepre",
										  "dpiquantidadecrechepac",
										  "dpicrecheconcluida"),
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
	
	switch($_GET['parametro'])
	{
		case "populacao":
			$sql = "select 
						munpopulacao as parametro,
						muncod,
						'{$_GET['parametro']}' as tipoparametro
					from 
						territorios.municipio mun
					where
						1=1
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
		break;
		default:
			$sql = "select 
						mun.muncod,
						{$_GET['parametro']} as parametro, 
						'{$_GET['parametro']}' as tipoparametro 
					from 
						painel.dadoproinfancia pro
					inner join
						territorios.municipio mun ON mun.muncod = pro.muncod
					where
						1=1
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
		break;
	}
	$dados = $db->carregar($sql);
	echo JSON_encode($dados);
}

function gerarRelatorio() {
	global $db;
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql = montaSQL();

	$agrupador = agp_relatorio();
	$dados = $db->carregar( $sql );

	$coluna = array();
	array_push( $coluna, array("campo" 	  => "popul",
					   		   "label" 	  => "Pop. 0-5",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "mat0_3",
					   		   "label" 	  => "Creche",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "mat4_5",
					   		   "label" 	  => "Pré-escola",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "deficitcreche",
					   		   "label" 	  => "Creche",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "deficitpreescola",
					   		   "label" 	  => "Pré-Escola",
					   		   "type"	  => "numeric") );
    array_push( $coluna, array("campo" 	  => "dpiquantidadecrechepre",
					   		   "label" 	  => "Pré-PAC",
					   		   "type"	  => "numeric") );
    array_push( $coluna, array("campo" 	  => "dpiquantidadecrechepac",
					   		   "label" 	  => "PAC",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "dpicrecheconcluida",
					   		   "label" 	  => "Concluídas",
					   		   "type"	  => "numeric") );
	
	$coltop = array(0 => array( "label" => "","rowspan" => 2 ),
					1 => array( "label" => "Matriculas" , "colspan" => 2 ),
					2 => array( "label" => "Déficit de Atendimento" , "colspan" => 2 ),
					3 => array( "label" => "Unidades de Educação Infantil Pactuadas" , "colspan" => 3 ),

				);
	$rel = new montaRelatorio();
	$rel->setColuna($coluna);
	$rel->setColunaTop($coltop);
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
	
	switch($_GET['parametro']){
		case "populacao":
			$arrSteps[] = array( "inicio" => 0 , "fim" => 10000 );
			$arrSteps[] = array( "inicio" => 10001 , "fim" => 50000 );
			$arrSteps[] = array( "inicio" => 50001 , "fim" => 100000 );
			$arrSteps[] = array( "inicio" => 100001 , "fim" => 200000 );
			$arrSteps[] = array( "inicio" => 200001 , "fim" => 11500000 );
		break;
		default:
			$sql = "select 
						parametro,
						count(1) as qtde
					from (
							select 
								mun.muncod,
								{$_GET['parametro']} as parametro
							from 
								painel.dadoproinfancia pro
							inner join
								territoriosgeo.municipio mun ON mun.muncod = pro.muncod
							where
								{$_GET['parametro']} >= 0
						  ) as foo 
					group by 
						parametro
					order by 
						1";

			$arrDados = $db->carregar($sql);
			
			$sqlChave = "select sum(qtde) - {valorChave} from (
							select 
									parametro,
									count(1) as qtde
								from (
										select 
											mun.muncod,
											{$_GET['parametro']} as parametro
										from 
											painel.dadoproinfancia pro
										inner join
											territoriosgeo.municipio mun ON mun.muncod = pro.muncod
										where
											{$_GET['parametro']} >= 0
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
							if($num >= ($valorChave/($numLegenda-count($arrSteps))) ){ //se o numero form maior que a chave
								$arrSteps[] = array("inicio" => $arrLeg[0], "fim" => $arrLeg[count($arrLeg)-1]); //atribui o inicio e o fim da legenda
								unset($arrLeg); //reinicia o array de legendas
							}	
						}
					}
				}
			}
		break;
	}
	
	$arrCores["dpiquantidadecrechepac"] = "#0000FF";
	$arrCores["dpiquantidadecrechepre"] = "#0000FF";
	$arrCores["dpideficitunidade"] = "#FF0000";
	$arrCores["dpipercentualdeficit"] = "#FF0000";
	$arrCores["dpipercentualpobreza"] = "#FF0000";
	$arrCores["populacao"] = "#FF0000";
	$arrCores["dpiquantidadeprevista2012"] = "#0000FF";
	$arrCores["dpiquantidadeprevista2013_14"] = "#0000FF";
	$arrCores["dpiquantidadeprevista2012_14"] = "#0000FF";
	$arrCores["dpimcmv"] = "#0000FF";
	
	$i = 1;
	if($arrSteps){
		$arrJS[] = "arrIncio['{$_GET['parametro']}'] = false; arrIncio['{$_GET['parametro']}'] = new Array(); ";
		$arrJS[] = "arrFim['{$_GET['parametro']}'] = false; arrFim['{$_GET['parametro']}'] = new Array(); ";
		foreach($arrSteps as $i => $range){
			if($range['inicio'] >= 0 && $range['fim'] >= 0){
				if($range['inicio'] == $range['fim']){
					echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".(strstr($_GET['parametro'],"percent") ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))."</div> </div> ";
				}else{
					echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".(strstr($_GET['parametro'],"percent") ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))." - ".(strstr($_GET['parametro'],"percent") ?number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.'))."</div> </div> ";	
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
<script>
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
	jQuery("#div_parametro").html("Carregando...");
 	var url = "?modulo=principal/mapas/mapaProInfancia&acao=A&requisicao=carregaParametro&parametro=" + parametro;
 	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#div_parametro").html(response);
			}
		});
}

html_municipio = "<div style=\"padding:5px\" ><iframe src=\"?modulo=principal/mapas/mapaProInfancia&acao=A&requisicao=montaBalao&muncod={muncod}\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"500px\" ></iframe></div>";

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
	
	arrCores['dpiquantidadecrechepac'] = "#0000FF";
	arrCores['dpiquantidadecrechepre'] = "#0000FF";
	arrCores['dpideficitunidade'] = "#FF0000";
	arrCores['dpipercentualdeficit'] = "#FF0000";
	arrCores['dpipercentualpobreza'] = "#FF0000";
	arrCores['populacao'] = "#FF0000";
	arrCores['dpiquantidadeprevista2012'] = "#0000FF";
	arrCores['dpiquantidadeprevista2013_14'] = "#0000FF";
	arrCores['dpiquantidadeprevista2012_14'] = "#0000FF";
	arrCores['dpimcmv'] = "#0000FF";
		
	var url = "?modulo=principal/mapas/mapaProInfancia&acao=A&requisicao=carregarParametroMapa&parametro=" + parametro + "&estuf=" + arrUF;	
	jQuery.ajax({
		type: "POST",
		url: url,
		async: false,
		dataType:'JSON',
		success: function(response){
			response = jQuery.parseJSON(response);
			jQuery.each(response,function(index,item){
			   	if(nomePoli[item.muncod]){
			   		var posicao = retornaPosicaoMunicipio(item.parametro,item.tipoparametro);
			   		var cor = '';
					if(posicao)
					{
						cor = '' + retornarCor('#dddddd', arrCores[item.tipoparametro] , 5 , posicao );
					}
			   		f_mudacor(item.muncod,cor);
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


function carregarMapasPontos(tipo,carrega) {
	
	if(pontoMarcadores['/imagens/tachinha_b.png']) {
		deleteOverlays(pontoMarcadores['/imagens/tachinha_b.png']);
	}
	
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );
	
	var filtros='';
	filtros += '&contexto='+jQuery('#contexto').val();
	filtros += '&'+jQuery("[name^='cond[']").serialize()
	filtros += '&'+jQuery('#estuf').serialize();
	filtros += '&'+jQuery('#muncod').serialize();
	filtros += '&'+jQuery('#agrupador').serialize();
	if(jQuery("[name^='icone']:checked").length>0) {
		filtros += '&'+jQuery("[name^='icone']").serialize();
	}
	if(jQuery("[name^='grupo[']:checked").length>0) {
		filtros += '&'+jQuery("[name^='grupo[']").serialize();
	}
	if(jQuery("[name^='semcreche']:checked").length>0) {
		filtros += '&semcreche=true';
	}
	if(jQuery("[name^='territorioscidadania']:checked").length>0) {
		filtros += '&territorioscidadania=true';
	}
	
	if(!carrega){
		divCarregando();
	}
	preencherRelatorio("?modulo=principal/mapas/mapaProInfancia&acao=A&requisicao=gerarRelatorio"+filtros);
	marcarPontos("?modulo=principal/mapas/mapaProInfancia&acao=A&requisicao=gerarXml"+filtros);
	divCarregado();

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
	
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_mapa", "320", "240", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipoMapasAntigo=mapas" + params,"loading":"Carregando gráfico..."} );
}

</script>
<?
$_opcontexto = array(0 => array('codigo'=>'todos','descricao'=>'Com'),
					 1 => array('codigo'=>'nenhum','descricao'=>'Sem'));

if($_SESSION['sem_cabecalho']){ ?>

	<input type="hidden" name="cond[dpiquantidadecrechepre]" value="todos" />
	<input type="checkbox" style="display:none" name="icone[]" checked="checked"   id="chk_1" value="dpiquantidadecrechepre"  />
	
	<input type="hidden" name="cond[dpiquantidadecrechepac]" value="todos" />
	<input type="checkbox" style="display:none" name="icone[]" checked="checked"   id="chk_2" value="dpiquantidadecrechepac"  />
	
	<input type="hidden" name="cond[dpiquantidadeprevista2012_14]" value="todos" />
	<input type="checkbox" style="display:none" name="icone[]" checked="checked"   id="chk_3" value="dpiquantidadeprevista2012_14"  />
	
	<input type="checkbox" style="display:none" name="semcreche" id="chk_4" value="semcreche" checked="checked"  />
	
	<input type="hidden" name="cond[dpimcmv]" value="todos" />
	<input type="checkbox" style="display:none" name="icone[]" id="chk_5" value="dpimcmv"  />
	
	<input type="hidden" name="contexto" id="contexto" value="OR" />
	
<?php }else{ ?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td><input type="checkbox" name="icone[]"    id="chk_1" value="dpiquantidadecrechepre"  /></td>
			<td><?
	$db->monta_combo('cond[dpiquantidadecrechepre]', $_opcontexto, 'S', '', '', '', '', '', 'N', '');
	?></td>
			<td><font size=1>1 - Creches Aprovadas Pré PAC 2</font></td>
		</tr>
		
		<tr>
			<td><input type="checkbox" name="icone[]"    id="chk_2" value="dpiquantidadecrechepac"  /></td>
			<td><?
	$db->monta_combo('cond[dpiquantidadecrechepac]', $_opcontexto, 'S', '', '', '', '', '', 'N', '');
	?></td>
			<td><font size=1>2 - Creches Aprovadas PAC 2</font></td>
		</tr>
		<tr>
			<td><input type="checkbox" name="icone[]"    id="chk_3" value="dpiquantidadeprevista2012_14"  /></td>
			<td><?
	$db->monta_combo('cond[dpiquantidadeprevista2012_14]', $_opcontexto, 'S', '', '', '', '', '', 'N', '');
	?></td>
			<td><font size=1>3 - Creches Propostas 2012/14</font></td>
		</tr>
		<tr>
			<td><input type="checkbox" name="semcreche"  id="chk_4" value="semcreche"  /></td>
			<td>-</td>
			<td><font size=1>4 - Município sem creche</font></td>
		</tr>
		<tr>
			<td><input type="checkbox" name="icone[]"    id="chk_5" value="dpimcmv"  /></td>
			<td><?
	$db->monta_combo('cond[dpimcmv]', $_opcontexto, 'S', '', '', '', '', '', 'N', '');
	?></td>
			<td><font size=1>5 - MCMV</font></td>
		</tr>
		<tr>
			<td colspan="3">
			<b>Contexto:</b> <?
			$_opcontexto = array(0 => array('codigo'=>'OR','descricao'=>'OU'),
								 1 => array('codigo'=>'AND','descricao'=>'E'));
			$db->monta_combo('contexto', $_opcontexto, 'S', '', '', '', '', '', 'N', 'contexto');
			?>
			</td>
		</tr>
		
	</table>
	<br>
	<br>
<?php } ?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
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
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_grupomunicipio" onclick="mostrar_painel('grupomunicipio');" border=0> Grupo Município
	</td>
</tr>
<tr>
	<td>
		<div id="grupomunicipio" style="display:none">
		<input type="checkbox" name="grupo[]"    			  id="grp_1" value="1"  /> <font size=1>Grupo 1</font><br/>
		<input type="checkbox" name="grupo[]"    			  id="grp_2" value="2"  /> <font size=1>Grupo 2</font><br/>
		<input type="checkbox" name="grupo[]"    			  id="grp_3" value="3"  /> <font size=1>Grupo 3</font><br/>
		<input type="checkbox" name="territorioscidadania"    id="grp_4" value="territorioscidadania"  /> <font size=1>Territórios da Cidadania</font><br/>
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
	<table width="100%">
	<tr>
			<td>
				<select id="agrupador" name="agrupador[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" class="combo campoEstilo">
				<option value="grupo">Grupo</option>
				<option value="regiao">Região</option>
				<option value="uf">UF</option>
				<option value="municipio">Município</option>
				</select>
			</td>
			<td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( 'agrupador' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( 'agrupador' ) );"/><br/>
			</td>
	</tr>
	<tr>
		<td colspan="2" align="right"><input type="button" name="removeritem" value="Remover" onclick="removeSelectedOptions(document.getElementById('agrupador'));"> <input type="button" name="restauraritens" value="Restaurar" onclick="restaurarItens();"></td>
	</tr>
	</table>
	<input type="hidden" name="hdn_agrup" id="hdn_agrup" value="0" />
	</div>
	</td>
</tr>
</table>
<p>
<input type="button" name="carregar" value="Carregar" onclick="carregarMapasPontos(1);">
<input type="button" name="limpar" value="Limpar pontos" onclick="deleteOverlays(pontoMarcadores['/imagens/tachinha_y.png']);">
</p>
<?
}

function mapaTemas()
{
	global $db;
?>
<?php $arrParametros = array(
							array("codigo" => "dpiquantidadecrechepac", "descricao" => "Creches Aprovadas PAC 2","cor" => "#0000FF"),
							array("codigo" => "dpiquantidadecrechepre", "descricao" => "Creches Aprovadas Pré PAC 2","cor" => "#0000FF"),
							array("codigo" => "dpideficitunidade", "descricao" => "Déficit de Unidade de 0 a 5","cor" => "#FF0000"),
							array("codigo" => "dpipercentualdeficit", "descricao" => "Percentual de Déficit de 0 a 5","cor" => "#FF0000"),
							array("codigo" => "dpipercentualpobreza", "descricao" => "Percentual de Pobreza","cor" => "#FF0000"),
							array("codigo" => "populacao", "descricao" => "População","cor" => "#FF0000"),
							array("codigo" => "dpiquantidadeprevista2012", "descricao" => "Creches Propostas 2012/13","cor" => "#0000FF"),
							array("codigo" => "dpiquantidadeprevista2013_14", "descricao" => "Creches Propostas 2013/14","cor" => "#0000FF"),
							array("codigo" => "dpiquantidadeprevista2012_14", "descricao" => "Creches Propostas 2012/14","cor" => "#0000FF"),
							array("codigo" => "dpimcmv", "descricao" => "Quantidade - MCMV","cor" => "#0000FF")
						  ); 
						  ?>
<table class="tabela" >
	<tr>
		<td class="SubtituloDireita" width="25%" >Tema:</td>
		<td><?php $db->monta_combo("combo_parametro",$arrParametros,"S","Selecione...","","","","200px","") ?></td>
	</tr>
</table>
<br /><br />
<b><span id="span_parametro" ></span></b><br /><br />
<div id="div_parametro" ></div>
<br />
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

function callBack()
{
	var arrUF = new Array();
	var i = 0;
	jQuery('#estuf').find('option').remove();
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    var id = jQuery(this).children().attr("id");
		    arrUF[i] = id;
		    jQuery('#estuf').append(new Option(jQuery(this).children().attr("title"),id.replace("estuf",""), true, true));
		}
	});
	
	if(arrUF.length > 0){
		carregarMapasPontos(1,true);
	}else{
		removeTodosPontos();
	}
}
</script>
<?php
include APPRAIZ . 'includes/maps/maps.inc';
if($_REQUEST['creche']){?>
	<script>
	jQuery(document).ready(function() {
  		//jQuery("#chk_3").attr("checked","checked");
  		carregarMapasPontos(1);
	});
	</script>
<?php }
?>