<?php

function monta_coluna_empenho(){

    $coluna = array();

    array_push( $coluna, array("campo" 	 => "data",
        "label" 	 => "Data") );

    array_push( $coluna, array("campo" 	 => "n_empenho",
            "label" 	 => "Nº de empenho" )
    );

    array_push( $coluna, array("campo" 	 => "cnpj",
            "label" 	 => "CNPJ" )
    );

    array_push( $coluna, array("campo" 	 => "n_contrato",
            "label" 	 => "Nº do Contrato" )
    );

    array_push( $coluna, array("campo" 	 => "nome_fantasia",
            "label" 	 => "Nome Fantasia" )
    );

    array_push( $coluna, array("campo" 	 => "descricao",
            "label" 	 => "Descrição" )
    );

    array_push( $coluna, array("campo" 	 => "valor",
            "label" 	 => "Valor" )
    );

    return $coluna;

}

function monta_coluna_ob(){

    $coluna = array();

    array_push( $coluna, array("campo" 	 => "data",
        "label" 	 => "Data") );

    array_push( $coluna, array("campo" 	 => "n_empenho",
            "label" 	 => "Nº da OB" )
    );

    array_push( $coluna, array("campo" 	 => "cnpj",
            "label" 	 => "CNPJ" )
    );

    array_push( $coluna, array("campo" 	 => "n_contrato",
            "label" 	 => "Nº do Contrato" )
    );

    array_push( $coluna, array("campo" 	 => "nome_fantasia",
            "label" 	 => "Nome Fantasia" )
    );

    array_push( $coluna, array("campo" 	 => "descricao",
            "label" 	 => "Descrição" )
    );

    array_push( $coluna, array("campo" 	 => "valor",
            "label" 	 => "Valor" )
    );

    return $coluna;

}

function monta_coluna_nf(){

    $coluna = array();

    array_push( $coluna, array("campo" 	 => "data",
        "label" 	 => "Data") );

    array_push( $coluna, array("campo" 	 => "n_empenho",
            "label" 	 => "Nº da NF" )
    );

    array_push( $coluna, array("campo" 	 => "cnpj",
            "label" 	 => "CNPJ" )
    );

    array_push( $coluna, array("campo" 	 => "n_contrato",
            "label" 	 => "Nº do Contrato" )
    );

    array_push( $coluna, array("campo" 	 => "nome_fantasia",
            "label" 	 => "Nome Fantasia" )
    );

    array_push( $coluna, array("campo" 	 => "descricao",
            "label" 	 => "Descrição" )
    );

    array_push( $coluna, array("campo" 	 => "valor",
            "label" 	 => "Valor" )
    );

    return $coluna;

}

function monta_agp_empenho(){

    $coluna = array();

    if ( in_array('numcontrato', $_REQUEST['agrupador']) ):
        array_push( $coluna, array("campo" 	 => "ctrdtiniciovig",
            "label" 	 => "Início contrato") );

        array_push( $coluna, array("campo" 	 => "ctrdtfimvig",
            "label" 	 => "Fim contrato") );
    endif;

// 	array_push( $coluna, array("campo" 	 => "valor_original",
// 							   "label" 	 => "Valor original") );

// 	array_push( $coluna, array("campo" 	 => "valor_aditivo",
// 							   "label" 	 => "Valor aditivo") );

    array_push( $coluna, array("campo" 	 => "valor_contrato",
        "label" 	 => "(A)<br>Valor do Contrato") );

    array_push( $coluna, array("campo" 	 => "vlr_empenho",
            "label" 	 => "(B)<br>Valor Empenhado",
            "php"	 => array(
                "expressao" => "'{nivelagrupador}' == 'numcontrato'",
                "type"		=> "string",
// 							   						"blockAgp"  => array(
// 							   											 "tipo",
// 							   											 "contratante",
// 							   											 "contratado",
// 							   											 "modalidade",
// 							   											 "situacaocontrato",
// 							   											 "possuitermoaditivo",
// 							   											 "mesanoiniciovigcontrato",
// 							   											 "mesanofimvigcontrato",
// 							   											 "estarea"
// 							   											),
                "var"		=> "html_empenho",
                "true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&amp;acao=A&amp;requisicao=popUpEmpenho&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar dados do empenho\" class=\"notprint\" style=\"float:left\">
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">
															        </a>",
                "false"		=> "",
                "html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_empenho}&nbsp;&nbsp;{vlr_empenho}</div>"
            )
        )
    );

    array_push( $coluna, array("campo" 	 => "valor_nf_cadastradas",
            "label" 	 => "(C)<br>Valor das Notas Ficais Cadastradas",
            "php"	 => array(
                "expressao" => "'{nivelagrupador}' == 'numcontrato'",
                "type"		=> "string",
                "var"		=> "html_nf",
                "true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as notas fiscais cadastradas\" class=\"notprint\" style=\"float:left\">
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">
															        </a>",
                "false"		=> "",
                "html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_nf}&nbsp;&nbsp;{valor_nf_cadastradas}</div>"
            )
        )
    );

    array_push( $coluna, array("campo" 	 	  => "glosa",
        "label" 	  => "(D)<br>Glosa") );

    array_push( $coluna, array("campo" 	 => "valor_nf_pagas",
        "label" 	 => "(E)<br>Valor das Notas Ficais Pagas",
        "php"	 => array(
            "expressao" => "'{nivelagrupador}' == 'numcontrato'",
            "type"		=> "string",
            "var"		=> "html_nfpaga",
            "true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&nf=paga&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as notas fiscais pagas\" class=\"notprint\" style=\"float:left\">
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">
															        </a>",
            "false"		=> "",
            "html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_nfpaga}&nbsp;&nbsp;{valor_nf_pagas}</div>"
        )
    ) );

    array_push( $coluna, array("campo" 	 => "valor_executado",
        "label" 	 => "(F)<br>Valor das Ordens Bancárias",
        "php"	 => array(
            "expressao" => "'{nivelagrupador}' == 'numcontrato'",
            "type"		=> "string",
            "var"		=> "html_ob",
            "true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaOB&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as ordens bancárias\" class=\"notprint\" style=\"float:left\">
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">
															        </a>&nbsp;&nbsp;",
            "false"		=> "",
            "html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_ob}{valor_executado}</div>"
        )
    ) );

    array_push( $coluna, array("campo" 	 	   => "({valor_contrato} - ({valor_nf_cadastradas} - {glosa}))",
        "label" 	   => "(G)<br>Saldo do Contrato<br>(G) = (A) - (C - D)",
        "expressaophp" => true
    ) );



// 	array_push( $coluna, array("campo" 	 	  => "valor_total",
// 							   "label" 	 	  => "Valor total",
// 							) );


// 	array_push( $coluna, array("campo" 	 	  => "valor_retencao",
// 								"label" 	  => "Valor retido",
// 								) );


// 	array_push( $coluna, array("campo" 	 => "valor_pago",
// 							   "label" 	 => "Valor pago") );

    return $coluna;

}

function contratos_monta_coluna_relatorio(){

	$coluna = array();

	if ( in_array('numcontrato', $_REQUEST['agrupador']) ):
		array_push( $coluna, array("campo" 	 => "ctrdtiniciovig",
								   "label" 	 => "Início contrato") );
		
		array_push( $coluna, array("campo" 	 => "ctrdtfimvig",
								   "label" 	 => "Fim contrato") );
	endif;

// 	array_push( $coluna, array("campo" 	 => "valor_original",
// 							   "label" 	 => "Valor original") );

// 	array_push( $coluna, array("campo" 	 => "valor_aditivo",
// 							   "label" 	 => "Valor aditivo") );

	array_push( $coluna, array("campo" 	 => "valor_contrato",
							   "label" 	 => "(A)<br>Valor do Contrato") );
	
	array_push( $coluna, array("campo" 	 => "vlr_empenho",
							   "label" 	 => "(B)<br>Valor Empenhado",
							   "php"	 => array(
							   						"expressao" => "'{nivelagrupador}' == 'numcontrato'",
							   						"type"		=> "string",
// 							   						"blockAgp"  => array(
// 							   											 "tipo",
// 							   											 "contratante",
// 							   											 "contratado",
// 							   											 "modalidade",
// 							   											 "situacaocontrato",
// 							   											 "possuitermoaditivo",
// 							   											 "mesanoiniciovigcontrato",
// 							   											 "mesanofimvigcontrato",
// 							   											 "estarea"
// 							   											),
													"var"		=> "html_empenho",
													"true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&amp;acao=A&amp;requisicao=popUpEmpenho&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar dados do empenho\" class=\"notprint\" style=\"float:left\">			        					
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">					
															        </a>",
													"false"		=> "",
													"html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_empenho}&nbsp;&nbsp;{vlr_empenho}</div>"
												  )
							  )
				);
	
	array_push( $coluna, array("campo" 	 => "valor_nf_cadastradas",
							   "label" 	 => "(C)<br>Valor das Notas Ficais Cadastradas",
							   "php"	 => array(
							   						"expressao" => "'{nivelagrupador}' == 'numcontrato'",
							   						"type"		=> "string",
													"var"		=> "html_nf",
													"true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as notas fiscais cadastradas\" class=\"notprint\" style=\"float:left\">			        					
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">					
															        </a>",
													"false"		=> "",
													"html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_nf}&nbsp;&nbsp;{valor_nf_cadastradas}</div>"
												  )
							  ) 
				);
	
	array_push( $coluna, array("campo" 	 	  => "glosa",
								"label" 	  => "(D)<br>Glosa") );
	
	array_push( $coluna, array("campo" 	 => "valor_nf_pagas",
							   "label" 	 => "(E)<br>Valor das Notas Ficais Pagas",
							   "php"	 => array(
							   						"expressao" => "'{nivelagrupador}' == 'numcontrato'",
							   						"type"		=> "string",
													"var"		=> "html_nfpaga",
													"true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&nf=paga&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as notas fiscais pagas\" class=\"notprint\" style=\"float:left\">			        					
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">					
															        </a>",
													"false"		=> "",
													"html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_nfpaga}&nbsp;&nbsp;{valor_nf_pagas}</div>"
												  )
								) );
	
	array_push( $coluna, array("campo" 	 => "valor_executado",
							   "label" 	 => "(F)<br>Valor das Ordens Bancárias",
							   "php"	 => array(
							   						"expressao" => "'{nivelagrupador}' == 'numcontrato'",
							   						"type"		=> "string",
													"var"		=> "html_ob",
													"true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaOB&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as ordens bancárias\" class=\"notprint\" style=\"float:left\">			        					
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">					
															        </a>&nbsp;&nbsp;",
													"false"		=> "",
													"html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_ob}{valor_executado}</div>"
												  )
								) );
	
	array_push( $coluna, array("campo" 	 	   => "({valor_contrato} - ({valor_nf_cadastradas} - {glosa}))",
								"label" 	   => "(G)<br>Saldo do Contrato<br>(G) = (A) - (C - D)",
								"expressaophp" => true
								) );

	
	
// 	array_push( $coluna, array("campo" 	 	  => "valor_total",
// 							   "label" 	 	  => "Valor total",
// 							) );
	
		
// 	array_push( $coluna, array("campo" 	 	  => "valor_retencao",
// 								"label" 	  => "Valor retido",
// 								) );	

	
// 	array_push( $coluna, array("campo" 	 => "valor_pago",
// 							   "label" 	 => "Valor pago") );

	return $coluna;
}


function contratos_monta_agp_relatorio(){

	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
			"agrupador" => array(),
			"agrupadoColuna" => array(
									  "valor_contrato",
									  "tipo",
									  "ctrdtiniciovig",
									  "ctrdtfimvig",
// 									  "contratante",
// 									  "contratado",
// 									  "modalidade",
// 									  "situacaocontrato",
// 									  "valor_original",
// 									  "valor_aditivo",
// 									  "valor_total",
									  "glosa",
									  "vlr_empenho",
// 									  "valor_retencao",
									  "valor_nf_cadastradas",
									  "valor_nf_pagas",
									  "valor_executado",
									  "ctrid",
									  "entidcontratada")
	);

	foreach ( $agrupador as $val ){
		switch( $val ){
			case "tipo":
				array_push($agp['agrupador'], array(
				"campo" => "tipo",
				"label" => "Tipo")
				);
				break;
			case "numcontrato":
				array_push($agp['agrupador'], array(
				"campo" => "numcontrato",
				"label" => "Nº do contrato")
				);
				break;
			case "contratante":
				array_push($agp['agrupador'], array(
				"campo" => "contratante",
				"label" => "Contratante")
				);
				break;
			case "contratado":
				array_push($agp['agrupador'], array(
				"campo" => "contratado",
				"label" => "Contratado")
				);
				break;
			case "modalidade":
				array_push($agp['agrupador'], array(
				"campo" => "modalidade",
				"label" => "Modalidade")
				);
				break;
			case "situacaocontrato":
				array_push($agp['agrupador'], array(
				"campo" => "situacaocontrato",
				"label" => "Situação do contrato")
				);
				break;
			case "possuitermoaditivo":
				array_push($agp['agrupador'], array(
				"campo" => "possuitermoaditivo",
				"label" => "Aditado?")
				);
				break;
			case "mesanoiniciovigcontrato":
				array_push($agp['agrupador'], array(
				"campo" => "mesanoiniciovigcontrato",
				"label" => "Início contrato")
				);
				break;
			case "mesanofimvigcontrato":
				array_push($agp['agrupador'], array(
				"campo" => "mesanofimvigcontrato",
				"label" => "Fim contrato")
				);
				break;
			case "estarea":
				array_push($agp['agrupador'], array(
				"campo" => "estarea",
				"label" => "Interessado")
				);
				break;

		}
	}

	return $agp;

}

function contratos_monta_sql_relatorio(){
	global $arHspidPermitido;
	
	$where = array();

	extract($_REQUEST);

	// Interessado
	$whereInteressado = '';
	if( $estid[0] && $estid_campo_flag ){
		$whereInteressado = " cr.estid " . (!$estid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estid ) . "') ";
		$joinInteressado = "JOIN ( SELECT
										DISTINCT
										ctrid
								   FROM contratos.contratosetorresponsavel cr
								   WHERE
										{$whereInteressado}) cr ON cr.ctrid = ctr.ctrid";
		$whereInteressado = " AND " . $whereInteressado;
	}
		
	// Contratante
	if( $hspid[0] && $hspid_campo_flag ){
		if ( count( $arHspidPermitido ) ){
			$hspid = array_intersect($arHspidPermitido, $hspid);
			$hspid = ($hspid ? $hspid : array(0));
		}
		array_push($where, " ctr.hspid " . (!$hspid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $hspid ) . "') ");
	}elseif ( $arHspidPermitido ){
		array_push($where, " ctr.hspid  IN ('" . implode( "','", $arHspidPermitido ) . "') ");
	}
	
	// Contratado
	if( $entid[0] && $entid_campo_flag ){
		array_push($where, " ctr.entidcontratada " . (!$entid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entid ) . "') ");
	}
	
	// Modalidade
	if( $modid[0] && $modid_campo_flag ){
		array_push($where, " ctr.modid " . (!$modid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $modid ) . "') ");
	}
	
	// Tipo Contrato
	if( $tpcid[0] && $tpcid_campo_flag ){
		array_push($where, " ctr.tpcid " . (!$tpcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpcid ) . "') ");
	}
	
	// Vigência
	if( $sitid[0] && $sitid_campo_flag ){
		array_push($where, " sit.sitid " . (!$sitid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sitid ) . "') ");
	}
	

	if ( $filtro_ctrobj ){
		array_push($where, " ctr.ctrobj ILIKE '%" . $filtro_ctrobj . "%'");
	}
	
	if ( $filtro_ctrnum ){
		array_push($where, " ctr.ctrnum = " . $filtro_ctrnum);
	}
	
	if ( $filtro_ctrano ){
		array_push($where, " ctr.ctrano ILIKE '%" . $filtro_ctrano . "%'");
	}
	
	if ( $filtro_ctrnummod ){
		array_push($where, " ctr.ctrnummod ILIKE '%" . $filtro_ctrnummod . "%'");
	}
	
	if ( $filtro_ctrproccontr ){
		array_push($where, " ctr.ctrproccontr ILIKE '%" . $filtro_ctrproccontr . "%'");
	}
	
	if ( $filtro_ctrprocexecctr ){
		array_push($where, " ctr.ctrprocexecctr ILIKE '%" . $filtro_ctrprocexecctr . "%'");
	}
	
	if ( $filtro_ctrprocexecfin ){
		array_push($where, " ctr.ctrprocexecfin ILIKE '%" . $filtro_ctrprocexecfin . "%'");
	}
	
	$dtinicio = formata_data_sql($dtinicio); 
	$dtfim	  = formata_data_sql($dtfim);
	
	if ( $dtinicio && $dtfim ){
		array_push($where, " ctr.ctrdtiniciovig::DATE BETWEEN '{$dtinicio}' AND '{$dtfim}'");
	}elseif ( $dtinicio ){
		array_push($where, " ctr.ctrdtiniciovig::DATE > '{$dtinicio}'");
	}elseif ( $dtfim ){
		array_push($where, " ctr.ctrdtiniciovig::DATE < '{$dtfim}'");
	}

	$dtinicio2 = formata_data_sql($dtinicio2);
	$dtfim2	   = formata_data_sql($dtfim2);
	
	if ( $dtinicio2 && $dtfim2 ){
		array_push($where, " ctr.ctrdtfimvig::DATE BETWEEN '{$dtinicio2}' AND '{$dtfim2}'");
	}elseif ( $dtinicio2 ){
		array_push($where, " ctr.ctrdtfimvig::DATE > '{$dtinicio2}'");
	}elseif ( $dtfim2 ){
		array_push($where, " ctr.ctrdtfimvig::DATE < '{$dtfim2}'");
	}

	// monta o sql
	$sql = "SELECT 
				DISTINCT
				ctr.ctrid,
				ctr.entidcontratada,
				tpc.tpcdsc as tipo, 
				h.hspabrev || ' - ' || h.hspdsc as contratante,
				COALESCE(ent.entnome, 'Não indicada') as contratado,
				mod.moddsc as modalidade,
/*			
				COALESCE(ctr.ctrvlrinicial, 0) as valor_original,			 
				(SELECT 
					(COALESCE(SUM(tdavlr), 0)) AS total
				 FROM contratos.cttermoaditivo adi
				 WHERE 
					adi.tdastatus = 'A' AND 
					adi.ctrid = ctr.ctrid) AS valor_aditivo,
*/
				(SELECT 
					COALESCE( COALESCE(ctr.ctrvlrinicial, 0) + (COALESCE(SUM(tdavlr), 0)), 0) AS total
				 FROM contratos.cttermoaditivo adi
				 WHERE 
					adi.tdastatus = 'A' AND 
					adi.ctrid = ctr.ctrid) AS valor_contrato,			
				
				(SELECT 
								COALESCE(SUM(retir+retcsll+retcofins+retpasep+retoutro), 0) AS retido
							FROM contratos.faturacontrato ftc
							WHERE 
								ftc.ftcstatus = 'A' AND 
								ftc.ctrid = ctr.ctrid) AS valor_retencao,	
				(SELECT 
								COALESCE(SUM(COALESCE(ftcvalor,0)-COALESCE(ftcglosa,0)), 0) AS pago
							FROM contratos.faturacontrato ftc
							INNER JOIN workflow.documento d ON ftc.docid=d.docid
							INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
							WHERE 
								d.esdid =".ESTADO_WK_FATURAMENTO_PAGO." AND 
								ftc.ftcstatus = 'A' AND 
								ftc.ctrid = ctr.ctrid) AS valor_nf_pagas,
				(SELECT 
								COALESCE(SUM(COALESCE(ftcvalor,0)-COALESCE(ftcglosa,0)), 0) AS pago
							FROM contratos.faturacontrato ftc
							WHERE 
								ftc.ftcstatus = 'A' AND 
								ftc.ctrid = ctr.ctrid) AS valor_nf_cadastradas,					
			   (
				   SELECT 
				   	COALESCE(SUM(a.total), 0)
				   FROM (
					   		SELECT
								obs.valor AS total
							FROM
								contratos.ctcontrato c
							JOIN entidade.entidade e ON e.entid = c.entidcontratada	
							JOIN contratos.hospital h ON h.hspid = c.hspid AND
										     			 h.hspstatus = 'A'
							JOIN contratos.hospitalug hu ON hu.hspid = h.hspid 
							JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid			     
							JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
															   es.ungcod = hu.ungcod AND
															   es.co_favorecido = e.entnumcpfcnpj
							JOIN contratos.ob_siafi obs ON obs.empenho = es.nu_empenho AND
													       obs.unidade = hu.ungcod AND
													       obs.it_co_credor = e.entnumcpfcnpj
							WHERE
								c.ctrid = ctr.ctrid AND
								obs.ob NOT IN (
												SELECT 
													obf.obfnumero 
												FROM 
													contratos.faturacontrato fc
												JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
												WHERE 
													fc.ftcstatus = 'A' AND
													fc.ctrid = ctr.ctrid
												)
					   	UNION ALL	
							SELECT 
								COALESCE(SUM(obfvalor), 0) AS total
							FROM contratos.faturacontrato ftc
							JOIN contratos.ordembancariafatura obf on ftc.ftcid = obf.ftcid
							WHERE 
								ftc.ftcstatus = 'A' AND 
								ftc.ctrid = ctr.ctrid
					) AS a				
				) AS valor_executado,
				( 
					SELECT COALESCE(SUM(ftcglosa), 0) AS total FROM contratos.faturacontrato ftc WHERE ftc.ftcstatus = 'A'
								  AND ftc.ctrid = ctr.ctrid
				)as glosa,
				(
					SELECT 
						COALESCE(SUM(valor), 0) AS vlr_empenho 
					FROM
						contratos.empenhovinculocontrato ec		 
					JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid
					WHERE 
						ec.ctrid = ctr.ctrid
				) AS vlr_empenho,			
							
				COALESCE(sit.sitdsc, 'Não indicada') AS situacaocontrato,
							
				(SELECT 
					CASE 
						WHEN count(*) > 0 THEN 'Sim' 
						ELSE 'Não' 
					END as possui 
				 FROM contratos.cttermoaditivo 
				 WHERE 
					tdastatus='A' AND 
					ctrid=ctr.ctrid) AS possuitermoaditivo,
							
				to_char(ctrdtiniciovig, 'mm/YYYY') AS mesanoiniciovigcontrato,
				to_char(ctrdtfimvig, 'mm/YYYY') AS mesanofimvigcontrato,
				to_char(ctrdtiniciovig, 'dd/mm/YYYY') AS ctrdtiniciovig,
				to_char(ctrdtfimvig, 'dd/mm/YYYY') AS ctrdtfimvig,
				'Nº: ' || COALESCE(ctr.ctrnum::varchar(100), '-') || '/' || COALESCE(ctr.ctrano::varchar(100), '-') AS numcontrato,
				ARRAY_TO_STRING( ARRAY( SELECT 
											'- ' || estareasigla || ' - ' || estarea 
										FROM 
											contratos.contratosetorresponsavel cr				
											LEFT JOIN corporativo.estrutura et ON et.estid = cr.estid AND 
												  								  et.eststatus = 'A'  
										WHERE 
											cr.ctrid = ctr.ctrid {$whereInteressado} ), '<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' ) AS estarea
			FROM contratos.ctcontrato ctr	
			{$joinInteressado}
	        LEFT JOIN entidade.entidade ent ON ent.entid = ctr.entidcontratada
	        LEFT JOIN contratos.cttipocontrato tpc ON tpc.tpcid = ctr.tpcid
	        LEFT JOIN contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
	        LEFT JOIN contratos.ctsituacaocontrato sit on ctr.sitid = sit.sitid
			LEFT JOIN contratos.hospital h ON h.hspid = ctr.hspid AND 
											  h.hspstatus = 'A'			
	        --LEFT JOIN contratos.contratante ung on ctr.ungid = ung.ungid
	        WHERE 
				ctr.ctrstatus = 'A' ".(($where) ? " AND " . implode(" AND ", $where) : "");
			//dbg($sql,d);

	return $sql;
}


set_time_limit(0);
ini_set("memory_limit", "1024M");

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
    $existe_rel = 0;
    $sql = sprintf(
        "select prtid from public.parametros_tela where prtdsc = '%s'",
        $_REQUEST['titulo']
    );
    $existe_rel = $db->pegaUm( $sql );
    if ($existe_rel > 0)
    {
        $sql = sprintf(
            "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
            $_REQUEST['titulo'],
            addslashes( addslashes( serialize( $_REQUEST ) ) ),
            $_SESSION['usucpf'],
            $_SESSION['mnuid'],
            $existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = 'contratos.php?modulo=relatorio/relatorio_geral&acao=A';
	</script>
	<?
	die;
}

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';


// instancia a classe de relatório
$rel = new montaRelatorio();
$relEmpenho = new montaRelatorio();
$relOB = new montaRelatorio();
$relNF = new montaRelatorio();

//monta empenho
$colunaEmpenho = monta_coluna_empenho();
$agrupadorEmpenho = monta_agp_empenho();
$sqlEmpenho = contratos_monta_sql_relatorio();

//monta OB
$colunaOB = monta_coluna_ob();


//monta NF
$colunaNF = monta_coluna_nf();


// monta o sql, agrupador e coluna do relatório
$sql       = contratos_monta_sql_relatorio();
$agrupador = contratos_monta_agp_relatorio();
$coluna    = contratos_monta_coluna_relatorio();

$dados 	   = $db->carregar($sql);

switch ($_REQUEST['tipoRelatorio']) {
    case 'empenho':
        $relEmpenho->setColuna($colunaEmpenho);
        $relEmpenho->setAgrupador($agrupadorEmpenho, $dados);
        $rel->setAgrupador([], $dados);
    break;
    case 'ob':
        $relOB->setColuna($colunaOB);
    break;
    case 'nf':
        $relNF->setColuna($colunaNF);
    break;
    default:
        $rel->setEspandir(true);
        $rel->setAgrupador($agrupador, $dados);
        $rel->setColuna($coluna);
        $rel->setTotNivel(true);
        $rel->setTotalizador(true);
        $rel->setTolizadorLinha(true);
        $rel->setMonstrarTolizadorNivel(true);
    break;
}



//dbg($rel->getAgrupar(), 1);

// Gera o XLS do relatório
if ( $_REQUEST['tipoRelatorio'] == 'xls' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title>SIG - Sistema de Informações Gerenciais da EBSERH</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php

                switch ($_REQUEST['tipoRelatorio']) {
                    case 'empenho':
                        echo monta_cabecalho_relatorio( '95' );
                        echo $relEmpenho->getRelatorio();
                        break;
                    case 'ob':
                        echo "ob";
                        break;
                    case 'nf':
                        echo "nf";
                        break;
                    default:
                        echo monta_cabecalho_relatorio( '95' );
                        echo $rel->getRelatorio();
                        break;
                }


                die();

            ?>
		
	</body>
</html>