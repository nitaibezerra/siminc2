<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select distinct dp.dopid, dp.prpid, md.mdoid, md.mdonome
		from par.documentopar dp 
            inner join par.modelosdocumentos md on md.mdoid = dp.mdoid
            inner join par.termocomposicao tc on tc.dopid = dp.dopid and tc.sbdid is not null
		where 
        	md.tpdcod in (21, 102) --and md.mdostatus = 'A'
            and dp.prpid is not null
			--and md.mdoid in (52, 56)
		order by dp.dopid";

$arrItenSubacao = $db->carregar($sql);
$arrItenSubacao = $arrItenSubacao ? $arrItenSubacao : array();

$arSubEmpEstado 		= array(32, 60, 55, 54, 28, 59);
$arSubEmpMunnicipio		= array(30, 31, 46, 53);
$arUnivPacto 			= array(71, 34, 40);
$arExeArticuladasEst 	= array(38, 47, 64, 59, 42, 68, 21, 19, 39, 70);
$arExeArticuladasMun 	= array(29, 45, 69, 66, 41, 65, 22, 43, 81, 51, 63, 20, 23);

foreach ($arrItenSubacao as $key => $v) {
	
	//$arrSub = $db->carregarColuna("select sbdid from par.termocomposicao where dopid = {$v['dopid']} and sbdid is not null");
	
	$sql = "SELECT
			    sd.sbaid as subacao,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    sic.icoid
			FROM
			    par.processopar prp
			    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			    inner join par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao pic ON pic.picid = sic.picid
			WHERE
			    prp.prpid = {$v['prpid']}
			    and prp.prpstatus = 'A'
			    and sic.icostatus = 'A'
			    and sic.icovalidatecnico = 'S'
			    and sd.sbdid in (select sbdid from par.termocomposicao where dopid = {$v['dopid']} and sbdid is not null)
			GROUP BY
			    sd.sbaid,
			    pic.picpregao,
			    sic.icoid";
	
	$arItens = $db->carregar($sql);
	$arItens = $arItens ? $arItens : array();
	
	/*if( $v['mdoid'] == 3 ){		
		$arItens = termo_Compromisso( $v['prpid'] );		
	} 
	elseif( in_array( $v['mdoid'], $arExeArticuladasEst ) ){
		$arItens = tabela_Extrato_execucao_plano_acoes_articuladas_estados($arrSub, $v['prpid']);
	}
	elseif( $v['mdoid'] == 52 || $v['mdoid'] == 56 ){
		$arItens = tabela_Termo_Compromisso_Universidades_Brasil_Pro( $arrSub, $v['prpid'] );
	}
	elseif( $v['mdoid'] == 16 ){
		$arItens = talela_Identificacao_Ente_Federado( $v['prpid'] );
	}
	elseif( in_array( $v['mdoid'], $arSubEmpEstado ) ){
		$arItens = tabela_subacao_empenho_estado( $arrSub, $v['prpid'] );
	}
	elseif( in_array( $v['mdoid'], $arUnivPacto ) ){
		$arItens = tabela_Termo_Compromisso_Universidades_Pacto( $arrSub, $v['prpid'] );
	}
	elseif( in_array( $v['mdoid'], $arExeArticuladasMun ) ){
		$arItens = tabela_Extrato_execucao_plano_acoes_articuladas_municipios( $arrSub, $v['prpid'] );
	}
	elseif( in_array( $v['mdoid'], $arSubEmpMunnicipio ) ){
		$arItens = tabela_subacao_empenho_municipio( $arrSub, $v['prpid'] );
	}*/
	
	if( is_array($arItens) && !empty($arItens[0]) ){
		foreach ($arItens as $item) {
			
			$total = $db->pegaUm("select count(idpid) from par.termocomposicaoitens where dopid = {$v['dopid']} and icoid = {$item['icoid']} and prpid = {$v['prpid']}");
			
			if( (int)$total == (int)0 ){
				$sql = "INSERT INTO par.termocomposicaoitens(dopid, icoid, prpid, idppregao, idpstatus)
						VALUES ({$v['dopid']},
								{$item['icoid']},
								{$v['prpid']},
								'{$item['pregao']}',
								'A'
						)";			
				$db->executar($sql);
			}
		}
		$db->commit();
	}
}
$db->close();

function termo_Compromisso( $prpid ){
	global $db;
	
	$sql = "SELECT sic.icodescricao as descricao,
				sic.icovalor as valor,
				sic.icoquantidade as quantidade,
				case when pic.picpregao = true then 'S' else 'N' end as pregao,
				sic.icoid,
				(sic.icovalor * sic.icoquantidade) as total
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			INNER JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = es.eobano
			inner join par.propostaitemcomposicao   pic ON pic.picid = sic.picid
			WHERE p.prpid = $prpid AND icostatus = 'A' and p.prpstatus = 'A'";
	
	$arItens = $db->carregar( $sql );
	$arItens = $arItens ? $arItens : array();
	
	return $arItens;
}

function talela_Identificacao_Ente_Federado( $prpid ){
	global $db;
	
	$sql = "SELECT  
				e.empid,
			    es.sbaid,
			    s.sbadsc,
			    sic.icoid,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    sic.icodescricao,
			    sic.icovalor as valor
			FROM par.empenho e
			    INNER JOIN par.empenhosubacao es  ON e.empid =  es.empid and eobstatus = 'A'
			    INNER JOIN par.processopar pp ON pp.prpnumeroprocesso = e.empnumeroprocesso and pp.prpstatus = 'A'
			    INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
			    INNER JOIN par.subacao s ON s.sbaid = es.sbaid
			    INNER JOIN par.subacaodetalhe sd  ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			    inner JOIN par.subacaoitenscomposicao sic ON sic.sbaid         = s.sbaid   AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao        pic ON pic.picid = sic.picid
			    LEFT  JOIN par.subacaoescolas se  ON se.sbaid           = sic.sbaid AND se.sesano = sic.icoano
			WHERE 
				empstatus <> 'I' and
				pp.prpid = $prpid
			GROUP BY e.empid,
			    es.sbaid,
			    s.sbadsc,
			    sic.icoid,
			    pic.picpregao,
			    sic.icodescricao,
			    sic.icovalor
				";
	
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	return $arDadosItem;
}

function tabela_Termo_Compromisso_Universidades_Brasil_Pro( $arrSub, $prpid ){
	global $db;
	
	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "select
			    pic.picdescricao,
			    sic.icoid,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    pic.picid,
			    sd.sbdano as sbdano,
			    s.sbaid,
			    sic.icovalor as valor
			from
			    par.processopar pp
			    inner join par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe 			sd ON sd.sbdid = ppc.sbdid
			    inner join par.subacao       			s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
			    inner join par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao        pic ON pic.picid = sic.picid
			    left  join par.subacaoescolas 			se on se.sbaid = s.sbaid
			WHERE
			    {$whereSubacao} 
			    s.sbastatus = 'A' AND
			    pp.prpid = $prpid AND
			    pp.prpstatus = 'A' and 
			    CASE WHEN sbacronograma = 1
			         THEN sic.icovalidatecnico <> 'N'
			         ELSE
			               CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
			                     se.sesvalidatecnico <> 'N'
			               ELSE
			                     sic.icovalidatecnico <> 'N'
			               END
			         END
			group by
				pic.picdescricao,
			    sic.icoid,
			    pic.picpregao,
			    pic.picid,
			    sd.sbdano,
			    s.sbaid,
			    sic.icovalor";
	//ver($sql, d);
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	return $arDadosItem;
}

function tabela_subacao_empenho_estado( $arrSub, $prpid ){
	global $db;
	
	if( $prpid == 3389 ){
		$sql = "SELECT
				    sub.sbadsc as subacao,
				    case when pic.picpregao = true then 'S' else 'N' end as pregao,
				    sic.icoid
				FROM
					par.processopar prp
				    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
				    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
				    left join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
				    inner join par.subacao sub on sub.sbaid = sd.sbaid
				    inner  JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sub.sbaid AND sic.icoano = sd.sbdano
				    inner join par.propostaitemcomposicao pic ON pic.picid = sic.picid
				WHERE
				    prp.prpid = $prpid
				    and prp.prpstatus = 'A'
				    and sd.sbdid in (".implode(',', $arrSub).")
				    and sub.sbastatus = 'A'
				GROUP BY
				    sub.sbadsc,
				    pic.picpregao,
				    sic.icoid";

	} else {
		$sql = "SELECT
				    sub.sbadsc as subacao,
				    case when pic.picpregao = true then 'S' else 'N' end as pregao,
				    sic.icoid
				FROM
				    par.processopar prp
				    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
				    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
				    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
				    inner join par.subacao sub on sub.sbaid = ems.sbaid
				    inner  JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sub.sbaid AND sic.icoano = sd.sbdano
				    inner join par.propostaitemcomposicao pic ON pic.picid = sic.picid
				WHERE
				    prp.prpid = $prpid
				    and prp.prpstatus = 'A'
				    and sd.sbdid in (".implode(',', $arrSub).")
				    and sub.sbastatus = 'A'
				GROUP BY
				    sub.sbadsc,
				    pic.picpregao,
				    sic.icoid";
	}

	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	return $arDadosItem;
}

function tabela_subacao_empenho_municipio( $arrSub, $prpid ){
	global $db;
	
	$sql = "SELECT
			    sub.sbaid as subacao,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    sic.icoid
			FROM
			    par.processopar prp
			    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
			    inner join par.subacao sub on sub.sbaid = ems.sbaid
			    inner  JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sub.sbaid AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao pic ON pic.picid = sic.picid
			WHERE
			    prp.prpid = $prpid
			    and prp.prpstatus = 'A'
			    and sd.sbdid in (".implode(',', $arrSub).")
			    and sub.sbastatus = 'A'
			GROUP BY
			    sub.sbaid,
			    pic.picpregao,
			    sic.icoid";
	ver($sql,d);
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
}

function tabela_Extrato_execucao_plano_acoes_articuladas_estados( $arrSub, $prpid ){
	global $db;
	
	if( is_array($arrSub) && $arrSub[0] ){
		
            $whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
            $sqlCronograma = "
                            SELECT sd.sbdid, sbacronograma from par.subacaodetalhe sd 
                            INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                            where $whereSubacao true";
            $sbdCronograma = $db->carregar($sqlCronograma);

            $arrSbdGlobal = array();
            $arrSbdEscola = array();

            foreach($sbdCronograma as $k => $infoSbd)
            {
                $cronograma = $infoSbd['sbacronograma'];
                $sbdAtual = $infoSbd['sbdid'];

                if($cronograma == 1)
                {
                    $arrSbdGlobal[] = $sbdAtual;
                }
                else
                {
                    $arrSbdEscola[] = $sbdAtual;
                }
            } 
	}
	
	if( count($arrSbdEscola) > 0 )
	{
		
		$whereSubacaoEscola = " sd.sbdid in (".implode(',', $arrSbdEscola).") and ";
		$sql = "select
			    pic.picdescricao,
			    sic.icoid,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    pic.picid,
			    sd.sbdano as sbdano,
			    s.sbaid,
			    sic.icovalor as valor
			from
			    par.processopar pp
			    inner join par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe 			sd ON sd.sbdid = ppc.sbdid
			    inner join par.subacao       			s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
			    inner join par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao        pic ON pic.picid = sic.picid
			    left  join par.subacaoescolas 			se on se.sbaid = s.sbaid
			WHERE
			    {$whereSubacaoEscola} 
			    s.sbastatus = 'A' AND
			    pp.prpid = $prpid AND
			    pp.prpstatus = 'A' and 
			    CASE WHEN sbacronograma = 1
			         THEN sic.icovalidatecnico <> 'N'
			         ELSE
			               CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
			                     se.sesvalidatecnico <> 'N'
			               ELSE
			                     sic.icovalidatecnico <> 'N'
			               END
			         END
			group by
				pic.picdescricao,
			    sic.icoid,
			    pic.picpregao,
			    pic.picid,
			    sd.sbdano,
			    s.sbaid,
			    sic.icovalor";
	    $arDadosItemEscola = $db->carregar($sql);    	
	}
	if( count($arrSbdGlobal) > 0)
	{
		$whereSubacaoGlobal = " sd.sbdid in (".implode(',', $arrSbdGlobal).") and ";
		$sql = "SELECT
				    pic.picdescricao,
				    pic.picid,
				    sd.sbdano as sbdano,
				    sic.icoid,
				    case when pic.picpregao = true then 'S' else 'N' end as pregao,
				    s.sbaid
				FROM
				    par.processopar pp
				    INNER JOIN par.processoparcomposicao 		ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
				    INNER JOIN par.subacaodetalhe 				sd  ON sd.sbdid = ppc.sbdid
				    INNER JOIN par.subacao       				s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
				    inner JOIN par.subacaoitenscomposicao   	sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano              	
				    INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
				WHERE
				    {$whereSubacaoGlobal} 
				    s.sbastatus = 'A' AND
				    pp.prpid = $prpid AND
				    pp.prpstatus = 'A' and 
				    sic.icostatus = 'A' AND 
				    sic.icovalidatecnico <> 'N'                   		
				GROUP BY
				    pic.picdescricao,
				    pic.picid,
				    sd.sbdano,
				    sic.icoid,
				    pic.picpregao,
				    s.sbaid";
	    $arDadosItemGlobal = $db->carregar($sql);    	
	}
	$arDadosItemEscola = $arDadosItemEscola ? $arDadosItemEscola : array();
	$arDadosItemGlobal = $arDadosItemGlobal ? $arDadosItemGlobal : array();
	
	$arDadosItem = array_merge($arDadosItemGlobal, $arDadosItemEscola);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	return $arDadosItem;
}

function tabela_Extrato_execucao_plano_acoes_articuladas_municipios( $arrSub, $prpid ){
	global $db;
	
	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT
			    pic.picdescricao,
			    pic.picid,
			    sic.icoid,
			    sd.sbdano as sbdano,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    s.sbaid
			FROM
			    par.processopar pp
			    inner join par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe 			sd  ON sd.sbdid = ppc.sbdid
			    inner join par.subacao       			s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
			    inner join par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
			    left  join par.subacaoescolas 			se ON se.sbaid = s.sbaid AND se.sesstatus = 'A'
			    inner join par.propostaitemcomposicao        pic ON pic.picid = sic.picid
			WHERE
			    {$whereSubacao} 
			    s.sbastatus = 'A' AND
			    pp.prpid = $prpid AND
			    pp.prpstatus = 'A' and
			    CASE WHEN sbacronograma = 1 THEN sic.icovalidatecnico <> 'N'
			    ELSE
			    	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
			    		se.sesvalidatecnico <> 'N'
			    	ELSE
			    		sic.icovalidatecnico <> 'N'
			    	END
			    END
			GROUP BY
			    pic.picdescricao,
			    pic.picid,
			    sic.icoid,
			    sd.sbdano,
			    pic.picpregao,
			    s.sbaid";
                                        
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	return $arDadosItem;
}

function tabela_Termo_Compromisso_Universidades_Pacto( $arrSub, $prpid ){
	global $db;
	
	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT
			    sub.sbaid as subacao,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    sic.icoid
			FROM
			    par.processopar prp
			    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
			    inner join par.subacao sub on sub.sbaid = ems.sbaid
			    inner join par.subacaoitenscomposicao sic ON sic.sbaid = sub.sbaid AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao pic ON pic.picid = sic.picid
			WHERE
				$whereSubacao
				prp.prpid = $prpid
				and prp.prpstatus = 'A'
                and sub.sbastatus = 'A'
			GROUP BY
			    sub.sbaid,
			    pic.picpregao,
			    sic.icoid";
	
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	return $arDadosItem;
}