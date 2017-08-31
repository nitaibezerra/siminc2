<?php

/**
 * Classe de mapeamento da entidade planacomorc.pi_delegacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-08-2017
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';

/**
 * Planacomorc_Model_Pi_delegacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_PiDelegacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.pi_delegacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pdeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
        'pliid' => array('tabela' => 'pi_planointerno', 'pk' => 'pliid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pdeid' => null,
        'pliid' => null,
        'suoid' => null,
    );

    public function recuperarPorPlanoInterno($pliid, $exercicio)
    {
        $sql = "
            SELECT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
                JOIN planacomorc.pi_delegacao pd ON(suo.suoid = pd.suoid)
            WHERE
                suo.suostatus = 'A'
                AND pliid = ". (int)$pliid. "
                AND suo.prsano = '". (int)$exercicio. "'
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    public function recuperarIdPorPlanoInterno($pliid, $exercicio)
    {
        $sql = "
            SELECT
                suo.suoid
            FROM public.vw_subunidadeorcamentaria suo
                JOIN planacomorc.pi_delegacao pd ON(suo.suoid = pd.suoid)
            WHERE
                suo.suostatus = 'A'
                AND pliid = ". (int)$pliid. "
                AND suo.prsano = '". (int)$exercicio. "'
        ";

        $dados = $this->carregar($sql);
        $dados ? $dados : [];
        
        $listaId = [];
        foreach($dados as $dado){
            $listaId[] = $dado['suoid'];
        }
        return $listaId;
    }
    
}//end Class
?>