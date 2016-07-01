<?php
if ( $_REQUEST['pesquisa'] == 'xls' ){
    include  APPRAIZ."obras2/modulos/relatorio/popupRelatorioOsObras.inc";
}
if($_POST['requisicaoAjax']){
    $_POST['requisicaoAjax']();
    die;
}

$arOrgid = verificaAcessoEmOrgid();
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
    $_SESSION['obras2']['orgid'] = '';
}

$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

switch ($_REQUEST['ajax']){
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');

        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('Relatório de Supervisão Obras', '&nbsp;');

extract( $_POST );
?>


<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js">
</script>
<script type="text/javascript">
    $(document).ready(function(){

        $('.pesquisar').click(function(){
            $('#formListaObra').submit();
        });

        $('.exportarxls').click(function(){
            $('#xls').val('1');
            $('#formListaObra').submit();
        });

        <?php
        if ( $abreBuscaAvancada ){
            echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";
        }
        ?>

    });

    function exibeBuscaAvancada(visivel){
        if ( visivel == true ){
            $('#tr_busca_avancada').show();
            $('#labelBuscaAvancada').hide();
            $('#abreBuscaAvancada').val('t');
        }else{
            $('#tr_busca_avancada').hide();
            $('#labelBuscaAvancada').show();
            $('#abreBuscaAvancada').val('f');
        }
    }

    function carregarMunicipio( estuf ){
        var td	= $('#td_municipio');
        if ( estuf != '' ){
            var url = location.href;
            $.ajax({
                url  		 : url,
                type 		 : 'post',
                data 		 : {ajax  : 'municipio',
                    estuf : estuf},
                dataType   : "html",
                async		 : false,
                beforeSend : function (){
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error 	 : function (){
                    divCarregado();
                },
                success	 : function ( data ){
                    td.html( data );
                    divCarregado();
                }
            });
        }else{
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                .attr('disabled', true);
        }
    }

    function gerarRelatorio(tipo) {
        formulario = document.getElementById('formListaObra');

        if(tipo != 'xls'){
            formulario.target = 'RelatorioOsObras';
            var janela = window.open( 'obras2.php?modulo=relatorio/popupRelatorioOsObras&acao=A', 'RelatorioOsObras', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
            janela.focus();

            // Tipo de relatorio
        } else{
            formulario.target = null;
        }
        document.getElementById('pesquisa').value= tipo ;
        formulario.submit();
    }
    $(function(){
       $('.pos_sim').change(function(){
           if($(this).val() == 'nao'){
               $('.f_supervisao').hide();
           }else{
               $('.f_supervisao').show();
           }
       });
    });
</script>
<form method="post" name="formListaObra" id="formListaObra" action="obras2.php?modulo=relatorio/popupRelatorioOsObras&acao=A">
    <input type="hidden" name="pesquisa" id="pesquisa" value="" />
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
            <td>
                <?=campo_texto('empbuscatexto','N','S','',70,100,'','', '', '', '', 'id="empbuscatexto"', '', $_REQUEST['empbuscatexto']);?>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <a href="javascript:exibeBuscaAvancada( true );" id="labelBuscaAvancada">[Busca avançada]</a>
            </td>
        </tr>
        <tr>
            <td id="tr_busca_avancada" colspan="2" style="display: none;">
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <th colspan="2">
                            Busca Avançada
                            <a style="float:right;" onclick="exibeBuscaAvancada( false );">[Fechar]</a>
                        </th>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
                        <td>
                            <?php
                            $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                            $db->monta_combo("doco", $sql, "S", "Todos", "", "", "", "200", "N", "doco");
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
                        <td>
                            <?php
                            $tipoObra = new TipoObra();
                            $db->monta_combo( "tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid" );
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
                        <td>
                            <?php
                            $classificacaoObra = new ClassificacaoObra();
                            $db->monta_combo( "cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid" );
                            ?>
                        </td>
                    </tr>
                    <?php
                    if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL ):
                        ?>
                        <tr>
                            <td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
                            <td>
                                <?php
                                $tipologiaObra = new TipologiaObra();
                                $db->monta_combo( "tpoid", $tipologiaObra->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todas", "", "", "", 200, "N", "tpoid" );
                                ?>
                            </td>
                        </tr>
                    <?php
                    endif;
                    ?>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Programa:</td>
                        <td>
                            <?php
                            $programa = new ProgramaFonte();
                            $db->monta_combo( "prfid", $programa->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todos", "", "", "", 200, "N", "prfid" );
                            ?>
                        </td>
                    </tr>
                    <?php if($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL): ?>
                        <tr>
                            <td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
                            <td>
                                <?php
                                $modalidade = new ModalidadeEnsino();
                                $db->monta_combo( "moeid", $modalidade->listaCombo(), "S", "Todos", "", "", "", 200, "N", "moeid" );
                                ?>
                            </td>
                        </tr>
                    <?php endif; ?>

                    <?php
                    $param = array("estuf"=>array( "obrigatorio"=>"N",                  "value"=>$_POST['estuf']),
                        "mescod"=>array("obrigatorio"=>"N", "disabled"=>"S", "value"=>$_POST['mescod']),
                        "miccod"=>array(                    "disabled"=>"S", "value"=>$_POST['miccod']),
                        "muncod"=>array(                    "disabled"=>"S", "value"=>$_POST['muncod']));
                    montaComboTerritorio("obr", $param);
                    ?>

                    <tr>
                        <td class="SubTituloDireita">Esfera:</td>
                        <td>
                            <?php
                            $sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),
                                Array('codigo'=>'E', 'descricao'=>'Estadual'),
                                Array('codigo'=>'F', 'descricao'=>'Federal'));
                            $db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
                        <td>
                            De:&nbsp;
                            <?php
                            echo campo_texto( 'empvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                            echo 'Até:&nbsp;';
                            echo campo_texto( 'empvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
                        <td>
                            <table>
                                <tr>
                                    <th>Mínimo</th>
                                    <th>Máximo</th>
                                </tr>
                                <tr>
                                    <?php
                                    for ($i = 0; $i <= 100; $i++) {
                                        $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                                    }
                                    $percentualinicial = $_POST['percentualinicial'];
                                    $percentualfinal   = $_POST['percentualfinal'];
                                    $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                                    print '<td>';
                                    $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                                    print '</td><td>';
                                    $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                                    print '</td>';
                                    ?>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Possui OS</td>
            <td>
                <input type="radio" class="pos_sim" id="pos_sim"   name="pos_obr" value="sim" />   Sim
                <input type="radio" class="pos_sim" id="pos_nao" checked='checked'  name="pos_obr" value="nao" />   Não
            </td>
        </tr>

        <tr class="f_supervisao" style="display: none">
            <td class="" colspan="2">
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tbody>
                    <tr>
                        <th colspan="2">
                            Filtros Supervisão
                        </th>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Número da OS:</td>
                        <td>
                            <?php
                            echo campo_texto( 'sosnum', 'N', 'S', '', 11, 30, '###########', '', 'right', '', 0, '');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Situação da OS:</td>
                        <td>
                            <?php
                            criaComboWorkflow(TPDID_OS,array("nome"=>"esdidsituaco","id"=>"esdidsituacao"));
                            ?>
                        </td>
                    </tr>
                    <?php
                    $param = array("estuf"=>array( "obrigatorio"=>"N",                  "value"=>$_POST['obraestuf']),
                        "mescod"=>array("obrigatorio"=>"N", "disabled"=>"S", "value"=>$_POST['obramescod']),
                        "miccod"=>array(                    "disabled"=>"S", "value"=>$_POST['obramiccod']),
                        "muncod"=>array(                    "disabled"=>"S", "value"=>$_POST['obramuncod']));
//                    montaComboTerritorio("obra", $param);
                    ?>
                    <tr>
                        <td class="SubTituloDireita">Empresa responsável</td>
                        <td>
                            <?php
                            $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
                            $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
                            echo $db->monta_combo("entid", $empresas, 'S','Selecione...','', '', '',200,'N','sgeid');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Situação da supervisão:</td>
                        <td>
                            <?php
                            criaComboWorkflow(WF_TPDID_LAUDO_SUPERVISAO_EMPRESA,array("nome"=>"esdidsupervisao","id"=>"esdidsupervisao","option" => array("nao_iniciado"=>"Não Iniciado")), true);
                            ?>
                        </td>
                    </tr>
                    </tbody>
                </table>
            </td>
        </tr>
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="Gerar Relatório"
                       value="Gerar Relatório"
                       onclick="javascript:gerarRelatorio('visual');" /> <input
                    type="button" name="Gerar Relatório" value="Visualizar XLS"
                    onclick="javascript:gerarRelatorio('xls');" />
            </td>
        </tr>
    </table>
</form>
