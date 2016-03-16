<?php
/**
 * Classe de mapeamento da entidade par.documentopar
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2015
 * @link     no link
 */



/**
 * Par3_Model_DocumentoparLegado
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Par3_Model_DocumentoParLegado extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par.documentopar';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dopid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpfinclusao' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'usucpfalteracao' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'mdoid' => array('tabela' => 'par.modelosdocumentos', 'pk' => 'mdoid'),
        'prpid' => array('tabela' => 'par.processopar', 'pk' => 'prpid'),
        'iueid' => array('tabela' => 'par.instrumentounidadeentidade', 'pk' => 'iueid'),
        'arqid_documento' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dopid' => null,
        'prpid' => null,
        'doptexto' => null,
        'dopstatus' => null,
        'dopdiasvigencia' => null,
        'dopdatainicio' => null,
        'dopdatafim' => null,
        'mdoid' => null,
        'dopdatainclusao' => null,
        'usucpfinclusao' => null,
        'dopdataalteracao' => null,
        'usucpfalteracao' => null,
        'dopjustificativa' => null,
        'dopdatavalidacaofnde' => null,
        'dopusucpfvalidacaofnde' => null,
        'dopdatavalidacaogestor' => null,
        'dopusucpfvalidacaogestor' => null,
        'dopusucpfstatus' => null,
        'dopdatastatus' => null,
        'dopdatapublicacao' => null,
        'doppaginadou' => null,
        'dopnumportaria' => null,
        'proid' => null,
        'dopreformulacao' => null,
        'dopidpai' => null,
        'dopidaditivo' => null,
        'dopnumerodocumento' => null,
        'dopobservacao' => null,
        'dopdatafimvigencia' => null,
        'dopvalortermo' => null,
        'dopdatainiciovigencia' => null,
        'arqid' => null,
        'dopacompanhamento' => null,
        'dopano' => null,
        'dopdocumentocarregado' => null,
        'iueid' => null,
        'arqid_documento' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'dopid' => array(  'Digits'  ),
            'prpid' => array( 'allowEmpty' => true, 'Digits'  ),
            'doptexto' => array( 'allowEmpty' => true ),
            'dopstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'dopdiasvigencia' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopdatainicio' => array( 'allowEmpty' => true ),
            'dopdatafim' => array( 'allowEmpty' => true ),
            'mdoid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopdatainclusao' => array( 'allowEmpty' => true ),
            'usucpfinclusao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopdataalteracao' => array( 'allowEmpty' => true ),
            'usucpfalteracao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopjustificativa' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 4000))  ),
            'dopdatavalidacaofnde' => array( 'allowEmpty' => true ),
            'dopusucpfvalidacaofnde' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopdatavalidacaogestor' => array( 'allowEmpty' => true ),
            'dopusucpfvalidacaogestor' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopusucpfstatus' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopdatastatus' => array( 'allowEmpty' => true ),
            'dopdatapublicacao' => array( 'allowEmpty' => true ),
            'doppaginadou' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'dopnumportaria' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 50))  ),
            'proid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopreformulacao' => array( 'allowEmpty' => true ),
            'dopidpai' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopidaditivo' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopnumerodocumento' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopobservacao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'dopdatafimvigencia' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'dopvalortermo' => array( 'allowEmpty' => true ),
            'dopdatainiciovigencia' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'arqid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopacompanhamento' => array( 'allowEmpty' => true ),
            'dopano' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopdocumentocarregado' => array( 'allowEmpty' => true ),
            'iueid' => array( 'allowEmpty' => true, 'Digits'  ),
            'arqid_documento' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função motnarFiltroPAR
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function motnarFiltroPAR($arrPost)
    {
        $where = array();

        if ($arrPost['dopstatus'] != '') $where[] = "dopstatus = '{$arrPost['dopstatus']}'";

        if ($arrPost['inuid'] != '') $where[] = "inuid = {$arrPost['inuid']}";

        if ($arrPost['id'] != '') $where[] = "id = {$arrPost['id']}";

        if ($arrPost['tipo_doc'] != '') $where[] = "tipo_doc = {$arrPost['tipo_doc']}";

        return $where;

    }//end motnarFiltroPAR()


    /**
     * Função montarSQLPAR
     * - monta a SQL da lista de documentos PAR.
     *
     * @return escreve a montarSQLPAR.
     *
     */
    public function montarSQLPAR($arrPost)
    {
        $where = self::motnarFiltroPAR($arrPost);

        $sql = "SELECT
                    id,
                	prpnumeroprocesso as processo,
                	doc,
                	tipodocumento,
                	CASE
                		WHEN	( (date_trunc('MONTH', to_date(data_vigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date - current_date) < 90
                			AND ( (date_trunc('MONTH', to_date(data_vigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date - current_date) > 0 THEN
                			'<span style=\"color:red\">'	|| data_vigencia || '</span>'
                		ELSE
                			data_vigencia
                	END as data_vigencia,
                	dopvalortermo as vt,
                	valorempenho as ve,
                	valorpagamentosolicitado as ps,
                	valorpagamento as vp,
                	dados_bancarios,
                	(
                		SELECT COALESCE(saldo) as saldo
                		FROM
                                    ( select par.retornasaldoprocesso(prpnumeroprocesso) AS saldo ) as saldomes
                	)  as sb,
            arqid
                FROM
                (
                	SELECT
                		dp.dopid as id,
                		dp.dopidpai as dopidpai,

                		pp.prpnumeroprocesso,
                		dp.dopnumerodocumento as doc,
                		mdonome as tipodocumento,
                		(
                			SELECT
                				to_char(vigencia, 'MM/YYYY') -- seleciona maior vigência entre documento validado e ex-ofício
                			FROM (
                			       SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia --Seleciona maior vigência entre termos validados
                				      FROM par.documentopar  d
                				      INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                				      WHERE d.prpid = pp.prpid --8146
                				      AND dopstatus <> 'E' AND dopdatafimvigencia IS NOT NULL
                				      AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                			       UNION ALL
                			       SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia -- Seleciona maior vigência de Ex-Ofício
                				      FROM par.documentopar  d
                				      WHERE d.prpid = pp.prpid
                				      AND dopstatus <> 'E' AND dopdatafimvigencia IS NOT NULL
                				      AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                			) as foo
                			GROUP BY vigencia
                			ORDER BY vigencia DESC LIMIT 1
                		) as data_vigencia,
                		(SELECT dopvalortermo::numeric(20,2) FROM par.documentopar  d
                			INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
                			WHERE prpid = pp.prpid
                			AND dopstatus <> 'E'
                			AND dpvstatus = 'A'
                			AND mdoid NOT IN (79,65,66,68,76,80,67,73,82)
                			ORDER BY d.dopid DESC
                			LIMIT 1
                		) as dopvalortermo,
                		COALESCE((select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = pp.prpnumeroprocesso ), 0.00) as valorempenho,
                		pgs.valor_pagamento as valorpagamentosolicitado,
                		pm.valor_pagamento as valorpagamento,
                		'Banco: '||coalesce(prpbanco, 'n/a')||'<br> Conta: '||coalesce(prpagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,

                		dp.arqid,
                		d.tpdcod as tipo_doc,
                		pp.prpfinalizado,
                		iu.inuid,
                		iu.estuf,
                		iu.muncod,
                		dp.dopstatus
                	FROM
                		par.documentopar dp
                	INNER JOIN par.modelosdocumentos  d  ON d.mdoid = dp.mdoid
                	INNER JOIN par.processopar 		  pp ON pp.prpid = dp.prpid
                	INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
                	LEFT  JOIN (
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid
                		INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                		INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
                		INNER JOIN par.pagamentosubacao 	ps  ON ps.pagid = pag.pagid and pobstatus = 'A'
                		WHERE d.dopstatus = 'A'
                		GROUP BY d.dopid, pagsituacaopagamento
                		) 				              pm ON pm.dopid = dp.dopid
                	LEFT  JOIN (
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid
                		INNER JOIN par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
                		INNER JOIN par.pagamentosubacao ps  on ps.pagid = pag.pagid and pobstatus = 'A'
                		WHERE d.dopstatus = 'A'
                		GROUP BY d.dopid, pagsituacaopagamento
                		)                             pgs ON pgs.dopid = dp.dopid
                ) as foo
                WHERE
                	".implode(' AND ', $where);

        return $sql;
    }//end listaPAR()


    /**
     * Função motnarFiltroObrasPAR
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function motnarFiltroObrasPAR($arrPost)
    {
        $where = array();

        if ($arrPost['inuid'] != '') $where[] = 'inuid = '.$arrPost['inuid'];

        return $where;

    }//end motnarFiltroObrasPAR()


    /**
     * Função montarSQLObrasPAR
     * - monta a SQL da lista de documentos PAR.
     *
     * @return escreve a montarSQLPAR.
     *
     */
    public function montarSQLObrasPAR($arrPost)
    {
        $where = self::motnarFiltroObrasPAR($arrPost);

        $sql = "SELECT
                    id,
                    dopidpai,
                	pronumeroprocesso,
                	doc,
                	tipodocumento,
                	data_vigencia,
                	qtdObra,
                	dopvalortermo as vt,
                	valorempenho as ve,
                	valorpagamentosolicitado as ps,
                	valorpagamento as vp,
                	dados_bancarios,
                	( SELECT par.retornasaldoprocesso(pronumeroprocesso) ) as sb
                FROM
                (
                	SELECT
                		dp.dopid as id,
                		dp.dopidpai,
                		(
                			SELECT
                				to_char(vigencia, 'MM/YYYY') -- seleciona maior vigência entre documento validado e ex-ofício
                			FROM
                			(
                				SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia --Seleciona maior vigência entre termos validados
                				FROM par.documentopar  d
                				INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                				WHERE
                					d.proid = dp.proid
                					AND dopstatus <> 'E'
                					AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                				UNION ALL
                				SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia -- Seleciona maior vigência de Ex-Ofício
                				FROM par.documentopar  d
                				WHERE
                					d.proid = dp.proid
                					AND dopstatus <> 'E'
                					AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                			) as foo
                			GROUP BY vigencia
                			ORDER BY vigencia DESC LIMIT 1
                		) as data_vigencia,
                		d.mdonome as tipodocumento,
                		iu.inuid,
                		iu.estuf,
                		iu.muncod,
                		dp.dopusucpfvalidacaogestor,
                		d.mdoqtdvalidacao,
                		(select dopnumerodocumento from par.documentopar where proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) as doc,
                		'Banco: '||coalesce(probanco, 'n/a')||'
                		Conta: '||coalesce(proagencia, 'n/a')||'
                		Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
                		(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem,
                		dp.dopvalortermo::numeric(20,2),
                		em.valor as valorempenho,
                		pgs.valor_pagamento as valorpagamentosolicitado,
                		pm.valor_pagamento as valorpagamento,
                		pp.pronumeroprocesso,
                		(select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = pp.proid) as qtdObra
                	FROM par.documentopar  dp
                	INNER JOIN par.modelosdocumentos   	d ON d.mdoid = dp.mdoid AND dp.dopstatus = 'A'
                	INNER JOIN par.processoobraspar 	pp ON pp.proid = dp.proid and pp.prostatus = 'A'
                	INNER JOIN par.instrumentounidade 	iu ON iu.inuid = pp.inuid
                	LEFT  JOIN
                	(
                		SELECT
                			dopid,
                			sum(valor) as valor
                		FROM
                		(
                			SELECT DISTINCT
                				d.dopid,
                				vve.empid,
                				vve.vrlempenhocancelado as valor
                			FROM
                				par.documentopar d
                			INNER JOIN par.processoobraspar prp on prp.proid = d.proid and prp.prostatus = 'A'
                			INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                			INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp.empid
                			LEFT  JOIN
                			(
                				SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie
                				FROM par.empenho
                				WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
                				GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
                			) as emr ON emr.empidpai = emp.empid
                			INNER JOIN par.empenhoobrapar ems on ems.empid = emp.empid and eobstatus = 'A'
                		) as foo
                		GROUP BY dopid
                	) 					em ON em.dopid = dp.dopid
                	LEFT  JOIN
                	(
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid AND d.dopstatus = 'A'
                		INNER JOIN par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
                		INNER JOIN par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid, pagsituacaopagamento
                	) 					pm ON pm.dopid = dp.dopid
                	LEFT  JOIN
                	(
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid AND d.dopstatus = 'A'
                		INNER JOIN par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
                		INNER JOIN par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid, pagsituacaopagamento
                	) 					pgs ON pgs.dopid = dp.dopid
                ) as foo
                WHERE ".implode(' AND ', $where)."";

        return $sql;
    }//end montarSQLObrasPAR()


    /**
     * Função formVizualizaDocumento
     * - monta tela de vizualização do documento PAR.
     *
     * @return documento.
     *
     */
    public function formVizualizaDocumento($arrPost){

        $sql = "select * from (
				SELECT dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
						FROM par.documentopar  dp
						INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
						INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
						INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
						LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
						left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
				union
				SELECT dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
						FROM par.documentopar  dp
						INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
						INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
						INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
						LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
						left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
				) as foo
			 WHERE
				dopid = ".$arrPost['dopid'];

        $html = self::pegaLinha($sql);

        $sql = "SELECT
				dpv.dpvcpf as cpfgestor,
				to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
				us.usunome,
				us.usucpf
			FROM par.documentopar  dp
			INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
			INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
			WHERE
				dpv.dpvstatus = 'A' and
				dp.dopid = ".$arrPost['dopid'];

        $dadosValidacao = self::carregar($sql);

        $textoValidacao = "";

        if(is_array($dadosValidacao)){
            foreach( $dadosValidacao as $dv ){
                $cpfvalidacao[] = $dv['cpfgestor'];
                $textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
            }
        }

        $cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array();

        $doptexto = self::pegarTermoCompromissoArquivo( $arrPost['dopid'], '');
        ?>
        <div style="background: #FFF; white-space: nowrap">
            <table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
                <tr>
                    <td style="font-size: 12px; font-family:arial;">
                        <div id="divImpressao">
                            <table width='100%' border='0' cellpadding='0' cellspacing='0'>
                                <tr bgcolor='#FFF'>
                                    <td valign='top' align='center'>
                                        <img src='../imagens/brasao.gif' width='45' height='45' border='0'>
                                        <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />
                                    </td>
                                </tr>
                            </table>
                            <?php echo simec_html_entity_decode($doptexto);?>
                        </div>
                    </td>
                </tr>
            </table>
            <table id="termo" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
                <tr <?php echo $displayValidado; ?> style="text-align: center;">
                    <td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
                                <?php echo $textoValidacao; ?>
                    </td>
                </tr>
            </table>
        </div>
    <?php
    }//end formVizualizaDocumento()


    /**
     * Função pegarTermoCompromissoArquivo
     * - retorna o termo de compromisso.
     *
     * @return documento.
     *
     */
    function pegarTermoCompromissoArquivo( $dopid, $terid ){

        if( $dopid || $terid ){

            if( $terid ){
                $dtanomearquivo = self::pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.terid = {$terid} order by d.dtaid desc");
            } else {
                $dtanomearquivo = self::pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.dopid = {$dopid} order by d.dtaid desc");
            }

            if( $dtanomearquivo ){
                $diretorio = APPRAIZ . 'arquivos/par/documentoTermo/'.$dtanomearquivo;
                if( is_file($diretorio) ){
                    $doptexto = file_get_contents($diretorio);
                }
            }
        }
        return $doptexto;
    }//end pegarTermoCompromissoArquivo

}//end Class
?>