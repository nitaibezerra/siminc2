<?php
ini_set("memory_limit", "64M");
header('Content-type: text/html; charset="iso-8859-1"', true);

$obrid = $_REQUEST['obrid'];

function verificaExistenciaDadoMobiliarioObra($obrid) {
    global $db;
    $sql = 'SELECT moeid FROM obras2.mobiliarioempenhado WHERE obrid = ' . $obrid . ' ';
    $resposta = $db->pegaLinha($sql);
    return $resposta;
}

function montaLista($obrid) {
    global $db;

    $acao = "'<center>
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarDadoMobiliario(\'' || moeid || '\');\"
                            title=\"Alterar\">
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/excluir.gif\"
                            style=\"cursor: pointer; margin-left: 3px;\"
                            onclick=\"javascript: excluirDadoMobiliario(\'' || moeid || '\');\"
                            title=\"Excluir\">
              </center>'";

    $sql = " 
            SELECT 
                    " . $acao . " as acao, 
                    moeid,
                    obrid,
                    moeconveniotipo,
                    moeorigemdado,
                    
                    CASE WHEN moeempenhadoflag = 'S'
                         THEN 'Sim'
                         ELSE 'N&atilde;o'
                    END as moeempenhadoflag,
                    CASE WHEN moeinaugrepmecflag = 'S'
                         THEN 'Sim'
                         ELSE 'N&atilde;o'
                    END as moeinaugrepmecflag,
                    CASE WHEN moeconvenioimobflag = 'S'
                         THEN 'Sim'
                         ELSE 'N&atilde;o'
                    END as moeconvenioimobflag,
                    CASE WHEN moeadquiridoflag = 'S'
                         THEN 'Sim'
                         ELSE 'N&atilde;o'
                    END as moeadquiridoflag,
                    CASE WHEN moefuncionamentoflag = 'S'
                         THEN 'Sim'
                         ELSE 'N&atilde;o'
                    END as moefuncionamentoflag,
                    
                    to_char( moedtconclusaoobra, 'DD/MM/YYYY')   as moedtconclusaoobra,
                    to_char( moedtiniciofunc, 'DD/MM/YYYY')      as moedtiniciofunc,
                    to_char( moedtprevinauguracao, 'DD/MM/YYYY') as moedtprevinauguracao,
                    to_char( moedtpreviniciofunc, 'DD/MM/YYYY')  as moedtpreviniciofunc,
                    
                    moeqtdcriancacrecheparcialefetivo,
                    moeqtdcriancacrecheintegralefetivo,
                    moeqtdcriancapreparcialefetivo,
                    moeqtdcriancapreintegralefetivo,
                    moeqtdcriancacrecheparcialprevisao,
                    moeqtdcriancacrecheintegralprevisao,
                    moeqtdcriancapreparcialprevisao,
                    moeqtdcriancapreintegralprevisao,
                    moepercexecutado,
                    moecodinep,
                    moecodinep,
                    moenumprocesso
            FROM 
                    obras2.mobiliarioempenhado 
            WHERE 
                obrid = " . $obrid . "
            AND 
                moestatus = 'A'
            ORDER BY
                moeid
           ";

    try {
        $arr = $db->carregar($sql);
    } catch (Exception $ex) {
        $arr = false;
        print("<script>
                        alert('Erro ao carregar a tabela com os dados!'); 
                        //window.close();
                  </script>");
    }

    $arr = $db->carregar($sql);

    if ($arr == false) {
        $str_tabela_lista = '';
    } else {
        
        $titulos_tabela = ' 
                            <tr>
                                <td class="subtitulocentro" colspan="15">Lista de Pagamentos</td>
                            </tr>
                            <tr>
                                <th style="width: 50px" rowspan="3">Ação</th>
                                <th>Origem do dado</th>
                                <th>Data de conclusão da Obra</th>
                                <th>Inauguração c/ representante MEC</th>
                                <th>Código INEP</th>
                                <th>Número do processo Imobiliário</th>
                                <th>Convênio Imobiliário</th>
                                <th>Mobiliário empenhado</th>
                                <th>Tipo do Convênio</th>
                                <th>Mobiliário já Adquirido?</th>
                                <th>Está em funcionamento?</th>
                                <th>Data Início de Funcionamento</th>
                                <th>Previsão de Inauguração</th>
                                <th>Previsão de Início de Funcionamento</th>
                                <th>Percentual executado da Obra</th>
                            </tr>
                            <tr>
                                <th colspan="14">Crianças Atendidas</th>
                            </tr>
                            <tr>
                                <th>             Efetivo Creche Parcial</th>
                                <th>             Efetivo Creche Integral</th>
                                <th>             Efetivo Pré-Escola Parcial</th>
                                <th colspan="3"> Efetivo Pré-Escola Integral</th>
                                <th colspan="2"> Previsão Creche Parcial</th>
                                <th colspan="2"> Previsão Creche Integral</th>
                                <th colspan="2"> Previsão Pré-Escola Parcial</th>
                                <th colspan="2"> Previsão Pré-Escola Integral</th>
                            </tr>
                           ';
        
        $topo_tabela_lista = ' 
                                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="listaDadosMobiliario">
                                    <thead>
                                        '.$titulos_tabela.' 
                                    </thead>
                                    <tbody style="text-align: center">
                                             ';

        $str_tabela_lista = '';

        $c = 0;
        foreach ($arr as $k => $v) {

            if ($c % 2 == 0) {
                $bg_color = '#F7F7F7';
            } else {
                $bg_color = '#E0E0E0';
            }
            
            $c++;
            
            if($c % 10 == 0){
                $str_tabela_lista .= $titulos_tabela;
            }

            //Tipo CO = Convênio, AD = Aditivo e TC = Termo de compromisso
            switch ($v['moeconveniotipo']) {
                case 'CO':
                    $tipo_convenio = 'Convênio';
                    break;
                case 'AD':
                    $tipo_convenio = 'Aditivo';
                    break;
                case 'TC':
                    $tipo_convenio = 'Termo de compromisso';
                    break;
            }

            switch ($v['moeorigemdado']) {
                case 1:
                    $origem_dado = 'Declaração do município do módulo Proinfancia Manutenção';
                    break;
                case 2:
                    $origem_dado = 'Informações cadastradas pelo FNDE';
                    break;
                case 3:
                    $origem_dado = 'Informações alimentadas pelo município';
                    break;
            }

            $str_tabela_lista .= '<tr id="tr1_' . $v['moeid'] . '" bgcolor="' . $bg_color . '">';
            $str_tabela_lista .= '  <td style="width: 50px" rowspan="3">' . $v['acao'] . '</td>';
            $str_tabela_lista .= '  <td>' . $origem_dado . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moedtconclusaoobra'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moeinaugrepmecflag'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moecodinep'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moenumprocesso'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moeconvenioimobflag'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moeempenhadoflag'] . '</td>';
            $str_tabela_lista .= '  <td>' . $tipo_convenio . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moeadquiridoflag'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moefuncionamentoflag'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moedtiniciofunc'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moedtprevinauguracao'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moedtpreviniciofunc'] . '</td>';
            $str_tabela_lista .= '  <td>' . number_format($v['moepercexecutado'], 2, ',', '') . '%</td>';
            $str_tabela_lista .= '</tr>';

            $str_tabela_lista .= '<tr id="tr2_' . $v['moeid'] . '" bgcolor="#FFFFFF"><td colspan="14" style="text-align: center; color: #505050"><b>Crianças Atendidas</b></td></tr>';

            $str_tabela_lista .= '<tr id="tr3_' . $v['moeid'] . '" bgcolor="' . $bg_color . '">';
            $str_tabela_lista .= '  <td>' . $v['moeqtdcriancacrecheparcialefetivo'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moeqtdcriancacrecheintegralefetivo'] . '</td>';
            $str_tabela_lista .= '  <td>' . $v['moeqtdcriancapreparcialefetivo'] . '</td>';
            $str_tabela_lista .= '  <td colspan="3">' . $v['moeqtdcriancapreintegralefetivo'] . '</td>';
            $str_tabela_lista .= '  <td colspan="2">' . $v['moeqtdcriancacrecheparcialprevisao'] . '</td>';
            $str_tabela_lista .= '  <td colspan="2">' . $v['moeqtdcriancacrecheintegralprevisao'] . '</td>';
            $str_tabela_lista .= '  <td colspan="2">' . $v['moeqtdcriancapreparcialprevisao'] . '</td>';
            $str_tabela_lista .= '  <td colspan="2">' . $v['moeqtdcriancapreintegralprevisao'] . '</td>';
            $str_tabela_lista .= '</tr>';
        }

        $rodape_tabela_lista = '</tbody>
                            </table>';
    }

    $lista = $topo_tabela_lista . $str_tabela_lista . $rodape_tabela_lista;

    return $lista;
}

function salvaDados() {

    global $db;
    $op   = $_POST['operacao'];
    $sql  = '';
    $tipo = ($_POST["moeempenhadoflag"] == 'S') ? $_POST["moeconveniotipo"] : '';

    switch ($op) {
        case 'salvar':
            $sql = " INSERT INTO obras2.mobiliarioempenhado 
                     ( 
                        obrid,
                        moeempenhadoflag,
                        moeconveniotipo,
                        moedtinclusao,
                        moedtupdate,
                        usucpf,
                        moestatus,
                        moeinaugrepmecflag,
                        moeconvenioimobflag,
                        moeadquiridoflag,
                        moefuncionamentoflag,
                        moeorigemdado,
                        moedtconclusaoobra,
                        moedtiniciofunc,
                        moedtprevinauguracao,
                        moedtpreviniciofunc,
                        moeqtdcriancacrecheparcialefetivo,
                        moeqtdcriancacrecheintegralefetivo,
                        moeqtdcriancapreparcialefetivo,
                        moeqtdcriancapreintegralefetivo,
                        moeqtdcriancacrecheparcialprevisao,
                        moeqtdcriancacrecheintegralprevisao,
                        moeqtdcriancapreparcialprevisao,
                        moeqtdcriancapreintegralprevisao,
                        moepercexecutado,
                        moecodinep,
                        moenumprocesso
                     )
                     VALUES 
                     ( 
                         " . $_POST['obrid'] . ",
                        '" . $_POST["moeempenhadoflag"] . "',
                        '" . $tipo . "',
                        '" . date('Y-m-d') . "',
                        '" . date('Y-m-d') . "',
                        '" . $_SESSION['obras2']['acessocpf'] . "',
                        'A',
                        '" . $_POST['moeinaugrepmecflag'] . "',
                        '" . $_POST['moeconvenioimobflag'] . "',
                        '" . $_POST['moeadquiridoflag'] . "',
                        '" . $_POST['moefuncionamentoflag'] . "',
                        '" . $_POST['moeorigemdado'] . "',
                        '" . $_POST['moedtconclusaoobra'] . "',
                        '" . $_POST['moedtiniciofunc'] . "',
                        '" . $_POST['moedtprevinauguracao'] . "',
                        '" . $_POST['moedtpreviniciofunc'] . "',                            
                         " .((trim($_POST['moeqtdcriancacrecheparcialefetivo']) == '' )   ? 0 : $_POST['moeqtdcriancacrecheparcialefetivo']   ). ",
                         " .((trim($_POST['moeqtdcriancacrecheintegralefetivo']) == '' )  ? 0 : $_POST['moeqtdcriancacrecheintegralefetivo']  ). ",
                         " .((trim($_POST['moeqtdcriancapreparcialefetivo']) == '' )      ? 0 : $_POST['moeqtdcriancapreparcialefetivo']      ). ",
                         " .((trim($_POST['moeqtdcriancapreintegralefetivo']) == '' )     ? 0 : $_POST['moeqtdcriancapreintegralefetivo']     ). ",
                         " .((trim($_POST['moeqtdcriancacrecheparcialprevisao']) == '' )  ? 0 : $_POST['moeqtdcriancacrecheparcialprevisao']  ). ",
                         " .((trim($_POST['moeqtdcriancacrecheintegralprevisao']) == '' ) ? 0 : $_POST['moeqtdcriancacrecheintegralprevisao'] ). ",
                         " .((trim($_POST['moeqtdcriancapreparcialprevisao']) == '' )     ? 0 : $_POST['moeqtdcriancapreparcialprevisao']     ). ",
                         " .((trim($_POST['moeqtdcriancapreintegralprevisao']) == '' )    ? 0 : $_POST['moeqtdcriancapreintegralprevisao']    ). ",
                         ". str_replace(',','.',$_POST['moepercexecutado']).",
                         '" . $_POST['moecodinep'] . "',
                         '" . $_POST['moenumprocesso'] . "'
                     )
                 ";

            try {
                $db->executar($sql);
                $db->commit();
                die("<script>
                            alert('Registro gravado com sucesso!'); 
                            var url = '/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A' + '&obrid=' + ".$_POST['obrid']." ;
                            window.location.href = url;
                     </script>");
            } catch (Exception $ex) {
                die("<script>
                            alert('Erro ao realizar a operação de Inserir!'); 
                            var url = '/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A' + '&obrid=' + ".$_POST['obrid']." ;
                            window.location.href = url;
                     </script>");
            }
            break;
        case 'update':
            $sql = "
                        UPDATE obras2.mobiliarioempenhado 
                        SET 
                            obrid                               = " . $_POST['obrid'] . ",
                            moeempenhadoflag                    = '" . $_POST["moeempenhadoflag"] . "',
                            moeconveniotipo                     = '" . $tipo . "',
                            moedtupdate                         = '" . date('Y-m-d') . "',
                            usucpf                              = '" . $_SESSION['obras2']['acessocpf'] . "',
                            moestatus                           = 'A',
                            -- Campos Novos
                            moeinaugrepmecflag                  ='" . $_POST['moeinaugrepmecflag'] . "',
                            moeconvenioimobflag                 ='" . $_POST['moeconvenioimobflag'] . "',
                            moeadquiridoflag                    ='" . $_POST['moeadquiridoflag'] . "',
                            moefuncionamentoflag                ='" . $_POST['moefuncionamentoflag'] . "',
                            moeorigemdado                       ='" . $_POST['moeorigemdado'] . "',

                            moenumprocesso                      ='" . $_POST['moenumprocesso'] . "',
                            moecodinep                          ='" . $_POST['moecodinep'] . "',

                            moedtconclusaoobra                  = " . (($_POST['moedtconclusaoobra'] != '')   ? "'".$_POST['moedtconclusaoobra']."'"   : 'NULL' ). ",
                            moedtiniciofunc                     = " . (($_POST['moedtiniciofunc'] != '')      ? "'".$_POST['moedtiniciofunc']."'"      : 'NULL' ). ",
                            moedtprevinauguracao                = " . (($_POST['moedtprevinauguracao'] != '') ? "'".$_POST['moedtprevinauguracao']."'" : 'NULL' ). ",
                            moedtpreviniciofunc                 = " . (($_POST['moedtpreviniciofunc'] != '')  ? "'".$_POST['moedtpreviniciofunc']."'"  : 'NULL' ). ",
                                
                            -- Campos de Crianças Atendidas
                            moeqtdcriancacrecheparcialefetivo   = " . ((trim($_POST['moeqtdcriancacrecheparcialefetivo']) == '' )   ? 0 : $_POST['moeqtdcriancacrecheparcialefetivo']   ). ",
                            moeqtdcriancacrecheintegralefetivo  = " . ((trim($_POST['moeqtdcriancacrecheintegralefetivo']) == '' )  ? 0 : $_POST['moeqtdcriancacrecheintegralefetivo']   ). ",
                            moeqtdcriancapreparcialefetivo      = " . ((trim($_POST['moeqtdcriancapreparcialefetivo']) == '' )      ? 0 : $_POST['moeqtdcriancapreparcialefetivo']   ). ",
                            moeqtdcriancapreintegralefetivo     = " . ((trim($_POST['moeqtdcriancapreintegralefetivo']) == '' )     ? 0 : $_POST['moeqtdcriancapreintegralefetivo']   ). ",
                            moeqtdcriancacrecheparcialprevisao  = " . ((trim($_POST['moeqtdcriancacrecheparcialprevisao']) == '' )  ? 0 : $_POST['moeqtdcriancacrecheparcialprevisao']   ). ",
                            moeqtdcriancacrecheintegralprevisao = " . ((trim($_POST['moeqtdcriancacrecheintegralprevisao']) == '' ) ? 0 : $_POST['moeqtdcriancacrecheintegralprevisao']   ). ",
                            moeqtdcriancapreparcialprevisao     = " . ((trim($_POST['moeqtdcriancapreparcialprevisao']) == '' )     ? 0 : $_POST['moeqtdcriancapreparcialprevisao']   ). ",
                            moeqtdcriancapreintegralprevisao    = " . ((trim($_POST['moeqtdcriancapreintegralprevisao']) == '' )    ? 0 : $_POST['moeqtdcriancapreintegralprevisao']   ). ",
                                
                            moepercexecutado                    = ". str_replace(',','.',$_POST['moepercexecutado'])."

                        WHERE
                            obrid = " . $_POST['obrid'] . " 
                        AND 
                            moeid = " . $_POST['moeid'] . " ;

                     ";
            try {
                $db->executar($sql);
                $db->commit();
                die("<script>
                            alert('Operação realizada com sucesso!'); 
                            var url = '/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A' + '&obrid=' + ".$_POST['obrid']." ;
                            window.location.href = url;
                     </script>");
            } catch (Exception $ex) {
                die("<script>
                            alert('Erro ao realizar a operação de Update!'); 
                            var url = '/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A' + '&obrid=' + ".$_POST['obrid']." ;
                            window.location.href = url;
                     </script>");
            }

            break;
        case 'excluir':
            $sql = " 
                        UPDATE obras2.mobiliarioempenhado 
                        SET 
                            moestatus = 'I'
                        WHERE
                            moeid = " . $_POST['moeid'] . '';
            try {
                $db->executar($sql);
                $db->commit();
                die("<script>
                            alert('Dado excluido com sucesso!'); 
                            var url = '/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A' + '&obrid=' + ".$_POST['obrid']." ;
                            window.location.href = url;
                     </script>");
            } catch (Exception $ex) {
                die("<script>
                            alert('Erro ao realizar a operação de Exclusão!'); 
                            var url = '/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A' + '&obrid=' + ".$_POST['obrid']." ;
                            window.location.href = url;
                     </script>");
            }

            break;
        default:
            break;
    }
}

$exists = verificaExistenciaDadoMobiliarioObra($obrid);

if (!$exists) {
    $fl_mostra_lista = false;
    $lista = '';
} else {
    $fl_mostra_lista = true;
    $lista = montaLista($obrid);
}

// Verifica se existe o dado mobiliário cadastrado relativo a obra
// Se sim, preenche o formulário e seta para update a ação
// Se não, mostra o formulário em estado inicial a ação é salvar
// Verifica se foi feito alguma modificação via POST
// Se sim, analisa a ação (insert/update) e executa a ação e fecha a janela
// Se não, mostra o fomulário normal de acordo com a verificação acima

if ($_POST['operacao'] == 'salvar' || $_POST['operacao'] == 'update' || $_POST['operacao'] == 'excluir') {
    salvaDados();
}

$somenteLeitura  = "";
$obra            = new Obras($obrid);
$percentual_obra = $obra->obrpercentultvistoria;

$arrDadosEiManutencao = importaDadosEiManutencao($obrid);
extract($arrDadosEiManutencao);

?>

<html>
    <head>
        <title>Inserir Dado Mobiliário</title>
        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
        <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
        <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    </head>
    <body topmargin="0" leftmargin="0">
        <form method="post" action="" name="form_mobiliario" id="form_mobiliario" enctype="multipart/form-data">

            <input type="hidden" name="operacao" id="operacao" value="salvar" />
            <input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid; ?>">
            <input type="hidden" name="moeid" id="moeid" value="0">

            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

                <tr>
                    <td class="subtitulocentro" colspan="2">Mobiliário</td>
                </tr>

                <tr>
                    <td class="subtitulodireita" width="140px">Obra:</td>
                    <td>
                        <?php echo '<b>' . $obrid . '</b>'; ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Percentual executado da Obra:</td>
                    <td>
                        <?php echo campo_texto('moepercexecutado', 'N', 'S', '', 10, 6, '[##,##]', '', 'left', '', 0, 'id="moepercexecutado"','',number_format($percentual_obra,2,',','')); ?> %
                    </td>
                </tr>

                <tr>
                    <td class="subtitulodireita" width="140px">Origem do dado:</td>
                    <td>
                        <?php
                        $arrOrigem = array(array('codigo' => '1', 'descricao' => 'Declaração do município do módulo Proinfancia Manutenção'),
                                           array('codigo' => '2', 'descricao' => 'Informações cadastradas pelo FNDE'),
                                           array('codigo' => '3', 'descricao' => 'Informações alimentadas pelo município'));

                        echo $db->monta_combo("moeorigemdado", $arrOrigem, "S", "Selecione...", "", "", '', "", "S", "moeorigemdado", true);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Data de conclusão de obra:</td>
                    <td>
                        <?php echo campo_data2('moedtconclusaoobra', 'S', true, '', 'S', '', ' ', '', ''); ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Inauguração c/ representante MEC:</td>
                    <td>
                        <input type="radio" name="moeinaugrepmecflag" id="moeinaugrepmecflag_s" value="S" /> Sim &nbsp;
                        <input type="radio" name="moeinaugrepmecflag" id="moeinaugrepmecflag_n" value="N" checked="checked" /> Não
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Código do INEP</td>
                    <td>
                        <input type="text" id="moecodinep" name="moecodinep" value="<?php echo $qcicodigoinep; ?>" />
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Convênio Imobiliário:</td>
                    <td>
                        <input type="radio" name="moeconvenioimobflag" id="moeconvenioimobflag_s" value="S" /> Sim &nbsp;
                        <input type="radio" name="moeconvenioimobflag" id="moeconvenioimobflag_n" value="N" checked="checked" /> Não
                    </td>
                </tr>

                <tr>
                    <td class="subtitulodireita" width="140px">Mobiliário empenhado:</td>
                    <td>
                        <input type="radio" name="moeempenhadoflag" id="moeempenhadoflag_s" value="S"  /> Sim &nbsp;
                        <input type="radio" name="moeempenhadoflag" id="moeempenhadoflag_n" value="N" checked="checked" /> Não
                    </td>
                </tr>
                <tr id="tr_tipo_moe" style="display: none">
                    <td class="subtitulodireita" width="140px">Tipo do Convênio:</td>
                    <td>
                        <?php
                        $arrTipos = array(array('codigo' => 'CO', 'descricao' => 'Convênio'),
                                          array('codigo' => 'AD', 'descricao' => 'Aditivo'),
                                          array('codigo' => 'TC', 'descricao' => 'Termo de compromisso'));

                        echo $db->monta_combo("moeconveniotipo", $arrTipos, "S", "Selecione...", "", "", '', "", "N", "moeconveniotipo", true);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Mobiliário já Adquirido?</td>
                    <td>
                        <input type="radio" name="moeadquiridoflag" id="moeadquiridoflag_s" value="S" /> Sim &nbsp;
                        <input type="radio" name="moeadquiridoflag" id="moeadquiridoflag_n" value="N" checked="checked" /> Não
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Número do Processo Mobiliário</td>
                    <td>
                        <input type="text" id="moenumprocesso" name="moenumprocesso" />
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Está em funcionamento?</td>
                    <td>
                        <input type="radio" name="moefuncionamentoflag" id="moefuncionamentoflag_s" value="S" /> Sim &nbsp;
                        <input type="radio" name="moefuncionamentoflag" id="moefuncionamentoflag_n" value="N" checked="checked" /> Não
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Data Início de Funcionamento:</td>
                    <td>
                        <?php echo campo_data2('moedtiniciofunc', 'S', true, '', 'N', '', ' ', $dtInicioAtendimento, ''); ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Previsão de Inauguração:</td>
                    <td>
                        <?php echo campo_data2('moedtprevinauguracao', 'S', true, '', 'S', '', ' ', '', ''); ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="140px">Previsão de Início de Funcionamento:</td>
                    <td>
                        <?php echo campo_data2('moedtpreviniciofunc', 'S', true, '', 'S', '', ' ', '', ''); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Crianças atendidas:</td>
                    <td> 
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 99%">
                            <tr>
                                <td colspan="4" style="border-bottom:1px solid #CCCCCC;"> <center> Efetivo </center> </td>
                                <td colspan="4" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Previsão </center> </td>
                            </tr>
                            <tr>
                                <td colspan="2" style="border-bottom:1px solid #CCCCCC;"> <center> Creche </center> </td>
                                <td colspan="2" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Pre-Escola </center> </td>
                                <td colspan="2" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Creche </center> </td>
                                <td colspan="2" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Pre-Escola </center> </td>
                            </tr>
                            <tr>
                                <td style="border-bottom:1px solid #CCCCCC;"> <center> Parcial </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Parcial </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Parcial </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Parcial </center> </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
                            </tr>
                            <tr>
                                <td style="border-bottom:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancacrecheparcialefetivo = $criancasAtendidas[6]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancacrecheparcialefetivo', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancacrecheparcialefetivo"','', $moeqtdcriancacrecheparcialefetivo); 
                                    ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancacrecheintegralefetivo = $criancasAtendidas[4]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancacrecheintegralefetivo', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancacrecheintegralefetivo"', '',$moeqtdcriancacrecheintegralefetivo); 
                                    ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancapreparcialefetivo = $criancasAtendidas[7]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancapreparcialefetivo', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancapreparcialefetivo"', '', $moeqtdcriancapreparcialefetivo); 
                                    ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancapreintegralefetivo = $criancasAtendidas[5]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancapreintegralefetivo', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancapreintegralefetivo"', '', $moeqtdcriancapreintegralefetivo); ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancacrecheparcialprevisao = $criancasAtendidas[2]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancacrecheparcialprevisao', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancacrecheparcialprevisao"'); 
                                    ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancacrecheintegralprevisao = $criancasAtendidas[0]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancacrecheintegralprevisao', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancacrecheintegralprevisao"'); 
                                    ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php 
                                    $moeqtdcriancapreparcialprevisao = $criancasAtendidas[3]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancapreparcialprevisao', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancapreparcialprevisao"'); 
                                    ?>
                                </td>
                                <td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
                                    <?php
                                    $moeqtdcriancapreintegralprevisao = $criancasAtendidas[1]['alaquantidade'];
                                    echo campo_texto('moeqtdcriancapreintegralprevisao', 'N', $somenteLeitura, '', 10, 14, '[.###]', '', 'left', '', 0, 'id="moeqtdcriancapreintegralprevisao"'); 
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                
                <tr bgcolor="#c0c0c0">
                    <td colspan="2" style="text-align: center">
                        <input type="button" value="Salvar" id="registraMobiliario" style="cursor:pointer;"/>
                        <input type="button" value="Cancelar Edição" id="cancelaEdicao"   style="display: none; cursor:pointer;"/>
                    </td>
                </tr>
            </table>
    </form>
<br />

<?php
    print($lista);
?>
<br />

<?php
importaFotosEiManutencao($obrid, $pinid);
?>

<script type="text/javascript">

    $(document).ready(function() {
        $("input[name='moeempenhadoflag']").click(function() {
            if ($("input[name='moeempenhadoflag']:checked").val() === 'S') {
                $("#tr_tipo_moe").show();
            } else {
                $("#tr_tipo_moe").hide();
            }
        });

        $('#registraMobiliario').click(function() {
            if (validaForm()) {
                $('#form_mobiliario').submit();
            }
        });

        validaForm = function() {
            var erro = false;
            if($('#moeorigemdado').val() == ''){
                alert('O campo Origem do Dado é obrigatório. ');
                $('#moeorigemdado').focus();
                erro = true;
            }
            
            if($('#moedtconclusaoobra').val() == ''){
                alert('O campo Data de conclusão da Obra é obrigatório. ');
                $('#moedtconclusaoobra').focus();
                erro = true;
            }
            if($('#moedtiniciofunc').val() == ''){
                alert('O campo Data de Início da Obra é obrigatório. ');
                $('#moedtiniciofunc').focus();
                erro = true;
            }
            if($('#moedtprevinauguracao').val() == ''){
                alert('O campo Previsão de Inauguração é obrigatório. ');
                $('#moedtprevinauguracao').focus();
                erro = true;
            }
            if($('#moedtpreviniciofunc').val() == ''){
                alert('O campo Previsão de Início de Funcionamento é obrigatório. ');
                $('#moedtpreviniciofunc').focus();
                erro = true;
            }
            
            if(!erro){                
                return true;
            }else{
                return false;
            }
        };

        excluirDadoMobiliario = function(moeid) {
            var r = window.confirm('O registro será excluido. Deseja continuar?');
            if (r) {
                $('#operacao').val('excluir');
                $('#moeid').val(moeid);
                $('#form_mobiliario').submit();
            }
        };
        
        var objDadoEmEdicao = {
                    moeid : 0,
                    obrid : 0,
                    moeempenhadoflag : '',
                    moeconveniotipo : '',
                    moedtinclusao : '',
                    moedtupdate : '',
                    usucpf : '',
                    moestatus : '',
                    moeinaugrepmecflag : '',
                    moeconvenioimobflag : '',
                    moeadquiridoflag : '',
                    moefuncionamentoflag : '',
                    moeorigemdado : '',
                    moedtconclusaoobra : '',
                    moedtiniciofunc : '',
                    moedtprevinauguracao : '',
                    moedtpreviniciofunc : '',
                    moeqtdcriancacrecheparcialefetivo : 0,
                    moeqtdcriancacrecheintegralefetivo : 0,
                    moeqtdcriancapreparcialefetivo : 0,
                    moeqtdcriancapreintegralefetivo : 0,
                    moeqtdcriancacrecheparcialprevisao : 0,
                    moeqtdcriancacrecheintegralprevisao : 0,
                    moeqtdcriancapreparcialprevisao : 0,
                    moeqtdcriancapreintegralprevisao: 0,
                    moepercexecutado: 0,
                    moecodinep: 0,
                    moenumprocesso: 0
        };
        
        resetObjDadoMobiliario = function() {
            objDadoEmEdicao.moeid                               = 0;
            objDadoEmEdicao.obrid                               = 0;
            objDadoEmEdicao.moeempenhadoflag                    = '';
            objDadoEmEdicao.moeconveniotipo                     = '';
            objDadoEmEdicao.moedtinclusao                       = '';
            objDadoEmEdicao.moedtupdate                         = '';
            objDadoEmEdicao.usucpf                              = '';
            objDadoEmEdicao.moestatus                           = '';
            objDadoEmEdicao.moeinaugrepmecflag                  = '';
            objDadoEmEdicao.moeconvenioimobflag                 = '';
            objDadoEmEdicao.moeadquiridoflag                    = '';
            objDadoEmEdicao.moefuncionamentoflag                = '';
            objDadoEmEdicao.moeorigemdado                       = '';
            objDadoEmEdicao.moedtconclusaoobra                  = '';
            objDadoEmEdicao.moedtiniciofunc                     = '';
            objDadoEmEdicao.moedtprevinauguracao                = '';
            objDadoEmEdicao.moedtpreviniciofunc                 = '';
            objDadoEmEdicao.moeqtdcriancacrecheparcialefetivo   = 0;
            objDadoEmEdicao.moeqtdcriancacrecheintegralefetivo  = 0;
            objDadoEmEdicao.moeqtdcriancapreparcialefetivo      = 0;
            objDadoEmEdicao.moeqtdcriancapreintegralefetivo     = 0;
            objDadoEmEdicao.moeqtdcriancacrecheparcialprevisao  = 0;
            objDadoEmEdicao.moeqtdcriancacrecheintegralprevisao = 0;
            objDadoEmEdicao.moeqtdcriancapreparcialprevisao     = 0;
            objDadoEmEdicao.moeqtdcriancapreintegralprevisao    = 0;
            objDadoEmEdicao.moepercexecutado                    = 0;
            objDadoEmEdicao.moecodinep                          = 0;
            objDadoEmEdicao.moenumprocesso                      = 0;
        };
        
        $('#cancelaEdicao').click(function() {
            var str_tr = '';
            var str_acao = '';

            str_acao = '<center>' +
                       '     <img' +
                       '             align="absmiddle"' +
                       '             src="/imagens/alterar.gif"' +
                       '             style="cursor: pointer"' +
                       '             onclick="javascript: alterarDadoMobiliario(\'' + objDadoEmEdicao.moeid + '\');"' +
                       '             title="Alterar">' +
                       '     <img' +
                       '             align="absmiddle"' +
                       '             src="/imagens/excluir.gif"' +
                       '             style="cursor: pointer; margin-left: 3px;"' +
                       '             onclick="javascript: excluirDadoMobiliario(\'' + objDadoEmEdicao.moeid + '\');"' +
                       '             title="Excluir">' +
                       ' </center>';
               
            var qtd_linhas = $('#listaDadosMobiliario > tbody > tr').length;

            var bg_color = '';
            
            if (qtd_linhas % 3 == 0) {
                bg_color = '#F7F7F7';
            } else {
                bg_color = '#E0E0E0';
            }
            
            str_tr  +='<tr id="tr1_' + objDadoEmEdicao.moeid +  '" bgcolor="' + bg_color  + '">';
            str_tr  +='  <td style="width: 50px" rowspan="3">' + str_acao +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moeorigemdado        + '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moedtconclusaoobra   +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moeinaugrepmecflag   +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moecodinep           +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moenumprocesso       +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moeconvenioimobflag  +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moeempenhadoflag     +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moeconveniotipo      + '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moeadquiridoflag     +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moefuncionamentoflag +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moedtiniciofunc      +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moedtprevinauguracao +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moedtpreviniciofunc  +  '</td>';
            str_tr  +='  <td>' + objDadoEmEdicao.moepercexecutado     +  '%</td>';
            str_tr  +='</tr>';

            str_tr  +='<tr id="tr2_' + objDadoEmEdicao.moeid +  '" bgcolor="#FFFFFF"><td colspan="13" style="text-align: center; color: #505050"><b>Crianças Atendidas</b></td></tr>';

            str_tr  +='<tr id="tr3_' + objDadoEmEdicao.moeid +  '" bgcolor="' + bg_color  + '">';
            str_tr  +='  <td>'             + objDadoEmEdicao.moeqtdcriancacrecheparcialefetivo +  '</td>';
            str_tr  +='  <td>'             + objDadoEmEdicao.moeqtdcriancacrecheintegralefetivo +  '</td>';
            str_tr  +='  <td>'             + objDadoEmEdicao.moeqtdcriancapreparcialefetivo +  '</td>';
            str_tr  +='  <td colspan="2">' + objDadoEmEdicao.moeqtdcriancapreintegralefetivo +  '</td>';
            str_tr  +='  <td colspan="2">' + objDadoEmEdicao.moeqtdcriancacrecheparcialprevisao +  '</td>';
            str_tr  +='  <td colspan="2">' + objDadoEmEdicao.moeqtdcriancacrecheintegralprevisao +  '</td>';
            str_tr  +='  <td colspan="2">' + objDadoEmEdicao.moeqtdcriancapreparcialprevisao +  '</td>';
            str_tr  +='  <td colspan="2">' + objDadoEmEdicao.moeqtdcriancapreintegralprevisao +  '</td>';
            str_tr  += '</tr>';
            
            $('#listaDadosMobiliario > tbody:last').append(str_tr);

            $('#cancelaEdicao').hide();
            resetCamposForm();
            resetObjDadoMobiliario();
               
        });
        
        resetCamposForm = function(){
            
                $('#moeid').val();
                $('#moeempenhadoflag_n').attr('checked','checked');
                $('#moeempenhadoflag_n').click().click();
                $('#moeinaugrepmecflag_n').attr('checked','checked');
                $('#moecodinep').val('');
                $('#moenumprocesso').val('');
                $('#moeconvenioimobflag_n').attr('checked','checked');
                $('#moeadquiridoflag_n').attr('checked','checked');
                $('#moefuncionamentoflag_n').attr('checked','checked');
                
                $('#moeconveniotipo').val('');
                $('#moeorigemdado').val('');
                
                $('#moedtconclusaoobra').val('');
                $('#moedtiniciofunc').val('');
                $('#moedtprevinauguracao').val('');
                $('#moedtpreviniciofunc').val('');
                
                $('#moeqtdcriancacrecheparcialefetivo').val('');
                $('#moeqtdcriancacrecheintegralefetivo').val('');
                $('#moeqtdcriancapreparcialefetivo').val('');
                $('#moeqtdcriancapreintegralefetivo').val('');
                $('#moeqtdcriancacrecheparcialprevisao').val('');
                $('#moeqtdcriancacrecheintegralprevisao').val('');
                $('#moeqtdcriancapreparcialprevisao').val('');
                $('#moeqtdcriancapreintegralprevisao').val('');
                $('#moepercexecutado').val('');
        };
        
        alterarDadoMobiliario = function(moeid) {
            
            if (objDadoEmEdicao.moeid !== 0) {
                alert('Já existe dados sendo editados, salve ou cancele antes de fazer uma nova edição.');
            } else {
                
                $('#operacao').val('update');
                $('#moeid').val(moeid);
                $('#cancelaEdicao').show();

                objDadoEmEdicao.moeid                               = moeid;
                objDadoEmEdicao.obrid                               = $('#obrid').val();
                objDadoEmEdicao.moeempenhadoflag                    = $('#tr1_' + moeid).children(":eq(5)").text();
                objDadoEmEdicao.moeconveniotipo                     = $('#tr1_' + moeid).children(":eq(8)").text();
                objDadoEmEdicao.moeinaugrepmecflag                  = $('#tr1_' + moeid).children(":eq(3)").text();
                objDadoEmEdicao.moecodinep                          = $('#tr1_' + moeid).children(":eq(4)").text();
                objDadoEmEdicao.moenumprocesso                      = $('#tr1_' + moeid).children(":eq(5)").text();
                objDadoEmEdicao.moeconvenioimobflag                 = $('#tr1_' + moeid).children(":eq(7)").text();
                objDadoEmEdicao.moeadquiridoflag                    = $('#tr1_' + moeid).children(":eq(9)").text();
                objDadoEmEdicao.moefuncionamentoflag                = $('#tr1_' + moeid).children(":eq(10)").text();
                objDadoEmEdicao.moeorigemdado                       = $('#tr1_' + moeid).children(":eq(1)").text();
                
                objDadoEmEdicao.moedtconclusaoobra                  = $('#tr1_' + moeid).children(":eq(2)").text();
                objDadoEmEdicao.moedtiniciofunc                     = $('#tr1_' + moeid).children(":eq(11)").text();
                objDadoEmEdicao.moedtprevinauguracao                = $('#tr1_' + moeid).children(":eq(12)").text();
                objDadoEmEdicao.moedtpreviniciofunc                 = $('#tr1_' + moeid).children(":eq(13)").text();
                objDadoEmEdicao.moepercexecutado                    = $('#tr1_' + moeid).children(":eq(14)").text().replace('%','');
                
                objDadoEmEdicao.moeqtdcriancacrecheparcialefetivo   = $('#tr3_' + moeid).children(":eq(0)").text();
                objDadoEmEdicao.moeqtdcriancacrecheintegralefetivo  = $('#tr3_' + moeid).children(":eq(1)").text();
                objDadoEmEdicao.moeqtdcriancapreparcialefetivo      = $('#tr3_' + moeid).children(":eq(2)").text();
                objDadoEmEdicao.moeqtdcriancapreintegralefetivo     = $('#tr3_' + moeid).children(":eq(3)").text();
                objDadoEmEdicao.moeqtdcriancacrecheparcialprevisao  = $('#tr3_' + moeid).children(":eq(4)").text();
                objDadoEmEdicao.moeqtdcriancacrecheintegralprevisao = $('#tr3_' + moeid).children(":eq(5)").text();
                objDadoEmEdicao.moeqtdcriancapreparcialprevisao     = $('#tr3_' + moeid).children(":eq(6)").text();
                objDadoEmEdicao.moeqtdcriancapreintegralprevisao    = $('#tr3_' + moeid).children(":eq(7)").text();
                
                $('#moeid').val(moeid);
                $('#obrid').val(objDadoEmEdicao.obrid);
                
                ////SIM/NÃO
                if(objDadoEmEdicao.moeempenhadoflag == 'Sim'){
                    $('#moeempenhadoflag_s').attr('checked','checked');
                    $('#moeempenhadoflag_s').click().click();
                }else{
                    $('#moeempenhadoflag_n').attr('checked','checked');
                }
                
                if(objDadoEmEdicao.moeinaugrepmecflag == 'Sim'){
                    $('#moeinaugrepmecflag_s').attr('checked','checked');
                }else{
                    $('#moeinaugrepmecflag_n').attr('checked','checked');
                }
                
                if(objDadoEmEdicao.moeconvenioimobflag == 'Sim'){
                    $('#moeconvenioimobflag_s').attr('checked','checked');
                }else{
                    $('#moeconvenioimobflag_n').attr('checked','checked');
                }
                
                if(objDadoEmEdicao.moeadquiridoflag == 'Sim'){
                    $('#moeadquiridoflag_s').attr('checked','checked');
                }else{
                    $('#moeadquiridoflag_n').attr('checked','checked');
                }
                
                if(objDadoEmEdicao.moefuncionamentoflag == 'Sim'){
                    $('#moefuncionamentoflag_s').attr('checked','checked');
                }else{
                    $('#moefuncionamentoflag_n').attr('checked','checked');
                }
                
                //Selects-options
                $('#moeconveniotipo').val(objDadoEmEdicao.moeconveniotipo);
                $('#moeorigemdado').val(objDadoEmEdicao.moeorigemdado);
                
                $('#moedtconclusaoobra').val(objDadoEmEdicao.moedtconclusaoobra);
                $('#moedtiniciofunc').val(objDadoEmEdicao.moedtiniciofunc);
                $('#moedtprevinauguracao').val(objDadoEmEdicao.moedtprevinauguracao);
                $('#moedtpreviniciofunc').val(objDadoEmEdicao.moedtpreviniciofunc);
                $('#moecodinep').val(objDadoEmEdicao.moecodinep);
                $('#moenumprocesso').val(objDadoEmEdicao.moenumprocesso);

                $('#moeqtdcriancacrecheparcialefetivo').val(objDadoEmEdicao.moeqtdcriancacrecheparcialefetivo);
                $('#moeqtdcriancacrecheintegralefetivo').val(objDadoEmEdicao.moeqtdcriancacrecheintegralefetivo);
                $('#moeqtdcriancapreparcialefetivo').val(objDadoEmEdicao.moeqtdcriancapreparcialefetivo);
                $('#moeqtdcriancapreintegralefetivo').val(objDadoEmEdicao.moeqtdcriancapreintegralefetivo);
                $('#moeqtdcriancacrecheparcialprevisao').val(objDadoEmEdicao.moeqtdcriancacrecheparcialprevisao);
                $('#moeqtdcriancacrecheintegralprevisao').val(objDadoEmEdicao.moeqtdcriancacrecheintegralprevisao);
                $('#moeqtdcriancapreparcialprevisao').val(objDadoEmEdicao.moeqtdcriancapreparcialprevisao);
                $('#moeqtdcriancapreintegralprevisao').val(objDadoEmEdicao.moeqtdcriancapreintegralprevisao);
                $('#moepercexecutado').val(objDadoEmEdicao.moepercexecutado);
                
                $('#tr1_' + moeid).remove();
                $('#tr2_' + moeid).remove();
                $('#tr3_' + moeid).remove();
                
            }            
            return true;
        };
        
    });
</script>
</body>
</html>

<?php

function importaFotosEiManutencao($obrid, $pinid){
    if(empty($obrid) || empty($pinid)){
        return '';
    }
    
    global $db;
    ini_set("memory_limit", "2048M");
    
    $_SESSION['proinfantil']['obrid']   = $obrid;
    $_SESSION['proinfantil']['pinid']   = $pinid;
    $_SESSION['downloadfiles']['pasta'] = array("origem" => "proinfantil", "destino" => "proinfantil");
    $_SESSION['imgparams']              = array("filtro" => "fotstatus = 'A'", "tabela" => "proinfantil.fotos");
    $_SESSION['sisarquivo']             = 'proinfantil';
    
    $arrSalas     = recuperaSalas($obrid, $pinid); 
    $checado      = "checked";
    $chkbloqueado = true;
    $habilitado   = 'N';
    
?>

    <style>
    	.field_foto {text-align:right}
    	.field_foto legend{font-weight:bold;}
    	.img_add_foto{cursor:pointer}
    	.hidden{display:none}
    	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
    	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
    	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
    </style>
    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script type="text/javascript">
    $(function() {
    	 $('[name=btn_upload]').click(function() {
    	 	var erro = 0;
    	 	if(!$('[name=arquivo]').val()){
    	 		alert('Favor selecionar a foto!');
    	 		erro = 1;
    	 		return false;
    	 	}
    	 	if(!$('[name=arqdescricao]').val()){
    	 		alert('Favor informar uma descrição!');
    	 		erro = 1;
    	 		return false;
    	 	}
    	 	if(erro == 0){
    	 		$('[name=btn_upload]').val("Carregando...");
    	 		$('[name=btn_cancelar]').val("Carregando...");
    	 		$('[name=btn_upload]').attr("disabled","disabled");
    	 		$('[name=btn_cancelar]').attr("disabled","disabled");
    	 		$('[name=form_upload]').submit();
    	 	}
    	 });
    	 $('[name=btn_cancelar]').click(function() {
    	 	$('[id=div_sub_foto]').addClass('hidden');
    	 });
    });
    
    function abrirGaleria(arqid,pagina,salid,pinid){
    	window.open("../slideshow/slideshow/index.php?_sisarquivo=proinfantil&getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&pinid=" + pinid,"imagem","width=850,height=600,resizable=yes");
    }
    	
    function checaAmbiente(id){
    	if(document.getElementById("chk_ambiente_"+id).checked == true){
    		document.getElementById("field_sala_"+id).style.display = '';
    	} else {
    		document.getElementById("field_sala_"+id).style.display = 'none';
    	}
    }

    </script>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtitulocentro" colspan="15">Fotos do Sistema E.I Manutenção</td>
        </tr>
        <tr>
            <td>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <?php if ($arrSalas): ?>
                    <?php $i = 0 ?>
                    <?php foreach ($arrSalas as $sala): ?>
                        <?php echo $i % 2 == 0 ? "<tr>" : "" ?>
                            <td width="50%" valign="top" id="td_sala_<?php echo $sala['salid'] ?>">
                            <fieldset id="field_sala_<?php echo $sala['salid'] ?>" class="field_foto" >
                                    <legend>Fotos - <?php echo $sala['tisdescricao'] ?></legend>
                                    <table width="100%">
                                        <tr>
                                            <td valign="top">
                                            <?php $arrFotos = recuperaFotosSala($sala['salid'], $pinid); ?>
                                            <?php if ($arrFotos): ?>
                                                <?php foreach ($arrFotos as $foto): ?>
                                                    <?php $pagina = floor($n / 16); ?>
                                                    <?php
                                                    $imgend = APPRAIZ . 'arquivos/' . 'proinfantil' . '/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                                                    if (is_file($imgend)) {
                                                        $img_max_dimX = 100;
                                                        $img_max_dimY = 85;

                                                        $imginfo = getimagesize($imgend);

                                                        $width = $imginfo[0];
                                                        $height = $imginfo[1];

                                                        if (($width > $img_max_dimX) or ( $height > $img_max_dimY)) {
                                                            if ($width > $height) {
                                                                $w = $width * 0.9;
                                                                while ($w > $img_max_dimX) {
                                                                    $w = $w * 0.9;
                                                                }
                                                                $w = round($w);
                                                                $h = ($w * $height) / $width;
                                                            } else {
                                                                $h = $height * 0.9;
                                                                while ($h > $img_max_dimY) {
                                                                    $h = $h * 0.9;
                                                                }
                                                                $h = round($h);
                                                                $w = ($h * $width) / $height;
                                                            }
                                                        } else {
                                                            $w = $width;
                                                            $h = $height;
                                                        }

                                                        $tamanho = " width=\"$w\" height=\"$h\" ";
                                                    } else {
                                                        $tamanho = "";
                                                    }
                                                    ?>
                                                    <div id="div_foto_<?php echo $foto['arqid'] ?>" class="div_foto">

                                                        <!--<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar"  />-->

                                                        <img onClick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>','<?php echo $sala['salid'] ?>','<?php echo $pinid ?>')" <?php echo $tamanho ?> 
                                                             onmouseover="return escape('<b>Descrição:</b> <?php echo $foto['arqdescricao'] ?><br /><b>Tamanho(Kb):</b> <?php echo $foto['arqtamanho'] ? round($foto['arqtamanho'] / 1024, 2) : "N/A" ?>');" 
                                                             class="img_foto" 
                                                             src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?= (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) ?>" 
                                                        />
                                                    </div>
                                                <?php endforeach; ?>
                                            <?php endif; ?>
                                            </td>
                                        </tr>
                                    </table>
                            </fieldset>
                            </td>
                            <?php echo $i % 2 == 1 ? "</tr>" : "" ?>
                            <?php $i++ ?>
                        <?php endforeach; ?>
                    <?php endif; ?>
                </table>
    		</td>
            </tr>
        </table>
    <?php 
}
    
    function importaDadosEiManutencao($obrid){        
        global $db;        
	$sql = "SELECT pin.pinid, qci.qcicodigoinep, r.resdsc
                FROM proinfantil.proinfantil pin

                INNER JOIN proinfantil.questionario q ON pin.pinid = q.pinid 
                INNER JOIN questionario.resposta    r ON r.qrpid   = q.qrpid

                INNER JOIN proinfantil.questionariocodigoinep qci ON r.qrpid = qci.qrpid


                WHERE pin.pinststus = 'A'
                  AND r.perid = 1587 --IN ( 1587, 3578 )
                  AND pin.obrid = {$obrid}

                ORDER BY r.perid";
	$dados = $db->pegaLinha($sql);
        
        if($dados){
            $pinid                   = $dados['pinid']; // Exmplo: 46
            $qcicodigoinep           = $dados['qcicodigoinep'];
            $data_inicio_atendimento = $dados['resdsc'];
        }else{
            $pinid                   = '';
            $qcicodigoinep           = '';
            $data_inicio_atendimento = '';
        }
        
        if(!empty($pinid)){
            $sql = "SELECT --*
                            maa.alaquantidade, maa.timid, tim.timdescricao, maa.titid, tit.titdescricao
                    FROM proinfantil.mdsalunoatendidopbf maa
                    LEFT JOIN proinfantil.tipoturno      tit ON maa.titid = tit.titid
                    LEFT JOIN proinfantil.tipomodalidade tim ON maa.timid = tim.timid
                    WHERE tit.titstatus = 'A' 
                      AND tit.modid     = 1 
                      AND tim.timstatus = 'A' 
                      AND tim.modid     = 1 
                      AND maa.pinid     = ".$pinid;
            
            $arrCriancasAtendidas = $db->carregar($sql);
        }
        
        $arrCriancasAtendidas = empty($arrCriancasAtendidas) ? array() : $arrCriancasAtendidas;
        
        $arrRetorno = array('pinid'               => $pinid,
                            'qcicodigoinep'       => $qcicodigoinep, 
                            'dtInicioAtendimento' => $data_inicio_atendimento, 
                            'criancasAtendidas'   => $arrCriancasAtendidas);
        return $arrRetorno;
        
    }
  
    /**
     * As funções abaixo foram copiadas do arquivo \simec\www\proinfantil\_funcoes_proinfancia.php
     * Pois o include do mesmo estava gerando conflito com os nomes das funções.
     */
    function recuperaSalas($obrid,$pinid){
	global $db;
	
	$aryModalidade = verificaExisteAluno($pinid);

	$where = '';
	
	if(in_array(CRECHE, $aryModalidade)){
		$where = 'AND sal.salid NOT IN (4,47,48,49,11,12,13,14)';
	}

	if(in_array(PREESCOLA, $aryModalidade)){
		$where = 'AND sal.salid NOT IN (1,2,3,8,9,10)';
	}
	
	if(in_array(CRECHE, $aryModalidade) && in_array(PREESCOLA, $aryModalidade)){
		$where = '';
	}	
		
	$sql = "SELECT DISTINCT sal.salid,
                       tis.tisdescricao
                FROM proinfantil.tiposala tis
                INNER JOIN proinfantil.sala sal ON sal.tisid = tis.tisid
                WHERE sal.tpoid IN(SELECT tpoid FROM obras2.obras WHERE obrid = {$obrid})
                $where
                ORDER BY tis.tisdescricao";
	
	return $db->carregar($sql);
}

    function recuperaFotosSala($salid,$pinid){
            global $db;
            $sql = "SELECT arq.arqid,
                           arq.arqnome||'.'||arq.arqextensao,
                           arq.arqdescricao,
                           arq.arqtamanho
                    FROM proinfantil.fotos fot
                    INNER JOIN public.arquivo arq ON arq.arqid = fot.arqid
                    WHERE fot.salid = {$salid}
                      AND fot.pinid = {$pinid}
                      AND fot.fotstatus = 'A'";
            return $db->carregar($sql);
    }

    function verificaExisteAluno($pinid){
            global $db;
            $sql = "SELECT ma.timid
                    FROM proinfantil.mdsalunoatendidopbf  ma
                    INNER JOIN proinfantil.tipomodalidade tm ON tm.timid = ma.timid
                    WHERE pinid = {$pinid} 
                    GROUP BY ma.timid, tm.timdescricao 
                    HAVING sum(ma.alaquantidade) > 0";
            $aryModalidade = $db->carregar($sql);

        !$aryModalidade? $aryModalidade = array() : $aryModalidade = $aryModalidade;
        $aryMod = array();
            foreach($aryModalidade as $mod){
                    $aryMod[] = $mod['timid'];
        }

        return $aryMod;
    }
    
?>