<?php

include APPRAIZ . 'elabrev/classes/modelo/RelatorioCumprimentoObjeto.class.inc';

if (isset($_GET['gerarDOU']) && ($_GET['gerarDOU'] == 'true')) {

    require_once APPRAIZ . 'elabrev/classes/modelo/ExtratoDOU.class.inc';
    require_once APPRAIZ . 'elabrev/classes/modelo/HtmlToPdf.class.inc';

    $extrato = new ExtratoDOU();
    $html = $extrato->getSigle();
    if (!$html) {
        echo 'Houve uma falha ao tentar gerar o extrato, favor contacte a equipe do SIMEC';
    } else {
        $documento = new HtmlToPdf($html);
        $documento->setTitle('Termo de Execução Descentralizada - Extrato DOU em: '.date('d-m-Y H:i:s'));
        $documento->getPDF();
    }
    die;
}

// Verifica requisição
if ($_REQUEST['requisicao'] == 'gravaCoordenacao') {
    if ($_REQUEST['cooid'] && $_REQUEST['tcpid']) {
        $sql = "update monitora.termocooperacao set cooid = {$_REQUEST['cooid']} where tcpid = {$_REQUEST['tcpid']}";
        $db->executar($sql);
        if($db->commit()){
            die('1');
        }
    }
    die('0');
}

if ( $_GET['dircodSalvar'] ){
    if ($_SESSION['elabrev']['tcpid']) {
        $sql = 'update monitora.termocooperacao set dircod ='.$_GET['dircodSalvar']. ' where tcpid = '.$_SESSION['elabrev']['tcpid'];
        $db->executar($sql);
        $db->commit();
    }
}

verificaCodigoTermoSessao();

/**
 * Lista as notas de crédito do termo de cooperação, incluíndo o status de cadastro no sigef.
 * @param int $tcpid ID do termo de cooperação.
 * @see abaTramite.inc
 */
function notasDeCredito($tcpid)
{
    $dadosNC = consultarNotasDeCredito($tcpid, true, true);
    //ver($dadosNC, d);
    if (empty($dadosNC)) {
        echo <<<HTML
<p style="text-align:center">Sem NC para disponibilizar para pagamento</p>
HTML;

        //Ser não tiver nenhuma NC para pagamento, verifica se ja existe alguma nc paga
        $dadosNC = pagamentosSolicitados($tcpid, true, true);
        $desabilitaBotao = true;
        if (!$dadosNC) return;
    }

    $output = '
        <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" width="95%" style="text-align:center">
        <tr class="SubTituloCentro">
            <th colspan="11">Requisições de Pagamentos</th></tr><tr>';

    if ($desabilitaBotao) {
        $output.= '<td>Nota de crédito</td>';
    } else {
        $output.= '<td>&nbsp;</td>';
    }

    $output.= '<td>Nº parcela</td>
        <td>Mês da<br />liberação</td>
        <td>Ano</td>
        <td>PTRES</td>
        <td>PI</td>
        <td>Natureza da Despesa</td>
        <td>Valor (R$)</td>
        <td>Cadastrada<br />no SIGEF?</td>
        <td>Último retorno SIGEF</td>';

    if ($desabilitaBotao) {
        $output.= '<td>Data Cadastro SIGEF</td>';
    }

    $output .= '</tr>';

    $meses = arrayDeMeses();

    $proids = array();
    foreach ($dadosNC as $nc) {

        /**
         * Solução paleativa para linhas duplicadas
         */
        if (in_array($nc['proid'], $proids)) {
            continue;
        }
        array_push($proids, $nc['proid']);

        $nc['valor_pagamento'] = formata_valor($nc['valor_pagamento']);
        $retornoSIGEFStyle = ('-' == $nc['ppaultimoretornosigef']?'':' style="text-align:left"');
        $ultimoRetorno = utf8_decode($nc['ppaultimoretornosigef']);

        $output.= '<tr>';

        if ($desabilitaBotao) {
            $output.= "<td>{$nc['nota_credito']}</td>";
        } else {
            $output.= "<td><input type='checkbox' name='proid[]' value='{$nc['proid']}' id='{$nc['proid']}' /></td>";
        }

        $output.= "<td>{$nc['numero_parcela']}</td>
            <td>{$meses[$nc['mes_pagamento']]}</td>
            <td>{$nc['ano_parcela']}</td>
            <td>{$nc['ptres']}</td>
            <td>{$nc['plano_interno']}</td>
            <td>{$nc['natureza_despesa']}</td>
            <td>{$nc['valor_pagamento']}</td>
            <td>{$nc['ppacadastradosigef']}</td>
            <td{$retornoSIGEFStyle}>{$ultimoRetorno}</td>";

        if ($desabilitaBotao) {
            $output .= "<td>{$nc['ppadata']}</td>";
        }

        $output .= "</tr>";
    }
    $output.= '</table>';

    echo $output;
}

/**
 * Array com os nomes dos meses.
 * @return array
 * @see abaTramite.inc
 */
function arrayDeMeses()
{
    $i = 0;
    $meses = array();
    $meses[++$i] = "Janeiro";
    $meses[++$i] = "Fevereiro";
    $meses[++$i] = "Março";
    $meses[++$i] = "Abril";
    $meses[++$i] = "Maio";
    $meses[++$i] = "Junho";
    $meses[++$i] = "Julho";
    $meses[++$i] = "Agosto";
    $meses[++$i] = "Setembro";
    $meses[++$i] = "Outubro";
    $meses[++$i] = "Novembro";
    $meses[++$i] = "Dezembro";
    return $meses;
}

/**
 * Cria a tabela de notas de crédito e a popup de login/senha para requisição no webservice.
 */
function disponibilizarParaPagamento()
{
    // -- não pode criar outro formulário tem que alterar o que já existe no momento do envio
    // -- se cancelar o envio, tem que voltar ele ao normal.
    ?>
    <style type="text/css">
        .popup_alerta{
            width:300px;height:160px;position:absolute;z-index:0;
            top:50%;left:50%;margin-top:-80px;margin-left:-150px;
            border:solid 2px black;background-color:#FFFFFF;display:none;
            overflow:auto}
    </style>
    <script type="text/javascript">
        $(document).ready(function(){
            $('#dispParaPagamento').click(function(){

                if (!$("input[name='proid[]']").is(":checked")) {
                    alert("Selecione as previsões orçamentárias que já possuem nota de crédito para pagamento.");
                    return false;
                }

                $('#div_auth').show();
                $('html, body').animate({scrollTop: $('#div_auth').offset().top - 100}, 500);
            });
        });
        function enviarSolicitacao()
        {
            if (jQuery("input.obrigatorio", '#div_auth').not("input[value='']").length < 2) {
                alert('Os campos "Usuário" e "Senha" são obrigatórios.');
                return;
            }
            // -- Alterando o formulário e realizando a envio
            $('#requisicaoNC').val('solicitarNC');
            $('#formulario').submit();
        }
    </script>
    <input type="button" id="dispParaPagamento" value="Disponibilizar para pagamento" />
    <div id="div_auth" class="popup_alerta" style="display:none">
        <div style="padding:5px;text-align:justify">
            <table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr class="SubtituloDireita">
                    <td colspan="2" style="text-align:center;font-weight:bold;font-size:14px">Credenciais SIGEF</td>
                </tr>
                <tr>
                    <td width="30%" class="SubtituloDireita">Usuário:</td>
                    <td>
                        <input type="text" name="wsusuario" size="21" maxlength="" onmouseover="MouseOver(this);"
                               onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);"
                               onblur="MouseBlur(this);" id='wsusuario' title='Usuário' class='obrigatorio normal' />
                        <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
                    </td>
                </tr>
                <tr>
                    <td class="SubtituloDireita">Senha:</td>
                    <td nowrap>
                        <input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);"
                               onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();"
                               onmouseover="MouseOver(this);" size="21" id="wssenha" name="wssenha" />
                        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                    </td>
                </tr>
                <tr>
                    <td align="center" bgcolor="#D5D5D5" colspan="2" style="padding:4px">
                        <input type="button" name="btn_enviar" onclick="enviarSolicitacao()" value="ok" />
                        <input type="button" name="btn_cancelar" value="cancelar"
                               onclick="$('#div_auth').hide();" />
                    </td>
                </tr>
            </table>
        </div>
    </div>
<?php
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script>
function salvaCoordenacao(id) {
    tcpid = $('[name=tcpid]').val();
    $.ajax({
        url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A',
        type	: 'post',
        data	: 'requisicao=gravaCoordenacao&cooid='+id+'&tcpid='+tcpid,
        success	: function(e){
            if(e == '1'){
                alert('Operação realizada com sucesso.');
                document.location.reload();
            }else{
                alert('Falha ao gravar a coordenação!');
            }
        }
    });
}
</script>
<?php

if(!$boPopup)
    require_once APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

$sql = "select tcpobsrelatorio from monitora.termocooperacao where tcpid = ". $_SESSION['elabrev']['tcpid'];
$tcpobsrelatorio = !empty($_SESSION['elabrev']['tcpid']) ? $db->pegaUm($sql) : array();

?>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
    <input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
    <input type="hidden" id="req" name="req" value="" />
    <input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
    <input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
    <input type="hidden" id="arptipo" name="arptipo" value="T">
    <input type="hidden" id="requisicaoNC" name="requisicaoNC" />

    <input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" valign="bottom"><center><b>Trâmite</b></center></td>
        </tr>
    </table>

    <?php
    /**
     * Mostra pendencia no preenchimento do
     * relatório de cumprimento do objeto
     */
    $objeto = new RelatorioCumprimentoObjeto();
    echo $objeto->termosPendenciaAprovacaoCoordenacao();
    ?>
    <table id="cronograma" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
        <?php
        if (pegarEstadoAtual($_SESSION['elabrev']['tcpid']) == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD) {
            echo '<tr><td>' . $objeto->getAlertaPrazoAprovacaoCoordenacao() . '</td></tr>';
        }
        ?>
        <tr id="tr_titulo">
            <td class="subtitulocentro">Pendências</td>
        </tr>
        <?php
        echo $objeto->mostraPendencias();

        /**
         * Lista demais pendencias
         */
        listaPendencias();
        ?>
    </table>
    <?php
    if ($_SESSION['elabrev']['tcpid'] != '') {
        $estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    }

    $strConcedente = "select ungcodconcedente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}";
    $_concedente = $db->pegaUm($strConcedente);

    if (EM_EXECUCAO == $estadoAtual && $_concedente == UG_FNDE) {
        notasDeCredito($_SESSION['elabrev']['tcpid']);

        if (isset($_SESSION['elabrev']['xmlWs'])) {
            echo $_SESSION['elabrev']['xmlWs'];
        }
    }

    if ($estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD
        && (in_array(PERFIL_COORDENADOR_SEC, $perfis)
        || in_array(PERFIL_SUPER_USUARIO, $perfis) || verificaEquipeTecnicaConcedente())) : ?>

        <table><tr><td>&nbsp;</td></tr></table>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
            <tr>
                <td colspan="4" class="subtitulocentro" style="background-color: #CFCFCF;">Parecer Técnico sobre o relatório de cumprimento do objeto do Termo de Execução Descentralizada</td>
            </tr>
            <tr>
                <td class="subtitulocentro" >
                    <?php
                    if (in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(UO_EQUIPE_TECNICA, $perfis)) {
                        $hab = "S";
                    } else {
                        $hab = "N";
                    }
                    echo campo_textarea("tcpobsrelatorio","N",$hab,"Observação",120,15,5000);
                    ?>
                </td>
            </tr>
        </table>

        <table id="anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
            <tr>
                <td></td>
            </tr>
            <?php listaAnexos('T'); ?>
        </table>

        <?php

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        if( $estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD ){
            if(in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(UO_EQUIPE_TECNICA, $perfis)){

                ?>
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
                    <tr>
                        <td colspan="3">
                            <a href="javascript:void(0)" class="inserirAnex2">
                                <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Inserir Anexo
                            </a>
                        </td>
                    </tr>
                </table>
            <?php
            }
        }else{
            ?>
            <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
                <tr>
                    <td colspan="3">
                        <a href="javascript:void(0)" class="inserirAnex2">
                            <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Inserir Anexo
                        </a>
                    </td>
                </tr>
            </table>
        <?php
        }
        ?>
    <?endif;?>

    <!--
    <?php /*if (EM_EXECUCAO == $estadoAtual) : ?>
    <script type="text/javascript">
        $(function(){
            $("#gerarExtratoDOU").click(function(){
                location.href=location.href+"&gerarDOU=true";
            });
        })
    </script>

    <!-- GERAR EXTRATO DO DOU -->
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td align="center">
                <input type="button" id="gerarExtratoDOU" value="Gerar Extrato DOU">
            </td>
        </tr>
    </table>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="extratoDOU" style="display: none;">
        <tr>
            <td align="center">
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td colspan="3" align="center">
                            <strong>EXTRATO DE EXECUÇÃO</strong>
                        </td>
                    </tr>
                    <tr>
                        <td width="25%"></td>
                        <td id="extrato"></td>
                        <td width="25%"></td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM MARCAÇÂO - GERAR EXTRATO DO DOU -->
    <?php endif;*/ ?>
    -->

    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="3" align="center">
                <?php
                // Apresenta o botão apenas se o termo estiver em execução
                if (EM_EXECUCAO == $estadoAtual && $_concedente == UG_FNDE) {
                    disponibilizarParaPagamento();
                }
                $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
                if ($_SESSION['elabrev']['termo']['aba'] == 'tramite' && $estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD &&
                    in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(UO_EQUIPE_TECNICA, $perfis)) {
                    echo '<input type="button" id="salva" value="Gravar" class="salvar">';
                }
                ?>
                <input type="button" value="Voltar" class="voltarT" onclick="history.back();">
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript">
$(function(){

    /**
     * Mensagem de alerta para estados que tramitam o relatório de cumprimento do objeto para analise
     * a mensagem informa que apos encaminhar o relatório, não será possivel retornar o termo
     * para o estado anterior
     */
    var aedid = [2929, 2930, 2931, 2935, 2937, 2939, 2940, 1644, 2932, 2928, 2934, 2938, 2936, 2933]
      , preSeletor = '#td_acao_'
      , $seletor;

    for (var i = 0; i < aedid.length; i++) {
        $seletor = $(preSeletor+aedid[i]);

        if ($seletor.length) {

            var el = $seletor.find("a")
              , action = $(el).attr("onclick");

            $(el).attr("onclick", "");
            $seletor.click(function(){
                if (confirm("O encaminhamento do relatório de cumprimento do objeto implicará na finalização do termo, não sendo possível outros ajustes posteriores. Confirma a tramitação?")) {
                    action();
                    return true;
                }
                return false;
            });
        }
    }
});
</script>