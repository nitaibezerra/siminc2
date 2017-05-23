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
class Pi_Localizacao extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planacomorc.pi_localizacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pilid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'pilid' => null,
                                        'pliid' => null,
                                        'paiid' => null,
                                        'estuf' => null,
                                        'muncod' => null,
    );

    public function recuperarPorPlanoInterno($pliid, $esfid)
    {
        if(!$esfid || $esfid == Territorios_Model_Esfera::K_FEDERAL){
            return [];
        }

        switch($esfid){
            case Territorios_Model_Esfera::K_EXTERIOR:
                $sql = "select pilid, p.paidescricao, p.paiid
                        from planacomorc.pi_localizacao l
                            inner join territorios.pais p on p.paiid = l.paiid
                        where l.pliid = $pliid";
                break;
            case Territorios_Model_Esfera::K_ESTADUAL:
                $sql = "select pilid, e.estuf, e.estdescricao, e.estcod 
                        from planacomorc.pi_localizacao l
                            inner join territorios.estado e on e.estuf = l.estuf
                        where l.pliid = $pliid";
                break;
            case Territorios_Model_Esfera::K_MUNICIPAL:
                $sql = "select pilid, e.estuf, e.estdescricao, e.estcod, m.muncod, m.mundescricao
                        from planacomorc.pi_localizacao l
                            inner join territorios.municipio m on m.muncod = l.muncod
                            inner join territorios.estado e on e.estuf = m.estuf
                        where l.pliid = $pliid";
                break;
        }

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}