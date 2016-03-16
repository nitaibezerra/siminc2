<?php

class ItensExecucaoOrcamentaria extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.itensexecucaoorcamentaria";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("iocid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'iocid' => null,
        'eorid' => null,
        'arqid' => null,
        'usucpf' => null,
        'iocvalor' => null,
        'iocdtposicao' => null,
        'iocdtinclusao' => null,
        'ioctipodetalhamento' => null,
        'iocid_1' => null
    );

    function listaSql(Array $param = array()) {
        $arWhere = array();

        if (!empty($param['eorid'])) {
            $param['eorid'] = (array) $param['eorid'];
            $arWhere[] = "eor.eorid IN(" . implode(", ", $param['eorid']) . ")";
        }

        if (!empty($param['empid'])) {
            $param['empid'] = (array) $param['empid'];
            $arWhere[] = "eor.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        if (!empty($param['obrid'])) {
            $param['obrid'] = (array) $param['obrid'];
            $arWhere[] = "eor.obrid IN(" . implode(", ", $param['obrid']) . ")";
        }

        if (!empty($param['ioctipodetalhamento'])) {
            $param['ioctipodetalhamento'] = (array) $param['ioctipodetalhamento'];
            $arWhere[] = "ioc.ioctipodetalhamento IN('" . implode("', '", $param['ioctipodetalhamento']) . "')";
        }

//		if ( !empty($param['rgadsccompleta']) ){
//			$arWhere[] 		= "ra.rgadsccompleta ILIKE '%" . $param['rgadsccompleta'] . "%'";
//		}

        if ($param['block_imgs_acao'] !== true) {
            $acao = "'<center>
                            <img
                                    align=\"absmiddle\"
                                    src=\"/imagens/alterar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: alterarExec(\'' || ioc.iocid || '\');\"
                                    title=\"Alterar\">
                            <img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir.gif\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    onclick=\"javascript: excluirExec(\'' || ioc.iocid || '\');\"
                                    title=\"Excluir\">
                      </center>'";
        } else {
            $acao = "''";
        }
        
        // Modificado para adequar a nova tela
        $acao = "''";

        $sql = "SELECT
                        --{$acao} AS acao,
                        TO_CHAR(ioc.iocdtposicao , 'DD/MM/YYYY' ) AS iocdtposicao,
                        iocvalor,
                        ( COALESCE(iocvalor, 0) / ( COALESCE(eor.eorvlrcusteio, 0) + COALESCE(eor.eorvlrcapital, 0) ) ) * 100 AS eorpercent,
                    CASE WHEN ioc.arqid IS NOT NULL 
                    THEN
                            '<a href=\"javascript:void(0);\" 
                            id=\"' || a.arqid || '\" 
                            class=\"download\" 
                            title=\"Baixar Arquivo\">' 
                            || a.arqnome || '.' || a.arqextensao || 
                        '</a>'
                    ELSE 
                        ''
                    END AS arquivo,
                    usu.usunome
                FROM
                    obras2.itensexecucaoorcamentaria ioc
                JOIN obras2.execucaoorcamentaria eor ON eor.eorid = ioc.eorid
                JOIN seguranca.usuario usu           ON usu.usucpf = ioc.usucpf
                LEFT JOIN public.arquivo a           ON a.arqid = ioc.arqid
                WHERE " .
                    ( implode(" AND ", $arWhere) ) . "
                ORDER BY
                    ioc.iocdtposicao";

        return $sql;
    }

    function listaDados(Array $param = array()) {
        $arWhere = array();

        if (!empty($param['eorid'])) {
            $param['eorid'] = (array) $param['eorid'];
            $arWhere[] = "eor.eorid IN(" . implode(", ", $param['eorid']) . ")";
        }

        if (!empty($param['empid'])) {
            $param['empid'] = (array) $param['empid'];
            $arWhere[] = "eor.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        if (!empty($param['obrid'])) {
            $param['obrid'] = (array) $param['obrid'];
            $arWhere[] = "eor.obrid IN(" . implode(", ", $param['obrid']) . ")";
        }

        if (!empty($param['ioctipodetalhamento'])) {
            $param['ioctipodetalhamento'] = (array) $param['ioctipodetalhamento'];
            $arWhere[] = "ioc.ioctipodetalhamento IN('" . implode("', '", $param['ioctipodetalhamento']) . "')";
        }

        $sql = "SELECT
                    iocid,
                    eor.eorid,
                    iocvalor,
                    CASE
                            WHEN (COALESCE(eor.eorvlrcusteio, 0) + COALESCE(eor.eorvlrcapital, 0)) > 0 THEN
                                    ( COALESCE(iocvalor, 0) / (COALESCE(eor.eorvlrcusteio, 0) + COALESCE(eor.eorvlrcapital, 0)) ) * 100 
                            ELSE
                                    0
                    END AS iocpercent,
                    TO_CHAR(iocdtposicao, 'DD/MM/YYYY' ) AS iocdtposicao,
                    TO_CHAR(iocdtinclusao, 'DD/MM/YYYY' ) AS iocdtinclusao,
                    ioctipodetalhamento,
                    iocid_1,
                    usu.usucpf,
                    usu.usunome,
                    a.arqid,
                    a.arqnome,
                    a.arqextensao					
                FROM
                    obras2.itensexecucaoorcamentaria ioc
                JOIN obras2.execucaoorcamentaria eor ON eor.eorid = ioc.eorid
                JOIN seguranca.usuario usu           ON usu.usucpf = ioc.usucpf
                LEFT JOIN public.arquivo a           ON a.arqid = ioc.arqid
                WHERE " .
                    ( implode(" AND ", $arWhere) ) . "
                ORDER BY
                    ioc.iocdtposicao";
        $dados = $this->carregar($sql);

        return ($dados ? $dados : array());
    }

}