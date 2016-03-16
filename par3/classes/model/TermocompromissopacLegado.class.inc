<?php
/**
 * Classe de mapeamento da entidade par.termocompromissopac
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2015
 * @link     no link
 */



/**
 * Par3_Model_TermocompromissopacLegado
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Par3_Model_TermocompromissopacLegado extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par.termocompromissopac';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'terid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'proid' => array('tabela' => 'par.processoobra', 'pk' => 'proid'),
        'tpdcod' => array('tabela' => 'tipodocumento', 'pk' => 'tpdcod'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'titid' => array('tabela' => 'par.tipoimpressaotermo', 'pk' => 'titid'),
        'teridpai' => array('tabela' => 'par.termocompromissopac', 'pk' => 'terid'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'terid' => null,
        'terdatainclusao' => null,
        'usucpf' => null,
        'arqid' => null,
        'muncod' => null,
        'terstatus' => null,
        'arqidanexo' => null,
        'terassinado' => null,
        'proid' => null,
        'estuf' => null,
        'usucpfassinatura' => null,
        'terdataassinatura' => null,
        'terdocumento' => null,
        'terreformulacao' => null,
        'teridpai' => null,
        'titid' => null,
        'tpdcod' => null,
        'ternumero' => null,
        'terano' => null,
        'teracaoorigem' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'terid' => array(  'Digits'  ),
            'terdatainclusao' => array(  ),
            'usucpf' => array(  new Zend_Validate_StringLength(array('max' => 11))  ),
            'arqid' => array( 'allowEmpty' => true, 'Digits'  ),
            'muncod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 6))  ),
            'terstatus' => array(  ),
            'arqidanexo' => array( 'allowEmpty' => true, 'Digits'  ),
            'terassinado' => array( 'allowEmpty' => true ),
            'proid' => array( 'allowEmpty' => true, 'Digits'  ),
            'estuf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))  ),
            'usucpfassinatura' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'terdataassinatura' => array( 'allowEmpty' => true ),
            'terdocumento' => array( 'allowEmpty' => true ),
            'terreformulacao' => array( 'allowEmpty' => true ),
            'teridpai' => array( 'allowEmpty' => true, 'Digits'  ),
            'titid' => array( 'allowEmpty' => true, 'Digits'  ),
            'tpdcod' => array( 'allowEmpty' => true, 'Digits'  ),
            'ternumero' => array( 'allowEmpty' => true, 'Digits'  ),
            'terano' => array( 'allowEmpty' => true, 'Digits'  ),
            'teracaoorigem' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 20))  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função montarFiltro
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function montarFiltro($arrPost)
    {
        $where = array();

        if ($arrPost['terstatus']) $where[] = "terstatus = '{$arrPost['terstatus']}'";

        if ($arrPost['inuid']) {
            $modelInstrumentounidade = new Par3_Model_InstrumentoUnidade($arrPost['inuid']);

            if ($modelInstrumentounidade->itrid === 2) {
                $where[] = "muncod = '{$modelInstrumentounidade->muncod}'";
            } else {
                $where[] = "estuf = '{$modelInstrumentounidade->estuf}'";
            }
        }


        return $where;

    }//end montarFiltro()


    /**
     * Função montarSQLLista
     * - monta a SQL da lista de documentos de obras PAC.
     *
     * @return escreve a montarSQLLista.
     *
     */
    public function montarSQLLista($arrPost)
    {
        $where = self::montarFiltro($arrPost);

        $sql = "SELECT
                    tc.terid as id,
                	coalesce(pro.pronumeroprocesso, '-') as pronumeroprocesso,
                	'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as ternum,
                	CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE to_char(tc.terdataassinatura, 'DD/MM/YYYY') END as data,
                	to_char(vig.data, 'DD/MM/YYYY') as datafimvigencia,
                	CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE u.usunome END as usu,
                	(select count(pc.preid) from par.processoobra po inner join par.processoobraspaccomposicao pc on pc.proid = po.proid where
                		pc.pocstatus = 'A' and po.proid = pro.proid) as qtdObra,
                	( select sum( prevalorobra )
                    from par.termoobra ter
                    inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A'
                    WHERE ter.terid = tc.terid ) as valor_termo,
                	em.valor as valorempenho,
                	sum(pm.valor_pagamento) as valorpagamentosolicitado,
                	sum(CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento ELSE 0 END) as valorpagamento,
                	'Banco: '||coalesce(probanco, 'n/a')||'
                	 Conta: '||coalesce(proagencia, 'n/a')||'
                	 Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
                	( select par.retornasaldoprocesso(pronumeroprocesso) ) as sb,
                    tc.arqid
                FROM
                	par.termocompromissopac  tc
                INNER JOIN par.processoobra 	pro ON pro.proid = tc.proid and pro.prostatus = 'A'
                LEFT  JOIN seguranca.usuario 	u 	ON u.usucpf = tc.usucpfassinatura
                LEFT  JOIN (
                	SELECT
                		terid,
                		sum(valor) as valor
                	FROM
                	(
                		SELECT DISTINCT
                			tc.terid,
                			vve.empid,
                			vve.vrlempenhocancelado + coalesce(emr.vrlreforco,0) as valor
                		FROM
                			par.termocompromissopac tc
                		INNER JOIN par.processoobra 		prp ON prp.proid = tc.proid and prp.prostatus = 'A'
                		INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.v_vrlempenhocancelado 	vve ON vve.empid = emp.empid
                		LEFT  JOIN (
                			SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie
                			FROM par.empenho
                			WHERE empcodigoespecie in ('02') AND empstatus = 'A'
                			GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
                		) as emr on emr.empidpai = emp.empid
                		INNER JOIN par.empenhoobra ems on ems.empid = emp.empid and eobstatus = 'A'
                	) as foo
                	GROUP BY terid
                ) as 				em ON em.terid = tc.terid
                LEFT JOIN
                (
                	SELECT
                		tc.terid, sum( pobvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
                	FROM
                		par.termocompromissopac tc
                	INNER JOIN par.processoobra 	prp ON prp.proid = tc.proid and prp.prostatus = 'A'
                	INNER JOIN par.empenho 		emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                	INNER JOIN par.pagamento 	pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
                	INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid
                	GROUP BY tc.terid, pagsituacaopagamento
                ) 				pm ON pm.terid = tc.terid
                LEFT  JOIN
                (
                	SELECT DISTINCT
                		terid,
                		max(prazo) as data
                	FROM
                	(
                		SELECT
                			po.preid,
                			case when vig.data is not null then vig.data else (MIN(pag.pagdatapagamentosiafi) + 720) end  as prazo,
                			tc.terid, popvalidacao
                		FROM
                			par.pagamentoobra po
                		INNER JOIN par.pagamento 		pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                		INNER JOIN par.empenho 			emp on emp.empid = pag.empid and emp.empstatus = 'A'
                		INNER JOIN par.termoobraspaccomposicao 	tc on tc.preid = po.preid
                		LEFT  JOIN obras.preobraprorrogacao 	pp on pp.preid = po.preid and pp.popdataprazoaprovado is not null and pp.popvalidacao = 't'
                		LEFT  JOIN (
                		    SELECT popdataprazoaprovado as data, preid FROM obras.preobraprorrogacao WHERE popstatus = 'A'
                		) 					vig on vig.preid = po.preid
                		GROUP BY po.preid, vig.data, tc.terid, popvalidacao
                	) as foo
                	GROUP BY terid
                ) 				vig on vig.terid = tc.terid
                WHERE
                	tc.terstatus = 'A'
                	AND tc.muncod = '5200050'
                GROUP BY
                	tc.usucpfassinatura, tc.terassinado, tc.proid, tc.terid, tc.terdatainclusao, tc.terdataassinatura,
                	u.usunome, vig.data,
                	em.valor, pro.proid, tc.arqid, tc.teridpai,
                	pro.probanco, pro.proagencia, pro.nu_conta_corrente, pro.pronumeroprocesso
                ORDER BY
                	tc.terid";

        return $sql;
    }//end montarSQLLista()





}//end Class
?>