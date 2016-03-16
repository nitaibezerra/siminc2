<?php
function montaBalao($tipo,$entid,$orgid){
	global $db;
	include(APPRAIZ."/includes/classes/AbaAjax.class.inc");
	
	$abaAjax = new AbaAjax("abasAjax",true,true,true,true);
	
	$arAba = array(
	 		  			array(	 "descricao" => "Dados", 
		 	   				 	 "padrao" => (!$abaPadrao || $abaPadrao = '' || $abaPadrao == "dados" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaInstituicoes&acao=A",
		 		   				 "parametro" => "abaAjax=dados&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			),
//	 		  			array(	 "descricao" => "Concursos", 
//		 	   				 	 "padrao" => ($abaPadrao == "concursos" ? true : false) ,
//		 		   				 "url" => "academico.php?modulo=principal/mapaInstituicoes&acao=A",
//		 		   				 "parametro" => "abaAjax=concursos&entid=$entid&tipo=$tipo&orgid=$orgid"
//	 		  			),
	 		  			array(	 "descricao" => "Academico", 
		 	   				 	 "padrao" => ($abaPadrao == "academico" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaInstituicoes&acao=A",
		 		   				 "parametro" => "abaAjax=academico&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			),
	 		  			array(	 "descricao" => "Obras", 
		 	   				 	 "padrao" => ($abaPadrao == "obras" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaInstituicoes&acao=A",
		 		   				 "parametro" => "abaAjax=obras&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			)
	  		   		 );

	  if($tipo == "instituicao"){
	  	
	  	array_push( $arAba,
	 		  			array(	 "descricao" => "Campus", 
		 	   				 	 "padrao" => ($abaPadrao == "campus" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaInstituicoes&acao=A",
		 		   				 "parametro" => "abaAjax=campus&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			)
	 		  				
	  				);
	  	
	  }
	  
	  $abaAjax->criaAba($arAba,'div_abas_ajax');
	
}

if($_REQUEST['montaBalao']){
	header('content-type: text/html; charset=ISO-8859-1');
	if($_REQUEST['tipo'] && $_REQUEST['entid'] && $_REQUEST['orgid']) {
		montaBalao($_REQUEST['tipo'],$_REQUEST['entid'],$_REQUEST['orgid']);
	} else {
		echo "<p>Não existem informações suficientes.</p>";
	}
	die;
}

if($_REQUEST['abaAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	
	echo '<style>
		.titulounidade {
			background-color:#CDCDCD;
			border-top:2px solid #000000;
			border-bottom:2px solid #000000;
			text-align:center;
			font-weight: bold;
		}
	</style>';
	
	switch($_REQUEST['abaAjax']){
		
		case "dados" :
			abaDadosInstituicao($_REQUEST['entid'],$_REQUEST['orgid']);
		break;
		
		case "concursos" :
			include APPRAIZ. 'includes/classes/relatorio.class.inc';
			ini_set("memory_limit", "1024M");
			abaConcursos($_REQUEST['entid'],$_REQUEST['orgid']);
		break;
		
		case "academico" :
			abaAcademico($_REQUEST['entid'],$_REQUEST['orgid']);
		break;
		
		case "obras" :
			abaObras($_REQUEST['entid'],$_REQUEST['orgid']);
		break;
		
		case "campus" :
			academico_lista_campus_painel($_REQUEST['entid'],$_REQUEST['orgid'],false);
		break;
		
	}
	
	exit;
	
}

function abaDadosInstituicao($entid,$orgid){
	
	global $db;	
	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
			break;
	}
	
	// monta quadro do painel
	!empty( $campus ) ? academico_cabecalho_quadro_painel( $entid, 'campus', $orgid ,'',false) : academico_cabecalho_quadro_painel( $entid ,'','','',false);
		
	// pega a caracterização da unidade
	$edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid={$entid}");
	$edtdsc = $edtdsc ? $edtdsc : 'Não Informado';
	
	// cria a tabela com os dados da unidade
	print '    <tr><td class="SubTituloEsquerda" colspan="2">Caracterização da Instituição</td></tr>'
	    . '    <tr><td colspan="2" style="text-align:justify;">' . $edtdsc . '</td></tr>'
	    . '    <tr><td class="SubTituloEsquerda" colspan="2">Dados da Instituição</td>';

	// monta os dados do endereço da unidade
	academico_monta_endereco( $entid );
	
	print '</tr>';		
	print '<tr><td class="SubTituloEsquerda" colspan="2">Dados do dirigente</td>';

	// monta dados dos dirigentes da unidade
	academico_monta_dirigente( $entid, 'unidade', TPENSSUP );
			
	print '		</tr>'
	    . '	</table>'
	    . '</div>';
	
}

function abaConcursos($entid,$orgid){
	global $db;
	
	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
			break;
	}
	
	// monta quadro do painel
	!empty( $campus ) ? academico_cabecalho_quadro_painel( $entid, 'campus', $orgid ,'',false) : academico_cabecalho_quadro_painel( $entid ,'','','',false);
	
	$sql   = academico_painel_sql( $orgid, $entid );
	$dados = $db->carregar($sql);
	$agrup = academico_painel_agrupador();
	$col   = academico_painel_coluna();

	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setBrasao($true ? true : false);
	$r->setTotNivel(true);

	echo $r->getRelatorio();
}


function abaAcademico( $entid, $orgid ){
	
	global $db, $anosanalisados, $tituloitens;
	
	$js = "<script>
			function selecionaranocomparacao2(value) {
			if(document.getElementById('tabelaxx0')) {
				var t = document.getElementById('tabelaxx0');
				for(i=1;i<t.rows.length;i++) {
					if(t.rows[i].id == value) {
						t.rows[i].style.display = '';
					} else {
						t.rows[i].style.display = 'none';
					}
				}
			}
			
			var t = document.getElementById('tabelaxx1');
			for(i=1;i<t.rows.length;i++) {
				if(t.rows[i].id == value) {
					t.rows[i].style.display = '';
				} else {
					t.rows[i].style.display = 'none';
				}
			}

		}
		</script>";
	echo $js;

	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
			break;
	}
	
	if(	!empty($campus) ) {
		
		academico_cabecalho_quadro_painel( $entid, 'campus', $orgid ,'',false);
		echo "<table class=\"tabela\" bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center width=95%>";
		echo "<tr>
				<td class=SubTituloCentro>Situação atual (até 2009)</td>
				<td class=SubTituloCentro>Até";
		
		$dados = array(0 => array('codigo' => '2010', 'descricao' => '2010'),
					   1 => array('codigo' => '2011', 'descricao' => '2011'),
					   2 => array('codigo' => '2012', 'descricao' => '2012')); 
		$db->monta_combo('anosit', $dados, 'S', '', 'selecionaranocomparacao2', '', '', '', 'N', 'anosit');
		
		echo "</td></tr>";
		
		echo "<tr><td valign=\"top\" >";
		echo "<center>TOTAL</center>";
		academico_situacao_atual( $orgid, array('entid2' => $entid));
		echo "<br />";
		if($orgid == TPENSSUP) {
			echo "<center>REUNI</center>";
			academico_situacao_atual( $orgid, array('entid2' => $entid), 0);
		}
		echo "</td><td>";
		echo "<center>TOTAL</center>";
		academico_situacao_atual_comparacao($orgid, array('entid2' => $entid), null, 'xx');
		echo "<br />";
		if($_SESSION['academico']['orgid'] == TPENSSUP) {
			echo "<center>REUNI</center>";
			academico_situacao_atual_comparacao( $orgid, array('entid2' => $entid), 0, 'xx' );
		}
		echo "</td></tr></table>";
			
	}else{
	
		academico_cabecalho_quadro_painel( $entid ,'','','',false);
		echo "<table class=\"tabela\" bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center width=95%>";
		echo "<tr>
				<td class=SubTituloCentro>Situação atual (até 2009)</td>
				<td class=SubTituloCentro>Até ";
		
		$dados = array(0 => array('codigo' => '2010', 'descricao' => '2010'),
					   1 => array('codigo' => '2011', 'descricao' => '2011'),
					   2 => array('codigo' => '2012', 'descricao' => '2012')); 
		$db->monta_combo('anosit', $dados, 'S', '', 'selecionaranocomparacao2', '', '', '', 'N', 'anosit');
		
		echo "</td></tr>";
		
		echo "<tr><td>";
		
		echo "<center>TOTAL</center>";
		academico_situacao_atual( $orgid, array('entid' => $entid));
		echo "<br />";
		if($orgid == TPENSSUP) {
			echo "<center>REUNI</center>";
			academico_situacao_atual( $orgid, array('entid' => $entid), 0);
		}
		
		echo "</td><td>";
		echo "<center>TOTAL</center>";
		academico_situacao_atual_comparacao($orgid, array('entid' => $entid), 1, 'xx');
		echo "<br />";
		if($_SESSION['academico']['orgid'] == TPENSSUP) {
			echo "<center>REUNI</center>";
			academico_situacao_atual_comparacao( $orgid, array('entid' => $entid), 0, 'xx');
		}
		echo "</td></tr></table>";
			
	}	
}

function abaObras( $entid, $orgid ){
	
	global $db;
	
	// verifica se é uma unidade ou um campus
	$unidade = $db->pegaUm("SELECT obrid FROM obras.obrainfraestrutura where entidunidade = {$entid}");
	
	if ( !empty( $unidade ) ){

		$sql = "SELECT 
					oi.stoid,
					oi.obrid, 
					oi.obrdesc as nome,
					oi.obrdtinicio, 
					oi.obrdttermino, 
					tm.mundescricao||'/'||ed.estuf as local,
					case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
					(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual, 
					oi.obrcomposicao 
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid   
				LEFT JOIN 
					entidade.endereco ed ON ed.endid = oi.endid 
				LEFT JOIN 
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE 
					oi.entidunidade = {$entid} AND oi.orgid = {$orgid} AND oi.obsstatus = 'A'
				ORDER BY
					oi.obrdesc, so.stodesc";
		
		$obras = $db->carregar($sql);
	
		// monta quadro do painel em modelo de unidade
		academico_cabecalho_quadro_painel( $entid ,'','','',false);
		
	}else{
		
		$sql = "SELECT 
					oi.stoid,
					oi.obrid, 
					oi.obrdesc as nome,
					oi.obrdtinicio, 
					oi.obrdttermino, 
					tm.mundescricao||'/'||ed.estuf as local,
					trim(ed.medlatitude) as latitude,
					trim(ed.medlongitude) as longitude,
					case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
					(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual, 
					oi.obrcomposicao 
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid  
				LEFT JOIN 
					entidade.endereco ed ON ed.endid = oi.endid 
				LEFT JOIN 
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE 
					oi.entidcampus = {$entid}  AND oi.orgid = {$orgid} AND oi.obsstatus = 'A'
				ORDER BY
					oi.obrdesc, so.stodesc";
		
		$obras = $db->carregar($sql);

		// monta quadro do painel em modelo de campus
		academico_cabecalho_quadro_painel( $entid, 'campus', $orgid ,'',false);
		
	}
?>	
	<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">
		<tr>
	 		<td>
				<div id="quadrosituacao1" style="width:100%; border:1px solid #cccccc;"/>	
					<table cellspacing="1" cellpadding="3" width="100%">
						<tr>
							<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Resumo de Obras </td>
						</tr>
						<tr>
							<td style="padding: 0px; margin: 0px;">
								<? academico_situacao_obras( $_SESSION['academico']['orgid'], '', $entid ) ?>
							</td>
						</tr>
					</table>
				</div>
			</td>
		</tr>
	</table>
	<div style="width: 97%; margin-top: 5px; margin-bottom: 1px; padding:3px; text-align: center; background-color: #dedede; font-weight: bold;"> Lista de Obras </div>
	
<?
	if( $obras[0] ) {
		$zoom = "<input type='hidden' id='endzoom'  value='15'/>"; 		
		foreach( $obras as $obr ) {
	
			switch ( $obr["stoid"] ){
				
				case "1":
					$obr['situacao'] = '<label style="color:#00AA00">' . $obr['situacao'] . '</label>';
				break;
				case "2":
					$obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
				break;
				case "3":
					$obr['situacao'] = '<label style="color:blue">' . $obr['situacao'] . '</label>';
				break;
				case "6":
					$obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
				break;
				
			}
			
			// latitude
			$dadoslatitude = explode(".", $obr["latitude"]);
			$graulatitude  = $dadoslatitude[0];
			$minlatitude   = $dadoslatitude[1];
			$seglatitude   = $dadoslatitude[2];
			$pololatitude  = $dadoslatitude[3];
			
			$latitude = !empty($graulatitude) ? $graulatitude . 'º ' . $minlatitude . '\' ' . $seglatitude .'" ' . $pololatitude : 'Não Informado';
						
			$campograulatitude = "<input type='hidden' id='{$obr["obrid"]}graulatitude' value='{$graulatitude}'/>";
			$campominlatitude  = "<input type='hidden' id='{$obr["obrid"]}minlatitude'  value='{$minlatitude}'/>";
			$camposeglatitude  = "<input type='hidden' id='{$obr["obrid"]}seglatitude'  value='{$seglatitude}'/>";
			$campopololatitude = "<input type='hidden' id='{$obr["obrid"]}pololatitude' value='{$pololatitude}'/>";
			
			$camposhiddenlat = $campograulatitude . $campominlatitude . $campopololatitude . $camposeglatitude;
			
			// longitude
			$dadoslongitude = explode(".", $obr["longitude"]);
			$graulongitude  = $dadoslongitude[0];
			$minlongitude   = $dadoslongitude[1];
			$seglongitude   = $dadoslongitude[2];
			
			$longitude = !empty($graulongitude) ? $graulongitude . 'º ' . $minlongitude . '\' ' . $seglongitude .'" W' : 'Não Informado';
			
			$campograulongitude = "<input type='hidden' id='{$obr["obrid"]}graulongitude' value='{$graulongitude}'/>";
			$campominlongitude  = "<input type='hidden' id='{$obr["obrid"]}minlongitude'  value='{$minlongitude}'/>";
			$camposeglongitude  = "<input type='hidden' id='{$obr["obrid"]}seglongitude'  value='{$seglongitude}'/>";
			
			$camposhiddenlog = $campograulongitude . $campominlongitude . $camposeglongitude;
			
			// visualizar mapa 
			$mapa = !empty($graulatitude) && !empty($graulongitude) ? '<tr><td class="SubTituloDireita"></td><td><a style="cursor:pointer;" onclick="abreMapa(' . $obr["obrid"] . ');">Visualizar / Buscar No Mapa</a></td></tr>': ''; 
			
			$obrid = "<input type='hidden' id='obrid'  value='{$obr["obrid"]}'/>";
			
//			print '			
//			<div id="conteudolistaunidades1" style="width:100%;"/>
//				<!-- Quadro de situação da Obra -->
//				<div id="quadrosituacao1" class="caixa_redefederal"/>	
//					<table cellspacing="1" cellpadding="3" style="width: 100%;">
//						<tr>
//							<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Obras </td>
//						</tr>
//						<tr>
//							<td style="padding: 0px; margin: 0px;">
//								' . academico_situacao_obras( $_SESSION['academico']['orgid'], '', $entid ) . '
//							</td>
//						</tr>
//					</table>
//				</div>
//		       				       
//			</div>';
			
			
			print '<form action="" method="post" id="formulario">'
				. '<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">'
				. '		<tr>'
			 	. '			<td class="SubTituloDireita" style="width:20%;">Nome da obra:</td><td colspan="2" style="width:45%;"><b>' . $obr['nome'] . $obrid . '</b></td>'
				. '			<td class="SubTituloDireita">Município/UF:</td><td>' . $obr['local'] . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Início programado:</td><td colspan="2">' . formata_data($obr['obrdtinicio']) . '</td>'
				. '			<td class="SubTituloDireita">Término programado:</td><td>' . formata_data($obr['obrdttermino']) . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Situação da Obra:</td><td colspan="2">' . $obr['situacao'] . '</td>'
				. '			<td class="SubTituloDireita">% Executado:</td><td colspan="2">' . $obr['percentual'] . '</td>'		
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Latitude:</td><td colspan="2">' . $latitude . $camposhiddenlat . '</td>'
				. '			<td class="SubTituloDireita">Longitude:</td><td colspan="2">' . $longitude . $camposhiddenlog . $zoom . '</td>'		
				. '		</tr>'
				. $mapa
				. '		<tr>'
				. '			<td class="SubTituloDireita">Descrição:</td><td colspan="4" align="justify">'. ( ($obr['obrcomposicao']) ? nl2br($obr['obrcomposicao']) : "Nenhuma observação inserida" ) . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloCentro" colspan="5">Fotos</td>'
				. '		</tr>';

				$sql = "SELECT 
							arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, 
							to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao 
						FROM 
							public.arquivo arq
						INNER JOIN 
							obras.arquivosobra oar ON arq.arqid = oar.arqid
						INNER JOIN 
							obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
						WHERE 
							obr.obrid='". $obr['obrid'] ."' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
						ORDER BY 
							arq.arqid DESC LIMIT 4";

				$fotos = $db->carregar($sql);
				
				print '<tr>';
				
				if( $fotos[0] ){
					for( $k = 0; $k < count($fotos); $k++ ){

						$_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array( "filtro" => "cnt.obrid=".$uni['obrid']." AND aqostatus = 'A'", 
																				"tabela" => "obras.arquivosobra");
						
						print "<td valign=\"top\" align=\"center\">"
							. "<img id='".$fotos[$k]["arqid"]."' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obr['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:80px; height:80px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\"/><br />"
							. $fotos[$k]["aqodtinclusao"]."<br />"
							. $fotos[$k]["arqdescricao"]
							. "</td>";
						
					}
				} else {
					print "<td colspan='5'>Não existe(m) foto(s) cadastrada(s).</td>";
				}
				
			print '		</tr>'
				. '</table>'
				. '</form>'
				. '<br/>';
				
		}
	}else{
		
		print '<tr><td align="center"><b>Não existe(m) Obra(s) cadastrada(s).</b></td></tr>';
		
	}
	
	// fecha quadro do painel
	print '	</table>'
	    . '</div>';
	
}


/**
 * Funçao que monta o agrupador do relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return array
 */
function ept_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("regiao",
										  "uf",
										  "tipo",
										  "campus","populacao","municipio"),
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
															"campo" => "tipo",
															"label" => "Tipo"
														  ),
													array(
															"campo" => "campus",
															"label" => "Campus"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "populacao",
															"label" => "População"
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
			case "tipo":
				array_push($agp['agrupador'], array(
													"campo" => "tipo",
											  		"label" => "Tipo")										
									   				);
			break;
			case "campus":
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											  		"label" => "Campus")										
									   				);
			break;
			case "populacao":
				array_push($agp['agrupador'], array(
													"campo" => "populacao",
											  		"label" => "População")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}


if($_REQUEST['relatorio_obras']) {
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_REQUEST['chk']) {
		if(in_array("7", $_REQUEST['chk'])) $classif[] = "1 - Câmpus Preexistentes";			
		if(in_array("8", $_REQUEST['chk']))	$classif[] = "2 - Criadas (2003/2010)";			
		if(in_array("4", $_REQUEST['chk']))	$classif[] = "3 - Previstos (2011/2012)";
		if(in_array("3", $_REQUEST['chk']))	$classif[] = "4 - Propostos (2013/2014)";
		if(in_array("6", $_REQUEST['chk']))	$classif[] = "5 - Universidades Previstas (2013/2014)";
		
		if($classif) $filtro= "r.rfstipo IN ('".implode("','",$classif)."') ";
	}
	
	
	$sql = "SELECT 
			'<a style=cursor:pointer; onclick=\"abrebalao(\''||r.rfsid||'\');\">('||mun.mundescricao || ') ' || r.rfsnome||'</a>' as campus,
			r.rfstipo as tipo,
			trim(r.estuf) as uf,
			reg.regdescricao as regiao,
			mun.munpopulacao as populacao,
			mun.mundescricao as municipio
			FROM academico.redefederalsuperior r 
			LEFT JOIN territorios.estado est ON est.estuf = r.estuf::character(2)
			LEFT JOIN territorios.municipio mun ON r.muncod = mun.muncod::character varying(255)
			LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
			".(($filtro)?"WHERE ".$filtro:"")." ".(($_REQUEST['agrupador'])?"ORDER BY ".implode(",",$_REQUEST['agrupador']):"");
		
	if(!$_REQUEST['agrupador']) $_REQUEST['agrupador'] = array("uf","tipo","municipio","campus","regiao");
	$agrupador = ept_monta_agp_relatorio();
	$dados = $db->carregar( $sql );

	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(false);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setTotalizador(true);
	
	$coluna[0] = array("campo" 	  => "populacao",
						"label" 	  => "População", 
						"type"	  => "numeric");
	$rel->setColuna($coluna);
	
	$rel->setAgrupador($agrupador, $dados); 
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
	
	$sql1 = "SELECT
			SUM(DISTINCT mun.munpopulacao) as pop 
			FROM academico.redefederalsuperior r 
			LEFT JOIN territorios.municipio mun ON r.muncod = mun.muncod::character varying(255)
			".(($filtro)?"WHERE ".$filtro:"")."
			";
	echo "<table class=listagem align=center width=95%>";
	echo "<tr><td class=SubTituloCentro>&nbsp;</td><td class=SubTituloCentro>TOTAL</td></tr>";
	echo "<tr><td class=SubTituloDireita>População atendida:</td><td>".number_format($db->pegaUm($sql1),0,",",".")."</td></tr>";
	echo "</table>";
	
	exit;
	
	
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = "Mapa da Rede Federal da SESU";

monta_titulo( $titulo, '&nbsp;');

include APPRAIZ."academico/modulos/principal/mapaInstituicoesSuperiorCore.inc";