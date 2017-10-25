<?php
/**
 * Classe de mapeamento da entidade public.metapnc
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Metapnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_MetaPnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.metapnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'mpnid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'mpnid' => null,
        'mpndsc' => null,
        'mpnstatus' => null,
        'mpncod' => null,
        'mpnnome' => null,
        'prsano' => null,
    );

    public function recuperarSqlCombo($dados = null)
    {
        $join = '';
        if(!empty($dados['suocod'])){
            $join = "inner join (
                        select smc.mpnid
                        from spo.subunidademetapnc smc
                                inner join public.vw_subunidadeorcamentaria suo on suo.suoid = smc.suoid and suo.prsano = '{$_SESSION['exercicio']}'
                        where suo.suocod = '{$dados['suocod']}'
                        union 
                        select mpn.mpnid from public.metapnc mpn
                                left join spo.subunidademetapnc smc on smc.mpnid = mpn.mpnid
                        where mpn.prsano = '{$_SESSION['exercicio']}'       
                        and smc.mpnid is null              
                    ) smc on smc.mpnid = mpn.mpnid";
        }

        $sql = "SELECT
                    mpn.mpncod::NUMERIC AS ordem,
                    mpn.mpnid AS codigo,
                    mpn.mpncod || ' - ' || mpn.mpnnome AS descricao
                FROM public.metapnc mpn
                $join
                WHERE mpn.mpnstatus = 'A'
                AND mpn.prsano  = '{$_SESSION['exercicio']}'
                ORDER BY ordem";

        return $sql;
    }

}//end Class
?>