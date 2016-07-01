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
$dados = $db->carregar($sql);

//formata as datas. está sendo feito aqui pois
//qdo formata direto no sql, o where por data não funcionou corretamente
if(is_array($dados)){
    foreach($dados as $key => $value){
        if(!empty($value['dataentrada'])){
            $auxDataHora = explode(' ',$value['dataentrada']);
            $auxData = explode('-',$auxDataHora[0]);
            $auxHora = explode(':',$auxDataHora[1]);
            $dados[$key]['dataentrada'] = $auxData[2].'/'.$auxData[1].'/'.$auxData[0].' '.$auxHora[0].':'.$auxHora[1];
        }
        if(!empty($value['datasaida'])){
            $auxDataHora = explode(' ',$value['datasaida']);
            $auxData = explode('-',$auxDataHora[0]);
            $auxHora = explode(':',$auxDataHora[1]);
            $dados[$key]['datasaida'] = $auxData[2].'/'.$auxData[1].'/'.$auxData[0].' '.$auxHora[0].':'.$auxHora[1];
        }

    }
}

$agrup = monta_agp();
$col   = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados);
$r->setColuna($col);
$r->setTotNivel(true);
$r->setMonstrarTolizadorNivel(true);
$r->setBrasao(true);
$r->setEspandir(false);
$r->setTolizadorLinha(false);
echo $r->getRelatorio();

function monta_sql(){
    extract($_REQUEST);

    //inicia cláusula where
    $where = " WHERE 1=1 ";

    //completa cláusula where de acordo com filtros
    if(!empty($mvedatentrada) && !empty($mvedatsaida)){
        $mvedatentrada = formata_data_sql($mvedatentrada);
        $mvedatsaida = formata_data_sql($mvedatsaida);

        if(!empty($horaentrada)){
            $horaentrada = addslashes($horaentrada) . ":00";
        }
        else{
            $horaentrada = '00:00:00';
        }
        if(!empty($horasaida)){
            $horasaida = addslashes($horasaida) . ":00";
        }
        else{
            $horasaida = '00:00:00';
        }
        $mvedatentrada .= ' '.$horaentrada;
        $mvedatsaida .= ' '.$horasaida;
        $where .= " AND dataentrada >= '{$mvedatentrada}' AND datasaida <= '{$mvedatsaida}' ";
    }
    else if(!empty($mvedatentrada) && empty($mvedatsaida)){
        $mvedatentrada = formata_data_sql($mvedatentrada);
        if(!empty($horaentrada)){
            $horaentrada = addslashes($horaentrada) . ":00";
        }
        else{
            $horaentrada = '00:00:00';
        }
        $mvedatentrada .= ' '.$horaentrada;
        $where .= " AND dataentrada >= '{$mvedatentrada}' ";
    }
    else if(empty($mvedatentrada) && !empty($mvedatsaida)){
        $mvedatsaida = formata_data_sql($mvedatsaida);
        if(!empty($horasaida)){
            $horasaida = addslashes($horasaida) . ":00";
        }
        else{
            $horasaida = '00:00:00';
        }
        $mvedatsaida .= ' '.$horasaida;
        $where .= " AND datasaida <= '{$mvedatsaida}' ";
    }


    // monta o sql
    $sql = "SELECT * FROM ( ";
    $sql .= "SELECT visitante.vttnome AS nome,movimentacao.mveid AS id,";
    $sql .= "movimentacao.mvedatentrada AS dataentrada,movimentacao.mvedatsaida AS datasaida,";
    $sql .= "tipo.tpedsc AS tipo,marca.mcedsc AS marca,equipamento.eqmnumserie AS numeroserie ";
    $sql .= "FROM sca.equipamento equipamento ";
    $sql .= "JOIN sca.tipoequipamento tipo ON equipamento.tpeid=tipo.tpeid ";
    $sql .= "JOIN sca.marcaequipamento marca ON equipamento.mceid=marca.mceid ";
    $sql .= "JOIN sca.movimentacaoequipamento movimentacao ON equipamento.eqmid=movimentacao.eqmid ";
    $sql .= "JOIN sca.visitante visitante ON movimentacao.vttid=visitante.vttid ";
    $sql .= "UNION ";
    $sql .= "SELECT servidor.no_servidor AS nome,movimentacao.mveid AS id,";
    $sql .= "movimentacao.mvedatentrada AS dataentrada,movimentacao.mvedatsaida AS datasaida,";
    $sql .= "tipo.tpedsc AS tipo,marca.mcedsc AS marca,equipamento.eqmnumserie AS numeroserie ";
    $sql .= "FROM sca.equipamento equipamento ";
    $sql .= "JOIN sca.tipoequipamento tipo ON equipamento.tpeid=tipo.tpeid ";
    $sql .= "JOIN sca.marcaequipamento marca ON equipamento.mceid=marca.mceid ";
    $sql .= "JOIN sca.movimentacaoequipamento movimentacao ON equipamento.eqmid=movimentacao.eqmid ";
    $sql .= "JOIN sca.vwservidorativo servidor ON movimentacao.nu_matricula_siape=servidor.nu_matricula_siape ";
    $sql .= " ) tabela ";
    $sql .= $where;
    $sql .= "ORDER BY dataentrada ";


    return $sql;
}

function monta_agp(){


        $agp = array(
                    "agrupador" => array(
                        array(
                            "campo"=>"id",
                            "label"=>"Código",
                        )
                    ),
                    "agrupadoColuna" => array('nome','dataentrada','datasaida','tipo','marca','numeroserie')
        );


    return $agp;
}

function monta_coluna(){

        $coluna    = array(

                        array(
                              "campo" => "nome",
                                 "label" => "Nome"
                        ),
                        array(
                              "campo" => "dataentrada",
                                 "label" => "Entrada"
                        ),
                        array(
                              "campo" => "datasaida",
                                 "label" => "Sa&iacute;da"
                        ),
                        array(
                              "campo" => "tipo",
                                 "label" => "Tipo Equipamento"
                        ),
                        array(
                              "campo" => "marca",
                                 "label" => "Marca"
                        ),
                        array(
                              "campo" => "numeroserie",
                                 "label" => "N&uacute;mero S&eacute;rie"
                        ),


                );


    return $coluna;
}
?>
<script>
    Event.observe(window, 'load', function() {
        var trs = $$('table.tabela tr');
        for(var i = 3; i < trs.length; i++){
            trs[i].cells[0].style.display = 'none';;
        }
      });
</script>
</body>
</html>