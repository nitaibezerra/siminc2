<?php
function pegaEstadoAtual( $docid ) {
    global $db;

    if($docid) {
        $docid = (integer) $docid;

        $sql = "SELECT		ed.esdid
				FROM		workflow.documento d
				INNER JOIN	workflow.estadodocumento ed on ed.esdid = d.esdid
				WHERE		d.docid = {$docid}";
        
        $estado = $db->pegaUm($sql);
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
    
    if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){
    	$arMnuid = array(ABA_ABRAGENCIA);
    } else {
    	$arMnuid = array();
    }
    echo $db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);
}

function preCriarDocumento( $curid, $tpdid = 167 ) {
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    $docid = prePegarDocid( $curid );

    if( !$docid ) {

        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento( $tpdid, $docdsc );

        // atualiza pap do EMI
        $sql = "UPDATE		catalogocurso2014.curso
				SET			docid = {$docid}
				WHERE		curid = {$curid}";

        $db->executar($sql);
        $db->commit();
    }
    return $docid;
}

function prePegarDocid( $curid ) {
    global $db;

    $curid = (integer) $curid;
    
    $sql = "SELECT		docid
			FROM		catalogocurso2014.curso
			WHERE	 	curid = {$curid} AND curano = {$_SESSION['exercicio']}";

    return (integer) $db->pegaUm($sql);
}

function salvarCatalogo($post){
    global $db,$_FILES;

    extract($post);

    $cursalamulti = $cursalamulti == 't' ? 'TRUE' : 'FALSE';
    $curid = $curid ? $curid : $_SESSION['catalogo']['curid'];
    $ateid = $ateid ? $ateid : 'NULL';
    $suaid = $suaid ? $suaid : 'NULL';
    $espid = $espid ? $espid : 'NULL';
    $curdesc = $curdesc ? "'".$curdesc."'" : 'NULL';
    $curementa = $curementa ? "'".$curementa."'" : 'NULL';
    $curfunteome = $curfunteome ? "'".$curfunteome."'" : 'NULL';
    $curobjetivo = $curobjetivo ? "'".$curobjetivo."'" : 'NULL';
    $curmetodologia = $curmetodologia ? "'".$curmetodologia."'" : 'NULL';
    $curchmim = $curchmim ? $curchmim : 'NULL';
    $curchmax = $curchmax ? $curchmax : 'NULL';
    $curpercpremim = $curpercpremim ? $curpercpremim : 'NULL';
    $curpercpremax = $curpercpremax ? $curpercpremax : 'NULL';
    $curcertificado = $curcertificado ? "'".$curcertificado."'" : 'NULL';
    $curnumestudanteidealpre = $curnumestudanteidealpre != '' ? $curnumestudanteidealpre : 'NULL';
    $curnumestudanteminpre = $curnumestudanteminpre != '' ? $curnumestudanteminpre : 'NULL';
    $curnumestudantemaxpre = $curnumestudantemaxpre != '' ? $curnumestudantemaxpre : 'NULL';
    $curnumestudanteidealdist = $curnumestudanteidealdist != '' ? $curnumestudanteidealdist : 'NULL';
    $curnumestudantemindist = $curnumestudantemindist != '' ? $curnumestudantemindist : 'NULL';
    $curnumestudantemaxdist = $curnumestudantemaxdist != '' ? $curnumestudantemaxdist : 'NULL';
    $curinicio = $curinicio ? "'".preparaData($curinicio, '/')."'" : 'NULL';
    $curfim = $curfim ? "'".preparaData($curfim, '/')."'" : 'NULL';
    $curqtdmonitora = $curqtdmonitora ? $curqtdmonitora : 'NULL';
    $uteid = $uteid ? $uteid : 'NULL';
    $curofertanacional = $curofertanacional ? $curofertanacional : 'NULL';
    $curcustoaluno = $curcustoaluno ? $curcustoaluno : 'NULL';
    $curinfra = $curinfra ? $curinfra : 'NULL';
    $lesid = $lesid ? $lesid : 'NULL';
    $ldeid = $ldeid ? $ldeid : 'NULL';
    $coordid = $coordid ? $coordid : 'NULL';
    $curduracao = $curduracao ? $curduracao : 'NULL';
    $curpademsocial = $curpademsocial == 't' ? 'TRUE' : 'FALSE';
    $curpademsocialpercmax = $curpademsocialpercmax && ($curpademsocial == 'TRUE') ? $curpademsocialpercmax : 'NULL';
    
    if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){
    	$nivelcurso = $ncuid ? $ncuid : 'NULL';
    } else {
    	$nivelcurso = 'NULL';
    } 

    if($curid!=''){
        $antcurchmim = $db->pegaUm("SELECT curchmim  FROM catalogocurso2014.curso WHERE curid = {$curid}");
        $antcurchmax = $db->pegaUm("SELECT curchmax  FROM catalogocurso2014.curso WHERE curid = {$curid}");

        if(($antcurchmim != ($curchmim) || $antcurchmax != ($curchmax))){
            $sql = "UPDATE 		catalogocurso2014.organizacaocurso 
            		SET			orcstatus = 'I'
					WHERE		curid = {$curid}";
            
            $db->executar($sql);
            $db->commit();
        }

        $sql = "UPDATE 		catalogocurso2014.curso 
        		SET			ateid = {$ateid},
							suaid = {$suaid},
							espid = {$espid},
							curdesc = {$curdesc},
							ncuid = {$nivelcurso},
							curementa = {$curementa},
							curfunteome = {$curfunteome},
							curobjetivo = {$curobjetivo},
							curmetodologia = {$curmetodologia},
							curchmim = {$curchmim},
							curchmax = {$curchmax},
							curpercpremim = {$curpercpremim},
							curpercpremax = {$curpercpremax},
							curcertificado = {$curcertificado},
							curnumestudanteidealpre = {$curnumestudanteidealpre},
							curnumestudanteminpre = {$curnumestudanteminpre},
							curnumestudantemaxpre = {$curnumestudantemaxpre},
							curnumestudanteidealdist = {$curnumestudanteidealdist},
							curnumestudantemindist = {$curnumestudantemindist},
							curnumestudantemaxdist = {$curnumestudantemaxdist},
							curinicio = {$curinicio},
							curfim = {$curfim},
							curqtdmonitora = {$curqtdmonitora},
							uteid = {$uteid},
							curofertanacional = {$curofertanacional},
							curcustoaluno = ".str_replace(Array('.',','),Array('','.'),$curcustoaluno).",
							curinfra = '{$curinfra}',
							lesid = {$lesid},
							ldeid = {$ldeid},
							cursalamulti = {$cursalamulti},
							coordid = {$coordid}, 
							curduracao = {$curduracao},
        					curpademsocial = {$curpademsocial},
        					curpademsocialpercmax = {$curpademsocialpercmax}
				WHERE		curid = {$curid}
				RETURNING	curid";
        
        $curid = $db->pegaUm($sql);
    } else {
        $sql = "INSERT INTO catalogocurso2014.curso (
					ateid, suaid, espid, curdesc, ncuid, curementa, curfunteome, curobjetivo,
					curmetodologia, curchmim, curchmax, curpercpremim, curpercpremax, curcertificado,
					curnumestudanteidealpre, curnumestudanteminpre, curnumestudantemaxpre, curnumestudanteidealdist, curnumestudantemindist,
					curnumestudantemaxdist, curinicio, curfim, curqtdmonitora, uteid, curofertanacional, curcustoaluno,
					curinfra, lesid, ldeid, cursalamulti, coordid, curano, curduracao, curpademsocial, curpademsocialpercmax)
				VALUES(
					$ateid, $suaid, $espid, $curdesc, $nivelcurso, $curementa, $curfunteome, $curobjetivo,
					$curmetodologia, $curchmim, $curchmax, $curpercpremim, $curpercpremax, $curcertificado,
					$curnumestudanteidealpre, $curnumestudanteminpre, $curnumestudantemaxpre, $curnumestudanteidealdist,
					$curnumestudantemindist, $curnumestudantemaxdist, $curinicio, $curfim, $curqtdmonitora, $uteid, $curofertanacional,
					".str_replace(Array('.',','),Array('','.'),$curcustoaluno).",
					'$curinfra', $lesid, $ldeid, $cursalamulti, $coordid, {$_SESSION['exercicio']}, {$curduracao}, {$curpademsocial},{$curpademsocialpercmax})
				RETURNING	curid";
					
        $curid = $db->pegaUm($sql);
        $pflcod = pegaPerfil($_SESSION['usucpf']);
        $usucpf = str_pad($_SESSION['usucpf'],11,'0');
        $sql = "INSERT INTO catalogocurso2014.usuarioresponsabilidade(pflcod,usucpf,curid) VALUES({$pflcod},'{$usucpf}',{$curid})";
        $db->executar($sql);
    }

    if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){
        $sql = "SELECT 	abcid
                FROM 	catalogocurso2014.abrangenciacurso
                WHERE 	curid = {$curid}";
       
        $idDb = $db->pegaUm($sql);
       
        if($idDb){
            $sql = "UPDATE 		catalogocurso2014.abrangenciacurso
                    SET 		abcdemanda = {$abcdemanda}, 
                    			abcstatus='A'
                    WHERE 		abcid = {$idDb}
                    RETURNING 	abcid";
        } else {
            $sql = "INSERT INTO catalogocurso2014.abrangenciacurso(curid, abcdemanda, abcstatus)
                    VALUES ({$curid}, {$abcdemanda}, 'A')
                    RETURNING abcid";
        }   	
        $db->executar($sql);
    }
    
    preCriarDocumento($curid);

    $sql = "INSERT INTO catalogocurso2014.historicocurso(usucpf,curid) VALUES ('{$_SESSION['usucpf']}',{$curid});";
    $db->executar($sql);

    $sql .= "DELETE FROM catalogocurso2014.nivelcurso_curso WHERE curid = {$curid};";
    if(is_array($ncuid) && count($ncuid) > 0 ){
        foreach($ncuid as $id){
            $sql .= "INSERT INTO catalogocurso2014.nivelcurso_curso(ncuid,curid) VALUES ({$id}, {$curid}) ;";
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.cursorede WHERE curid = {$curid};";
    if(is_array($redid) && count($redid) > 0 ){
        foreach($redid as $id){
            $sql .= "INSERT INTO catalogocurso2014.cursorede(redid,curid) VALUES ({$id}, {$curid}) ;";
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.modalidadecurso_curso WHERE curid = {$curid};";
    if(is_array($modid) && count($modid) > 0 ){
        foreach($modid as $id){
            $sql .= "INSERT INTO catalogocurso2014.modalidadecurso_curso(modid,curid) VALUES ({$id}, {$curid}) ;";
        }
    }
    
    if($_SESSION['exercicio']==ANO_EXERCICIO_2015){
        $sql .= "DELETE FROM catalogocurso2014.etapaensino_curso_dadosgerais WHERE curid = {$curid};";
        if($cod_etapa_ensino[0] != ''){
	        foreach($cod_etapa_ensino as $id){
	            $sql .= "INSERT INTO catalogocurso2014.etapaensino_curso_dadosgerais(cod_etapa_ensino,curid) VALUES ({$id}, {$curid});";
	        }
    	} 	
    	
        $sql .= "DELETE FROM catalogocurso2014.cursodemandasocial WHERE curid = {$curid};";
    
	    if($padid[0] != '' && $curpademsocial == 'TRUE'){
	        if(in_array('999',$padid) ){
	            $sql .= "INSERT INTO catalogocurso2014.cursodemandasocial (padid,curid) VALUES (999, {$curid});";
	        } else {
	            foreach($padid as $id){
	                $sql .= "INSERT INTO catalogocurso2014.cursodemandasocial (padid,curid) VALUES ({$id}, {$curid});";
	            }
	        }
	    }    	
    }

    if(is_array($arqdsc_old)){
        foreach($arqdsc_old as $k => $old){
            if( $arqdsc[$k] != $old ){
                $sql .= "UPDATE catalogocurso2014.arquivocurso SET arcdesc = '$arqdsc[$k]' WHERE arcid = $k;";
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

                $file = new FilesSimec("arquivocurso", $campos,"catalogocurso2014");

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
        } else {
            $_SESSION['catalogo']['curid'] = $curid;
            $db->commit();
            if($post['link'] == 'proximo'){
                $link = '\'catalogocurso2014.php?modulo=principal/cadOrganizacaoCurso&acao=A\'';
            }else{
                if($_SESSION['catalogo']['curid']){
                    $link = 'window.location';
                }else{
                    $link = '\'catalogocurso2014.php?modulo=inicio&acao=C\'';
                }
            }
            echo "<script>
					alert('Dados salvos.');
					window.location = $link;
				  </script>";
        }
    } else {
        $db->rollback();
        echo "<script>
				alert('Erro ao gravar');
			  </script>";
    }
}

function recuperaDadosCurso($request){
    global $db;
    
    extract($request);

    if($curid){
        $sql = "SELECT			c.curid,
								curstatus,
								ateid,
								suaid,
								espid,
								curdesc,
								ncuid,
								curementa,
								curfunteome,
								curobjetivo,
								curmetodologia,
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
								to_char(curinicio,'DD/MM/YYYY') AS curinicio,
								to_char(curfim,'DD/MM/YYYY') AS curfim,
								curofertanacional,
								curcustoaluno,
								curqtdmonitora,
								uteid,
								curinfra,
								usunome,
								to_char(hicdata,'DD/MM/YYY - HH24:MI:SS') AS hicdata,
								cursalamulti,
								lesid,
								ldeid,
								coordid,
								curduracao,
								corid,
								curpademsocial,
								curpademsocialpercmax
				FROM			catalogocurso2014.curso c
				LEFT JOIN 		catalogocurso2014.etapaensino e ON e.eteid = c.eteid AND e.eteano = {$_SESSION['exercicio']}
				LEFT JOIN 		catalogocurso2014.historicocurso h ON h.curid = c.curid
				LEFT JOIN 		seguranca.usuario u ON u.usucpf = h.usucpf
				WHERE			c.curid = {$curid} AND c.curano = {$_SESSION['exercicio']}";
        
        $curso = $db->pegaLinha($sql);

        $sql = "SELECT		r.redid AS codigo
				FROM		catalogocurso2014.cursorede cr
				INNER JOIN 	catalogocurso2014.rede r ON r.redid = cr.redid
				WHERE		curid = {$curid} AND r.redano = {$_SESSION['exercicio']}";
        
        $curso['redid'] = $db->carregarColuna($sql);

        $sql = "SELECT		mod.modid AS codigo
				FROM		catalogocurso2014.modalidadecurso_curso mcu
				INNER JOIN 	catalogocurso2014.modalidadecurso mod ON mod.modid = mcu.modid
				WHERE		mcu.curid = {$curid} AND mod.modano = {$_SESSION['exercicio']}";
        
        $curso['modid'] = $db->carregarColuna($sql);
        
        if($_SESSION['exercicio']==ANO_EXERCICIO_2015){
	        $sql = "SELECT		ncc.ncuid AS codigo
					FROM		catalogocurso2014.nivelcurso_curso ncc
					INNER JOIN 	catalogocurso2014.nivelcurso ncu ON ncc.ncuid = ncu.ncuid
					WHERE		ncc.curid = {$curid} AND ncu.ncuano = {$_SESSION['exercicio']}";
	        
	        $curso['ncuid'] = $db->carregarColuna($sql);    
	        
	        $sql = "SELECT		e.pk_cod_etapa_ensino::integer AS codigo,
								replace(e.no_etapa_ensino,'-','/') AS descricao
					FROM		catalogocurso2014.etapaensino_curso_dadosgerais ec
					INNER JOIN 	educacenso_".(ANO_CENSO).".tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino 
					WHERE		ec.curid = {$curid}";
	        
	        $curso['cod_etapa_ensino'] = $db->carregar($sql);	      

	        $sql = "SELECT		pad.padid AS codigo,
								pad.paddesc AS descricao
					FROM		catalogocurso2014.cursodemandasocial cms
					INNER JOIN 	catalogocurso2014.publicoalvodemandasocial pad ON pad.padid = cms.padid
					WHERE		curid = {$curid}";
	        $curso['padid'] = $db->carregar($sql);	        
        }
        return $curso;
    }
}

function recuperaModalidadePublicoAlvo($request){
    global $db;

    if($request['curid']){
        $sql = "SELECT		moc.cod_mod_ensino AS codigo
				FROM		catalogocurso2014.tab_mod_ensino_curso moc
				INNER JOIN 	educacenso_".(ANO_CENSO).".tab_mod_ensino mod ON mod.pk_cod_mod_ensino = moc.cod_mod_ensino
				WHERE		moc.curid = {$request['curid']}";
        
        return $db->carregarColuna($sql);
    }
}

function recuperaRedesCurso($request){
    global $db;

    if($request['curid']){
        $sql = "SELECT		redid
				FROM		catalogocurso2014.cursorede
				WHERE		crestatus = 'A' AND	curid = {$request['curid']}";
        
        return $db->carregarColuna($sql);
    }
}

function recuperaArquivos($request){
    global $db;

    if($request['curid']){
        $arrPflcods = Array();
        $arrCurids = Array();
        $arrCoords = Array();
        $pflcods = pegaPerfis($_SESSION['usucpf']);
        if( !$db->testa_superuser() && !in_array(PERFIL_CONSULTA,$pflcods) && !in_array(PERFIL_ADMINISTRADOR,$pflcods) ){
            $arrCoords = recuperaCoordenacaoResponssavel();
            $arrCurids = recuperaCursoResponssavel();
            array_push($arrCoords,'0');
            array_push($arrCurids,'0');
            $pflcod     = pegaPerfil($_SESSION['usucpf']);
            $arrPflcods = Array(PERFIL_COORDENADOR,
                PERFIL_GESTOR);
        }
        $coordid = $db->pegaUm('SELECT coordid FROM catalogocurso2014.curso WHERE curid = '.$request['curid']);
        $excluir = false;
        if($db->testa_superuser() || in_array(PERFIL_ADMINISTRADOR,$pflcods)){
            $excluir = true;
        } elseif(in_array($coordid,$arrCoords) || in_array($request['curid'],$arrCurids)){
            $excluir = true;
        }

        $sql = "SELECT		arcid,
							arcdesc,
							a.arqid,
							a.arqnome,
							curid
				FROM		catalogocurso2014.arquivocurso c
				INNER JOIN 	public.arquivo a ON a.arqid = c.arqid
				WHERE		arcstatus = 'A' AND curid = {$request['curid']}";
        
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
    $file = new FilesSimec(NULL, NULL, 'catalogocurso2014');
    $file->getDownloadArquivo($request['arqid']);
    echo "<script>
			window.close();
		  </script>";
    exit();
}

function excluirArquivo($request){
    global $db;

    $sql = "UPDATE 		catalogocurso2014.arquivocurso 
    		SET			arcstatus = 'I'
			WHERE		arcid = ".$request['arcid'];
    
    if($db->executar($sql)){
        $db->commit();
        return true;
    } else {
        $db->rollback();
        return false;
    }
}

function excluirCurso($request){
    global $db;
    
    extract($request);

    $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocareaformacao WHERE pafid IN (SELECT pafid FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid});
            DELETE FROM catalogocurso2014.publicoalvo_assocdisciplina WHERE pafid IN (SELECT pafid FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid});
            DELETE FROM catalogocurso2014.publicoalvo_assocetapaensino WHERE pafid IN (SELECT pafid FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid});
            DELETE FROM catalogocurso2014.publicoalvo_assocmodensino WHERE pafid IN (SELECT pafid FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid});
            DELETE FROM catalogocurso2014.publicoalvo_assocnivelescolaridade WHERE pafid IN (SELECT pafid FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid});
            DELETE FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.iesofertante WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.areaformacaocurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.cursorede WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.diciplinacurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.escolaridade_equipe WHERE eqcid IN (SELECT eqcid FROM catalogocurso2014.equipecurso WHERE curid = {$curid});
            DELETE FROM catalogocurso2014.equipecurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.etapaensino_curso_publicoalvo WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.etapaensino_curso WHERE curid ={$curid};
            DELETE FROM catalogocurso2014.funcaoexercida_curso_publicoalvo WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.historicocurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.modalidadecurso_curso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.organizacaocurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.publicoalvo_curso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.tab_mod_ensino_curso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.usuarioresponsabilidade WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.arquivocurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.cursodemandasocial WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.membrosdemandasocial WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.abrangenciacurso WHERE curid = {$curid};
            DELETE FROM catalogocurso2014.curso WHERE curid = {$curid};";

    $db->executar($sql);
    $db->commit();
    return true;
}

//// INICIO EQUIPE
function salvarEquipe($request){
    global $db;

    extract($request);

    $curid = $_SESSION['catalogo']['curid'];

    $camid = $camid ? $camid : 'NULL';
    $eqcbolsista = $eqcbolsista ? $eqcbolsista : 'FALSE';
    $qtdfuncao = $qtdfuncao ? $qtdfuncao : 'NULL';
    $eqcfuncao = $eqcfuncao ? "'".$eqcfuncao."'" : 'NULL';
    $eqcminimo = $eqcminimo ? $eqcminimo : 'NULL';
    $eqcmaximo = $eqcmaximo ? $eqcmaximo : 'NULL';
    $unrid = $unrid ? $unrid : 'NULL';
    $qtdfuncao = $qtdfuncao ? $qtdfuncao : 'NULL';
    $eqcatribuicao = $eqcatribuicao ? "'".$eqcatribuicao."'" : 'NULL';
    $eqcoutrosreq = $eqcoutrosreq ? "'".$eqcoutrosreq."'" : 'NULL';
	
    $eqcvalorunitario 	= $eqcvalorunitario ? "$eqcvalorunitario" : 'NULL';
    $eqcvalorunitario = desformata_valor($eqcvalorunitario);

    $eqccargahorariames	= $eqccargahorariames ? "'".$eqccargahorariames."'" : 'NULL';
    
	$fueid = $fueid ? $fueid : 'NULL';
    
    if($eqcid == ''){
        $sql = "INSERT INTO catalogocurso2014.equipecurso(camid, eqcbolsista, eqcfuncao, eqcminimo, eqcmaximo, unrid,eqcatribuicao, eqcoutrosreq, curid, qtdfuncao, eqcvalorunitario, eqccargahorariames, fueid)
			    VALUES ({$camid}, '{$eqcbolsista}', {$eqcfuncao}, {$eqcminimo}, {$eqcmaximo}, {$unrid},{$eqcatribuicao}, {$eqcoutrosreq}, {$_SESSION['catalogo']['curid']}, {$qtdfuncao}, {$eqcvalorunitario}, {$eqccargahorariames}, {$fueid})
			    RETURNING eqcid";
    } else {
        $sql = "UPDATE 		catalogocurso2014.equipecurso 
        		SET	    	camid = {$camid},
        					eqcbolsista = '{$eqcbolsista}', 
        					eqcfuncao = {$eqcfuncao}, 
        					eqcminimo = {$eqcminimo}, 
        					eqcmaximo = {$eqcmaximo}, 
        					unrid = {$unrid},
			    			eqcatribuicao = {$eqcatribuicao}, 
			    			eqcoutrosreq = {$eqcoutrosreq}, 
			    			qtdfuncao = {$qtdfuncao}, 
			    			eqcvalorunitario = {$eqcvalorunitario},
			    			eqccargahorariames = {$eqccargahorariames},
			    			fueid = {$fueid}
			 	WHERE 		eqcid = {$eqcid}
			    RETURNING  	eqcid";

    }
    $eqcid = $db->pegaUm($sql);
    
    if($eqcid){
        $sql = "DELETE FROM catalogocurso2014.escolaridade_equipe WHERE eqcid = {$eqcid};";
	    $db->executar($sql);
	    if(is_array($cod_escolaridade) && count($cod_escolaridade) > 0 ){
	    	$sql = '';
	        foreach($cod_escolaridade as $id){
	            $sql .= "INSERT INTO catalogocurso2014.escolaridade_equipe(cod_escolaridade, eqcid, eeqano) VALUES ({$id}, {$eqcid}, {$_SESSION['exercicio']}) ;";
	        }
	    }   	
    }

    if($db->executar($sql)){
        $db->commit();
        
        if($request['link'] == 'proximo'){
            $link = '\'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A\'';
        } else {
            $link = '\'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A\'';
        }
        
        echo "<script>
				alert('Dados salvos.');
				window.location = $link;
			  </script>";
    } else {
        $db->rollback();
        echo "<script>
				alert('Erro ao gravar.');
				window.location = 'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A';
			  </script>";
    }

}

function excluirEquipe($request){
    global $db;

    $sql = "UPDATE 		catalogocurso2014.equipecurso 
    		SET			eqcstatus = 'I'
			WHERE		eqcid = {$request['eqcid']}";

    if($db->executar($sql)){
        $db->commit();
        echo "<script>
				alert('Equipe excuída.');
				window.location = window.location;
			  </script>";
    } else {
        $db->rollback();
        echo "<script>
				alert('Erro ao excluir.');
				window.location = window.location;
			  </script>";
    }
}

function recuperaDadosEquipe($request){
    global $db;
    
    extract($request);

    if($request['eqcid']){
        $sql = "SELECT 		camid,
							unrid,
							cod_escolaridade,
							eqcfuncao,
							eqcminimo,
							eqcmaximo,
					        eqcatribuicao,
					        eqcoutrosreq,
					        eqcbolsista,
					        qtdfuncao,
					        TRIM(TO_CHAR(eqcvalorunitario, '999G999G999D99')) AS eqcvalorunitario,
					        eqccargahorariames,
					        fueid
				FROM		catalogocurso2014.equipecurso
				WHERE		eqcid = {$eqcid} AND curid = {$_SESSION['catalogo']['curid']}";

        $equipe = $db->pegaLinha($sql);

        $sql = "SELECT		foo.codigo,
							foo.descricao
				FROM		((SELECT		to_char(pk_cod_escolaridade,'9') AS codigo,
											pk_cod_escolaridade||' - '||no_escolaridade AS descricao,
											h.nivel
		  					  FROM			educacenso_".(ANO_CENSO).".tab_escolaridade e
		  					  INNER JOIN	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
		  					  WHERE			hneano = {$_SESSION['exercicio']})
		  					UNION ALL
		  					(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo,
											pk_pos_graduacao||'0 - '||no_pos_graduacao AS descricao,
											h.nivel
							 FROM			educacenso_".(ANO_CENSO).".tab_pos_graduacao e
		  					 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
		  					 WHERE			hneano = {$_SESSION['exercicio']})) AS foo
				INNER JOIN 	 catalogocurso2014.escolaridade_equipe eeq ON eeq.cod_escolaridade = foo.codigo::integer AND eeq.eqcid = {$eqcid}";
        
        $equipe['cod_escolaridade'] = $db->carregar($sql);

        return $equipe;
    }
}

function recuperaDadosPublicoAlvo($request){
    global $db;

    $curid = $_SESSION['catalogo']['curid'];

    if($curid){
        $sql = "SELECT		panesid AS nesid,
							paeteid AS cod_etapa_ensino,
							curpaoutrasexig,
							curpatutor,
							curpatutortxt,
		       				curpademsocial,
		       				pacod_escolaridade,
		       				curpademsocialpercmax
				FROM		catalogocurso2014.curso
				WHERE		curid = {$curid}";
        
        $retorno = $db->pegaLinha($sql);

        $sql = "SELECT		pk_cod_area_ocde AS codigo,
							no_nome_area_ocde AS descricao
				FROM		educacenso_".(ANO_CENSO).".tab_area_ocde t
				INNER JOIN 	catalogocurso2014.areaformacaocurso a ON a.cod_area_ocde = t.pk_cod_area_ocde
				WHERE		a.curid = {$curid}";
        $area = $db->carregar($sql);

        $sql = "SELECT		pk_cod_disciplina AS codigo,
							no_disciplina AS descricao
				FROM		educacenso_".(ANO_CENSO).".tab_disciplina t
				INNER JOIN 	catalogocurso2014.diciplinacurso a ON a.cod_disciplina = t.pk_cod_disciplina
				WHERE		a.curid = {$curid}";
        $disc = $db->carregar($sql);

        $sql = "SELECT		e.pk_cod_etapa_ensino AS codigo,
							e.no_etapa_ensino AS descricao
				FROM		catalogocurso2014.etapaensino_curso_publicoalvo ec
				INNER JOIN 	educacenso_".(ANO_CENSO).".tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino
				WHERE		ec.curid = {$curid}";
        $retorno['cod_etapa_ensino'] = $db->carregar($sql);

        $sql = "SELECT		e.fexid AS codigo,
							e.fexdesc AS descricao
				FROM		catalogocurso2014.funcaoexercida_curso_publicoalvo ec
				INNER JOIN 	catalogocurso2014.funcaoexercida e ON e.fexid = ec.fexid
				WHERE		ec.curid = {$curid}";
        $retorno['fexid'] = $db->carregar($sql);

        $retorno["cod_area_ocde"] = $area;
        $retorno["cod_disciplina"] = $disc;

        $sql = "SELECT		foo.codigo,
							foo.descricao
				FROM		((SELECT		to_char(pk_cod_escolaridade,'9') AS codigo,
											pk_cod_escolaridade||' - '||no_escolaridade AS descricao,
											h.nivel
		  					  FROM			educacenso_".(ANO_CENSO).".tab_escolaridade e
		  					  INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
		  					  WHERE			h.hneano = {$_SESSION['exercicio']})
		  					 UNION ALL
		  					(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo,
											pk_pos_graduacao||'0 - '||no_pos_graduacao AS descricao,
											h.nivel
							 FROM			educacenso_".(ANO_CENSO).".tab_pos_graduacao e
		  					 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
		  					 WHERE			h.hneano = {$_SESSION['exercicio']})) AS foo
				INNER JOIN 	 catalogocurso2014.publicoalvo_curso eeq ON eeq.cod_escolaridade = foo.codigo::integer AND eeq.curid = {$request['curid']}";
        $retorno['cod_escolaridade'] = $db->carregar($sql);

        $sql = "SELECT		tmod.pk_cod_mod_ensino
				FROM		catalogocurso2014.tab_mod_ensino_curso mod
				INNER JOIN 	educacenso_".(ANO_CENSO).".tab_mod_ensino tmod ON tmod.pk_cod_mod_ensino = mod.cod_mod_ensino
				WHERE		curid = {$curid}";
        $retorno['cod_mod_ensino'] = $db->carregarColuna($sql);

        $sql = "SELECT		pad.padid AS codigo,
							pad.paddesc AS descricao
				FROM		catalogocurso2014.cursodemandasocial cms
				INNER JOIN 	catalogocurso2014.publicoalvodemandasocial pad ON pad.padid = cms.padid
				WHERE		curid = {$curid}";
        $retorno['padid'] = $db->carregar($sql);

        return $retorno;
    }
}

// Organização
function recuperaOrganizacaoCurso($post){
    global $db;

    $sql = "SELECT		orc.orcdesc,
						orc.orcchmim,
						orc.orcchmax,
						orc.orcpercpremim,
						orc.orcpercpremax,
						orc.orcementa,
						orc.modid,
						orc.tioid
			FROM		catalogocurso2014.organizacaocurso orc
			LEFT JOIN 	catalogocurso2014.tipoorganizacao tor ON tor.tioid = orc.tioid
			WHERE		orcid = {$post['orcid']} AND orcstatus = 'A' AND curid = {$_SESSION['catalogo']['curid']}";
    
    return $db->pegaLinha($sql);
}


function salvarOrganizacaoCurso($request){
    global $db;

    extract($request);

    $curid = $_SESSION['catalogo']['curid'];

    $tioid = $tioid ? $tioid : 'NULL';
    $orcdesc = $orcdesc ? "'".$orcdesc."'" : 'NULL';
    $orcchmim = $orcchmim ? $orcchmim : 'NULL';
    $orcchmax = $orcchmax ? $orcchmax : 'NULL';
    $orcpercpremim = $orcpercpremim ? $orcpercpremim : 'NULL';
    $orcpercpremax = $orcpercpremax ? $orcpercpremax : 'NULL';
    $orcementa = $orcementa ? "'".$orcementa."'" : 'NULL';
    $modid = $modid ? $modid : 'NULL';

    if( $orcid == '' ){
        $sql = "INSERT INTO catalogocurso2014.organizacaocurso(tioid, orcdesc, orcchmim, orcchmax, orcpercpremim,orcpercpremax, orcementa, modid, curid)
			    VALUES ($tioid, $orcdesc, $orcchmim, $orcchmax, $orcpercpremim, $orcpercpremax,$orcementa, $modid, $curid);";
    } else {
        $sql = "UPDATE 		catalogocurso2014.organizacaocurso 
        		SET			tioid = {$tioid},
							orcdesc = {$orcdesc},
							orcchmim = {$orcchmim},
							orcchmax = {$orcchmax},
					      	orcpercpremim = {$orcpercpremim},
					      	orcpercpremax = {$orcpercpremax},
					      	orcementa = {$orcementa},
					       	modid = {$modid}
			 	WHERE 		orcid = {$orcid}";
    }

    if($db->executar($sql)){
        $db->commit();
        if($request['link'] == 'proximo'){
            $link = '\'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A\'';
        } else {
            $link = 'window.location';
        }
        
        echo "<script>
				alert('Dados salvos.');
				window.location = $link;
			  </script>";
    } else {
        $db->rollback();
        echo "<script>
				alert('Erro ao gravar.');
				window.location = 'catalogocurso2014.php?modulo=principal/cadOrganizacaoCurso&acao=A';
			  </script>";
    }

}

function excluirOrganizacaoCurso($request){
    global $db;

    $sql = "UPDATE 		catalogocurso2014.organizacaocurso 
    		SET			orcstatus = 'I'
			WHERE		orcid = {$request['orcid']}";

    if($db->executar($sql)){
        $db->commit();
        echo "<script>
				alert('Organização excuída.');
				window.location = window.location;
			  </script>";
    } else {
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

    $sql = "SELECT		curconttel,
						curconttel2,
						TRIM(curcontdesc) AS curcontdesc,
						TRIM(curcontemail) AS curcontemail,
						TRIM(curcontsite) AS curcontsite,
						TRIM(curcontinfo) AS curcontinfo
			FROM		catalogocurso2014.curso
			WHERE		curid = {$_SESSION['catalogo']['curid']} AND curano = {$_SESSION['exercicio']}";
    
    return $db->pegaLinha($sql);
}

function salvarContato($request){
    global $db;

    extract($request);

    $curconttel = $curconttel ? "'".$curconttel."'" : 'NULL';
    $curconttel2 = $curconttel2 ? "'".$curconttel2."'" : 'NULL';
    $curcontdesc = $curcontdesc ? "'".$curcontdesc."'" : 'NULL';
    $curcontemail = $curcontemail ? "'".$curcontemail."'" : 'NULL';
    $curcontsite = $curcontsite ? "'".$curcontsite."'" : 'NULL';
    $curcontinfo = $curcontinfo ? "'".$curcontinfo."'" : 'NULL';
    $corid = $corid ? $corid : 'NULL';  

    if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
	    $sql = "UPDATE 		catalogocurso2014.curso
	    		SET			curconttel = {$curconttel},
							curconttel2 = {$curconttel2},
							curcontdesc = {$curcontdesc},
							curcontemail = {$curcontemail},
							curcontsite = {$curcontsite},
							curcontinfo = {$curcontinfo},
							coordid = {$coordid}
				WHERE		curid = {$_SESSION['catalogo']['curid']};";
    } else {
 	    $sql = "UPDATE 		catalogocurso2014.curso 
	    		SET			curconttel = {$curconttel},
							curconttel2 = {$curconttel2},
							curcontdesc = {$curcontdesc},
							curcontemail = {$curcontemail},
							curcontsite = {$curcontsite},
							curcontinfo = {$curcontinfo},
							coordid = {$coordid},
							corid = {$corid}
				WHERE		curid = {$_SESSION['catalogo']['curid']};";   	
    }
	    
    if($db->executar($sql)){
        $db->commit();
        echo "<script>
				alert('Dados salvos.');
				window.location = window.location;
			  </script>";
    } else {
        $db->rollback();
        echo "<script>
				alert('Erro ao gravar.');
				window.location = window.location;
			  </script>";
    }
}

// Permissões
function testaPermissao(){
    global $db;

    $pflcod = pegaPerfil($_SESSION['usucpf']);

    if( $db->testa_superuser() || $pflcod == PERFIL_ADMINISTRADOR ){
        $permissoes['gravar'] = true;
    } else {
        $docid = prePegarDocid( $_SESSION['catalogo']['curid'] );
        $estadoDoc = pegaEstadoAtual($docid);

        if($estadoDoc == WF_EM_ELABORACAO){
            $permissoes['gravar'] = true;
        } else {
            $permissoes['gravar'] = false;
        }
    }
    return $permissoes;
}

//
//Workflow
//

function verificaPreenchimento( $curid ){
    global $db;

    $sql = "SELECT		ateid,
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
			FROM		catalogocurso2014.curso c
			WHERE		c.curid = {$curid}";
  
    $dadosAba1 = $db->pegaLinha($sql);
    foreach($dadosAba1 as $dado){
        if( !($dado!='') ){
            return false;
        }
    }

    $sql = "SELECT		true
			FROM		catalogocurso2014.funcaoexercida_curso_publicoalvo
			WHERE		curid = {$curid}";

    if(!$db->pegaUm($sql)){
        return false;
    }

    $sql = "SELECT		fexid
			FROM		catalogocurso2014.funcaoexercida_curso_publicoalvo
			WHERE		curid = {$curid}";

    $fexids = $db->carregarColuna($sql);

    if(in_array(FE_DOCENTE,$fexids)){

        if( in_array($dadosAba1['pacod_escolaridade'],Array(6,7)) ){
            $sql = "SELECT		true
					FROM		catalogocurso2014.areaformacaocurso
					WHERE		curid = {$curid}";

            if(!$db->pegaUm($sql)){
                return false;
            }
        }

        $sql = "SELECT		true
				FROM		catalogocurso2014.etapaensino_curso_publicoalvo
				WHERE		curid = {$curid}";

        if(!$db->pegaUm($sql)){
            return false;
        }

        $sql = "SELECT		true
				FROM		catalogocurso2014.diciplinacurso
				WHERE		curid = {$curid}";

        if(!$db->pegaUm($sql)){
            return false;
        }

        $sql = "SELECT		cod_mod_ensino
				FROM		catalogocurso2014.tab_mod_ensino_curso
				WHERE		curid = {$curid}";

        if(!$db->pegaUm($sql)){
            return false;
        }
    }

    $sql = "SELECT	true
			FROM	catalogocurso2014.equipecurso
			WHERE	eqcstatus = 'A' AND curid = {$curid}";

    if(!$db->pegaUm($sql)){
        return false;
    }

    $sql = "SELECT		true
			FROM		catalogocurso2014.etapaensino_curso
			WHERE		curid = {$curid}";

    if(!$db->pegaUm($sql)){
        return false;
    }

    $sql = "SELECT		true
			FROM		catalogocurso2014.organizacaocurso
			WHERE		orcstatus = 'A' AND curid = $curid";

    if(!$db->pegaUm($sql)){
        return false;
    }

    $sql = "SELECT		true
			FROM		catalogocurso2014.publicoalvo_curso
			WHERE		curid = {$curid}";

    if(!$db->pegaUm($sql)){
        return false;
    }
    return true;
}

function recuperaCursoResponssavel(){
    global $db;

    $pflcod = pegaPerfil($_SESSION['usucpf']);
    if( !$db->testa_superuser() || $pflcod != PERFIL_ADMINISTRADOR || $pflcod != PERFIL_CONSULTA ){
    	
        $sql = "SELECT 		DISTINCT curid
				FROM		catalogocurso2014.usuarioresponsabilidade
				WHERE		usucpf = '{$_SESSION['usucpf']}' AND pflcod = {$pflcod} AND curid IS NOT NULL AND rpustatus = 'A'";
        return $db->carregarColuna($sql);
    }
    return true;
}

function recuperaCoordenacaoResponssavel(){
    global $db;

    if(!$db->testa_superuser() && $pflcod != PERFIL_ADMINISTRADOR && $pflcod != PERFIL_CONSULTA ){
        $pflcod = pegaPerfil($_SESSION['usucpf']);
        $sql = "SELECT 		DISTINCT coordid
				FROM		catalogocurso2014.usuarioresponsabilidade
				WHERE		usucpf = '{$_SESSION['usucpf']}' AND pflcod = {$pflcod} AND coordid IS NOT NULL AND rpustatus = 'A'";

        return $db->carregarColuna($sql);
    }
    return $db->testa_superuser();
}

function pegaPerfis($usucpf){
    global $db;

    $sql = "SELECT		pu.pflcod
			FROM 		seguranca.perfil AS p
			LEFT JOIN 	seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE		p.sisid = '{$_SESSION['sisid']}' AND pu.usucpf = '{$usucpf}'";


    $pflcods = $db->carregarColuna($sql);
    return $pflcods;
}

function pegaPerfil($usucpf){
    global $db;

    $sql = "SELECT		pu.pflcod
			FROM 		seguranca.perfil AS p
			LEFT JOIN 	seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE		p.sisid = '{$_SESSION['sisid']}' AND pu.usucpf = '{$usucpf}'";

    $pflcod = $db->pegaUm($sql);
    return $pflcod;
}

function pegaDocidCurso($curid){
    global $db;

    $sql = "SELECT	docid
			FROM 	catalogocurso2014.curso
			WHERE	curid = $curid";

    $docid = $db->pegaUm($sql);
    return $docid;
}

function criarversao(){
    global $db;

    $curidpai = $_SESSION['catalogo']['curid'];

    if( $_SESSION['catalogo']['curid'] ){

        $sql = "INSERT INTO catalogocurso2014.curso
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
					curchmim, curchmax, curpercpremim, curpercpremax, curpademsocialtxt, curpademsocialpercmax, curano, curversao
				)
				SELECT		ncuid, modid, ateid, curdesc, curementa,
							curfunteome, curobjetivo, curmetodologia, curcertificado, curcoordenacao,
							curstatus, curchpremim, curchdistmim, curnumestudanteidealpre,
							curnumestudanteminpre, curnumestudantemaxpre, curqtdmonitora,
							uteid, curdtinclusao, eteid, curchpremax, curchdistmax,
							curinfra, curcustoaluno, curofertanacional, panesid,
							paeteid, pamodid, curpaoutrasexig, curpatutor, curpatutortxt,
							curpademsocial, curconttel, curconttel2, curcontdesc, curcontemail,
							curcontsite, curcontinfo, co_interno_uorg, curnumestudanteidealdist,
							curnumestudantemindist, curnumestudantemaxdist, lesid, ldeid, cursalamulti, coordid,
							curchmim, curchmax, curpercpremim, curpercpremax, curpademsocialtxt, curpademsocialpercmax, curano, curversao+1
				FROM		catalogocurso2014.curso
				WHERE		curid = {$curidpai}
				RETURNING	curid";
        $curidnovo = $db->pegaUm($sql);

        preCriarDocumento( $curidnovo );

        // Atualiza pai com id do filho
        $sql = "UPDATE 		catalogocurso2014.curso SET
							curidfilho = {$curidnovo}
				WHERE		curid = $curidpai;";


        $sql .= "INSERT INTO catalogocurso2014.funcaoexercida_curso_publicoalvo(fexid,curid)
				SELECT		fexid,
							$curidnovo
				FROM		catalogocurso2014.funcaoexercida_curso_publicoalvo
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.areaformacaocurso(cod_area_ocde,curid)
				SELECT		cod_area_ocde,
							$curidnovo
				FROM		catalogocurso2014.areaformacaocurso
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.etapaensino_curso_publicoalvo(cod_etapa_ensino, curid)
				SELECT		cod_etapa_ensino,
							$curidnovo
				FROM		catalogocurso2014.etapaensino_curso_publicoalvo
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.diciplinacurso(cod_disciplina,curid)
				SELECT		cod_disciplina,
							$curidnovo
				FROM		catalogocurso2014.diciplinacurso
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.tab_mod_ensino_curso(cod_mod_ensino, curid)
				SELECT		cod_mod_ensino,
							$curidnovo
				FROM		catalogocurso2014.tab_mod_ensino_curso
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.equipecurso
        		(
					camid, unrid, nesid, eqcfuncao, eqcminimo, eqcmaximo,
			        eqcatribuicao, eqcoutrosreq, eqcbolsista, cod_escolaridade, qtdfuncao, curid
			    )
				SELECT		camid, unrid, nesid, eqcfuncao, eqcminimo, eqcmaximo,
		        			eqcatribuicao, eqcoutrosreq, eqcbolsista, cod_escolaridade, qtdfuncao,
							$curidnovo
				FROM		catalogocurso2014.equipecurso
				WHERE		eqcstatus = 'A'	AND curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.etapaensino_curso(cod_etapa_ensino, curid)
				SELECT		cod_etapa_ensino,
							$curidnovo
				FROM		catalogocurso2014.etapaensino_curso
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.organizacaocurso
				(
					tioid, orcdesc, orchadisminimo, orchadismaximo, orchapreminimo,
			        orchapremaximo, orcementa, orcstatus, modid, orcchmim,
			        orcchmax, orcpercpremim, orcpercpremax, curid
				)
				SELECT		tioid, orcdesc, orchadisminimo, orchadismaximo, orchapreminimo,
			        		orchapremaximo, orcementa, orcstatus, modid, orcchmim,
			        		orcchmax, orcpercpremim, orcpercpremax, $curidnovo
				FROM		catalogocurso2014.organizacaocurso
				WHERE		orcstatus = 'A' AND curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.publicoalvo_curso(cod_escolaridade, curid)
				SELECT		cod_escolaridade,
							$curidnovo
				FROM		catalogocurso2014.publicoalvo_curso
				WHERE		curid = {$curidpai} ;";

        $sql .= "INSERT INTO catalogocurso2014.cursorede(redid, curid)
				SELECT		redid,
							$curidnovo
				FROM		catalogocurso2014.cursorede
				WHERE		curid = {$curidpai};";

        $sql .= "INSERT INTO catalogocurso2014.nivelcurso_curso(ncuid, curid)
				SELECT		ncuid,
							$curidnovo
				FROM		catalogocurso2014.nivelcurso_curso
				WHERE		curid = {$curidpai};";

        if($db->executar($sql)){
            $db->commit();
            echo "<script>alert('Nova versão criada.');window.location = 'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A&curid=$curidnovo';</script>";
        } else {
            echo "<script>alert('Escolha um curso.');window.location = 'catalogocurso2014.php?modulo=principal/listaCursos&acao=A';</script>";
        }
    } else {
        echo "<script>alert('Escolha um curso.');window.location = 'catalogocurso2014.php?modulo=principal/listaCursos&acao=A';</script>";
    }
}

function salvarPublicoAlvo($request){
    global $db;

    extract($request);

    $curid = $_SESSION['catalogo']['curid'];

    $panesid = $nesid ? $nesid : 'NULL';
    $pamodid = $modid ? $modid : 'NULL';
    $curpaoutrasexig = $curpaoutrasexig ? "'".$curpaoutrasexig."'" : 'NULL';
    $curpatutor = $curpatutor == 'S' ? 'TRUE' : 'FALSE';
    $curpatutortxt = $curpatutortxt ? "'".$curpatutortxt."'" : 'NULL';
    $curpademsocial = $curpademsocial == 'S' ? 'TRUE' : 'FALSE';
    $curpafuncao = $curpafuncao == 'S' ? 'TRUE' : 'FALSE';
    $pacod_escolaridade = $pacod_escolaridade ? $pacod_escolaridade : 'NULL';
    $pacod_mod_ensino = $pacod_mod_ensino ? $pacod_mod_ensino : 'NULL';
    $curpademsocialpercmax = $curpademsocialpercmax && ($curpademsocial == 'TRUE') ? $curpademsocialpercmax : 'NULL';
    $cursalamulti = $cursalamulti ? $cursalamulti : 'NULL';
    $lesid = $lesid ? $lesid : 'NULL';
    $ldeid = $ldeid ? $ldeid : 'NULL';
    $cod_etapa_ensino = $cod_etapa_ensino ? $cod_etapa_ensino : array();

    $sql = "DELETE FROM catalogocurso2014.etapaensino_curso WHERE curid = {$curid};";
    if( $cod_etapa_ensino[0] != '' ){
        foreach($cod_etapa_ensino as $id){
            $sql .= "INSERT INTO catalogocurso2014.etapaensino_curso (cod_etapa_ensino,curid) VALUES ({$id}, {$curid});";
        }
    }

    $sql .= "UPDATE 		catalogocurso2014.curso 
    		 SET			panesid = {$panesid},
					        curpaoutrasexig = {$curpaoutrasexig},
					        curpatutor = {$curpatutor},
					        curpatutortxt = {$curpatutortxt},
					        curpademsocial = {$curpademsocial},
					        pacod_escolaridade = {$pacod_escolaridade},
					        pacod_mod_ensino = {$pacod_mod_ensino},
					        curpademsocialpercmax = {$curpademsocialpercmax},
					        cursalamulti = '{$cursalamulti}',
					        lesid = {$lesid},
					        ldeid = {$ldeid}
			  WHERE			curid = {$curid};";

    $sql .= "DELETE FROM catalogocurso2014.areaformacaocurso WHERE curid = {$curid};";
    
    if(is_array($cod_area_ocde) && $cod_area_ocde[0] != ''){
        if( in_array('999',$cod_area_ocde) ){
            $sql .= "INSERT INTO catalogocurso2014.areaformacaocurso(cod_area_ocde,curid) VALUES ('999', {$curid});";
        } else {
            foreach($cod_area_ocde as $id){
                $sql .= "INSERT INTO catalogocurso2014.areaformacaocurso(cod_area_ocde,curid) VALUES ('{$id}', {$curid});";
            }
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.diciplinacurso WHERE curid = {$curid};";

    if(is_array($cod_disciplina) && $cod_disciplina[0] != ''){
        if( in_array('999',$cod_disciplina) ){
            $sql .= "INSERT INTO catalogocurso2014.diciplinacurso(cod_disciplina,curid) VALUES (999, {$curid}) ;";
        } else {
            foreach($cod_disciplina as $id){
                $sql .= "INSERT INTO catalogocurso2014.diciplinacurso(cod_disciplina,curid) VALUES ({$id}, {$curid}) ;";
            }
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.etapaensino_curso_publicoalvo WHERE curid = {$curid};";
    
    if( $cod_etapa_ensino[0] != '' ){
        if( in_array('999',$cod_etapa_ensino) ){
            $sql .= "INSERT INTO catalogocurso2014.etapaensino_curso_publicoalvo (cod_etapa_ensino,curid) VALUES (999, {$curid}) ;";
        }else{
            foreach($cod_etapa_ensino as $id){
                $sql .= "INSERT INTO catalogocurso2014.etapaensino_curso_publicoalvo (cod_etapa_ensino,curid) VALUES ({$id}, {$curid}) ;";
            }
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.funcaoexercida_curso_publicoalvo WHERE curid = {$curid};";
    
    if( $fexid[0] != '' ){
        if( in_array('999',$fexid) ){
            $sql .= "INSERT INTO catalogocurso2014.funcaoexercida_curso_publicoalvo (fexid,curid) VALUES (999, {$curid}) ;";
        }else{
            foreach($fexid as $id){
                $sql .= "INSERT INTO catalogocurso2014.funcaoexercida_curso_publicoalvo (fexid,curid) VALUES ({$id}, {$curid}) ;";
            }
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.tab_mod_ensino_curso WHERE curid = {$curid};";
    
    if( $cod_mod_ensino[0] != '' ){
        if( in_array('999',$cod_mod_ensino) ){
            $sql .= "INSERT INTO catalogocurso2014.tab_mod_ensino_curso (cod_mod_ensino,curid) VALUES (999, {$curid}) ;";
        }else{
            foreach($cod_mod_ensino as $id){
                $sql .= "INSERT INTO catalogocurso2014.tab_mod_ensino_curso (cod_mod_ensino,curid) VALUES ({$id}, {$curid}) ;";
            }
        }
    }

    $sql .= "DELETE FROM catalogocurso2014.cursodemandasocial WHERE curid = {$curid};";
    
    if( $padid[0] != '' && ($curpademsocial == 'TRUE') ){
        if( in_array('999',$padid) ){
            $sql .= "INSERT INTO catalogocurso2014.cursodemandasocial (padid,curid) VALUES (999, {$curid}) ;";
        }else{
            foreach($padid as $id){
                $sql .= "INSERT INTO catalogocurso2014.cursodemandasocial (padid,curid) VALUES ({$id}, {$curid}) ;";
            }
        }
    }

    $eqcid = $db->pegaUm($sql);

    $sql = "DELETE FROM catalogocurso2014.publicoalvo_curso WHERE curid = {$curid};";
    
    if(is_array($cod_escolaridade) && count($cod_escolaridade) > 0 ){
        if( in_array('999',$cod_escolaridade) ){
            $sql .= "INSERT INTO catalogocurso2014.publicoalvo_curso(cod_escolaridade,curid) VALUES (999, {$curid}) ;";
        } else {
            foreach($cod_escolaridade as $id){
                $sql .= "INSERT INTO catalogocurso2014.publicoalvo_curso(cod_escolaridade,curid) VALUES ({$id}, {$curid}) ;";
            }
        }
    }

    if($db->executar($sql)){
        $db->commit();
        if($request['link'] == 'proximo'){
            $link = '\'catalogocurso2014.php?modulo=principal/cadContato&acao=A\'';
        }else{
            $link = 'window.location';
        }
        echo "<script>
				alert('Dados salvos.');
				window.location = $link;
			  </script>";
    } else {
        $db->rollback();
        echo "<script>
				alert('Erro ao gravar.');
				window.location = window.location;
			  </script>";
    }
}

function atualizarCoordenacaoResponsavel($post){
	global $db;
	
	extract($post);
	
	$sql = "SELECT 	corid AS codigo, 
					cordesc AS descricao
			FROM 	catalogocurso2014.coordresponsavel
  			WHERE 	coordid = {$coordid} AND corstatus = 'A'";
	
	$db->monta_combo('corid', $sql, 'S', 'Selecione...', '', '', 'Coordenação Responsável', '450', 'S', 'corid');
} 

function alertlocation($dados) {
    die("<script>
			" . (($dados['alert']) ? "alert('" . $dados['alert'] . "');" : "") . "
			" . (($dados['location']) ? "window.location='" . $dados['location'] . "';" : "") . "
			" . (($dados['javascript']) ? $dados['javascript'] : "") . "
		 </script>");
} 


function excluirPerfil($request){
	global $db;
	
	extract($request);
	
    $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocareaformacao WHERE pafid = {$pafid};";
    $sql .= "DELETE FROM catalogocurso2014.publicoalvo_assocdisciplina WHERE pafid = {$pafid};";
    $sql .= "DELETE FROM catalogocurso2014.publicoalvo_assocetapaensino WHERE pafid = {$pafid};";
    $sql .= "DELETE FROM catalogocurso2014.publicoalvo_assocnivelescolaridade WHERE pafid = {$pafid};";
    $sql .= "DELETE FROM catalogocurso2014.publicoalvo_assocmodensino WHERE pafid = {$pafid};";
    $sql .= "DELETE FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE pafid = {$pafid};";

    $db->executar($sql);
    
    if($db->commit()){
    	return true;
    } else {
    	return false;
    }
}

function listarPerfil($curid,$tipo=NULL){
	global $db;	
	
	if($tipo=='editar'){
		$acao = "'<img border=\"0\" src=\"../imagens/editar_nome_vermelho.gif\" id=\"'|| paf.pafid ||'\" onclick=\"editarPerfil('|| paf.pafid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
				  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| paf.pafid ||'\" onclick=\"excluirPerfil('|| paf.pafid ||');\" style=\"cursor:pointer;\"/>' AS acao,";

		if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
			$tamanho = array('10%', '30%', '', '', '');
    		$alinhamento = array('center', '', '', '', '');	
	    	$cabecalho = array('Ação', 'Perfil', 'Nivel de Escolaridade mínimo exigido', 'Área de Formação', 'Disciplina','Etapa de ensino','Modalidade em que leciona');
	    } else {
	    	$cabecalho = array('Ação', 'Perfil','Nivel de Escolaridade mínimo exigido','Outras Exigências');
	    }
	} else {
		$acao = "";
		$tamanho = array();
		$alinhamento = array();
		if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
	    	$cabecalho = array('Perfil', 'Nivel de Escolaridade mínimo exigido', 'Área de Formação', 'Disciplina','Etapa de ensino','Modalidade em que leciona');
	    } else {
	    	$cabecalho = array('Perfil','Nivel de Escolaridade mínimo exigido','Outras Exigências');
	    }
	}
	
	if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
		$select =  "fexdesc,
					-- Nivel Escolaridade
					array_to_string(array(
					(SELECT 	no_escolaridade
					 FROM 		catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					 INNER JOIN educacenso_".(ANO_CENSO).".tab_escolaridade ne on ne.pk_cod_escolaridade = pne.nivelescolaridadeid
					 WHERE 		pne.pafid = paf.pafid AND pne.aniano = {$_SESSION['exercicio']}
					 ORDER BY 	no_escolaridade)
					 UNION ALL
					(SELECT		no_pos_graduacao AS no_escolaridade
					 FROM		catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					 INNER JOIN educacenso_".(ANO_CENSO).".tab_pos_graduacao e ON (e.pk_pos_graduacao||'0')::integer = pne.nivelescolaridadeid
					 INNER JOIN catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
					 WHERE 		pne.pafid = paf.pafid AND h.hneano = {$_SESSION['exercicio']} AND pne.aniano = {$_SESSION['exercicio']})), '<br /> ') AS nivel_escolaridade,		
					-- Area
					array_to_string(array(
					SELECT 		no_nome_area_ocde
					FROM 		catalogocurso2014.publicoalvo_assocareaformacao a
					INNER JOIN 	educacenso_".(ANO_CENSO).".tab_area_ocde af on af.pk_cod_area_ocde = a.pk_cod_area_ocde
					WHERE		a.pafid = paf.pafid AND a.pk_cod_area_ocde_ano = {$_SESSION['exercicio']}
					ORDER BY 	no_nome_area_ocde
					), '<br /> ') AS area_formacao,
					-- Disciplina
					array_to_string(array(
					SELECT 		no_disciplina
					FROM 		catalogocurso2014.publicoalvo_assocdisciplina pad
					INNER JOIN 	educacenso_".(ANO_CENSO).".tab_disciplina d on d.pk_cod_disciplina = pad.pk_cod_disciplina
					WHERE 		pad.pafid = paf.pafid AND pad.asdano = {$_SESSION['exercicio']}
					ORDER BY 	no_disciplina
					), '<br /> ') AS disciplina,
					-- Etapa de ensino
					array_to_string(array(
					SELECT 		no_etapa_ensino
					FROM 		catalogocurso2014.publicoalvo_assocetapaensino e
					INNER JOIN 	educacenso_".(ANO_CENSO).".tab_etapa_ensino ee on ee.pk_cod_etapa_ensino = e.pk_cod_etapa_ensino
					WHERE 		e.pafid = paf.pafid AND e.aseano = {$_SESSION['exercicio']}
					ORDER BY 	no_etapa_ensino
					), '<br /> ') AS etapa_ensino,
					-- Mod Ensino
					array_to_string(array(
					SELECT 		no_mod_ensino
					FROM 		catalogocurso2014.publicoalvo_assocmodensino pme
					INNER JOIN 	educacenso_".(ANO_CENSO).".tab_mod_ensino me on me.pk_cod_mod_ensino = pme.modensinoid
					WHERE 		pme.pafid = paf.pafid AND pme.amoano = {$_SESSION['exercicio']}
					ORDER BY 	no_mod_ensino
					), '<br /> ') AS mod_ensino";
	} else {
		$select =	"fexdesc,
					 -- Nivel Escolaridade
					 array_to_string(array(
					 (SELECT 	no_escolaridade
					 FROM 		catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					 INNER JOIN educacenso_".(ANO_CENSO).".tab_escolaridade ne on ne.pk_cod_escolaridade = pne.nivelescolaridadeid
					 WHERE 		pne.pafid = paf.pafid AND pne.aniano = {$_SESSION['exercicio']}
					 ORDER BY 	no_escolaridade)
					 UNION ALL
					 (SELECT		no_pos_graduacao AS no_escolaridade
					 FROM			catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					 INNER JOIN 	educacenso_".(ANO_CENSO).".tab_pos_graduacao e ON (e.pk_pos_graduacao||'0')::integer = pne.nivelescolaridadeid
					 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
					 WHERE 		pne.pafid = paf.pafid AND h.hneano = {$_SESSION['exercicio']} AND pne.aniano = {$_SESSION['exercicio']})), '<br /> ') AS nivel_escolaridade,
					 pafoutrasexigencias";
	}
	
    $sql = "SELECT			$acao
							$select
			FROM 			catalogocurso2014.publicoalvo_assocfuncaoexercida paf
			LEFT JOIN 		catalogocurso2014.funcaoexercida fe ON fe.fexid = paf.fexid
			WHERE 			curid = {$curid} AND fexstatus = 'A'";
    
	$db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', '', $tamanho, $alinhamento);    
}

function salvarPerfil($request){
	global $db;		

	extract($request);	

    if(!$perfil){
    	$perfil = 2;
    } 
    
    if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
		$pafoutrasexigencias = "";
    }
    
    if($perfil && $_SESSION['catalogo']['curid']){
    	if($pafid){
        	$sql = "UPDATE catalogocurso2014.publicoalvo_assocfuncaoexercida SET fexid = {$perfil}, pafoutrasexigencias = '{$pafoutrasexigencias}' WHERE pafid = {$pafid};";
            $db->executar($sql);
        } else {
            $sql = "INSERT INTO catalogocurso2014.publicoalvo_assocfuncaoexercida (curid, fexid, pafoutrasexigencias) VALUES ({$_SESSION['catalogo']['curid']}, {$perfil}, '{$pafoutrasexigencias}') RETURNING pafid;";
            $pafid = $db->pegaUm($sql);
        }

        if($pafid){
        	// Area
            $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocareaformacao WHERE pafid = {$pafid}";
            $db->executar($sql);
            if($_POST['pk_cod_area_ocde'] && is_array($_POST['pk_cod_area_ocde']) && !empty($_POST['pk_cod_area_ocde'][0]) ){
            	$sql = '';
                foreach($_POST['pk_cod_area_ocde'] as $pk_cod_area_ocde){
                	$sql .= "INSERT INTO catalogocurso2014.publicoalvo_assocareaformacao (pafid, pk_cod_area_ocde, pk_cod_area_ocde_ano) VALUES ({$pafid},'{$pk_cod_area_ocde}',{$_SESSION['exercicio']});";
                }
                $db->executar($sql);
            }

            // Disciplina
            $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocdisciplina WHERE pafid = {$pafid}";
            $db->executar($sql);
            if($_POST['pk_cod_disciplina'] && is_array($_POST['pk_cod_disciplina'])  && !empty($_POST['pk_cod_disciplina'][0])){                	
            	$_POST['pk_cod_disciplina'] = array_values($_POST['pk_cod_disciplina']);
                $sql = '';
                foreach($_POST['pk_cod_disciplina'] as $pk_cod_disciplina){
                	$sql .= "INSERT INTO catalogocurso2014.publicoalvo_assocdisciplina (pafid , pk_cod_disciplina, asdano) VALUES ({$pafid}, {$pk_cod_disciplina}, {$_SESSION['exercicio']});";
                }
                $db->executar($sql);
            }

            // Etapa ensino
            $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocetapaensino WHERE pafid = {$pafid}";
            $db->executar($sql);
            if($_POST['pk_cod_etapa_ensino'] && is_array($_POST['pk_cod_etapa_ensino'])  && !empty($_POST['pk_cod_etapa_ensino'][0])){
            	$sql = '';
                foreach($_POST['pk_cod_etapa_ensino'] as $pk_cod_etapa_ensino){
                	$sql .= "INSERT INTO catalogocurso2014.publicoalvo_assocetapaensino(pafid, pk_cod_etapa_ensino, aseano) VALUES ({$pafid}, {$pk_cod_etapa_ensino}, {$_SESSION['exercicio']});";
                }
                $db->executar($sql);
            }

            // Nivel Escolaridade.
            $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocnivelescolaridade WHERE pafid = {$pafid}";
            $db->executar($sql);
            if($_POST['cod_escolaridade'] && is_array($_POST['cod_escolaridade']) && !empty($_POST['cod_escolaridade'][0])){
            	 $sql = '';
                 foreach($_POST['cod_escolaridade'] as $cod_escolaridade){
                 	$sql .= "INSERT INTO catalogocurso2014.publicoalvo_assocnivelescolaridade(pafid, nivelescolaridadeid,aniano) VALUES ({$pafid}, {$cod_escolaridade},{$_SESSION['exercicio']});";
                 }
                 $db->executar($sql);
            }

            // Modalidade em que leciona.
            $sql = "DELETE FROM catalogocurso2014.publicoalvo_assocmodensino WHERE pafid = {$pafid}";
            $db->executar($sql);
            if($_POST['cod_mod_ensino'] && is_array($_POST['cod_mod_ensino']) && count($_POST['cod_mod_ensino']) > 0 && !empty($_POST['cod_mod_ensino'][0])){
            	$sql = '';
                foreach($_POST['cod_mod_ensino'] as $cod_mod_ensino){
                	$sql .= "INSERT INTO catalogocurso2014.publicoalvo_assocmodensino(pafid, modensinoid, amoano) VALUES ({$pafid}, {$cod_mod_ensino}, {$_SESSION['exercicio']});";
                }
                $db->executar($sql);
            }

            $db->commit();
            if($linkp=='proximo'){
				$al = array('alert' => 'Salvo com sucesso!', 'location' => 'catalogocurso2014.php?modulo=principal/cadIesOfertante&acao=A');
            } else {
            	$al = array('alert' => 'Salvo com sucesso!', 'location' => 'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A');
            }
		} else {
          	$al = array('alert' => 'Não pode salvar, pois não possui funcção exercida neste curso!', 'location' => 'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A');
        }
	}
    alertlocation($al);
} 

function listarIESOfertante($curid){
	global $db;			

	$sql = "SELECT 		ieo.unicod, 
						ies.entsig, 
						ies.entnome, 
						cur.curdesc, 
						ieo.ieoanoprojeto, 
						ieo.ieoqtdvagas
            FROM 		catalogocurso2014.iesofertante ieo
            INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid
            INNER JOIN 	entidade.entidade ies ON ies.entid = ieo.entid
            LEFT JOIN 	catalogocurso2014.coordenacao cor ON cor.coordid = cur.coordid
            WHERE 		ieo.curid = {$curid} AND ieo.ieostatus = 'A' AND cur.curano = {$_SESSION['exercicio']}
            ORDER BY 	ies.entsig, ies.entnome";

	$tamanho = array('5%', '50%', '25%', '10%', '10%');
    $alinhamento = array('center', '', '', '', '');	
	$cabecalho = array('Código UO', 'Sigla IES', 'Nome IES', 'Ano','Qtd. Vagas');

	$db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', '', $tamanho, $alinhamento);   
}

function carregarEquipeAtribuicao($request){
	global $db;			

	extract($request);
	
	if($fueid){
		$sql = "SELECT		fueatribuicao,
							fueexperiencia     
				FROM 		catalogocurso2014.funcaoequipe 
				WHERE		fueid = {$fueid}";
		
		$fueatribuicao = $db->pegaLinha($sql);
		$fueatribuicao['fueatribuicao'] = iconv("ISO-8859-1", "UTF-8", $fueatribuicao['fueatribuicao']);
		$fueatribuicao['fueexperiencia'] = iconv("ISO-8859-1", "UTF-8", $fueatribuicao['fueexperiencia']);
		
	    echo simec_json_encode($fueatribuicao);
	} else {
		$fueatribuicao['funcao'] = "N";
		echo simec_json_encode($fueatribuicao);
	}
}

function carregarEquipeEscolaridade($request){
	global $db;			

	extract($request);	

	if($fueid){
	    $sql = "SELECT 		esc.codigo AS codigo,
	    					esc.descricao AS descricao
				FROM 		catalogocurso2014.funcaoequipeescolaridade func
				INNER JOIN 	((SELECT		to_char(pk_cod_escolaridade,'9') AS codigo, 
							 				no_escolaridade AS descricao
						  	 FROM 			educacenso_2013.tab_escolaridade e)
							 UNION ALL
							(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo, 
											no_pos_graduacao AS descricao				
							 FROM 			educacenso_2013.tab_pos_graduacao e)) AS esc ON esc.codigo::integer = func.cod_escolaridade
			   WHERE		 func.fueid = {$fueid}";
	    
	      $niveis = $db->carregar($sql);
	      $nv = Array();
	      if(is_array($niveis)){
	      	foreach($niveis as $nivel){
	        	if(!in_array($nivel['codigo'],$nv)){
	            	array_push($nv, $nivel['codigo']);
	            }
	            $checked = 'checked="checked"';
	            echo "<br><input type=\"checkbox\" id=\"".$nivel['codigo']."\" class=\"".implode(" ",$nv)."\" value=\"".$nivel['codigo']."\" $checked name=\"cod_escolaridade[]\" disabled=\"disabled\"> ".$nivel['descricao'];
	     	}
		}
	} else {
		echo 'Selecione a Função.';		
	}
}
?>