<?php

/**
 * Class para manter restricoes do FNDE
 * @author Jair Foro <jairforo@gmail.com>
 * @since 21/08/2014
 */
class RestricaoFnde extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     * @author Jair Foro <jairforo@gmail.com>
     */
    protected $stNomeTabela = "par.restricaofnde";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     * @author Jair Foro <jairforo@gmail.com>
     */
    protected $arChavePrimaria = array("resid");

    /**
     * Atributos
     * @var array
     * @access protected
     * @author Jair Foro <jairforo@gmail.com>
     */
    protected $arAtributos = array(
        'resid' => null,
        'resdescricao' => null,
        'resdatainicio' => null,
        'resdatafim' => null,
        'restemporestricao' => null,
        'usucpf' => null,
        'tprid' => null,
        'resinformar' => null,
        'resprovidencia' => null,
        'ressuperado' => null,
    );

    /**
     * Inseri uma restricão FNDE
     * @param array $arrPost
     * @return int resid;
     * @author Jair Foro <jairforo@gmail.com>
     * @TODO Tratar data de forma inteligente
     */
    public function insert(array $arrPost = array()) {
        $resdatainicio = formata_data_sql($arrPost['resdatainicio']);
        $resdatainicio = !empty($resdatainicio) ? "'$resdatainicio'" : ' NULL ';
        $resdatafim = formata_data_sql($arrPost['resdatafim']);
        $resdatafim = !empty($resdatafim) ? "'$resdatafim'" : 'NULL';
        $restemporestricao = $arrPost['restemporestricao'] == 1 ? ' TRUE ' : ' FALSE ';
        $sql = "
                INSERT INTO
                    par.restricaofnde (
                        resdescricao,
                        resdatainicio,
                        resdatafim,
                        restemporestricao,
                        usucpf,
                        resstatus,
                        tprid,
                        resinformar,
                        resprovidencia,
                        ressuperado
                    ) 
                VALUES (
                    '" . $arrPost['resdescricao'] . "',
                    $resdatainicio,
                    $resdatafim,
                    " . $restemporestricao . ",
                    '" . $arrPost['usucpf'] . "',
                    'A',
                    " . $arrPost['tprid'] . ",
                    '" . $arrPost['resinformar'] . "',
                    '" . $arrPost['resprovidencia'] . "',
                    '" . $arrPost['ressuperado'] . "'
                ) 
                RETURNING 
                    resid;
            ";
        #retornar o resid inserido
        return $this->pegaUm($sql);
    }
    
    /**
     * Atualiza uma restricão FNDE
     * @param array $arrPost
     * @param integer $resid
     * @return integer resid
     * @author Jair Foro <jairforo@gmail.com>
     * @TODO Tratar data de forma inteligente
     */
    public function update(array $arrPost = array(), $resid = null) {
        $resdatainicio = formata_data_sql($arrPost['resdatainicio']);
        $resdatainicio = !empty($resdatainicio) ? "'$resdatainicio'" : ' NULL ';
        $resdatafim = formata_data_sql($arrPost['resdatafim']);
        $resdatafim = !empty($resdatafim) ? "'$resdatafim'" : 'NULL';
        $restemporestricao = $arrPost['restemporestricao'] == 1 ? 'TRUE' : 'FALSE';
        
        $sql = "
                UPDATE 
                    par.restricaofnde  
                SET
                    resdescricao = '" . $arrPost['resdescricao'] . "',
                    resdatainicio = $resdatainicio,
                    resdatafim = $resdatafim,
                    restemporestricao = " . $restemporestricao . ",
                    usucpf = '" . $arrPost['usucpf'] . "',
                    tprid = " . $arrPost['tprid'] . ",
                    resstatus = 'A',
                    resinformar = '" . $arrPost['resinformar'] . "',
                    resprovidencia = '" . $arrPost['resprovidencia'] . "',
                    ressuperado = '" . $arrPost['ressuperado'] . "'
                WHERE 
                  resid = {$resid}
            ";

        return $this->executar($sql);
    }

}
