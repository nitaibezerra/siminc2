<?php
$perfis = pegaPerfilGeral();

if (in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis)) {
    $sqlGestorEst = "SELECT estuf
  FROM eja.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
// ver($sqlGestorEst,d);
    $uf = $db->pegaUm($sqlGestorEst);

    $whereGestorEst = "AND estuf = '{$uf}'";
    $whereGeralGestorEst = "AND lesuf = '{$uf}' ";
}

if (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfis)) {
    $sqlGestorMun = "SELECT muncod
  FROM eja.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
    $municipio = $db->carregar($sqlGestorMun);
    if ($municipio) {
        foreach ($municipio as $muni) {
            $municipios .= $muni['muncod'] . "','";
        }
        $municipiosGestor = substr($municipios, 0, -2);
        //ver($municipiosGestor,d);
        //condição adicionado para retornar somente sobre o municipio em questao
        $whereGestorMun = "AND muncod in ('{$municipiosGestor})";
        //Recupera UF associado ao usuario
        $sqlGestorMunUf = "SELECT
                                                estuf
                                            FROM
                                                territorios.municipio m
                                            WHERE
                                                1 =1 $whereGestorMun";

        $uf = $db->carregar($sqlGestorMunUf);
        foreach ($uf as $UF) {
            $ufGestor .= $UF['estuf'] . "','";
        }
        $ufGestorFinal = substr($ufGestor, 0, -2);

        $whereMunUf = "AND estuf IN ('{$ufGestorFinal})";
        $whereGeralGestorMun = "AND lotmuncod in ('{$municipiosGestor})";
    } else {
        echo "
                <script>
                    alert('É necessário ser associado(a) a um município, solicite ao Gestor Estadual.');
                    window.location = 'eja.php?modulo=inicio&acao=C';
                </script>
            ";
    }
}
if ($_REQUEST['carregarComboMunicipio'] == 'ok') {
    carregaComboMunicipio($_REQUEST['uf'], $whereGestorMun);
    die();
}

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
                   <br><b>Recursos Liberados</b><br>
                   Exercício: ' . $_SESSION["exercicioParametro"] . '</center><br>';

    $sql = $_SESSION['sqlPrincipal'];
    $listagem->setQuery($sql);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    //echo '<table><tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td>';
    echo '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
    echo '</body>';
    die();
}

function carregaComboMunicipio($estuf, $whereGestorMun = false) {
    global $db;
    $sqlComboAcoes = "SELECT
                                                m.muncod AS codigo,
                                                m.estuf || ' - ' || m.mundescricao AS descricao
                                            FROM
                                                territorios.municipio m
                                            WHERE
                                                m.estuf = '{$estuf}' {$whereGestorMun} 
                                            ORDER BY
                                                descricao";

    $combo = $db->carregar($sqlComboAcoes);
//                                                ver($combo,$ptresSe,d);
//combo_popup("ptres", $sqlComboAcoes, "A??es", "600x600", 0, array(), "", "S", false, false, 5, 400);
    ?>
    <select name="muncod[]" id="muncod" multiple class="form-control chosen-select-no-single" required="required" data-placeholder="Selecione">
        <option value=""></option>
    <?php
    foreach ($combo as $muni):
        ?>
            <option  value="<?php echo $muni['codigo'] ?>"><?php echo $muni['descricao'] ?></option>-->
        <?php endforeach; ?>
    </select> <?php
    }

    require (APPRAIZ . 'includes/library/simec/Listagem.php');
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    include_once APPRAIZ . "includes/funcoesspo_componentes.php";

    if ($_REQUEST['requisicao'] == 'excel') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    } else {
        $listagem = new Simec_Listagem();
    }
    if ($_REQUEST['tiporel'] == 'a' || !$_REQUEST['tiporel']) {
        $cabecalhoTabela = array(
            'UF',
            'Razão Social',
            'Esfera',
            'CNPJ',
            'Quantidade de alunos',
            'Valor R$',
            'Data da Geração do Lote'
        );
    } else {
        $cabecalhoTabela = array(
            'UF',
            'Quantidade de alunos',
            'Valor R$',
        );
    }
    $_SESSION['cabecalho_rl'] = $cabecalhoTabela;
    $listagem->setCabecalho($cabecalhoTabela);

    $listagem->addCallbackDeCampo('valor_total', 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('qnt_alunos', 'valor_total'));

    if ($_REQUEST['requisicao'] == 'listar' || $_REQUEST['requisicao'] == 'excel') {
        $whereGeralGestorMun = '';
        if ($_REQUEST['exercicioP']) {
            $whereExercicio .= " AND lesexercicio = '{$_REQUEST['exercicioP']}'";
        }

        if ($_REQUEST['uf']) {
            $ufF = str_replace(",", "','", $_REQUEST['uf']);
            $whereGeral .= " AND lesuf in ('{$ufF}')";
        }
    }

    if ($_REQUEST['tiporel'] == 'a' || !$_REQUEST['tiporel']) {
        if (!$whereExercicio) {
            $whereExercicio = " AND lesexercicio = '2015'";
        }

        $sqlPrincipal = <<<DML
SELECT lesuf, lesrazaosocial, lesesfera, lescnpj, lesqtaluno as qnt_alunos, lesvlparcela as valor_total,to_char(lesdtgerado,'dd/mm/yyyy') as data
  FROM eja.loteestado lot
WHERE lesstatus = 'A' {$whereExercicio} {$whereGeralGestorEst} {$whereGeralGestorMun} {$whereGeral}  
group by lesuf, lesqtaluno, lesnumparcela, lesvlparcela,lesdtgerado, lesrazaosocial, lesesfera, lescnpj
order by lesuf,   lesnumparcela,data
DML;
    } else {
        if (!$whereExercicio) {
            $whereExercicio = " AND lesexercicio = '2015'";
        }

        $sqlPrincipal = <<<DML
SELECT distinct lesuf, sum(lesqtaluno) as qnt_alunos, sum(lesvlparcela) as valor_total
  FROM eja.loteestado WHERE lesstatus = 'A' {$whereExercicio} {$whereGeralGestorEst} {$whereGeral} group by lesuf order by lesuf
DML;
    }

    if ($_REQUEST['exercicioP']) {
        $_SESSION["exercicioParametro"] = $_REQUEST['exercicioP'];
    } else {
        $_SESSION["exercicioParametro"] = $_SESSION['exercicio'];
    }
    $_SESSION["sqlPrincipal"] = $sqlPrincipal;
//ver($sqlPrincipal,d);
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
        var url = "eja.php?modulo=principal/relatorio/recursos_liberados&acao=A";
        var params = "&tiporelatorio=" + tipo;
        window.open(url + params, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }

    function tiposintetico(valor) {
        if (valor == 'a') {
            $('#municipio').css('display', 'block');
            $('#muncod_chosen').css('width', '100%');
        }
        if (valor == 's') {
            $('#municipio').css('display', 'none');
        }
    }
</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<div class="page-header">
    <h4 id="forms">Recursos Liberados - Estado - <?php
if (!$_REQUEST['exercicioP']) {
    echo $_SESSION['exercicio'];
} else {
    echo $_REQUEST['exercicioP'];
}
    ?></h4>
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao"value="listar">
            <input type="hidden" name="sql" id="sql"value="<?php echo $sqlPrincipal; ?>">
            <div class="form-group">
                <label for="inputExercicio" class="col-lg-2 control-label">Exercício:</label>
                <div class="col-lg-10" >
                    <input type="text" maxlength="4" style="width:80px;" class="normal form-control" name="exercicioP" id="exercicioP"  value="<?php echo $_REQUEST['exercicioP']; ?>" />
                </div>
            </div>
<!--            <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">Ano:</label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['ano']) {

                        $ano = $_REQUEST['ano'];
                    }
                    $sql = "
                                        SELECT DISTINCT lotexercicio codigo,
                                                        lotexercicio AS descricao
                                        FROM eja.lote
                                        WHERE lotexercicio IS NOT NULL
                                        ORDER BY codigo ASC
                                    ";
                    $db->monta_combo('ano', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'uf', null, isset($uf) ? $uf : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>-->
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
//                                            ver($sql,d);
                    $db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregaMuni', null, null, null, 'N', 'uf', null, isset($uf) ? $uf : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>
            <div id="municipio" style="display:<?php if ($_REQUEST['tiporel'] == 'a' || !$_REQUEST['tiporel']) {
                        echo 'block';
                    } else {
                        echo 'none';
                    } ?>">
      
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

