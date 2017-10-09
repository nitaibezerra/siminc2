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
                <title> <?php echo NOME_SISTEMA; ?> </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Relatório Município Proinfância Analítico - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 85% !important;" align="center" border="1" >';
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

    <br><b>Relatório Município Proinfância - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 100% !important;" align="center" >';
            }
            if ($_REQUEST['esdid'] == WF_PROINFANTIL_ENVIADO_PARA_PAGAMENTO) {
                $codigo = AEDID_PRO_ENCAMINHAR_AGUARDANDO_PAGAMENTO;
            } else if ($_REQUEST['esdid'] == WF_PROINFANTIL_PAGAMENTO_EFETUADO) {
                $codigo = AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO;
            } else {
                $codigo = AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO;
            }
            
       //ver($_POST,d);
                extract($_POST);
                    $arWhere = Array("obr.obrstatus = 'A'", "ent.entstatus = 'A'", " obr.obrpercentultvistoria >= 90", " pf.prfid = 41 ", "esd1.esdid IN (690, 693)", "obr.obridpai IS NULL", "tpl.tpoid in (16,9,10)");

                    // UF
                    if ($estuf[0] && $estuf_campo_flag) {
                        $arWhere[] = " mun.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $estuf) . "' ) ";
                    }

                    // Municipio
                    if ($muncod[0] && $muncod_campo_flag) {
                        $arWhere[] = " mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $muncod) . "' ) ";
                    }
                    
                    if ($id_obra) {
                        $arWhere[] = " obr.obrid =  {$id_obra}";
                    }
                    
                    //Ano do Pagamento
                    if ($anopagamento) {
                        $arWhere[] = "pag.anopagamento = '{$anopagamento}'";
                        
                        
                        
                    $join_pagamento = "LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = " . $codigo . " 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid";
                    
                    }

                    if (!$anopagamento && $situacao == WF_PROINFANTIL_PAGAMENTO_EFETUADO) {
                        $tratamento2analises = ' limit 1';
                    }
                  
                    //Tipologia
	if($tpoid[0]){
		$arWhere[] = " tpl.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpoid ) . "')";
	}	
	
	// Situação do Plano
	if( $esdid[0] ){
           $arWhere[] = " esd.esdid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "')";
        }

        // Lote
	if( $lotid ){
		$arWhere[] = " lot.lotid = $lotid ";
	}

	//Ano de Inicio do atendimento
	if($anoini){
		$arWhere[] = "to_char(to_date(res.resdsc,'DD/MM/YYYY'),'YYYY') = '{$anoini}'";
	}
	
	//Ano do Pagamento
	if($anopagamento){
		$arWhere[] 			= "pag.anopagamento = '{$anopagamento}'";
		$join_pagamento 	= "LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = ".$codigo." 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid";	
	} else {
		$join_pagamento = "";
	}
	

            $sqlUF = "SELECT distinct ede.estuf AS UF,
    estdescricao as estado
				 FROM
				obras2.obras obr		
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			LEFT JOIN proinfantil.loteproinfancia lot ON lot.lotid = pin.lotid
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			INNER JOIN territorios.estado estt ON ede.estuf = estt.estuf

			LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = 48
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			{$join_pagamento}
			
			
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
WHERE ".( $arWhere[0] ? implode(' AND ', $arWhere) : '' )."
ORDER BY UF, estado";
            
            //ver($sqlUF,d);
            global $db;
            $UFs = $db->carregar($sqlUF);

            if ($UFs) {
                foreach ($UFs as $key) {
                    extract($_POST);
                    $arWhere = Array("obr.obrstatus = 'A'", "ent.entstatus = 'A'", " obr.obrpercentultvistoria >= 90", " pf.prfid = 41 ", "esd1.esdid IN (690, 693)", "obr.obridpai IS NULL", "tpl.tpoid in (16,9,10)");

                    // UF
                    if ($estuf[0] && $estuf_campo_flag) {
                        $arWhere[] = " mun.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $estuf) . "' ) ";
                    }

                    // Municipio
                    if ($muncod[0] && $muncod_campo_flag) {
                        $arWhere[] = " mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '" . implode("', '", $muncod) . "' ) ";
                    }
                    
                    if ($id_obra) {
                        $arWhere[] = " obr.obrid =  {$id_obra}";
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
                  
                    //Tipologia
	if($tpoid[0]){
		$arWhere[] = " tpl.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpoid ) . "')";
	}	
	
	// Situação do Plano
	if( $esdid[0] ){
            $arWhere[] = " esd.esdid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "')";
	}
		
	// Lote
	if( $lotid ){
		$arWhere[] = " lot.lotid = $lotid ";
	}

	//Ano de Inicio do atendimento
	if($anoini){
		$arWhere[] = "to_char(to_date(res.resdsc,'DD/MM/YYYY'),'YYYY') = '{$anoini}'";
	}
	
	//Ano do Pagamento
	if($anopagamento){
		$arWhere[] 			= "pag.anopagamento = '{$anopagamento}'";
		$join_pagamento 	= "LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = ".$codigo." 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid";	
	} else {
		$join_pagamento = "";
	}
	
	//Monta Colunas lent

                    $sql = "SELECT 
       mun.mundescricao AS municipio,
			 vlr.mncintcre
			, vlr.mncintpre
			, vlr.mncparcre
			, vlr.mncparpre
			 ,vlr.mncintcre + vlr.mncintpre + vlr.mncparcre + vlr.mncparpre as totalMatNaoComp
			, vlr.totalintcre
			, vlr.totalintpre
			, vlr.totalparcre
			, vlr.totalparpre
			, vlr.totalintcre + vlr.totalintpre + vlr.totalparcre +  vlr.totalparpre totalMatComp
			, vlr.valorintcre + vlr.valorintcre2 as valorintcre
			, vlr.valorintpre + vlr.valorintpre2 as valorintpre
			, vlr.valorparcre + vlr.valorparcre2 as valorparcre
			, vlr.valorparpre + vlr.valorparpre2 as valorparpre
			, vlr.valorintcre+vlr.valorintpre+vlr.valorparcre+vlr.valorparpre+
							   				vlr.valorintcre2+vlr.valorintpre2+vlr.valorparcre2+vlr.valorparpre2 as total_repasse
			, tpl.tpodsc as tipologia_obra
			, obr.obrpercentultvistoria as percentual_obra
			, esd1.esddsc as estado_obra
			, obr.obrid as id_obra
			, 1 as qtd_obra
			, to_char(to_date(res.resdsc,'DD/MM/YYYY'),'DD/MM/YYYY') as data_inicio_atendimento
			, lotnumportaria as portaria
			,  lotdsc as lote
			, esd.esddsc as estado_plano
			, ede.estuf as estado
			, mun.mundescricao as municipio
				 FROM
				obras2.obras obr		
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			LEFT JOIN proinfantil.loteproinfancia lot ON lot.lotid = pin.lotid
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = 48
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			{$join_pagamento}
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			
			
			
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
  ".( $arWhere[0] ? implode(' AND ', $arWhere) : '' )."
 and mun.estuf ='" . $key[uf] . "'
   
ORDER BY estado,
         municipio";

                        $registros = $db->carregar($sql);
                          $valorintcre  = 0;
                            $valorintpre = 0;
                            $valorparcre = 0;
                            $valorparpre = 0;
                            $total_repasse = 0;

                   echo '<tr>
                <td style="text-align: center; font-size: 20px;padding: 10px 10px 10px 0" colspan="25">' . $key[uf] . ' - ' . $key[estado] . '</td>
            </tr>
    <tr>
    <td style="text-align: center"><b>Município</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Integral / Creche</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Integral / Pré-Escola</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Parcial / Creche</b></td>
    <td style="text-align: left"><b>Qtd. Matrículas não computadas para recebimento do Fundeb - Tempo Parcial / Pré-Escola</b></td>
    <td style="text-align: left"><b>Total de  Matrículas não computadas na escola</b></td>
    <td style="text-align: left"><b>Qtd. de Matrículas na Escola - Tempo Integral/Creche</b></td>
    <td style="text-align: left"><b>Qtd. de Matrículas na Escola - Tempo Integral/Pré-Escola</b></td>
    <td style="text-align: left"><b>Qtd. de Matrículas na Escola - Tempo Parcial/Creche</b></td>
    <td style="text-align: left"><b>Qtd. de Matrículas na Escola - Tempo Parcial/Pré-Escola</b></td>
    <td style="text-align: left"><b>Total de Matrículas na Escola</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Intregal / Creche (R$)</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Intregal / Pré-Escola (R$)</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Parcial / Creche (R$)</b></td>
    <td style="text-align: left"><b>Valor de repasse - Tempo Parcial / Pré-Escola (R$)</b></td>
    <td style="text-align: left"><b>Valor Total de Repasse (R$)</b></td>
    <td><b>Tipologia</b></td>
    <td><b>Percentual de Execução (%)</b></td>
    <td><b>Situação da Obra</b></td>
    <td><b>ID da Obra</b></td>
    <td><b>Qtd. de Obras</b></td>
    <td><b>Data de Início de Atendimento</b></td>
    <td><b>Nº Portaria</b></td>
    <td><b>Nº Lote</b></td>
    <td><b>Situação do Plano</b>
    </td>
    </tr>';
                    if (!empty($registros)) {
                        $total_repasse = 0;
                        foreach ($registros as $value) {
                            $valorintcre += $value[valorintcre];
                            $valorintpre += $value[valorintpre];
                            $valorparcre += $value[valorparcre];
                            $valorparpre += $value[valorparpre];
                            $total_repasse += $value[total_repasse]; 
                            $valorintcreG += $value[valorintcre];
                            $valorintpreG += $value[valorintpre];
                            $valorparcreG += $value[valorparcre];
                            $valorparpreG += $value[valorparpre];
                            $total_repasseG += $value[total_repasse]; 
                            
                            echo
                            '<tr>
            <td style="text-align: center">' . $value[municipio] . '</td>
            <td style="text-align: center">' . $value[mncintcre] . '</td>
            <td style="text-align: center">' . $value[mncintpre] . '</td>
            <td style="text-align: center">' . $value[mncparcre] . '</td>
            <td style="text-align: center">' . $value[mncparpre] . '</td>
            <td style="text-align: center">' . $value[totalmatnaocomp] . '</td>
            <td style="text-align: center">' . $value[totalintcre] . '</td>
            <td style="text-align: center">' . $value[totalintpre] . '</td>
            <td style="text-align: center">' . $value[totalparcre] . '</td>
            <td style="text-align: center">' . $value[totalparpre] . '</td>
            <td style="text-align: center">' . $value[totalmatcomp] . '</td>
            <td style="text-align: right">' . number_format($value[valorintcre], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[valorintpre], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[valorparcre], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[valorparpre], 2, ',', '.') . '</td>
            <td style="text-align: right">' . number_format($value[total_repasse], 2, ',', '.') . '</td>
            <td style="text-align: center">' . $value[tipologia_obra] . '</td>
            <td style="text-align: center">' . $value[percentual_obra] . '</td>
            <td style="text-align: center">' . $value[estado_obra] . '</td>
            <td style="text-align: center">' . $value[id_obra] . '</td>
            <td style="text-align: center">' . $value[qtd_obra] . '</td>
            <td style="text-align: center">' . $value[data_inicio_atendimento] . '</td>
            <td style="text-align: center">' . $value[portaria] . '</td>
            <td style="text-align: center">' . $value[lote] . '</td>
            <td>' . $value[estado_plano] . '</td>
        </tr>';
                        }

                        echo
                        '<tr><td colspan="11" style="background-color: #cccccc;text-align: center"><b>Total do Estado:</b></td>
             <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorintcre  , 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorintpre  , 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorparcre  , 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorparpre  , 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($total_repasse , 2, ',', '.') . '</b></td>
           <td colspan="9" style="background-color: #cccccc;text-align: right"></td></tr>';
                    }
                }
                echo
                 '<tr><td colspan="11" style="background-color: #cccccc;text-align: center"><b>Total Geral:</b></td>
             <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorintcreG  , 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorintpreG  , 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorparcreG  , 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($valorparpreG  , 2, ',', '.') . '</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($total_repasseG , 2, ',', '.') . '</b></td>
           <td colspan="9" style="background-color: #cccccc;text-align: right"></td></tr>';
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