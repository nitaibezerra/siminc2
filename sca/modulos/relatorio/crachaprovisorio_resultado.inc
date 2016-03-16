<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">

<?php

ini_set("memory_limit", "1024M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$sql   = monta_sql();
$agrup = monta_agp();
$dados = $db->carregar($sql);
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setColuna($col);
$r->setAgrupador($agrup, $dados);
$r->setTotNivel(false);
$r->setBrasao(true);
$r->setMonstrarTolizadorNivel(false);
$r->setTolizadorLinha(false);
$r->setTotalizador(false);
$r->setTextoTotalizador();
$r->setEspandir($_REQUEST['expandir']);
echo $r->getRelatorio();

function monta_sql(){

    extract($_REQUEST);

    //data
    $dtinicio = addslashes($dtinicio);
    $dtfim = addslashes($dtfim);
    $dtinicio = formata_data_sql($dtinicio);
    $dtfim = formata_data_sql($dtfim);

    // hora
    $hrinicio = addslashes($hrinicio);
    $hrfim = addslashes($hrfim);
    if(empty($hrinicio)){
        $hrinicio = "00:00:00";
    }
    else{
        $hrinicio .= ':00';
    }
    if(empty($hrfim)){
        $hrfim = "00:00:00";
    }
    else{
        $hrfim .= ':00';
    }

    // selecionado os crachas
    $sql = "select cp.cpsid, cp.nu_matricula_siape, s.no_servidor, cp.vstnumcracha, to_char(cp.cpsdatentrada, 'DD/MM/YYYY HH24:MI') cpsdatentrada, to_char(cp.cpsdatsaida, 'DD/MM/YYYY HH24:MI') cpsdatsaida
            from sca.crachaprovisorio cp
            inner join sca.vwservidorativo s on s.nu_matricula_siape=cp.nu_matricula_siape
            where 1=1
            ";

    // se for mostrar somente os que nao deram baixa
    if($saida)
        $sql .= " and cpsdatsaida is null";

    // periodo
    if($dtinicio)
        $sql .= " and cpsdatentrada>='" . $dtinicio . " " . $hrinicio . "'";
    if($dtfim)
        $sql .= " and cpsdatsaida<='" . $dtfim . " " . $hrfim . "'";

    $sql .= ' order by cp.cpsid asc';

    return $sql;
}

function monta_agp(){

    $agp = array(
        "agrupador" => array(
            array(
                "campo" => "cpsid",
                "label" => "Código "
            )
        ),
        "agrupadoColuna" => array(
            'nu_matricula_siape',
            'no_servidor',
            'vstnumcracha',
            'cpsdatentrada',
            'cpsdatsaida')
    );

    return $agp;

}

function monta_coluna(){

    $coluna = array(
        array(
            "campo" => "nu_matricula_siape",
            "label" => "Matrícula SIAPE",
            "type" => 'string',
        ),
        array(
            "campo" => "no_servidor",
            "label" => "Nome",
            "type" => 'string',
        ),
        array(
            "campo" => "vstnumcracha",
            "label" => "Crachá",
            "type" => 'string'
        ),
        array(
            "campo" => "cpsdatentrada",
            "label" => "Entrada",
            "type" => 'string'
        ),
        array(
            "campo" => "cpsdatsaida",
            "label" => "Saída",
            "type" => 'string'
        ),
    );

    return $coluna;

}
?>
<script>
    Event.observe(window, 'load', function() {
        var trs = $$('table.tabela tr');
        for(var i = 3; i < trs.length; i++){
            trs[i].cells[0].style.display = 'none';
        }
      });
</script>
</body>
</html>

