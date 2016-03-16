<?php
if($_REQUEST['amiid']){
    $acompanhamentoMI = new Acompanhamento_Mi($_REQUEST['amiid']);
    $obrid = $acompanhamentoMI->obrid;
}

$obrid = isset($_REQUEST['obrid']) ? $_REQUEST['obrid'] :  (($obrid) ? $obrid : $_SESSION['obras2']['obrid']) ;
$obra = new Obras($obrid);

$empresaMI = new EmpresaMi();
if(!$empresaMI->verificaAcesso($obrid)){
    die("<script>
            alert('Você não possui acesso a esta obra!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}


$_SESSION['obras2']['obrid'] = $obrid;
$obrid = $_SESSION['obras2']['obrid'];
// empreendimento || obra || orgao

switch ($_REQUEST['ajax']) {
    case 'delete':

        $amiid = $_GET['amiid'];
        $acompanhamento = new Acompanhamento_Mi($amiid);

        $empresaMI = new EmpresaMi();
        if($empresaMI->verificaAcesso($acompanhamento->obrid)){
            $acompanhamento->amistatus = 'I';
            $acompanhamento->salvar();
            $acompanhamento->commit();
            $retorno = array(
                'exclusao' => 'ok',
                'amiid' => $amiid
            );
        } else {
            $retorno = array(
                'exclusao' => 'erro',
                'amiid' => $amiid
            );
        }

        die(simec_json_encode($retorno));
}

if($_GET['download']){
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid( $_GET['download'] );
    $eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo( $_GET['download'] );

    die('<script type="text/javascript">
			window.close();
		  </script>');
}

if($_POST){
    $dados = $_POST;
    $acompanhamentoMI = new Acompanhamento_Mi();
    if($dados['amiid']){
        $acompanhamentoMI->carregarPorId($dados['amiid']);
    }

    $acompanhamentoMI->obrid = $obrid;
    $acompanhamentoMI->amiobraexecucao = $dados['amiobraexecucao'];
    $acompanhamentoMI->amipercexecucao = MoedaToBd($dados['amipercexecucao']);
    $acompanhamentoMI->amiobs = $dados['amiobs'];
    $acompanhamentoMI->amiproblema = $dados['amiproblema'];
    $acompanhamentoMI->amidescproblema = $dados['amidescproblema'];
    $acompanhamentoMI->amiedicao = ($dados['amiedicao']) ? $dados['amiedicao'] : 'f';
    $acompanhamentoMI->amidataacompanhamento = formata_data_sql($dados['amidataacompanhamento']);
    $acompanhamentoMI->amiusuario = $_SESSION['usucpf'];
    $acompanhamentoMI->salvar();
    $acompanhamentoMI->commit();

    if($_FILES['arquivo']['name'][0] != ''){

        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $arquivos = $_FILES;

        foreach($arquivos['arquivo']['name'] as $key => $value){

            $files = array(
                'arquivo' => array(
                    'name' => $arquivos['arquivo']['name'][$key],
                    'type' => $arquivos['arquivo']['type'][$key],
                    'tmp_name' => $arquivos['arquivo']['tmp_name'][$key],
                    'error' => $arquivos['arquivo']['error'][$key],
                    'size' => $arquivos['arquivo']['size'][$key],
                )
            );
            $_FILES = $files;
            $file = new FilesSimec('arquivo', null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload(null, 'arquivo', false);
            $arqid = $file->getIdArquivo();
            $_POST['arquivo'][] = $arqid;

            $arquivoAcompanhamento = new Acompanhamento_Mi_Arquivos();
            $arquivoAcompanhamento->amiid = $acompanhamentoMI->amiid;
            $arquivoAcompanhamento->arqid = $arqid;
            $arquivoAcompanhamento->amastatus = 'A';
            $arquivoAcompanhamento->salvar();
            $arquivoAcompanhamento->commit();
        }

    }
    verificaArqid($acompanhamentoMI->amiid, $_POST['arquivo']);

    // Envia email novo acompanhamento
    if(!$dados['amiid']){
        enviaEmailAcompanhamento($acompanhamentoMI->amiid);
    }

    echo '<script type="text/javascript">
                alert("Operação realizada com sucesso.");
                document.location.href = "/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A" + "&amiid='.$acompanhamentoMI->amiid.'";
          </script>';
    exit;
}

function verificaArqid($amiid, $arqid)
{
    $arqid = (is_array($arqid)) ? $arqid : array();
    $arquivosMi = new Acompanhamento_Mi_Arquivos();
    $arquivos = $arquivosMi->pegaArquivosPorAmiid($amiid);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $arquivosMi->carregarPorId($arquivo['amaid']);
                $arquivosMi->amastatus = 'I';
                $arquivosMi->salvar();
                $arquivosMi->commit();
                $arquivosMi->clearDados();
            }
        }
    }
}


include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);

echo cabecalhoObra($obrid);
echo "<br>";
monta_titulo( 'Acompanhamento MI', '' );
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <script type="text/javascript">

        $(document).ready(function(){
            $('[type="text"]').keyup();
            $('.habilitarEdicao').click(function () {
                $('input[name=amiedicao]').val('t');
                $('#formAcompanhamentoMI').submit();
            });

            $('.incluir').click(function(){
                var stop = false;
                $('#tableAcompanhamentoMI tr:visible').find('.obrigatorio').each(function(){
                    if( $(this).val() == '' ){
                        stop = true;
                        alert('Campo obrigatório.');
                        $(this).focus();
                        return false;
                    }
                });
                if( stop ){
                    return false;
                }
                $('#req').val('salvar');
                $('#formAcompanhamentoMI').submit();
            });
        });
    </script>

    <?php
    $acompanhamentoMI = array();
    if($_REQUEST['amiid']){
        $acompanhamentoMI = new Acompanhamento_Mi($_REQUEST['amiid']);
        $dados = $acompanhamentoMI->getDados();

        $dados['amidataacompanhamento'] = formata_data($dados['amidataacompanhamento']);
        $dados['amipercexecucao'] = number_format($dados['amipercexecucao'],2,',','.');
        extract($dados);
    }

    $habilitado = true;
    $habilita 	= 'S';

    if($amiedicao == 'f'){
        $habilitado = false;
        $habilita 	= 'N';
    }

    ?>

    <form method="post" name="formAcompanhamentoMI" id="formAcompanhamentoMI" enctype="multipart/form-data">
            <table id="tableAcompanhamentoMI" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <input type="hidden" name="amiid" value="<?=$amiid?>"/>
                <input type="hidden" name="amiedicao" value=""/>
                <tr>
                    <td class="SubTituloDireita" width="20%">Data de referência deste acompanhamento</td>
                    <td>
                        <?php
                        echo campo_data2('amidataacompanhamento', 'S', $habilita, '', 'S', '', '', null, '');
                        ?>
                    </td>
                    <td rowspan="8" align="right" valign="topcampo_radio" width="1">
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">A obra está em execução?</td>
                    <td>
                        <?
                            $opcoes = array(
                                'Sim' => array(
                                    'valor' => 'S',
                                    'id' => 'tipo_s',
                                ),
                                'Não' => array(
                                    'valor' => 'N',
                                    'id' => 'tipo_N',
                                    'default' => true,
                                ),
                                'Conluída' => array(
                                    'valor' => 'C',
                                    'id' => 'tipo_C',
                                )
                            );
                            if($amiobraexecucao){
                                unset($opcoes['Não']['default']);
                            }

                            campo_radio('amiobraexecucao', $opcoes, 'v'); ?>

                    </td>
                </tr>
                <tr class="amipercexecucao">
                    <td class="SubTituloDireita" style="width: 190px;">Qual o percentual executado da obra?</td>
                    <td>
                        <?=campo_texto('amipercexecucao', 'S', $habilita, '', 7, 6, '', '', 'right', '', 0, '', 'managerEtapa(this)', number_format($amipercexecucao, 2, ',', '.')); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">Observações</td>
                    <td>
                        <?=campo_textarea('amiobs', 'N', $habilita, '', '70', '8', '2000', '', 0, '', '', false, '', '');?>
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">Deseja sinalizar problema(s) com esta obra para o FNDE/MEC?</td>
                    <td>
                        <?
                        $opcoes = array(
                            'Sim' => array(
                                'valor' => 't',
                                'id' => 'tipo_s',
                            ),
                            'Não' => array(
                                'valor' => 'f',
                                'id' => 'tipo_N',
                                'default' => true,
                            ),
                        );
                        if($amiproblema)
                            unset($opcoes['Não']['default']);
                        campo_radio('amiproblema', $opcoes, 'h'); ?>
                    </td>
                </tr>

                <tr class="amidescproblema">
                    <td class="SubTituloDireita" style="width: 190px;">Descreva o problema</td>
                    <td>
                        <?=campo_textarea('amidescproblema', 'S', $habilita, '', '70', '8', '2000', '', 0, '', '', false, '', '');?>
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">Arquivos</td>
                    <td>
                        <div>
                            <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela listagem " cellpadding="3" cellspacing="1">
                                <tr>
                                    <td class="" colspan="2">
                                        <? if ($habilitado): ?>
                                        <div style=""><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo">Adicionar</a></div>
                                        <? endif; ?>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="SubTituloEsquerda">Ação</td>
                                    <td class="SubTituloEsquerda">Arquivo</td>
                                </tr>
                                <?
                                    $arquivosMi = new Acompanhamento_Mi_Arquivos();
                                    $arquivos = ($amiid) ? $arquivosMi->pegaArquivosPorAmiid($amiid) : array();
                                ?>
                                <? if (!empty($arquivos)): ?>
                                <? foreach ($arquivos as $arquivo): ?>
                                    <tr class="anexos">
                                        <td class="" style="width: 56px;">
                                        <? if ($habilitado): ?>
                                            <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span>
                                        <? endif; ?>
                                        </td>
                                        <td class="">
                                            <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo[]" id="arquivo"/>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&download=<?=$arquivo['arqid']?>">
                                                <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                            </a>
                                        </td>
                                    </tr>
                                <? endforeach; ?>
                                <? endif; ?>
                                <? if ($habilitado): ?>
                                <tr class="anexos">
                                    <td class="" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td>
                                    <td class="" ><input type="file" name="arquivo[]" id="arquivo"/></td>
                                </tr>
                                <? endif; ?>

                            </table>
                        </div>
                    </td>
                </tr>
                <script type="text/javascript">
                    $(function(){
                        $('#adicionar_anexo').click(function (e) {
                            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class=""><input type="file" name="arquivo[]" id="arquivo"/></td></tr>').removeClass('anexos-base'));
                            e.preventDefault();
                        });
                        $('.excluir_anexo').live('click',function (e) {
                            $(this).parents('tr.anexos').remove();
                            e.preventDefault();
                        });
                    });
                </script>


                <tr>
                    <td style="background-color:#DCDCDC" colspan="3" align="center">
                        <?php if( $habilitado ){ ?>
                            <input type="button" name="salvar" class="incluir" value="Salvar"/>
                        <?php } else if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))){ ?>
                            <input type="button" name="habilitarEdicao" class="habilitarEdicao" value="Habilitar Edição"/>
                        <?php } ?>

                        <?php if( $_GET['amiid'] ){ ?>
                            <input type="button" name="novo" id="novo_acompanhamento" value="Novo Acompanhamento"/>
                        <?php } ?>
                    </td>
                </tr>
            </table>
    </form>

<script type="text/javascript">

    $(function(){
        $('#novo_acompanhamento').click(function (e) {
            window.location = "/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&obrid=<?=$obrid?>";
            return false;
        });
    })

    var obCalc = new Calculo();

    managerEtapa = function(obj) {
        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (obCalc.comparar(new String(vlrObj), '100,00', '>')) {
            obj.value = '100,00';
        }

        formatarNumeroMonetario(obj);
    }

    formatarNumeroMonetario = function(obj) {
        var valor = new String(obCalc.retiraZeroEsquerda(new String(obj.value)));
        if (valor.length == 2) {
            valor = '0' + valor;
        } else if (valor.length == 1) {
            valor = '00' + valor;
        } else if (valor.length == 0) {
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }

    $(function(){

        if($('input[name=amiobraexecucao]:checked').val() != 'S')
            $('tr.amipercexecucao').hide();
        if( $('input[name=amiproblema]:checked').val() != 't')
            $('tr.amidescproblema').hide();

        $('input[name=amiobraexecucao]').change(function(){
            if($(this).val() == 'S')
                $('tr.amipercexecucao').show();
            else
                $('tr.amipercexecucao').hide();
        });

        $('input[name=amiproblema]').change(function(){
            if($(this).val() == 't')
                $('tr.amidescproblema').show();
            else
                $('tr.amidescproblema').hide();
        });

        <? if($habilita == 'N'): ?>
            $('#tableAcompanhamentoMI input[type="radio"]').attr('disabled','disabled');
        <? endif; ?>

    });
</script>


<?

if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))){
    $excluir = "<img
                            align=\"absmiddle\"
                            src=\"/imagens/excluir.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: excluirAcompanhamento('|| ami.amiid ||');\"
                            title=\"Alterar Obra\" />";
}


$acao = "
               ' <center><div style=\"width:100px\">
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/consultar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: verAcompanhamento('|| ami.amiid ||');\"
                            title=\"Alterar Obra\" />'
                    || CASE WHEN ami.amiedicao = 't' THEN
                        ' <img
                                align=\"absmiddle\"
                                src=\"/imagens/editar_nome.gif\"
                                style=\"cursor: pointer\"
                                onclick=\"javascript: alterarAcompanhamento('|| ami.amiid ||');\"
                                title=\"Alterar Obra\" /> '
                      ELSE ' '
                    END ||
                    '$excluir
                </div></center>'

     as acao, ";


$sql = "

    SELECT
        $acao
        o.obrid,
        o.obrnome,
        est.estuf,
        mun.mundescricao,
        emi.emidsc,
        u.usunome,
        TO_CHAR(ami.amidataacompanhamento, 'DD/MM/YYYY'),
        (
            SELECT CASE WHEN COUNT(*) > 0 THEN 'SIM' ELSE 'NÃO' END
            FROM obras2.acompanhamento_mi_arquivos am
            JOIN arquivo a ON a.arqid = am.arqid
            WHERE am.amiid = ami.amiid AND am.amastatus = 'A' AND (a.arqtipo ILIKE '%jpg%' OR a.arqtipo ILIKE '%png%' OR a.arqtipo ILIKE '%bmp%' OR a.arqtipo ILIKE '%image%')
        ) as foto,
        (SELECT CASE WHEN COUNT(*) > 0 THEN 'SIM' ELSE 'NÃO' END FROM obras2.acompanhamento_mi_arquivos am WHERE am.amiid = ami.amiid AND am.amastatus = 'A') as anexo,
        CASE WHEN amiobraexecucao = 'S'::bpchar THEN 'SIM' WHEN  amiobraexecucao = 'N'::bpchar THEN 'NÃO' WHEN amiobraexecucao = 'C'::bpchar THEN 'CONCLUÍDO' END as execucao,
        amipercexecucao,
        CASE WHEN amiproblema = 't' THEN 'SIM' ELSE 'NÃO' END as amiproblema

    FROM obras2.acompanhamento_mi ami
    JOIN obras2.obras o ON o.obrid = ami.obrid AND o.obridpai IS NULL AND o.obrstatus = 'A'
    LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
    LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
    LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
    LEFT JOIN obras2.empresami_uf euf ON euf.estuf = mun.estuf AND euf.eufstatus = 'A'
    LEFT JOIN obras2.empresami emi ON emi.emiid = euf.emiid AND emi.emistatus = 'A'
    LEFT JOIN seguranca.usuario u ON u.usucpf = ami.amiusuario
    WHERE amistatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
    AND o.obrid = $obrid
     ORDER BY ami.amidataacompanhamento

";

monta_titulo( 'Acompanhamentos da obra (' . $obra->obrid . ') ' . $obra->obrnome, '' );

$cabecalho = array('Ação', 'ID', 'Nome', 'UF', 'Municipio', 'Empresa', 'Quem Fez', 'Quando Fez', 'Possui Foto', 'Possui anexos', 'Execução', 'Percentual', 'Problema');
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', 'formulario', '', '', null, $_param);
?>


<script type="text/javascript">
    function verAcompanhamento(amiid)
    {
        document.location.href = "/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&amiid=" + amiid;
    }
    function alterarAcompanhamento(amiid)
    {
        document.location.href = "/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&amiid=" + amiid;
    }
    function excluirAcompanhamento(amiid)
    {
        if (window.confirm("Realmente deseja excluir o acompanhamento?")) {
            $.getJSON('/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&ajax=delete&amiid=' + amiid, function(data){
                console.info(data);
                if(data.exclusao == 'ok'){
                    alert('Acompanhamento exluído com sucesso.');
                    window.location = '/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A';
                } else {
                    alert('Erro ao tentar excluir o acompanhamento.');
                }
            });
        }

    }
</script>




<?php

function enviaEmailAcompanhamento($amiid)
{
    require_once APPRAIZ . "includes/classes/dateTime.inc";
    require_once APPRAIZ . "includes/classes/entidades.class.inc";
    global $db;

    //INSERT INTO obras2.tipoemail (temid, temnome, temdescricao) VALUES (35, 'Acompanhamento MI', 'Acompanhamento MI');

    $acompanhamentoMI = new Acompanhamento_Mi($amiid);
    $obra = new Obras($acompanhamentoMI->obrid);
    $contato = new Contato();
    $email = new Email();

    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
    $dados = array(
        'usucpf' => $_SESSION['usucpf'],
        'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Cadastro de Acompanhamento na obra (' . $obra->obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor,</p>

                                                <p>Um novo acompanhamento foi inserido na obra (' . $obra->obrid . ') ' . $obra->obrnome . '.</p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
        'emlassunto' => 'Cadastro de Acompanhamento na obra (' . $obra->obrid . ') ' . $obra->obrnome . '',
        'temid' => 35,
        'emlregistroatividade' => 'false',
        'obrid' => $obra->obrid
    );

    $dadosRemetentes = array('marcelo.santos@fnde.gov.br');

    $email->popularDadosObjeto($dados);
    $email->salvar($dadosRemetentes);
    $email->enviar();

}

?>