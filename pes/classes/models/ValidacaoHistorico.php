<?php

class Model_ValidacaoHistorico extends Abstract_Model {

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesvalidacaohistorico';

    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = 'vahcodigo';



    public function gerarHistorico($entcodigo)
    {
        $ano = AEXANO;
        $usucpf = $_SESSION['usucpforigem'];

        $sql = "
                    select distinct
                        tid.tidcodigo,
                        lco.lcocodigo,
                        valano as vahano,
                        val.entcodigo,
                        coalesce(valstatus, 'NI') as status,
                        now() as vahdata,
                        '{$usucpf}' as usucpf,
                        sum(cea.ceavalor) as vahvalor
                    from pes.pestipodespesa tid
                        inner join pes.pesconfigcontratodespesa ccd on tid.tidcodigo = ccd.tidcodigo
                        inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                        inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                        left  join pes.pescontrato con on con.tidcodigo = tid.tidcodigo and entcodigo = '{$entcodigo}'
                        left  join pes.pescelulaacompanhamento cea on cea.concodigo = con.concodigo and cea.ccdcodigo = ccd.ccdcodigo and ceaano = '{$ano}'
                        left  join pes.pesvalidacao val on val.tidcodigo = tid.tidcodigo and val.entcodigo = '{$entcodigo}' and valano = '{$ano}' and valmes = lco.lcocodigo
                    where ccdtipoconfig = 'CA'
                    and ccototaliza = 'S'
                    and valano is not null
                    group by tid.tidcodigo, valano, val.entcodigo, lco.lcocodigo, lco.lconome, valcodigo, valstatus
                    union all
                    select
                        12 as tidcod,
                        lco.lcocodigo,
                        valano as vahano,
                        val.entcodigo,
                        coalesce(valstatus, 'NI') as status,
                        now() as vahdata,
                        '{$usucpf}' as usucpf,
                        sum(canvalor) as vahvalor
                    from pes.peslinhacontrato lco
                        left join pes.pescelulaacompnatdespesa can
                            inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo and tidcodigo = 12 and cnd.entcodigo = '{$entcodigo}'
                        on can.canmes = lco.lcocodigo and cantipovalor = 'FN' and canano = '{$ano}'
                        left  join pes.pesvalidacao val on val.tidcodigo = 12 and val.entcodigo = '{$entcodigo}' and valano = '{$ano}' and valmes = lco.lcocodigo
                    where glccodigo = 1
                    and valano is not null
                    group by tidcod, lco.lcocodigo, vahano, val.entcodigo, lco.lconome, valcodigo, valstatus";

        $result = $this->_db->carregar($sql);

        if($result){
            $sql = "
                    INSERT INTO pes.pesvalidacaohistorico(
                        tidcodigo,
                        vahmes,
                        vahano,
                        entcodigo,
                        vahstatus,
                        vahdata,
                        usucpf,
                        vahvalor)
                    $sql";

            $result = $this->_db->executar($sql);

            if(!$result){
                $result = array('status' => false ,  'msg' => MSG012 . 'Não foi possível gerar o histórico.');
                return $result;
            }

            $this->_db->commit();

        }

        if($result)
            $result = array('status' => true ,  'msg' => MSG011);
        else
            $result = array('status' => false ,  'msg' => MSG012 . 'Pois não existe nenhuma valor preenchido ainda.');

        return $result;
    }

}
