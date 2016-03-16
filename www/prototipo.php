<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */

// carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select * from painel.prototipopainel order by tema, etapa, categoria, tipo, idindicador";
$dados = $db->carregar($sql);
$dados = $dados ? $dados : array();
$dadosAgrupados = array();

$arAgrupadores = array( 'idtema' => 'tema', 'idetapa' => 'etapa', 'idcategoria' => 'categoria', 'idtipo' => 'tipo' );

$arAgrupador = array('idtema', 'idetapa');
$arAgrupador = array('idtema', 'idetapa');
$arAgrupador = array('idtema', 'idetapa', 'idcategoria', 'idtipo');

foreach ($dados as $dado) {
    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][$dado[$arAgrupador[3]]][] = $dado;

    $dadosAgrupados[$dado[$arAgrupador[0]]]['qtd']++;
    $dadosAgrupados[$dado[$arAgrupador[0]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[0]]];

    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]]['qtd']++;
    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[1]]];

    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]]['qtd']++;
    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[2]]];

    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][$dado[$arAgrupador[3]]]['qtd']++;
    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][$dado[$arAgrupador[3]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[3]]];
}

?>

<script src="library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>

<style type="text/css">
    #divRelatorio{
        margin: 10px;
    }

    .tabelaRelatorio{
        border-collapse: collapse;
        border-spacing: 1;
        margin-top: 5px;
        width: 100%;
        margin-bottom: 15px;
    }

    .relCabecalho td{
        border: 1px #000 solid;
        padding: 2px 5px;
        font-size: 12px;
        text-align: center;
        background: #263A71;
        color: #fff;
    }

    .relDados td{
        border: 1px #000 solid;
        padding: 2px 5px;
        font-size: 12px;
        font-weight: normal;
    }

    .relCliente{
        color: red;
        margin: 5px 0;
    }

    .relAgrupador{
        color: blue;
        margin-bottom: 5px;
    }

    .relAgrupador2{
        color: green;
        margin-bottom: 5px;
    }

    .relCampanha{
        color: #263A71;
    }

    .cliente{
        color: red;
        margin: 5px 0;
    }

    .agencia{
        color: green;
        margin-bottom: 5px;
    }

    .veiculo{
        color: #263A71;
    }

    .centro{
        text-align: center;
    }
</style>

<?php

echo '<table class="tabelaRelatorio" >';
montarRelatorio( $dadosAgrupados, $arAgrupadores, $arAgrupador, 0, $class = '' );
echo '</table>';

function montarRelatorio( $arDadosAgrupados, $arAgrupadores, $arAgrupador, $nrCount, $class ){


if( key_exists( $arAgrupador[$nrCount], $arAgrupadores ) ){
    foreach( $arDadosAgrupados as $id => $arDetalhe ){
        
        if($id == 'nome' || $id == 'qtd') continue;

        $nome = $arDadosAgrupados[$id]['nome'];
        $qtd = $arDadosAgrupados[$id]['qtd'];

        $cor = $nrCount + 6;
        ?>
        <tr style="background: #<?php echo $cor . $cor . $cor ?>; class="linha<?php echo $class; ?>">
            <td colspan="2" style="text-indent: <?php echo 20*$nrCount ?>px">
                <span class="ocultar-linha" data-ocultar="linha<?php echo $class; ?>"><?php echo X; ?></span> ->
                <?php echo $arAgrupadores[$arAgrupador[$nrCount]] . ' - ' . $nome . ' (' . $qtd . ')' ?>
            </td>
        </tr>
        <?php
        $class .= '_' .  $id;
        montarRelatorio( $arDetalhe, $arAgrupadores, $arAgrupador, $nrCount+1, $class );
    }
}
else{ ?>
    <tr class="relCabecalho" style="display: none">
        <td width="100px;">Indicadores</td>
        <td width="100px;">Qtd</td>
    </tr>

    <?php
    $cor = "#eee";
    foreach( $arDadosAgrupados as $id => $arDados ){

//ver($id, $arDados, d);
        if($id == 'nome' || $id == 'qtd'){ continue; }

        $cor = $cor == "#fff" ? "#eee" : "#fff" ?>
        <tr class="relDados" style="background: <?php echo $cor ?>; display: none">
            <td width="90%"><?php echo $arDados['indicador']; ?></td>
            <td>10</td>
        </tr>
    <?php }
    return true;
    }
}

?>

<script type="text/javascript">
    $(function(){
        $('.ocultar-linha').click(function(){
            var linha = $(this).attr('data-ocultar');
//            $('tr[class^=' + linha + ']').toggle();
        });
    });        
</script>