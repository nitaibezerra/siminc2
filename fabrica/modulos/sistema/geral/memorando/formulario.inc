<?php
header( 'content-type: text/html; charset=iso-8859-1;' );

include_once APPRAIZ . 'includes/cabecalho.inc';

print "<br/>";
$prestadorServico  = new PrestadorServico();
$fiscalRepositorio = new FiscalRepositorio();
$fiscal            = $fiscalRepositorio->recuperePorId( $_SESSION['usucpf'] );

$ordemServico           = new OrdemServico();
$_REQUEST['empresaContratada'] = $_REQUEST['empresaContratada'] ? $_REQUEST['empresaContratada'] : PrestadorServico::PRESTADORA_SERVICO_FABRICA;
$listaDeOrdensDeServico = $ordemServico->recupereTodasOsCandidatasParaMemorandoDaFABRICA( null, null, $_REQUEST['empresaContratada'], $_REQUEST['anomemorando']);
$formmemotpdpsid		= $_REQUEST['formmemotpdpsid'];

$textoPadrao = Memorando::TEXTO_PADRAO_MEMORANDO_SQUADRA;

monta_titulo( "Memorando", "" );
$memorando = new Memorando();

if ( $_GET['memo'] != null )
{
	$tmpMemoId = $_GET['memo'];

    $memorandoRepositorio = new MemorandoRepositorio();
    $memorando            = $memorandoRepositorio->recuperePorId( $tmpMemoId );

    if ( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_FABRICA ) {
        //die('aqui sim');
        $listaDeOrdensDeServico = $ordemServico->recupereTodasOsCandidatasParaMemorandoDaFABRICAMenosQueEstaoEmMemorando( $tmpMemoId, $formmemotpdpsid, $_REQUEST['empresaContratada'], $_REQUEST['anomemorando'] );
    }elseif ( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_POLITEC ) {
        //die('aqui sim');
        $_REQUEST['empresaContratada'] = PrestadorServico::PRESTADORA_SERVICO_POLITEC;
        $listaDeOrdensDeServico = $ordemServico->recupereTodasOsCandidatasParaMemorandoDaFABRICAMenosQueEstaoEmMemorando( $tmpMemoId, $formmemotpdpsid, $_REQUEST['empresaContratada'], $_REQUEST['anomemorando'] );

    }elseif ( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_SAA ) {
        //die('aqui sim');
        $_REQUEST['empresaContratada'] = PrestadorServico::PRESTADORA_SERVICO_SAA;
        $listaDeOrdensDeServico = $ordemServico->recupereTodasOsCandidatasParaMemorandoDaAUDITORAMenosQueEstaoEmMemorando( $tmpMemoId, null, $_REQUEST['empresaContratada'], $_REQUEST['anomemorando'] );

    }elseif ( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_MBA ) {
        //die('aqui sim');
        $_REQUEST['empresaContratada'] = PrestadorServico::PRESTADORA_SERVICO_MBA;
        $listaDeOrdensDeServico = $ordemServico->recupereTodasOsCandidatasParaMemorandoDaFABRICAMenosQueEstaoEmMemorando( $tmpMemoId, $formmemotpdpsid, $_REQUEST['empresaContratada'], $_REQUEST['anomemorando'] );

    } else {
        //die('aqui não');
        $_REQUEST['empresaContratada'] = PrestadorServico::PRESTADORA_SERVICO_AUDITORA;
        $listaDeOrdensDeServico = $ordemServico->recupereTodasOsCandidatasParaMemorandoDaAUDITORAMenosQueEstaoEmMemorando( $tmpMemoId, $formmemotpdpsid, $_REQUEST['empresaContratada'], $_REQUEST['anomemorando'] );
    }


    $textoPadrao            = $memorando->getTextoMemorando();
}

$subTotalAReceber       			= 0;
$subTotalComPorcentagemDeDisciplina = 0;

?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="./js/jquery.validate.min.js"></script>
<script type="text/javascript" src="./js/jquery-limit.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language="javascript" type="text/javascript" src="./js/tiny_mce/tiny_mce.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript" src="./js/memorando.js"></script>

<script type="text/javascript">
    //Editor de textos
    tinyMCE.init({
        mode : "exact",
        elements : "textoMemorando",
        theme : "advanced",
        plugins : "save,advhr,advimage,advlink,iespell,insertdatetime,preview,searchreplace,print,contextmenu,paste,directionality,fullscreen",
        theme_advanced_buttons1_add_before : "save,newdocument,separator",
        theme_advanced_buttons1_add : "fontselect,fontsizeselect",
        theme_advanced_buttons2_add : "separator,insertdate,inserttime,preview,zoom,separator,forecolor,backcolor",
        theme_advanced_buttons2_add_before: "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator",
        theme_advanced_buttons3_add_before : "tablecontrols,separator",
        theme_advanced_buttons3_add : "emotions,iespell,advhr,separator,print,separator,ltr,rtl,separator,fullscreen",
        theme_advanced_toolbar_location : "top",
        theme_advanced_toolbar_align : "left",
        theme_advanced_path_location : "bottom",
        plugin_insertdate_dateFormat : "%d-%m-%Y",
        plugin_insertdate_timeFormat : "%H:%M:%S",
        extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
        external_link_list_url : "example_link_list.js",
        external_image_list_url : "example_image_list.js",
        flash_external_list_url : "example_flash_list.js",
        file_browser_callback : "fileBrowserCallBack",
        language : "pt",
        entity_encoding : "raw",
        width : "90%",
        height : "500px"
    });


</script>
<style>
    .glosa-memorando{
        display: none;
    }
    .alerta-vermelho{
    	background-color: #DD4B39 !important;
    }
</style>


<form id="formularioMemorando" name="formularioMemorando" action="?modulo=sistema/geral/memorando/listar&acao=A" method="post">

    <table class="tabela listaAtualizacao" cellspacing="1" cellpadding="3" align="center" border="0">

		<tr>
			<td class="SubtituloDireita">Ano de Criação:</td>
			<td>
				<?php

                    //recupera anomemorando
                    if ( $_GET['memo'] ) {
                        $anomemorando = $db->pegaUm("select to_char(memodata, 'yyyy') as ano from fabrica.memorando where memoid = ".$_GET['memo']);
                    }
					// SQL que retorna o ano do memorando para a combo anomemorando
					$sql = "select distinct to_char(ss.dataabertura, 'yyyy') as codigo, to_char(ss.dataabertura, 'yyyy') as descricao from fabrica.solicitacaoservico ss order by to_char(ss.dataabertura, 'yyyy')";
					echo $db->monta_combo("anomemorando", $sql, 'S', 'Todos', '', '', '', '80', 'N', 'anomemorando', '', '', '', '');

				?>
				</td>
		</tr>

    	<tr>
            <td class="SubTituloDireita">Empresa Contratada</td>
            <td>
				<?php
				if ( $_GET['memo'] == null ) {
					?>
                    <select id="empresaContratada" class="CampoEstilo" name="empresaContratada" style="width: auto;">
                    <?php
                    foreach ( $prestadorServico->recupereSquadraEEficacia() as $ps ) {
                        ?>
                            <option <?php echo $ps->getId() == $memorando->getPrestadorServicoMemorando() ? "selected='selected'" : ""; ?>
                                value="<?php echo $ps->getId(); ?>"><?php echo $ps->getNome(); ?></option>
                        <?php } ?>
                    </select>
					<?php
                 } else {
                        ?>
                    <input type="hidden" name="empresaContratada" id="empresaContratada" value="<?php echo $memorando->getPrestadorServicoMemorando() ?>" />
                            <? echo campo_texto( 'prestadorServico', 'S', 'N', "Campo Obrigatório", 50, '', '', '', '', '', 0, 'id="prestadorServico"', '', $prestadorServico->recuperePorId( $memorando->getPrestadorServicoMemorando() )->getNome(), '', '' );
                            ?>
                <?php } ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo de Despesa</td>
            <td>
                <?php
                //var_dump( $memorando->getTipoDespesaId() ); exit;
                $sqlDespesa = "SELECT tpdpsid AS codigo, tpdpsdsc AS descricao FROM fabrica.tipodespesa where tpdpsstatus = 'A'";
                $db->monta_combo('formmemotpdpsid', $sqlDespesa, 'S', 'Todos', '', '', '', '', 'N', 'formmemotpdpsid', '', $memorando->getTipoDespesaId() );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">N&uacute;mero Memorando</td>
            <td>
                <? echo campo_texto( 'numeroMemorando', 'S', 'S', "Campo Obrigatório", 11, '', '[#]', '', '', '', 0, 'id="numeroMemorando"', '', $memorando->getNumeroMemorando(), '', '' );
                ?>
                <span id="mensagem-erro-numero-memorando" class="mensagem-erro">Memorando j&aacute; existente.</span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data do memorando</td>
            <td>
				<?php
				$data = $memorando->getDataMemorando() == null ? "" : $memorando->getDataMemorando()->format( "d-m-Y" );
				echo campo_data2( 'dataMemorando', 'S', 'S', 'Campo Obrigatório', 'S', '', '', $data );
				?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Texto do Memorando</td>
            <td>
                <?php
                $numeroDeLinhas   = 200;
                $numeroDeColunas  = 10;
                $maximoCaracteres = 2000;
                ?>
                <span id="textAreaDinamico">
                <?php echo campo_textarea( 'textoMemorando', 'S', $habil, null, $numeroDeLinhas, $numeroDeColunas, $maximoCaracteres, null, null, null, null, null, $textoPadrao ); ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Respons&aacute;vel pelo Memorando</td>
            <td>
                    <? echo campo_texto( 'servidorPublicoResponsavel', 'S', 'N', "Campo Obrigatório", 50, '', '', '', '', '', 0, 'id="servidorPublicoResponsavel"', '', $fiscal->getNome(), '', '' );
                    ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Valor de ajuste </td>
            <td>
                <?php
                $tmpValorAjuste = ( $memorando->getValorAjuste() ? number_format( $memorando->getValorAjuste(), 2, ",", ".") : "" );
                echo 'R$: ', campo_texto( 'memovlrajuste', 'N', 'S', "Valor de ajuste", 10, '', '##.###.###,##', '', '', '', 0, 'id="memovlrajuste"', '', $tmpValorAjuste, '', '' );
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Descrição de ajuste do Memorando</td>
            <td>
                <?php
                echo campo_textarea( 'memodscajuste', 'N', 'S', '', 150, 5, false, '', 0, 'Descrição de ajuste do Memorando', false, NULL, $memorando->getDescricaoAjuste() );
                ?>
                <p>caracteres a serem digitados: <span id="left"></span></p>
                <span id="mensagem-erro-descricao-ajuste" class="mensagem-erro">Campo obrigatório.</span>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Tipo de Glosa</td>
            <td>
                <?php
                $sql = "SELECT  tpglmemoid as codigo, 'Grau ' || tpglmemograuvalor as descricao, tpglmemopercvalor
                        FROM fabrica.tipoglosamemorando tgm
                        WHERE tgm.tpglmemostatus = 'A';";

                $arTipoGLosaMemorando = $db->carregar( $sql );
                $db->monta_combo( "glosaMemorando", $sql, 'S', 'Selecione', '', '', '', '', 'N', 'glosaMemorando', false, $memorando->getGlosaMemorando() );
                ?>
            </td>
        </tr>
        <tr class="glosa-memorando"  >
            <td class="SubTituloDireita">Glosa Memorando</td>
            <td>
                <?
                echo campo_texto( 'tpglmemopercvalor', 'N', 'N', "Porcentagem utilizada na glosa", 5, '', '[#]', '', '', '', 0, 'id="tpglmemopercvalor"', '', '0,00', '', '' );
                ?> %
            </td>
        </tr>
        <tr class="glosa-memorando">
            <td class="SubTituloDireita">Justificativa da glosa</td>
            <td>
                <? echo campo_textarea( 'justificativaGlosaMemorando', 'S', 'S', '', 150, 5, false, '', 0, 'Justificativa da glosa', false, NULL, $memorando->getJustificativaGlosaMemorando() ); ?>
            </td>
        </tr>
        <tr>
            <td></td>
            <td>
                <input type="hidden" name="memo" id="memo" value="<?php echo $memorando->getId(); ?>" />
                <input type="hidden" name="tipoForm" value="<?php echo $memorando->getId() == null ? "novo" : "edita"; ?>" />
                <input type="hidden" name="cpfServidorPublico" value="<?php echo $_SESSION['usucpf'] ?>" />
                <input id="salvarMemorando" type="submit" value="Salvar"/><?php //Regra de salvar em fabrica.js  ?>
                <input type="button" value="Voltar" onclick="parent.location='?modulo=sistema/geral/memorando/listar&acao=A'"/>
                <input type="button" id="pesquisarMemorando" value="Pesquisar" />
<?php if ( $memorando->getId() != null ) {  ?>
                    <input id="emitirMemorando" type="button" value="Emitir Memorando"/>
<?php } ?>
                <span id="mensagem-erro" class="mensagem-erro">Selecione pelo menos uma ordem de servi&ccedil;o</span>
            </td>
        </tr>
    </table>

    <div id="tabelaDinamicaMemorando">

<?php
if ( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_FABRICA || $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_POLITEC || $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_MBA || $memorando->getPrestadorServicoMemorando() == null ) {
?>
            <table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
                <thead>
                    <tr>
                        <th>
                        	<!--
                            <input type="checkbox" name="checkall" id="checkall" value="" />
                             -->
                        </th>
                        <th>Data de Abertura da SS</th>
                        <th>Solicita&ccedil;&atilde;o de Servi&ccedil;o</th>
                        <th>Ordem de Servi&ccedil;o</th>
                        <th>Quantidade de Pontos de Fun&ccedil;&atilde;o</th>
                        <th>Porcentagens das disciplinas</th>
                        <th>PF a pagar com % de esforço</th>
                        <th>Glosa(PF)</th>
                        <th>PF a pagar após cálculo da glosa</th>
                        <th>Valor de Ponto de Fun&ccedil;&atilde;o Unit&aacute;rio</th>
                        <th width="8%">Valor Total</th>
                    </tr>
                </thead>
                <tbody>

    <?php

    if ( !empty($listaDeOrdensDeServico)  ) {

        $count = 1;
        $contaLinha = 0;

        $class = '';
        foreach ( $listaDeOrdensDeServico as $os ) {
            ++$contaLinha;
            $class = $count % 2 ? 'even' : 'odd';
            ?>
                            <tr class="<?php echo $class ?>" >
                            <?php
                            //Calculando a glosa;

                            $valorGlosa             = 0;
                            $pfComEsforco           = 0;
                            $pfComEsforcoGlosa      = 0;
                            //echo('<pre>');var_dump($os);exit;
                            if ( $os->possuiGlosa() ) {
                                $glosa              = new Glosa();
                                $glosa              = $glosa->recupereGlosaPeloId( $os->getIdGlosa() );
                                $valorGlosa         = $glosa->getValorEmPf();
                                $valorTotalAReceber = $os->getValorAReceberGlosado();
                            } else {
                                $valorGlosa         = 0;
                                $valorTotalAReceber = $os->getValorAReceberDaOs();
                            }

                            $valorApagarComPorcentagemDeDisciplina = $os->getValorAReceberDaOs();

                            $menorValorPF       = $os->getMenorValorPF( );
                            //die( $valorTotalAReceber);
                            $pfComEsforco       = ($os->getPorcentagemDisciplina() * $menorValorPF) / 100;
                            $pfComEsforcoGlosa  = $pfComEsforco - $valorGlosa;


                            if ( $_GET['memo'] != null && $memorando->getListaDeOrdensDeServico() != null ) {
                                foreach ( $memorando->getListaDeOrdensDeServico() as $osMemorando ) {
                                    if ( $osMemorando->equals( $os ) ) {
                                        $checked = 'checked="checked"';

                                        $subTotalQtdePontoFuncao 			= $subTotalQtdePontoFuncao + $menorValorPF;
                                        $subTotalQtdeGlosa       			= $subTotalQtdeGlosa + $valorGlosa;
                                        $subTotalAReceber        			= $subTotalAReceber + $valorTotalAReceber;
                                        $subTotalComPorcentagemDeDisciplina = $subTotalComPorcentagemDeDisciplina + $valorApagarComPorcentagemDeDisciplina;
                                        $subTotalpfComEsforcoGlosa          = $subTotalpfComEsforcoGlosa + $pfComEsforcoGlosa;

                                        break;
                                    } else {
                                        $checked = '';
                                    }
                                }
                                ?>
                                    <td>
                                        <input type="checkbox" tagpersonalida="<?php echo( $os->temTermo( $os->getIdSolicitacaoServico() ) );?>" class="selecionadas" name="osSelecionadas[]" <?php echo $checked; ?> value="<?php echo $os->getId(); ?>" />
                                    </td>
                                    <?php
                                } else {
                                    $subTotalQtdePontoFuncao 			= $subTotalQtdePontoFuncao + $menorValorPF;
                                    $subTotalQtdeGlosa       			= $subTotalQtdeGlosa + $valorGlosa;
                                    $subTotalAReceber        			= $subTotalAReceber + $valorTotalAReceber;
                                    $subTotalComPorcentagemDeDisciplina = $subTotalComPorcentagemDeDisciplina + $valorApagarComPorcentagemDeDisciplina;
                                    $subTotalpfComEsforcoGlosa          = $subTotalpfComEsforcoGlosa + $pfComEsforcoGlosa;
                                    ?>
                                    <td>
                                        <input type="checkbox" class="selecionadas" tagpersonalida="<?php echo( $os->temTermo( $os->getIdSolicitacaoServico() ) );?>" name="osSelecionadas[]" value="<?php echo $os->getId(); ?>" />
                                    </td>
                                    <?php
                                }
                                ?>

                                <td class="alignCenter"><?php echo $os->getDataAbertura(); ?></td>
                                <td class="alignCenter"><?php echo $os->getIdSolicitacaoServico(); ?></td>
                                <td class="alignCenter"><?php echo $os->getId(); ?></td>
                                <td id="<?php echo $os->getId(); ?>memorandoQtdeValorPF" class="alignRight"><?php echo number_format( $menorValorPF, 2, ",", "." ); ?></td>
                                <td class="alignRight"><?php echo number_format( $os->getPorcentagemDisciplina(), 2, ",", "." ) . "%"; ?></td>
                                <td class="alignRight"><?php echo number_format( $pfComEsforco, 2, ",", ".") ?></td>
                                <td id="<?php echo $os->getId(); ?>memorandoQtdePFGlosa" class="alignCenter"><?php echo number_format( $valorGlosa, 2, ",", "." ); ?></td>
                                <td id="<?php echo $os->getId(); ?>memorandoQtdePFGlosaApos" class="alignRight"><?php echo number_format($pfComEsforcoGlosa, 2, ",", "."); ?></td>
                                <td class="alignRight"><?php echo "R$ " . number_format( $os->getValorUnitarioDePf(), 2, ",", "." ); ?></td>
                                <!--<td id="<?php echo $os->getId(); ?>memorandoTotalComPorcentagemDeDisciplina" class="alignRight"><?php echo "R$ " . number_format( $valorApagarComPorcentagemDeDisciplina, 2, ",", "." ); ?></td>-->
                                <td id="<?php echo $os->getId(); ?>memorandoTotalAReceber" class="alignRight"><?php echo "R$ " . number_format( $valorTotalAReceber, 2, ",", "." ); ?></td>
                            </tr>
                                <?php
                                $count            = $count + 1;
                            }
                            ?>
                    </tbody>
                    <tfoot>
                        <?php

                        $descricaoAjuste = $memorando->getDescricaoAjuste();
                        $valorAjuste     = $memorando->getValorAjuste();
                        if( !empty($descricaoAjuste) ){
                        $subTotalAReceber -= $valorAjuste;
                        ?>

                        <tr>
                            <td  class="left">
                            </td>
                            <td colspan="5" class="alignCenter">
                                <?php echo $descricaoAjuste; ?>
                            </td>
                            <td>
                            </td>
                            <td></td>
                            <td></td>
                            <td style="color: red" class="alignRight"> R$ <?php echo number_format( $valorAjuste, 2, ",", "." ); ?> </td>
                        </tr>
                        <?php
                        }
                        ?>

                        <tr class="glosa-memorando">
                            <td  class="alignRight" colspan="2">
                            <?php
                            $porcentagemGlosa = '';
                            $idGlosaMemorando = $memorando->getGlosaMemorando();
                            if ( !empty( $idGlosaMemorando ) ) {
                                $sql = "SELECT tpglmemograuvalor, tpglmemopercvalor
                                                FROM fabrica.tipoglosamemorando tgm
                                                WHERE tgm.tpglmemostatus = 'A'
                                                AND tgm.tpglmemoid = $idGlosaMemorando";

                                $grauGlosa = $db->pegaLinha( $sql );

                                echo "Grau {$grauGlosa['tpglmemograuvalor']}";
                                $porcentagemGlosa = "{$grauGlosa['tpglmemopercvalor']}%";
                            }
                            ?>
                            </td>
                            <td colspan="4" class="alignCenter"> <?php echo $memorando->getJustificativaGlosaMemorando(); ?> </td>
                            <td>
                                <?php echo $porcentagemGlosa ?>
                            </td>
                            <td></td>
                            <td></td>
                            <td style="color: red" class="alignRight"> R$ <span id="valor_glosa_memorando"></span> </td>
                        </tr>

                        <tr>
                            <td class="alignRight" colspan='4'>Total:</td>
                            <td id="memorandoSubTotalPF" class="alignRight"><?php echo number_format( $subTotalQtdePontoFuncao, 2, ",", "." ); ?></td>
                            <td></td>
                            <td></td>
                            <td id="memorandoSubTotalGlosa" class="alignCenter"><?php echo number_format( $subTotalQtdeGlosa, 2, ",", "." ); ?></td>

                            <!--
                            <td id="memorandoSubTotalComPorcentagemDeDisciplina" class="alignRight">
                            Não sei para que serve, a Patricia, pediu para tirar.
                                R$ <span id="valorTotalComPorcentagemDeDisciplina"> <?php // echo number_format( $subTotalComPorcentagemDeDisciplina, 2, ",", "." ) ?> </span>

                            </td>
                            -->

                            <td id="memorandoSubTotalGlosaApos" class="alignRight">
                                <?php echo number_format( $subTotalpfComEsforcoGlosa, 2, ",", "." ); ?>
                            </td>
                            <td></td>
                            <td id="memorandoSubtotalAReceber" class="alignRight">
                                R$ <span id="valor_total_memorando"> <?php echo number_format( $subTotalAReceber, 2, ",", "." ) ?> </span>
                            </td>
                        </tr>

                        <tr>
                            <td colspan='11'>Total de Registros: <?php echo $contaLinha; ?></td>
                        </tr>
                    </tfoot>
                                <?php
                            } else {
                                ?>
                    <tbody>
                        <tr>
                            <td class="alignCenter" colspan="10">N&atilde;o foram encontrados registros</td>
                        </tr>
                    </tbody>
    <?php } ?>
            </table>

    <?php
} else {
    ?>

            <table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
                <thead>
                    <tr>
                        <th>
                            <input type="checkbox" name="checkall" id="checkall" value="" />
                        </th>
                        <th>Data de Abertura da SS</th>
                        <th>Solicita&ccedil;&atilde;o de Servi&ccedil;o</th>
                        <th>Ordem de Servi&ccedil;o</th>
                        <th>Quantidade de Pontos de Fun&ccedil;&atilde;o</th>
                        <th>Glosa (PF)</th>
                        <th>Valor de Ponto de Fun&ccedil;&atilde;o Unit&aacute;rio</th>
                        <th>PF a pagar após cálculo da glosa</th>
                        <th width="8%">Valor Total</th>
                    </tr>
                </thead>
                <tbody>
                    <?php

                    if ( $listaDeOrdensDeServico != null ) {
                        $count              = 0;
                        $subTotalAReceber   = 0;
                        $valorTotalAReceber = 0;
                        $menorValorPF       = 0;
                        $valorUnitarioPF    = 0;
                        $pfGlosa            = 0;
                        $subTotalQtdePontoFuncao    = 0;
                        $contaLinha         = 0;
                        $subTotalpfComEsforcoGlosa = 0;

                        foreach ( $listaDeOrdensDeServico as $os ) {
                            ++$count;
                            ++$contaLinha;
                            $rowClass   = ++$count % 2 ? 'even' : 'odd';

                            $valorGlosa         = 0;
                            $qtdPFGlosa         = 0;
                            $menorValorPF       = $os->getMenorValorPFEmpresaItem2( $os->getId() );
                            $valorUnitarioPF    = $os->getValorUnitarioDePf();
                            $valorTotalAReceber = $menorValorPF * $valorUnitarioPF;


                            if ( $os->possuiGlosa() ) {
                                $glosa      = new Glosa();
                                $glosa      = $glosa->recupereGlosaPeloId( $os->getIdGlosa() );
                                $qtdPFGlosa = $glosa->getValorEmPf();
                                $valorGlosa = $qtdPFGlosa * $valorUnitarioPF;
                            }

                            $valorTotalAReceber -= $valorGlosa;

                            if ( $_GET['memo'] != null && $memorando->getListaDeOrdensDeServico() != null ) {

                                foreach ( $memorando->getListaDeOrdensDeServico() as $osMemorando ) {
                                    if ( $osMemorando->equals( $os ) ) {
                                        $checked                 = 'checked="checked"';
                                        $subTotalQtdePontoFuncao = $subTotalQtdePontoFuncao + $menorValorPF;
                                        $subTotalAReceber += $valorTotalAReceber;
                                        $subTotalpfComEsforcoGlosa = $subTotalpfComEsforcoGlosa + $pfComEsforcoGlosa;
                                        break;
                                    } else {
                                        $checked = '';
                                    }
                                }
                            } else {
                                $subTotalQtdePontoFuncao = $subTotalQtdePontoFuncao + $menorValorPF;
                                $subTotalpfComEsforcoGlosa = $subTotalpfComEsforcoGlosa + $pfComEsforcoGlosa;
                                $checked = 'checked="checked"';
                            }

                            $pfGlosa = $menorValorPF - $qtdPFGlosa;
                            ?>
                            <tr class="<?php echo $rowClass; ?>">
                                <td>
                                    <input type="checkbox" class="selecionadas" name="osSelecionadas[]" tagpersonalida="<?php echo( $os->temTermo( $os->getIdSolicitacaoServico() ) );?>" <?php echo $checked; ?> value="<?php echo $os->getId(); ?>"/>
                                </td>
                                <td class="alignCenter">
                                    <?php echo $os->getDataAbertura(); ?>
                                </td>
                                <td class="alignCenter">
                                    <?php echo $os->getIdSolicitacaoServico(); ?>
                                </td>
                                <td class="alignCenter">
                                    <?php echo $os->getId(); ?>
                                </td>
                                <td id="<?php echo $os->getId(); ?>memorandoQtdeValorPF" class="alignRight">
                                    <?php echo number_format( $menorValorPF, 2, ",", "." ); ?>
                                </td>
                                <td  class="alignCenter">
                                    <?php echo number_format( $qtdPFGlosa, 2, ",", "." ); ?>
                                </td>
                                <td class="alignCenter">
                                    <?php echo number_format( $pfGlosa, 2, ",", "." ); ?>
                                </td>
                                <td class="alignCenter">
                                    <?php echo "R$ " . number_format( $os->getValorUnitarioDePf(), 2, ",", "." ); ?>
                                </td>
                                <td id="<?php echo $os->getId(); ?>memorandoTotalAReceber" class="alignRight">
                                    <?php echo "R$ " . number_format( $valorTotalAReceber, 2, ",", "." ); ?>
                                </td>
                            </tr>
                            <?php
                        }
                        ?>
                </tbody>
                <tfoot>
                    <?php

                    $descricaoAjuste = $memorando->getDescricaoAjuste();
                    $valorAjuste     = $memorando->getValorAjuste();
                    if( !empty($descricaoAjuste) )
                    {
                        $subTotalAReceber -= $valorAjuste;
                    ?>
                        <tr>
                            <td  class="left">
                            </td>
                            <td colspan="4" class="alignCenter">
                                <?php echo $descricaoAjuste; ?>
                            </td>
                            <td></td>
                            <td></td>
                            <td style="color: red" class="alignRight"> R$ <?php echo number_format( $valorAjuste, 2, ",", "." ); ?> </td>
                        </tr>
                        <?php
                    }
                    ?>

                    <tr class="glosa-memorando">
                        <td  class="alignRight">
                            <?php
                            $idGlosaMemorando        = $memorando->getGlosaMemorando();
                            if ( !empty( $idGlosaMemorando ) ) {
                                $sql = "SELECT tpglmemograuvalor, tpglmemopercvalor
                                            FROM fabrica.tipoglosamemorando tgm
                                            WHERE tgm.tpglmemostatus = 'A'
                                            AND tgm.tpglmemoid = $idGlosaMemorando";

                                $grauGlosa = $db->pegaLinha( $sql );

                                echo "Grau {$grauGlosa['tpglmemograuvalor']}";
                            }
                            ?>
                        </td>
                        <td colspan="5" class="alignCenter"> <?php echo $memorando->getJustificativaGlosaMemorando(); ?> </td>
                        <td>
                            <?php echo "{$grauGlosa['tpglmemopercvalor']}%" ?>
                        </td>
                        <td style="color: red" class="alignRight"> R$ <span id="valor_glosa_memorando"></span> </td>
                    </tr>

                    <tr>
                        <td class="alignRight" colspan="4">Total:</td>
                        <td id="memorandoSubTotalPF" class="alignRight"><?php echo number_format( $subTotalQtdePontoFuncao, 2, ",", "." ); ?></td>
                        <td></td>
                        <td id="memorandoSubTotalGlosaApos" class="alignRight">
                            <?php echo number_format( $subTotalpfComEsforcoGlosa, 2, ",", "." ); ?>
                        </td>
                        <td></td>
                        <td id="memorandoSubtotalAReceber" class="alignRight">
                            R$ <span id="valor_total_memorando"> <?php echo number_format( $subTotalAReceber, 2, ",", "." ) ?> </span>
                        </td>
                    </tr>

                    <tr>
                        <td colspan='8'>Total de Registros: <?php echo $contaLinha; ?></td>
                    </tr>

                </tfoot>
                <?php
                } else {
                ?>
                    <tbody>
                        <tr>
                            <td class="alignCenter" colspan="8">N&atilde;o foram encontrados registros</td>
                        </tr>
                    </tbody>
                            <?php } ?>
            </table>

                        <?php } ?>
    </div>
</form>

<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>
                        <?php
                        if ( empty( $subTotalAReceber ) ) {
                            $subTotalAReceber = 0;
                        }
                        ?>
<script type="text/javascript" >
    var grausGlosaMemorando = <?php echo simec_json_encode( $arTipoGLosaMemorando ); ?>;

    $(document).ready(function(){


       //$('textarea#memodscajuste').limit('500','#left');
       //$('textarea#memodscajuste').limit('500');

        var totalOS             = $('.selecionadas').length,
            totalSelecionadas   = $('.selecionadas:checked').length;


        if( totalOS == totalSelecionadas && totalOS != 0 )
        {
            //$('#checkall').attr('checked', true);
        }

        $('#checkall').click(function(){
            var checked = $('#checkall').is(':checked');
            $('.selecionadas[tagpersonalida=1]').attr('checked', checked);
        });


        Memorando.valorTotal 		 = <?php echo $subTotalAReceber; ?>;
        Memorando.valorTotalPercDisc = <?php echo $subTotalComPorcentagemDeDisciplina; ?>;

        $('#justificativaGlosaMemorando').attr('title', 'Campo obrigatório');


        $("#formularioMemorando").validate({
            rules : {
                prestadorServico: "required",
                numeroMemorando:  "required",
                dataMemorando:    "required",
                textoMemorando:   "required",
                servidorPublicoResponsavel: "required"
            }
        });

        $('#glosaMemorando').change(function(){

            //console.log('chama glosaMemorando');

            $('#justificativaGlosaMemorando').addClass('required');

            $('tr[class="glosa-memorando"]').show();

            var grauGlosaSelecionado = $('#glosaMemorando').val();
            var valorGlosado         = 0;
            var valorTot             = 0;
            var valorTotalPercDisc	 = 0;

            if(grauGlosaSelecionado == '' )
            {
                $('tr[class="glosa-memorando"]').hide();
                $('#justificativaGlosaMemorando').val('');
                $('#justificativaGlosaMemorando').removeClass('required');
            }

            $.each(grausGlosaMemorando, function( idx, objGlosaMemo ){
                if( objGlosaMemo.codigo == grauGlosaSelecionado )
                {
                    valorGlosado = Memorando.calcularGlosaMemorando( objGlosaMemo.tpglmemopercvalor );
                    $('#tpglmemopercvalor').val( objGlosaMemo.tpglmemopercvalor );
                }
            });

            valorTot 			= Memorando.valorTotal - valorGlosado;
            valorTotalPercDisc  = Memorando.valorTotalPercDisc;

            $('#valor_glosa_memorando').html(  mascaraglobal('[#].###,##',valorGlosado.toFixed(2) ) );
            $('#valor_total_memorando').html( mascaraglobal('[#].###,##',valorTot.toFixed(2) ) );
            $('#valor_total_memorando').html( mascaraglobal('[#].###,##',valorTot.toFixed(2) ) );
        });


        $('#glosaMemorando').trigger('change');

    });
</script>