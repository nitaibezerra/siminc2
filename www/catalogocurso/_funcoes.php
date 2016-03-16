<?php 
function pegaEstadoAtual( $docid ) {
	
	global $db; 
	
	if($docid) {
		$docid = (integer) $docid;
		 
		$sql = "
			select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estado = $db->pegaUm( $sql );
		 
		return $estado;
	} else {
		return false;
	}
}

function preparaData($data,$delimitador){
	$data = explode($delimitador,$data);
	$data = $data[2].'/'.$data[1].'/'.$data[0];
	return $data;
}

function monta_abas($cod = null){
	global $db,$abacod_tela,$url;
	$abacod_tela = $cod ? $cod : $abacod_tela;
	echo "<br>";
	echo $db->cria_aba($abacod_tela,$url,$parametros);
}

function preCriarDocumento( $curid, $tpdid = 52 ) {
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$docid = prePegarDocid( $curid );
	
	if( !$docid ) {
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					catalogocurso.curso
				SET 
					docid = {$docid} 
				WHERE
					curid = {$curid}";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function prePegarDocid( $curid ) {
	
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				catalogocurso.curso
			WHERE
			 	curid = " . (integer) $curid;
	
	return (integer) $db->pegaUm( $sql );
	
}

function salvarCatalogo($post){
	
	global $db,$_FILES;
	
	extract($post);
	
	$cursalamulti = $cursalamulti == 't' ? 'true' : 'false';
	$curid = $curid ? $curid : $_SESSION['catalogo']['curid'];
	$ateid = $ateid ? $ateid : 'null';
	$curdesc = $curdesc ? "'".$curdesc."'" : 'null';
	$curementa = $curementa ? "'".$curementa."'" : 'null';
	$curfunteome = $curfunteome ? "'".$curfunteome."'" : 'null';
	$curobjetivo = $curobjetivo ? "'".$curobjetivo."'" : 'null';
	$curmetodologia = $curmetodologia ? "'".$curmetodologia."'" : 'null';
	$curchmim = $curchmim ? $curchmim : 'null';
	$curchmax = $curchmax ? $curchmax : 'null';
	$curpercpremim = $curpercpremim ? $curpercpremim : 'null';
	$curpercpremax = $curpercpremax ? $curpercpremax : 'null';
	$curcertificado = $curcertificado ? "'".$curcertificado."'" : 'null';
	$curnumestudanteidealpre = $curnumestudanteidealpre != '' ? $curnumestudanteidealpre : 'null';
	$curnumestudanteminpre = $curnumestudanteminpre != '' ? $curnumestudanteminpre : 'null';
	$curnumestudantemaxpre = $curnumestudantemaxpre != '' ? $curnumestudantemaxpre : 'null';
	$curnumestudanteidealdist = $curnumestudanteidealdist != '' ? $curnumestudanteidealdist : 'null';
	$curnumestudantemindist = $curnumestudantemindist != '' ? $curnumestudantemindist : 'null';
	$curnumestudantemaxdist = $curnumestudantemaxdist != '' ? $curnumestudantemaxdist : 'null';
	$curinicio = $curinicio ? "'".preparaData($curinicio, '/')."'" : 'null';
	$curfim = $curfim ? "'".preparaData($curfim, '/')."'" : 'null';
	$curqtdmonitora = $curqtdmonitora ? $curqtdmonitora : 'null';
	$uteid = $uteid ? $uteid : 'null';
	$ncuid = $ncuid ? $ncuid : 'null';
	$curofertanacional = $curofertanacional ? $curofertanacional : 'null';
	$curcustoaluno = $curcustoaluno ? $curcustoaluno : 'null';
	$curinfra = $curinfra ? $curinfra : 'null';
	$lesid = $lesid ? $lesid : 'null';
	$ldeid = $ldeid ? $ldeid : 'null';
	$coordid = $coordid ? $coordid : 'null';
	
	if($curid!=''){
		
		$antcurchmim = $db->pegaUm("SELECT curchmim
									   FROM catalogocurso.curso 
									   WHERE curid = ".$curid);
		$antcurchmax = $db->pegaUm("SELECT curchmax
									   FROM catalogocurso.curso 
									   WHERE curid = ".$curid);
		
		if( ($antcurchmim != ($curchmim) ||
			 $antcurchmax != ($curchmax) ) ){
			$sql = "UPDATE catalogocurso.organizacaocurso SET 
						orcstatus = 'I'
					WHERE
						curid = ".$curid;
			$db->executar($sql);
			$db->commit();
		}
		
		$sql = "UPDATE catalogocurso.curso SET
					ateid = $ateid,
					curdesc = $curdesc,
					ncuid = $ncuid,
					curementa = $curementa,
					curfunteome = $curfunteome,
					curobjetivo = $curobjetivo,
					curmetodologia = $curmetodologia,
					curchmim = $curchmim,
					curchmax = $curchmax,
					curpercpremim = $curpercpremim,
					curpercpremax = $curpercpremax,
					curcertificado = $curcertificado,
					curnumestudanteidealpre = $curnumestudanteidealpre,
					curnumestudanteminpre = $curnumestudanteminpre,
					curnumestudantemaxpre = $curnumestudantemaxpre,
					curnumestudanteidealdist = $curnumestudanteidealdist,
					curnumestudantemindist = $curnumestudantemindist,
					curnumestudantemaxdist = $curnumestudantemaxdist,
					curinicio = $curinicio,
					curfim = $curfim,
					curqtdmonitora = $curqtdmonitora,
					uteid = $uteid,
					curofertanacional = $curofertanacional,
					curcustoaluno = ".str_replace(Array('.',','),Array('','.'),$curcustoaluno).",
					curinfra = '$curinfra',
					--co_interno_uorg = $co_interno_uorg,
					lesid = $lesid,
					ldeid = $ldeid,
					cursalamulti = $cursalamulti,
					coordid = $coordid
				WHERE
					curid = $curid
				RETURNING 
					curid";
		
		$curid = $db->pegaUm($sql);
	}else{
		$sql = "INSERT INTO catalogocurso.curso (
					ateid, curdesc, ncuid, curementa, curfunteome, curobjetivo,
					curmetodologia, curchmim, curchmax, curpercpremim, curpercpremax, curcertificado,
					curnumestudanteidealpre, curnumestudanteminpre, curnumestudantemaxpre, curnumestudanteidealdist, curnumestudantemindist, 
					curnumestudantemaxdist, curinicio, curfim, curqtdmonitora, uteid, curofertanacional, curcustoaluno,
					curinfra, lesid, ldeid, cursalamulti, coordid)
				VALUES(
					$ateid, $curdesc, $ncuid, $curementa, $curfunteome, $curobjetivo,
					$curmetodologia, $curchmim, $curchmax, $curpercpremim, $curpercpremax, $curcertificado,
					$curnumestudanteidealpre, $curnumestudanteminpre, $curnumestudantemaxpre, $curnumestudanteidealdist, 
					$curnumestudantemindist, $curnumestudantemaxdist, $curinicio, $curfim, $curqtdmonitora, $uteid, $curofertanacional, 
					".str_replace(Array('.',','),Array('','.'),$curcustoaluno).",
					'$curinfra', $lesid, $ldeid, $cursalamulti, $coordid)
				RETURNING
					curid";
		$curid = $db->pegaUm($sql);
		$pflcod    = pegaPerfil($_SESSION['usucpf']);
		$sql = "INSERT INTO catalogocurso.usuarioresponsabilidade(pflcod,usucpf,curid) VALUES($pflcod,'".str_pad( $_SESSION['usucpf'],11,'0')."',$curid)";
		$db->executar($sql);
	}
	
	preCriarDocumento( $curid );
	
	$sql = "INSERT INTO catalogocurso.historicocurso(usucpf,curid) VALUES ('".$_SESSION['usucpf']."',$curid)";
	$db->executar($sql);
	
	$sql = "DELETE FROM catalogocurso.etapaensino_curso WHERE curid = $curid;";
	if( $cod_etapa_ensino[0] != '' ){
		foreach($cod_etapa_ensino as $id){
			$sql .= "INSERT INTO catalogocurso.etapaensino_curso (cod_etapa_ensino,curid) VALUES ($id, $curid) ;";
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.nivelcurso_curso WHERE curid = $curid;";
	if(is_array($ncuid) && count($ncuid) > 0 ){
		foreach($ncuid as $id){
			$sql .= "INSERT INTO catalogocurso.nivelcurso_curso(ncuid,curid) VALUES ($id, $curid) ;";
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.cursorede WHERE curid = $curid;";
	if(is_array($redid) && count($redid) > 0 ){
		foreach($redid as $id){
			$sql .= "INSERT INTO catalogocurso.cursorede(redid,curid) VALUES ($id, $curid) ;";
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.modalidadecurso_curso WHERE curid = $curid;";
	if(is_array($modid) && count($modid) > 0 ){
		foreach($modid as $id){
			$sql .= "INSERT INTO catalogocurso.modalidadecurso_curso(modid,curid) VALUES ($id, $curid) ;";
		}
	}

	if(is_array($arqdsc_old)){
		foreach($arqdsc_old as $k => $old){
			if( $arqdsc[$k] != $old ){
				$sql .= "UPDATE catalogocurso.arquivocurso SET arcdesc = '$arqdsc[$k]' WHERE arcid = $k;";
			}
		}
	}
	$erro = false;
	if($db->executar($sql)){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		if(is_array($_FILES)){
			foreach($_FILES as $k => $file){
				$campos	= array("curid"	    => $curid,
								"arcdesc"	=> "'".$arqdsc[substr($k, 3)]."'",
								"arcstatus" =>"'A'");
		
				$file = new FilesSimec("arquivocurso", $campos,"catalogocurso");
				
				if(is_file($_FILES[$k]["tmp_name"]) && !$erro){	
					$arquivoSalvo = $file->setUpload("arquivo brasilpro",$k);
					$erro = false;
				}else{
					$erro = true;
				}
			}
			
		}
		if($erro){
			$db->rollback();
			echo "<script>
					alert('Erro ao gravar');
				  </script>";
		}else{
			$_SESSION['catalogo']['curid'] = $curid;
			$db->commit();
			if($post['link'] == 'proximo'){
				$link = '\'catalogocurso.php?modulo=principal/cadOrganizacaoCurso&acao=A\'';
			}else{
				if($_SESSION['catalogo']['curid']){
					$link = 'window.location';
				}else{
					$link = '\'catalogocurso.php?modulo=inicio&acao=C\'';
				}
			}
			echo "<script>
					alert('Dados salvos.');
					window.location = $link;
				  </script>";
		}
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao gravar');
			  </script>";
	}
}

function recuperaDadosCurso($request){
	
	global $db;
	
	if($request['curid']){
		$sql = "SELECT
					c.curid,
					curstatus, 
					ateid, 
					curdesc, 
					ncuid, 
					curementa, 
					curfunteome,
					curobjetivo,
					curmetodologia,
					--modid, 
					curchmim, 
					curchmax, 
					curpercpremim,
					curpercpremax,
					curcertificado,
					curnumestudanteidealpre, 
					curnumestudanteminpre, 
					curnumestudantemaxpre,
					curnumestudanteidealdist, 
					curnumestudantemindist, 
					curnumestudantemaxdist,
					to_char(curinicio,'DD/MM/YYYY') as curinicio,
					to_char(curfim,'DD/MM/YYYY') as curfim,
					curofertanacional,
					curcustoaluno,
					curqtdmonitora,
					uteid,
					curinfra,
					usunome, 
					to_char(hicdata,'DD/MM/YYY - HH24:MI:SS') as hicdata,
					--co_interno_uorg,
					cursalamulti,
					lesid,
					ldeid,
					coordid
				FROM
					catalogocurso.curso c 
				LEFT JOIN catalogocurso.etapaensino e ON e.eteid = c.eteid
				LEFT JOIN catalogocurso.historicocurso h ON h.curid = c.curid
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
				WHERE
					c.curid = ".$request['curid'];
		$curso = $db->pegaLinha($sql);
		
		$sql = "SELECT 
					e.pk_cod_etapa_ensino as codigo,
					e.no_etapa_ensino as descricao
				FROM 
					catalogocurso.etapaensino_curso ec
				INNER JOIN educacenso_".(ANO_CENSO).".tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino
				WHERE
					ec.curid = ".$request['curid'];
		$curso['cod_etapa_ensino'] = $db->carregar($sql);
		
		$sql = "SELECT 
					r.redid as codigo
				FROM 
					catalogocurso.cursorede cr
				INNER JOIN catalogocurso.rede r ON r.redid = cr.redid
				WHERE
					curid = ".$request['curid'];
		$curso['redid'] = $db->carregarColuna($sql);
		
		$sql = "SELECT 
					mod.modid as codigo
				FROM 
					catalogocurso.modalidadecurso_curso mcu
				INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = mcu.modid
				WHERE
					mcu.curid = ".$request['curid'];
		$curso['modid'] = $db->carregarColuna($sql);
		
		return $curso;
	}
}

function recuperaModalidadePublicoAlvo($request){
	
	global $db;
	
	if($request['curid']){
		$sql = "SELECT 
					moc.cod_mod_ensino as codigo
				FROM 
					catalogocurso.tab_mod_ensino_curso moc
				INNER JOIN educacenso_".(ANO_CENSO).".tab_mod_ensino mod ON mod.pk_cod_mod_ensino = moc.cod_mod_ensino
				WHERE
					moc.curid = ".$request['curid'];
		return $db->carregarColuna($sql);
	}
}

function recuperaRedesCurso($request){
	
	global $db;
	
	if($request['curid']){
		$sql = "SELECT 
					redid
				FROM 
					catalogocurso.cursorede
				WHERE
					crestatus = 'A' AND
					curid = ".$request['curid'];
		return $db->carregarColuna($sql);
	}
}

function recuperaArquivos($request){
	
	global $db;
	
	if($request['curid']){
		
		$arrPflcods = Array();
		$arrCurids  = Array();
		$arrCoords  = Array();
		$pflcods    = pegaPerfis($_SESSION['usucpf']);
		if( !$db->testa_superuser() && !in_array(PERFIL_CONSULTA,$pflcods) && !in_array(PERFIL_ADMINISTRADOR,$pflcods) ){
			$arrCoords = recuperaCoordenacaoResponssavel();
			$arrCurids = recuperaCursoResponssavel();
			array_push($arrCoords,'0');
			array_push($arrCurids,'0');
			$pflcod     = pegaPerfil($_SESSION['usucpf']);
			$arrPflcods = Array(PERFIL_COORDENADOR,
								PERFIL_GESTOR);
		}
		$coordid = $db->pegaUm('SELECT coordid FROM catalogocurso.curso WHERE curid = '.$request['curid']);
		$excluir = false;
		if( $db->testa_superuser() || in_array(PERFIL_ADMINISTRADOR,$pflcods)  ){
			$excluir = true;
		}elseif( in_array($coordid,$arrCoords) || in_array($request['curid'],$arrCurids) ){
			$excluir = true;
		}
		
		$sql = "SELECT 
					arcid, 
					arcdesc,
					a.arqid,
					a.arqnome,
					curid
				FROM 
					catalogocurso.arquivocurso c
				INNER JOIN public.arquivo a ON a.arqid = c.arqid
				WHERE
					arcstatus = 'A' AND
					curid = ".$request['curid'];
		$arquivos = $db->carregar($sql);
		if(is_array($arquivos)){
			foreach($arquivos as $k=>$arquivo){
				echo '<tr class="linha" id="arq'.$arquivo['arcid'].'" name="'.$arquivo['arcid'].'">'.
						'<td style="border-bottom: 1px solid #cccccc;">'.
							'<input type="text" class=" normal" title="" onblur="MouseBlur(this);" 
							onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
							onmouseover="MouseOver(this);"  
							value="'.$arquivo['arcdesc'].'" maxlength="50" size="51" name="arqdsc['.$arquivo['arcid'].']" id="'.$arquivo['arcid'].'" style="text-align:left;">'.
							'<input type="hidden" name="arqdsc_old['.$arquivo['arcid'].']"/>'.
						'</td>'.
						'<td style="border-bottom: 1px solid #cccccc;">'.
							'<a onclick="abreArquivo('.$arquivo['arqid'].')">'.$arquivo['arqnome'].$arquivo['arcid'].'</a>'.
						'</td>'.
						'<td style="border-bottom: 1px solid #cccccc;">'.
							'<center>'.
								($excluir?'<img src="../imagens/excluir.gif" title="Excluir" class="excluirarq" name="arq'.$arquivo['arcid'].'" id="'.$arquivo['arcid'].'" />':'').
							'</center>'.
						'</td>'.
					'</tr>';
			}
		}
	}
}

function abreArquivo($request){
	
	ob_clean();
	
	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	$file = new FilesSimec(NULL, NULL, 'catalogocurso');
	$file->getDownloadArquivo($request['arqid']);
	echo "<script>
			window.close();
		  </script>";
	exit();
}

function excluirArquivo($request){
	
	global $db;
	
	$sql = "UPDATE catalogocurso.arquivocurso SET
				arcstatus = 'I'
			WHERE
				arcid = ".$request['arcid'];
	if($db->executar($sql)){
		$db->commit();
		return true;
	}else{
		$db->rollback();
		return false;
	}
}

function excluirCurso($request){
	
	global $db;
	
	$sql = "UPDATE catalogocurso.curso SET curstatus = 'I' WHERE curid = ".$_REQUEST['curid'];
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Curso excluído.');
			window.location = 'catalogocurso.php?modulo=inicio&acao=C';
		  </script>";

	return true;
}

//// INICIO EQUIPE

function salvarEquipe($request){

	global $db;
	
	extract($request);
	
	$curid = $_SESSION['catalogo']['curid'];
	
	$camid 			= $camid ? $camid : 'null'; 
	$eqcbolsista 	= $eqcbolsista ? $eqcbolsista : 'FALSE'; 
	$qtdfuncao 		= $qtdfuncao ? $qtdfuncao : 'null';
	$eqcfuncao 		= $eqcfuncao ? "'".$eqcfuncao."'" : 'null';
	$eqcminimo 		= $eqcminimo ? $eqcminimo : 'null';
	$eqcmaximo 		= $eqcmaximo ? $eqcmaximo : 'null';
	$unrid 			= $unrid ? $unrid : 'null';
	$qtdfuncao 		= $qtdfuncao ? $qtdfuncao : 'null';
	$eqcatribuicao 	= $eqcatribuicao ? "'".$eqcatribuicao."'" : 'null';
	$eqcoutrosreq 	= $eqcoutrosreq ? "'".$eqcoutrosreq."'" : 'null';
	
	$eqcvalorunitario 	= $eqcvalorunitario ? "$eqcvalorunitario" : 'null';
	
	$eqcvalorunitario = formata_valor_sql($eqcvalorunitario);
	
	$eqccargahorariames	= $eqccargahorariames ? "'".$eqccargahorariames."'" : 'null';
	
	//$cod_escolaridade = $cod_escolaridade ? $cod_escolaridade : 'null';
	
	if( $eqcid == '' ){	
		
		$sql = "INSERT INTO catalogocurso.equipecurso(
			            camid, eqcbolsista, eqcfuncao, eqcminimo, eqcmaximo, unrid,   
			            eqcatribuicao, eqcoutrosreq, curid, qtdfuncao, eqcvalorunitario, eqccargahorariames)
			    VALUES ($camid, '$eqcbolsista', $eqcfuncao, $eqcminimo, $eqcmaximo, $unrid,    
			            $eqcatribuicao, $eqcoutrosreq, ".$_SESSION['catalogo']['curid'].", $qtdfuncao, $eqcvalorunitario, $eqccargahorariames)
			    RETURNING
			    	eqcid";
	}else{
		
		$sql = "UPDATE catalogocurso.equipecurso SET 
			    	camid = $camid, eqcbolsista = '$eqcbolsista', eqcfuncao = $eqcfuncao, eqcminimo = $eqcminimo, eqcmaximo = $eqcmaximo, unrid = $unrid, 
			    	eqcatribuicao = $eqcatribuicao, eqcoutrosreq = $eqcoutrosreq, qtdfuncao = $qtdfuncao, eqcvalorunitario = $eqcvalorunitario,
			    	eqccargahorariames = $eqccargahorariames
			 	WHERE 
			 		eqcid =".$eqcid."
			    RETURNING
			    	eqcid";
		
	}
	$eqcid = $db->pegaUm($sql);
	
	$sql = "DELETE FROM catalogocurso.escolaridade_equipe WHERE eqcid = $eqcid;";
	if(is_array($cod_escolaridade) && count($cod_escolaridade) > 0 ){
		foreach($cod_escolaridade as $id){
			$sql .= "INSERT INTO catalogocurso.escolaridade_equipe(cod_escolaridade,eqcid) VALUES ($id, $eqcid) ;";
		}
	}
	
	if($db->executar($sql)){
		$db->commit();
		if($request['link'] == 'proximo'){
			$link = '\'catalogocurso.php?modulo=principal/cadPublicoAlvo&acao=A\'';
		}else{
			$link = '\'catalogocurso.php?modulo=principal/cadEquipe&acao=A\'';
		}
		echo "<script>
				alert('Dados salvos.');
				window.location = $link;
			  </script>";
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao gravar.');
				window.location = 'catalogocurso.php?modulo=principal/cadEquipe&acao=A';
			  </script>";
	}

}

function excluirEquipe($request){
	
	global $db;
	
	$sql = "UPDATE catalogocurso.equipecurso SET 
				eqcstatus = 'I'
			WHERE
				eqcid = ".$request['eqcid'];
	
	if($db->executar($sql)){
		$db->commit();
		echo "<script>
				alert('Equipe excuída.');
				window.location = window.location;
			  </script>";
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao excluir.');
				window.location = window.location;
			  </script>";
	}
}

function recuperaDadosEquipe($request){
	
	global $db;
	
	if($request['eqcid']){
		$sql = "SELECT 
					camid, 
					unrid, 
					cod_escolaridade, 
					eqcfuncao, 
					eqcminimo, 
					eqcmaximo, 
			        eqcatribuicao, 
			        eqcoutrosreq,  
			        eqcbolsista,
			        qtdfuncao,
			        to_char(eqcvalorunitario, '999G999G999D99') as eqcvalorunitario,
			        eqccargahorariames
				FROM 
					catalogocurso.equipecurso
				WHERE
					eqcid = ".$request['eqcid']."
					AND curid = ".$_SESSION['catalogo']['curid'];
		
		$equipe = $db->pegaLinha($sql);
		
		$sql = "SELECT
					foo.codigo,
					foo.descricao
				FROM
					((SELECT 
						to_char(pk_cod_escolaridade,'9') as codigo, 
						pk_cod_escolaridade||' - '||no_escolaridade as descricao,
						h.nivel
		  			FROM 
		  				educacenso_".(ANO_CENSO).".tab_escolaridade e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade)
		  			UNION ALL
		  			(SELECT 
						to_char(pk_pos_graduacao,'9')||'0' as codigo, 
						pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao,
						h.nivel
					FROM 
						educacenso_".(ANO_CENSO).".tab_pos_graduacao e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer)) as foo
				INNER JOIN catalogocurso.escolaridade_equipe eeq ON eeq.cod_escolaridade = foo.codigo::integer AND eeq.eqcid = ".$request['eqcid'];
		$equipe['cod_escolaridade'] = $db->carregar($sql);
		
		return $equipe;
	}
}

// Publico alvo

function salvarPublicoAlvo($request){

	global $db;
	
	extract($request);

	$curid = $_SESSION['catalogo']['curid'];

	$panesid = $nesid ? $nesid : 'null';
//	$paeteid = $cod_etapa_ensino ? $cod_etapa_ensino : 'null';
	$pamodid = $modid ? $modid : 'null';
	$curpaoutrasexig = $curpaoutrasexig ? "'".$curpaoutrasexig."'" : 'null';
	$curpatutor = $curpatutor == 'S' ? 'true' : 'false';
	$curpatutortxt = $curpatutortxt ? "'".$curpatutortxt."'" : 'null';
	$curpademsocial = $curpademsocial == 'S' ? 'true' : 'false';
	$curpafuncao = $curpafuncao == 'S' ? 'true' : 'false';
	$pacod_escolaridade = $pacod_escolaridade ? $pacod_escolaridade : 'null';
	$pacod_mod_ensino = $pacod_mod_ensino ? $pacod_mod_ensino : 'null';
	$curpademsocialpercmax = $curpademsocialpercmax && ($curpademsocial == 'true') ? $curpademsocialpercmax : 'null';
		
	$sql = "UPDATE catalogocurso.curso SET 
				panesid = $panesid, 
		        curpaoutrasexig = $curpaoutrasexig, 
		        curpatutor = $curpatutor, 
		        curpatutortxt = $curpatutortxt, 
		        curpademsocial = $curpademsocial, 
		        pacod_escolaridade = $pacod_escolaridade,
		        pacod_mod_ensino = $pacod_mod_ensino,
		        curpademsocialpercmax = $curpademsocialpercmax
			WHERE 
				curid = $curid;";

	$sql .= "DELETE FROM catalogocurso.areaformacaocurso WHERE curid = $curid;";
	if(is_array($cod_area_ocde) && $cod_area_ocde[0] != ''){
		if( in_array('999',$cod_area_ocde) ){
			$sql .= "INSERT INTO catalogocurso.areaformacaocurso(cod_area_ocde,curid) VALUES ('999', $curid) ;";
		}else{
			foreach($cod_area_ocde as $id){
				$sql .= "INSERT INTO catalogocurso.areaformacaocurso(cod_area_ocde,curid) VALUES ('$id', $curid) ;";
			}
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.diciplinacurso WHERE curid = $curid;";
	if(is_array($cod_disciplina) && $cod_disciplina[0] != ''){
		if( in_array('999',$cod_disciplina) ){
			$sql .= "INSERT INTO catalogocurso.diciplinacurso(cod_disciplina,curid) VALUES (999, $curid) ;";
		}else{
			foreach($cod_disciplina as $id){
				$sql .= "INSERT INTO catalogocurso.diciplinacurso(cod_disciplina,curid) VALUES ($id, $curid) ;";
			}
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.etapaensino_curso_publicoAlvo WHERE curid = $curid;";
	if( $cod_etapa_ensino[0] != '' ){
		if( in_array('999',$cod_etapa_ensino) ){
			$sql .= "INSERT INTO catalogocurso.etapaensino_curso_publicoAlvo (cod_etapa_ensino,curid) VALUES (999, $curid) ;";
		}else{
			foreach($cod_etapa_ensino as $id){
				$sql .= "INSERT INTO catalogocurso.etapaensino_curso_publicoAlvo (cod_etapa_ensino,curid) VALUES ($id, $curid) ;";
			}
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.funcaoexercida_curso_publicoAlvo WHERE curid = $curid;";
	if( $fexid[0] != '' ){
		if( in_array('999',$fexid) ){
			$sql .= "INSERT INTO catalogocurso.funcaoexercida_curso_publicoAlvo (fexid,curid) VALUES (999, $curid) ;";
		}else{
			foreach($fexid as $id){
				$sql .= "INSERT INTO catalogocurso.funcaoexercida_curso_publicoAlvo (fexid,curid) VALUES ($id, $curid) ;";
			}
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.tab_mod_ensino_curso WHERE curid = $curid;";
	if( $cod_mod_ensino[0] != '' ){
		if( in_array('999',$cod_mod_ensino) ){
			$sql .= "INSERT INTO catalogocurso.tab_mod_ensino_curso (cod_mod_ensino,curid) VALUES (999, $curid) ;";
		}else{
			foreach($cod_mod_ensino as $id){
				$sql .= "INSERT INTO catalogocurso.tab_mod_ensino_curso (cod_mod_ensino,curid) VALUES ($id, $curid) ;";
			}
		}
	}
	
	$sql .= "DELETE FROM catalogocurso.cursodemandasocial WHERE curid = $curid;";
	if( $padid[0] != '' && ($curpademsocial == 'true') ){
		if( in_array('999',$padid) ){
			$sql .= "INSERT INTO catalogocurso.cursodemandasocial (padid,curid) VALUES (999, $curid) ;";
		}else{
			foreach($padid as $id){
				$sql .= "INSERT INTO catalogocurso.cursodemandasocial (padid,curid) VALUES ($id, $curid) ;";
			}
		}
	}
	
	$eqcid = $db->pegaUm($sql);
	
	$sql = "DELETE FROM catalogocurso.publicoalvo_curso WHERE curid = $curid;";
	if(is_array($cod_escolaridade) && count($cod_escolaridade) > 0 ){
		if( in_array('999',$cod_escolaridade) ){
			$sql .= "INSERT INTO catalogocurso.publicoalvo_curso(cod_escolaridade,curid) VALUES (999, $curid) ;";
		}else{
			foreach($cod_escolaridade as $id){
				$sql .= "INSERT INTO catalogocurso.publicoalvo_curso(cod_escolaridade,curid) VALUES ($id, $curid) ;";
			}
		}
	}

	if($db->executar($sql)){
		$db->commit();
		if($request['link'] == 'proximo'){
			$link = '\'catalogocurso.php?modulo=principal/cadContato&acao=A\'';
		}else{
			$link = 'window.location';
		}
		echo "<script>
				alert('Dados salvos.');
				window.location = $link;
			  </script>";
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao gravar.');
				window.location = window.location;
			  </script>";
	}

}

function recuperaDadosPublicoAlvo($request){
	
	global $db;
	
	$curid = $_SESSION['catalogo']['curid'];
	
	if($curid){
		$sql = "SELECT 
					panesid as nesid, 
					paeteid as cod_etapa_ensino,  
					curpaoutrasexig, 
					curpatutor, 
					curpatutortxt, 
       				curpademsocial, 
       				pacod_escolaridade,
       				curpademsocialpercmax
				FROM 
					catalogocurso.curso
				WHERE
					curid = ".$curid;
		$retorno = $db->pegaLinha($sql);

		$sql = "SELECT 
					pk_cod_area_ocde as codigo, 
					no_nome_area_ocde as descricao
				FROM 
					educacenso_2010.tab_area_ocde t
				INNER JOIN catalogocurso.areaformacaocurso a ON a.cod_area_ocde = t.pk_cod_area_ocde
				WHERE
					a.curid = $curid";
		$area = $db->carregar($sql);
		
		$sql = "SELECT 
					pk_cod_disciplina as codigo, 
					no_disciplina as descricao
				FROM 
					educacenso_2010.tab_disciplina t
				INNER JOIN catalogocurso.diciplinacurso a ON a.cod_disciplina = t.pk_cod_disciplina
				WHERE
					a.curid = $curid";
		$disc = $db->carregar($sql);
		
		$sql = "SELECT 
					e.pk_cod_etapa_ensino as codigo,
					e.no_etapa_ensino as descricao
				FROM 
					catalogocurso.etapaensino_curso_publicoAlvo ec
				INNER JOIN educacenso_2010.tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino
				WHERE
					ec.curid = ".$curid;
		$retorno['cod_etapa_ensino'] = $db->carregar($sql);
		
		$sql = "SELECT 
					e.fexid as codigo,
					e.fexdesc as descricao
				FROM 
					catalogocurso.funcaoexercida_curso_publicoAlvo ec
				INNER JOIN catalogocurso.funcaoexercida e ON e.fexid = ec.fexid
				WHERE
					ec.curid = ".$curid;
		$retorno['fexid'] = $db->carregar($sql);
		
		$retorno["cod_area_ocde"] = $area;
		$retorno["cod_disciplina"] = $disc;
		
		$sql = "SELECT
					foo.codigo,
					foo.descricao
				FROM
					((SELECT 
						to_char(pk_cod_escolaridade,'9') as codigo, 
						pk_cod_escolaridade||' - '||no_escolaridade as descricao,
						h.nivel
		  			FROM 
		  				educacenso_2010.tab_escolaridade e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade)
		  			UNION ALL
		  			(SELECT 
						to_char(pk_pos_graduacao,'9')||'0' as codigo, 
						pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao,
						h.nivel
					FROM 
						educacenso_2010.tab_pos_graduacao e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer)) as foo
				INNER JOIN catalogocurso.publicoalvo_curso eeq ON eeq.cod_escolaridade = foo.codigo::integer AND eeq.curid = ".$request['curid'];
		$retorno['cod_escolaridade'] = $db->carregar($sql);
		
		$sql = "SELECT
					tmod.pk_cod_mod_ensino
				FROM
					catalogocurso.tab_mod_ensino_curso mod
				INNER JOIN educacenso_2010.tab_mod_ensino tmod ON tmod.pk_cod_mod_ensino = mod.cod_mod_ensino
				WHERE
					curid = ".$curid;
		$retorno['cod_mod_ensino'] = $db->carregarColuna($sql);
		
		$sql = "SELECT
					pad.padid as codigo,
					pad.paddesc as descricao
				FROM
					catalogocurso.cursodemandasocial cms
				INNER JOIN catalogocurso.publicoalvodemandasocial pad ON pad.padid = cms.padid 
				WHERE
					curid = ".$curid;
		$retorno['padid'] = $db->carregar($sql);

		return $retorno;
	}
}

// Organização

function recuperaOrganizacaoCurso( $post ){
	
	global $db;
	
	$sql = "SELECT 
				orcdesc, 
				orcchmim, 
				orcchmax, 
				orcpercpremim, 
				orcpercpremax, 
				orcementa, 
				modid
			FROM 
				catalogocurso.organizacaocurso
			WHERE
				orcid = ".$post['orcid']."
				AND orcstatus = 'A'
				AND curid = ".$_SESSION['catalogo']['curid'];
	
	return $db->pegaLinha($sql);
}

function salvarOrganizacaoCurso($request){

	global $db;
	
	extract($request);
	
	$curid = $_SESSION['catalogo']['curid'];

	$tioid = $tioid ? $tioid : 'null';
	$orcdesc = $orcdesc ? "'".$orcdesc."'" : 'null';
	$orcchmim = $orcchmim ? $orcchmim : 'null';
	$orcchmax = $orcchmax ? $orcchmax : 'null';
	$orcpercpremim = $orcpercpremim ? $orcpercpremim : 'null';
	$orcpercpremax = $orcpercpremax ? $orcpercpremax : 'null';
	$orcementa = $orcementa ? "'".$orcementa."'" : 'null';
	$modid = $modid ? $modid : 'null';
	
	if( $orcid == '' ){
		
		$sql = "INSERT INTO catalogocurso.organizacaocurso(
			            tioid, orcdesc, orcchmim, orcchmax, orcpercpremim, 
			            orcpercpremax, orcementa, modid, curid)
			    VALUES ($tioid, $orcdesc, $orcchmim, $orcchmax, $orcpercpremim, $orcpercpremax, 
			            $orcementa, $modid, $curid);";
	}else{
		
		$sql = "UPDATE catalogocurso.organizacaocurso SET 
					tioid = $tioid, 
					orcdesc = $orcdesc, 
					orcchmim = $orcchmim, 
					orcchmax = $orcchmax, 
			      	orcpercpremim = $orcpercpremim, 
			      	orcpercpremax = $orcpercpremax, 
			      	orcementa = $orcementa, 
			       	modid = $modid
			 	WHERE 
			 		orcid = ".$orcid;
		
	}
		
	if($db->executar($sql)){
		$db->commit();
		if($request['link'] == 'proximo'){
			$link = '\'catalogocurso.php?modulo=principal/cadEquipe&acao=A\'';
		}else{
			$link = 'window.location';
		}
		echo "<script>
				alert('Dados salvos.');
				window.location = $link;
			  </script>";
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao gravar.');
				window.location = 'catalogocurso.php?modulo=principal/cadOrganizacaoCurso&acao=A';
			  </script>";
	}

}

function excluirOrganizacaoCurso($request){
	
	global $db;
	
	$sql = "UPDATE catalogocurso.organizacaocurso SET 
				orcstatus = 'I'
			WHERE
				orcid = ".$request['orcid'];
	
	if($db->executar($sql)){
		$db->commit();
		echo "<script>
				alert('Organização excuída.');
				window.location = window.location;
			  </script>";
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao excluir.');
				window.location = window.location;
			  </script>";
	}
}

//Contato

function recuperaContato($request){
	
	global $db;
	
	$sql = "SELECT
				curconttel,
				curconttel2,
				curcontdesc,
				curcontemail,
				curcontsite,
				curcontinfo
			FROM
				catalogocurso.curso
			WHERE
				curid = ".$_SESSION['catalogo']['curid'];
	return $db->pegaLinha($sql);
}

function salvarContato($request){

	global $db;
	
	extract($request);
	
	$curid = $_SESSION['catalogo']['curid'];

	$curconttel   = $curconttel   ? "'".$curconttel."'"   : 'null';
	$curconttel2  = $curconttel2  ? "'".$curconttel2."'"  : 'null';
	$curcontdesc  = $curcontdesc  ? "'".$curcontdesc."'"  : 'null';
	$curcontemail = $curcontemail ? "'".$curcontemail."'" : 'null';
	$curcontsite  = $curcontsite  ? "'".$curcontsite."'"  : 'null';
	$curcontinfo  = $curcontinfo  ? "'".$curcontinfo."'"  : 'null';
		
	$sql = "UPDATE catalogocurso.curso SET 
				curconttel = $curconttel,
				curconttel2 = $curconttel2,
				curcontdesc = $curcontdesc,
				curcontemail = $curcontemail,
				curcontsite = $curcontsite,
				curcontinfo = $curcontinfo
			WHERE 
				curid = $curid;";
	
	if($db->executar($sql)){
		$db->commit();
		echo "<script>
				alert('Dados salvos.');
				window.location = window.location;
			  </script>";
	}else{
		$db->rollback();
		echo "<script>
				alert('Erro ao gravar.');
				window.location = window.location;
			  </script>";
	}

}

// Permissões

function testaPermissao( ){
	
	global $db;
	
	$pflcod = pegaPerfil($_SESSION['usucpf']);
	
	if( $db->testa_superuser() || $pflcod == PERFIL_ADMINISTRADOR ){
		$permissoes['gravar'] = true;
	}elseif( $pflcod == PERFIL_GESTOR ){
		$sql = "SELECT 
					'true'
				FROM 
					catalogocurso.curso c 
				INNER JOIN workflow.documento d ON d.docid = c.docid
				INNER JOIN workflow.acaoestadodoc a ON a.esdidorigem = d.esdid
				INNER JOIN workflow.estadodocumentoperfil e ON e.aedid = a.aedid
				INNER JOIN seguranca.perfilusuario p ON p.pflcod = e.pflcod
				INNER JOIN catalogocurso.usuarioresponsabilidade ur ON ur.coordid = c.coordid
				WHERE
					ur.usucpf = '".$_SESSION['usucpf']."'
					AND c.curid = ".$_SESSION['catalogo']['curid']."
					AND d.esdid = ".WF_EM_ANALISE_GESTOR_CURSO;
		$permissoes['gravar'] = $db->pegaUm($sql);
		$permissoes['gravar'] = $permissoes['gravar'] == 'true' ? true : false;
	}elseif( $pflcod == PERFIL_COORDENADOR ){
		$sql = "SELECT 
					'true'
				FROM 
					catalogocurso.curso c 
				INNER JOIN workflow.documento d ON d.docid = c.docid
				INNER JOIN workflow.acaoestadodoc a ON a.esdidorigem = d.esdid
				INNER JOIN workflow.estadodocumentoperfil e ON e.aedid = a.aedid
				INNER JOIN seguranca.perfilusuario p ON p.pflcod = e.pflcod
				INNER JOIN catalogocurso.usuarioresponsabilidade ur ON ur.curid = c.curid
				WHERE
					ur.usucpf = '".$_SESSION['usucpf']."'
					AND c.curid = ".$_SESSION['catalogo']['curid']."
					AND d.esdid = ".WF_EM_ELABORACAO;
		$permissoes['gravar'] = $db->pegaUm($sql);
		$permissoes['gravar'] = $permissoes['gravar'] == 'true' ? true : false;
	}elseif( $pflcod == PERFIL_CONSULTA ){
		$permissoes['gravar'] = false;
	}
	
	return $permissoes;
}

//
//Workflow
//

function verificaPreenchimento( $curid ){
	
	global $db;
	
	$sql = "SELECT
				ateid, 
				curdesc, 
				ncuid, 
				curobjetivo,
				curementa, 
				curcertificado,
				curofertanacional,
				curchmim as mim, 
				curchmax as max,
				curnumestudanteidealpre,
				curnumestudanteidealdist, 
				curnumestudanteminpre,
				curnumestudantemindist, 
				curnumestudantemaxpre,
				curnumestudantemaxdist,
				curqtdmonitora,
				uteid,
				cursalamulti,
				lesid,
				ldeid,
				curcustoaluno,
				to_char(curinicio,'DD/MM/YYYY') as curinicio,
				coordid,
				curpademsocial,
				curconttel,
				curcontdesc,
				curcontemail
			FROM
				catalogocurso.curso c 
			WHERE
				c.curid = ".$curid;
	$dadosAba1 = $db->pegaLinha($sql);
	foreach($dadosAba1 as $dado){
		if( !($dado!='') ){
			return false;
		}
	}
	
	$sql = "SELECT 
				true
			FROM
				catalogocurso.funcaoexercida_curso_publicoalvo
			WHERE
				curid = $curid";
	
	if( !$db->pegaUm($sql) ){
		return false;
	}
	
	$sql = "SELECT 
				fexid
			FROM
				catalogocurso.funcaoexercida_curso_publicoalvo
			WHERE
				curid = $curid";
	
	$fexids = $db->carregarColuna($sql);
	
	if(in_array(FE_DOCENTE,$fexids)){

		if( in_array($dadosAba1['pacod_escolaridade'],Array(6,7)) ){
			$sql = "SELECT 
						true
					FROM
						catalogocurso.areaformacaocurso
					WHERE
						curid = $curid";
			
			if( !$db->pegaUm($sql) ){
				return false;
			}
		}	
		
		$sql = "SELECT 
					true
				FROM
					catalogocurso.etapaensino_curso_publicoalvo
				WHERE
					curid = $curid";
		
		if( !$db->pegaUm($sql) ){
			return false;
		}
	
		$sql = "SELECT 
					true
				FROM
					catalogocurso.diciplinacurso
				WHERE
					curid = $curid";
		
		if( !$db->pegaUm($sql) ){
			return false;
		}
	
		$sql = "SELECT
					cod_mod_ensino
				FROM
					catalogocurso.tab_mod_ensino_curso
				WHERE
					curid = $curid";
		
		if( !$db->pegaUm($sql) ){
			return false;
		}
	}
	
	$sql = "SELECT 
				true
			FROM
				catalogocurso.equipecurso
			WHERE
				eqcstatus = 'A'
				AND curid = $curid";
	
	if( !$db->pegaUm($sql) ){
		return false;
	}
	
	
	$sql = "SELECT 
				true
			FROM
				catalogocurso.etapaensino_curso
			WHERE
				curid = $curid";
	
	if( !$db->pegaUm($sql) ){
		return false;
	}
	
	$sql = "SELECT 
				true
			FROM
				catalogocurso.organizacaocurso
			WHERE
				orcstatus = 'A'
				AND curid = $curid";
	
	if( !$db->pegaUm($sql) ){
		return false;
	}
	
	$sql = "SELECT 
				true
			FROM
				catalogocurso.publicoalvo_curso
			WHERE
				curid = $curid";
	
	if( !$db->pegaUm($sql) ){
		return false;
	}
	
	return true;
}

function recuperaCursoResponssavel(){
	global $db;
	
	$pflcod = pegaPerfil($_SESSION['usucpf']);
	if( !$db->testa_superuser() || $pflcod != PERFIL_ADMINISTRADOR || $pflcod != PERFIL_CONSULTA ){
		$sql = "SELECT DISTINCT
					curid 
				FROM 
					catalogocurso.usuarioresponsabilidade
				WHERE 
					usucpf = '".$_SESSION['usucpf']."' AND pflcod = $pflcod AND curid is not null AND rpustatus = 'A'";
		return $db->carregarColuna($sql);
	}
	
	return true;
	
}

function recuperaCoordenacaoResponssavel(){
	global $db;
	
	if( !$db->testa_superuser() && $pflcod != PERFIL_ADMINISTRADOR && $pflcod != PERFIL_CONSULTA ){
		$pflcod = pegaPerfil($_SESSION['usucpf']);
		$sql = "SELECT DISTINCT
					coordid 
				FROM 
					catalogocurso.usuarioresponsabilidade
				WHERE 
					usucpf = '".$_SESSION['usucpf']."' AND pflcod = $pflcod AND coordid is not null AND rpustatus = 'A'";
		
		return $db->carregarColuna($sql);
	}
	
	return $db->testa_superuser();
	
}

function pegaPerfis($usucpf){
	
	global $db;
	
	$sql = "SELECT 
				pu.pflcod
			FROM seguranca.perfil AS p 
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE 
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";


	$pflcods = $db->carregarColuna( $sql );
	return $pflcods;
}

function pegaPerfil($usucpf){
	
	global $db;
	
	$sql = "SELECT 
				pu.pflcod
			FROM seguranca.perfil AS p 
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE 
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";


	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}

function pegaDocidCurso($curid){
	
	global $db;
	
	$sql = "SELECT 
				docid
			FROM catalogocurso.curso
			WHERE 
				curid = $curid";

	$docid = $db->pegaUm( $sql );
	return $docid;
}

function criarversao(){
	
	global $db;
	
	$curidpai = $_SESSION['catalogo']['curid'];
	
	if( $_SESSION['catalogo']['curid'] ){
		
		$sql = "INSERT INTO catalogocurso.curso
				(	
					ncuid, modid, ateid, curdesc, curementa, 
					curfunteome, curobjetivo, curmetodologia, curcertificado, curcoordenacao, 
					curstatus, curchpremim, curchdistmim, curnumestudanteidealpre, 
					curnumestudanteminpre, curnumestudantemaxpre, curqtdmonitora, 
					uteid, curdtinclusao, eteid, curchpremax, curchdistmax,  
					curinfra, curcustoaluno, curofertanacional, panesid, 
					paeteid, pamodid, curpaoutrasexig, curpatutor, curpatutortxt, 
					curpademsocial, curconttel, curconttel2, curcontdesc, curcontemail, 
					curcontsite, curcontinfo, co_interno_uorg, curnumestudanteidealdist, 
					curnumestudantemindist, curnumestudantemaxdist, lesid, ldeid, cursalamulti, coordid, 
					curchmim, curchmax, curpercpremim, curpercpremax, curpademsocialtxt, curpademsocialpercmax, curversao
				)
				SELECT
					ncuid, modid, ateid, curdesc, curementa, 
					curfunteome, curobjetivo, curmetodologia, curcertificado, curcoordenacao, 
					curstatus, curchpremim, curchdistmim, curnumestudanteidealpre, 
					curnumestudanteminpre, curnumestudantemaxpre, curqtdmonitora, 
					uteid, curdtinclusao, eteid, curchpremax, curchdistmax,  
					curinfra, curcustoaluno, curofertanacional, panesid, 
					paeteid, pamodid, curpaoutrasexig, curpatutor, curpatutortxt, 
					curpademsocial, curconttel, curconttel2, curcontdesc, curcontemail, 
					curcontsite, curcontinfo, co_interno_uorg, curnumestudanteidealdist, 
					curnumestudantemindist, curnumestudantemaxdist, lesid, ldeid, cursalamulti, coordid, 
					curchmim, curchmax, curpercpremim, curpercpremax, curpademsocialtxt, curpademsocialpercmax, curversao+1
				FROM
					catalogocurso.curso
				WHERE
					curid = $curidpai
				RETURNING 
					curid";
		$curidnovo = $db->pegaUm($sql);
		
		preCriarDocumento( $curidnovo );
		
		// Atualiza pai com id do filho
		$sql = "UPDATE catalogocurso.curso SET
					curidfilho = $curidnovo
				WHERE
					curid = $curidpai;";
		
		
		$sql .= "INSERT INTO catalogocurso.funcaoexercida_curso_publicoalvo(fexid,curid)
				SELECT 
					fexid,
					$curidnovo
				FROM
					catalogocurso.funcaoexercida_curso_publicoalvo
				WHERE
					curid = $curidpai ;";
	
		$sql .= "INSERT INTO catalogocurso.areaformacaocurso(cod_area_ocde,curid)
				SELECT 
					cod_area_ocde,
					$curidnovo
				FROM
					catalogocurso.areaformacaocurso
				WHERE
					curid = $curidpai ;";
		
		$sql .= "INSERT INTO catalogocurso.etapaensino_curso_publicoalvo(cod_etapa_ensino, curid)
				SELECT 
					cod_etapa_ensino,
					$curidnovo
				FROM
					catalogocurso.etapaensino_curso_publicoalvo
				WHERE
					curid = $curidpai ;";
		
		$sql .= "INSERT INTO catalogocurso.diciplinacurso(cod_disciplina,curid)
				SELECT 
					cod_disciplina,
					$curidnovo
				FROM
					catalogocurso.diciplinacurso
				WHERE
					curid = $curidpai ;";
		
		$sql .= "INSERT INTO catalogocurso.tab_mod_ensino_curso(cod_mod_ensino, curid)
				SELECT
					cod_mod_ensino,
					$curidnovo
				FROM
					catalogocurso.tab_mod_ensino_curso
				WHERE
					curid = $curidpai ;";
		
		$sql .= "INSERT INTO catalogocurso.equipecurso
				(
					camid, unrid, nesid, eqcfuncao, eqcminimo, eqcmaximo, 
			        eqcatribuicao, eqcoutrosreq, eqcbolsista, cod_escolaridade, qtdfuncao, curid
			    )
				SELECT 
					camid, unrid, nesid, eqcfuncao, eqcminimo, eqcmaximo, 
		        	eqcatribuicao, eqcoutrosreq, eqcbolsista, cod_escolaridade, qtdfuncao,
					$curidnovo
				FROM 
					catalogocurso.equipecurso
				WHERE
					eqcstatus = 'A'
					AND curid = $curidpai ;";
	
		$sql .= "INSERT INTO catalogocurso.etapaensino_curso(cod_etapa_ensino, curid)
				SELECT 
					cod_etapa_ensino,
					$curidnovo
				FROM
					catalogocurso.etapaensino_curso
				WHERE
					curid = $curidpai ;";
	
		$sql .= "INSERT INTO catalogocurso.organizacaocurso
				(
					tioid, orcdesc, orchadisminimo, orchadismaximo, orchapreminimo, 
			        orchapremaximo, orcementa, orcstatus, modid, orcchmim, 
			        orcchmax, orcpercpremim, orcpercpremax, curid
				)
				SELECT 
					tioid, orcdesc, orchadisminimo, orchadismaximo, orchapreminimo, 
			        orchapremaximo, orcementa, orcstatus, modid, orcchmim, 
			        orcchmax, orcpercpremim, orcpercpremax, $curidnovo
				FROM 
					catalogocurso.organizacaocurso
				WHERE
					orcstatus = 'A'
					AND curid = $curidpai ;";
	
		$sql .= "INSERT INTO catalogocurso.publicoalvo_curso(cod_escolaridade, curid)
				SELECT 
					cod_escolaridade,
					$curidnovo
				FROM
					catalogocurso.publicoalvo_curso
				WHERE
					curid = $curidpai ;";

		$sql .= "INSERT INTO catalogocurso.cursorede(redid, curid)
				SELECT
					redid,
					$curidnovo
				FROM
					catalogocurso.cursorede
				WHERE
					curid = $curidpai;";

		$sql .= "INSERT INTO catalogocurso.nivelcurso_curso(ncuid, curid)
				SELECT
					ncuid,
					$curidnovo
				FROM
					catalogocurso.nivelcurso_curso
				WHERE
					curid = $curidpai;"; 
		
		if($db->executar($sql)){
			$db->commit();
		
			echo "<script>alert('Nova versão criada.');window.location = 'catalogocurso.php?modulo=principal/cadCatalogo&acao=A&curid=$curidnovo';</script>";
		}else{
			echo "<script>alert('Escolha um curso.');window.location = 'catalogocurso.php?modulo=principal/listaCursos&acao=A';</script>";
		}
		
	}else{
		echo "<script>alert('Escolha um curso.');window.location = 'catalogocurso.php?modulo=principal/listaCursos&acao=A';</script>";
	}
}


/** Formata o valor numeric para ser inserido no banco
 * @name formata_valor_sql
 * @author Luciano F. Ribeiro
 * @access public
 * @return float
 */
function formata_valor_sql($valor){
	$valor = str_replace('.', '', $valor);
	$valor = str_replace(',', '.', $valor);
	return $valor;
}

?>