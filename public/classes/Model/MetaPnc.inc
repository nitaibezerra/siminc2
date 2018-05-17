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

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '')
    {
        $where['exercicio'] = !empty($where['exercicio']) ? $where['exercicio'] : $_SESSION['exercicio'];

        $join = '';
        if(!empty($where['suoid'])){
            $join = "
                JOIN (
                    SELECT
                        smc.mpnid
                    FROM spo.subunidademetapnc smc
                        JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = smc.suoid AND suo.prsano = '{$where['exercicio']}'
                    WHERE
                        suo.suoid = '{$where['suoid']}'
                    UNION 
                    SELECT
                        mpn.mpnid
                    FROM public.metapnc mpn
                        LEFT JOIN spo.subunidademetapnc smc ON smc.mpnid = mpn.mpnid
                    WHERE
                        mpn.prsano = '{$where['exercicio']}'       
                        AND smc.mpnid IS NULL
                ) smc ON smc.mpnid = mpn.mpnid
            ";
        }

        $sql = "
            SELECT
                mpn.mpncod::NUMERIC AS ordem,
                mpn.mpnid AS codigo,
                mpn.mpncod || ' - ' || mpn.mpnnome AS descricao
            FROM public.metapnc mpn
            $join
            WHERE
                mpn.mpnstatus = 'A'
                AND mpn.prsano  = '{$where['exercicio']}'
            ORDER BY
                ordem
        ";
//ver($sql, d);
        return $sql;
    }

}

