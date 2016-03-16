<?php

class TermoCompromissoPac extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.termocompromissopac";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("terid");

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
        'teracaoorigem' => null
    );

    public function recuperaTermoPorMunicipio($muncod) {
        $sql = "select terid, arqid, to_char(terdatainclusao, 'YYYY') as teranoinclusao from {$this->stNomeTabela}  where muncod = '$muncod' and terstatus = 'A'";
        return $this->carregar($sql);
    }

    public function editar($arrCampo, $terid){
        
        $strSet = "";
        foreach ($arrCampo as $key => $value) {
            $strSet .= "{$key}={$value},";
        }
        $strSet = substr($strSet, 0, -1);
        $sql = "
                UPDATE {$this->stNomeTabela}
                   SET {$strSet} WHERE terid = {$terid}";
        return $this->executar($sql);
    }
}
