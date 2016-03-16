<?php

	
class SupervisaoEmpresa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisaoempresa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sueid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
        'sueid' => null, 
        'sobid' => null, 
        'empid' => null, 
        'entidvistoriador' => null, 
        'usucpf' => null, 
        'endid' => null, 
        'sosid' => null, 
        'docid' => null, 
        'suedtsupervisao' => null, 
        'suedtcadastro' => null, 
        'suefuncionamento' => null, 
        'suecargovistoriador' => null, 
        'sueendcorreto' => null, 
        'sueacordo' => null, 
        'suedtatualizacao' => null, 
        'suestatus' => null ,
        'tplid' => null,
        'sueobservacao' => null,
        'sueruralurbano' => null,
        'sueprojetofnde' => null,
        'suejustificativa' => null,
        'suepercentualexe' => null,
        'suearqmemcalc' => null,
        'sueobsretificacaoos' => null,
        'sueproblema' => null,
        'sueobsproblema' => null,
    );

	public function  listaDados( Array $param = array() ){
            
		$where = array();
		$join  = array();
		
		if ( $param['empid'] ){
			$where[] = "se.empid = {$param['empid']}";
		}
                
		if ( $param['sueid'] ){
			$where[] = "se.sueid = {$param['sueid']}";
		}

		if( $param['obrid'] ){
			$join[] = "
			JOIN obras2.obras obr ON obr.empid = soo.empid AND obr.obrid = {$param['obrid']}";
		}
		
		if ( !possui_perfil( array( PFLCOD_SUPER_USUARIO, 
                                            PFLCOD_EMPRESA_MI_FISCAL,
                                            PFLCOD_EMPRESA_MI_GESTOR,
                                            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                                            PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                                            PFLCOD_GESTOR_UNIDADE,
                                            PFLCOD_GESTOR_MEC,
                                            PFLCOD_SUPERVISOR_UNIDADE,
                                            PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                                            PFLCOD_EMPRESA_CONTRATADA ) ) ){
			$where[] = "ed.esdid = " . ESDID_VISTORIA_EMP_LAUDO;	
		}		
		
                    $sql = "

                    SELECT
                    se.sueid,
                                os.sosid,
                                sos.sosnum,
                                TO_CHAR(se.suedtsupervisao, 'dd/mm/yyyy') AS suedtsupervisao,
                                TO_CHAR(se.suedtcadastro, 'dd/mm/yyyy') AS suedtcadastro,
                                --so.sobdsc,
                                ed.esdid,
                                CASE WHEN ed.esddsc IS NOT NULL
                                        THEN ed.esddsc
                                        ELSE 'Não iniciado' END AS esddsc,

                                e.entnome,
                                e.entnumcpfcnpj AS entcpf,
                                se.suecargovistoriador,
                                CASE WHEN se.suefuncionamento = 'n'
                                        THEN 'Não'
                                      WHEN se.suefuncionamento = 's'
                                        THEN 'Sim' ELSE ''
                                END AS suefuncionamento,
                                u.usucpf,
                                u.usunome,
                                ( SELECT
                                        CASE
                                            WHEN SUM(icovlritem) > 0 THEN
                                                ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                                            ELSE
                                                0
                                        END AS total
                                                -- SUM(spivlritemsobreobraexecanterior) AS total
                                    FROM
                                        obras2.itenscomposicaoobra i
                                    JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A', 'H') AND s.croid = cro.croid
                                    LEFT JOIN
                                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                                        AND sic.supid = s.supid
                                        AND sic.icoid IS NOT NULL
                                        AND sic.ditid IS NULL
                                    WHERE
                                        i.icostatus = 'A' AND
                                        i.relativoedificacao = 'D' AND
                                        cro.obrid = s.obrid AND i.obrid = cro.obrid
                                ) as percentual


                    FROM obras2.supervisao_os_obra soo
                    JOIN obras2.supervisao_os sos ON sos.sosid = soo.sosid AND sos.sosstatus = 'A'

                    ".(count($join) ? implode(' ',$join) : "") . "

                    LEFT JOIN obras2.supervisaoempresa se ON se.sosid = sos.sosid AND se.empid = soo.empid AND se.suestatus = 'A'
                    LEFT JOIN obras2.supervisao s ON s.sueid = se.sueid AND s.supstatus = 'A'
                    LEFT JOIN workflow.documento	      d ON d.docid = se.docid
                    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                    LEFT JOIN obras2.supervisao_os     os ON os.sosid = se.sosid AND os.sosstatus = 'A'
                    LEFT JOIN seguranca.usuario         u ON u.usucpf = se.usucpf
                    LEFT JOIN entidade.entidade         e ON e.entid = se.entidvistoriador
                    WHERE soo.soostatus = 'A'".
                        (count($where) ? " AND " . implode(' AND ',$where) : "") . "
                        ORDER BY
                                se.sueid";


//		ver(simec_htmlentities($sql),d);
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}
	
	public function excluir( $sueid ){
		$sql = "UPDATE obras2.supervisao SET 
					supstatus='I'
				WHERE 
				 	sueid = {$sueid};";
		$this->executar( $sql );
		
		$sql = "UPDATE obras2.supervisaoempresa SET 
					suestatus='I'
				WHERE 
				 	sueid = {$sueid};";
		$this->executar( $sql );
		
	}
	
	public function pegaUltSueidByEmpreendimento( $empid ){
		$arJoin  = array();
		$arWhere = array();
		
//		if ( $param['not(emsid)'] ){
//			$arWhere[] = "s.emsid IS NULL";
//		}
//		
//		if ( $param['not(sueid)'] ){
//			$arWhere[] = "s.sueid IS NULL";
//		}
//		
//		if ( $param['empid'] && $param['is(sueid)'] ){
//			$arJoin[] = "JOIN obras2.supervisaoempresa se ON se.sueid = s.sueid";
//			$arWhere[] = "se.empid = {$param['empid']}";
//		}elseif ( $param['empid'] ){
//			$arJoin[] = "JOIN obras2.empreendimentosupervisao es ON es.emsid = s.emsid";
//			$arWhere[] = "es.empid = {$param['empid']}";
//		}
		
		$sql = "SELECT
                	se.sueid
                FROM
                	obras2.supervisaoempresa se
                " . (count($arJoin) ? implode(" ", $arJoin) : "") . "	
                WHERE
                	se.suestatus = 'A' AND
                	" . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                    se.empid = {$empid}
                ORDER BY
                	se.suedtsupervisao DESC, se.sueid DESC
                LIMIT 1";
		
		return $this->pegaUm( $sql );
	}
	
	public function pegaNomeResponsavelPorSueid( $sueid ){
		$sql = "SELECT
                                entnome
                        FROM
                                obras2.supervisaoempresa se
                        JOIN entidade.entidade e ON e.entid = se.entidvistoriador
                        WHERE
                                sueid = {$sueid}";
		
		return $this->pegaUm( $sql );
	}

	public function carregaDadosHomologacao( $sueid ){
		$sql = "SELECT
					MAX(hstid) AS hstid,
					TO_CHAR(htddata, 'DD/MM/YYYY') AS htddata,
					u.usucpf,
					u.usunome
				FROM
					obras2.supervisaoempresa se 
				JOIN workflow.historicodocumento hd ON hd.docid = se.docid
				JOIN seguranca.usuario u ON u.usucpf = hd.usucpf
				WHERE
					se.sueid = {$sueid} AND 
					hd.aedid = " . WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR . "
				GROUP BY	
					htddata,
					u.usucpf,
					u.usunome";
		
		$dados = $this->pegaLinha( $sql );
		
		return ($dados ? $dados : array());
	}
        
        public function pegaDataOsSupEmpresa($sueid){
            if(empty($sueid)){
                $sueid = $this->sueid;
                if(empty($sueid)){
                    return date('Y-m-d');
                }
            }
            $sql = " 
                    SELECT sosdtinicio
                    FROM obras2.supervisao_os sos
                    INNER JOIN obras2.supervisaoempresa sue ON sue.sosid = sos.sosid
                    WHERE sue.sueid = ".$sueid;

            $data = $this->pegaUm($sql);
            return $data;
            
        }
        
        public function pegaStatusSupEmpresa($docid){
            $sql = "SELECT wd.docid, wd.tpdid, wd.esdid, ed.esddsc
                    FROM workflow.documento wd 
                    INNER JOIN workflow.estadodocumento ed ON (ed.esdid = wd.tpdid)
                    WHERE wd.docid = {$docid}";
            
            $dados = $this->pegaLinha($sql);
            
            return ($dados ? $dados : array());
        }
        
}