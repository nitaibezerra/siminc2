<?php

class RelatorioAtendimentoEstado extends Modelo {

    public $sql;
    public $data;
    public $agrupador;
    public $coluna;
    public $estuf;
    public $titulo;

    function __construct($estuf) {
        $this->estuf = $estuf;
    }

    public function montarData() {
        
        $sql = $this->montarSQLTransferenciaVoluntaria();;

        return $this->carregar($sql);
    }

    public function montarSQLTransferenciaVoluntaria() {
        
    	if( $this->estuf ){
    		$where[] = "estado IN ('{$this->estuf}')";
    	}
    	 
    	if( $_POST['dimensao'][0] && ( $_POST['dimensao_campo_flag'] || $_POST['dimensao_campo_flag'] == '1' )){
    		$where[] = " dimid " . (( $_POST['dimensao_campo_excludente'] == null || $_POST['dimensao_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['dimensao'] ) . "') ";
    	}
    	 
    	if( $_POST['areas'][0] && ( $_POST['areas_campo_flag'] || $_POST['areas_campo_flag'] == '1' )){
    		$where[] = " ardid " . (( $_POST['areas_campo_excludente'] == null || $_POST['areas_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['areas'] ) . "') ";
    	}
    	 
    	if( $_POST['indicador'][0] && ( $_POST['indicador_campo_flag'] || $_POST['indicador_campo_flag'] == '1' )){
    		$where[] = " indid " . (( $_POST['indicador_campo_excludente'] == null || $_POST['indicador_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['indicador'] ) . "') ";
    	}
    	 
    	if( $_POST['programa'][0] && ( $_POST['programa_campo_flag'] || $_POST['programa_campo_flag'] == '1' )){
    		$where[] = " prgid " . (( $_POST['programa_campo_excludente'] == null || $_POST['programa_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['programa'] ) . "') ";
    	}	
    
        $sql = "SELECT DISTINCT 
        			rgfnumprocessosape, estado, numeroconvenio, descricaosubacao, coalesce(valorconvenioporsub, 0) as valorconvenioporsub, valorempenhado, valorpago, porcentagemexecucaoconv,
					porcentagemrepasse, porcentagemexecucao, coalesce(saldoglobalconta, 0) as saldoglobalconta, ultimadataconta, dcodatainicio, prsfimvigencia 
        		FROM par.dm_rel_atendimento_estados_transf_voluntaria ".( $where[0] != '' ? "WHERE ".implode(' AND ', $where) : '' )." ORDER BY estado, dcodatainicio";

        return $sql;
    }

    public function montarSQLAssistenciaTecnica() {
        
    	$where = Array("s.frmid IN (4,6)");
    	
    	if( $this->estuf ){
    		$where[] = "iu.estuf IN ('{$this->estuf}')";
    	}
    	
        if( $_POST['dimensao'][0] && ( $_POST['dimensao_campo_flag'] || $_POST['dimensao_campo_flag'] == '1' )){
			$where[] = " d.dimid " . (( $_POST['dimensao_campo_excludente'] == null || $_POST['dimensao_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['dimensao'] ) . "') ";		
		}
    	
        if( $_POST['areas'][0] && ( $_POST['areas_campo_flag'] || $_POST['areas_campo_flag'] == '1' )){
			$where[] = " ad.ardid " . (( $_POST['areas_campo_excludente'] == null || $_POST['areas_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['areas'] ) . "') ";		
		}
    	
        if( $_POST['indicador'][0] && ( $_POST['indicador_campo_flag'] || $_POST['indicador_campo_flag'] == '1' )){
			$where[] = " i.indid " . (( $_POST['indicador_campo_excludente'] == null || $_POST['indicador_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['indicador'] ) . "') ";		
		}
    	
        if( $_POST['programa'][0] && ( $_POST['programa_campo_flag'] || $_POST['programa_campo_flag'] == '1' )){
			$where[] = " prg.prgplanointerno " . (( $_POST['programa_campo_excludente'] == null || $_POST['programa_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['programa'] ) . "') ";		
		}

		$sql = "SELECT
					estuf AS estado,
		            codigo || ' - ' || sbadsc AS descricaosubacao,
		            unddsc AS unidademedida,
		            sum(quantidadePorEscolaPlanejado + quantidadeGlobalPlanejado) as planejado, -- valordasubacao
		            sum(quantidadePorEscolaAprovado + quantidadeGlobalAprovado) AS aprovado,
		            sum(quantidadePorEscolaEmanalise + quantidadeGlobalEmanalise) AS emanalise,
		            sum(COALESCE(dstqtdmonitorada,'0')::integer) AS qtdmonitoramento,
		     		CASE WHEN (dstqtdpainel IS NULL ) OR (dstqtdpainel = '') THEN '0' ELSE dstqtdpainel END as qtdpainel,
		            dstindicadorpainel AS qtdindcadorpainel,
		            COALESCE(dstqtdoutrafonte,'0') AS qtdoutrafonte,
		            dstfonte AS qtdfonte,
		            0 AS valor,  sbaid, prgdsc as programa
				FROM (  -- POR ESCOLA --
		            SELECT           
            			iu.estuf,
                        s.sbaid,
                        s.sbadsc,
                        u.unddsc,
                        dst.dstqtdpainel,
                        dst.dstindicadorpainel,
                        dst.dstqtdoutrafonte,
                        dst.dstfonte,
                        0 as quantidadeGlobalPlanejado,
                        0 as quantidadeGlobalAprovado,
                        0 as quantidadeGlobalEmanalise,
                        sum(coalesce(qf.qfaqtd,0)) as quantidadePorEscolaPlanejado,
                        CASE WHEN spt.ssuid = 3  THEN sum(coalesce(qf.qfaqtd,0)) ELSE 0 END as quantidadePorEscolaAprovado,
                        CASE WHEN spt.ssuid = 1  THEN sum(coalesce(qf.qfaqtd,0)) ELSE 0 END as quantidadePorEscolaEmanalise,
                        d.dimcod || '.' || ad.ardcod || '.' || i.indcod || '.' || COALESCE(s.sbaordem, 0) as codigo,
                        dst.dstqtdmonitorada,
                        prg.prgdsc
		            FROM cte.dimensao d
					INNER JOIN cte.areadimensao 			ad 	ON ad.dimid = d.dimid
					INNER JOIN cte.indicador 				i 	ON i.ardid = ad.ardid
					INNER JOIN cte.criterio 				c 	ON c.indid = i.indid
					INNER JOIN cte.pontuacao 				p 	ON p.crtid = c.crtid AND p.indid = i.indid
					INNER JOIN cte.instrumentounidade 		iu	ON iu.inuid = p.inuid
					INNER JOIN cte.acaoindicador 			a 	ON a.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador 		s 	ON s.aciid = a.aciid
					INNER JOIN cte.unidademedida 			u 	ON u.undid = s.undid
					INNER JOIN cte.qtdfisicoano 			qf  ON qf.sbaid = s.sbaid 
					INNER JOIN cte.dadossubacaotecnica 		dst ON dst.sbaid = s.sbaid
					INNER JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid AND qf.qfaano = spt.sptano
					INNER JOIN cte.programa 				prg ON prg.prgid = s.prgid
					--INNER JOIN financeiro.planointerno 		pi  ON pi.plicod = p.prgplanointerno
		            WHERE
                         s.sbaporescola = true -- por escola.
                         AND ".implode(' AND ', $where)."
		            GROUP BY 
                        iu.estuf,
                        s.sbaid,
                        s.sbadsc,
                        u.unddsc,
                        spt.ssuid,
                        dst.dstqtdpainel,
						dst.dstindicadorpainel,
                        dst.dstqtdoutrafonte,
                        dst.dstfonte,
                        d.dimcod, 
                        ad.ardcod,
                        i.indcod,
                        s.sbaordem,
                        dst.dstqtdmonitorada, prg.prgdsc
		            UNION ALL
		            -- GLOBAL --
		            SELECT           
						iu.estuf,
		                s.sbaid,
		                s.sbadsc,
		                u.unddsc,
		                dst.dstqtdpainel,
		                dst.dstindicadorpainel,
		                dst.dstqtdoutrafonte,
		                dst.dstfonte,
		                sum(coalesce(spt.sptunt,0)) as quantidadeGlobalPlanejado,
		                CASE WHEN spt.ssuid = 3  THEN sum(coalesce(spt.sptunt,0)) ELSE 0 END as quantidadeGlobalAprovado,
		                CASE WHEN spt.ssuid = 1  THEN sum(coalesce(spt.sptunt,0)) ELSE 0 END as quantidadeGlobalEmanalise,
		                0 as quantidadePorEscolaPlanejado,
		                0 as quantidadePorEscolaAprovado,
		                0 as quantidadePorEscolaEmanalise,
		                d.dimcod || '.' || ad.ardcod || '.' || i.indcod || '.' || COALESCE(s.sbaordem, 0) as codigo,
		                dst.dstqtdmonitorada,
		                prg.prgdsc
		            FROM cte.dimensao d
					INNER JOIN cte.areadimensao 			ad 	ON ad.dimid = d.dimid
					INNER JOIN cte.indicador 				i 	ON i.ardid = ad.ardid
					INNER JOIN cte.criterio 				c 	ON c.indid = i.indid
					INNER JOIN cte.pontuacao 				p 	ON p.crtid = c.crtid AND p.indid = i.indid AND p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade 		iu 	ON iu.inuid = p.inuid
					INNER JOIN cte.acaoindicador 			a 	ON a.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador 		s 	ON s.aciid = a.aciid
					INNER JOIN cte.unidademedida 			u 	on u.undid = s.undid
					INNER JOIN cte.dadossubacaotecnica 		dst ON dst.sbaid = s.sbaid
					INNER JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid 
					INNER JOIN cte.programa 				prg ON prg.prgid = s.prgid
					--INNER JOIN financeiro.planointerno 		pi  ON pi.plicod = p.prgplanointerno
		            WHERE
						s.sbaporescola = false -- global.
						AND ".implode(' AND ', $where)."
		            GROUP BY 
            			iu.estuf,
                    	s.sbaid,
                       	s.sbadsc,
                       	u.unddsc, spt.ssuid,
                       	dst.dstqtdpainel,
                        dst.dstindicadorpainel,
                        dst.dstqtdoutrafonte,
                        dst.dstfonte,
                        d.dimcod, 
                        ad.ardcod,
                        i.indcod,
                        s.sbaordem,
                        dst.dstqtdmonitorada, prg.prgdsc
		) AS foo
		GROUP BY      
		            estuf, sbadsc, unddsc, dstqtdpainel, dstindicadorpainel, dstqtdoutrafonte, dstfonte, codigo,  sbaid, programa 
		ORDER BY 
		            estuf, codigo, sbadsc";

        return $sql;
    }

    public function montarSQLTransferenciasLegaisOutrosProgramas() {
        
    	$where1 = Array("sehstatus <> 'I'", "i.indpublicado is true", "dpeanoref >= '2008'");
    	$where2 = Array();
    	
    	if( $this->estuf ){
    		$where1[] = "dshuf IN ('{$this->estuf}')";
    		$where2[] = "iu.estuf IN ('{$this->estuf}')";
    	}
    	
        if( $_POST['indicador'][0] && ( $_POST['indicador_campo_flag'] || $_POST['indicador_campo_flag'] == '1' )){
			$where1[] = " i.indid " . (( $_POST['indicador_campo_excludente'] == null || $_POST['indicador_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_POST['indicador'] ) . "') ";
			$hwere2[] = "1=0";		
		}
    	
		$sql = "SELECT
					*
				FROM ( 
					SELECT
						i.indid as codindicador,
						i.indnome as indicador,
						dpeanoref as ano,
						dpeordem as ordem,
						dpe.dpedsc as periodo,
						dshuf as estado,
						sec.secdsc as responsavel,
						aca.acadsc || ' - Valor de apoio repassado aos estados'  as acao,
						ume.umedesc as unidademedida,
						'Não se aplica' as detalhe,
						sum(dshqtde) as valor
					FROM
						painel.indicador i
					INNER JOIN painel.seriehistorica 				sh  ON sh.indid = i.indid
					INNER JOIN painel.detalheseriehistorica 		dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade 			dpe ON sh.dpeid = dpe.dpeid
					INNER JOIN territorios.estado 					est ON est.estuf = dsh.dshuf
					INNER JOIN painel.secretaria 					sec ON i.secid = sec.secid
					INNER JOIN painel.acao 							aca ON i.acaid = aca.acaid
					INNER JOIN painel.unidademeta 					ume ON i.umeid = ume.umeid
					LEFT  JOIN painel.detalhetipodadosindicador 	tid ON tid.tidid = dsh.tidid1
					WHERE
						i.indid in (471) 
						AND ".implode(' AND ', $where1)."
					GROUP BY
						i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
					
					UNION ALL
				
					SELECT 
						i.indid as codindicador,
						i.indnome as indicador,
						dpeanoref as ano,
						dpeordem as ordem,
						dpe.dpedsc as periodo,
						dshuf as estado,
						sec.secdsc as responsavel,
						aca.acadsc || ' - Repasse a estados' as acao,
						ume.umedesc as unidademedida,
						'Não se aplica' as detalhe,
						sum(dshvalor) as valor
					FROM
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					inner join territorios.estado est on est.estuf = dsh.dshuf
					inner join painel.secretaria sec on i.secid = sec.secid
					inner join painel.acao aca on i.acaid = aca.acaid
					inner join painel.unidademeta ume on i.umeid = ume.umeid
					left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					WHERE
						i.indid in (297)
						AND tidid = 474
						AND ".implode(' AND ', $where1)."
					GROUP BY
						i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
					
					UNION ALL
				
					SELECT 
						i.indid as codindicador,
						i.indnome as indicador,
						dpeanoref as ano,
						dpeordem as ordem,
						dpe.dpedsc as periodo,
						dshuf as estado,
						sec.secdsc as responsavel,
						aca.acadsc || ' - Repasse a estados' as acao,
						ume.umedesc as unidademedida,
						tiddsc as detalhe,
						sum(dshvalor) as valor
					FROM
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					inner join territorios.estado est on est.estuf = dsh.dshuf
					inner join painel.secretaria sec on i.secid = sec.secid
					inner join painel.acao aca on i.acaid = aca.acaid
					inner join painel.unidademeta ume on i.umeid = ume.umeid
					left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					WHERE
						i.indid in (677)
						AND dsh.tidid2 in (2472)
						AND ".implode(' AND ', $where1)."
					GROUP BY
						i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
					
					UNION ALL
				
					SELECT 
						i.indid as codindicador,
						i.indnome as indicador,
						dpeanoref as ano,
						dpeordem as ordem,
						dpe.dpedsc as periodo,
						dshuf as estado,
						sec.secdsc as responsavel,
						aca.acadsc || ' - Complementação da União repassado aos estados' as acao,
						ume.umedesc as unidademedida,
						'Não se aplica' as detalhe,
						sum(dshqtde) as valor
					FROM
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					inner join territorios.estado est on est.estuf = dsh.dshuf
					inner join painel.secretaria sec on i.secid = sec.secid
					inner join painel.acao aca on i.acaid = aca.acaid
					inner join painel.unidademeta ume on i.umeid = ume.umeid
					left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					WHERE
						i.indid in (603)
						AND ".implode(' AND ', $where1)."
					GROUP BY
						i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
				
					UNION ALL 
					
					SELECT 
						i.indid as codindicador,
						i.indnome as indicador,
						dpeanoref as ano,
						dpeordem as ordem,
						dpe.dpedsc as periodo,
						dshuf as estado,
						sec.secdsc as responsavel,
						aca.acadsc || ' - Repasse a estados' as acao,
						ume.umedesc as unidademedida,
						tiddsc as detalhe,
						sum(dshqtde) as valor
					FROM
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					inner join territorios.estado est on est.estuf = dsh.dshuf
					inner join painel.secretaria sec on i.secid = sec.secid
					inner join painel.acao aca on i.acaid = aca.acaid
					inner join painel.unidademeta ume on i.umeid = ume.umeid
					left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					WHERE
						i.indid in (614)
						AND ".implode(' AND ', $where1)."
					GROUP BY
						i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
					
					UNION ALL 
				
					SELECT
						i.indid as codindicador,
						i.indnome as indicador,
						dpeanoref as ano,
						dpeordem as ordem,
						dpe.dpedsc as periodo,
						dshuf as estado,
						sec.secdsc as responsavel,
						aca.acadsc || ' - Distribuição para estados' as acao,
						ume.umedesc as unidademedida,
						'Não se aplica' as detalhe,
						sum(dshqtde) as valor
					FROM
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					inner join territorios.estado est on est.estuf = dsh.dshuf
					inner join painel.secretaria sec on i.secid = sec.secid
					inner join painel.acao aca on i.acaid = aca.acaid
					inner join painel.unidademeta ume on i.umeid = ume.umeid
					left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					WHERE
						i.indid in (520)
						AND ".implode(' AND ', $where1)."
					GROUP BY
						i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
						
					UNION ALL 
				
					SELECT  550 as codindicador,
	                	'Outros Programas' as indicador,
	                    oppano || '' as ano,
	                    0 as ordem,
	                     oppano || '' as periodo,
	                    iu.estuf as estado,
	                    oppresponsavel as responsavel,
	                    oppacao as acao,
	                    unddsc as unidademedida,
	                    'Não se aplica' as detalhe,
	                    oppvalor as valor
	                FROM cte.outrosprogramaspainel o
               		INNER JOIN cte.unidademedida 		u on u.undid = o.undid
                	INNER JOIN cte.instrumentounidade 	iu on iu.inuid = o.inuid
					".( $where2[0] != '' ? "WHERE ".implode(' AND ', $where2) : '' )."
						
				) as foo
				WHERE
					1=1
					".$where[1]."
				ORDER BY
					estado, codindicador, indicador, ano, ordem, periodo, responsavel, acao, unidademedida, detalhe";

        return $sql;
    }

    public function montarColuna() {
        $agrupador = $_POST['agrupador'];
        if (!is_array($agrupador)) {
            $agrupador = array();
        }

        $coluna = array();

        array_push($agrupador, 'rgfnumprocessosape');
        array_push($agrupador, 'numconvenio');
        array_push($agrupador, 'subacao');
        array_push($agrupador, 'vlrconvenio');
        array_push($agrupador, 'vlrempenhado');
        array_push($agrupador, 'vlrpago');
        array_push($agrupador, 'porcentvalorconv');
        array_push($agrupador, 'saldoglobalconta');
        array_push($agrupador, 'dataatualizacaoconta');

        foreach ($agrupador as $val) {
            switch ($val) {
                case 'rgfnumprocessosape':
                    array_push($coluna, array(
                        "campo" => "rgfnumprocessosape",
                        "label" => "Processo",
                        "type" => "string",
                        "blockAgp" => array("descricaosubacao"),
                        "mostraNivel" => "numeroconvenio",
                        "html" => "<b>{rgfnumprocessosape}</b>"
                    ));
                    continue;
                    break;
                case 'valorpagosubacao':
                    array_push($coluna, array(
                        "campo" => "valorpagosubacao",
                        "label" => "Valor Pago da Subacao",
                        "html" => "<b>{valorpagosubacao}</b>")
                    );
                    continue;
                    break;
                case 'valorempenhadosubacao':
                    array_push($coluna, array(
                        "campo" => "valorempenhadosubacao",
                        "label" => "Valor Empenhado da Subacao",
                        "html" => "<b>{valorempenhadosubacao}</b>")
                    );
                    continue;
                    break;
                case 'porcentvalorconv':
                    array_push($coluna, array(
                        "campo" => "porcentagemexecucaoconv",
                        "label" => "Execução Convênio (%)",
                        "type" => "numeric",
                        "blockAgp" => array("estado", "descricaosubacao"),
                        "mostraNivel" => "numeroconvenio",
                        "php" => array("expressao" => "({valorpago} > 0 && {valorconvenioporsub} > 0)",
                            "html" => "{percentconv}%",
                            "type" => "numeric",
                            "var" => "percentconv",
                            "true" => "number_format(({valorpago} / {valorconvenioporsub}) * 100,1,',','.')",
                            "false" => "0")
                            )
                    );
                    continue;
                    break;

                case 'porcentagemrepasse':
                    array_push($coluna, array(
                        "campo" => "porcentagemrepasse",
                        "label" => "Execução Convênio (%)",
                        "type" => "numeric",
                        "blockAgp" => array("estado", "descricaosubacao"),
                        "mostraNivel" => "numeroconvenio",
                        "php" => array("expressao" => "({valorpago} > 0 && {saldoglobalconta} > 0)",
                            "html" => "{percentrepasse}%",
                            "type" => "numeric",
                            "var" => "percentrepasse",
                            "true" => "str_replace('.', ',', substr(((({valorpago} - {saldoglobalconta}) / {valorpago}) * 100), 0, 2 + (strpos((({valorpago} - {saldoglobalconta}) / {valorpago}) * 100, '.'))))",
                            "false" => "0")
                            )
                    );
                    continue;
                    break;

                case 'porcentexec':
                    array_push($coluna, array(
                        "campo" => "porcentagemexecucao",
                        "label" => "Execução (%)",
                        "type" => "numeric")
                    );
                    continue;
                    break;
                case 'dimensao':
                    array_push($coluna, array(
                        "campo" => "descricaodimensao",
                        "label" => "Dimensão",
                        "type" => "string")
                    );
                    continue;
                    break;
                case 'area':
                    array_push($coluna, array(
                        "campo" => "descricaoarea",
                        "label" => "Área",
                        "type" => "string")
                    );
                    continue;
                    break;
                case 'indicador':
                    array_push($coluna, array(
                        "campo" => "descricaoindicador",
                        "label" => "Indicador",
                        "type" => "string")
                    );
                    continue;
                    break;
                case 'acao':
                    array_push($coluna, array(
                        "campo" => "descricaoacao",
                        "label" => "Ação",
                        "type" => "string")
                    );
                    continue;
                    break;

                case 'dataatualizacaoconta':
                    array_push($coluna, array(
                        "campo" => "ultimadatadaconta",
                        "label" => "Data da Última Atualização da Conta",
                        "type" => "string")
                    );
                    continue;
                    break;

                case 'saldoaplicacao':
                    array_push($coluna, array(
                        "campo" => "saldoglobalaplicacao",
                        "label" => "Saldo Global da Aplicação",
                        "html" => "<b>{saldoglobalaplicacao}</b>")
                    );
                    continue;
                    break;
                case 'saldoglobalconta':
                    array_push($coluna, array(
                        "campo" => "saldoglobalconta",
                        "label" => "Saldo Global da Conta",
                        "type" => "string",
                        "blockAgp" => array("descricaosubacao"),
                        "mostraNivel" => array("numeroconvenio"),
                        "html" => "<b style='color: rgb(0, 102, 204);'>{saldoformatado}</b>",
                        "php" => array("expressao" => "is_numeric({saldoglobalconta})",
                            "type" => "numeric",
                            "var" => "saldoformatado",
                            "true" => "number_format({saldoglobalconta},2,',','.')",
                            "false" => "{saldoglobalconta}")
                            )
                    );
                    continue;
                    break;

                case 'vlrproponente':
                    array_push($coluna, array(
                        "campo" => "valorglobalproponente",
                        "label" => "Valor Global do Proponente",
                        "html" => "<b>{valorglobalproponente}</b>")
                    );
                    continue;
                    break;
                case 'vlrconcedente':
                    array_push($coluna, array(
                        "campo" => "valorglobalconcedente",
                        "label" => "Valor Global do Concedente",
                        "html" => "<b>{valorglobalconcedente}</b>")
                    );
                    continue;
                    break;
                case 'vlraempenhar':
                    array_push($coluna, array(
                        "campo" => "valorglobalaempenhar",
                        "label" => "Valor Global a Empenhar",
                        "html" => "<b>{valorglobalaempenhar}</b>")
                    );
                    continue;
                    break;
                case 'vlrapagar':
                    array_push($coluna, array(
                        "campo" => "valorglobalapagar",
                        "label" => "Valor Global a Pagar",
                        "html" => "<b>{valorglobalapagar}</b>")
                    );
                    continue;
                    break;
                case 'vlrpago':
                    array_push($coluna, array(
                        "campo" => "valorpago",
                        "label" => "Valor Pago",
                        "html" => "<b>{valorpago}</b>")
                    );
                    continue;
                    break;
                case 'vlrempenhado':
                    array_push($coluna, array(
                        "campo" => "valorempenhado",
                        "label" => "Valor Empenhado",
                        "html" => "<b>{valorempenhado}</b>")
                    );
                    continue;
                    break;
                case 'vlrconvenio':
                    array_push($coluna, array(
                        "campo" => "valorconvenioporsub",
                        "label" => "Valor do Convênio",
                        "blockAgp" => "descricaosubacao",
                        "mostraNivel" => "numeroconvenio",
                        "html" => "<b style='color: rgb(0, 102, 204);'>{valorformatado}</b>",
                        "php" => array("expressao" => "is_numeric({valorconvenioporsub})",
                            "type" => "numeric",
                            "var" => "valorformatado",
                            "true" => "number_format({valorconvenioporsub},2,',','.')",
                            "false" => "{valorconvenioporsub}")
                            )
                    );
                    continue;
                    break;
                case 'planointerno':
                    array_push($coluna, array(
                        "campo" => "planointerno",
                        "label" => "Plano Interno",
                        "type" => "string")
                    );
                    continue;
                    break;
                case 'programa':
                    array_push($coluna, array(
                        "campo" => "programa",
                        "label" => "Programa",
                        "type" => "string")
                    );
                    continue;
                    break;
            }
        }

        return $coluna;
    }

    public function montarAgrupador($arrParameters = array()) {
        $agrupador = $_POST['agrupador'];
        if (!is_array($agrupador)) {
            $agrupador = array();
        }

        $agrupador = array(
            "agrupador" => array(),
            "agrupadoColuna" => array(
                "rgfnumprocessosape",
                "numeroconvenio",
                "descricaosubacao",
                "valorconvenioporsub",
                "valorempenhado",
                "valorpago",
                "porcentagemexecucao",
                "porcentagemexecucaoconv",
                "porcentagemrepasse",
                "saldoglobalconta",
                "ultimadatadaconta"
            )
        );

        $count = 1;
        $i = 0;

        if ($i > 1 || $i == 0) {
            $vari = "";
        }

        $novoAgrup = array();
        array_push($novoAgrup, 'estado');
        array_push($agrupador, 'numconvenio');
        array_push($agrupador, 'subacao');
        array_push($agrupador, 'vlrconvenio');
        array_push($agrupador, 'vlrempenhado');
        array_push($agrupador, 'vlrpago');
        array_push($agrupador, 'porcentexec');
        array_push($agrupador, 'saldoglobalconta');
        array_push($agrupador, 'dataatualizacaoconta');

        foreach ($agrupador as $val) {
            array_push($novoAgrup, $val);
        }

        foreach ($novoAgrup as $val):
            if ($count == 1) {
                $var = $vari;
            } else {
                $var = "";
            }
            switch ($val) {
                case 'estado':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "estado",
                        "label" => "$var Estado")
                    );
                    continue;
                    break;
                case 'numconvenio':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "numeroconvenio",
                        "label" => "$var Convênio")
                    );
                    continue;
                    break;
                case 'descricaodimensao':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "descricaodimensao",
                        "label" => "$var Dimensão")
                    );
                    continue;
                    break;
                case 'area':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "descricaoarea",
                        "label" => "$var Área")
                    );
                    continue;
                    break;
                case 'indicador':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "descricaoindicador",
                        "label" => "$var Índicador")
                    );
                    continue;
                    break;
                case 'acao':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "descricaoacao",
                        "label" => "$var Ação")
                    );
                    continue;
                    break;
                case 'subacao':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "descricaosubacao",
                        "label" => "$var Subação")
                    );
                    continue;
                    break;
                case 'planointerno':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "planointerno",
                        "label" => "$var Plano Interno")
                    );
                    continue;
                    break;
                case 'programa':
                    array_push($agrupador['agrupador'], array(
                        "campo" => "programa",
                        "label" => "$var Programa")
                    );
                    continue;
                    break;
            }
            $count++;
        endforeach;

        return $agrupador;
    }

}
