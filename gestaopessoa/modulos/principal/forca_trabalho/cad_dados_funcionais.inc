<?PHP
    if ($_POST['ajaxsiape']) {
        header('content-type: text/html; charset=ISO-8859-1');
        $sql = "
            SELECT  ds_email,
                    nu_telefone
            FROM gestaopessoa.vw_simec_consulta_servidor_siape

            WHERE replace( replace(nu_cpf,'.', ''), '-','') = replace( replace('" . $_POST['ajaxsiape'] . "','.', ''), '-','')
        ";
        $rs = $db->carregar($sql);
        if ($rs) {
            $res = $rs[0]['ds_email'] . '_' .$rs[0]['nu_telefone']. '_' .$rs[0]['temInfo'];
            die($res);
        }
        die();
    }

    if ($_GET['pop'] == 'sim') {
        $fdpcpf = $_GET['fdpcpf'];
        $nao_visualizar = array(5461, 5542, 5543, 5544, 5545, 6480, 7556);
        $db->cria_aba($abacod_tela, $url, '&pop=sim&fdpcpf=' . $fdpcpf, $nao_visualizar);

        $bloquearEdicao = "disabled=disabled";

        echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
        echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
    }

    if (!isset($_GET['pop'])) {
        include APPRAIZ . "includes/cabecalho.inc";
        echo '<br>';
        include_once( APPRAIZ . "gestaopessoa/classes/FtDadoFuncional.class.inc" );
    } else {
        include_once( APPRAIZ . "gestaopessoa/classes/FtDadoFuncional.class.inc" );
    }

    if (!isset($_GET['pop'])) {
        $bloquearEdicao = bloqueiaEdicaoFT();

        $db->cria_aba($abacod_tela, $url, '');
    }

    monta_titulo('Força de Trabalho', 'Dados Funcionais');

    $tipo = getSituacaoMEC($_SESSION['fdpcpf']);
    $arCamposFuncionais = controlaDadoFuncional($tipo);

    echo cabecalhoPessoa($_SESSION['fdpcpf']);

    $df = new FtDadoFuncional();

    if ($_REQUEST['del'] != '') {
        $df->excluir($_REQUEST['del']);
        $df->commit();
        $df->sucesso("principal/forca_trabalho/cad_dados_funcionais");
    }

    $id = $df->pegaUm("select fdfid from gestaopessoa.ftdadofuncional where fdpcpf = '" . $_SESSION["fdpcpf"] . "'");

    if ($id) {

        $df->carregarPorId($id);

        $sql = "SELECT * FROM gestaopessoa.ftdadofuncional WHERE fdfid = $id";
        $dados = $db->carregar($sql);
    }

    if ($_POST['tipo_acao'] == 'GRAVAR') {

        $arDados = controlaDadoFuncional($tipo);

        $df->fdpcpf = $_SESSION['fdpcpf'];
        $df->fdfatualizacao = $_SESSION['exercicio'];

        $df->popularObjeto($arDados);
        $df->salvar();
        $df->commit();

        //atualiza o resto dos dados
        $fdfid = $df->pegaUm("select fdfid from gestaopessoa.ftdadofuncional where fdpcpf = '" . $_SESSION["fdpcpf"] . "'");
        if( $fdfid ){

            $feoid          = $_POST['feoid'] ? $_POST['feoid'] : 'NULL';
            $fdfsituacao    = $_POST['fdfsituacao'] ? "'".$_POST['fdfsituacao']."'" : 'NULL';
            $fdfloginrede   = $_POST['fdfloginrede'] ? "'".$_POST['fdfloginrede']."'" : 'NULL';
            $fdfdtentrada   = $_POST['fdfdtentrada'] ? "'".formata_data_sql($_POST['fdfdtentrada'])."'" : 'NULL';
            $fdfdtsaida     = $_POST['fdfdtsaida'] ? "'".formata_data_sql($_POST['fdfdtsaida'])."'" : 'NULL';

            $sql = "
                UPDATE gestaopessoa.ftdadofuncional
                    SET feoid           = {$feoid},
                        fdfsituacao     = {$fdfsituacao},
                        fdfloginrede    = {$fdfloginrede},
                        fdfdtentrada    = {$fdfdtentrada},
                        fdfdtsaida      = {$fdfdtsaida}
                WHERE fdfid = {$fdfid}
            ";
            $db->executar($sql);

            //vincula sistemas
            $sql = "delete from gestaopessoa.ftdadofuncionalsistema where fdfid = $fdfid";
            $db->executar($sql);

            if ($_POST['fsaid'][0]) {
                foreach ($_POST['fsaid'] as $v) {
                    $sql = "INSERT INTO gestaopessoa.ftdadofuncionalsistema(fdfid, fsaid)VALUES ($fdfid, {$v['fsaid']})";
                    $db->executar($sql);
                }
            }
            $db->commit();
        }
        $df->sucesso("principal/forca_trabalho/cad_dados_funcionais");
    }
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <input type="hidden" id="tipo_acao" name="tipo_acao" value="">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <? if (in_array('fcmid', $arCamposFuncionais)) { ?>
            <tr>
                <td class ="SubTituloDireita" align="right">
                    <?PHP
                        if( ($_SESSION['sit'] == 5) || ($_SESSION['sit'] == 6) || ($_SESSION['sit'] == 10) || ($_SESSION['sit'] == 11) ){
                            echo "Cargo Efetivo:";
                        } else {
                            echo "Cargo Efetivo no MEC:";
                        }
                    ?>
                </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  fcmid as codigo,
                                    fcmdescricao as descricao
                            FROM gestaopessoa.ftcargoefetivomec
                            ORDER BY fcmdescricao
                        ";
                        $fcmid = $dados[0]['fcmid'];
                        $db->monta_combo('fcmid', $sql, 'S', "Selecione...", '', '', '', '310', 'S', 'fcmid');
                    ?>
                </td>
            </tr>
        <?PHP
            }

            if (in_array('fdfexercecargofuncao', $arCamposFuncionais)) {
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right"> Exerce Cargo/Função: </td>
                <td>
                    <input type="radio" name="fdfexercecargofuncao" id="fdfexercecargofuncao" <? if ($dados[0]['fdfexercecargofuncao'] == 't') echo 'checked = checked'; ?> value="t" > Sim
                    <input type="radio" name="fdfexercecargofuncao" id="fdfexercecargofuncao" <? if ($dados[0]['fdfexercecargofuncao'] == 'f') echo 'checked = checked'; ?> value="f"> Não
                    &nbsp;&nbsp;&nbsp;&nbsp;
                    <?PHP
                        $fdespecificacaocargofuncao = $dados[0]['fdespecificacaocargofuncao'];

                        echo campo_texto('fdespecificacaocargofuncao', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '', 0, 'id="fdespecificacaocargofuncao" onblur="MouseBlur(this);"');
                    ?>
                </td>
            </tr>
        <?PHP
            }

            if (in_array('fulid', $arCamposFuncionais)) {
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Unidade de Lotação: </td>
                <td>
                    <?PHP
                        $fulid = $dados[0]['fulid'];
                        $sql = "
                            SELECT  fulid AS codigo,
                                    fuldescricao AS descricao
                            FROM gestaopessoa.ftunidadelotacao
                        ";
                        $db->monta_combo('fulid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'fulid');
                    ?>
                </td>
            </tr>
        <?
            }

            if (in_array('forid', $arCamposFuncionais)){
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Orgão Origem do Requisitado: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT forid AS codigo,
                                    fordescricao AS descricao
                            FROM gestaopessoa.ftorigem
                        ";
                        $forid = $dados[0]['forid'];
                        $db->monta_combo('forid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'forid');
                    ?>
                </td>
            </tr>
        <?PHP
            }

            if (in_array('fooid', $arCamposFuncionais)) {
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Orgão Origem do Exercicio Provisório: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  fooid AS codigo,
                                    foodescricao AS descricao
                             FROM gestaopessoa.ftorgaoorigem
                        ";
                        $fooid = $dados[0]['fooid'];
                        $db->monta_combo('fooid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'fooid');
                    ?>
                </td>
            </tr>
        <?PHP
            } 
        
            if (in_array('furid', $arCamposFuncionais)) { 
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Unidade Responsável: </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT furid AS codigo,
                                    furdescricao AS descricao
                            FROM gestaopessoa.ftunidaderesponsavel
                        ";
                        $furid = $dados[0]['furid'];
                        $db->monta_combo('furid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'furid');
                    ?>
                </td>
            </tr>
        <?PHP
            } 
            
            if( in_array('fdfsala', $arCamposFuncionais) ){ 
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Sala: </td>
                <td>
                    <?PHP 
                        $fdfsala = $dados[0]['fdfsala']; 
                        echo campo_texto('fdfsala', 'S', $somenteLeitura, '', 10, 50, '', '', 'left', '', 0, 'id="fdfsala" onblur="MouseBlur(this);"'); 
                    ?>                   
                </td>
            </tr>
        <?PHP
            }
            
            if( in_array('fdfpostotrabalho', $arCamposFuncionais) ){ 
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Posto de Trabalho: </td>
                <td>
                    <?PHP
                        $fdfpostotrabalho = $dados[0]['fdfpostotrabalho'];
                        echo campo_texto('fdfpostotrabalho', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdfpostotrabalho" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
        <?PHP 
            } 
            
            if( in_array('fdfempresa', $arCamposFuncionais) ){ 
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Empresa do Terceirizado: </td>
                <td>
                    <?PHP
                        $fdfempresa = $dados[0]['fdfempresa']; 
                        campo_texto('fdfempresa', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdfempresa" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
        <?PHP
            } 
        
            if( in_array('fdfcnpjempresa', $arCamposFuncionais) ){ 
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">CNPJ da Empresa: </td>
                <td>
                    <?PHP
                        $fdfcnpjempresa = $dados[0]['fdfcnpjempresa'];
                        echo campo_texto('fdfcnpjempresa', 'S', $somenteLeitura, '', 20, 18, '', '', 'left', '', 0, 'id="fdfcnpjempresa" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
        <?PHP
            }
        
            if(( in_array('fdfprojetodatainicio', $arCamposFuncionais) && (in_array('fdfprojetodatafim', $arCamposFuncionais) ))){
        ?>
            <tr><?
        
            ?>
                <td class ="SubTituloDireita" align="right">Período do Projeto: </td>
                <td>
                    <?PHP
                        $fdfprojetodatainicio = formata_data($dados[0]['fdfprojetodatainicio']);
                        $fdfprojetodatafim = formata_data($dados[0]['fdfprojetodatafim']);
                        
                        echo campo_data2('fdfprojetodatainicio', 'S', 'S', '', 'S');
                        echo 'à';
                        echo campo_data2('fdfprojetodatafim', 'S', 'S', '', 'S'); 
                    ?>
                </td>
            </tr>
        <?PHP
            }
            
            if( in_array('fdftelefone', $arCamposFuncionais) ){ 
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Telefone (Ramal): </td>
                <td>
                    <?PHP
                        $fdftelefone = $dados[0]['fdftelefone'];
                        echo campo_texto('fdftelefone', 'S', $somenteLeitura, 'Telefone (Ramal)', 20, 20, '####-####', '', 'left', '', 0, 'id="fdftelefone" onblur="MouseBlur(this);"');
                    ?>
                </td>
            </tr>
        <?PHP
            } 
        
            if( in_array('fdfemail', $arCamposFuncionais) ){
        ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Email do MEC: </td>
                <td>
                    <?PHP
                        $fdfemail = $dados[0]['fdfemail'];
                        echo campo_texto('fdfemail', 'S', $somenteLeitura, '', 50, 100, '', '', 'left', '', 0, 'id="fdfemail" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
        <? } ?>
        <tr>
            <td class ="SubTituloDireita" align="right">Situação: </td>
            <td>
                <input type="radio" name="fdfsituacao" id="fdfsituacao" <? if ($dados[0]['fdfsituacao'] == 'A') echo 'checked = checked'; ?> value="A" > Ativo
                <input type="radio" name="fdfsituacao" id="fdfsituacao" <? if ($dados[0]['fdfsituacao'] == 'I') echo 'checked = checked'; ?> value="I"> Inativo
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Login na Rede: </td>
            <td>
                <?PHP
                    $fdfloginrede = $dados[0]['fdfloginrede']; 
                    echo campo_texto('fdfloginrede', 'N', $somenteLeitura, '', 50, 100, '', '', 'left', '', 0, 'id="fdfloginrede" onblur="MouseBlur(this);"'); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top">Atuação em Sistemas de TI do MEC: </td>
            <td valign="top">
                <?PHP

                    $sql_combo = "
                        SELECT  fsaid as codigo,
                                fsadsc as descricao
                        FROM gestaopessoa.ftsistema
                        ORDER BY 2
                    ";

                    if( $id ){
                        $sql = "
                            SELECT  b.fsaid as codigo,
                                    b.fsadsc as descricao
                            FROM gestaopessoa.ftdadofuncionalsistema a
                            INNER JOIN gestaopessoa.ftsistema b on b.fsaid = a.fsaid
                            WHERE a.fdfid = {$id}
                            ORDER BY 2
                        ";
                        $fsaid = $db->carregar($sql);
                    }

                    $filtro = array(
                        array(
                            "codigo" => "fsadsc",
                            "descricao" => "<b>Sistema:</b>",
                            "string" => "1"
                        )
                    );
                    combo_popup('fsaid', $sql_combo, 'Selecione o(s) Sistema(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data de Entrada:</td>
            <td>
                <?PHP
                    $fdfdtentrada = $dados[0]['fdfdtentrada']; 
                    echo campo_data2('fdfdtentrada', 'N', 'S', '', 'S'); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data de Saída:</td>
            <td>
                <?PHP
                    $fdfdtsaida = $dados[0]['fdfdtsaida'];
                    echo campo_data2('fdfdtsaida', 'N', 'S', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top">Empresa de Origem: </td>
            <td valign="top">
                <?PHP
                    $sql = "
                        SELECT  feoid AS codigo,
                                feodsc AS descricao
                        FROM gestaopessoa.ftempresaorigem
                    ";
                    $feoid = $dados[0]['feoid'];
                    $db->monta_combo('feoid', $sql, 'S', "Selecione...", '', '', '', '400', 'N', 'feoid');
                ?>
            </td>
        </tr>
        <?PHP
            if( !isset($_GET['pop']) ){ 
        ?>
                <tr>
                    <td class ="SubTituloDireita" align="right">   </td>
                    <td><input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?= $bloquearEdicao; ?>></td>
                </tr>
        <?PHP 
            } 
        ?>
    </table>
</form>

<script type="text/javascript">
<? if (!$id) { ?>
        pegaDadosSIAPE('<?= $_SESSION['fdpcpf']; ?>');
<? } ?>
    function pegaDadosSIAPE(cpf) {
        if (!cpf) {
            return false;
        }
        var fdfemail = document.getElementById('fdfemail');
        var fdftelefone = document.getElementById('fdftelefone');
        var req = new Ajax.Request('gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_funcionais&acao=A', {
            method: 'post',
            parameters: '&ajaxsiape=' + cpf,
            onComplete: function (res){
                var arResp = res.responseText.split("_");

                var ds_email    = arResp[0];
                var nu_telefone = arResp[1];
                var temInfo     = arResp[2];
                
                if( temInfo == 'temInfo' ){
                    fdfemail.value = ds_email;
                    if (nu_telefone){
                        fdftelefone.value = nu_telefone;
                    }
                }
            }
        });
    }

    function validaForm() {

<? if (in_array('fcmid', $arCamposFuncionais)) { ?>
            var fcmid = document.getElementById('fcmid');
            if (fcmid.value == '') {
                alert('Campo Cargo Efetivo é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdfexercecargofuncao', $arCamposFuncionais)) { ?>
            var fdfexercecargofuncao = document.getElementById('fdfexercecargofuncao');
            if (fdfexercecargofuncao.value == '') {
                alert('Campo "Exerce Cargo/Função é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fulid', $arCamposFuncionais)) { ?>
            var fulid = document.getElementById('fulid');
            if (fulid.value == '') {
                alert('Campo Unidade de Lotação é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('forid', $arCamposFuncionais)) { ?>
            var forid = document.getElementById('forid');
            if (forid.value == '') {
                alert('Campo Orgão requisitado é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fooid', $arCamposFuncionais)) { ?>
            var fooid = document.getElementById('fooid');
            if (fooid.value == '') {
                alert('Campo Orgão de Origem é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('furid', $arCamposFuncionais)) { ?>
            var furid = document.getElementById('furid');
            if (furid.value == '') {
                alert('Campo Unidade Responsável é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdfsala', $arCamposFuncionais)) { ?>
            var fdfsala = document.getElementById('fdfsala');
            if (fdfsala.value == '') {
                alert('Campo Sala é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdfpostotrabalho', $arCamposFuncionais)) { ?>
            var fdfpostotrabalho = document.getElementById('fdfpostotrabalho');
            if (fdfpostotrabalho.value == '') {
                alert('Campo Posto de Trabalho é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdfempresa', $arCamposFuncionais)) { ?>
            var fdfempresa = document.getElementById('fdfempresa');
            if (fdfempresa.value == '') {
                alert('Campo Empresa é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdfcnpjempresa', $arCamposFuncionais)) { ?>
            var fdfcnpjempresa = document.getElementById('fdfcnpjempresa');
            if (fdfcnpjempresa.value == '') {
                alert('Campo CNPJ da Empresa é obrigatório. ');
                return false;
            }
<? } ?>
<? if (( in_array('fdfprojetodatainicio', $arCamposFuncionais) && (in_array('fdfprojetodatafim', $arCamposFuncionais) ))) { ?>
            var fdfprojetodatainicio = document.getElementById('fdfprojetodatainicio');
            var fdfprojetodatafim = document.getElementById('fdfprojetodatafim');
            if (fdfprojetodatainicio.value == '' || fdfprojetodatafim.value == '') {
                alert('O período do projeto é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdftelefone', $arCamposFuncionais)) { ?>
            var fdftelefone = document.getElementById('fdftelefone');
            if (fdftelefone.value == '') {
                alert('Campo Telefone é obrigatório. ');
                return false;
            }
<? } ?>
<? if (in_array('fdfemail', $arCamposFuncionais)) { ?>
            var fdfemail = document.getElementById('fdfemail');
            if (fdfemail.value == '') {
                alert('Campo Email é obrigatório. ');
                return false;
            }
<? } ?>

        selectAllOptions(document.formulario.fsaid);

        document.getElementById('tipo_acao').value = 'GRAVAR';
        document.formulario.submit();
    }
</script>