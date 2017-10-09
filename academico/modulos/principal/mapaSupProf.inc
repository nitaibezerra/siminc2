<?

function montaBalao($tipo,$entid,$orgid){
	global $db;
	
	include(APPRAIZ."/includes/classes/AbaAjax.class.inc");
	
	$abaAjax = new AbaAjax("abasAjax",true,true,true,true);
	
	$arAba = array(
	 		  			array(	 "descricao" => "Dados", 
		 	   				 	 "padrao" => (!$abaPadrao || $abaPadrao = '' || $abaPadrao == "dados" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaSupProf&acao=".$_REQUEST['acao'],
		 		   				 "parametro" => "abaAjax=dados&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			),
	 		  			array(	 "descricao" => "Academico", 
		 	   				 	 "padrao" => ($abaPadrao == "academico" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaSupProf&acao=".$_REQUEST['acao'],
		 		   				 "parametro" => "abaAjax=academico&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			),
	 		  			array(	 "descricao" => "Obras", 
		 	   				 	 "padrao" => ($abaPadrao == "obras" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaSupProf&acao=".$_REQUEST['acao'],
		 		   				 "parametro" => "abaAjax=obras&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			)
	  		   		 );

	  if($tipo == "instituicao"){
	  	
	  	array_push( $arAba,
	 		  			array(	 "descricao" => "Campus", 
		 	   				 	 "padrao" => ($abaPadrao == "campus" ? true : false) ,
		 		   				 "url" => "academico.php?modulo=principal/mapaSupProf&acao=".$_REQUEST['acao'],
		 		   				 "parametro" => "abaAjax=campus&entid=$entid&tipo=$tipo&orgid=$orgid"
	 		  			)
	 		  				
	  				);
	  	
	  }
	  
	  $abaAjax->criaAba($arAba,'div_abas_ajax');
	
}

if($_REQUEST['montaBalao']){
	header('content-type: text/html; charset=ISO-8859-1');
	if($_REQUEST['acao']=="S") {
		$orgid = "1";
	} elseif($_REQUEST['acao']=="P") {
		$orgid = "2";
	}
	montaBalao($_REQUEST['tipo'],$_REQUEST['entid'],$orgid);
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

if($_REQUEST['relatorio_superior']) {
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_REQUEST['chk_superior']) {
		if(in_array("1", $_REQUEST['chk_superior']))    $classif[] = "Preexistentes";
		if(in_array("2", $_REQUEST['chk_superior']))	$classif[] = "Criados em 2003/2010";			
		if(in_array("3", $_REQUEST['chk_superior']))	$classif[] = "Novos 2011/2012";
		if(in_array("4", $_REQUEST['chk_superior']))	$classif[] = "Novos 2013/2014";
		if(in_array("5", $_REQUEST['chk_superior']))	$classif[] = "Novas expansões";
		
		if($classif) $filtro[] = "exi.exidsc IN ('".implode("','",$classif)."') ";
	}
	
	if( $_REQUEST['buscaTextual'] ){
		$filtro[] = "UPPER(entcampus.entnome) like UPPER('%".$_REQUEST['buscaTextual']."%')";
	}
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "trim(est.estuf) in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "mun.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	$order= str_replace(array("municipio","uf","campus"),array("mun.mundescricao","est.estuf","entcampus.entnome"),$_REQUEST['agrupador']);
	
	/* SQL ANTIGO
	$sql = "SELECT 
			'<a style=cursor:pointer; onclick=\"abrebalao(\''||r.rfsid||'\');\">('||mun.mundescricao || ') ' || r.rfsnome||'</a>' as campus,
			r.rfstipo as tipo,
			'<a style=cursor:pointer; onclick=\"graficoHabitantes(\''||trim(r.estuf)||'\');\">'||trim(r.estuf)||'</a>' as uf,
			reg.regdescricao as regiao,
			mun.munpopulacao as populacao,
			'<a onmouseover=\"f_mouseover(\''||trim(r.estuf)||mun.muncod||'\',\'#F0F\',\''||mun.mundescricao||'/'||trim(r.estuf)||'\');\" onmouseout=\"f_mouseout(\''||trim(r.estuf)||mun.muncod||'\',\'\');\" style=cursor:pointer; onclick=\"infmunicipio(\''||mun.muncod||'\');abreAcordion(1);\">'||mun.mundescricao||'</a>' as municipio
			FROM academico.redefederalsuperior r 
			LEFT JOIN territorios.estado est ON est.estuf = r.estuf::character(2)
			LEFT JOIN territorios.municipio mun ON r.muncod = mun.muncod::character varying(255)
			LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
			".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." ".(($order)?"ORDER BY ".implode(",",$order):"");
	*/

	
	$sql = "select 
	'<a style=cursor:pointer; onclick=\"abrebalao(\''||entcampus.entid||'\');\">('||mun.mundescricao || ') '||entcampus.entnome||'</a>' as campus, 
	exi.exidsc as tipo, 
	'<a style=cursor:pointer; onclick=\"graficoHabitantes(\''||trim(est.estuf)||'\');\">'||trim(est.estuf)||'</a>' as uf, 
	reg.regdescricao as regiao, 
	mun.munpopulacao as populacao, 
	'<a onmouseover=\"f_mouseover(\''||trim(est.estuf)||mun.muncod||'\',\'#F0F\',\''||mun.mundescricao||'/'||trim(est.estuf)||'\');\" onmouseout=\"f_mouseout(\''||trim(est.estuf)||mun.muncod||'\',\'\');\" style=cursor:pointer; onclick=\"infmunicipio(\''||mun.muncod||'\');abreAcordion(1);\">'||mun.mundescricao||'</a>' as municipio
	from entidade.entidade entcampus 
	inner join entidade.endereco edc ON edc.entid = entcampus.entid 
	LEFT JOIN territorios.municipio mun ON edc.muncod = mun.muncod
	LEFT JOIN territorios.estado est ON est.estuf = mun.estuf 
	LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
	inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
	inner join entidade.funentassoc a on a.fueid = fn.fueid
	inner join entidade.entidade entinst on entinst.entid = a.entid 
	inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
	inner join academico.campus cmp on cmp.entid = entcampus.entid
	inner join academico.existencia exi on exi.exiid = cmp.exiid
	left join academico.entidadedetalhe edt on edt.entid = entcampus.entid 
	".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." ".(($order)?"ORDER BY ".implode(",",$order):"");
	
	if(!$_REQUEST['agrupador']) $_REQUEST['agrupador'] = array("uf","tipo","municipio","campus","regiao");
	$agrupador = ept_monta_agp_relatorio();
	$dados = $db->carregar( $sql );

	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(false);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setTotalizador(true);
	$rel->setAgrupador($agrupador, $dados); 
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
	exit;
	
}

if($_REQUEST['relatorio_profissional']) {
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_REQUEST['chk_profissional']) {
		
		if(in_array("1", $_REQUEST['chk_profissional']))	$classif[] = "Preexistentes";
		if(in_array("2", $_REQUEST['chk_profissional'])) $classif[] = "Criados em 2003/2010";			
		if(in_array("3", $_REQUEST['chk_profissional']))	$classif[] = "Novos 2011/2012";
		if(in_array("4", $_REQUEST['chk_profissional']))	$classif[] = "Novos 2013/2014";
        if(in_array("5", $_REQUEST['chk_profissional']))	$classif[] = "Novas expansões";

		if($classif) $filtro[] = "exi.exidsc IN ('".implode("','",$classif)."') ";
	}
	
	if( $_REQUEST['buscaTextual'] ){
		$filtro[] = "UPPER(entcampus.entnome) like UPPER('%".$_REQUEST['buscaTextual']."%')";
	}
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "est.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "mun.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	$order= str_replace(array("municipio","uf","campus"),array("mun.mundescricao","est.estuf","entcampus.entnome"),$_REQUEST['agrupador']);
	
	/*
	$sql = "SELECT 
			'<a style=cursor:pointer; onclick=\"abrebalao(\''||r.rfeid||'\');\">('||mun.mundescricao || ') ' || r.rfecampus||'</a>' as campus,
			r.rfeclassificacao as tipo,
			trim(r.rfeuf) as uf,
			reg.regdescricao as regiao,
			mun.munpopulacao as populacao,
--			mun.mundescricao as municipio
			'<a onmouseover=\"f_mouseover(\''||trim(r.rfeuf)||mun.muncod||'\',\'#F0F\',\''||mun.mundescricao||'/'||trim(r.rfeuf)||'\');\" onmouseout=\"f_mouseout(\''||trim(r.rfeuf)||mun.muncod||'\',\'\');\" style=cursor:pointer; onclick=\"infmunicipio(\''||mun.muncod||'\');abreAcordion(1);\">'||mun.mundescricao||'</a>' as municipio
			FROM academico.redefederal r 
			LEFT JOIN territorios.estado est ON est.estuf = r.rfeuf::character(2)
			LEFT JOIN territorios.municipio mun ON r.rfeibge = mun.muncod
			LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
			".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." ".(($order)?"ORDER BY ".implode(",",$order):"");
	*/

	$sql = "select 
	'<a style=cursor:pointer; onclick=\"abrebalao(\''||entcampus.entid||'\');\">('||mun.mundescricao || ') '||entcampus.entnome||'</a>' as campus, 
	exi.exidsc as tipo, 
	'<a style=cursor:pointer; onclick=\"graficoHabitantes(\''||trim(est.estuf)||'\');\">'||trim(est.estuf)||'</a>' as uf, 
	reg.regdescricao as regiao, 
	mun.munpopulacao as populacao, 
	'<a onmouseover=\"f_mouseover(\''||trim(est.estuf)||mun.muncod||'\',\'#F0F\',\''||mun.mundescricao||'/'||trim(est.estuf)||'\');\" onmouseout=\"f_mouseout(\''||trim(est.estuf)||mun.muncod||'\',\'\');\" style=cursor:pointer; onclick=\"infmunicipio(\''||mun.muncod||'\');abreAcordion(1);\">'||mun.mundescricao||'</a>' as municipio
	from entidade.entidade entcampus 
	inner join entidade.endereco edc ON edc.entid = entcampus.entid 
	LEFT JOIN territorios.municipio mun ON edc.muncod = mun.muncod
	LEFT JOIN territorios.estado est ON est.estuf = mun.estuf 
	LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
	inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
	inner join entidade.funentassoc a on a.fueid = fn.fueid
	inner join entidade.entidade entinst on entinst.entid = a.entid 
	inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 11 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
	inner join academico.campus cmp on cmp.entid = entcampus.entid
	inner join academico.existencia exi on exi.exiid = cmp.exiid
	left join academico.entidadedetalhe edt on edt.entid = entcampus.entid 
	".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." ".(($order)?"ORDER BY ".implode(",",$order):"");
	
	if(!$_REQUEST['agrupador']) $_REQUEST['agrupador'] = array("uf","tipo","municipio","campus","regiao");
	$agrupador = ept_monta_agp_relatorio();
	$dados = $db->carregar( $sql );

	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(false);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setTotalizador(true);
	$rel->setAgrupador($agrupador, $dados);
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
	exit;
	
}


if($_REQUEST['graficohabitantes']) {
	ob_clean();
	
	$_arrValores = array('Nacional' => array('Brasil'   => '9.8'),
						 'Região Norte' => array('Acre'     => '26.9',
												 'Amapa'    => '13.4',
												 'Amazonas' => '13.8',
												 'Para' => '7.5',
												 'Rondonia' => '14.6',
												 'Roraima' => '23.5',
												 'Tocantins' => '19.3'),
						 'Região Nordeste' => array('Alagoas' => '13.1',
													'Bahia' => '6.8',
													'Ceara' => '5.7',
													'Maranhao' => '6.3',
													'Paraíba' => '27.4',
													'Pernambuco' => '11.0',
													'Piaui' => '18.6',
													'Rio Grande do Norte' => '21.8',
													'Sergipe' => '21.8'),
						 'Região Centro Oeste' => array('Distrito Federal' => '22.8',
														'Goias' => '9.6',
														'Mato Grosso' => '14.4',
														'Mato Grosso do Sul' => '19.7'),
						 'Região Sudeste' => array('Espirito Santo' => '13.2',
												   'Minas Gerais' => '13.8',
												   'Rio de Janeiro' => '10.7',
												   'Sao Paulo' => '1.4'),
						 'Região Sul' => array('Parana' => '10.1',
											   'Rio Grande do Sul' => '15.5',
											   'Santa Catarina' => '8.4'));
	
	?>
	<html>
	<head>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
	<meta http-equiv="Connection" content="Keep-Alive">
	<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
	<title>Detalhes do monitoramento</title>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
	<script language="javascript" type="text/javascript" src="../includes/open_flash_chart/swfobject.js"></script>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloCentro">Relação de vagas para cada 10.000 habitantes</td>
	</tr>
	<tr>
		<td>
	<div id="graficohabitantes"></div>
	<script type="text/javascript">
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "graficohabitantes", "600", "400", "9.0.0", "expressInstall.swf", {"data-file":"chart-data.php?uf=<?=(($_REQUEST['uf'])?$db->pegaUm("SELECT lower(removeacento(estdescricao)) as est FROM territorios.estado WHERE estuf='".$_REQUEST['uf']."'"):"") ?>","loading":"Carregando gráfico..."} );
	</script>
		</td>
	</tr>
	<tr>
	<td>
	<? foreach($_arrValores as $regiao => $arrEstado): ?>
		<fieldset style="float: left;"><legend><?=$regiao ?></legend>
		<table>
		<? foreach($arrEstado as $estado => $valor): ?>
		<tr>
			<td><?=$estado ?></td>
			<td><?=$valor  ?></td>
		</tr>
		<? endforeach; ?>
		</table>
		</fieldset>
	<? endforeach; ?>
	</td>
	</tr>
	</table>
	</body>
	</html>
	<?
	exit; 
}

?>
<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js"></script>
<script type="text/javascript">
$(document).ready(function() {
    $("#accordion").accordion();
  });


var map;
var draw_circle = null;  // object of google maps polygon for redrawing the circle

var shape = {
    coord: [1, 1, 1, 20, 18, 20, 18 , 1],
    type: 'poly'
};

var baseIcon_superior_1 = new google.maps.MarkerImage('/imagens/icone_capacete_1.png', new google.maps.Size(9, 14));
var baseIcon_superior_2 = new google.maps.MarkerImage('/imagens/icone_capacete_3.png', new google.maps.Size(9, 14));
var baseIcon_superior_3 = new google.maps.MarkerImage('/imagens/icone_capacete_2.png', new google.maps.Size(9, 14));
var baseIcon_superior_4 = new google.maps.MarkerImage('/imagens/icone_capacete_4.png', new google.maps.Size(9, 14));
var baseIcon_superior_5 = new google.maps.MarkerImage('/imagens/icone_capacete_7.png', new google.maps.Size(9, 14));

var baseIcon_profissional_1 = new google.maps.MarkerImage('/imagens/icone_capacete_1.png', new google.maps.Size(9, 14));
var baseIcon_profissional_2 = new google.maps.MarkerImage('/imagens/icone_capacete_3.png', new google.maps.Size(9, 14));
var baseIcon_profissional_3 = new google.maps.MarkerImage('/imagens/icone_capacete_2.png', new google.maps.Size(9, 14));
var baseIcon_profissional_4 = new google.maps.MarkerImage('/imagens/icone_capacete_4.png', new google.maps.Size(9, 14));

var markers_superior_1  = new Array();
var markers_superior_2  = new Array();
var markers_superior_3  = new Array();
var markers_superior_4  = new Array();
var markers_superior_5  = new Array();

var markers_profissional_1  = new Array();
var markers_profissional_2  = new Array();
var markers_profissional_3  = new Array();
var markers_profissional_4  = new Array();

var nomePoli        = new Array();
var nomePoli2       = new Array();
var nomePont        = new Array();
var corPoli         = new Array();
var arrMuncod		= new Array();

var estadoPoligono  = new Array();
var estadoPoligono2 = new Array();
var centroPoligono2 = new Array();
var estadoPonto     = new Array();

var infowindow      = '';
var htmlInfo        = '';
var filtro_antigo   = '';

var directionDisplay;
var directionsService;
var stepDisplay;
var markerArray = [];


/*
 * Função de inicialização do google maps
 */
function initialize() {
	
	var myLatLng = new google.maps.LatLng(-14.689881, -52.373047);
		
    var center = myLatLng;
    var myOptions = {
      zoom: 4,
      center: myLatLng,
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map_canvas"),
        myOptions);

	/* Rotas (origem e destino) */
	var rendererOptions = {
      map: map,
      draggable: true
      
    }
    
	directionsDisplay = new google.maps.DirectionsRenderer(rendererOptions)
	
    directionsService = new google.maps.DirectionsService();
    
    directionsDisplay.setMap(map);
    directionsDisplay.setPanel(document.getElementById("directionsPanel"));
    
    // Instantiate an info window to hold step text.
    stepDisplay = new google.maps.InfoWindow();
    
    
	google.maps.event.addListener(map, "rightclick",function(event){showContextMenu(event.latLng);});
	
    
}

/*
 * Centraliza o mapa em determianadas coordenadas e zoom 
 */
function centraliza(zoom, coords) {
  	zoom = parseInt(zoom);
  	map.setZoom(zoom);
	map.setCenter(new google.maps.LatLng(coords[1], coords[0])); //Centraliza e aplica o zoom
}

function showContextMenu(caurrentLatLng) {
         var LatLng = '' + caurrentLatLng + '';
         LatLng = LatLng.replace("(","");
         LatLng = LatLng.replace(")","");
         var arrLatLng = [];
         arrLatLng = LatLng.split(",");
         var projection;
         var contextmenuDir;
         projection = map.getProjection() ;
         $('.contextmenu').remove();
          contextmenuDir = document.createElement("div");
           contextmenuDir.className  = 'contextmenu';
           contextmenuDir.innerHTML = '<div id="menu1" onclick="defineRotaStart(' + arrLatLng[0] + ',' + arrLatLng[1] + ')" class="context">Como chegar a partir daqui<\/div>'
                                   + '<div id="menu2" onclick="defineRotaEnd(' + arrLatLng[0] + ',' + arrLatLng[1] + ')" class="context">Como chegar até aqui<\/div>';

         $(map.getDiv()).append(contextmenuDir);
      
         setMenuXY(caurrentLatLng);

         contextmenuDir.style.visibility = "visible";
        }

function defineRotaStart(lat,lng)
{
	$('.contextmenu').hide();
	abreAcordion(3);
	$('#start').val("");
	$('#start_lat').val(lat);
	$('#start_lng').val(lng);
	if($('#end_lat').val() && $('#end_lng').val()){
		calcRoute();
	}
}

function defineRotaEnd(Lat,Lng)
{
	$('.contextmenu').hide();
	$('#end').val("");
	abreAcordion(3);
	$('#end_lat').val(Lat);
	$('#end_lng').val(Lng);
	if($('#start_lat').val() && $('#start_lng').val()){
		calcRoute();
	}
}

 function getCanvasXY(caurrentLatLng){
       var scale = Math.pow(2, map.getZoom());
      var nw = new google.maps.LatLng(
          map.getBounds().getNorthEast().lat(),
          map.getBounds().getSouthWest().lng()
      );
      var worldCoordinateNW = map.getProjection().fromLatLngToPoint(nw);
      var worldCoordinate = map.getProjection().fromLatLngToPoint(caurrentLatLng);
      var caurrentLatLngOffset = new google.maps.Point(
          Math.floor((worldCoordinate.x - worldCoordinateNW.x) * scale),
          Math.floor((worldCoordinate.y - worldCoordinateNW.y) * scale)
      );
      return caurrentLatLngOffset;
   }

function setMenuXY(caurrentLatLng){
     var mapWidth = $('#map_canvas').width();
     var mapHeight = $('#map_canvas').height();
     var menuWidth = $('.contextmenu').width();
     var menuHeight = $('.contextmenu').height();
     var clickedPosition = getCanvasXY(caurrentLatLng);
     var x = clickedPosition.x ;
     var y = clickedPosition.y ;

      if((mapWidth - x ) < menuWidth)//if to close to the map border, decrease x position
          x = x - menuWidth;
     if((mapHeight - y ) < menuHeight)//if to close to the map border, decrease y position
         y = y - menuHeight;

     $('.contextmenu').css('left',x  );
     $('.contextmenu').css('top',y );
     };


function removeRoute() {
  directionsDisplay.setMap(null);

}
// Create a renderer for directions and bind it to the map.  
  function calcRoute() {
	directionsDisplay.setMap(map);
    // First, remove any existing markers from the map.
    for (i = 0; i < markerArray.length; i++) {
      markerArray[i].setMap(null);
    }

    // Now, clear the array itself.
    markerArray = [];

    // Retrieve the start and end locations and create
    // a DirectionsRequest using WALKING directions.
    var start = document.getElementById("start").value;
    var end = document.getElementById("end").value;
    
    if(!start){
		var start = new google.maps.LatLng($('#start_lat').val(), $('#start_lng').val());
    }
    
    if(!end){
    	var end = new google.maps.LatLng($('#end_lat').val(), $('#end_lng').val());
    }    
    
    var request = {
        origin: start,
        destination: end,
        travelMode: google.maps.DirectionsTravelMode.WALKING
    };

    // Route the directions and pass the response to a
    var request = {
        origin:start, 
        destination:end,
        travelMode: google.maps.DirectionsTravelMode.DRIVING
    };

    // function to create markers for each step.
    directionsService.route(request, function(response, status) {
      if (status == google.maps.DirectionsStatus.OK) {
        var warnings = document.getElementById("warnings_panel");
	    warnings.innerHTML = "<b>" + response.routes[0].warnings + "</b>";
        directionsDisplay.setDirections(response);
        showSteps(response);
        $('#start').val(response.routes[0].legs[0].start_address);
        $('#end').val(response.routes[0].legs[0].end_address);
      }
    });
  }

  function showSteps(directionResult) {
    // For each step, place a marker, and add the text to the marker's
    // info window. Also attach the marker to an array so we
    // can keep track of it and remove it when calculating new
    // routes.
    var myRoute = directionResult.routes[0].legs[0];

    for (var i = 0; i < myRoute.steps.length; i++) {
//      var marker = new google.maps.Marker({
//        position: myRoute.steps[i].start_point, 
//        map: map
//      });
      //attachInstructionText(marker, myRoute.steps[i].instructions);
      //markerArray[i] = marker;
    }
  }

  function attachInstructionText(marker, text) {
    google.maps.event.addListener(marker, 'click', function() {
      // Open an info window when the marker is clicked on,
      // containing the text of the step.
      stepDisplay.setContent(text);
      stepDisplay.open(map, marker);
    });
  }

function centralizaBrasil(){

  	map.setZoom(4);
	map.setCenter(new google.maps.LatLng(-14.689881, -52.373047)); //Centraliza e aplica o zoom
    	
}

/*
 * Função que cria o HTML exibido ao clicar em algum ponto 
 */
function gerarHtmlPonto(marker) {

	var html='';
	
	if(marker.attr("entid")) {
		html = "<div style=\"font-family:verdana;font-size:11px;\" >";
		html += "<b>Campus:</b> " + marker.attr("nome") + "<br />";
		html += "<b>Localização:</b> " + marker.attr("mundsc") + "/" + marker.attr("estuf") + "<br /><br />";
		html += "<span style=\"cursor:pointer;font-weight:bold\">Mais detalhes...</span>";
		html += "</div>";
		html = "<div style=\"padding:5px\" ><iframe src=\"academico.php?modulo=principal/mapaSupProf&acao=<?=$_REQUEST['acao'] ?>&montaBalao=1&entid=" +  marker.attr("entid") + "&tipo=" + marker.attr("balao") + "&orgid=" + marker.attr("orgid") + "\" frameborder=0 scrolling=\"auto\" height=\"180px\" width=\"330px\" ></iframe></div>";
	} else {
		if(document.getElementById('educacao').value=="S") {
			var dadoscaract = marker.attr("rfscaracteristica").split(";");		
		} else if(document.getElementById('educacao').value=="P") {
			var dadoscaract = marker.attr("rfecaracteristica").split(";");
		}

		for(var j=0;j<dadoscaract.length;j++) {
			html += dadoscaract[j]+"<br/>";
		}
	}
	
	return html;
}

/*
 * Define qual o icone o ponto vai utilizar 
 */
function retornarIcone(tipo, id) {
	var icon;
	var cor;

	if(document.getElementById('educacao').value=="S") {
		
		switch(tipo) {
			case '1':
				icon = baseIcon_superior_1;
				cor  = "#FCD116";
				break;
			case '2':
				icon = baseIcon_superior_2;
				cor  = "#B3EE3A";
				break;
			case '3':
				icon = baseIcon_superior_3;
				cor  = "#00CED1";
				break;
			case '4':
				icon = baseIcon_superior_4;
				cor  = "#FF3333";
				break;
			case '5':
				icon = baseIcon_superior_5;
				cor  = "#6A5ACD";
				break;
		}
		
	} else if(document.getElementById('educacao').value=="P") {
		switch(tipo) {
			case '1':
				icon = baseIcon_profissional_1;
				cor  = "#FCD116";
				break;
			case '2':
				icon = baseIcon_profissional_2;
				cor  = "#B3EE3A";
				break;
			case '3':
				icon = baseIcon_profissional_3;
				cor  = "#00CED1";
				break;
			case '4':
				icon = baseIcon_profissional_4;
				cor  = "#FF3333";
				break;
		}
	
	}
	
	if(nomePoli[id]) {
		f_mudacor(id,cor);
	}
	
	return icon;

}

function verificarEducacao() {
	if(document.getElementById('educacao').value=="S") {
		document.getElementById('marcadores_superior').style.display = '';
		document.getElementById('marcadores_profissional').style.display = 'none';
	} else if(document.getElementById('educacao').value=="P") {
		document.getElementById('marcadores_profissional').style.display = '';
		document.getElementById('marcadores_superior').style.display = 'none';
		
	}
}

function armazenarPonto(tipo, ponto) {
	if(document.getElementById('educacao').value=="S") {
		switch(tipo) {
			case '1':
				markers_superior_1.push(ponto);
				break;
			case '2':
				markers_superior_2.push(ponto);
				break;
			case '3':
				markers_superior_3.push(ponto);
				break;
			case '4':
				markers_superior_4.push(ponto);
				break;
			case '5':
				markers_superior_5.push(ponto);
				break;
		}
	} else if(document.getElementById('educacao').value=="P") {
		switch(tipo) {
			case '1':
				markers_profissional_1.push(ponto);
				break;
			case '2':
				markers_profissional_2.push(ponto);
				break;
			case '3':
				markers_profissional_3.push(ponto);
				break;
			case '4':
				markers_profissional_4.push(ponto);
				break;
		}
	}
}


    function marcarPontos(xml_filtro) {
	jQuery.ajax({
            type: "POST",
            url: xml_filtro,
            async: false,
            success: function(data) {
                  jQuery(data).find("marker").each(function() {

                    var marker = jQuery(this);
                    var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")), parseFloat(marker.attr("lng")));

                    var html = gerarHtmlPonto(marker);

                    var icon = retornarIcone(marker.attr("tipo"), marker.attr("estuf")+ marker.attr("muncod") );

                    var ponto = new google.maps.Marker({
                        position: latlng,
                        map: map,
                        icon: icon,
                        shape: shape,
                        zIndex: 1
                    });

                    if(!estadoPonto[marker.attr("estuf")]) {
                        estadoPonto[marker.attr("estuf")]='';
                    }

                    if(document.getElementById('educacao').value=="S") {
                        nomePont[marker.attr("entid")] = ponto;
                        estadoPonto[marker.attr("estuf")] += marker.attr("entid")+",";
                    } else if(document.getElementById('educacao').value=="P") {
                        nomePont[marker.attr("entid")] = ponto;
                        estadoPonto[marker.attr("estuf")] += marker.attr("entid")+",";
                    }

                    armazenarPonto(marker.attr("tipo"), ponto);

                    google.maps.event.addListener(ponto, "click", function(){
                        if (infowindow)
                            infowindow.close(); 

                        infowindow = new google.maps.InfoWindow({
                            content: html
                        }); 

                        infowindow.open(map, ponto); 
                    });
                });
            }
        });

    }

function carregarFiltros() {

	var filtros=''; 
	
	if(document.getElementById('educacao').value=="S") {
		filtros += "&educacao=S";
	} else if(document.getElementById('educacao').value=="P") {
		filtros += "&educacao=P";
	}
	
	if(document.getElementById('texto_busca').value != "") {
		filtros += "&texto_busca="+document.getElementById('texto_busca').value;
	}
	
	if(document.getElementById('educacao').value=="S") {
	
		if(document.getElementById('chk_superior_1').checked) { filtros += "&chk_superior[]=1"; }
		if(document.getElementById('chk_superior_2').checked) { filtros += "&chk_superior[]=2"; }
		if(document.getElementById('chk_superior_3').checked) { filtros += "&chk_superior[]=3"; }
		if(document.getElementById('chk_superior_4').checked) { filtros += "&chk_superior[]=4"; }
		if(document.getElementById('chk_superior_5').checked) { filtros += "&chk_superior[]=5"; }
		
	} else if(document.getElementById('educacao').value=="P") {
	
		if(document.getElementById('chk_profissional_1').checked) { filtros += "&chk_profissional[]=1"; }
		if(document.getElementById('chk_profissional_2').checked) { filtros += "&chk_profissional[]=2"; }
		if(document.getElementById('chk_profissional_3').checked) { filtros += "&chk_profissional[]=3"; }
		if(document.getElementById('chk_profissional_4').checked) { filtros += "&chk_profissional[]=4"; }
	}
	
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );

	var linha_uf = document.getElementById('linha_uf');
	for(var i=0;i<linha_uf.cells.length;i++) {
		if(linha_uf.cells[i].style.backgroundColor!='') {
			filtros += '&estuf[]='+linha_uf.cells[i].childNodes[0].innerHTML;
		}
	}
	filtros += '&'+$('#estuf').serialize();
	filtros += '&'+$('#muncod').serialize();
	filtros += '&'+$('#agrupador').serialize();
	
	return filtros;

}

function preencherRelatorio(filtros) {

	if(document.getElementById('educacao').value=="S") {
		var relat = "relatorio_superior=1"; 
	} else if(document.getElementById('educacao').value=="P") {
		var relat = "relatorio_profissional=1";
	}

	$.ajax({
 		type: "POST",
   		url: "academico.php?modulo=principal/mapaSupProf&acao=<?=$_REQUEST['acao'] ?>",
   		data: relat+filtros,
   		async: false,
   		success: function(msg){
   			abreAcordion(2);
   			extrairScript(msg);
   			document.getElementById('colunainfo').innerHTML = msg;
   		}
 		});
 		
}

function apagarPontos() {
	deleteOverlays(markers_superior_1);
	deleteOverlays(markers_superior_2);
	deleteOverlays(markers_superior_3);
	deleteOverlays(markers_superior_4);
	deleteOverlays(markers_superior_5);
	deleteOverlays(markers_profissional_1);
	deleteOverlays(markers_profissional_2);
	deleteOverlays(markers_profissional_3);
	deleteOverlays(markers_profissional_4);
}

function carregarPontos() {

	divCarregando();
	
	document.getElementById('colunainfo').innerHTML = 'Carregando...';
	
	var linha_uf = document.getElementById('linha_uf');
	for(var i=0;i<linha_uf.cells.length;i++) {
		linha_uf.cells[i].style.backgroundColor='';
	}
	
	var filtros = carregarFiltros();
	
	apagarPontos();
	
	if(document.getElementById('educacao').value=="S") {
		marcarPontos("XMLmapaInstituicoesSuperior.php?1=1"+filtros);
	} else if(document.getElementById('educacao').value=="P") {
		marcarPontos("XMLmapaInstituicoesProfissionais.php?1=1"+filtros);
	}
	
	preencherRelatorio(filtros);
		
	filtro_antigo = filtros;
	
	divCarregado();
}
 


// Deletes all markers in the array by removing references to them
function deleteOverlays(markersArray_) {
  if (markersArray_.length > 0) {
    for (i=0;i<markersArray_.length;i++) {
      markersArray_[i].setMap(null);
    }
    markersArray_.length = 0;
  }
}

// Removes the overlays from the map, but keeps them in the array
function clearOverlays(markersArray_) {
  if(markersArray_.length>0) {
  	for(i=0;i<markersArray_.length;i++) {
      markersArray_[i].setMap(null);
  	}
  }
}

// Shows any overlays currently in the array
function showOverlays(markersArray_) {
  if(markersArray_.length>0) {
  	for(i=0;i<markersArray_.length;i++) {
      markersArray_[i].setMap(map);
  	}
  }
}

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}


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
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Tipo","tipo",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
    addOption(document.getElementById('agrupador'),"Campus","campus",false);
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
}

function controlarVisualizacaoEstado(estuf, exibe) {
	if(estadoPoligono2[estuf]) {
		var poligon2 = estadoPoligono2[estuf].split(",");
		for(var i=0;i<(poligon2.length-1);i++) {
			if(exibe == true) {
				nomePoli2[poligon2[i]].setMap(map);
			} else {
				nomePoli2[poligon2[i]].setMap(null);
			}
		}
	}
	
	if(estadoPoligono[estuf]) {
		var poligon = estadoPoligono[estuf].split(",");
		for(var i=0;i<(poligon.length-1);i++) {
			if(exibe == true) {
				nomePoli[poligon[i]].setMap(map);
			} else {
				nomePoli[poligon[i]].setMap(null);
			}
		}
	}
	
	if(estadoPonto[estuf]) {
		var point = estadoPonto[estuf].split(",");
		for(var i=0;i<(point.length-1);i++) {
			if(exibe == true) {
				nomePont[point[i]].setMap(map);
			} else {
				nomePont[point[i]].setMap(null);
			}
		}
	}
}

function carregarEstado(estuf, obj) {

	divCarregando();
	
	if(obj.style.backgroundColor=='') {
	
		obj.style.backgroundColor='#f6ead9';
		
		if(estadoPoligono[estuf]) {
		
			controlarVisualizacaoEstado(estuf, true);
			
		} else {
		
			carregauf2(estuf);		
			carregauf(estuf);
		
		}
		
		var filtros = carregarFiltros();
			
		if(filtro_antigo!=filtros) {
		
			apagarPontos();
			
			if(document.getElementById('educacao').value=="S") {
				marcarPontos("XMLmapaInstituicoesSuperior.php?1=1"+filtros);
			} else if(document.getElementById('educacao').value=="P") {
				marcarPontos("XMLmapaInstituicoesProfissionais.php?1=1"+filtros);
			}
			
			preencherRelatorio(filtros);
				
			filtro_antigo = filtros;
		}
		
		if(centroPoligono2[estuf]) {
			var arrCoords = centroPoligono2[estuf].split(",");
			var arr = new Array(arrCoords[0],arrCoords[1]);
			centraliza(arrCoords[2],arr);
		}
			
	} else {
		
		controlarVisualizacaoEstado(estuf, false);
		obj.style.backgroundColor='';
		
	}
	
	divCarregado();
}

/////////////////////////////////////////////


function infmunicipio(muncod){
	$.ajax({
		type: "POST",
		url: "inf_municipio.php",
		data: "muncod="+muncod,
		async: false,
		success: function(response){
			mostrainfmunicipio(response);
		}
	});
}

function mostrainfmunicipio(dados) {
	document.getElementById('inf_mun').innerHTML = dados;
}

function carregauf(estuf) {

	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "uf="+estuf,
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos(response);
		}
	});
}

function carregaMunicipio(muncod) {

	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "muncod="+muncod,
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos(response,1);
		}
	});
}

function carregauf2(estuf) {

	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "uf2="+estuf,
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos2(response);
		}
	});
}

function montarPoligonos2(response) {

	response = jQuery.parseJSON(response);
	
	$.each(response,function(index,item){
	    var centro = item.centro;
	    var arrCoord = centro.split(" ");
		var GeoJSON = jQuery.parseJSON(item.poli);
		var coords = GeoJSON.coordinates;
         var paths = [];
            for (var i = 0; i < coords.length; i++) {
                for (var j = 0; j < coords[i].length; j++) {
                    var path = [];
                    for (var k = 0; k < coords[i][j].length; k++) {
                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
                        path.push(ll);
                    }
                    paths.push(path);
                }
            } 

	
	nomePoli2[item.estuf] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000',
      strokeOpacity: 1,
      strokeWeight: 1.5,
      fillOpacity: 0,
      zIndex: 0
    });
    
	nomePoli2[item.estuf].setMap(map);
	
	if(!estadoPoligono2[item.estuf]) {
		estadoPoligono2[item.estuf]='';
	}

	estadoPoligono2[item.estuf] = estadoPoligono2[item.estuf]+item.estuf+",";
	centroPoligono2[item.estuf] = arrCoord[0]+","+arrCoord[1]+",6";
	centraliza(6,arrCoord);
	
	});
	
}


function montarPoligonos(response,carregaEstado){
	response = jQuery.parseJSON(response);
	$.each(response,function(index,item){
		var corpolyd = "#d82b40";
		var corpoly = "#f6ead9";
		var GeoJSON = jQuery.parseJSON(item.poli);
		var coords = GeoJSON.coordinates;
         var paths = [];
            for (var i = 0; i < coords.length; i++) {
                for (var j = 0; j < coords[i].length; j++) {
                    var path = [];
                    for (var k = 0; k < coords[i][j].length; k++) {
                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
                        path.push(ll);
                    }
                    paths.push(path);
                }
            } 

	corPoli[item.estuf+item.muncod] = corpoly;
	
	if(!arrMuncod[muncod]){
			
			arrMuncod[item.muncod] = item.estuf;
			
			if(nomePoli[item.estuf+item.muncod]){
				nomePoli[item.estuf+item.muncod].setMap(null);
			}
			
			nomePoli[item.estuf+item.muncod] = new google.maps.Polygon({
		      paths: paths, 
		      strokeColor: '#000000',
		      strokeOpacity: 0.6,
		      strokeWeight: 0.5,
		      fillColor: item.cor,
		      fillOpacity: 0.5,
		      zIndex: 1
		    });
		    
			nomePoli[item.estuf+item.muncod].setMap(map);
			
		}
	
	
	if(!estadoPoligono[item.estuf]) {
		estadoPoligono[item.estuf]='';
	}
	
	if(carregaEstado != 1){
		estadoPoligono[item.estuf] = estadoPoligono[item.estuf]+item.estuf+item.muncod+",";
	}
	
	google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'rightclick', function(event){showContextMenu(event.latLng);});	
	google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseover', function(event){f_mouseover(item.estuf+item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseout', function(event){f_mouseout(item.estuf+item.muncod,item.cor);});
	google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'click', function(event){infmunicipio(item.muncod);abreAcordion(1);});});
	
	
}

 function f_mouseover(obj,cor,mundescricao)
  {
  if (nomePoli[obj]){
	nomePoli[obj].setOptions( {fillColor: cor} );
	document.getElementById('nome_mun').innerHTML = mundescricao;
	}
  }

 function f_mouseout(obj,cor)
  {
  if (nomePoli[obj]){
	f_mudacor(obj,corPoli[obj]); 
	document.getElementById('nome_mun').innerHTML = '&nbsp;';
	}
  }

 function f_mudacor(obj,cor){
	corPoli[obj] = cor;
    nomePoli[obj].setOptions( {fillColor: cor} );
  }

function f_mudacores(arr, obj){
	var cor;
	var codes = arr.split(",");
	for(var i=0;i<codes.length;i++) {
		if(nomePoli[codes[i]]) {
			if(obj.checked) {nomePoli[codes[i]].setOptions( {fillColor: '#00ffff'} );} else {nomePoli[codes[i]].setOptions( {fillColor: corPoli[codes[i]]} );}
		}	
	}
}

function abreAcordion(id) {
	$("#accordion").accordion( "activate" , id );
}

function graficoHabitantes(uf) {
	window.open('academico.php?modulo=principal/mapaSupProf&acao=<?=$_REQUEST['acao'] ?>&graficohabitantes=true&uf='+uf,'Desafios','scrollbars=yes,height=650,width=650,status=no,toolbar=no,menubar=no,location=no');
}



</script>

<style>

/*
 * jQuery UI Accordion 1.8.14
 *
 * Copyright 2011, AUTHORS.txt (http://jqueryui.com/about)
 * Dual licensed under the MIT or GPL Version 2 licenses.
 * http://jquery.org/license
 *
 * http://docs.jquery.com/UI/Accordion#theming
 */
/* IE/Win - Fix animation bug - #4615 */
.ui-accordion { width: 100%; }
.ui-accordion .ui-accordion-header { cursor: pointer; position: relative; margin-top: 1px; zoom: 1; }
.ui-accordion .ui-accordion-li-fix { display: inline; }
.ui-accordion .ui-accordion-header-active { border-bottom: 0 !important; }
.ui-accordion .ui-accordion-header a { display: block; font-size: 1em; padding: 0px; }
.ui-accordion-icons .ui-accordion-header a { padding-left:0px; }
.ui-accordion .ui-accordion-header .ui-icon { position: absolute; left: .5em; top: 50%; margin-top: -8px; }
.ui-accordion .ui-accordion-content { padding: 0px; border-top: 0; margin-top: -2px; position: relative; top: 1px; margin-bottom: 2px; overflow: auto; display: none; zoom: 1; }
.ui-accordion .ui-accordion-content-active { display: block; }
.tituloAcordion{}

.contextmenu{
  visibility:hidden;
  background:#ffffff;
  border:1px solid #8888FF;
  z-index: 10;
  position: relative;
  width: 140px;
  cursor:pointer;
}
.contextmenu div{
padding-left: 5px
}

</style>


<?php 
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';
	
	$titulo = SIGLA_SISTEMA. " Maps";
	monta_titulo( $titulo, '' );

?>
<input type="hidden" name="educacao" id="educacao" value="<?=$_REQUEST['acao'] ?>">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
	<td colspan="2" align="center" bgcolor="#dedede">
	<TABLE width="100%" cellspacing="0" cellpadding="3" border ="1">
		<tr id="linha_uf">
			<?
				$sql = "SELECT	estuf, estdescricao	FROM territorios.estado	ORDER BY estuf ";
				$arrDados = $db->carregar($sql);
				if($arrDados){
					for ( $i = 0; $i < count( $arrDados ); $i++ )
						{
			?>
			<td bgcolor="#dedede" align="center" onclick="carregarEstado('<?=$arrDados[$i]['estuf'];?>', this);"><div id="estuf<?=$arrDados[$i]['estuf'];?>"><?=$arrDados[$i]['estuf'];?></div></td>
			<?
						}

				}
			?>
		</tr>
	</TABLE>
	<div id="nome_mun" style="color: black;font-family: Arial;font-size: 12pt;font-weight: bolder;">Brasil</div>
	</td>
	</tr>
</table>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td valign="top" style="width:250px;height:400px">
			
			<div id="accordion">
				<a class="tituloAcordion" href="#">FILTROS<hr></a>
				<div>
			<table cellSpacing="0" cellPadding="3" align="left" class="tabela" width="240px">
				<tr>
					<td class="SubTituloEsquerda">Busca Textual</td>
				</tr>
				<tr>
					<td>
						<input type="text" id=texto_busca name=texto_busca size=25> 
					</td>
				</tr>
				<tr>
					<td class="SubTituloEsquerda">Marcadores</td>
				</tr>
				<tr>
					<td>
						<? if($_REQUEST['acao']=="S") : ?>
						<img src="/imagens/icone_capacete_1.png"> <input onclick="if(this.checked){showOverlays(markers_superior_1);}else{clearOverlays(markers_superior_1);}" type="checkbox" name="chk_superior[]" checked="checked" title="Câmpus Preexistentes"    id="chk_superior_1" value="1"  /> <font size=1>1 - Câmpus Preexistentes</font><br/>
						<img src="/imagens/icone_capacete_3.png"> <input onclick="if(this.checked){showOverlays(markers_superior_2);}else{clearOverlays(markers_superior_2);}" type="checkbox" name="chk_superior[]" checked="checked" title="Criados em 2003/2010"    id="chk_superior_2" value="2"  /> <font size=1>2 - Criados (2003/2010)</font><br/>
						<img src="/imagens/icone_capacete_2.png"> <input onclick="if(this.checked){showOverlays(markers_superior_3);}else{clearOverlays(markers_superior_3);}" type="checkbox" name="chk_superior[]" checked="checked" title="Novos 2011/2012"         id="chk_superior_3" value="3"  /> <font size=1>3 - Novos (2011/2012)</font><br/>
						<img src="/imagens/icone_capacete_4.png"> <input onclick="if(this.checked){showOverlays(markers_superior_4);}else{clearOverlays(markers_superior_4);}" type="checkbox" name="chk_superior[]" checked="checked" title="Novos 2013/2014"         id="chk_superior_4" value="4"  /> <font size=1>4 - Novos (2013/2014)</font><br/>
						<img src="/imagens/icone_capacete_7.png"> <input onclick="if(this.checked){showOverlays(markers_superior_5);}else{clearOverlays(markers_superior_5);}" type="checkbox" name="chk_superior[]" checked="checked" title="Novas expansões"         id="chk_superior_5" value="5"  /> <font size=1>5 - Novas expansões</font><br/>
						<? else : ?>
						<img src="/imagens/icone_capacete_1.png"> <input onclick="if(this.checked){showOverlays(markers_profissional_1);}else{clearOverlays(markers_profissional_1);}" type="checkbox" name="chk_profissional[]" checked="checked" title="140 Pré existentes"                   id="chk_profissional_1" value="1"  /> <font size=1>1 - Câmpus Preexistentes</font><br/>
						<img src="/imagens/icone_capacete_3.png"> <input onclick="if(this.checked){showOverlays(markers_profissional_2);}else{clearOverlays(markers_profissional_2);}" type="checkbox" name="chk_profissional[]" checked="checked" title="214 da expansão da rede EPT"          id="chk_profissional_2" value="2"  /> <font size=1>2 - Criadas (2003/2010)</font><br/>
						<img src="/imagens/icone_capacete_2.png"> <input onclick="if(this.checked){showOverlays(markers_profissional_3);}else{clearOverlays(markers_profissional_3);}" type="checkbox" name="chk_profissional[]" checked="checked" title="81 Programadas para 2011/2012"        id="chk_profissional_3" value="3"  /> <font size=1>3 - Previstos (2011/2012)</font><br/>
						<img src="/imagens/icone_capacete_4.png"> <input onclick="if(this.checked){showOverlays(markers_profissional_4);}else{clearOverlays(markers_profissional_4);}" type="checkbox" name="chk_profissional[]" checked="checked" title="120 da expansão fase III (2013/2014)" id="chk_profissional_4" value="4"  /> <font size=1>4 - Propostos (2013/2014)</font><br/>
						<? endif; ?>
						
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
				<table width="100%">
				<tr>
					<td>
						<select id="agrupador" name="agrupador[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" class="combo campoEstilo">
						<option value="uf">UF</option>
						<option value="tipo">Tipo</option>
						<option value="municipio">Município</option>
						<option value="campus">Campus</option>
						<option value="regiao">Região</option>
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
				
				
				<tr>
					<td class="SubTituloDireita">
						<input type="button" value="Carregar" id="carregar" onclick="carregarPontos();" >
					</td>
				</tr>
			</table>
					</div>
					<a class="tituloAcordion" href="#">INFORMAÇÃO DO MUNICÍPIO<hr></a>
					<div>
						<div id="inf_mun" style="color: black;font-family: Arial;font-size: 14pt;font-weight: bolder;width:230px"></div>
					</div>
					<a class="tituloAcordion" href="#">RELATÓRIO QUANTITATIVO<hr></a>
					<div>
						<div style="overflow:auto;height:398px" id="colunainfo"></div>
					</div>
					<a class="tituloAcordion" href="#">ROTAS<hr></a>
					<div>
						<table class="tabela" width="100%" >
							<tr>
								<td class="SubtituloDireita" style="font-weight:bold" >Origem:</td>
								<td>
									<input type="text" size="30" id="start" name="from" value=""/>
									<input type="hidden" id="start_lat" name="start_lat" value=""/>
									<input type="hidden" id="start_lng" name="start_lng" value=""/>
								</td>
							</tr>
							<tr>
								<td class="SubtituloDireita" style="font-weight:bold" >Destino:</td>
								<td>
									<input type="text" size="30" id="end" name="to" value="" />
									<input type="hidden" id="end_lat" name="end_lat" value=""/>
									<input type="hidden" id="end_lng" name="end_lng" value=""/>
								</td>
							</tr>
							<tr>
								<td class="SubtituloEsquerda" colspan="2"><input name="submit" type="button" onclick="calcRoute()" value="Exibir Rota" />   <input name="submit" type="button" onclick="removeRoute()" value="Remover Rota" /> </td>
							</tr>
						</table>
					  <div id="directionsPanel" style="width: 205px"></div>
					  <div id="warnings_panel" style="width: 205px"></div>
					</div>
					<a class="tituloAcordion" href="#">LOCALIZAR MUNICÍPIO<hr></a>
					<div>
						<table class="tabela" width="100%" >
							<tr>
							    <td align='right' class="SubTituloDireita"><b>Município:</b></td>
							    <td>
							    	<?=campo_texto('busca_municipio','N','S',30,20,'','','','','','','id="busca_municipio"','BuscaMunicipioEnter2(event);') ?>
							    	 <input type="button" value="OK" onclick="BuscaMunicipio2();">
							    </td>
							</tr>
							<tr>
								<td colspan=2>
									<div id="resultado_pesquisa">
									</div>
								</td>
							</tr>
						</table>
					</div>
			</div>

					<? if($_REQUEST['acao'] == "S"): ?>
					<a style="cursor:pointer;" onclick="graficoHabitantes('');">GRÁFICO VAGAS / 10.000 HABITANTES</a>
					<? endif; ?>

			
		</td>
		<td valign="top">
		<div style="position:absolute;z-index:3;text-align:center;padding:1px;width:60px;margin-left:80px;margin-top:10px;"><input type="button" value="Brasil"  onclick="centralizaBrasil()" /></div>
		<div id="map_canvas" style="width:100%;height:600px;position:relative;"></div>
		</td>
	</tr>
</table>
<script>
	function BuscaMunicipioEnter2(e)
	{
	    if (e.keyCode == 13)
	    {
	        BuscaMunicipio2();
	    }
	}

	function BuscaMunicipio2(){
		var mun = document.getElementById('busca_municipio');

		if(!mun.value){
			alert('Digite o Município para busca.');
			return false;
		}

		if(mun.value){
			document.getElementById('resultado_pesquisa').innerHTML = "<center>Carregando...</center>";
			$.ajax({
				type: "POST",
				url: 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/painel/_funcoes_mapa_painel_controle.php?redefederal=1',
				data: 'BuscaMunicipioAjax=' + mun.value,
				async: false,
				success: function(response){
					document.getElementById('resultado_pesquisa').innerHTML = response;
				}
			});

		}
	}
	function localizaMapa2(muncod,latitude,longitude){
				
		var myLatLng = new Array();
		myLatLng[0] = latitude;
		myLatLng[1] = longitude;
		centraliza(parseInt(8), myLatLng);
		infmunicipio(muncod);
		if(!arrMuncod[muncod]){
			carregaMunicipio(muncod);
		}
		abreAcordion(1);
		nomePoli[arrMuncod[muncod]+muncod].setOptions( {fillColor: '#F0F'} );
	}
	
	initialize();
</script>
<?php if($_GET['cockpit']): ?>
<script>carregarPontos();</script>
<?php endif; ?>