<?php
ini_set("memory_limit", "2048M");

if ($_REQUEST['filtrosession']) {
    $filtroSession = $_REQUEST['filtrosession'];
}

$isXls = ($_POST['req'] == 2) ? true : false;

if ($isXls) {
    $file_name = "Relatorio_Proinfancia_" . date('d_m_Y') . ".xls";
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

        $sql = monta_sql($isXls);

        function monta_sql($tipoRelatorio) {

            if ($_REQUEST['anopagamento']) {
                $exercicio = $_REQUEST['anopagamento'];
            } else {
                $exercicio = ' Geral ';
            }
            if (!$tipoRelatorio) {
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
                   <br><b>Relatório Município Proinfância - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 85% !important;" align="center" >';
            } else {
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

    <br><b>Relatório Município Proinfância - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 85% !important;" align="center" >';
            }
            if ($_REQUEST['situacao'] == WF_PROINFANTIL_ENVIADO_PARA_PAGAMENTO) {
                $codigo = AEDID_PRO_ENCAMINHAR_AGUARDANDO_PAGAMENTO;
            } else if ($_REQUEST['situacao'] == WF_PROINFANTIL_PAGAMENTO_EFETUADO) {
                $codigo = AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO;
            } else {
                $codigo = AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO;
            }

            $sqlUF = "
	SELECT distinct 
							mun.estuf as estuf
						       , estdescricao as descricao
						       
	FROM
				obras2.obras obr
			
			
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = pin.docid AND d1.tpdid = 48
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			--LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = 48
			--LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			INNER JOIN territorios.estado est ON mun.estuf = est.estuf

			LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = '{$codigo}'
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
			
			 WHERE 	   
				obr.obrstatus = 'A' AND ent.entstatus = 'A' AND  obr.obrpercentultvistoria >= 90 AND  pf.prfid = 41  AND esd1.esdid IN (373, 372) AND obr.obridpai IS NULL AND tpl.tpoid in (16,9,10) ";

            if ((is_array($_REQUEST['estuf']) && $_REQUEST['estuf'][0] != '')) {
                $sqlUF .= "  AND  mun.estuf  IN ('" . implode("','", $_REQUEST['estuf']) . "')";
            }

            if ($_REQUEST['anopagamento']) {
                $anoPagamento = "'" . $_REQUEST['anopagamento'] . "'";
                $sqlUF .= " AND anopagamento = '{$_REQUEST['anopagamento']}'";
            } else {
                $anoPagamento = 'NULL';
            }

            if ($_REQUEST['situacao']) {
                $sqlUF .= "AND esd1.esdid  = '{$_REQUEST['situacao']}'";
            }

            $sqlUF .= ' GROUP BY mun.estuf, estdescricao
				 
			 ORDER BY mun.estuf ';

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


            if ($UFs) {
                foreach ($UFs as $key) {
                    extract($_POST);
                    $arWhere = Array("obr.obrstatus = 'A'", "ent.entstatus = 'A'", " obr.obrpercentultvistoria >= 90", " pf.prfid = 41 ", "esd1.esdid IN (373, 372)", "obr.obridpai IS NULL", "tpl.tpoid in (16,9,10)");

                    // UF
                    if ($estuf[0] && $estuf_campo_flag) {
                        $arWhere[] = " mun.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $estuf) . "' ) ";
                    }

                    // Municipio
                    if ($muncod[0] && $muncod_campo_flag) {
                        $arWhere[] = " mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $muncod) . "' ) ";
                    }
                    //Ano do Pagamento
                    if ($anopagamento) {
                        $arWhere[] = "pag.anopagamento = '{$anopagamento}'";
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

                    if (!$anopagamento && $situacao == WF_PROINFANTIL_PAGAMENTO_EFETUADO) {
                        $tratamento2analises = ' limit 1';
                    }

                    if ($situacao) {
                        $arWhere[] = "esd1.esdid  = '{$situacao}'";
                    }

                    $sql = "SELECT DISTINCT
 	estado,
	municipio,
	sum(qnttic) as qnttic , 
	sum(qnttipe) as qnttipe,	
	sum(qnttpc) as qnttpc,
	sum(qnttppe) as qnttppe,
	sum(total_nao) as total_nao,
	sum(vtic) as vtic,
	sum(vtipe) as vtipe,
	sum(vtpc) as vtpc,
	sum(vtppe) as vtppe, 
	sum(qnt_obras) as qnt_obras,
	estado_plano as estado_plano 
FROM ( 	SELECT DISTINCT
 	estado,
	municipio,
	sum(mncintcre) as qnttic , 
	sum(mncintpre) as qnttipe,	
	sum(mncparcre) as qnttpc,
	sum(mncparpre) as qnttppe,
	sum(mncintcre) + sum(mncintpre) + sum(mncparcre) + sum(mncparpre) as total_nao,
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
	END AS vtppe, 
	q_obras as qnt_obras,
	estado_p as estado_plano 
FROM (	

	SELECT 
							mun.estuf as estado
						       , mun.mundescricao as municipio
						       , pin.pinanoseguinte as analiseseguinte
						       , pinpareceraprovacao2
						       , pag.anopagamento
						       , vlr.mncintcre as mncintcre
						       , vlr.mncintpre as mncintpre
						       , vlr.mncparcre as mncparcre
						       , vlr.mncparpre as mncparpre
						       , count(obr.obrid) as q_obras
						       , esd1.esddsc as estado_p
						       ,pin.docid
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
			INNER JOIN workflow.documento d1 ON d1.docid = pin.docid AND d1.tpdid = " . WF_PROINFANTIL . "
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			--LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = " . WF_PROINFANTIL . "
			--LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
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
										hst.aedid = '{$codigo}'
									ORDER BY		
										hst.htddata DESC {$tratamento2analises}
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

                    $sql .= " and mun.estuf ='" . $key[estuf] . "'";
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
								       , esd1.esddsc 
								       ,pin.docid
				 ";

                    $sql .= "
			ORDER BY mun.estuf ,mun.mundescricao, esd1.esddsc ) AS foo group by foo.estado,
	foo.municipio, foo.mncintcre, foo.analiseseguinte, foo.maiordata, foo.valorintcre2, foo.valorintcre,
	foo.valorintpre2, foo.valorintpre, foo.valorparcre2, foo.valorparcre, foo.valorparpre2, foo.valorparpre,foo.q_obras,
	foo.estado_p 

	) AS foo2 
	group by estado,
	municipio,
	estado_plano order by estado,municipio";
                    //ver($sql,d);
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
                    //die($sql);
                    //$db->monta_lista($sql, $cabecalho, 500000, 5, 'N', '90%', 'N');

                    echo '<tr>
                <td style="font-size: 20px;padding: 10px 10px 10px 0" colspan="4">' . $key[descricao] . '</td>
            </tr>
    <tr>
    <td style="text-align: left"><b>UF</td></b> <td style="text-align: left"><b>Município</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Integral / Creche</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Integral / Pré-Escola</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Parcial / Creche</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Parcial / Pré-Escola</b></td>
    <td style="text-align: left"><b>Total de  Matrículas não computadas na escola</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Intregal / Creche (R$)</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Intregal / Pré-Escola (R$)</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Parcial / Creche (R$)</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Parcial / Pré-Escola (R$)</b></td>
    <td style="text-align: left"><b>Valor Total de Repasse (R$)</b></td>
    <td><b>Qtd. de Obras</b></td>
    <td><b>Situação do Plano</b></td></tr>';
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
                            $total_repasse = $value[vtic] + $value[vtipe] + $value[vtpc] + $value[vtppe];
                            $totalqtrg += $total_repasse;
                            $totalqtr += $total_repasse;
                            echo
                            '<tr>
            <td  style="text-align: center">' . $value[estado] . '</td>
            <td style="text-align: center">' . $value[municipio] . '</td>
            <td style="text-align: center">' . $value[qnttic] . '</td>
            <td style="text-align: center">' . $value[qnttipe] . '</td>
            <td style="text-align: center">' . $value[qnttpc] . '</td>
            <td style="text-align: center">' . $value[qnttppe] . '</td>
            <td style="text-align: center">' . $value[total_nao] . '</td>
            <td style="text-align: right">' . number_format($value[vtic], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[vtipe], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[vtpc], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[vtppe], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($total_repasse, 2, ',', '.') . '</td>
            <td style="text-align: center">' . $value[qnt_obras] . '</td>
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
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv1, 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv2, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv3, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv4, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalqtr, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqo . '</b></td>
           </tr>';
                    }
                }
                echo
                '<tr></tr><tr></tr><tr></tr><tr></tr><tr></tr><tr></tr><tr><td colspan="2" style="text-align: right"><b>Total Geral:</b></td><td style="background-color: #cccccc;text-align: center"><b>' . $totalq1g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq2g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq3g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalq4g . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqtg . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv1g, 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv2g, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv3g, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalv4g, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalqtrg, 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: center"><b>' . $totalqog . '</b></td>
           </tr>';
                echo '</table> <br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table></body>';
              
                if ($tipoRelatorio) {
                    // Finaliza processo
                    die('<script>window.close();</script>');
                } else {
                    die();
                }
            } else {
                echo 'Não constam registros para o ano e situação selecionados.';
            }
        }
        ?>
    </body>