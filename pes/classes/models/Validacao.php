<?php

class Model_Validacao extends Abstract_Model {

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesvalidacao';

    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = 'valcodigo';

    public function carregarValidacaoPor($entcodigo, $tidcodigo , $valmes , $valano)
    {
        $sql = "SELECT * FROM $this->_names WHERE entcodigo = {$entcodigo} AND tidcodigo ={$tidcodigo} AND valmes = {$valmes} valano = {$valano} ORDER BY valano";
        
    }
    
    public function carregarDespesasPorEntidade($entcodigo ,  $ano = null)
    {
        if(!$ano) $ano = AEXANO;

        $sql = "select *
                from  (
                    select     distinct tid.tidcodigo, tid.tidnome,
                        lco.lcocodigo, lco.lconome,
                        valcodigo, coalesce(valstatus, 'NI') as status,
                        sum(cea.ceavalor) as total
                    from pes.pestipodespesa tid
                        inner join pes.pesconfigcontratodespesa ccd on tid.tidcodigo = ccd.tidcodigo
                        inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                        inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                        left  join pes.pescontrato con on con.tidcodigo = tid.tidcodigo and entcodigo = '{$entcodigo}'
                        left  join pes.pescelulaacompanhamento cea on cea.concodigo = con.concodigo and cea.ccdcodigo = ccd.ccdcodigo and ceaano = '{$ano}'
                        left  join pes.pesvalidacao val on val.tidcodigo = tid.tidcodigo and val.entcodigo = '{$entcodigo}' and valano = '{$ano}' and valmes = lco.lcocodigo
                    where ccdtipoconfig = 'CA'
                    and ccototaliza = 'S'
                    group by tid.tidcodigo, tid.tidnome, lco.lcocodigo, lco.lconome, valcodigo, valstatus
                    union all
                    select 12 as tidcod,
                        'Material de Consumo' as tidnome,
                        lco.lcocodigo, lco.lconome,
                        valcodigo, coalesce(valstatus, 'NI') as status,
                        sum(canvalor) as total
                    from pes.peslinhacontrato lco
                        left join pes.pescelulaacompnatdespesa can
                            inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo and tidcodigo = 12 and cnd.entcodigo = '{$entcodigo}'
                        on can.canmes = lco.lcocodigo and cantipovalor = 'FN' and canano = '{$ano}'
                        left  join pes.pesvalidacao val on val.tidcodigo = 12 and val.entcodigo = '{$entcodigo}' and valano = '{$ano}' and valmes = lco.lcocodigo
                    where glccodigo = 1
                    group by tidcod, tidnome, lco.lcocodigo, lco.lconome, valcodigo, valstatus
                ) as acompanhamento   
                order by tidnome, lcocodigo";

        $result = $this->_db->carregar($sql);
        return $result;
    }
    
    /**
     * 
     * @todo colocar constantes
     */
    public function carregarValorDespesa($entcodigo , $tidcodigo , $valano , $valmes)
    {                      
        if (K_DESPESA_APOIO_ADM == $tidcodigo || K_DESPESA_VIGILANCIA == $tidcodigo || K_DESPESA_LIMPEZA == $tidcodigo){
            $sql = "select DISTINCT cec.concodigo, con.contitulo, cec.cecvalor, cec.ccdcodigo, lco.lconome, tid.tidnome,  tid.tidcodigo,
                mff.ccdcodigofinanceiro -- , mff.ccdcodigofisico
                , coalesce(fisico.fisicopreenchido, 0) as fisicopreenchido
                from pes.pescontrato con
                inner join pes.pescelulacontrato cec on cec.concodigo = con.concodigo
                inner join pes.pesmapafisicofinanceiro  mff on mff.ccdcodigofinanceiro = cec.ccdcodigo          
                inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cec.ccdcodigo
                inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                inner join pes.pestipodespesa tid on tid.tidcodigo = con.tidcodigo
                left  join (
                               select
                               cecfis.concodigo, mffis.ccdcodigofinanceiro, count(cecfis.cecvalor) as fisicopreenchido
                               from pes.pescontrato confis
                                               inner join pes.pescelulacontrato cecfis on cecfis.concodigo = confis.concodigo
                                               inner join pes.pesmapafisicofinanceiro mffis on mffis.ccdcodigofisico = cecfis.ccdcodigo
                               where confis.entcodigo = {$entcodigo}
                               and confis.tidcodigo = {$tidcodigo}
                               and coalesce(cecfis.cecvalor, 0) != 0
                               group by cecfis.concodigo, mffis.ccdcodigofinanceiro
                ) fisico on fisico.ccdcodigofinanceiro = mff.ccdcodigofinanceiro and fisico.concodigo = cec.concodigo 
                where con.entcodigo = {$entcodigo}
                and con.tidcodigo = {$tidcodigo}
                and coalesce(cecvalor, 0) != 0   ";
                
        }elseif(K_DESPESA_MATERIAL_CONSUMO == $tidcodigo) {
            $sql = "select DISTINCT cnd.cndcodigo, cnd.entcodigo, cnd.tidcodigo, tid.tidnome, cnd.unicodigo, cnd.natcodigo, cnd.cndtitulo as contitulo, can.cancodigo, can.canano as ano, can.canmes as mes, can.canvalor, coalesce(fisico.canvalor, 0) as fisicopreenchido
                    from pes.pescontratonaturezadespesa cnd
                                    inner join pes.pescelulaacompnatdespesa can on can.cndcodigo = cnd.cndcodigo and cantipovalor = 'FN'
                                    inner join pes.pestipodespesa tid on tid.tidcodigo = cnd.tidcodigo
                                    left join (
                                                   select cndf.cndcodigo, cndf.entcodigo, cndf.tidcodigo, cndf.unicodigo, cndf.natcodigo, cndf.cndtitulo, canf.cancodigo, canf.canano, canf.canmes, canf.canvalor
                                                   from pes.pescontratonaturezadespesa cndf
                                                                   inner join pes.pescelulaacompnatdespesa canf on canf.cndcodigo = cndf.cndcodigo and cantipovalor = 'FS'
                                                   where cndf.entcodigo = {$entcodigo}
                                                   and cndf.tidcodigo = {$tidcodigo}
                                                   and canf.canano = {$valano}
                                                   and coalesce(canf.canvalor, 0) != 0        
                                    ) fisico on fisico.cndcodigo = cnd.cndcodigo and fisico.unicodigo = cnd.unicodigo and fisico.natcodigo = cnd.natcodigo and fisico.canmes = can.canmes
                    where cnd.entcodigo = {$entcodigo}
                    and cnd.tidcodigo = {$tidcodigo}
                    and can.canano = {$valano}
                    and can.canmes = {$valmes}
                    and coalesce(can.canvalor, 0) != 0";
        } else {
            // Sql para validar se tem valor.
            $sql = "select DISTINCT cea.concodigo, con.contitulo, tid.tidcodigo, tid.tidnome, ccd.lcocodigo as mes, lco.lconome, cea.ceaano as ano, cea.ceavalor, cea.ccdcodigo,
                        mff.ccdcodigofinanceiro
                        -- , mff.ccdcodigofisico
                        , coalesce(fisico.fisicopreenchido, 0) as fisicopreenchido
                    from pes.pescontrato con
                        inner join pes.pescelulaacompanhamento  cea on cea.concodigo = con.concodigo
                        inner join pes.pesmapafisicofinanceiro  mff on mff.ccdcodigofinanceiro = cea.ccdcodigo
                        inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                        INNER JOIN pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                        inner join pes.pestipodespesa tid on tid.tidcodigo = con.tidcodigo
                        left  join (
                                       select
                                       ceafis.concodigo, mffis.ccdcodigofinanceiro, count(ceafis.ceavalor) as fisicopreenchido
                                       from pes.pescontrato confis
                                                       inner join pes.pescelulaacompanhamento ceafis on ceafis.concodigo = confis.concodigo
                                                       inner join pes.pesmapafisicofinanceiro mffis on mffis.ccdcodigofisico = ceafis.ccdcodigo
                                       where confis.entcodigo = {$entcodigo}
                                       and confis.tidcodigo = {$tidcodigo}
                                       and ceafis.ceaano = {$valano}
                                       and coalesce(ceafis.ceavalor, 0) != 0
                                       group by ceafis.concodigo, mffis.ccdcodigofinanceiro
                        ) fisico on fisico.ccdcodigofinanceiro = mff.ccdcodigofinanceiro and fisico.concodigo = cea.concodigo
                    where con.entcodigo = {$entcodigo}
                    and con.tidcodigo = {$tidcodigo}
                    and cea.ceaano = {$valano}
                    and ccd.lcocodigo = {$valmes}
                    and coalesce(ceavalor, 0) != 0";
        }
        $result = $this->_db->carregar($sql);
        return $result;
    }

}
