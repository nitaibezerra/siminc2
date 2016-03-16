<?php
if ($_GET['arquivo']) {
    include_once APPRAIZ . "includes/classes/file.class.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $file->getDownloadArquivo($_GET['arquivo']);
    exit;
}




if ($_REQUEST['requisicaoAjax'] && !$_REQUEST['classe']) {
    $_REQUEST['requisicaoAjax']();
    die;
}
if ($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']) {
    $n = new $_REQUEST['classe'];
    $n->$_REQUEST['requisicaoAjax']();
    die;
}
if ($_REQUEST['requisicao'] && $_REQUEST['classe']) {
    $n = new $_REQUEST['classe'];
    $n->$_REQUEST['requisicao']();
}
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

if ($_GET['pagid']) {
    $pag = new Pagamento($_GET['pagid']);
    extract($pag->getDados());
    if ($sgeid) {
        $grupoEmpresa = new Supervisao_Grupo_Empresa($sgeid);
        $sgrid = $grupoEmpresa->sgrid;
    }
}

if ($docid) {
    $arrEstadoAtual = wf_pegarEstadoAtual($docid);
    if ($arrEstadoAtual['esdid'] != WF_ESDID_PAGAMENTO_SUPERVISAO_EM_CADASTRAMENTO) {
        $somente_leitura = true;
        $habilitado = "N";
    }
    $arrPerfil = pegaPerfilGeral();
    //Devem ser colocados aqui os perfis que podem SOLICITAR pagamento (situação Enviado para FNDE) 
    if (in_array(PFLCOD_SUPER_USUARIO, $arrPerfil) && $arrEstadoAtual['esdid'] == WF_ESDID_PAGAMENTO_SUPERVISAO_ENVIADO_FNDE) {
        $exibeWorkFlow = true;
    }
}

$db->cria_aba($abacod_tela, $url, $parametro);
//criaAbaPagamento();
if ($somente_leitura && !$exibeWorkFlow) {
    monta_titulo("Pagamento", 'Nenhuma ação disponível, pois a situação atual do pagamento é \'' . $arrEstadoAtual['esddsc'] . '.\'');
} else {
    if ($exibeWorkFlow) {
        monta_titulo(" Solicitar Pagamento", '');
    } else {
        monta_titulo(($_GET['pagid'] ? "Editar" : "Cadastrar") . " Pagamento", '');
    }
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<style>.link{cursor:pointer}</style>
<form id="formulario" name="formulario" method="post" enctype="multipart/form-data" action="">
    
    <input type="hidden" name="pagid"      id="pagid" value="<?php echo $pagid ?>" />
    <input type="hidden" name="requisicao" id="requisicao" value="" />
    <input type="hidden" name="classe"     id="classe" value="" />
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr>
            <td class="SubTituloDireita" width="35%">Grupo</td>
            <td>
                <?php
                $grupo = new Supervisao_Grupo();
                $dados = $grupo->listaCombo();
                $db->monta_combo("sgrid", $dados, $habilitado, "Selecione...", "carregaDependenciaGrupo", '', '', '', 'S', 'sgrid');
                ?>
            </td>
            <td width="" rowspan="6" valign="top" >
                <?php
                if ($docid && (!$somente_leitura || $exibeWorkFlow)) {
                    // Barra de estado WORKFLOW
                    wf_desenhaBarraNavegacao($docid, array('pagid' => $pagid, 'aedid' => WF_AEDID_LAUDO_SUPERVISAO_SOLICITAR_PAGAMENTO));
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Empresa</td>
            <td id="tdComboEmpresa">
                <?php
                if (!empty($sgrid)) {
                    $grupoEmpresa = new Supervisao_Grupo_Empresa();
                    $dados = $grupoEmpresa->listaCombo(array('sgrid' => $sgrid));
                    $db->monta_combo("sgeid", $dados, $habilitado, "Selecione...", "carregaDependenciaEmpresa", '', '', '', 'S', 'sgeid');
                } else {
                    echo "Selecione o Grupo.";
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Número da Nota Fiscal:</td> 
            <td>
                <?php echo campo_texto('pagnrnotafiscal', 'S', 'false', '', 17, 35, '###########', '', 'left', '', 0, 'id="pagnrnotafiscal"', '', null, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Nota Fiscal</td>
            <td id="td_arquivo">
                <?php if ($arqid): ?>
                    <?php $arrArquivo = recuperaArquivo($arqid) ?>
                    <a href="javascript:downloadArquivo(<?php echo $arqid ?>)" ><?php echo $arrArquivo['arqnome'] ?>.<?php echo $arrArquivo['arqextensao'] ?></a>
                    <?php if (!$somente_leitura): ?>
                        <img src="../imagens/editar_nome.gif" class="link" onclick="excluirNotaFiscal()" />
                    <?php endif; ?>
                    <input type="hidden" name="arquivo" id="arquivo" value="<?php echo $arqid ?>" />
                <?php else: ?>
                    <input type="file" name="arquivo" id="arquivo" />
                <?php endif; ?>
            </td>
        </tr>


        <tr>
            <td colspan="2" align="center" style="background: #FFFFFF;">
                <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">

                    <tr>
                        <td colspan="2" align="center"><b>Filtrar</b></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Número da OS:</td>
                        <td>
                            <?php
                            echo campo_texto('sosnum', 'N', 'S', '', 47, 60, '', '', 'right', '', 0, '');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" >ID da Obra</td>
                        <td id="td_arquivo">
                            <?php echo campo_texto( 'obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" >UF</td>
                        <td id="td_arquivo">
                            <?php
                            $sql = "SELECT
                                        estuf as codigo,
                                        estdescricao as descricao
                                FROM
                                        territorios.estado
                                ORDER BY
                                        estdescricao";

                            $db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="center">
                            <input type="button" name="btn_filtrar" id="btn_filtrar" value="Filtrar" onclick="" />
                            <input type="button" name="btn_clear" id="btn_clear" value="Limpar" onclick="" />
                        </td>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td colspan="2" class="SubtituloCentro" >Selecione as Supervisões</td>
        </tr>

        <tr>
            <td colspan="2" align="center" style="background: #FFFFFF;">
                <div id="div_lista_supervisao">
                    <?php if ($_GET['pagid']): ?>
                        <?php $pag->carregarSupervisaoPorPagamento($_GET['pagid'], $sgeid, $somente_leitura); ?>
                    <?php else: ?>
                        Selecione a Empresa.
                    <?php endif; ?>
                </div>
            </td>
        </tr>
        <tr>
            <td colspan="2" align="center">
                <?php if (!$somente_leitura): ?>
                    <input type="button" name="btn_pagar" value="Salvar" onclick="salvarPagamento()" />
                <?php endif; ?>
                <input type="button" name="btn_pagar" value="Voltar" onclick="window.location.href = 'obras2.php?modulo=principal/listaPagamento&acao=A'" />
            </td>
        </tr>
    </table>


    <script>

        $(function(){

            $('#btn_clear').click(function(e){
                e.preventDefault();
                $('input[name=obrid]').val('');
                $('input[name=sosnum]').val('');
                $('select[name=estuf]').val('');
                $('#btn_filtrar').click();
            });

            $('#btn_filtrar').click(function(e){
                e.preventDefault();

                var columIdIndex = 1;
                var columOSIndex = 1;
                var columUFIndex = 1;

                var idValue = $('input[name=obrid]').val();
                var oSValue = $('input[name=sosnum]').val();
                var UFValue = $('select[name=estuf]').val();

                if(!idValue && !oSValue && !UFValue) {
                    $('#div_lista_supervisao tbody tr').show();
                } else {
                    $('#div_lista_supervisao tbody tr').each(function(){
                        find = false;

                        if(idValue){
                            if($(this).find('td:eq(1)').text() == idValue){
                                find = true;
                            }
                        }

                        if(UFValue){
                            if($(this).find('td:eq(5)').text() == UFValue){
                                find = true;
                            }
                        }

                        if(oSValue) {
                            if ($(this).find('td:eq(10)').text() == oSValue) {
                                find = true;
                            }
                        }

                        if(!find)
                            $(this).hide();
                        else
                            $(this).show();
                    });
                }

            });

        });

        function carregaDependenciaGrupo(sgrid) {
            if (sgrid) { // , "not(listaObras)":true
                $.post("?modulo=principal/pagamento&acao=A", {"sgrid": sgrid, "requisicaoAjax": "carregaEmpresaAndListaObra", "not(listaObras)": true}, function(data) {
                    var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
                    $('#tdComboEmpresa').html(comboEmpresa);
                    $('#tdComboEmpresa select').val('');
                    $('#div_lista_supervisao').html('Selecione a Empresa.');
                    //                        carregaDependenciaEmpresa($('#tdComboEmpresa select').val());
                });
            } else {
                $('#tdComboEmpresa').html('Selecione o Grupo.');
            }
        }
        function carregaDependenciaEmpresa(sgeid) {

            if (sgeid) {
                $.post("?modulo=principal/pagamento&acao=A", {"sgeid": sgeid, "requisicaoAjax": "carregarSupervisaoPorEmpresa", "classe": "Pagamento"}, function(data) {
                    $('#div_lista_supervisao').html(data);
                });
            } else {
                $('#div_lista_supervisao').html('Selecione a Empresa.');
            }
        }

        function salvarPagamento()
        {
            var erro = 0;
            if (!$("#sgeid").val()) {
                erro = 1;
                alert('Selecione a empresa.');
                return false;
            }
            if (!$("#arquivo").val()) {
                erro = 1;
                alert('Insira a notal fiscal.');
                return false;
            }
            if ($("[name='chk_sueid[]']:checked").length == 0) {
                erro = 1;
                alert('Selecione as supervisões.');
                return false;
            }

            if (!$("#pagnrnotafiscal").val()) {
                erro = 1;
                alert('Informe o número da notal fiscal.');
                return false;
            }

            if (erro == 0) {
                $("#classe").val("Pagamento");
                $("#requisicao").val("salvarPagamento");
                $("#formulario").submit();
            }
        }

        function downloadArquivo(arqid)
        {
            window.location.href = "obras2.php?modulo=principal/pagamento&acao=A&arquivo=" + arqid;
        }

        function excluirNotaFiscal()
        {
                var arqid = $("#arquivo").val();
                var pagid = $("#pagid").val();
                $("#arquivo").val("");
                $('#td_arquivo').html("<input type=\"file\" name=\"arquivo\" id=\"arquivo\" />");
//                $.ajax({
//                type: "POST",
//                url: window.location,
//                data: "requisicaoAjax=excluirNotaFiscal&classe=Pagamento&pagid=" + pagid + "&arqid=" + arqid,
//                success: function(msg) {
//                    }
//                });
        }

        $(function() {
<?php if ($_SESSION['obras2']['pagamento']['alert']): ?>
                alert('<?php echo $_SESSION['obras2']['pagamento']['alert'] ?>');
    <?php unset($_SESSION['obras2']['pagamento']['alert']) ?>
<?php endif; ?>
        });
    </script>