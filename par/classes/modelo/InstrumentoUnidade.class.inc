<?php

class InstrumentoUnidade extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.instrumentounidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("inuid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'inuid' => null,
        'itrid' => null,
        'inudata' => null,
        'estuf' => null,
        'muncod' => null,
        'mun_estuf' => null,
        'usucpf' => null,
    );

    public function verificaInuidMunicipio($muncod) {
        $sql = "select inuid from {$this->stNomeTabela} where muncod = '{$muncod}'";
        return $this->pegaUm($sql);
    }

    public function verificaInuidEstado($estuf) {
        $sql = "select inuid from {$this->stNomeTabela} where estuf = '{$estuf}'";
        return $this->pegaUm($sql);
    }

    /**
     * Retorna Lista de E-mail e Nome da Entidades por Municipio ou UF
     * @param type $arrPost
     * return array
     * @TODO Correcao de Tipo de envio de e-mail para termo
     */
    public function getMailByMunicipioUf($arrPost) {
        $where = '';
        if ($arrPost['esfera'] == 'M') {
            $arrMunicipio = implode("','", $arrPost['listaMunicipio']);
            $where = "iu.muncod in ('{$arrMunicipio}')";
        } else if ($arrPost['esfera'] == 'E') {
            $arrUf = implode("','", $arrPost['estuf']);
            $where = "iu.mun_estuf in ('{$arrUf}')";
        } else {
           return array();
        }

        # Buscar email para envio do municipio
        $sql = "
            SELECT 
                en.entnome,
                en.entemail
            FROM par.instrumentounidade iu 
            INNER JOIN par.entidade en ON iu.inuid=en.inuid
            WHERE {$where}
            AND en.dutid = 6
        ";
        return $this->carregar($sql);
    }

}
