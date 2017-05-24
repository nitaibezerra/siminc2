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
    protected $arAtributos     = array(
                                        'pirid' => null,
                                        'pliid' => null,
                                        'usucpf' => null,
									  );

    public function recuperarPorPlanoInterno($pliid)
    {
        $sql = "select r.pliid, u.usunome, u.usuemail, u.usucpf, usufoneddd || ' - ' ||usufonenum as usutelefone
                from planacomorc.pi_responsavel r
                    inner join seguranca.usuario u on u.usucpf = r.usucpf
                where r.pliid = $pliid";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}