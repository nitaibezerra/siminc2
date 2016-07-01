<?php
ini_set("memory_limit", "2048M");

if ($_REQUEST['filtrosession']) {
    $filtroSession = $_REQUEST['filtrosession'];
}

$isXls = ($_POST['req'] == 2) ? true : false;


if ($isXls) {
    $file_name = "RelatorioGeralEiManutencao" . date('d_m_Y_G_i_s') . ".xls";
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Content-Type: application/force-download");
    header("Content-Type: application/octet-stream");
    header("Content-Type: application/download");
    header("Content-Disposition: attachment;filename=" . $file_name);
    header("Content-Transfer-Encoding: binary ");
}

if (!$isXls) {
    header('Content-Type: text/html; charset=iso-8859-1');
    ?>
    <html>
        <head>
            <script type="text/javascript" src="../includes/funcoes.js"></script>
            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        </head>
        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
            <?php
        }

        $sql = monta_sql();

        function monta_sql() {
            if ($_REQUEST['anopagamento']) {
                $exercicio = $_REQUEST['anopagamento'];
            } else {
                $exercicio = ' Geral ';
            }
            echo '<html>
            <head>
         
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Relatório Município Proinfância - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 98% !important;" align="center"  >';

            extract($_POST);
                $arWhere = Array("obr.obrstatus = 'A'", "ent.entstatus = 'A'", " obr.obrpercentultvistoria >= 90", " pf.prfid = 41 ", "esd1.esdid IN (690, 693)", "obr.obridpai IS NULL", "tpl.tpoid in (16,9,10)");

                // UF
//                if ($estuf[0] && $estuf_campo_flag) {
//                    $arWhere[] = " ede.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $estuf) . "' ) ";
//                }
                // Municipio
                if ($muncod[0] && $muncod_campo_flag) {
                    $arWhere[] = " mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $muncod) . "' ) ";
                }

                if ($obraid) {
                    $arWhere[] = " obr.obrid = '{$obraid}'";
                }

                $innerLote = "INNER JOIN proinfantil.loteproinfancia lot ON lot.lotid = pin.lotid";
                $campoLote = ", lot.lotid";
                $campoLote2 = ", lotid";

                if ($lotid) {
                    $arWhere[] = " lot.lotid = $lotid ";
// 		$arWhere[] = " lot.lotid ". (!$lotid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $lotid )."' ) ";
                }

                if ($anoini) {
                    $arWhere[] = "to_char(to_date(res.resdsc,'DD/MM/YYYY'),'YYYY') = '{$anoini}'";
                }

                if (!$anopagamento) {
                    $anoPagamento = 'null';
                }

                //Ano do Pagamento
                if ($anopagamento) {
                    $anoPagamento = "'".$anopagamento."'";
                    $anopagamento = "'" . $anopagamento . "'";
                    $arWhere[] = "pag.anopagamento = {$anopagamento}";
//                    $join_pagamento = "LEFT  JOIN 
//									(
//									SELECT			
//										pin.pinid, 
//										to_char(hst.htddata,'YYYY') AS anopagamento
//									FROM 			
//										workflow.historicodocumento hst
//									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
//									WHERE 			
//										hst.aedid = " . AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO . " 
//									ORDER BY		
//										hst.htddata DESC
//									) as pag ON pag.pinid = pin.pinid";
//                    
                }
                
                $sqlUF = "SELECT DISTINCT
 	estado,
	municipio,
	sum(mncintcre) as qnttic , 
	sum(mncintpre) as qnttipe,	
	sum(mncparcre) as qnttpc,
	sum(mncparpre) as qnttppe,
        sum(mncintcre) + sum(mncintpre) + sum(mncparcre) + sum(mncparpre) as total_nao,
        sum(totalintcre) as totalintcre,
	sum(totalintpre) as totalintpre,
	sum(totalparcre) as totalparcre,
	sum(totalparpre) as totalparpre,
        sum(totalintcre) + sum(totalintpre) + sum(totalparcre) + sum(totalparpre) as total_matricula,
	CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorintcre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} < maiordata ))
		THEN COALESCE( valorintcre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorintcre + valorintcre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorintcre, 0)
				END
			END
		END 
	END AS vtic,
	CASE WHEN (analiseseguinte = 'S'  AND ({$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorintpre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ({$anoPagamento} < maiordata ))
		THEN COALESCE( valorintpre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorintpre + valorintpre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorintpre, 0)
				END
			END
		END 
	END AS vtipe,
	CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorparcre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} < maiordata ))
		THEN COALESCE( valorparcre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorparcre + valorparcre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorparcre, 0)
				END
			END
		END 
	END AS vtpc,
	CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorparpre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} < maiordata ))
		THEN COALESCE( valorparpre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorparpre + valorparpre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorparpre, 0)
				END
			END
		END 
	END AS vtppe 
        ,q_obras as qnt_obras
	, tipologia_obra as tipologia_obra
	, percentual_obra as percentual_obra
	, estado_obra as estado_obra
        , id_obra
	, to_char(to_date(data_inicio_atendimento,'DD/MM/YYYY'),'DD/MM/YYYY') as data_inicio_atendimento
        {$campoLote2}
	,estado_p as estado_plano 
FROM (	

	SELECT 
							ede.estuf as estado
						       , mun.mundescricao as municipio
						       , pin.pinanoseguinte as analiseseguinte
						       , pinpareceraprovacao2
						       , pag.anopagamento
						       , vlr.mncintcre as mncintcre
						       , vlr.mncintpre as mncintpre
						       , vlr.mncparcre as mncparcre
						       , vlr.mncparpre as mncparpre
						       , vlr.totalintcre
						       , vlr.totalintpre
							, vlr.totalparcre
							, vlr.totalparpre
						       , count(obr.obrid) as q_obras
						       , esd.esddsc as estado_p
						       ,pin.docid
						       , tpl.tpodsc as tipologia_obra
							, obr.obrpercentultvistoria as percentual_obra
							, to_char(to_date(res.resdsc,'DD/MM/YYYY'),'DD/MM/YYYY') as data_inicio_atendimento
							, esd1.esddsc as estado_obra
                                                        {$campoLote}
                                                        , obr.obrid as id_obra
                                                        ,
						
							(  SELECT                                 
									     max(to_char(hst.htddata,'YYYY'))
							     FROM                                  
									     workflow.historicodocumento hst
							     INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
							     WHERE                                                
									     hst.aedid = 1034 
									     and hst.docid = pin.docid
									    
							    
							) as maiordata,

						       
						       
						      sum(vlr.valorintcre) as valorintcre,
						        sum(vlr.valorintcre2) as valorintcre2,
						        sum(vlr.valorintpre) as valorintpre,
						       sum( vlr.valorintpre2) as valorintpre2,
						        sum(vlr.valorparcre) as valorparcre,
						        sum(vlr.valorparcre2) as valorparcre2,
						       sum(vlr.valorparpre) as valorparpre,
						        sum(vlr.valorparpre2) as valorparpre2
	FROM
				obras2.obras obr
			
			
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = " . WF_PROINFANTIL . "
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			$inner_total_repassado
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			$innerLote
			LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = " . AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO . " 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid
								LEFT JOIN entidade.funentassoc fea ON fea.entid = obr.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.fueid = fea.fueid AND fen.funid=2 
					LEFT JOIN entidade.entidade pre ON pre.entid = fen.entid 
INNER JOIN obras2.tipologiaobra AS tpl ON obr.tpoid = tpl.tpoid AND tpl.tpostatus = 'A' 
LEFT  JOIN 
				(
				SELECT    		
					pin.pinid, 
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse
						ELSE 0
					END) as valorintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse 
						ELSE 0
					END) as valorintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparpre,	    
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorintcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorintpre2,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorparcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorparpre2		    
				FROM
					proinfantil.mdsalunoatendidopbf  mat 
				INNER JOIN proinfantil.valoraluno 	val ON val.timid = mat.timid and vaastatus = 'A'
				INNER JOIN proinfantil.proinfantil 	pin ON pin.pinid = mat.pinid
				INNER JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
				INNER JOIN questionario.resposta 	res ON res.qrpid = que.qrpid
				WHERE
					res.perid = 1587 AND to_date(res.resdsc,'DD/MM/YYYY') BETWEEN vaadatainicial AND vaadatafinal
				GROUP BY
					pin.pinid
				) as 				vlr ON vlr.pinid = pin.pinid
			LEFT JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
			LEFT JOIN questionario.resposta 		res ON res.qrpid = que.qrpid AND res.perid = 1587
			
			";
                $sqlUF .= " WHERE 	   
				" . ( $arWhere[0] ? implode(' AND ', $arWhere) : '' );

                $sqlUF .="
					GROUP BY estado,
		municipio,
		pin.pinanoseguinte 
								       , pinpareceraprovacao2
								       , pag.anopagamento
								       , vlr.mncintcre 
								       , vlr.mncintpre
								       , vlr.mncparcre
								       , vlr.mncparpre 
								       , esd.esddsc 
								       ,pin.docid
								       	, vlr.totalintcre
									, vlr.totalintpre
									, vlr.totalparcre
									, vlr.totalparpre
									, tpl.tpodsc 
									, obr.obrpercentultvistoria 
									, to_char(to_date(res.resdsc,'DD/MM/YYYY'),'DD/MM/YYYY') 
									, esd1.esddsc 
                                                                        {$campoLote}
                                                                        ,obr.obrid
				 ";

                $sqlUF .= "
			ORDER BY ede.estuf ,mun.mundescricao, esd.esddsc ) AS foo group by foo.estado,
	foo.municipio, foo.mncintcre, foo.analiseseguinte, foo.maiordata, foo.valorintcre2, foo.valorintcre,
	foo.valorintpre2, foo.valorintpre, foo.valorparcre2, foo.valorparcre, foo.valorparpre2, foo.valorparpre,foo.q_obras,
	foo.estado_p , foo.tipologia_obra, foo.percentual_obra, foo.data_inicio_atendimento, foo.estado_obra {$campoLote2},
        foo.id_obra";
        
       // ver($sqlUF,d);
            global $db;
            $UFs = $db->carregar($sqlUF);

            $totalq1g = 0;
            $totalq2g = 0;
            $totalq3g = 0;
            $totalq4g = 0;
            $totalqtg = 0;
            $totalv1g = 0;
            $totalv2g = 0;
            $totalv3g = 0;
            $totalv4g = 0;
            $totalqtrg = 0;
            $totalqog = 0;

            if($UFs){
            foreach ($UFs as $key) {
                extract($_POST);
                $arWhere = Array("obr.obrstatus = 'A'", "ent.entstatus = 'A'", " obr.obrpercentultvistoria >= 90", " pf.prfid = 41 ", "esd1.esdid IN (690, 693)", "obr.obridpai IS NULL", "tpl.tpoid in (16,9,10)");

                // UF
//                if ($estuf[0] && $estuf_campo_flag) {
//                    $arWhere[] = " ede.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $estuf) . "' ) ";
//                }
                // Municipio
                if ($muncod[0] && $muncod_campo_flag) {
                    $arWhere[] = " mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $muncod) . "' ) ";
                }

                if ($obraid) {
                    $arWhere[] = " obr.obrid = '{$obraid}'";
                }

                $innerLote = "INNER JOIN proinfantil.loteproinfancia lot ON lot.lotid = pin.lotid";
                $campoLote = ", lot.lotid";
                $campoLote2 = ", lotid";

                if ($lotid) {
                    $arWhere[] = " lot.lotid = $lotid ";
// 		$arWhere[] = " lot.lotid ". (!$lotid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $lotid )."' ) ";
                }

                if ($anoini) {
                    $arWhere[] = "to_char(to_date(res.resdsc,'DD/MM/YYYY'),'YYYY') = '{$anoini}'";
                }

                if (!$anopagamento) {
                    $anoPagamento = 'null';
                }

                //Ano do Pagamento
                if ($anopagamento) {
                    $anoPagamento = $anoPagamento;
                    $anoPagamento = "'".$anopagamento."'";
                    $anopagamento = "'" . $anopagamento . "'";
                    $arWhere[] = "pag.anopagamento = {$anopagamento}";
//                    $join_pagamento = "LEFT  JOIN 
//									(
//									SELECT			
//										pin.pinid, 
//										to_char(hst.htddata,'YYYY') AS anopagamento
//									FROM 			
//										workflow.historicodocumento hst
//									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
//									WHERE 			
//										hst.aedid = " . AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO . " 
//									ORDER BY		
//										hst.htddata DESC
//									) as pag ON pag.pinid = pin.pinid";
//                    
                }

                $sql = "SELECT DISTINCT
 	estado,
	municipio,
	sum(mncintcre) as qnttic , 
	sum(mncintpre) as qnttipe,	
	sum(mncparcre) as qnttpc,
	sum(mncparpre) as qnttppe,
        sum(mncintcre) + sum(mncintpre) + sum(mncparcre) + sum(mncparpre) as total_nao,
        sum(totalintcre) as totalintcre,
	sum(totalintpre) as totalintpre,
	sum(totalparcre) as totalparcre,
	sum(totalparpre) as totalparpre,
        sum(totalintcre) + sum(totalintpre) + sum(totalparcre) + sum(totalparpre) as total_matricula,
	CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorintcre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} < maiordata ))
		THEN COALESCE( valorintcre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorintcre + valorintcre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorintcre, 0)
				END
			END
		END 
	END AS vtic,
	CASE WHEN (analiseseguinte = 'S'  AND ({$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorintpre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ({$anoPagamento} < maiordata ))
		THEN COALESCE( valorintpre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorintpre + valorintpre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorintpre, 0)
				END
			END
		END 
	END AS vtipe,
	CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorparcre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} < maiordata ))
		THEN COALESCE( valorparcre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorparcre + valorparcre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorparcre, 0)
				END
			END
		END 
	END AS vtpc,
	CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} = maiordata ))
	THEN  	COALESCE( valorparpre2, 0)
	ELSE 
		CASE WHEN (analiseseguinte = 'S'  AND ( {$anoPagamento} < maiordata ))
		THEN COALESCE( valorparpre, 0)
		ELSE
			CASE WHEN analiseseguinte = 'S' 
			THEN COALESCE( valorparpre + valorparpre2, 0)
			ELSE
				CASE WHEN (analiseseguinte = 'N' OR analiseseguinte = '' OR analiseseguinte IS NULL  ) 
				THEN COALESCE( valorparpre, 0)
				END
			END
		END 
	END AS vtppe 
        ,q_obras as qnt_obras
	, tipologia_obra as tipologia_obra
	, percentual_obra as percentual_obra
	, estado_obra as estado_obra
        , id_obra
	, to_char(to_date(data_inicio_atendimento,'DD/MM/YYYY'),'DD/MM/YYYY') as data_inicio_atendimento
        {$campoLote2}
	,estado_p as estado_plano 
FROM (	

	SELECT 
							ede.estuf as estado
						       , mun.mundescricao as municipio
						       , pin.pinanoseguinte as analiseseguinte
						       , pinpareceraprovacao2
						       , pag.anopagamento
						       , vlr.mncintcre as mncintcre
						       , vlr.mncintpre as mncintpre
						       , vlr.mncparcre as mncparcre
						       , vlr.mncparpre as mncparpre
						       , vlr.totalintcre
						       , vlr.totalintpre
							, vlr.totalparcre
							, vlr.totalparpre
						       , count(obr.obrid) as q_obras
						       , esd.esddsc as estado_p
						       ,pin.docid
						       , tpl.tpodsc as tipologia_obra
							, obr.obrpercentultvistoria as percentual_obra
							, to_char(to_date(res.resdsc,'DD/MM/YYYY'),'DD/MM/YYYY') as data_inicio_atendimento
							, esd1.esddsc as estado_obra
                                                        {$campoLote}
                                                        , obr.obrid as id_obra
                                                        ,
						
							(  SELECT                                 
									     max(to_char(hst.htddata,'YYYY'))
							     FROM                                  
									     workflow.historicodocumento hst
							     INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
							     WHERE                                                
									     hst.aedid = 1034 
									     and hst.docid = pin.docid
									    
							    
							) as maiordata,

						       
						       
						      sum(vlr.valorintcre) as valorintcre,
						        sum(vlr.valorintcre2) as valorintcre2,
						        sum(vlr.valorintpre) as valorintpre,
						       sum( vlr.valorintpre2) as valorintpre2,
						        sum(vlr.valorparcre) as valorparcre,
						        sum(vlr.valorparcre2) as valorparcre2,
						       sum(vlr.valorparpre) as valorparpre,
						        sum(vlr.valorparpre2) as valorparpre2
	FROM
				obras2.obras obr
			
			
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = " . WF_PROINFANTIL . "
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			$inner_total_repassado
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			$innerLote
			LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = " . AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO . " 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid
								LEFT JOIN entidade.funentassoc fea ON fea.entid = obr.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.fueid = fea.fueid AND fen.funid=2 
					LEFT JOIN entidade.entidade pre ON pre.entid = fen.entid 
INNER JOIN obras2.tipologiaobra AS tpl ON obr.tpoid = tpl.tpoid AND tpl.tpostatus = 'A' 
LEFT  JOIN 
				(
				SELECT    		
					pin.pinid, 
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse
						ELSE 0
					END) as valorintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse 
						ELSE 0
					END) as valorintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparpre,	    
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorintcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorintpre2,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorparcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorparpre2		    
				FROM
					proinfantil.mdsalunoatendidopbf  mat 
				INNER JOIN proinfantil.valoraluno 	val ON val.timid = mat.timid and vaastatus = 'A'
				INNER JOIN proinfantil.proinfantil 	pin ON pin.pinid = mat.pinid
				INNER JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
				INNER JOIN questionario.resposta 	res ON res.qrpid = que.qrpid
				WHERE
					res.perid = 1587 AND to_date(res.resdsc,'DD/MM/YYYY') BETWEEN vaadatainicial AND vaadatafinal
				GROUP BY
					pin.pinid
				) as 				vlr ON vlr.pinid = pin.pinid
			LEFT JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
			LEFT JOIN questionario.resposta 		res ON res.qrpid = que.qrpid AND res.perid = 1587
			
			";
                $sql .= " WHERE 	   
				" . ( $arWhere[0] ? implode(' AND ', $arWhere) : '' );

                $sql .= " and ede.estuf ='" . $key[estado] . "'";
                $sql .="
					GROUP BY estado,
		municipio,
		pin.pinanoseguinte 
								       , pinpareceraprovacao2
								       , pag.anopagamento
								       , vlr.mncintcre 
								       , vlr.mncintpre
								       , vlr.mncparcre
								       , vlr.mncparpre 
								       , esd.esddsc 
								       ,pin.docid
								       	, vlr.totalintcre
									, vlr.totalintpre
									, vlr.totalparcre
									, vlr.totalparpre
									, tpl.tpodsc 
									, obr.obrpercentultvistoria 
									, to_char(to_date(res.resdsc,'DD/MM/YYYY'),'DD/MM/YYYY') 
									, esd1.esddsc 
                                                                        {$campoLote}
                                                                        ,obr.obrid
				 ";

                $sql .= "
			ORDER BY ede.estuf ,mun.mundescricao, esd.esddsc ) AS foo group by foo.estado,
	foo.municipio, foo.mncintcre, foo.analiseseguinte, foo.maiordata, foo.valorintcre2, foo.valorintcre,
	foo.valorintpre2, foo.valorintpre, foo.valorparcre2, foo.valorparcre, foo.valorparpre2, foo.valorparpre,foo.q_obras,
	foo.estado_p , foo.tipologia_obra, foo.percentual_obra, foo.data_inicio_atendimento, foo.estado_obra {$campoLote2},
        foo.id_obra";
               // ver($sql, d);

                $registros = $db->carregar($sql);
                $totalq1 = 0;
                $totalq2 = 0;
                $totalq3 = 0;
                $totalq4 = 0;
                $totalqt = 0;
                $totalv1 = 0;
                $totalv2 = 0;
                $totalv3 = 0;
                $totalv4 = 0;
                $totalqtr = 0;
                $totalqo = 0;
                $totaltic = 0;
                $totaltip = 0;
                $totaltpc = 0;
                $totaltpp = 0;
                $totaltm = 0;

                //ver($sql,d);
                //$db->monta_lista($sql, $cabecalho, 500000, 5, 'N', '90%', 'N');

                echo '<tr>
                <td style="font-size: 20px;padding: 10px 10px 10px 0" colspan="4">' . $key[estado] . '</td>
            </tr>
            <tr>
                <td style="text-align: left"><b>UF</td></b> <td style="text-align: left"><b>Município</b></td>
                <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Integral / Creche</b></td>
                <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Integral / Pré-Escola</b></td>
                <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Parcial / Creche</b></td>
                <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Parcial / Pré-Escola</b></td>
                <td style="text-align: left; color:#999999;"><b>Total de  Matrículas não computadas na escola</b></td>
                <td style="text-align: left"><b>Total de Matrículas na Escola - Tempo Integral/Creche</b></td>
                <td style="text-align: left"><b>Total de Matrículas na Escola - Tempo Integral/Pré-Escola</b></td>
                <td style="text-align: left"><b>Total de Matrículas na Escola - Tempo Parcial/Creche</b></td>
                <td style="text-align: left"><b>Total de Matrículas na Escola - TEmpo Parcial/Pré-Escola</b></td>
                <td style="text-align: left; color:#999999;"><b>Total de  Matrículas na escola</b></td>
                <td style="text-align: left"><b>Valor de repasse - Tempo Intregal / Creche (R$)</b></td>
                <td style="text-align: left"><b>Valor de repasse - Tempo Intregal / Pré-Escola (R$)</b></td>
                <td style="text-align: left"><b>Valor de repasse - Tempo Parcial / Creche (R$)</b></td>
                <td style="text-align: left"><b>Valor de repasse - Tempo Parcial / Pré-Escola (R$)</b></td>
                <td style="text-align: left; color:#999999;"><b>Valor Total de Repasse (R$)</b></td>
                <td><b>Tipologia</b></td>
                <td><b>Percentual de Execução (%)</b></td>
                <td><b>Situação da Obra</b></td>
                <td><b>ID da Obra</b></td>
                <td><b>Qtd. de Obras</b></td>
                <td><b>Data de Início de Atend.</b></td>
                <td><b>Nº de Portaria</b></td>
                <td><b>Nº do Lote</b></td>
                <td><b>Situação do Plano</b></td>
            </tr>';
                if (!empty($registros)) {
                    foreach ($registros as $value) {
                        $total_repasse = 0;
                        $totalq1 += $value[qnttic];
                        $totalq2 += $value[qnttipe];
                        $totalq3 += $value[qnttpc];
                        $totalq4 += $value[qnttppe];
                        $totalqt += $value[total_nao];
                        $totalv1 += $value[vtic];
                        $totalv2 += $value[vtipe];
                        $totalv3 += $value[vtpc];
                        $totalv4 += $value[vtppe];
                        $totalqo += $value[qnt_obras];
                        $totalq1g += $value[qnttic];
                        $totalq2g += $value[qnttipe];
                        $totalq3g += $value[qnttpc];
                        $totalq4g += $value[qnttppe];
                        $totalqtg += $value[total_nao];
                        $totalv1g += $value[vtic];
                        $totalv2g += $value[vtipe];
                        $totalv3g += $value[vtpc];
                        $totalv4g += $value[vtppe];
                        $totalqog += $value[qnt_obras];
                        $totalticg += $value[totalintcre];
                        $totaltipg += $value[totalintpre];
                        $totaltpcg += $value[totalparcre];
                        $totaltppg += $value[totalparpre];
                        $totaltmg += $value[total_matricula];
                        $total_repasse = $value[vtic] + $value[vtipe] + $value[vtpc] + $value[vtppe];
                        $totalqtrg += $total_repasse;
                        $totalqtr += $total_repasse;
                        $totaltic += $value[totalintcre];
                        $totaltip += $value[totalintpre];
                        $totaltpc += $value[totalparcre];
                        $totaltpp += $value[totalparpre];
                        $totaltm += $value[total_matricula];
                        echo
                        '<tr>
            <td  style="text-align: center">' . $value[estado] . '</td>
            <td style="text-align: center">' . $value[municipio] . '</td>
            <td style="text-align: center">' . $value[qnttic] . '</td>
            <td style="text-align: center">' . $value[qnttipe] . '</td>
            <td style="text-align: center">' . $value[qnttpc] . '</td>
            <td style="text-align: center">' . $value[qnttppe] . '</td>
            <td style="text-align: center">' . $value[total_nao] . '</td>
            <td style="text-align: center">' . $value[totalintcre] . '</td>
            <td style="text-align: center">' . $value[totalintpre] . '</td>
            <td style="text-align: center">' . $value[totalparcre] . '</td>
            <td style="text-align: center">' . $value[totalparpre] . '</td>
            <td style="text-align: center">' . $value[total_matricula] . '</td>
            <td style="text-align: right">' . number_format($value[vtic], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[vtipe], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[vtpc], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[vtppe], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($total_repasse, 2, ',', '.') . '</td>
            <td style="text-align: center">' . $value[tipologia_obra] . '</td>
            <td style="text-align: center">' . $value[percentual_obra] . '</td>
            <td style="text-align: center">' . $value[estado_obra] . '</td>
            <td style="text-align: center">' . $value[id_obra] . '</td>
            <td style="text-align: center">' . $value[qnt_obras] . '</td>
            <td style="text-align: center">' . $value[data_inicio_atendimento] . '</td>
            <td style="text-align: center"></td>
            <td style="text-align: center">' . $value[lotid] . '</td>
            <td>' . $value[estado_plano] . '</td>
        </tr>';
                    }
//                    if( $totalq1 == 0 && $totalq2 == 0 && $totalq3 == 0 && $totalq4 == 0 && $totalqt == 0 && $totalv1 == 0 && 
//                            $totalv2 == 0 && $totalv3 == 0 && $totalv4 == 0 && $totalqtr == 0 && $totalqo == 0){
//                              
//                        echo '<tr><td colspan="14" style="text-align: center">Nenhum registro para este Estado.</td></tr>';
//                    }else{
                    echo
                    '<tr><td colspan="2" style="text-align: right"><b>Total do Estado:</b></td><td style="background-color: #cccccc;text-align: center"><b>' . $totalq1 . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq2 . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq3 . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq4 . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqt . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltic . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltip . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltpc . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltpp . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltm . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv1, 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv2, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv3, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv4, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalqtr, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqo . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
           </tr>';

//                    }
                }
            }
            echo
            '<tr></tr><tr></tr><tr></tr><tr></tr><tr></tr><tr></tr><tr><td colspan="2" style="text-align: right"><b>Total Geral:</b></td><td style="background-color: #cccccc;text-align: center"><b>' . $totalq1g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq2g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq3g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq4g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqtg . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalticg . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltipg . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltpcg . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltppg . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totaltmg . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv1g, 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv2g, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv3g, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv4g, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalqtrg, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>   
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqog . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
            <td style="background-color: #cccccc;text-align: center"><b></b></td>
           </tr>';
            echo '</table> <br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table></body>';
            die();
            }
            else{
                        echo 'Nenhum registro encontrado!';
                        die();
                    }
            
                    }
        ?>
    </body>