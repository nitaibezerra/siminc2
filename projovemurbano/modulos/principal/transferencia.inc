<?php

if( $_POST['arqid'] != '' ){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_POST['arqid']);
    echo"<script>window.location.href = window.location.href;</script>";
    exit;
} 

header('Content-Type: text/html; charset=cp1252');

function testaidade($caeid,$ppuid){
	
	global $db;
	$ppuid = trim($ppuid);
	if($ppuid!=3 && !empty($ppuid)){
	$sql = "SELECT
                ppuano 
            FROM
                projovemurbano.programaprojovemurbano
            WHERE
                ppustatus = 'A'
			AND ppuid = $ppuid
            ORDER BY
                ppuano";
	$anodestino = $db->pegaUm($sql);
	$sql = "SELECT
				caecpf as cpf,
				caenome as nome
			FROM
				projovemurbano.cadastroestudante
			WHERE
				caeid = $caeid
			AND 	(($anodestino - extract(year from caedatanasc)<18) OR ($anodestino - extract(year from caedatanasc)>29))";
	
	$dadosaluno = $db->pegaLinha($sql);
	}else{
		$anodestino1 = 2014;
		$anodestino2 = 2015;
		$sql = "SELECT
					caecpf as cpf,
					caenome as nome
				FROM
					projovemurbano.cadastroestudante
				WHERE
					caeid = $caeid
				AND 	(($anodestino1 - extract(year from caedatanasc)<18) OR ($anodestino1 - extract(year from caedatanasc)>29))";
		$dadosaluno = $db->pegaLinha($sql);
		if($dadosaluno){
			$sql = "SELECT
						caecpf as cpf,
					caenome as nome
					FROM
						projovemurbano.cadastroestudante
					WHERE
						caeid = $caeid
					AND 	(($anodestino2 - extract(year from caedatanasc)<18) OR ($anodestino2 - extract(year from caedatanasc)>29))";
			$dadosaluno = $db->pegaLinha($sql);
		}
	}
	
	return $dadosaluno;
}
function anoDestino(){
    
     global $db;
    
    $sql = "SELECT 
                ppuid as codigo,
                ppuano as descricao
            FROM 
                projovemurbano.programaprojovemurbano
            WHERE   
                ppustatus = 'A'
            ORDER BY
                ppuano";
    $ppuid = $_REQUEST['ppuid'];
//    ver($_POST['modalidade']);
    if($_POST['modalidade'] == 'M'){
    $db->monta_combo('ppuid', $sql, 'S', 'Selecione o Ano', '', '', '', '', 'S', 'ppuid');
    }else{
     $db->monta_combo('ppuid_outros', $sql, 'S', 'Selecione o Ano', '', '', '', '', 'S', 'ppuid_outros');
    }
}

function testaPossuiPolo(){

    global $db;
    extract( $_POST );
    if( $esfera == 'E'){
        $filtrocodigo = "AND pro.estuf = '$codigo'";
    }elseif( $esfera == 'M'){
        $filtrocodigo = "AND pro.muncod = '$codigo'";
    }
    
    $sql = "SELECT 
                pmu.pmupossuipolo 
            FROM projovemurbano.polomunicipio pmu
            INNER JOIN projovemurbano.projovemurbano pro ON pro.pjuid = pmu.pjuid   
            WHERE 
                pmu.pmustatus = 'A'
            AND pro.ppuid = ".$_POST['ppuid']." 
            $filtrocodigo            
            ";
	
    $testap = $db->pegaUm($sql);
    
    echo $testap;
    exit;
}

function buscarComboAlunos(){

    global $db;
    
    $sql1 = "SELECT 
                entnome 
            FROM entidade.entidade ent 
            INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid 
            WHERE 
                nes.nucid=nuc.nucid 
                AND nes.nuetipo='S'";
        
    $sql2 = "SELECT 
                entnome 
            FROM entidade.entidade ent 
            INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid 
            WHERE 
                nes.nucid=nuc.nucid 
                AND nes.nuetipo='A'";
    
    $where = Array('cae.caeid NOT IN ( SELECT cad_caeid FROM projovemurbano.transferencia tra
                                        INNER JOIN projovemurbano.historico_transferencia htr ON htr.htrid = tra.htrid_ultimo
                                        WHERE htr.shtid_status IN (1,2) )');
    
    if( $_POST['polid'] != '' && $_POST['polid'] != 'undefined' ){
        $where[] = "pol.polid = {$_POST['polid']}";
    }         
    
    if( $_POST['nucid'] != '' && $_POST['nucid'] != 'undefined'){
        $where[] = "nuc.nucid = {$_POST['nucid']}";
    }
    
    if( $_POST['turid'] != '' && $_POST['turid'] != 'undefined' ){
        $where[] = "t.turid = {$_POST['turid']}";
    }

    $sqlSelecionaVarios = "SELECT DISTINCT
                               cae.caeid as codigo,
                               removeacento(cae.caecpf||' - '||
                               cae.caenome || ': ' ||
                               coalesce('POLO '|| pol.polid || ' ','') ||
                               'NUCLEO '||nuc.nucid||',SEDE: '||
                               ($sql1)||COALESCE(', ANEXO:'||
                               ($sql2),'')|| ' Turma: '||
                               turdesc) as descricao
                           FROM 
                               projovemurbano.cadastroestudante cae
                           LEFT  JOIN projovemurbano.polo pol ON pol.polid = cae.polid
                           INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid
                           INNER JOIN projovemurbano.turma t ON t.turid = cae.turid
                           INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
                           INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                           LEFT  JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
                           WHERE
                               ".implode(" AND ", $where).(count($where)<2?'LIMIT 100':'');
//ver($sqlSelecionaVarios);
    mostrarComboPopup( 'Aluno(s):', 'caeid_combo',  $sqlSelecionaVarios, '', 'Selecione a(s) Situação(ões)' );
}

function enviar_aluno_cancelarTranferencia(){
    
    global $db;
    
    extract($_POST);
//    ver($_POST,d);
    if( $traid != '' ){
        
        $sql = "INSERT INTO projovemurbano.historico_transferencia(
                        traid,
                        shtid_status, 
                        usucpf_fezacao, 
                        htrdatahoraacao, 
                        justificativa)
                VALUES ($traid, 5,
                        '{$_SESSION['usucpf']}',
                        now(), 
                        'cancelado na lista')
                RETURNING 
                        htrid";
        $htrid  = $db->pegaUm($sql);
        
	    $sqlhistorico= "UPDATE projovemurbano.transferencia SET
					               	htrid_ultimo = $htrid
					            WHERE
						                traid = $traid;";
        if($sqlhistorico!=''){
        	$db->executar($sqlhistorico);
        }
        $db->commit();
        echo "
            <script>
                alert('Tranferência cancelada.');
                window.location.href = window.location.href;
            </script>";  
    }
}

function enviar_aluno_salvar(){
	
	global $db;
    extract($_POST);
    if(empty($caeid_combo)&&empty($caeid)){
    	echo "
	          <script>
	            alert('Você não selecionou nenhum aluno.');
	             window.location.href = window.location.href;
	          </script>
	             ";
    	die();
    }
    if( is_array($caeid_combo) ){
        $arrCaeid = $caeid_combo;
    }else{
        $arrCaeid = Array($caeid);
    }
	if($_SESSION['projovemurbano']['modalidade'] == 'M'){
		if($_SESSION['projovemurbano']['estuf']){
			$filtrolocalidade = "AND pju.estuf is not null";
		}ElSE{
			$filtrolocalidade = "AND pju.muncod is not null";
		}    
	}
	if($_SESSION['projovemurbano']['modalidade'] == 'M'){
		$ppuid1 = $_REQUEST['ppuid'];
	}else{
		$ppuid1 = $_REQUEST['ppuid_outros'];
	}

	if($_SESSION['projovemurbano']['modalidade'] == 'O'){  
	// 	ver($nucid ,d);
		if($nucid=='0'){
			echo "
	          <script>
	            alert('Você não selecionou um núcleo válido.');
	             window.location.href = window.location.href;
	          </script>
	             ";
			die();
		}
		
		if($nucid != '0' ||$nucid != ''){
	// 		VER(d);
	    $sql=" SELECT
	                nuc2.nucqtdestudantes - ( SELECT count(caeid) 
	                                                        FROM projovemurbano.cadastroestudante cae 
	                                                        WHERE 
	                                                        cae.nucid = $nucid
	                                                        AND cae.caestatus = 'A' )
	            FROM
	                projovemurbano.nucleo nuc2
	            WHERE
	                nuc2.nucid = $nucid";
	               
	            $vagasnucleo = $db->pegaUm( $sql );
		}
			
	    if( count($arrCaeid)> $vagasnucleo ){
	        echo "
	          <script>
	            alert('Você ultrapassou a lotação do núcleo.');
	             window.location.href = window.location.href;
	          </script>
	             ";
	        die();
	    }
	} 

	if($_SESSION['projovemurbano']['modalidade'] == 'M'){
	
		if($turid == ''){
			echo "
	          <script>
	            alert('Você não selecionou uma turma válida.');
	             window.location.href = window.location.href;
	          </script>
	             ";
			die();
		}
		
	    $sql=" SELECT
	                40 - ( SELECT count(caeid) 
	                        FROM projovemurbano.cadastroestudante cae 
	                        WHERE 
	                        cae.turid = $turid
	                        AND cae.caestatus = 'A' )";
		$vagasturma = $db->pegaUm( $sql );
	//     VER($sql,d);
	    if( count($arrCaeid)> $vagasturma ){
	        echo "
	          <script>
	            alert('Você ultrapassou a lotação da turma.');
	            window.location.href = window.location.href;
	          </script>
	             ";
	        die();
	    }
	}  
// 	$dadosalunoidade = array();
    foreach( $arrCaeid as $caeid ){
    
   	 	if($_SESSION['projovemurbano']['modalidade'] == 'M'){ $ppuid1 = $_REQUEST['ppuid'];  }else{$ppuid1 = $_REQUEST['ppuid_outros'];}
    	$idadeforapadaro = testaidade($caeid,$ppuid1);
    	if(empty($idadeforapadaro)){
	    	if($caeid !=''){	
		        $sql = "SELECT DISTINCT true FROM projovemurbano.transferencia tra
		                INNER JOIN projovemurbano.historico_transferencia htr ON htr.htrid = tra.htrid_ultimo
		                WHERE cad_caeid = $caeid AND htr.shtid_status NOT IN (3,4,5)";
		        
		        $temTranferenciaAtiva = $db->pegaUm( $sql );
	// 	        ver($temTranferenciaAtiva != 't',d);
	    	}ELSE{
	    		echo "<script>
	           			alert('Escolha um aluno válido!');
	            		window.location.href = window.location.href;
	          			</script>";
	    		die;
	    	}
	//     	die;
	        if( $temTranferenciaAtiva != 't' ){
	            if( $_SESSION['projovemurbano']['modalidade'] == 'M' ){
	            
	                $sqldadoaluno = "SELECT DISTINCT
			                               ppuid as anoorigem,
			                               to_char((SELECT max(hicdataacao) 
			                               FROM projovemurbano.historicocadastro 
			                               WHERE caeid = $caeid AND hicacao <> 'D'),'DD/MM/YYY')  as dataingresso,
			                               polid as polori,
			                               nucid as nucori,
			                               turid as turidorigem
			                        FROM 
			                               projovemurbano.cadastroestudante cae
			                        WHERE
			                               cae.caeid = $caeid
			                        GROUP BY
			                            caeid,
			                            ppuid,
			                            polid,
			                            nucid,
			                            turid ";
	            
	                $dadoaluno = $db->pegaLinha($sqldadoaluno);
	                extract($dadoaluno);
	                if( $turidorigem  == $turid){
	                	echo "<script>
           						alert('Não se pode transferir aluno(s) para a turma na qual se encontram');
          					</script>";
	                	die;
	                }
	                $sqlperiodo = " SELECT
					                	MAX(per.perperiodo)
					                FROM
					                	projovemurbano.periodocurso per
					                INNER JOIN projovemurbano.diario dia ON dia.perid = per.perid
					                WHERE
					                	dia.turid = $turid";
// 	                ver($sqlperiodo,d);
	                $periodo  = $db->pegaUm($sql);
	                $periodo = $periodo ? "'".$periodo."'" : 'NULL';
	                $periodo1 = ",$periodo";
	//                 ver($traperiodotransferido1,d);
	                $traperiodotransferido1 = ", traperiodotransferido";
	            }
	    		
	            $polori = $polori != '' ? $polori : 'NULL';
	            $polid  = $polid != '' ? $polid : 'NULL';
	            $turid  = $turid != '' ? $turid : 'NULL';
	            
	           if($_SESSION['projovemurbano']['modalidade'] == 'M'){
	                        
		            if($_SESSION['projovemurbano']['estuf']){
		            	$filtrolocalidade = "AND pju.estuf is not null";
		            }ElSE{
		            	$filtrolocalidade = "AND pju.muncod is not null";
		            }
		            
		            $sqlpjuidteste ="   SELECT DISTINCT
							            	pjuid
							            FROM projovemurbano.nucleo nuc
							            INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
							            INNER JOIN territorios.municipio tmun ON tmun.muncod = mun.muncod
							            INNER JOIN projovemurbano.projovemurbano pju ON (pju.muncod = tmun.muncod OR pju.estuf =tmun.estuf)  AND pju.ppuid = $ppuid1
							            WHERE
							            	nucid = $nucid
							            AND nuc.ppuid = $ppuid1
							            $filtrolocalidade
		            ";
		            $pjuidteste = $db->pegaUm($sqlpjuidteste);
		            
	           }else{
	//                 dbg($polid != 'NULL');
	//                 die;
	                if($polid != 'NULL' && $polid != ''){
	                	/*Estadual com polo*/
	                	$sqlpjuidteste = "  SELECT DISTINCT
						                		pju.pjuid
						                	FROM projovemurbano.nucleo nuc
						                	INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
						                	INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
						                	INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
						                	INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
						                	INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
						                	WHERE
						                	mun.munstatus='A'
						                	AND plm.pmustatus='A'
						                	AND pol.polstatus='A'
						                	AND nuc.ppuid = {$ppuid1}
						                	AND pju.estuf <>''
						                	AND pol.polid = {$polid}
						                	";
	                	$pjuidteste = $db->pegaUM($sqlpjuidteste);
	                	if(!$pjuidteste){
	                	/*Municipal com polo*/
		                	$sqlpjuidteste = "  SELECT DISTINCT
							                		pju.pjuid
							                	FROM projovemurbano.nucleo nuc
							                	INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
							                	INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
							                	INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
						                		INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
						                		INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
						                		WHERE
						                			mun.munstatus='A'
						                		AND plm.pmustatus='A'
						                		AND pol.polstatus='A'
						                		AND nuc.ppuid = {$ppuid1}
						                		AND pju.muncod <> ''
						                		AND pol.polid = {$polid}
							                		";
							$pjuidteste = $db->pegaUM($sqlpjuidteste);
	                	}
					}ELSE{
						
	                	/*Estadual sem polo*/
	                		$sqlpjuidteste = "  SELECT DISTINCT
							                		pju.pjuid
							                	FROM
							                	projovemurbano.nucleo nuc
							                	INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
							                	INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid
							                	INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
							                	WHERE
							                		mun.munstatus='A'
						                		AND plm.pmustatus='A'
						                		AND nuc.ppuid = {$ppuid1}
							                	AND pju.estuf <> ''
							                	AND nuc.nucid = {$nucid}
						                	";
	                		$pjuidteste = $db->pegaUM($sqlpjuidteste);
						/*Municpal sem polo*/
							if(!$pjuidteste){
								$sqlpjuidteste = "  SELECT DISTINCT
														pju.pjuid
													FROM
														projovemurbano.nucleo nuc
													INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
													INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid
													INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
													WHERE
														plm.pmustatus='A'
													AND nuc.ppuid = {$ppuid1}
													AND pju.muncod <> ''
													AND nuc.nucid = {$nucid}
												";
								$pjuidteste = $db->pegaUM($sqlpjuidteste);
	               			}
						}
	           }
	            $sql12 = "SELECT true FROM projovemurbano.metasdoprograma WHERE pjuid = $pjuidteste AND tpmid = 3";
	            $temAjuste = $db->pegaUm($sql12);
	                
	            if($temAjuste == 't'){
	            	$sql="
	            	SELECT
	            		true
	            	FROM
	            	(
		            	SELECT
		            		pjuid,
		            		mtpvalor as qtd
		            	FROM projovemurbano.metasdoprograma
		            	WHERE
		            		tpmid = 3
		            	AND ppuid = $ppuid1
		            	AND pjuid = $pjuidteste
	            	) as foo
	            	WHERE
	            		foo.qtd <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.pjuid = foo.pjuid AND cae.caestatus = 'A'  )
	            	AND foo.pjuid = $pjuidteste ";
	            	$metaatingida = $db->pegaUm($sql);
	            }else{
		            $sql = "SELECT
		            	true
		            	FROM
		            		(
			            		SELECT
			            			pu.pjuid,
			            			COALESCE(sa.suametaajustada,cm.cmemeta) as qtd
			            		FROM
			            		projovemurbano.projovemurbano pu
			            		LEFT JOIN territorios.estado est ON est.estuf = pu.estuf
			            		LEFT JOIN projovemurbano.cargameta cm ON cm.cmecodibge = pu.muncod::numeric OR cm.cmecodibge = est.estcod::numeric
			            		LEFT JOIN projovemurbano.sugestaoampliacao sa ON sa.pjuid = pu.pjuid
			            		WHERE
			            			cm.cmemeta IS NOT NULL
			            		AND pu.ppuid = $ppuid1
			            		AND cm.ppuid = $ppuid1
			            		AND pu.pjuid = $pjuidteste
		            		) as foo
		            		WHERE
		            			foo.qtd <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.pjuid = foo.pjuid AND cae.caestatus = 'A'  )
		            		AND foo.pjuid = $pjuidteste";
					$metaatingida = $db->pegaUm($sql);
				}
		         if( $metaatingida  == 't'){
			    	echo
			         	"<script>
				    			alert('Meta atingida.');
				    			window.location.href = window.location.href;
				    		</script>";
			    	die;
		    	}
// 	           ver(empty($turidorigem),d);
	           if(empty($turidorigem)){
	           	$turidorigem = 'NULL';
	           }
	           if(empty($nucori)){
	           	$nucori = 'NULL';
	           }
	           if(empty($polori)){
	           	$polori = 'NULL';
	           }
	        	$sql = "INSERT INTO projovemurbano.transferencia(
	        		            cad_caeid,
	        		            pjuid_origem,
	        		            turid_origem,
	                            nucid_origem,
	                            polid_origem,  
	                            ppuid_origem,
	        		            turid_destino,
	                            nucid_destino,
	                            polid_destino,
	        		            usucpfresponsavelorigem,  
	        		            tranroordemtransferencia, 
	        		            tradataingressoorigem, 
	        		            ppuid_destino
	        					$traperiodotransferido1)
	        		    VALUES (
	        		    		$caeid,
	        		    		{$_SESSION['projovemurbano']['pjuid']},
	        			    	".($turidorigem!=''?"$turidorigem":NULL).",
	                            ".($nucori!=''?"$nucori":NULL).",
	                            ".($polori!=''?"$polori":NULL).",
	                            $anoorigem, 
	        			    	$turid,
	                            $nucid,
	                            $polid,
	        			    	'{$_SESSION['usucpf']}', 
	        		    		(SELECT count(traid) FROM projovemurbano.transferencia WHERE cad_caeid = $caeid)+1, 
	        		            ".($dataingresso!=''?"'".str_replace(Array('/'),'-',$dataingresso)."'":'NOW()').", 
	        		            $ppuid1
	        					$periodo1)
	                    RETURNING
	                            traid";
// 	             ver($sql,d);
	            $traid = $db->pegaUm($sql);
	            
	            $justificativa = $justificativa != '' ? "'".$justificativa."'" : 'NULL';
	            
	            $sql = "INSERT INTO projovemurbano.historico_transferencia(
	                            traid,
	                            shtid_status, 
	                            usucpf_fezacao, 
	                            htrdatahoraacao, 
	                            justificativa)
	                    VALUES ($traid, 1,
	                            '{$_SESSION['usucpf']}',
	                            now(), 
	                            $justificativa)
	                    RETURNING 
	                            htrid";
	            $htrid  = $db->pegaUm($sql);
	            
	            unset($justificativa);
	            $sqlhistorico.= "UPDATE projovemurbano.transferencia SET
						               	htrid_ultimo = $htrid
						            WHERE
							                traid = $traid;";
		            
	            if( $arqid != '' ){
	                $sqlarquivo.= "INSERT INTO projovemurbano.arquivo_tranferencia(arqid, traid)
	                        		VALUES($arqid, $traid);";
	            }else{
	                if($_FILES["arquivo"]['name']!=''){  
		                require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		            
		                $campos = array("traid" => $traid);
		                
		                $file = new FilesSimec("arquivo_tranferencia", $campos, 'projovemurbano');
		                
		                $file->setUpload("Documento de Pedido de Transferência CPF: $caecpf");   
		                
		                $sql = "SELECT arqid FROM projovemurbano.arquivo_tranferencia WHERE traid = $traid";
		                $arqid = $db->pegaUm($sql);
	                }
	            }
	        }
	    }else{
	    	$dadosalunoidade .= '\n'.'CPF:'.$idadeforapadaro['cpf'].'-NOME:'.$idadeforapadaro['nome'];
	    }
    }
    if($sqlhistorico!=''){
    	$db->executar($sqlhistorico);
    }
    if($sqlarquivo!=''){
		$db->executar($sqlarquivo);
    }
	$db->commit ();
	if(!empty($dadosalunoidade)){
		echo "<script>
           alert('Os seguintes alunos não puderam ser transferidos por não se enquadrar nos requisitos de idade 18 a 29 anos $dadosalunoidade');
            window.location.href = window.location.href;
          </script>";
	}else{
    echo "<script>
            window.location.href = window.location.href;
          </script>";
	}
}

function enviar_aluno_transferir(){
    
    global $db;
    
    extract($_POST);
    
//    ver($_POST);
//    die;
    
    if( $traid[0] != '' ){
        
        foreach( $traid as $id ){
            
            $sql = "SELECT
                        polid_destino as polid,
                        nucid_destino as nucid,
                        turid_destino as turid,
                        ppuid_destino as ppuid,
                        turid_origem,
                        cad_caeid as caeid
                    FROM
                        projovemurbano.transferencia
                    WHERE
                        traid = $id";
                      
            $dados = $db->pegaLinha($sql);
           
            extract($dados);
            
            if( $polid == '' ){
                $polid = "NULL";
            }
//             ver($ppuid,d);
            if($_SESSION['projovemurbano']['modalidade'] == 'M'){
                $shtid_status = 3;
            }else{
                $shtid_status = 2;
            }
            
            $sql = "INSERT INTO projovemurbano.historico_transferencia(
                            traid,
                            shtid_status, 
                            usucpf_fezacao, 
                            htrdatahoraacao, 
                            justificativa)
                    VALUES ($id, $shtid_status,
                            '{$_SESSION['usucpf']}',
                            now(), 
                            'direta')
                    RETURNING 
                            htrid";
            
            $htrid = $db->pegaUm($sql); 
            
            if($_SESSION['projovemurbano']['modalidade'] == 'M'){
            	if($polid != 'NULL'){ 
            	$sql = "
            			SELECT DISTINCT pjuid
							FROM projovemurbano.polo pol
						INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid
						WHERE
							polid = $polid
            			"; 
            	}else{
            		IF($_SESSION['projovemurbano']['estuf']){
            			$filtrolocalidade = "AND pju.estuf is not null";
            		}ElSE{
            			$filtrolocalidade = "AND pju.muncod is not null";
            		}
            		$sql="SELECT DISTINCT pjuid 
            			FROM projovemurbano.nucleo nuc
            			INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
						INNER JOIN territorios.municipio tmun ON tmun.muncod = mun.muncod
						INNER JOIN projovemurbano.projovemurbano pju ON (pju.muncod = tmun.muncod OR pju.estuf =tmun.estuf)  AND pju.ppuid = $ppuid
						WHERE
						nucid = $nucid
						AND nuc.ppuid = $ppuid
						$filtrolocalidade
						";
            		
            	}
//             	ver($sql,d);
            	$pjuidy = $db->pegaUm($sql);
                $sqlmudacadastro.= "UPDATE projovemurbano.cadastroestudante SET 
			                            polid = $polid,  
			                            nucid = $nucid,
			                            turid = $turid,
			                            ppuid = $ppuid,
			                            pjuid = $pjuidy,
			                            caestatus = 'A'
			                        WHERE 
			                            caeid = $caeid ;";
//                 ver($sql,d);
                
//                 Atualiza histórico Aluno
				if($turid_origem!=''){
	                $sql = "SELECT max(notaciclo1) as nota1, max(notaciclo2) as nota2, max(notaciclo3) as nota3 
				            FROM projovemurbano.notasporciclo 
				            WHERE 
				                turid = $turid_origem
				    		AND caeid = $caeid ";
					// ver($sql,d);
					$dados = $db->pegaLinha ( $sql );
					
					extract ( $dados );
					
					if ($nota1 == '') {
						$notaciclo1 = "NULL";
					} else {
						$notaciclo1 = $nota1;
					}
					
					if ($nota2 == '') {
						$notaciclo2 = "NULL";
					} else {
						$notaciclo2 = $nota2;
					}
					
					if ($nota3 == '') {
						$notaciclo3 = "NULL";
					} else {
						$notaciclo3 = $nota3;
					}
					$sqlverificaturma = "SELECT 
											TRUE
										 FROM
											projovemurbano.notasporciclo
										 WHERE 
										 	turid = $turid
				    					 AND caeid = $caeid
				    					 AND npc_status = 'I'";
					$verificaturma = $db->pegaUm ( $sqlverificaturma );
					
					if ($sqlverificaturma) {
						$sqlorigem.= "UPDATE projovemurbano.notasporciclo  SET 
											npc_status = 'A',																		
											notaciclo1 = $notaciclo1, 
											notaciclo2 = $notaciclo2, 
											notaciclo3 = $notaciclo3
										WHERE
											turid = $turid_origem
										AND caeid = $caeid
										AND npc_status = 'I';";
						
	// 					$db->executar ( $sql );
					} else {
						$sqldestino.= "INSERT INTO projovemurbano.notasporciclo (caeid, turid, notaciclo1, notaciclo2, notaciclo3, ppuid)
					                 VALUES ($caeid, $turid, $notaciclo1, $notaciclo2, $notaciclo3, $ppuid);
					                 ";
						// ver($sql,d);
						
						$sqldestino2.= "UPDATE projovemurbano.notasporciclo  SET npc_status = 'I'
					            WHERE 
					                turid = $turid_origem
					            AND caeid = $caeid
					    		AND npc_status = 'A';";
						
					}
					
					$sqlfrequenciainativa.= "UPDATE projovemurbano.frequenciaestudante SET
							                 frqstatus = 'I'
							            WHERE
							                 caeid = $caeid
							                 AND frqid IN (SELECT DISTINCT
							                             frqid
							                         FROM
							                             projovemurbano.frequenciaestudante frq
							                         INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
							                         INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
							                         WHERE
							                             caeid = $caeid
							                             AND dia.perid IN (SELECT perid FROM projovemurbano.diario WHERE turid = $turid_origem));";
				// ver($sql,d);
			}
//                 Fim Atualiza histórico Aluno
// 					Atualiza trabalho frequencia
	            $sql = "SELECT DISTINCT true 
						FROM projovemurbano.transferencia 
				 		WHERE 
				 			 cad_caeid = $caeid";
		
				$temtrans = $db->pegaUm ( $sql );
	
			if ($temtrans == 't') {
				
				$sql1 = "SELECT max(htrid) 
					FROM projovemurbano.transferencia tra 
					INNER JOIN projovemurbano.historico_transferencia htr ON tra.traid = htr.traid
					WHERE cad_caeid = $caeid";
				
				$htrid = $db->pegaUm ( $sql1 );
				
				$htridx = "AND htr.htrid = $htrid";
			}
			$sqldadostrans = "SELECT 
		    					turid_origem, 
		    					turid_destino, 
		    					ppuid_origem, 
		    					ppuid_destino 
							FROM projovemurbano.transferencia tra 
							INNER JOIN projovemurbano.historico_transferencia htr ON tra.traid = htr.traid 
		    				WHERE 
		    					cad_caeid = $caeid $htridx";
			$dadostrasns = $db->pegaLinha ( $sqldadostrans );
			if($dadostrasns['turid_origem']){
				$sqlperiodoorigem = "SELECT 
			    						max(perperiodo) 
			    					FROM projovemurbano.periodocurso 
			    					WHERE 
			    						ppuid = {$dadostrasns['ppuid_origem']}
									AND perid = (SELECT 
			    									max(perid) 
			    								FROM projovemurbano.diario 
			    								WHERE turid = {$dadostrasns['turid_origem']})";
				$periodoorigem = $db->pegaUm ( $sqlperiodoorigem );
			}
			$sqlperiododestino = "SELECT
		    						 max(perperiodo) 
								FROM projovemurbano.periodocurso 
								WHERE 
									ppuid = {$dadostrasns['ppuid_destino']} 
								AND perid = (SELECT 
												max(perid) 
											FROM projovemurbano.diario 
											WHERE turid = {$dadostrasns['turid_destino']})";
			// ver($sqlperiododestino,d);
			$periododestino = $db->pegaUm ( $sqlperiododestino );
			// ver($dadostrasns,d);
			
			$sqltestaturma = "SELECT DISTINCT
									frq.difid
							  FROM
									projovemurbano.frequenciaestudante frq
							  INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
							  INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
						  	  WHERE
							  	dia.turid = {$dadostrasns['turid_destino']}
		    				  AND frq.caeid = $caeid
		    				  AND frq.frqstatus = 'I'
		    				  order by
		    				  	difid";
			$testaturma = $db->pegarColunas( $sqltestaturma );
			
			$sqlperiodoscursados = "
									SELECT DISTINCT
										dia.perid
									FROM
										projovemurbano.frequenciaestudante frq
								  INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
								  INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
							  	  WHERE
								  	dia.turid = {$dadostrasns['turid_destino']}
			    				  AND frq.caeid = $caeid
			    				  AND frq.frqstatus = 'I'
			    				  order by
				    				  perid";
    		$periodoscursados = $db->pegarColunas( $sqlperiodoscursados );
			// ver($testaturma,d);
			if($dadostrasns['turid_origem']){
				if (is_array ( $testaturma ) && ! empty ( $testaturma )) {
					$sqlfrequenciavolta.= "UPDATE projovemurbano.frequenciaestudante  SET 
												frqstatus = 'A'
											WHERE
												caeid = $caeid
											AND frqstatus = 'I'
							    			AND difid in ($testaturma);";
					$sqlinserefrequencianova.= "
											INSERT INTO projovemurbano.frequenciaestudante(caeid, difid, frqqtdpresenca, frqtrabalho)
												SELECT
													caeid,
													(
													SELECT DISTINCT
														difid
													FROM
														projovemurbano.diariofrequencia dif2
													INNER JOIN projovemurbano.diario dia2 ON dia2.diaid = dif2.diaid
													INNER JOIN projovemurbano.periodocurso per2 ON per2.perid = dia2.perid
													WHERE
														dia2.diaid in(
																	SELECT
																		diaid
																	FROM
																		projovemurbano.diario
																	WHERE
																		turid = {$dadostrasns['turid_destino']}
																	AND perid NOT IN($periodoscursados)
																	)
																	AND dif2.grdid = dif.grdid
																	AND per2.perperiodo = per.perperiodo
													) as difid,
													frqqtdpresenca,
													frqtrabalho
											FROM
												projovemurbano.frequenciaestudante fre
											INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = fre.difid
											INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
											INNER JOIN projovemurbano.periodocurso per ON per.perid = dia.perid
											WHERE
												caeid = $caeid
											AND turid = {$dadostrasns['turid_origem']}
											AND frqstatus = 'I'
											AND dia.perid NOT IN($periodoscursados)
											ORDER BY
											3
											;";
					
				} else {
					$sqlinserefrequencianova.= "
							INSERT INTO projovemurbano.frequenciaestudante(caeid, difid, frqqtdpresenca, frqtrabalho)
								SELECT 
								caeid,
								(
									SELECT DISTINCT
										difid
									FROM 
										projovemurbano.diariofrequencia dif2 
									INNER JOIN projovemurbano.diario dia2 ON dia2.diaid = dif2.diaid
									INNER JOIN projovemurbano.periodocurso per2 ON per2.perid = dia2.perid
									WHERE
										dia2.diaid in(        
														SELECT 
															diaid
														FROM 
															projovemurbano.diario
														WHERE 
															turid = {$dadostrasns['turid_destino']}
														AND perid BETWEEN (SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_destino']} AND perperiodo = 1)
														AND(SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_destino']} AND perperiodo = ($periododestino - 1))
													)
									AND dif2.grdid = dif.grdid
									AND per2.perperiodo = per.perperiodo
								) as difid,
								frqqtdpresenca,
								frqtrabalho
							FROM 
								projovemurbano.frequenciaestudante fre
							INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = fre.difid
							INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
							INNER JOIN projovemurbano.periodocurso per ON per.perid = dia.perid
							WHERE
								caeid = $caeid
							AND turid = {$dadostrasns['turid_origem']}
							AND frqstatus = 'I'
							AND dia.perid BETWEEN (SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_origem']} AND perperiodo = 1)
							AND(SELECT perid FROM projovemurbano.periodocurso WHERE ppuid =  {$dadostrasns['ppuid_origem']} AND perperiodo = ($periododestino - 1))
							ORDER BY
								       3
				            ;";
					// ver($sql12,d);
				}
			}
//                 Fim Atualiza trabalho frequencia
				$sqlhistorico.= "UPDATE projovemurbano.transferencia SET
									htrid_ultimo = $htrid
								WHERE
									traid = $id;";
            }else{
                
               	$sqlhistorico.= "UPDATE projovemurbano.transferencia SET
									htrid_ultimo = $htrid
								WHERE
									traid = $id;";
                if( $polid != 'NULL' ){
                    
                    $sql = "SELECT DISTINCT
                                usu.usuemail
                            FROM
                                projovemurbano.coordenadorresponsavel coo
                            INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid  = coo.pjuid
                            INNER JOIN projovemurbano.polo      pol ON pol.pmuid  = pmu.pmuid
                            INNER JOIN seguranca.usuario        usu ON usu.usucpf = coo.corcpf
                            WHERE
                                pol.polid = $polid";
                }else{
                    $sql = "SELECT DISTINCT
                                usu.usuemail
                            FROM
                                projovemurbano.coordenadorresponsavel coo
                            INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid  = coo.pjuid
                            INNER JOIN projovemurbano.municipio mun ON mun.pmuid  = pmu.pmuid
                            INNER JOIN projovemurbano.nucleo    nuc ON nuc.munid  = mun.munid
                            INNER JOIN seguranca.usuario        usu ON usu.usucpf = coo.corcpf
                            WHERE
                                nuc.nucid = $nucid";
                }
//                ver($sql,d);
                $email_resp_destino = $db->pegaUm($sql);
                
                $sql = "SELECT caecpf||' - '||caenome FROM projovemurbano.cadastroestudante WHERE caeid = $caeid";
                
                $alunos = $db->carregarColuna($sql);
                
                $assunto = "PROJOVEM URBANO - Pedido de transferência sob sua reponsabilidade.";
                
                $conteudo = "Foi feito pedido de tranferência do seguinte aluno para nucleo de sua reponsabilidade:
                                ".implode('; <br>',$alunos).".";
//                ver($conteudo,$assunto,$alunos,$email_resp_destino,d);
//                enviar_email( $_SESSION['email_sistema'], $email_resp_destino, $assunto, $conteudo );
            }
        }
            if($sqlmudacadastro!=''){
            	$db->executar($sqlmudacadastro);
            }
            if($sqlorigem!=''){
            	$db->executar($sqlorigem);
            }
            if($sqldestino!=''){
            	$db->executar($sqldestino);
            }
            if($sqldestino2!=''){
            	$db->executar($sqldestino2);
            }
            if($sqlfrequenciainativa!=''){
            	$db->executar($sqlfrequenciainativa);
            }
            if($sqlfrequenciavolta!=''){
            	$db->executar($sqlfrequenciavolta);
            }
            if($sqlinserefrequencianova!=''){
            	$db->executar($sqlinserefrequencianova);
            }
            if($sqlhistorico!=''){
            	$db->executar($sqlhistorico);
            }
            $db->commit ();
        echo "<script>
                alert('Transferência concluída.');
                window.location.href = window.location.href;
            </script>";
    }
}

function receber_aluno_tranferir(){

    global $db;
    
    extract($_POST);
	
    if( testaQtdAlunoMetaProjovem( $_SESSION['projovemurbano']['pjuid'] ) ){
    	die("<script>
    			alert('Meta atingida.');
    			window.location.href = window.location.href;
    			</script>");
    } 
    
	    if(is_array($turid) ){
		    foreach( $turid as $idturma ){
		    	
			    $sql=" SELECT
					    	40 - ( SELECT count(caeid)
					   FROM projovemurbano.cadastroestudante cae
					   WHERE
					   		cae.turid = {$idturma}
					   AND  cae.caestatus = 'A' )";
			    $vagasturma = $db->pegaUm( $sql );
				
			    if( count($idturma)> $vagasturma ){
			    echo "
			    <script>
			    alert('Você ultrapassou a lotação da turma.');
			            window.location.href = window.location.href;
			          </script>
			             ";
			        die();
			    }
		    }
	    }
	    if( $traid[0] != '' ){
	        $arrCaeid = Array();
	        foreach( $traid as $idtransferencia ){
	        	$turiddestino = $turid[$idtransferencia];
	        	
	        	if(!$turiddestino){
	        		echo "<script>
               				alert('Problema no envio de dados.');
                			window.location.href = window.location.href;
            			  </script>";
        			die();
	        	}
	        	$sqlperiodo = " SELECT
	        						MAX(per.perperiodo)
	        					FROM
	        						projovemurbano.periodocurso per
	        					INNER JOIN projovemurbano.diario dia ON dia.perid = per.perid
	        					WHERE
	        						dia.turid = {$idturma}";
	        	$periodoturma  = $db->pegaUm($sqlperiodo);
	        	
	        	$periodoturma = $periodoturma ? "'".$periodoturma."'" : 'NULL';
	        	
	            $sqlturmadestino.= "UPDATE projovemurbano.transferencia SET
	                        turid_destino = {$turiddestino},
	                        usucpfresponsaveldestino = '{$_SESSION['usucpf']}',
	                        traperiodotransferido = {$periodoturma} 
	                    WHERE
	                        traid = $idtransferencia;";
	                        
	            $sql= "INSERT INTO projovemurbano.historico_transferencia(
	                            traid,
	                            shtid_status, 
	                            usucpf_fezacao, 
	                            htrdatahoraacao, 
	                            justificativa)
	                    VALUES ($idtransferencia, 3,
	                            '{$_SESSION['usucpf']}',
	                            now(), 
	                            'indireta')
	                    RETURNING 
	                            htrid";
	//             ver($sql);
	//             die;
	            $htrid = $db->pegaUm($sql); 
// 	            Atualiza Status transferência

            	$sqlstatustrans.= "UPDATE projovemurbano.transferencia SET
                            htrid_ultimo = $htrid
                        WHERE
                            traid = $idtransferencia;";
	            
// 	            Fim Atualiza Status
	            $sql = "SELECT
	                        polid_destino as polid,
	                        nucid_destino as nucid,
	                        polid_origem as polo_origem,
	                        nucid_origem as nucleo_origem,
	                        ppuid_destino as ppuid,
	                        turid_origem,
	                        cad_caeid as caeid
	                    FROM
	                        projovemurbano.transferencia
	                    WHERE
	                        traid = $idtransferencia";
	            $dados = $db->pegaLinha($sql);
// 	            ver($dados,d);
	            extract($dados);
	                        
	            $polid = $polid ? $polid : 'NULL';

	            $sqlalteracadastro.= "UPDATE projovemurbano.cadastroestudante SET 
	                            polid = $polid,  
	                            nucid = $nucid,
	                            turid = {$turiddestino},
	                            ppuid = $ppuid,
	                            pjuid = {$_SESSION['projovemurbano']['pjuid']},
	                            caestatus = 'A'
	                        WHERE 
	                            caeid = $caeid;";
// 	            ver($polid,$nucid,$turiddestino,$ppuid,$_SESSION['projovemurbano']['pjuid'],d);    
	            $arrCaeid[] = $caeid;
	            
	//             ver($turid,d);
// 	            Atualiza histórico Aluno

                $sql = "SELECT max(notaciclo1) as nota1, max(notaciclo2) as nota2, max(notaciclo3) as nota3 
			            FROM projovemurbano.notasporciclo 
			            WHERE 
			                turid = $turid_origem
			    		AND caeid = $caeid ";
				// ver($sql,d);
				$dados = $db->pegaLinha ( $sql );
				
				extract ( $dados );
				
				if ($nota1 == '') {
					$notaciclo1 = "NULL";
				} else {
					$notaciclo1 = $nota1;
				}
				
				if ($nota2 == '') {
					$notaciclo2 = "NULL";
				} else {
					$notaciclo2 = $nota2;
				}
				
				if ($nota3 == '') {
					$notaciclo3 = "NULL";
				} else {
					$notaciclo3 = $nota3;
				}
				$sqlverificaturma = "SELECT 
										TRUE
									 FROM
										projovemurbano.notasporciclo
									 WHERE 
									 	turid = $turid_origem
			    					 AND caeid = $caeid
			    					 AND npc_status = 'I'";
				$verificaturma = $db->pegaUm ( $sqlverificaturma );
				
				if ($sqlverificaturma) {
					$sqlorigem.= "UPDATE projovemurbano.notasporciclo  SET npc_status = 'A'
							WHERE
								turid = $turid_origem
							AND caeid = $caeid
							AND npc_status = 'I';";
					
					$db->executar ( $sql );
				} else {
					$sqldestino.= "INSERT INTO projovemurbano.notasporciclo (caeid, turid, notaciclo1, notaciclo2, notaciclo3, ppuid)
				                 VALUES ($caeid, $turid, $notaciclo1, $notaciclo2, $notaciclo3, $ppuid);
				                 ";
					// ver($sql,d);
					
					$sqldestino2.= "UPDATE projovemurbano.notasporciclo  SET npc_status = 'I'
				            WHERE 
				                turid = $turid_origem
				            AND caeid = $caeid
				    		AND npc_status = 'A';";
					
				}
				
				$sqlfrequenciainativa.= "UPDATE projovemurbano.frequenciaestudante SET
						                 frqstatus = 'I'
						            WHERE
						                 caeid = $caeid
						                 AND frqid IN (SELECT DISTINCT
						                             frqid
						                         FROM
						                             projovemurbano.frequenciaestudante frq
						                         INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
						                         INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
						                         WHERE
						                             caeid = $caeid
						                             AND dia.perid IN (SELECT perid FROM projovemurbano.diario WHERE turid = $turid_origem));";
				// ver($sql,d);
				
//                 Fim Atualiza histórico Aluno
// 					Atualiza trabalho frequencia
	            $sql = "SELECT DISTINCT true 
						FROM projovemurbano.transferencia 
				 		WHERE 
				 			 cad_caeid = $caeid";
		
				$temtrans = $db->pegaUm ( $sql );
	
			if ($temtrans == 't') {
				
				$sql1 = "SELECT max(htrid) 
					FROM projovemurbano.transferencia tra 
					INNER JOIN projovemurbano.historico_transferencia htr ON tra.traid = htr.traid
					WHERE cad_caeid = $caeid";
				
				$htrid = $db->pegaUm ( $sql1 );
				
				$htridx = "AND htr.htrid = $htrid";
			}
			$sqldadostrans = "SELECT 
		    					turid_origem, 
		    					turid_destino, 
		    					ppuid_origem, 
		    					ppuid_destino 
							FROM projovemurbano.transferencia tra 
							INNER JOIN projovemurbano.historico_transferencia htr ON tra.traid = htr.traid 
		    				WHERE 
		    					cad_caeid = $caeid $htridx";
			$dadostrasns = $db->pegaLinha ($sqldadostrans);
// 			if(!$dadostrasns['turid_origem']){
// 			ver($dadostrasns,d);
// 			}
			$sqlperiodoorigem = "SELECT 
		    						max(perperiodo) 
		    					FROM projovemurbano.periodocurso 
		    					WHERE 
		    						ppuid = {$dadostrasns['ppuid_origem']}
								AND perid = (SELECT 
		    									max(perid) 
		    								FROM projovemurbano.diario 
		    								WHERE turid = {$turid_origem})";
			$periodoorigem = $db->pegaUm ( $sqlperiodoorigem );
			
			$sqlperiododestino = "SELECT
		    						 max(perperiodo) 
								FROM projovemurbano.periodocurso 
								WHERE 
									ppuid = {$dadostrasns['ppuid_destino']} 
								AND perid = (SELECT 
												max(perid) 
											FROM projovemurbano.diario 
											WHERE turid = {$turiddestino})";
			// ver($sqlperiododestino,d);
			$periododestino = $db->pegaUm ( $sqlperiododestino );
			// ver($dadostrasns,d);
			
			$sqltestaturma = "SELECT
									difid
							  FROM
									projovemurbano.frequenciaestudante frq
							  INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
							  INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
						  	  WHERE
							  	dia.turid = {$turiddestino}
		    				  AND frq.caeid = $caeid
		    				  AND frq.frqstatus = 'I'";
			$testaturma = $db->pegarColunas ( $sqltestaturma );
			// ver($testaturma,d);
			if (is_array ( $testaturma ) && ! empty ( $testaturma )) {
				$sqlfrequenciavolta.= "UPDATE projovemurbano.frequenciaestudante  SET frqstatus = 'A'
									WHERE
										caeid = $caeid
									AND frqstatus = 'I'
					    			AND difid in ($testaturma);";
				
			} else {
				if(!$periododestino){
					$periododestino = 1;
				}
				$sqlinserefrequencianova.= "INSERT INTO projovemurbano.frequenciaestudante(caeid, difid, frqqtdpresenca, frqtrabalho)
							SELECT 
							caeid,
							(
							SELECT DISTINCT
								difid
							FROM 
								projovemurbano.diariofrequencia dif2 
							INNER JOIN projovemurbano.diario dia2 ON dia2.diaid = dif2.diaid
							INNER JOIN projovemurbano.periodocurso per2 ON per2.perid = dia2.perid
							WHERE
								dia2.diaid in(        
												SELECT 
													diaid
												FROM 
													projovemurbano.diario
												WHERE 
													turid = {$turiddestino}
												AND perid BETWEEN (SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_destino']} AND perperiodo = 1)
												AND(SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_destino']} AND perperiodo = ($periododestino - 1))
											)
							AND dif2.grdid = dif.grdid
							AND per2.perperiodo = per.perperiodo
							) as difid,
							frqqtdpresenca,
							frqtrabalho
							FROM 
								projovemurbano.frequenciaestudante fre
							INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = fre.difid
							INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
							INNER JOIN projovemurbano.periodocurso per ON per.perid = dia.perid
							WHERE
								caeid = $caeid
							AND turid = {$dadostrasns['turid_origem']}
							AND frqstatus = 'I'
							AND dia.perid BETWEEN (SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_origem']} AND perperiodo = 1)
							AND(SELECT perid FROM projovemurbano.periodocurso WHERE ppuid =  {$dadostrasns['ppuid_origem']} AND perperiodo = ($periododestino - 1))
							ORDER BY
							       3
			            ;";
				// ver($sql12,d);
			}
//                 Fim Atualiza trabalho frequencia
	            if( $polo_origem != '' ){
	                
	                $sql = "SELECT DISTINCT
	                            usu.usuemail
	                        FROM
	                            projovemurbano.coordenadorresponsavel coo
	                        INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid  = coo.pjuid
	                        INNER JOIN projovemurbano.polo      pol ON pol.pmuid  = pmu.pmuid
	                        INNER JOIN seguranca.usuario        usu ON usu.usucpf = coo.corcpf
	                        WHERE
	                            pol.polid = $polo_origem";
	            }else{
	                $sql = "SELECT DISTINCT
	                            usu.usuemail
	                        FROM
	                            projovemurbano.coordenadorresponsavel coo
	                        INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid  = coo.pjuid
	                        INNER JOIN projovemurbano.municipio mun ON mun.pmuid  = pmu.pmuid
	                        INNER JOIN projovemurbano.nucleo    nuc ON nuc.munid  = mun.munid
	                        INNER JOIN seguranca.usuario        usu ON usu.usucpf = coo.corcpf
	                        WHERE
	                            nuc.nucid = $nucleo_origem";
	            }
	            
	            $email_resp_destino = $db->pegaUm($sql);
	            
	            $sql = "SELECT caecpf||' - '||caenome FROM projovemurbano.cadastroestudante WHERE caeid = $caeid";
	            
	            $alunos = $db->carregarColuna($sql);
	            
	            $assunto = "PROJOVEM URABNO - Pedido de transferência aceito.";
	            
	            $conteudo = "O seguinte aluno teve a transferência aceita:
	                            ".implode('; <br>',$alunos).".";
	//            ver($conteudo,$assunto,$alunos,$email_resp_destino,d);
	//            enviar_email( $_SESSION['email_sistema'], $email_resp_destino, $assunto, $conteudo );
	        }
	    }
// 	    ver(
// $sqlturmadestino,
// $sqlstatustrans,
// $sqlalteracadastro,
// $sqlorigem,
// $sqldestino,
// $sqldestino2,
// $sqlfrequenciainativa,
// $sqlfrequenciavolta,
// $sqlinserefrequencianova,
// 	        d);
 		if($sqlturmadestino!=''){
            $db->executar($sqlturmadestino);
        }
 		if($sqlstatustrans!=''){
            $db->executar($sqlstatustrans);
        }
 		if($sqlalteracadastro!=''){
            $db->executar($sqlalteracadastro);
        }
 		if($sqlorigem!=''){
            $db->executar($sqlorigem);
        }
 		if($sqldestino!=''){
            $db->executar($sqldestino);
        }
 		if($sqldestino2!=''){
            $db->executar($sqldestino2);
        }
 		if($sqlfrequenciainativa!=''){
            $db->executar($sqlfrequenciainativa);
        }
 		if($sqlfrequenciavolta!=''){
            $db->executar($sqlfrequenciavolta);
        }
 		if($sqlinserefrequencianova!=''){
            $db->executar($sqlinserefrequencianova);
        }
        
        $db->commit ();
	    echo "<script>
	                alert('Transferência concluída.');
	                window.location.href = window.location.href;
	            </script>";
}

function receber_aluno_cancelar(){

    global $db;
    
    extract($_POST);
//    ver($_POST,d);
    if( $traid[0] != '' ){
        foreach( $traid as $traid_ ){
            
            $sql = "UPDATE projovemurbano.transferencia SET
                        usucpfresponsaveldestino = '{$_SESSION['usucpf']}'
                    WHERE
                        traid = $traid_;
                    INSERT INTO projovemurbano.historico_transferencia(
                            traid,
                            shtid_status, 
                            usucpf_fezacao, 
                            htrdatahoraacao, 
                            justificativa)
                    VALUES ($traid_, 4,
                            '{$_SESSION['usucpf']}',
                            now(), 
                            '$justificativa')
                    RETURNING 
                            htrid";
            $htrid = $db->pegaUm($sql); 
// 	            Atualiza Status transferência

            $sqlstatustrans.= "UPDATE projovemurbano.transferencia SET
			                            htrid_ultimo = $htrid
			                        WHERE
			                            traid = $traid_;";
	            
// 	            Fim Atualiza Status
            
            $sql = "SELECT
                        polid_origem as polo_origem,
                        nucid_origem as nucleo_origem,
                    cad_caeid        as caeid
                    FROM
                        projovemurbano.transferencia
                    WHERE
                        traid = $traid_";
            $dados = $db->pegaLinha($sql);
            
            extract($dados);
            
            if( $polo_origem != '' ){
                
                $sql = "SELECT DISTINCT
                            usu.usuemail
                        FROM
                            projovemurbano.coordenadorresponsavel coo
                        INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid  = coo.pjuid
                        INNER JOIN projovemurbano.polo      pol ON pol.pmuid  = pmu.pmuid
                        INNER JOIN seguranca.usuario        usu ON usu.usucpf = coo.corcpf
                        WHERE
                            pol.polid = $polo_origem";
            }else{
                $sql = "SELECT DISTINCT
                            usu.usuemail
                        FROM
                            projovemurbano.coordenadorresponsavel coo
                        INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid  = coo.pjuid
                        INNER JOIN projovemurbano.municipio mun ON mun.pmuid  = pmu.pmuid
                        INNER JOIN projovemurbano.nucleo    nuc ON nuc.munid  = mun.munid
                        INNER JOIN seguranca.usuario        usu ON usu.usucpf = coo.corcpf
                        WHERE
                            nuc.nucid = $nucleo_origem";
            }
            
            $email_resp_destino = $db->pegaUm($sql);
            
            $sql = "SELECT caecpf||' - '||caenome FROM projovemurbano.cadastroestudante WHERE caeid = $caeid";
//            ver($sql,d);
            $alunos = $db->carregarColuna($sql);
            
            $assunto = "PROJOVEM URABNO - Pedido de transferência rejeitado.";
            
            $conteudo = "O seguinte aluno teve a transferência rejeitado:
                            ".implode('; <br>',$alunos).".";
//           ver($conteudo,$assunto,$alunos,$email_resp_destino,d); 
//            enviar_email( $_SESSION['email_sistema'], $email_resp_destino, $assunto, $conteudo );
        }
    }
    if($sqlstatustrans!=''){
    	$db->executar($sqlstatustrans);
    }
    $db->commit ();
    echo "<script>
                alert('Cancelamento concluído.');
                window.location.href = window.location.href;
            </script>";
}
    
function buscarDadosAluno(){
	
	global $db;
	
	extract($_POST);
    
    $sql="SELECT 
                pjuid as pjuid,
                CASE WHEN estuf IS NOT NULL
                     THEN estuf
                     ELSE muncod END as local,  
                ppuid as ppuid
            FROM 
                projovemurbano.cadastroestudante 
            WHERE
                caecpf = '$caecpf'";
    $pjuidaluno = $db->pegalinha( $sql );
    
//    if( $_SESSION['projovemurbano']['estuf']){
//         $sql = "SELECT 
//               estuf as local
//            FROM
//                projovemurbano.projovemurbano
//            WHERE
//                estuf = '".$_SESSION['projovemurbano']['estuf']."'";
//    }else{
//         $sql = "SELECT 
//               muncod as local
//            FROM
//                projovemurbano.projovemurbano
//            WHERE
//                muncod = '".$_SESSION['projovemurbano']['muncod']."'";
//    }
//   
//    $testalocal = $db->pegaUm( $sql );
   
    
    $sql = "SELECT true FROM projovemurbano.transferencia tra
            INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = tra.cad_caeid
            INNER JOIN projovemurbano.historico_transferencia htr ON htr.htrid = tra.htrid_ultimo
            WHERE caecpf = '$caecpf' AND htr.shtid_status NOT IN (3,4,5)";
              
    $temTranferenciaAtiva = $db->pegaUm( $sql );
//    ver($sql,d);
// ver($pjuidaluno['pjuid'],$_SESSION['projovemurbano']['pjuid'],d);
	if( $pjuidaluno['pjuid'] == $_SESSION['projovemurbano']['pjuid']){
        if($pjuidaluno['ppuid'] == $_SESSION['projovemurbano']['ppuid']){
            if( $temTranferenciaAtiva != 't' ){
            	$sql1 = "SELECT 
            				entnome 
            			FROM entidade.entidade ent 
            			INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid 
            			WHERE 
            				nes.nucid=nuc.nucid 
            				AND nes.nuetipo='S'";
            	
            	$sql2 = "SELECT 
            				entnome 
            			FROM entidade.entidade ent 
            			INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid 
            			WHERE 
            				nes.nucid=nuc.nucid 
            				AND nes.nuetipo='A'";
            	
            	if( $pmupossuipolo == 't' ){
            	
            		$sql = "SELECT DISTINCT
            				 	cae.caeid as caeid,
            		 			cae.turid as turidorigem,
                                cae.ppuid as anoorigem,
            					cae.caenome as nome,
            					to_char(his.hicdataacao,'DD/MM/YYY') as dataingresso,
            					'POLO '||pol.polid as polo,
                                pol.polid as polid,
            					'NUCLEO'||nuc.nucid||',SEDE: '||($sql1)||COALESCE(', ANEXO:'||($sql2),'') as nucleo,
                                nuc.nucid as nucid,
            					turdesc as turma
            				FROM 
            					projovemurbano.cadastroestudante cae
            				LEFT JOIN projovemurbano.polo pol ON pol.polid = cae.polid
            				LEFT JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid
            				LEFT JOIN projovemurbano.turma t ON t.turid = cae.turid
            				LEFT  JOIN projovemurbano.historicocadastro his ON his.caeid = cae.caeid
            				LEFT JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
            				LEFT JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
            				LEFT  JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
            				WHERE
            					caecpf = '$caecpf'";
//             		ver($sql);
            	}else{
            	
            		$sql = "SELECT DISTINCT
            				 	cae.caeid as caeid,
            		 			cae.turid as turidorigem,
                                cae.ppuid as anoorigem,
            					cae.caenome as nome,
            					to_char(his.hicdataacao,'DD/MM/YYY') as dataingresso,
            					'' as polo,
            					'NUCLEO'||nuc.nucid||',SEDE: '||($sql1)||COALESCE(', ANEXO:'||($sql2),'') as nucleo,
                                nuc.nucid as nucid,
            					turdesc as turma
            				FROM 
            					projovemurbano.cadastroestudante cae
            				LEFT JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid
            				LEFT JOIN projovemurbano.turma t ON t.turid = cae.turid
            				LEFT  JOIN projovemurbano.historicocadastro his ON his.caeid = cae.caeid
            				LEFT JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
            				LEFT JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
            				LEFT  JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
            				WHERE
            					caecpf = '$caecpf'";
//                                ver($sql);
            	}
//                ver($sql,d);
            	$dadosAluno = $db->pegaLinha($sql);
            }else{
                
                $dadosAluno = Array('nome'=>utf8_encode("Este aluno já possui tranferência em andamento."));
            }
        }else{
            if($pjuidaluno['ppuid'] == '1'){
               $dadosAluno = Array('nome'=>utf8_encode("Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando"));
            }else{
               $dadosAluno = Array('nome'=>utf8_encode("Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando"));
                }
            }
     }else{
            $dadosAluno = Array('nome'=>utf8_encode("Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando"));
     }
//      ver($dadosAluno,d);
	echo simec_json_encode($dadosAluno);
    die;
//    }
}

function buscarComboPolo(){

    global $db;

    $sql = "SELECT pol.polid as codigo, 'POLO '||pol.polid as descricao 
            FROM projovemurbano.polo pol 
            LEFT  JOIN projovemurbano.polomunicipio             pmu ON pmu.pmuid = pol.pmuid 
            WHERE 
                pmustatus='A' 
                AND pmu.pjuid = {$_SESSION['projovemurbano']['pjuid']}
                AND polstatus='A' 
            ORDER BY pol.polid";    

    mostrarComboPopup( 'Polo:', 'polid',  $sql, '', 'Selecione o(s) Polo(s)', array(), 'filtraNucleo' );
}

function buscarComboNucleo(){

    global $db;
    
    extract($_POST);
    
    if( $polid[0] != '' ) {
        
        $sql = "SELECT DISTINCT
                    nuc.nucid as codigo, 
                    'NUCLEO '||nuc.nucid as descricao 
                FROM projovemurbano.nucleo nuc
                INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
                INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
                INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
                INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
                WHERE 
                    nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
                    AND pol.polstatus='A'  AND pol.polid IN (".implode(', ',$polid).")";

        
    } else {
        
        $sql = "SELECT DISTINCT
                    nuc.nucid as codigo, 
                    'NUCLEO '||nuc.nucid as descricao 
                FROM 
                    projovemurbano.nucleo nuc 
                INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
                INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
                INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = plm.pjuid
                WHERE 
                    pju.pjuid = {$_SESSION['projovemurbano']['pjuid']} AND nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' ";

    }
    
    mostrarComboPopup( 'Núcleo:', 'nucid',  $sql, '', 'Selecione o(s) nucleo(s)', array(), 'filtraTurma' );
}

function buscarComboTurma(){

    global $db;
    
    extract($_POST);
    
    if( $nucid[0] != '' ){
        $sql = "SELECT DISTINCT 
                    turid AS codigo,
                    'Nucleo: '|| nes.nucid || ' - ' ||
                    turdesc || CASE WHEN nes.nuetipo = 'S'
                                 THEN ' SEDE '
                                 ELSE ' ANEXO ' END AS descricao
                FROM projovemurbano.turma t
                INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = t.entid AND nes.nucid IN ( ".implode(', ',$nucid)." )
                WHERE 
                    t.nucid IN ( ".implode(', ',$nucid)." )
                    AND turstatus = 'A'
                ORDER BY 2";
    }else{
        $sql = "SELECT
                    '' as codigo, '' as descricao";
    }
//    ver($_SESSION);
    mostrarComboPopup( 'Turma:', 'turid',  $sql, '', 'Selecione a(s) Turma(s)' );
}

function buscarPolo( ){

	global $db,$polid;
    
	if( $_REQUEST['ppuid'] ){
        
        $where = array("ppuid = '{$_REQUEST['ppuid']}'");
        if( $_REQUEST['estuf'] != '' ){
            $where[] = "estuf = '{$_REQUEST['estuf']}'";
        }elseif( $_REQUEST['muncod'] != '' ){
            $where[] = "muncod = '{$_REQUEST['muncod']}'";
        }elseif( $_SESSION['projovemurbano']['estuf'] != '' ){
            $where[] = "estuf = '".($_REQUEST['estuf']!=''?$_REQUEST['estuf']:$_SESSION['projovemurbano']['estuf'])."'";
        }else{
            $where[] = "muncod = '".($_REQUEST['muncod']!=''?$_REQUEST['muncod']:$_SESSION['projovemurbano']['muncod'])."'";
        }
        if($_SESSION['projovemurbano']['modalidade'] == 'O'){
        	$where[] = "pjuid != {$_SESSION['projovemurbano']['pjuid']}";
        }
        $pjuidx = "AND pjuid IN(SELECT DISTINCT 
        							pjuid
                                FROM projovemurbano.projovemurbano
        						WHERE	
                                ".implode(' AND ', $where).")";
		$sql = "SELECT pol.polid as codigo, 'POLO '||pol.polid as descricao 
				FROM projovemurbano.polo pol 
				LEFT  JOIN projovemurbano.polomunicipio             pmu ON pmu.pmuid = pol.pmuid 
				WHERE 
                    pmustatus='A' 
                    $pjuidx
                    AND polstatus='A' 
				ORDER BY pol.polid";
//            ver($sql,d);     
        if( $_REQUEST['form'] == 'M' ){
            $db->monta_combo('polidM', $sql, 'S', 'Selecione', 'buscarNucleoM', '', '', '', 'N', 'polidM');
        }else{
    		$db->monta_combo('polid', $sql, 'S', 'Selecione', 'buscarNucleo', '', '', '', 'S', 'polid');
        }
	}
}

function buscarNucleo(){

	global $db,$nucid;
    
    $possuipolo = $_POST['pmupossuipolo'] != '' ? $_POST['pmupossuipolo'] : $possuipolo;
//	ver($_POST,d);
   
    $where = array("ppuid = {$_REQUEST['ppuid']}");
   
    if( $_REQUEST['estuf'] != '' ){
    	$where[] = "estuf = '{$_REQUEST['estuf']}'";
    }elseif( $_REQUEST['muncod'] != '' ){
    	$where[] = "muncod = '{$_REQUEST['muncod']}'";
    }elseif( $_SESSION['projovemurbano']['estuf'] != '' ){
    	$where[] = "estuf = '".($_REQUEST['estuf']!=''?$_REQUEST['estuf']:$_SESSION['projovemurbano']['estuf'])."'";
    }else{
    	$where[] = "muncod = '".($_REQUEST['muncod']!=''?$_REQUEST['muncod']:$_SESSION['projovemurbano']['muncod'])."'";
    }
    $where[] = "pjustatus = 'A'";
   		$sql = "SELECT DISTINCT
					pjuid
				FROM projovemurbano.projovemurbano
				WHERE
					".implode(' AND ', $where)."";
//    		ver($sql,d);
//     ver($sql);
        $pjuidx = $db->pegaUm($sql);
     
//    ver( $_REQUEST['com_vaga']);
    if( $_REQUEST['com_vaga'] == 'true' ){
        $sql_com_vaga = "AND nuc.nucid NOT IN (SELECT
                                                    nuc2.nucid
                                                FROM
                                                    projovemurbano.nucleo nuc2
                                                WHERE
                                                    nuc2.nucqtdestudantes <= ( SELECT count(caeid) 
                                                                                            FROM projovemurbano.cadastroestudante cae 
                                                                                            WHERE 
                                                                                            cae.nucid = nuc2.nucid 
                                                                                            AND cae.caestatus = 'A'  ))";
    }
//    ver($sql_com_vaga);
	if($possuipolo=="t") {
        
		
		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NUCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||COALESCE(', ANEXO: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				WHERE 
					nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
				    AND nuc.ppuid = '{$_REQUEST['ppuid']}' AND pol.polstatus='A' AND pol.polid = {$_REQUEST['polid']} 
                    $sql_com_vaga";
// 			ver($sql,d);
			$nucleos = $db->carregar($sql);
		
	} else {
        
        $where = array("nuc.nucstatus='A'","mun.munstatus='A'","plm.pmustatus='A'","nuc.ppuid = {$_REQUEST['ppuid']}");
        
        if( $pjuidx != null ){
            $where[] = "pju.pjuid = $pjuidx";
//         ver($pjuidx != '',d);
        if( $_REQUEST['estuf'] != '' ){
            $where[] = "pju.estuf = '{$_REQUEST['estuf']}'";
        }elseif( $_REQUEST['muncod'] != '' ){
            $where[] = "pju.muncod = '{$_REQUEST['muncod']}'";
        }
		
        if($_SESSION['projovemurbano']['modalidade'] == 'O'){
        	$where[] = "pju.pjuid != {$_SESSION['projovemurbano']['pjuid']}";
        }
        
		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NUCLEO '||nuc.nucid||', 
					SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||
					COALESCE(', ANEXO:'||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM 
					projovemurbano.nucleo nuc 
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
			    INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
			    INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
                INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = plm.pjuid
			    WHERE 
			  		".implode(' AND ',$where)."$sql_com_vaga";
// 		ver($sql,d);
		$nucleos = $db->carregar($sql);
        }else{
        	$nucleos = ( $nucleos ? $nucleos : array(0=> array("codigo"=>0 , "descricao"=>"Sem Nucleos")));
        }
	}
//    ver($sql);
	$nucleos = ( $nucleos ? $nucleos : array(0=> array("codigo"=>0 , "descricao"=>"Sem Nucleos")));
    
    if( $_REQUEST['form'] == 'M' ){
        $db->monta_combo('nucidM', $nucleos, 'S', 'Selecione', 'buscarTurmasM', '', '', '', 'N', 'nucidM');
    }else{
    	$db->monta_combo('nucid', $nucleos, 'S', 'Selecione', 'buscarTurmas', '', '', '', 'S', 'nucid');
    }
}

function anexarArquivos(){
        monta_titulo('Lista de Arquivos','');

        $acao = "
            <center>
                <img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" onclick=\"excluirArquivoInfra('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">
                &nbsp;&nbsp;
                <img src=\"../imagens/indicador-vermelha.png\" title=\"Baixar Arquivo (Download)\" style=\"cursor: pointer\" onclick=\"downloadArquivoInfra('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">            
            </center>
        ";

        $sql = "
            SELECT '{$acao}' as acao,
                    ta.tpadsc as tipo_doc,
                    arqdescricao as descricao,
                    arqnome||'.'||arqextensao as arqnome,
                    arqtamanho as tamanho,
                    to_char(arqdata, 'DD/MM/YYYY') as data
            FROM academico.demandainfra di
            JOIN academico.arquivotipologia AS a ON a.dinid = di.dinid
            JOIN academico.tipoarquivo AS ta ON ta.tpaid = a.tpaid
            JOIN public.arquivo AS arq ON arq.arqid = a.arqid
            WHERE di.dinid  = {$dinid}
        ";
        $cabecalho = array("<center>Ação</center>", "Tido de Documento", "Descrição Arquivo", "Nome Arquivo", "Tamanho (MB)", "Data Inclusão");
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
        $tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
}

if (!$_SESSION['projovemurbano']['pjuid']) {
	die("<script>
    		alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
            window.location='projovemurbano.php?modulo=inicio&acao=C';
		</script>");
}

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

if( $_POST['traid'][0] != '' ){
    enviar_aluno_transferir();
    exit;
}

if ($_REQUEST['estuf']){
    $_SESSION['projovemurbano']['estuf'] = $_REQUEST['estuf'];
} else if ($_REQUEST['muncod']){
    $_SESSION['projovemurbano']['muncod'] = $_REQUEST['muncod'];
}

if (!$_SESSION['projovemurbano']['pjuid']) {
    carregarProJovemUrbano();
    //encaminharUltimoAcesso();
}
if (!$_SESSION['projovemurbano']['pjuid']) {
    die("<script>
			alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
					window.location='projovemurbano.php?modulo=inicio&acao=C';
		</script>");
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
echo montarAbasArray(montaMenuProJovemUrbano(),"/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A");
monta_titulo('Transferência de Alunos',montaTituloEstMun());

$db->cria_aba($abacod_tela,$url,$parametros);
if($_SESSION['projovemurbano']['ppuid']!=1){	
	$sql = "SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}";
	
	$ajustados = $db->pegaUm($sql);
	
	$perfis = pegaPerfilGeral();
	
	if( $ajustados == 't' ){
		$not = ",7,10,13";
	}else{
		$not = ",9,12,15";
	}
	$filtrotprid = '';
	if(in_array(PFL_DIRETOR_POLO, $perfis)){
		$sql = "SELECT DISTINCT
					pmu.tprid
				FROM
					projovemurbano.polomunicipio pmu
				INNER JOIN projovemurbano.polo pol ON pol.pmuid = pmu.pmuid
				INNER JOIN projovemurbano.usuarioresponsabilidade res ON res.polid = pol.polid
				INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = pmu.pjuid
				WHERE
					usucpf = '" . $_SESSION ['usucpf'] . "'
					AND pju.ppuid ={$_SESSION['projovemurbano']['ppuid']}";
		$tprid = $db->pegaUm($sql);
		$filtrotprid = "AND tpr.tprid = {$tprid}";
	
	}elseif(in_array(PFL_DIRETOR_NUCLEO, $perfis)||in_array(PFL_DIRETOR_ESCOLA, $perfis)){
		$sql = "SELECT DISTINCT
						nuc.tprid
					FROM
						projovemurbano.nucleo nuc
					INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
					INNER JOIN projovemurbano.usuarioresponsabilidade res ON res.entid = nes.entid
					WHERE
						usucpf = '" . $_SESSION ['usucpf'] . "'
					AND ppuid ={$_SESSION['projovemurbano']['ppuid']}";
		$tprid = $db->pegaUm($sql);
		$filtrotprid = "AND tpr.tprid = {$tprid}";
	
	}
	$sql = "SELECT DISTINCT
				removeacento(replace(tprdesc,' ','_')) as aba,
				tprdesc as descricao, mtpvalor,min(tpr.tprid)as tprid
			FROM
				projovemurbano.tipoprograma tpr
			INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
			INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
			INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
			WHERE
				mtp.ppuid = {$_SESSION['projovemurbano']['ppuid']}
			AND mtp.pjuid = {$_SESSION['projovemurbano']['pjuid']}
			AND mtpvalor > 0
			AND tmp.tpmid NOT IN  (2,5,8,11,14$not)
			$filtrotprid
			GROUP by
				tprdesc,
				mtpvalor
			ORDER BY
				tprid";
	$programas = $db->pegaLinha($sql);
	if($_GET['aba'] == ''){
		$_GET['aba'] = $programas['aba'];
		$_SESSION['projovemurbano']['tprid'] = $programas['tprid'];
	}
	// ver($_GET['aba'],$tpridestudante,d);
	$us = pegarUsuarioProJovem();
	$rsMetas = recuperaMetasPorUfMuncod($us);
	switch( $_GET['aba'] )
	{
	case 'Publico_Juventude_Viva':
			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=Publico_Juventude_Viva";
			$_SESSION['projovemurbano']['tprid'] = 1;
			break;
		case 'Publico_Unidades_Prisionais':
			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=Publico_Unidades_Prisionais";
			$_SESSION['projovemurbano']['tprid'] = 2;
			break;
		case 'Publico_Geral':
			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=Publico_Geral";
			$_SESSION['projovemurbano']['tprid'] = 3;
			break;
		default:
			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}";
			$pagAtiva = "transferenciaAluno/enviar_aluno.inc";
		break;
	}
	
	$sql = "SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}";
		
	$ajustado = $db->pegaUm($sql);
	
	if( $ajustado == 't' ){
		$not = ",7,10,13";
	}else{
		$not = ",9,12,15";
	
	}
	$sql = "SELECT DISTINCT
				removeacento(replace(tprdesc,' ','_')) as aba,
				tprdesc as descricao, mtpvalor,tpr.tprid
			FROM
				projovemurbano.tipoprograma tpr
			INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
			INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
			INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
			WHERE
				mtp.ppuid = {$_SESSION['projovemurbano']['ppuid']}
			AND mtp.pjuid = {$_SESSION['projovemurbano']['pjuid']}
			AND mtpvalor > 0
			AND tmp.tpmid NOT IN  (2,5,8,11,14$not)
			$filtrotprid
			ORDER BY
				tpr.tprid";
		$programas = $db->carregar($sql);
	if($programas){
		foreach( $programas as $k => $programa ){
			$menu[] = array("id" => $k+1,
			"descricao" => $programa['descricao'],
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=".$programa['aba']);
		}
	}
	echo montarAbasArray($menu, $abaAtiva);
	/*** Array com os itens da aba de identificação ***/
	if( $_GET['aba'] != ''){
		switch ($_REQUEST['aba2']) {
			case 'enviar_aluno':
				$abaAtiva2 = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=enviar_aluno";
				$pagAtiva = "transferenciaAluno/enviar_aluno.inc";
			 break;
			case 'receber_aluno':
				$abaAtiva2 = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=receber_aluno";
				$pagAtiva = "transferenciaAluno/receber_aluno.inc";
				break;
			case 'paineldetransferencia':
				$abaAtiva2 = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=paineldetransferencia";
				$pagAtiva = "transferenciaAluno/paineldetransferencia.inc";
				break;
			default:
				$abaAtiva2 = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=enviar_aluno";
				$pagAtiva = "transferenciaAluno/enviar_aluno.inc";
			break;
		}
		$menu2 = array( 	
						0 => array("id" => 1, "descricao" => "Enviar Alunos", "link" => 		  "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=enviar_aluno")
				  	   ,1 => array("id" => 2, "descricao" => "Receber Aluno", "link" => 		  "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=receber_aluno")
				  	   ,2 => array("id" => 3, "descricao" => "Painel de Transferência", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba={$_GET['aba']}&aba2=paineldetransferencia")
					  );
		echo montarAbasArray($menu2, $abaAtiva2);
	}							
	echo "<br/>"; 
	$perfis = pegaPerfilGeral();
	if($db->testa_superuser()||in_array(PFL_ADMINISTRADOR, $perfis)||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_CONSULTA, $perfis)){
		include_once $pagAtiva;
	}
}else{
	$db->cria_aba($abacod_tela,$url,$parametros);
	$aba = !$_GET['aba'] ? "enviar_aluno" : $_GET['aba'];
	
	$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=$aba";
	
	/*** Array com os itens da aba de identificação ***/
	$menu = array( 0 => array("id" => 1, "descricao" => "Enviar Alunos", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=enviar_aluno")
			,1 => array("id" => 2, "descricao" => "Receber Aluno", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=receber_aluno")
			,2 => array("id" => 3, "descricao" => "Painel de Transferência", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A&aba=paineldetransferencia")
	);
	echo "<br/>";
	$perfis = pegaPerfilGeral();
	if($db->testa_superuser()||in_array(PFL_ADMINISTRADOR, $perfis)||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_CONSULTA, $perfis)){
	
		echo montarAbasArray($menu, $abaAtiva);
	
		include_once "transferenciaAluno/".$aba.".inc";
	}
}
 ?>
<? registarUltimoAcesso(); ?>
