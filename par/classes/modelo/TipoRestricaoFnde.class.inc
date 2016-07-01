<?php

class TipoRestricaoFnde extends Modelo {
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.tiporestricaofnde";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tprid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tprid' => null,
        'tprdescricao' => null,
        'tprstatus' => null,
        'tprdata' => null,
    );

    /**
     * Retorna o registro pela PK
     * @param integer $tprid
     * @return array TipoRestricaoFnde
     * @author Jair Foro <jairforo@gmail.com>
     */
    public function findByPK($tprid = null) {
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE tprid = {$tprid}";
        $arrTipoRestricaoFnde = $this->carregar($sql);
        return reset($arrTipoRestricaoFnde);
    }
}
