<?php

class PerfilUsuario extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.perfilusuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ptoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'pflcod' => null,
    );

    public function carregarPerfilUsuario($pflcod) {
        $sql = "SELECT DISTINCT
                    poa.poausucpfinclusao as codigo,
                    u.usunome as descricao
                FROM seguranca.perfilusuario pu
                INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
                INNER JOIN obras.preobraanalise poa ON poa.poausucpfinclusao = u.usucpf
                WHERE pu.pflcod IN (" . $pflcod . ")
                ORDER BY u.usunome";
        return $this->carregar($sql);
    }

}
