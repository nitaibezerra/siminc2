<?php

function pegaObservacoes( $dopid ){
	
	global $db;
	
	$sql = "SELECT  DISTINCT
				'<label style=color:red ><b> - '||si.icodescricao||' - ( '||
				array_to_string(array(SELECT atpdsc FROM par.acompanhamentotipopendencia tpp 
							INNER JOIN par.acompanhamentotipopendencia_acompanhamentoparecer tppap ON tppap.atpid = tpp.atpid 
							WHERE tppap.acpid = acp.acpid),', ')||' ) '||acpdsc||'</b></label>' as observacoes
			FROM 
				par.subacao s
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano  AND si.icostatus = 'A'
			INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
			INNER JOIN par.subacaoitenscomposicao_acompanhamentoparecer sap ON sap.icoid = si.icoid AND sap.siastatus = 'A' 
			INNER JOIN par.acompanhamentoparecer acp ON acp.acpid = sap.acpid AND acp.acpstatus = 'A' AND acp.acprestritiva IS TRUE AND acp.atpid = 1
			WHERE 
				sd.sbdid IN(
					SELECT DISTINCT  
						CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
							sd.sbdid 
						END
					FROM 
						par.subacao s
					INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
					INNER JOIN par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
					INNER JOIN par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
					WHERE 
						dp.dopid = $dopid
				AND si.icoquantidaderecebida > 0
				)
				AND icomonitoramentocancelado = false";
	
	$arrObservacoes = $db->carregarColuna($sql);
	
	return $arrObservacoes;
}

header('content-type: text/html; charset=ISO-8859-1');

if( ! temPagamento($_SESSION['dopid']))
{
	echo "	<script>
					window.location=\"par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid= {$_SESSION['dopid']}\" ;
			</script>";
}
// Aqui lista a quantidade de itens com nota
$sql = "
	SELECT  
		count( DISTINCT si.icoid ) as qtditenscomnota,
		(	SELECT  
				count( DISTINCT si.icoid )
			FROM
				par.subacao s
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
				INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
				INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
				INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
				LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND epistatus = 'A'
			WHERE 
				sd.sbdid in (
					SELECT DISTINCT  
							sd.sbdid
					FROM
						par.subacao s
					inner join 
						par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
					inner join 
						par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
					inner join 
						par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
					WHERE
						dp.dopid = {$_SESSION['dopid']}
			) 
			AND
				si.icomonitoramentocancelado = FALSE
			AND
				si.icovalidatecnico = 'S'
		) as qtditens
	
	FROM
		par.subacao s
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
		INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
		INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
		INNER JOIN par.subacaoitenscomposicaonotasfiscais nf ON si.icoid = nf.icoid 
		INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
		INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
		LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND epistatus = 'A'
		WHERE 
			sd.sbdid in (
				SELECT DISTINCT  
						sd.sbdid
				FROM
					par.subacao s
				inner join 
					par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
				inner join 
					par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
				inner join 
					par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
				WHERE
					dp.dopid = {$_SESSION['dopid']}
			)
		AND
			si.icomonitoramentocancelado = FALSE
		AND
				si.icovalidatecnico = 'S'
";
	
$result = $db->carregar($sql);

$qtditenscomNota	=	$result[0]['qtditenscomnota'];
$qtditens 			=	$result[0]['qtditens'];

$testaTermoEmReformulacao = verificaTermoEmReformulacao( $_SESSION['dopid'] );

if( $testaTermoEmReformulacao ){
	$finaliza = 'disabled="disabled"';
	$finalizaOk = false;
	$arrPendencias[] = "<label style=\"color:red\">Este termo se encontra em reprogramação.</label>";
}elseif( $qtditenscomNota >= $qtditens )
{
	$finaliza = '';
	$finalizaOk = true;
}
else
{
	$finaliza = 'disabled="disabled"';
	$finalizaOk = false;
	
	
	// 1 - Texto: Há item(ns) sem contrato anexado, na aba “Contratos”. Para finalizar o processo, todos os itens devem ter seus respectivos contratos anexados, ou o item deve ser cancelado.
	$sql1 = "SELECT  
				si.icoid,
				CASE WHEN
					EXISTS(	
						SELECT 
							sit.icoid
						FROM
							
						par.subacaoitenscomposicaoContratos sicon 
						INNER JOIN par.subacaoitenscomposicao sit ON si.icoid = sicon.icoid  AND sit.icostatus = 'A'
					
						where sicon.icoid = si.icoid
					)
				THEN
					1
				ELSE
					0
				END as contratos_item
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid	
			INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
			LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND epistatus = 'A'
			WHERE 
				sd.sbdid in (
					SELECT DISTINCT  
							sd.sbdid
					FROM
						par.subacao s
					inner join 
						par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
					inner join 
						par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
					inner join 
						par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
					WHERE
						dp.dopid = {$_SESSION['dopid']})
					AND
							si.icomonitoramentocancelado = FALSE
					AND
							si.icovalidatecnico = 'S'		
							";

	$result1 = $db->carregar($sql1);
	$pendencia1 = false;
	if(is_array($result1) && count($result1) )
	{
		foreach( $result1 as $k1 => $v1 )
		{
			if( ! $v1['contratos_item'])
			{
				$pendencia1 = true;
			}
		}
	} 
	if($pendencia1)
	{
		$arrPendencias[] = "Há item(ns) sem contrato anexado, na aba “Contratos”. Para finalizar o processo, todos os itens devem ter seus respectivos contratos anexados, ou o item deve ser cancelado.";
	}
 
	///2 - Texto: Existem itens com contrato que não foi informado a quantidade recebida na aba de "Monitoramento".
	//- Há item(ns) com contrato(s) anexado(s) que não informaram a "Quantidade recebida" na aba de "Monitoramento".

	$sql2 = "
		SELECT  
			(CASE WHEN sbacronograma = 1 THEN
                               ( SELECT DISTINCT
                                               CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
                               FROM par.subacaoitenscomposicao sic 
                               WHERE sic.sbaid = s.sbaid
                               AND sic.icoano = sd.sbdano
                               and sic.icoid = si.icoid
                               AND sic.icostatus = 'A'
                               GROUP BY sic.sbaid, sic.icovalidatecnico )
                ELSE 
                               ( SELECT DISTINCT
                                                               CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
                                                                              THEN -- escolas sem itens
                                                                                              sum(coalesce(se.sesquantidadetecnico,0))
                                                                              ELSE -- escolas com itens
                                                                                              CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
                                                                                                              sum(coalesce(ssi.seiqtdtecnico,0))
                                                                                              END
                                                                              END
                                                               as vlrsubacao
                                               FROM entidade.entidade t
                                               inner join entidade.funcaoentidade f on f.entid = t.entid
                                               left join entidade.entidadedetalhe ed on t.entid = ed.entid
                                               inner join entidade.endereco d on t.entid = d.entid
                                               left join territorios.municipio m on m.muncod = d.muncod
                                               left join par.escolas e on e.entid = t.entid
                                               INNER JOIN par.subacaoescolas se ON se.escid = e.escid
                                               INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano  AND sic.icostatus = 'A'
                                               LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
                                               WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano

                                               and sic.icoid = si.icoid
                                               and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
                                               GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
                END ) as quantidade,
			CASE WHEN si.icoquantidaderecebida > 0  
				THEN si.icoquantidaderecebida
				ELSE 0
			END as adicionado
		FROM 
			par.subacao s
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
		INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
		INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano  AND si.icostatus = 'A'
		INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
		INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
		LEFT  JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
		WHERE sd.sbdid in (
			SELECT DISTINCT  
					sd.sbdid
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			INNER JOIN par.termocomposicao 	tc ON tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			INNER JOIN par.documentopar 	dp ON dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			WHERE
				dp.dopid = {$_SESSION['dopid']})
				AND icomonitoramentocancelado = false
				AND (CASE WHEN sbacronograma = 1 THEN
	                               ( SELECT DISTINCT
	                                               CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
	                               FROM par.subacaoitenscomposicao sic 
	                               WHERE sic.sbaid = s.sbaid
	                               AND sic.icoano = sd.sbdano
	                               and sic.icoid = si.icoid
                                       AND sic.icostatus = 'A'
	                               GROUP BY sic.sbaid, sic.icovalidatecnico )
	                ELSE 
	                               ( SELECT DISTINCT
	                                                               CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
	                                                                              THEN -- escolas sem itens
	                                                                                              sum(coalesce(se.sesquantidadetecnico,0))
	                                                                              ELSE -- escolas com itens
	                                                                                              CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
	                                                                                                              sum(coalesce(ssi.seiqtdtecnico,0))
	                                                                                              END
	                                                                              END
	                                                               as vlrsubacao
	                                               FROM entidade.entidade t
	                                               inner join entidade.funcaoentidade f on f.entid = t.entid
	                                               left join entidade.entidadedetalhe ed on t.entid = ed.entid
	                                               inner join entidade.endereco d on t.entid = d.entid
	                                               left join territorios.municipio m on m.muncod = d.muncod
	                                               left join par.escolas e on e.entid = t.entid
	                                               INNER JOIN par.subacaoescolas se ON se.escid = e.escid
	                                               INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
	                                               LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
	                                               WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
	
	                                               and sic.icoid = si.icoid
	                                               and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
	                                               GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
	                END ) IS NOT NULL";
	///2 - Texto: 

	$result2 = $db->carregar($sql2);
	$pendencia2 = false;
	
	if(is_array($result2) && count($result2) )
	{
		foreach( $result2 as $k2 => $v2 )
		{
			if( $v2['adicionado'] == 0 )
			{
				$pendencia2 = true;
			}
		}
	} 
	if($pendencia2)
	{
		$arrPendencias[] = "Há item(ns) com contrato(s) anexado(a) para o(s)  qual(is) não foi informada a \"Quantidade recebida\", na aba \"Monitoramento\".";
	}

	//3 - Texto: Existem itens que não foram detalhados na aba "Detalhamento do Serviço/Itens"
	//4 - Texto: A quantidade detalhada do item/serviço é menor que a recebida na aba de "Detalhamento do Serviço/Itens". 
	$sql3 = "SELECT  DISTINCT
				si.icoid,
				si.icoquantidaderecebida   as recebida,
				(select count(icoid) from par.detalhamentoquantidadeitens where icoid = si.icoid ) as detalhada
			FROM par.subacao s
			INNER JOIN par.subacaodetalhe 					sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao 					tc ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao 			si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			INNER JOIN par.subacaoitenscomposicaocontratos 	sicc ON  si.icoid = sicc.icoid
			INNER JOIN par.propostaitemcomposicao 			pic ON pic.picid = si.picid
			LEFT  JOIN par.empenhopregaoitensenviados 		emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
	WHERE 
		s.sbaid in (
			SELECT DISTINCT  
					s.sbaid
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			INNER JOIN par.termocomposicao 	tc ON tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			INNER JOIN par.documentopar 	dp ON dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			WHERE
				dp.dopid = {$_SESSION['dopid']}
		)
		AND si.icoquantidaderecebida > 0
		AND si.icomonitoramentocancelado = FALSE
		AND si.icovalidatecnico = 'S'
		AND
			(SELECT 
				CASE WHEN ( COUNT( foo.sbaid ) = COUNT(foo.onibus) ) AND ( COUNT( foo.sbaid ) > 0 ) THEN 1 -- somente onibus
					WHEN ( COUNT( foo.sbaid ) > COUNT(foo.onibus) ) AND ( COUNT( foo.sbaid ) > 0 ) AND (COUNT(foo.onibus) > 0) THEN 2	
					ELSE 3
				END as onibus
			FROM (
					select   
						( SELECT  s1.sbaid  FROM par.subacao s1 WHERE s1.sbaid = sa.sbaid and s1.prgid in (81,50,153,164,158)) as onibus,
						 sa.sbaid
					from par.documentopar  dp
					inner join  par.termocomposicao tc on tc.dopid = dp.dopid
					inner join par.subacaodetalhe sd on sd.sbdid = tc.sbdid
					inner join par.subacao sa on sa.sbaid = sd.sbaid
					WHERE dp.dopid = {$_SESSION['dopid']}
					AND sa.sbaid = s.sbaid
				) as foo
			) IN ( 1,2 )";

	$result3 = $db->carregar($sql3);
	$pendencia3 = false;
	$pendencia3_1 = false;

	//3 - Texto: Existem itens que não foram detalhados na aba "Detalhamento do Serviço/Itens"
	//4 - Texto: A quantidade detalhada do item/serviço é menor que a recebida na aba de "Detalhamento do Serviço/Itens". 
	if(is_array($result3) && count($result3) )
	{
		
		foreach( $result3 as $k3 => $v3 )
		{
			if( $v3['detalhada'] == 0 )
			{
				$pendencia3 = true;
			}
			elseif( $v3['detalhada'] < $v3['recebida']  )
			{
				$pendencia3_1 = true;
			}
		}
	} 
	if($pendencia3)
	{
		$arrPendencias[] = "Existem itens que não foram detalhados na aba \"Detalhamento do Serviço/Itens\"";
	}
	if($pendencia3_1)
	{
		$arrPendencias[] = "A quantidade detalhada do item/serviço é menor que a recebida na aba de \"Detalhamento do Serviço/Itens\"";
	}
		
	$arrPendencias[] = "Há item(ns) sem nota fiscal anexada, na aba “Notas Fiscais”. Para finalizar o processo, todos os itens devem ter suas respectivas notas fiscais anexadas, ou o item deve ser cancelado.";
	
        #Contratos
        $sqlContratoDiligencia = "SELECT DISTINCT
                            '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location.href=\'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid='||dop.dopid||'\'\" border=\"0\">' as acao,
                            dop.dopnumerodocumento as numero_termo,
                            par.retornacodigosubacao(sd.sbaid) as subacao,
                            con.connumerocontrato,
                            con.condiligencia
                        FROM par.processopar pp 
                        INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = pp.prpid
                        INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = pp.prpid AND ppc.ppcstatus = 'A'
                        INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid 
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                        INNER JOIN par.subacaoitenscomposicaocontratos sicc ON sicc.icoid = sic.icoid AND sicc.sccstatus = 'A'
                        INNER JOIN par.contratos con ON con.conid = sicc.conid AND con.constatus = 'A' AND con.condiligencia IS NOT NULL
                        WHERE dop.dopid = {$dopid}";
                $arrContratoDiligencia = $db->carregar($sqlContratoDiligencia);
                $arrContratoDiligencia = ($arrContratoDiligencia) ? $arrContratoDiligencia : array();
        if ($arrContratoDiligencia) {
            $arrPendencias[] = "Há item(ns) sem nota fiscal anexadaExistem Contratos em Diligência.";
        }
        
        #Nota Fiscal
        $sqlNFDiligencia = "SELECT DISTINCT
                                    '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location.href=\'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid='||dop.dopid||'\'\" border=\"0\">' as acao,
                                    dop.dopnumerodocumento as numero_termo,
                                    par.retornacodigosubacao(sd.sbaid) as subacao,
                                    ntf.ntfnumeronotafiscal,
                                    ntf.ntfdiligencia
                                FROM par.processopar pp 
                                INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = pp.prpid
                                INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = pp.prpid AND ppc.ppcstatus = 'A'
                                INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid 
                                INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                                INNER JOIN par.subacaoitenscomposicaonotasfiscais sicntf ON sicntf.icoid = sic.icoid AND sicntf.scnstatus = 'A'
                                INNER JOIN par.notasfiscais ntf ON ntf.ntfid = sicntf.ntfid AND ntf.ntfstatus = 'A' AND ntf.ntfdiligencia IS NOT NULL
                                WHERE dop.dopid = {$dopid}";
        $arrNFDiligencia = $db->carregar($sqlNFDiligencia);
        $arrNFDiligencia = ($arrNFDiligencia) ? $arrNFDiligencia : array();
        if ($arrNFDiligencia) {
            $arrPendencias[] = "Existem Notais Fiscais em Diligência.";
        }
}

echo'<br>';

if( $perfilTipoAcompanhamento == 3 )
{
	$arrAbas =	array
	(	 
		0 => array( "descricao" => "Contratos", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
		1 => array( "descricao" => "Monitoramento", 				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
		2 => array( "descricao" => "Notas fiscais",					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
		3 => array( "descricao" => "Pendências/Finalizar",							"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
	);
}
else 
{
	$arrAbas =	array
	(	 
		0 => array( "descricao" => "Contratos", 						"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
		1 => array( "descricao" => "Monitoramento", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
		2 => array( "descricao" => "Detalhamento do Serviço / Item",	"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=detalhamentoItem&dopid=" . $_SESSION['dopid']),
		3 => array( "descricao" => "Notas fiscais",						"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
		4 => array( "descricao" => "Pendências/Finalizar",				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
	);
}

if( temPermissaoAnaliseFNDE() ){
	array_push($arrAbas, array( "descricao" => "Analise FNDE",				
								"link"	  	=> "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoAnaliseFNDE&dopid=" . $_SESSION['dopid']));
}

echo montarAbasArray( $arrAbas, "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'] );

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
print '<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Finalizar monitoramento</label></td></tr><tr>';
print '<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> '.$local.' </b></td>';
print '<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>';
print '<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'; 
?>
	<img src="../imagens/icone_lupa.png" style="cursor:pointer;" title="Visualizar Termo" onclick="carregaTermoMinuta('<?=$dopid; ?>');" border="0">
<?php 
print getNumDoc( $dopid ) .'</td>';
print '<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" ></td></tr>';
print '<td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>
</b></td></tr>';
if( ! $finalizaOk )
{
	print '<tr><td bgcolor="#F5F5F5" colspan="2" ><b> Pendencias</b> </td>
			<td colspan="2" bgcolor="#F5F5F5" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >';
				
		foreach($arrPendencias as $k => $pendencia )
		{
			print '- ' . $pendencia . '<br>';
		}
	
	print	'</td></tr>';
}

$arrObservacoes = pegaObservacoes( $_SESSION['dopid'] );

if( count($arrObservacoes) > 1 ){
	$finaliza = 'disabled="disabled"';
	$finalizaOk = false;
}

//validar se pode 
?>
	<tr>
		<td bgcolor="#F5F5F5" colspan="2" ><b>Observações</b></td>
		<td colspan="2" bgcolor="#F5F5F5" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
			<?
			if( count($arrObservacoes) > 0 ){
				echo implode('<br>',$arrObservacoes); 
			}else{
				echo 'Não possui';
			}
			?>
		</td>
	</tr>
</table>
<script>
</script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <style type="">
    .ui-dialog-titlebar{
    text-align: center;
    }
    </style>

<div id="debug"></div>
<script type="text/javascript">

function voltar()
{
	location.href='par.php?modulo=principal/administracaoDocumentos&acao=A';
}

function finalizarMonitoramento( dopid )
{

	if(confirm('Deseja realmente finalizar o acompanhamento?')){
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
	   		data: "requisicao=finalizaAcompanhamento&finalizaOk=<?=$finalizaOk ?>&dopid=" + dopid,
	   		async: false,
	   		success: function(msg){
   				alert(msg); 
   				location.href='par.php?modulo=principal/administracaoDocumentos&acao=A'
	   		}
		});
	}
	
}



</script>
<?php
if( ( ! bloqueiaParaConsultaEstMun() ) && (getFinalizado() != 'finalizado' ) )
{ 
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;"><input ' . $finaliza . 'type="button" onclick="finalizarMonitoramento(' . $_SESSION['dopid'] . ')" value="Finalizar monitoramento"></label></td></tr><tr>';
	print '</tr></table>';
}
?>
