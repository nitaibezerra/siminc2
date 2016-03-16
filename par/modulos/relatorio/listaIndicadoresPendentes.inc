<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 11/08/2015
 * Time: 09:27
 */

include_once APPRAIZ . "includes/classes/relatorio.class.inc";
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php
    monta_titulo('PAR - Lista de Indicadores Pendentes', '');

    $sql   = monta_sql($_REQUEST);
    $dados = $db->carregar($sql);
    $agrup = monta_agp();
    $col   = monta_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao(true);
    $r->setMonstrarTolizadorNivel(false);
    $r->setTolizadorLinha(false);
    $r->setTotNivel(false);
    $r->setEspandir(true);

    echo $r->getRelatorio();

    function monta_sql($dados){
        $where = $sql = '';
        if($dados['tipo'] == 'est'){
            $where = "where pne.estuf = '{$_SESSION['par']['estuf']}'";
        } else {
            $where = "where pne.muncod = '{$_SESSION['par']['muncod']}'";
        }
        if($where != ''){
            $sql = "select
                            sub.subid,
                            sub.subtitulo
                        from sase.pne pne
                        inner join sase.submeta sub on sub.subid = pne.subid and sub.substatus = 'A'
                        inner join sase.meta met on met.metid = sub.metid
                        {$where}
                        and case when
                                met.metid = 1 or
                                met.metid = 2 or
                                met.metid = 3 or
                                met.metid = 5 or
                                met.metid = 8 or
                                met.metid = 9
                            then case
                                when pne.muncod is not null then pne.pneano = 2010
                                when pne.estuf is not null then pne.pneano = 2012
                                else pne.pneano = 2010
                                 end
                            when
                                met.metid = 4
                            then pne.pneano = 2010
                            when
                                met.metid = 12 or
                                met.metid = 13 or
                                met.metid = 14 or
                                met.metid = 17
                            then pne.pneano = 2012
                            else pne.pneano = 2013
                            end
                        and pne.pnesemvalor = 't'
                        and (pne.pnevalormeta is null or pne.pnevalormeta = 0)
                        order by met.metid";

        }
        return $sql;
    }

    function monta_agp(){
        $agp = array(
            "agrupador" => array(
                array(
                    "campo" => "subid",
                    "label" => "Código"
                )
            ),
            "agrupadoColuna" => array(
                'subid',
                'subtitulo'
            )
        );
        return $agp;
    }

    function monta_coluna(){
        $coluna = array(
            array(
                "campo" => "subtitulo",
                "label" => "Indicador",
                "type"  => "string"
            )
        );
        return $coluna;
    }
?>