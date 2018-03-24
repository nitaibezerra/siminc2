<?php
/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamento
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Acompanhamento
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Acompanhamento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'acompanhamento.programacaoexercicio', 'pk' => 'prsano'),
        'janid' => array('tabela' => 'acompanhamento.janela', 'pk' => 'janid'),
        'pliid' => array('tabela' => 'pi_planointerno', 'pk' => 'pliid'),
        'ipnid' => array('tabela' => 'indicadorpnc', 'pk' => 'ipnid'),
        'mppid' => array('tabela' => 'metappa', 'pk' => 'mppid'),
        'claid' => array('tabela' => 'acompanhamento.classificacao', 'pk' => 'claid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acoid' => null,
        'acoquantidade' => null,
        'acodata' => null,
        'acoanalise' => null,
        'claid' => null,
        'acoprovidencias' => null,
        'suoid' => null,
        'mppid' => null,
        'ipnid' => null,
        'pliid' => null,
        'janid' => null,
        'prsano' => null,
        'acostatus' => null,
    );

    public function getMetasPpa($suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select mpp.*, aco.acoid, smp.suoid, aco.acoquantidade
                    from public.metappa mpp
                            inner join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                            left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = smp.suoid and acostatus = 'A'
                    where mpp.prsano = '$exercicio'
                    and mpp.mppstatus = 'A'
                    and smp.suoid = " . (int) $suoid . "
                    union
                    select mpp.*, aco.acoid, smp.suoid, aco.acoquantidade
                    from public.metappa mpp
                            left join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                            left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = $suoid and acostatus = 'A'
                    where mpp.prsano = '$exercicio'
                    and mpp.mppstatus = 'A'
                    and smp.suoid is null
                    and mpp.mppcod != '0'
                    order by mppcod, mppdsc";

//            ver($sql, d);
            
            return $this->carregar($sql);
        }

        return [];
    }
    
    public function getMetasPpaRelatorio(stdClass $param){
        $where = "";
        $where .= $param->suoid? "/n AND smp.suoid = ". (int)$param->suoid: '';

        $sql = "
            SELECT
                aco.acoid,
                aco.suoid,
                aco.janid,
                mpp.mppid,
                mpp.mppdsc,
                mpp.mppcod,
                mpp.mppnome,
                mpp.mppquantificavel,
                mpp.mppquantidade,
                mpp.mppanualizavel,
                aco.acoquantidade
            FROM public.metappa mpp
                LEFT JOIN spo.subunidademetappa smp ON smp.mppid = mpp.mppid
                JOIN acompanhamento.acompanhamento aco ON(
                    aco.mppid = mpp.mppid
--                    AND aco.suoid = smp.suoid
                    AND acostatus = 'A'
                )
            WHERE
                mpp.prsano = '". (int)$param->exercicio. "'
                AND mpp.mppstatus = 'A'
                $where
            ORDER BY
                aco.janid,
                mppcod,
                mppdsc
        ";
//ver($sql,d);
        $listaResultado = $this->carregar($sql);

        return $listaResultado? $listaResultado: array();
    }

    public function getMetasPpaComPi($suoid, $janid = null, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select  ac.mppid, ac.mppdsc, ac.mppcod, ac.mppnome, ac.mppstatus, ac.prsano, ac.mppquantificavel, ac.mppquantidade, ac.acoid, ac.suoid, ac.acoquantidade,
                            case 
                                when  (select count(1) from spo.subunidademetappa m where m.mppid = ac.mppid) = 0 then true
                                when  (select count(1) from spo.subunidademetappa m where m.mppid = ac.mppid) > 2 then true
                                else false      
                            end as compartilhada,
                            count(pic.pliid) pis, sum(pic.picquantidade) picquantidade
                    from (
                            select mpp.mppid, mpp.mppdsc, mpp.mppcod, mpp.mppnome, mpp.mppstatus, mpp.prsano, mpp.mppquantificavel, mpp.mppquantidade, aco.acoid, smp.suoid, aco.acoquantidade
                            from public.metappa mpp
                                inner join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                                left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = smp.suoid and acostatus = 'A' and aco.janid = " . (int) $janid . "
                            where mpp.prsano = '$exercicio'
                            and mpp.mppstatus = 'A'
                            and smp.suoid = $suoid
                            union
                            select mpp.mppid, mpp.mppdsc, mpp.mppcod, mpp.mppnome, mpp.mppstatus, mpp.prsano, mpp.mppquantificavel, mpp.mppquantidade, aco.acoid, $suoid, aco.acoquantidade
                            from public.metappa mpp
                                left join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                                left join acompanhamento.acompanhamento aco on aco.mppid = mpp.mppid and aco.suoid = $suoid and acostatus = 'A' and aco.janid = " . (int) $janid . "
                            where mpp.prsano = '$exercicio'
                            and mpp.mppstatus = 'A'
                            and smp.suoid is null
                            and mpp.mppcod != '0'
                    ) ac
                            left join public.vw_subunidadeorcamentaria suo on suo.suoid = ac.suoid
                            left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.unicod = suo.unocod        
                            left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid and pic.mppid = ac.mppid        
                    group by ac.mppid, ac.mppdsc, ac.mppcod, ac.mppnome, ac.mppstatus, ac.prsano, ac.mppquantificavel, ac.mppquantidade, ac.acoid, ac.suoid, ac.acoquantidade, compartilhada
                    order by mppcod, mppdsc        
                    ;";

//            ver($sql, d);

            return $this->carregar($sql);
        }

        return [];
    }

    public function getIndicadoresPnc($suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select ipn.*, aco.acoid, sic.suoid, aco.acoquantidade, mpn.mpncod::int, mpn.mpndsc
                    from public.indicadorpnc ipn
                        inner join public.metapnc mpn on mpn.mpnid = ipn.mpnid      
                        inner join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                        left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = sic.suoid and acostatus = 'A'
                    where ipn.prsano = '$exercicio'
                    and ipn.ipnstatus = 'A'
                    and sic.suoid = " . (int) $suoid . "
                    union
                    select ipn.*, aco.acoid, sic.suoid, aco.acoquantidade, mpn.mpncod::int, mpn.mpndsc
                    from public.indicadorpnc ipn
                        inner join public.metapnc mpn on mpn.mpnid = ipn.mpnid
                        left join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                        left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = $suoid and acostatus = 'A'
                    where ipn.prsano = '$exercicio'
                    and ipn.ipnstatus = 'A'
                    and sic.suoid is null
                    and ipn.ipncod != '0'
                    order by mpncod, ipncod, ipndsc";

            return $this->carregar($sql);
        }

        return [];
    }

    public function getMetasPncComPi($suoid, $janid = null, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select distinct ac.prsano, ac.suoid, mpn.mpncod::int, mpn.mpndsc, mpn.mpnid, count(pic.pliid) pis, count(ac.acoid) acompanhamento
                    from (
                        select ipn.ipnid, ipn.mpnid, ipn.ipndsc, ipn.ipnstatus, ipn.prsano, ipn.ipncod, aco.acoid, sic.suoid, aco.acoquantidade
                        from public.indicadorpnc ipn
                            inner join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                            left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = sic.suoid and acostatus = 'A' and aco.janid = " . (int) $janid . "
                        where ipn.prsano = '$exercicio'
                        and ipn.ipnstatus = 'A'
                        and sic.suoid = $suoid
                        union
                        select ipn.ipnid, ipn.mpnid, ipn.ipndsc, ipn.ipnstatus, ipn.prsano, ipn.ipncod, aco.acoid, $suoid, aco.acoquantidade
                        from public.indicadorpnc ipn
                            left join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                            left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = $suoid and acostatus = 'A' and aco.janid = " . (int) $janid . "
                        where ipn.prsano = '$exercicio'
                        and ipn.ipnstatus = 'A'
                        and sic.suoid is null
                        and ipn.ipncod != '0'
                    ) ac
                        inner join public.metapnc mpn on mpn.mpnid = ac.mpnid
                        left join public.vw_subunidadeorcamentaria suo on suo.suoid = ac.suoid
                        left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.unicod = suo.unocod        
                        left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid and pic.ipnid = ac.ipnid        
                    group by ac.prsano, ac.suoid, mpn.mpncod, mpn.mpndsc, mpn.mpnid
                    order by mpncod;
                    ";

            return $this->carregar($sql);
        }

        return [];
    }

    public function getIndicadoresPncComPi($suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        if($suoid){
            $sql = "select ac.ipnid, ac.ipndsc, ac.ipncod, ac.ipndsc, ac.ipnstatus, ac.prsano, ac.acoid, ac.suoid, mpn.mpncod, mpn.mpndsc, count(pic.pliid) pis
                    from (
                        select ipn.ipnid, ipn.mpnid, ipn.ipndsc, ipn.ipnstatus, ipn.prsano, ipn.ipncod, aco.acoid, sic.suoid, aco.acoquantidade
                        from public.indicadorpnc ipn
                            inner join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                            left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = sic.suoid and acostatus = 'A'
                        where ipn.prsano = '$exercicio'
                        and ipn.ipnstatus = 'A'
                        and sic.suoid = $suoid
                        union
                        select ipn.ipnid, ipn.mpnid, ipn.ipndsc, ipn.ipnstatus, ipn.prsano, ipn.ipncod, aco.acoid, $suoid, aco.acoquantidade
                        from public.indicadorpnc ipn
                            left join spo.subunidadeindicadorpnc sic on sic.ipnid = ipn.ipnid
                            left join acompanhamento.acompanhamento aco on aco.ipnid = ipn.ipnid and aco.suoid = $suoid and acostatus = 'A'
                        where ipn.prsano = '$exercicio'
                        and ipn.ipnstatus = 'A'
                        and sic.suoid is null
                        and ipn.ipncod != '0'
                    ) ac
                        inner join public.metapnc mpn on mpn.mpnid = ac.mpnid
                        left join public.vw_subunidadeorcamentaria suo on suo.suoid = ac.suoid
                        left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.unicod = suo.unocod        
                        left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid and pic.ipnid = ac.ipnid        
                    group by ac.ipnid, ac.ipndsc, ac.ipncod, ac.ipndsc, ac.ipnstatus, ac.prsano, ac.acoid, ac.suoid, mpn.mpncod, mpn.mpndsc
                    order by mpn.mpncod::int, ac.ipncod, ac.ipndsc       
                    ";

            return $this->carregar($sql);
        }

        return [];
    }

    public function recuperarMedidas()
    {
        if($this->acoid){
            $medidas = (new Acompanhamento_Model_AcompanhamentoMedida())->recuperarTodos('*', ['acoid = ' . $this->acoid]);
            $aRetorno = [];
            foreach($medidas as $medida){
                $aRetorno[$medida['medid']] = $medida['medid'];
            }
            return $aRetorno;
        }

        return [];
    }
    
    public function associarAnexo($dados)
    {
        if($this->acoid){
            $mAcompanhamentoAnexo = new Acompanhamento_Model_AcompanhamentoArquivo();
            $mAcompanhamentoAnexo->excluirVarios("acoid = '$this->acoid'");

            if(isset($dados['listaAnexos']) && is_array($dados['listaAnexos'])){

                $mAcompanhamentoAnexo->acoid = $this->acoid;

                foreach($dados['listaAnexos'] as $arqid){
                    $mAcompanhamentoAnexo->arqid = $arqid;
                    $mAcompanhamentoAnexo->salvar();

                    $mAcompanhamentoAnexo->acaid = null;
                }
            }
        }
    }

}
