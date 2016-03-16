<?php

include APPRAIZ . "includes/classes/PaginacaoAjax.class.inc";
include APPRAIZ . "conjur/classes/controle/Documentos.class.inc";

if ($_GET['nudidDel']) {
    if (possuiPerfil(array(PRF_ADMINISTRADOR, PRF_SUPERUSUARIO, PRF_TECNICO_ADM))) {
        $sql = "select tpdid from conjur.sapiensdocumento where spdstatus = 'A' and spdid = " . $_GET['nudidDel'];
        $tpdid = $db->pegaUm($sql);
        $sql = "select count(1) from conjur.sapiensanexo where spastatus = 'A' and spaid = {$tpdid} and spdid = " . $_GET['nudidDel'];
        if ($db->pegaUm($sql)) {
            echo "<script>alert('Já existe um arquivo com este número, exclua primeiro o documento.'); window.location.href = 'conjur.php?modulo=principal/documentosSapiens&acao=A';</script>";
            exit();
        }
        $sql = "UPDATE conjur.sapiensdocumento SET spdstatus = 'I' where spdid = " . $_GET['nudidDel'];
        $db->executar($sql);
        $db->commit();
        echo "<script>window.location.href = 'conjur.php?modulo=principal/documentosSapiens&acao=A';</script>";
        exit();
    }
}

if ($_POST['requisicao'] == 'pesquisar') {
    Documentos::lista(array("filtro" => $_POST, 'nrRegPorPagina' => 200));
    die;
}

if ($_POST['requisicao'] == 'gerarNumero') {
    $prcnumsidoc = trim($_POST['prcnumsidoc']);
    
    if( $prcnumsidoc ){
        $sql = "SELECT prcid as numero FROM conjur.processoconjur WHERE prcstatus = 'A' AND prcnumsidoc = '{$prcnumsidoc}' ";
        $prcid = $db->pegaUm($sql);
    }
    
    if( !$prcid ){
        die('semNumeroSidoc');
    }else{
        $tpdid      = trim( $_POST['tpdid'] );
        $spdnumero  = trim( $_POST['spdnumero'] );

        if( $tpdid > 0 && $spdnumero > 0 ){
            
            $sql = "SELECT spdid FROM conjur.sapiensdocumento WHERE spdnumero = '{$spdnumero}' and tpdid = {$tpdid} and spdstatus = 'A'";
            $spdid = $db->pegaUm($sql);

            if( $spdid > 0 ){
                die('existeDocComSidocTipo');
            }else{
                $advid  = $_POST['advid'] ? $_POST['advid'] : 'NULL';
                $usucpf = $_SESSION['usucpf'];
                $dtDia  = date('Y-m-d');
                $anoAt  = date('Y');

                $sql = "SELECT spdid FROM conjur.sapiensdocumento WHERE spdnumero = '{$spdnumero}' and tpdid = {$tpdid}";
                $spdid = $db->pegaUm($sql);

                if ($spdid == '') {
                    #INSERE UMA NUMERAÇÃO
                    $sql = "
                    INSERT INTO conjur.sapiensdocumento(
                            tpdid, spdnumero, usucpf, spdddatainclusao, spdstatus, spdano, prcid, advid
                        ) VALUES (
                            {$tpdid}, {$spdnumero}, '{$usucpf}', '{$dtDia}', 'A',  '{$anoAt}', $prcid, {$advid}
                    ) RETURNING spdid
                ";
                    $spdid = $db->pegaUm($sql);
                } else {
                    $sql = "update conjur.sapiensdocumento set
                                prcid = {$prcid},
                                spdstatus = 'A'
                            where spdid = {$spdid}";
                    $db->executar($sql);
                }

                if ($spdid > 0 ) {
                    $db->commit();
                    Documentos::lista(array('nrRegPorPagina' => 200));
                    die;
                }
            }
        }
    }

    # verifica se o número do sidoc existe
    if ($prcnumsidoc){
//		$sql = "SELECT count(1) FROM conjur.processoconjur WHERE prcstatus = 'A' AND prcnumsidoc = '$prcnumsidoc'";
//		$boExistePrcnumsidoc = $db->pegaUm($sql);
        # pegamos o prcid
        $sql = "SELECT prcid as numero FROM conjur.processoconjur WHERE prcstatus = 'A' AND prcnumsidoc = '$prcnumsidoc' ";
        $prcid = $db->pegaUm($sql);

        if (!$prcid) {
            die('semNumeroSidoc');
        }

        if ($_POST['tpdid']) {

            $_POST['advid'] = $_POST['advid'] ? $_POST['advid'] : 'NULL';


            //insere uma numeração
            $sql = "INSERT INTO conjur.sapiensdocumento (tpdid, spdnumero, usucpf, spdddatainclusao, spdstatus, spdano, prcid, advid)
				        VALUES ({$_POST['tpdid']}, {$_POST['spdnumero']}, '{$_SESSION['usucpf']}', '" . date('Y-m-d') . "', 'A',  '" . date('Y') . "', $prcid, " . $_POST['advid'] . ")";
            $db->executar($sql);
            if ($db->commit()) {
                Documentos::lista(array('nrRegPorPagina' => 200));
                die;
            }

        }

    }

    die;
}

if ($_REQUEST['requisicao'] == 'buscanumero') {
    $sql = "select prcnumsidoc from conjur.processoconjur";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '') return;

    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($key), $q) !== false) {
            $prcnumsidoc = trim($value['prcnumsidoc']);
            echo "{$prcnumsidoc}|{$prcnumsidoc}\n";
        }
    }

    die;

}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
if ($_SESSION['conjur_var']['prcid'] != '') {
    echo montarAbasArray(monta_abas_processo($_SESSION['conjur_var']['tprid']), $_SERVER['REQUEST_URI']);
} else {
    montaAbaInicio("conjur.php?modulo=principal/documentosSapiens&acao=A");
}

$titulo = "Documentos Sapiens";
monta_titulo($titulo, '&nbsp;');

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="./js/conjur.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"/>

<script type="text/javascript">
    <!--
    $(document).ready(function () {

        $('#prcnumsidoc').autocomplete("conjur.php?modulo=principal/geraNumeracao&acao=A&requisicao=buscanumero", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true

        });

        $('#limpar').click(function () {
            $('#tpdid_pesq').val('');
            $('#usucpf').val('');
            $('#advid').val('');
            $('#dtinicio').val('');
            $('#dtfim').val('');
            $('#prcnumsidoc_pesq').val('');

        });

        $('#pesquisar').click(function () {
            $('#pesquisar').attr("disabled", true);
            var data = new Array();
            var nudstatus = 'A';
            if ($('#nudstatusI').attr("checked")) {
                nudstatus = 'I';
            }
            data.push({name: 'requisicao', value: 'pesquisar'},
                {name: 'tpdid_pesq', value: $('#tpdid_pesq').val()},
                {name: 'nudnumero', value: $('#nudnumero').val()},
                {name: 'prcnumsidoc_pesq', value: $('#prcnumsidoc_pesq').val()},
                {name: 'usucpf', value: $('#usucpf').val()},
                {name: 'advid', value: $('#advid2').val()},
                {name: 'administrativo', value: $('#administrativo').attr('checked')},
                {name: 'dtinicio', value: $('#dtinicio').val()},
                {name: 'nudstatus', value: nudstatus},
                {name: 'dtfim', value: $('#dtfim').val()}
            );
            $.ajax({
                type: "POST",
                url: "conjur.php?modulo=principal/documentosSapiens&acao=A",
                data: data,
                async: false,
                success: function (data) {
                    $('#listaNumeracao').html(data);
                    $('#pesquisar').attr("disabled", false);
                }
            });


        });

        $('#gerar').click(function () {
            var msg = '';

            if ($('#tpdid').val() == '') {
                msg += 'É necessário escolher Tipo de Documento\n';
            }
            if ($('#prcnumsidoc').val() == '') {
                msg += 'É necessário digitar algum Número do Processo\n';
            }
//		if($('#advid').val() == ''){
//			msg += 'É necessário informar o Advogado Requerinte\n';
//		}

            if (msg) {
                alert(msg);
                return false;
            }

            var data = new Array();
            data.push({
                    name: 'requisicao', value: 'gerarNumero'
                },{
                    name: 'tpdid', value: $('#tpdid').val()
                },{
                    name: 'prcnumsidoc', value: $('#prcnumsidoc').val()
                },{
                    name: 'spdnumero', value: $('#spdnumero').val()
                },{
                    name: 'advid', value: $('#advid').val()
                }
            );
            $.ajax({
                type: "POST",
                url: "conjur.php?modulo=principal/documentosSapiens&acao=A",
                data: data,
                async: false,
                success: function (data) {
                    if( data == 'semNumeroSidoc' ){
                        alert('Número do Processo inválido.');
                    } else if( data == 'jaExisteNota' ){
                        alert('Erro: Número Sidoc diferente do que já existe.');
                    } else if( data == 'existeDocComSidocTipo' ){
                        alert('Erro: Não é possível realizar a operação. Já existe documento cadastrado com o "Nº Doc. SAPIENS" e "Tipo de Documento" informado. Verifique e tente novamente!');
                    } else {
                        alert('Número gerado com sucesso.');

                        $('#listaNumeracao').html(data);
                        $('#tpdid').val('');
                        $('#prcnumsidoc').val('');
                        $('#advid').val('');
                    }
                }
            });
        });
    });

    function excluirNudid(nudid) {
        if (confirm('Deseja excluir o Item?')) {
            location.href = 'conjur.php?modulo=principal/documentosSapiens&acao=A&nudidDel=' + nudid;
        }
    }

    function numProcesso(obj) {
        var prcnumsidoc = obj.value;

        if (prcnumsidoc == '') {
            return;
        }

        if (!conferirDigitoVerificadorModulo11SemMensagem(obj)) {
            if (prcnumsidoc.length != 9) {
                alert('Número inválido');
                obj.focus();
                return false;
            }
            else {
                var ano = prcnumsidoc.substring(0, 4);
                var hoje = new Date();

                if (ano < 1900 || ano > hoje.getFullYear()) {
                    alert('número inválido');
                    obj.focus();
                    return false;
                }
                else {
                    //é um número emec válido
                    return true;
                }
            }
        }
        else {
            //é um numero sidoc válido
            return true;
        }


    }

    --></script>
    <form method="post" name="formulario" id="formulario">
        <table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
            <tr>
                <td style="font-weight: bold; text-align: center; width: 350px;">Tipo de documento:</td>
                <td style="font-weight: bold; text-align: center; width: 200px;">Nº Doc. SAPIENS</td>
                <td style="font-weight: bold; text-align: center; width: 200px;">Número Sidoc/Emec:</td>
                <td style="font-weight: bold; text-align: center; width: 350px;">Advogado Requerinte:</td>
                <td style="width: 80px;">&nbsp;</td>
            </tr>
            <tr>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tipodoc.tpdid  as codigo,
                                    tipodoc.tpddsc as descricao
                            FROM conjur.tipodocumento as tipodoc

                            WHERE tipodoc.tpdstatus = 'A' and tpdgeranumeracao = 'S'
                            ORDER BY tipodoc.tpddsc ASC
                        ";
                        $db->monta_combo('tpdid', $sql, 'S', "Selecione...", '', '', '', 400, 'S', 'tpdid');
                    ?>
                </td>
                <td>
                    <?PHP
                        //retirada a validação do numero sidoc emec -> chamar função numProcesso(this)
                        echo campo_texto('spdnumero', 'S', $permissao_formulario, 'Nº Doc. SAPIENS', 30, 8, '#################', '', '', '', '', 'id="spdnumero" autocomplete="off" ', '', '', '');
                    ?>
                </td>
                <td>
                    <?PHP
                        //retirada a validação do numero sidoc emec -> chamar função numProcesso(this)
                        echo campo_texto('prcnumsidoc', 'S', $permissao_formulario, 'Número Processo', 30, 255, '#################', '', '', '', '', 'id="prcnumsidoc" autocomplete="off" ', '', '', '');
                    ?>
                </td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  a.advid AS codigo,
                                    e.entnome AS descricao
                            FROM  conjur.advogados a

                            INNER JOIN entidade.entidade e ON e.entid=a.entid

                            WHERE entstatus='A'

                            ORDER BY 2
                        ";
                        $db->monta_combo('advid', $sql, 'S', "ADMINISTRATIVO", '', '', '', 400, 'N', 'advid');
                    ?>
                </td>
                <td style="font-weight: bold; text-align: center;">
                    <input type="button" name="gerar" id="gerar" value="Incluir"/>
                </td>
            </tr>
        </table>
    </form>

<?php if (possuiPerfil(array(PRF_ADMINISTRADOR, PRF_SUPERUSUARIO, PRF_TECNICO_ADM))): ?>

    <form method="post" name="formPesq" id="formPesq">
        <input type="hidden" name="salvar" value="0">
        <table class="tabela listagem" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
            <tr style="background-color: #cccccc">
                <td align='left' style="vertical-align:top; width:25%; font-size:12px" colspan="2"><strong>Filtro de
                        pesquisa</strong></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Nome:</td>
                <td>
                    <?php
                    $sql = "select distinct u.usucpf as codigo, u.usunome as descricao from conjur.numeracaodocumento nd
							inner join seguranca.usuario u on u.usucpf = nd.nudusucpf
							ORDER BY
								u.usunome ASC";
                    $db->monta_combo('usucpf', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'usucpf');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Advogado Requerente:
                </td>
                <td>
                    <?php
                    $sql = "SELECT
							a.advid AS codigo,
							e.entnome AS descricao
						FROM
							conjur.advogados a
						INNER JOIN entidade.entidade e ON e.entid=a.entid
						WHERE
							entstatus='A'
						ORDER BY
							2";
                    $db->monta_combo('advid2', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'advid2');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Requerente
                    Administrativo:
                </td>
                <td>
                    <input type="checkbox" name="administrativo" id="administrativo" value="true"/>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
                <td>
                    <?php
                    $sql = "SELECT
							 	tipodoc.tpdid  as codigo,
								tipodoc.tpddsc as descricao
							FROM
								conjur.tipodocumento as tipodoc
							WHERE
								tipodoc.tpdstatus = 'A' and tpdgeranumeracao = 'S'
							ORDER BY
								tipodoc.tpddsc ASC";
                    $db->monta_combo('tpdid_pesq', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'tpdid_pesq');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Número do Documento:
                </td>
                <td>
                    <?php
                    echo campo_texto('nudnumero', 'N', 'S', 'Número Documento', 6, 7, '######', '', '', '', '', 'id="nudnumero" autocomplete="off" ');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Número do Processo:
                </td>
                <td>
                    <?php
                    echo campo_texto('prcnumsidoc_pesq', 'N', 'S', 'Número Processo', 30, 255, '', '', '', '', '', 'id="prcnumsidoc_pesq" autocomplete="off" ');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Período:</td>
                <td>
                    <?php
                    echo campo_data2('dtinicio', 'N', 'S', 'Data', '##/##/####');
                    echo "&nbsp;&nbsp;";
                    echo campo_data2('dtfim', 'N', 'S', 'Data', '##/##/####');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivos Excluidos:
                </td>
                <td>
                    <input type="radio" value="I" id="nudstatusI" name="nudstatus">Excluídos
                    <br/>
                    <input type="radio" value="A" id="nudstatusA" name="nudstatus" checked="checked">Não Excluídos
                </td>
            </tr>
            <tr style="background-color: #cccccc">
                <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
                <td height="30"><input type="button" name="pesquisar" id="pesquisar" value="Pesquisar"/>&nbsp;
                    <input type="button" name="limpar" id="limpar" value="Limpar"/></td>
            </tr>
        </table>
    </form>
<?php endif; ?>
<div id="listaNumeracao"><?php Documentos::lista(array("filtro" => $_POST, 'nrRegPorPagina' => 200)); ?></div>
<div id="divDebug"></div>