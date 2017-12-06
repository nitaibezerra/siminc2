<?php
/**
 * Classe de abstração da tabela monitora.pi_complemento.
 *
 * @version $Id$
 * @see Pi_PlanoInterno
 */

/**
 *
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Pi_Responsavel extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planacomorc.pi_responsavel";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pirid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pirid' => null,
        'pliid' => null,
        'usucpf' => null,
    );

    public function recuperarPorPlanoInterno($pliid)
    {
        $sql = "
            SELECT
                u.usucpf,
                u.usunome,
                u.usuemail,
                usufoneddd || ' - ' || usufonenum AS telefone
            FROM planacomorc.pi_responsavel r
                JOIN seguranca.usuario u ON u.usucpf = r.usucpf
            WHERE
                r.pliid = $pliid
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}