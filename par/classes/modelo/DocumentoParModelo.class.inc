<?php

class DocumentoParModelo extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.documentopar";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dopid");

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
        'dopprorrogacaovigenciaobra' => null
    );

    public function relatorioTermoCompromisso($arrPost = array(), $booSQL = FALSE) {
        #armazena as condicoes da consulta
        $arrWhere = array("foo.status = 'A'");

        #numero termo
        if (!empty($arrPost['numerotermo'])) {
            $arrWhere[] = "foo.termo ilike '%{$arrPost['numerotermo']}%'";
        }

        #ano termo
        if (!empty($arrPost['anotermo'])) {
            $arrWhere[] = "foo.dopano = '{$arrPost['anotermo']}'";
        }

        #processo
        if (!empty($arrPost['processo'])) {
            $arrPost['processo'] = str_replace(array('/','.','-'), '', $arrPost['processo']);
            $arrWhere[] = "foo.processo = '{$arrPost['processo']}'";
        }

        #uf
        if (!empty($arrPost['estuf'][0])) {
            $arrWhere[] = "foo.uf IN ('" . implode("', '", $arrPost['estuf']) . "')";
        }

        #municipio
        if (!empty($arrPost['listaMunicipio'][0])) {
            $arrWhere[] = "foo.muncod IN ('" . implode("', '", $arrPost['listaMunicipio']) . "')";
        }
        
        #Processo
        $processo = "'<span class=\"processo_detalhe\" >' || to_char(foo.processo::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
        if (isset($arrPost['btnExcel'])) {
            $processo = "(TO_CHAR(foo.processo::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'))";
        }

        $sql = " WITH
                    temp_documentoparreprogramacao as 
                    (SELECT dopid FROM par.documentoparreprogramacao WHERE (dprstatus = 'P' OR dprstatus = 'A') AND dprvalidacao NOT IN ('f')),
                    temp_documentoparreprogramacaosubacao as 
                    (SELECT dopid FROM par.documentoparreprogramacaosubacao WHERE (dpsstatus = 'P' OR dpsstatus = 'A') AND dpsvalidacao NOT IN ('f')) ";
        
        $sql .= " SELECT 
                    uf,
                    entidade,
                    esfera,
                    termo,
                    dopano,
                    $processo as processo,
                    documento,
                    vigencia,
                    COALESCE(saldo::numeric, 0.00) as saldo,
                    subacao,
                    repro_prazo,
                    repro_sub,
                    tipo
                FROM (";
            if ($arrPost['tipo'] != 'T') {
                if ($arrPost['tipo'] == 'SUBACAO'){
                    $sql .= $this->relatorioTermoCompromissoSubacao($arrPost, TRUE);
                } else if ($arrPost['tipo'] == 'PAC'){
                    $sql .= $this->relatorioTermoCompromissoPac($arrPost, TRUE);
                } else {
                    $sql .= $this->relatorioTermoCompromissoPar($arrPost, TRUE);
                }                
            } else {
                $sql .= $this->relatorioTermoCompromissoSubacao(array(), TRUE);
                $sql .= " UNION ALL ";
                $sql .= $this->relatorioTermoCompromissoPac(array(), TRUE);
                $sql .= " UNION ALL ";
                $sql .= $this->relatorioTermoCompromissoPar(array(), TRUE);
            }
        $sql .=" ) foo 
            WHERE ". implode(' AND ', $arrWhere)."
         ORDER BY tipo, uf, entidade, documento";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }
    
    public function relatorioTermoCompromissoSubacao($arrPost = array(), $booSQL = FALSE){
        $sql = "SELECT DISTINCT
                    dp.dopstatus as status,
                    m.muncod,
                    CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END AS uf,
                    CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE m.mundescricao END AS entidade,
                    CASE WHEN iu.itrid = 1 THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                    prp.prpnumeroprocesso as processo,
                    COALESCE(dp.dopdatafimvigencia, '-') as vigencia,
                    par.retornasaldoprocesso(prp.prpnumeroprocesso) as saldo,
                    mdo.mdonome AS documento,
                    CAST((SELECT dp.dopnumerodocumento FROM par.documentopar WHERE prpid = dp.prpid AND dopstatus <> 'E' ORDER BY dopid ASC LIMIT 1) AS text) AS termo,
                    (SELECT dopano FROM par.documentopar WHERE prpid = dp.prpid ORDER BY dopid ASC LIMIT 1) as dopano,
                    CASE 
                        WHEN (
                            SELECT
                                count(sbaid)
                            FROM par.termocomposicao tc 
                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
                            WHERE tc.dopid = dp.dopid
                            ) > 0 THEN 'Sim' 
                        ELSE 'Não' 
                    END as subacao,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacao WHERE dopid = dp.dopid)  THEN 'Sim' ELSE 'Não' END as repro_prazo,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacaosubacao WHERE dopid = dp.dopid) THEN 'Sim' ELSE 'Não' END as repro_sub,
                    'SUBAÇÃO' as tipo
                FROM {$this->stNomeTabela} dp
                INNER JOIN  par.processopar prp on prp.prpid = dp.prpid
                INNER JOIN  par.instrumentounidade iu on iu.inuid = prp.inuid
                LEFT JOIN  territorios.municipio m ON m.muncod = iu.muncod
                INNER JOIN  par.modelosdocumentos mdo ON mdo.mdoid = dp.mdoid
                WHERE dp.dopstatus = 'A'";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }
    
    /**
     * @TODO Verificar campos
     */
    public function relatorioTermoCompromissoPac($arrPost = array(), $booSQL = FALSE){
        $sql = "SELECT DISTINCT
                   CAST(ter.terstatus AS character(1)) as status,
                    m.muncod,
                    CASE WHEN pro.estuf IS NOT NULL THEN pro.estuf ELSE m.estuf END AS uf,
                    CASE WHEN pro.muncod IS NOT NULL THEN m.mundescricao ELSE pro.estuf END AS entidade,
                    CASE WHEN pro.estuf IS NOT NULL THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                    pro.pronumeroprocesso as processo,
                    (
                        SELECT 
                            COALESCE(TO_CHAR(MAX(pop.popdataprazoaprovado), 'MM/YYYY'), '-')
                        FROM par.termocompromissopac tc
                        INNER JOIN par.processoobraspaccomposicao popc ON popc.proid = tc.proid 
                        LEFT JOIN obras.preobraprorrogacao pop ON pop.preid = popc.preid
                        WHERE tc.terid = ter.terid                   
                    ) as vigencia,
                    par.retornasaldoprocesso(pro.pronumeroprocesso) as saldo,
                    'Termo de Compromisso' AS documento,
                    (SELECT par.retornanumerotermopac(ter.proid)) AS termo,
                    ter.terano as dopano,
                    'Não' as subacao,
                    CASE 
                        WHEN 
                            (SELECT 
                                COUNT(pop.preid)
                            FROM par.processoobraspaccomposicao popc
                            LEFT JOIN obras.preobraprorrogacao pop ON pop.preid = popc.preid
                            WHERE popc.proid = ter.proid) > 0 THEN 'Sim'  
                        ELSE 'Não'
                    END as repro_prazo,
                    'Não' as repro_sub,
                    'PAC' as tipo
                FROM par.termocompromissopac ter
                INNER JOIN  par.processoobra pro on pro.proid = ter.proid
                LEFT JOIN  territorios.municipio m ON m.muncod = pro.muncod
                LEFT JOIN  territorios.estado e ON e.estuf = pro.estuf
                WHERE ter.terstatus = 'A'";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }
    
    public function relatorioTermoCompromissoPar($arrPost = array(), $booSQL = FALSE){
        $sql = "SELECT DISTINCT
                    dp.dopstatus as status,
                    m.muncod,
                    CASE WHEN pro.estuf IS NOT NULL THEN pro.estuf ELSE m.estuf END AS uf,
                    CASE WHEN pro.muncod IS NOT NULL THEN m.mundescricao ELSE pro.estuf END AS entidade,
                    CASE WHEN pro.estuf IS NOT NULL THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                    pro.pronumeroprocesso as processo,
                    COALESCE(dp.dopdatafimvigencia, '-') as vigencia,
                    par.retornasaldoprocesso(pro.pronumeroprocesso) as saldo,
                    mdo.mdonome AS documento,
                    CAST((SELECT dp.dopnumerodocumento FROM par.documentopar WHERE proid = dp.proid AND dopstatus <> 'E' ORDER BY dopid ASC LIMIT 1) AS text) AS termo,
                    (SELECT dopano FROM par.documentopar WHERE proid = dp.proid ORDER BY dopid ASC LIMIT 1) as dopano,
                    CASE 
                        WHEN (
                            SELECT 
                                count(sd.sbaid) 
                            FROM par.termocomposicao tc 
                            INNER JOIN par.subacaoobra so on so.preid = tc.preid
                            INNER JOIN par.subacaodetalhe sd on sd.sbaid = so.sbaid
                            WHERE tc.dopid = dp.dopid
                            ) > 0 THEN 'Sim' 
                        ELSE 'Não' 
                    END as subacao,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacao WHERE dopid = dp.dopid)  THEN 'Sim' ELSE 'Não' END as repro_prazo,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacaosubacao WHERE dopid = dp.dopid) THEN 'Sim' ELSE 'Não' END as repro_sub,
                    'PAR' as tipo
                FROM {$this->stNomeTabela} dp
                INNER JOIN  par.processoobraspar pro on pro.proid = dp.proid
                LEFT JOIN  territorios.municipio m ON m.muncod = pro.muncod
                LEFT JOIN  territorios.estado e ON e.estuf = pro.estuf
                INNER JOIN  par.modelosdocumentos mdo ON mdo.mdoid = dp.mdoid
                WHERE dp.dopstatus = 'A'
                ORDER BY
                    uf, entidade, documento";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

}
