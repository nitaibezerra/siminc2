<?php
$perfis = pegaPerfilGeral();

if ($_REQUEST['tiporelatorio']) {
    require (APPRAIZ . 'includes/library/simec/Listagem.php');
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $cabecalhoTabela = $_SESSION['cabecalho_rl'];

    $listagem->setCabecalho($cabecalhoTabela);
    $listagem->addCallbackDeCampo('valor_total', 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('qnt_alunos', 'valor_total'));
    //$listagem->addCallbackDeCampo(array('valor_total', 'qnt_alunos' ), 'alinhaParaEsquerda');


    echo '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('95') . ' 
                   <br><b>Quantitativos Alunos Público Prioritário</b><br><br><br>';

    $sql = $_SESSION['sqlPrincipal'];
    $listagem->setQuery($sql);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    //echo '<table><tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td>';
    echo '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
    echo '</body>';
    die();
}

require (APPRAIZ . 'includes/library/simec/Listagem.php');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/funcoesspo_componentes.php";

if ($_REQUEST['requisicao'] == 'excel') {
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
} else {
    $listagem = new Simec_Listagem();
}
//if ($_REQUEST['tiporel'] == 'a' || !$_REQUEST['tiporel']) {
    $cabecalhoTabela = array(
        'UF',
        'Público Prioritário',
        'Quantidade'
    );
//} else {
//    $cabecalhoTabela = array(
//        'UF',
//        'Quantidade de alunos',
//        'Valor R$',
//    );
//}
$_SESSION['cabecalho_rl'] = $cabecalhoTabela;
$listagem->setCabecalho($cabecalhoTabela);

$listagem->addCallbackDeCampo('valor_total', 'mascaraMoeda');
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('qnt_alunos', 'valor_total'));

if ($_REQUEST['requisicao'] == 'listar' || $_REQUEST['requisicao'] == 'excel') {
//    if ($_REQUEST['exercicioP']) {
//        $whereGeral .= " AND lotexercicio = '{$_REQUEST['exercicioP']}'";
//    }
//    else{
//         $whereGeral .= " AND lotexercicio = '2015'";
//    }

    if ($_REQUEST['uf']) {
        $ufF = str_replace(",", "','", $_REQUEST['uf']);
        $whereGeral .= " AND est.estuf in ('{$ufF}')";
    }
     if ($_REQUEST['publicop']) {
        $publicopP = str_replace(",", "','", $_REQUEST['publicop']);
        $whereGeral .= " AND ate.pupid in ('{$publicopP}')";
    }
}

if ($_REQUEST['ano']) {
    $anoQuery = $_REQUEST['ano'];
} else {
    $anoQuery = '2015';
}

$sqlPrincipal = "SELECT est.estuf AS uf,
       ppi.pupdescricao AS publicoprioritario,
       COUNT(ate.atecpf) AS quantidade
FROM eja.publicoprioritario ppi
INNER JOIN eja.alunoturmaeja ate ON ppi.pupid = ate.pupid
INNER JOIN eja.novaturmaeja nte ON nte.nteid=ate.nteid
INNER JOIN entidade.endereco ende ON ende.entid = nte.pk_cod_entidade
JOIN territorios.municipio mun ON mun.muncod = ende.muncod
JOIN territorios.estado est ON est.estuf = mun.estuf
WHERE ateexercicio = '{$anoQuery}'
  AND nteexercicio = '{$anoQuery}' {$whereGeral}
GROUP BY est.estuf,
         ppi.pupdescricao,
         ppi.pupid
ORDER BY est.estuf,
         ppi.pupid,
         quantidade;";
  
//            ver($sqlPrincipal,d);
$_SESSION["sqlPrincipal"] = $sqlPrincipal;
$listagem->setQuery($sqlPrincipal);

if ($_REQUEST['requisicao'] == 'excel') {

    $listagem->render();

    die();
}
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
    function carregaMuni(uf) {
        var params = '&carregarComboMunicipio=ok&uf=' + uf;
        $.post(window.location + params, function(response) {
            $('#muncod_chosen').remove();
            $('.muncodDiv').html(response);
            $(".chosen-select-no-single").chosen();
        });
    }

    jQuery(document).ready(function() {
        $('#buscar').click(function() {
            $('#requisicao').val('listar');
            $("#formPrincipal").submit();
        });
        $('#exportar').click(function() {
            $('#requisicao').val('excel');
            $("#formPrincipal").submit();
        });
        jQuery('#relatorio').click(function() {
            $('#requisicao').val('listar');
            $("#formPrincipal").submit();
            gerarRelatorio('html');
        });
    });

    //gerando relatório via pop, submetendo a mesma tela.
    function gerarRelatorio(tipo) {
        var url = "eja.php?modulo=principal/relatorio/quantitativo_alunos&acao=A";
        var params = "&tiporelatorio=" + tipo;
        window.open(url + params, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }

</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<div class="page-header">
    <h4 id="forms">Quantitativo de Alunos Público Prioritário </h4>
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao"value="listar">
            <input type="hidden" name="sql" id="sql"value="<?php echo $sqlPrincipal; ?>">
            <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">Público Prioritário:</label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['publicop']) {
                        $publicoP = $_REQUEST['publicop'];
                    }
                    $sql = "select pupid as codigo, pupdescricao as descricao from eja.publicoprioritario";
//                                            ver($sql,d);
                    $db->monta_combo('publicoP', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'publicoP', null, isset($publicoP) ? $publicoP : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">UF:</label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['uf']) {

                        $uf = $_REQUEST['uf'];
                    }
                    $sql = "
                                        SELECT DISTINCT
                                            e.estuf AS codigo,
                                            e.estuf || ' - ' || e.estdescricao AS descricao
                                        FROM
                                            territorios.estado e where 1 = 1  {$whereGestorEst} {$whereMunUf}
                                        ORDER BY
                                            codigo ASC
                                    ";
                    $db->monta_combo('uf', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'uf', null, isset($uf) ? $uf : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>
              <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">Ano:</label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['ano']) {

                        $ano = $_REQUEST['ano'];
                    }
                    $sql = "
                                        SELECT DISTINCT ateexercicio codigo,
                                                        ateexercicio AS descricao
                                        FROM eja.alunoturmaeja
                                        WHERE ateexercicio IS NOT NULL
                                        ORDER BY codigo ASC
                                    ";
                    $db->monta_combo('ano', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'uf', null, isset($uf) ? $uf : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <button class="btn btn-primary" id="buscar" type="button">Pesquisar</button>
                    <button class="btn btn-info" id="relatorio" type="button"><i class="glyphicon glyphicon-th-list"></i> Relatório </button>
                    <button id="exportar" class="btn btn-danger" type="button">Exportar XLS</button>
                </div>
            </div>
        </form>
    </fieldset>
</div>
<br><br>
<?php
// -- Saída do relatório
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?> 

