<?php
	
class Questao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qstid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qstid' => null, 
									  	'dvqid' => null, 
									  	'itcid' => null, 
									  	'etqid' => null, 
									  	'orgid' => null, 
									  	'qstdsc' => null, 
									  	'qstnumero' => null, 
									  	'qstctrlimagem' => null, 
									  	'qstctrlobs' => null, 
									  	'qstescopo' => null, 
									  	'qstordem' => null, 
									  	'qstcontundente' => null, 
									  	'qststatus' => null 
									  );

	public function pegaTodaEstrutura( Array $filtro = array() ){
		$arWhere   = array();
		$arWhereQS = array();
		
		if ( $filtro['qstescopo'] ){
			$arWhere[] = "q.qstescopo = '{$filtro['qstescopo']}'";
		}
		
		if ( $filtro['orgid'] ){
			$arWhere[] = "q.orgid = {$filtro['orgid']}";
		}
		
		if ( $filtro['etqid'] ){
			$arWhere[] = "eq.etqid = {$filtro['etqid']}";
		}
		
		if ( isset($filtro['supid']) ){
//			$arWhereQS   = array();
			$arWhereQS[] = "qs.supid = {$filtro['supid']}";
		}
		
		if ( isset($filtro['emsid']) ){
//			$arWhereQS   = array();
			$arWhereQS[] = "qs.emsid = {$filtro['emsid']}";
		}
		
		if ( isset($filtro['sueid']) ){
//			$arWhereQS   = array();
			$arWhereQS[] = "qs.sueid = {$filtro['sueid']}";
		}
		
		if ( isset($filtro['smiid']) ){
//			$arWhereQS   = array();
			$arWhereQS[] = "qs.smiid = {$filtro['smiid']}";
		}

		if ( isset($filtro['sfndeid']) ){
//			$arWhereQS   = array();
			$arWhereQS[] = "qs.sfndeid = {$filtro['sfndeid']}";
		}

		
		$sql = "SELECT
					ic.*,
					dq.*,
					qs.*,
					eq.*,
					q.*
				FROM
					obras2.etapaquestao eq
				JOIN obras2.questao q ON q.etqid = eq.etqid AND
							 			 q.qststatus = 'A'
				JOIN obras2.itemcomposicaoquestao ic ON ic.itcid = q.itcid AND
								  				  		ic.itcstatus = 'A'
				LEFT JOIN obras2.divisaoquestao dq ON dq.dvqid = q.dvqid AND
								      				  dq.dvqstatus = 'A'
				LEFT JOIN obras2.questaosupervisao qs ON qs.qstid = q.qstid AND
									 					 qs.qtsstatus = 'A'
									 					 " . (count($arWhereQS) ? " AND " . implode(" AND ", $arWhereQS) : "") . "
				WHERE
					eq.etqstatus = 'A'
				" . (count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "") . "
				ORDER BY
					eq.etqordem, q.qstordem";
		
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}


    public function pegaTodaEstruturaSolicitacaoVinculada($slvid = 'NULL', Array $filtro = array() ){
        $arWhere   = array();

        if ( $filtro['qstid'] ){
            $arWhere[] = "q.qstid = '{$filtro['qstid']}'";
        }

        if ( $filtro['slvid'] ){
            $arWhere[] = "qv.slvid = '{$filtro['slvid']}'";
        }

        $sql = "

                    SELECT
                        eq.etqdsc,
                        itc.itcdsc,
                        q.qstid,
                        q.itcid,
                        q.qstnumero,
                        q.qstdsc,
                        q.qstordem,
                        q.qstctrlobs,
                        q.qstobs,
                        qv.qsvid serial,
                        qv.slvid,
                        qv.qsvresposta,
                        qv.qsvobs,
                        v.vdrid
                    FROM
                        obras2.etapaquestao eq
                    JOIN obras2.questao q ON q.etqid = eq.etqid AND q.qststatus = 'A'
                    JOIN obras2.itemcomposicaoquestao itc ON itc.itcid = q.itcid
                    LEFT JOIN obras2.questaosolicitacaovinculada qv ON qv.qstid = q.qstid AND qv.slvid = $slvid
                    LEFT JOIN obras2.validador_questao vq ON vq.qstid = q.qstid AND vq.vdqstatus = 'A'
                    LEFT JOIN obras2.validador v ON v.vdrid = vq.vdrid AND v.vdrstatus = 'A'
                    WHERE eq.etqid = ".ETQID_VINCULADA."
                    " . (count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "") . "
                    ORDER BY q.qstordem;
        ";

        $dados = $this->carregar( $sql );
        return ($dados ? $dados : array());
    }

    public function pegaTodaEstruturaSolicitacaoDesembolso($sldid = 'NULL', Array $filtro = array() ){
        $arWhere   = array();

        if ( $filtro['qstid'] ){
            $arWhere[] = "q.qstid = '{$filtro['qstid']}'";
        }

        if ( $filtro['sldid'] ){
            $arWhere[] = "qv.sldid = '{$filtro['sldid']}'";
        }

        $sql = "

                    SELECT
                        eq.etqdsc,
                        itc.itcdsc,
                        q.qstid,
                        q.itcid,
                        q.qstnumero,
                        q.qstdsc,
                        q.qstordem,
                        q.qstctrlobs,
                        q.qstobs,
                        qv.qsdid serial,
                        qv.sldid,
                        qv.qsdresposta,
                        qv.qsdobs,
                        v.vdrid
                    FROM
                        obras2.etapaquestao eq
                    JOIN obras2.questao q ON q.etqid = eq.etqid AND q.qststatus = 'A'
                    JOIN obras2.itemcomposicaoquestao itc ON itc.itcid = q.itcid
                    LEFT JOIN obras2.questaosolicitacaodesembolso qv ON qv.qstid = q.qstid AND qv.sldid = $sldid
                    LEFT JOIN obras2.validador_questao vq ON vq.qstid = q.qstid AND vq.vdqstatus = 'A'
                    LEFT JOIN obras2.validador v ON v.vdrid = vq.vdrid AND v.vdrstatus = 'A'
                    WHERE eq.etqid = ".ETQID_DESEMBOLSO."
                    " . (count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "") . "
                    ORDER BY q.qstordem;
        ";

        $dados = $this->carregar( $sql );
        return ($dados ? $dados : array());
    }

    public function pegaTodaEstruturaCumprimentoObjeto($codid = 'NULL', $qstescopo = 'QDCO'){
        if(!$codid) {
            $codid = 'NULL';
        }
        $sql = "
            SELECT
                eq.etqdsc,
                eq.etqid,
                itc.itcdsc,
                q.qstid,
                q.itcid,
                q.qstnumero,
                q.qstdsc,
                q.qstordem,
                q.qstctrlobs,
                q.qstobs,
                cod.codid,
                qcod.qcodid,
                qcod.qcodresposta,
                qcod.qcodobs,
                v.vdrid,
                array_to_string(array_agg(qca.arqid), ',') AS arqid,
                array_to_string(array_agg(arq.arqnome), ',') AS arqnome,
                array_to_string(array_agg(arq.arqextensao), ',') AS arqextensao
            FROM obras2.etapaquestao eq
            JOIN obras2.questao q ON q.etqid = eq.etqid AND q.qststatus = 'A'
            JOIN obras2.itemcomposicaoquestao itc ON itc.itcid = q.itcid
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON cod.codid = $codid
            LEFT JOIN obras2.questao_cumprimento_obj_documentacao qcod ON qcod.qstid = q.qstid AND qcod.cumid = cod.codid
            LEFT JOIN obras2.questao_cumprimento_arquivo qca ON (qcod.qcodid = qca.qcodid AND qca.status = 'A')
            LEFT JOIN public.arquivo arq ON (qca.arqid = arq.arqid)
            LEFT JOIN obras2.validador_questao vq ON vq.qstid = q.qstid AND vq.vdqstatus = 'A'
            LEFT JOIN obras2.validador v ON v.vdrid = vq.vdrid AND v.vdrstatus = 'A'
            WHERE q.qstescopo in ('$qstescopo')
            GROUP BY 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
            ORDER BY q.qstescopo,q.qstordem;
        ";

        $dados = $this->carregar( $sql );
        return ($dados ? $dados : array());
    }
}