<?php
if ($_REQUEST['act'] == 'download'){
	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	$file = new FilesSimec(null, null, "portos");
	$file->setPasta($_SESSION['sisarquivo']);
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die();
}

$_SESSION['obras2']['empid'] = ($_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid']);
$empid = 					   $_SESSION['obras2']['empid'];

if ( empty($empid) ){
	die("<script>
			alert('Faltam parametros para acessar a tela!');
			location.href = '?modulo=principal/listaEmpreendimentos&acao=A';
		 </script>");	
}

$empreendimento = new Empreendimento();
$dadosResumoEmp = $empreendimento->dadosResumo( $empid );



//$atiid   = $_REQUEST['atiid'];
//$where   = array();
//$where[] = "ati.atiid = {$atiid}";
//$where   = implode(' AND ', $where);
//
//$ati = new Atividade();
//$carimbo = $ati->getSituacaoAtividade($atiid);
//
//$sql = "SELECT
//                *,
//                TO_CHAR( sup.supvdt, 'DD/MM/YYYY') as supvdt,
//                ati.arqid AS atiarqid,
//                ( SELECT
//                        SUM(supvlritemsobreobraexecanterior) AS total
//                    FROM
//                        portos.estruturaobra e
//                    LEFT JOIN
//                        portos.supervisaoitenscomposicao sic ON sic.eobid = e.eobid
//                                                                 AND sic.supvid = sup.supvid
//                                                                 AND sic.eobid IS NOT NULL
//                                                                 AND sic.iceid IS NULL
//                                                                 AND sic.ditid IS NULL
//                    WHERE
//                        e.eobstatus = 'A' AND
//                        e.atiid = ati.atiid
//                ) as percentualexecucao,
//                ati.*,
//                TO_CHAR( ati.atidatainicio, 'DD/MM/YYYY') as atidatainicio2,
//                TO_CHAR( ati.atidataconclusao, 'DD/MM/YYYY') as atidataconclusao2
//        FROM
//            pde.atividade ati
//        LEFT JOIN
//            portos.executor exe ON ati.exeid = exe.exeid
//        LEFT JOIN
//            portos.tipoprojeto tpp ON ati.tppid = tpp.tppid
//        LEFT JOIN
//            portos.situacaoatividade sta ON ati.staid = sta.staid
//        LEFT JOIN
//            portos.supervisao sup ON ati.atiid = sup.atiid  AND sup.supvid = (SELECT
//                                                                                    supvid
//                                                                                FROM
//                                                                                    portos.supervisao
//                                                                                WHERE
//                                                                                    supstatus = 'A'
//                                                                                AND
//                                                                                    atiid = sup.atiid
//                                                                                ORDER BY
//                                                                                    supvdt DESC, supvid DESC
//                                                                                LIMIT
//                                                                                    1)
//        WHERE
//            $where";
////dbg($sql,d);
//$dados = $db->pegaLinha($sql);
//
//$sql = "SELECT
//            pornome
//        FROM
//            portos.portoatividade pa
//        JOIN portos.porto p ON p.porid = pa.porid
//        WHERE
//            atiid = {$dados['atiid']}";
//$pornome = $db->carregarColuna($sql);
//$dados['pornome'] = implode(', ', $pornome);
?>


<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
<!--    <link rel="stylesheet"  href="/includes/jquery-mobile/jquery.mobile-1.0.1.min.css" />-->
<!--    <link rel="stylesheet"  href="/includes/jquery-mobile/jqm-docs.css" />-->
<!--    <script src="/includes/jquery-mobile/jquery.js"></script>-->
<!--    <script src="/includes/jquery-mobile/jquery.mobile-1.0.1.min.js"></script>-->

		<script language="JavaScript" src="../includes/funcoes.js"></script>

        <link type="text/css" href="/includes/jquery-ui/jquery-ui-1.8.22.custom.css" rel="stylesheet" />
<!--        <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.js"></script>-->

        <script type="text/javascript" src="/includes/jquery-nicescroll/jquery.min.js"></script>
        <script type="text/javascript" src="/includes/jquery-nicescroll/jquery.nicescroll.min.js"></script>

        <script type="text/javascript" src="/includes/jquery-ui/jquery-ui-1.8.20.custom.min.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.core.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.widget.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.mouse.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.button.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.resizable.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.position.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.dialog.js"></script>
        <script type="text/javascript" src="/includes/jquery-ui/ui/jquery.ui.draggable.js"></script>
        <script type="text/javascript" src="/includes/jquery-slimscroll/slimScroll.js"></script>
        <script type="text/javascript" src="/includes/jquery-jshowoff/jquery.jshowoff.js"></script>
        <script>
    $(function() {

        $("#div-rolagem").niceScroll({cursorborder:"",cursorcolor:"#fff",boxzoom:false });

        // a workaround for a flaw in the demo system (http://dev.jqueryui.com/ticket/4375), ignore!
        $( "#dialog:ui-dialog" ).dialog( "destroy" );
    	$( "#dialog-message" ).dialog({
            modal: true,
            resizable: false,
            height: 600,
            width: 600,
            draggable: false
        }).dialog( "close" );
    });

    function abrirFotos(){
    	$( "#dialog-message" ).dialog( "open" );
    }
    
	function abreEvolucaoFinan( obrid ){
		janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid='+obrid ,800,650);
	}
    
    </script>





    <script type="text/javascript" src="/includes/estouvivo.js"></script>
    <style type="text/css">

		#pagina_1{
			min-height:100%;
	    	position:relative;
		}

		body {
		    -webkit-background-size: cover;
		    -moz-background-size: cover;
		    -o-background-size: cover;
		    background-size: cover;
		    margin: 0;

		}

        .content-primary{
            float:left;
            margin-left: 20px;
            width: 60%;
            margin-left: 390px;
            position: absolute;
            margin-top: 10px;
        }

        .content-secondary{
            margin-top: 10px;
            width: 340px;
            border:2px solid #CCCCCC;
            margin-left: 10px;
            padding-right: 0%;
            position: absolute;
            padding: 0px;
            height: 565px;

        }

        .contentInfo{
            padding:2px;
            margin:5px 10px;
            width:320px;
            height:200px;
            float:left
        }

        body, html {
            background-color: #EDEDED;
            padding: 0px;
            margin: 0px;
            font-family: Helvetica,Arial,sans-serif;
            height: 100%;
        }

        [data-role=header]{
            display: block;
            font-size: 16px;
            min-height: 1.1em;
            outline: 0 none !important;
            overflow: hidden;
            padding: 0;
            text-align: center;
            text-overflow: ellipsis;
            white-space: nowrap;
            border: 1px solid #456F9A;
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #254F7A;
            width: 99.8%;
            background: #4F807C; /* for non-css3 browsers */
        }
        [data-role=header] h1{
            font-size: 16px;
            margin-top: 10px;
        }

        [data-role=page]{
        }

        [data-role=content]{
            height: auto;
            min-height: 100%;
            /* height: 550px; */
            padding-bottom: 40px;
        }

        [data-icon=home]{
            display: inline-block;
            font-size: 13px;
            margin: 0;
            position: absolute;
            left: 10px;
            top: 12px;
            height: 24px;
            width: 24px;
            cursor: pointer;
            text-align: center;
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #145072;
        }

        [data-role=footer]{
            border-left-width: 0;
            border-right-width: 0;
            overflow: hidden;
            width: 99.8%;
            display: block;
            font-family: Helvetica,Arial,sans-serif;
            border: 1px solid #456F9A;
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #254F7A;
            position: relative;
            height: 40px;
            clear:both;
            background: #4F807C; /* for non-css3 browsers */

            position:absolute;
		    bottom:0;
            width: 99.8%;
        }


        [data-role=button], .ui-btn-up-b{
            border: 1px solid #456F9A;
            background: #4F807C; /* for non-css3 browsers */
            text-align: center;
            padding-bottom: 0px;
            width: 99.8%;
            height: 35px;
            padding-top: 10px;
            cursor: pointer;
        }

        [data-role=button]:hover{
            background: #4F807C; /* for non-css3 browsers */
        }

        [data-role=button] a{
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #254F7A;
        }

        .ui-btn-up-b{
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #145072;
            padding-top: 0px;
            width: 100px;
            cursor: auto;
        }

        #div_imagem a, #div_imagem  img{
            text-decoration: none;
            border: 0px;
        }

		.linkInicio{
			background-image:url(/imagens/inicio_mobile.png);
			background-repeat:repeat-x;
			height: 26px;
			width: 26px;
		}

		.linkInicio:hover{
			background-image:url(/imagens/inicio_hover_mobile.png);
			background-repeat:repeat-x;
			height: 27px;
			width: 27px;
		}

		.linkVoltar{
			background-image:url(/imagens/voltar_mobile.png);
			background-repeat:repeat-x;
			height: 26px;
			width: 26px;
		}

		.linkVoltar:hover{
			background-image:url(/imagens/voltar_hover_mobile.png);
			background-repeat:repeat-x;
			height: 26px;
			width: 26px;
		}

        #rodape {
        	margin-top: 555px;
            height: 30px;
            width: 100%;
            text-align: center;
            padding-top: 5px;
            background: -moz-linear-gradient(#81A8CE, #5E87B0) repeat scroll 0 0 #5E87B0;
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#81A8CE', endColorstr='#5E87B0');
            background: -webkit-gradient(linear, left top, left bottom, from(#81A8CE), to(#5E87B0));
        }

		a:link {
		    text-decoration: none;
		}

		#rodape a{
    		cursor: pointer;
		    left: 10px;
		    position: absolute;

		}
    </style>
</head>
<body>
<!-- page -->
<div data-role="page" id="pagina_1">
    <!-- header -->
    <div data-role="header" data-theme="b">
			<h1><a href="javascript: document.location = '?modulo=principal/cadEmpreendimento&acao=A&empid=<?php echo $dadosResumoEmp['empid'] ?>'" style="color: #FFFFFF"><?= "{$dadosResumoEmp['empdsc']} - {$dadosResumoEmp['mundescricao']} / {$dadosResumoEmp['estuf']}" ?></a></h1>
<!--        <a href="projetos.php?modulo=relatorio/mobile&acao=A" data-icon="home" data-iconpos="notext" data-direction="reverse" class="ui-btn-right jqm-home">Início</a>-->
        <a href="?modulo=principal/listaEmpreendimentos&acao=A" class="linkInicio" data-icon="home" data-iconpos="notext" data-direction="reverse" class="ui-btn-right jqm-home"></a>
    </div>
    <!-- /header -->

    <!-- content -->
    <div data-role="content" >
        <div class="content-secondary">
            <div data-role="button" onclick="document.location = '?modulo=principal/grafico_evolucao_financeira&acao=C&filtro=<?php echo $dadosResumoEmp['empid'] ?>'">
                <a data-theme="b">EVOLUÇÃO FINANCEIRA</a>
            </div>

            <div id="div_imagem" style="border:2px dashed #CCCCCC; padding:2px; margin:5px 10px; width:310px; height:200px;">
                <a href="javascript: abrirFotos();" data-rel="dialog" data-transition="flip">
                     <img id="PhotoPrev" src="/imagens/no_image.png" style="width:310px;height:200px;"/>
                     <!--  
                     <img id="PhotoPrev" src="../slideshow/slideshow/verimagem.php?&arqid=<?= $dados['atiarqid']?>&newwidth=310&newheight=200" style="width:310px;height:200px;"/>
                     -->
                </a>
            </div>
            <div class="contentInfo">
                <p>
                  <div style="width:90px; float:left;">
                      <b>Unidade:</b>
                  </div>
                  <?=$dadosResumoEmp['entnomeunidade'] ?>
                </p>
                <p>
                  <div style="width:90px; float:left;">
                      <b>Campus:</b>
                  </div>
                  <?=(!empty($dadosResumoEmp['entnomecampus']) ? $dadosResumoEmp['entnomecampus'] : "   - ") ?>
                </p>
                <p>
                  <div style="width:90px; float:left;">
                      <b>Tipologia:</b>
                  </div>
                  <?= !empty($dadosResumoEmp['tpodsc']) ? $dadosResumoEmp['tpodsc'] : "   - " ?>
                </p>
                <p>
                  <div style="width:90px; float:left;">
                      <b>Situação:</b>
                  </div>
                  <?= !empty($dadosResumoEmp['esddsc']) ? $dadosResumoEmp['esddsc'] : "   - " ?>
                </p>
                <p>
                  <div style="width:110px; float:left;">
                      <b>% Execução:</b>
                  </div>
                  <?
                      if (!empty( $dadosResumoEmp['emppercentultvistoria'] )){
                        echo str_replace('.', ',', $dadosResumoEmp['emppercentultvistoria'])."% ( {$dadosResumoEmp['empdtultvistoria']} )";
                      }else{
                        echo "0,00%";
                      }
                   ?>
                </p>
                <p>
                  <div style="width:145px; float:left;">
                      <b>Valor Contratado:</b>
                  </div>
                  <?= formataNumeroApresentacao($dadosResumoEmp['somavlrcontrato']) ?>
                </p>
<!-- 
	                <div style="float:right;">
	                   <img border="0" style="width:70px; height:70px;" src="/imagens/pac_<?php echo $carimbo; ?>.png">
	                </div>
-->
            </div>
            <br>
        </div>

        <div class="content-primary">
            <div style="height:30px; border-bottom:1px solid #CCCCCC;">
                <div style="float:left;">
                    <b>Monitoramento do(s) Objeto(s)</b>
                </div>
            </div>

            <div id="div-rolagem" style="height:530px; min-width:575px; width: 100%; overflow:auto;">
<?php
$obra = new Obras();
$param = array('empid' 		   => $empid,
			   'not(obridpai)' => true);
$arObra = $obra->listaDadosResumo( $param );
foreach ($arObra as $dadoObra):
	$itensComposicao = new ItensComposicao();
	$dadosPreli = $itensComposicao->getByObraAndMacro($dadoObra['obrid'], MACROETAPA_SERVICO_PRELIMINARES);
    $dadosInfra = $itensComposicao->getByObraAndMacro($dadoObra['obrid'], MACROETAPA_INFRAESTRUTURA);
    $dadosSuper = $itensComposicao->getByObraAndMacro($dadoObra['obrid'], MACROETAPA_SUPERESTRUTURA);
    $dadosCompl = $itensComposicao->getByObraAndMacro($dadoObra['obrid'], MACROETAPA_SERVICO_COMPLEMENTARES);
//            $itensComposicao = new ItensComposicaoEstrutura();
//            $dadosPreli = $itensComposicao->getByEstrutuaAndItensByMacro($arEstrutura['eobid'], MACROETAPA_SERVICO_PRELIMINARES);
//            $dadosInfra = $itensComposicao->getByEstrutuaAndItensByMacro($arEstrutura['eobid'], MACROETAPA_INFRAESTRUTURA);
//            $dadosSuper = $itensComposicao->getByEstrutuaAndItensByMacro($arEstrutura['eobid'], MACROETAPA_SUPERESTRUTURA);
//            $dadosCompl = $itensComposicao->getByEstrutuaAndItensByMacro($arEstrutura['eobid'], MACROETAPA_SERVICO_COMPLEMENTARES);
?>
            <div data-role="button" onclick="abreEvolucaoFinan(<? echo $dadoObra['obrid']; ?>)">
                <a data-theme="b">
                    <?php echo '(' . $dadoObra['obrid'] . ') ' . $dadoObra['obrnome'];?>
                </a>
            </div>
            <table width="100%" border="0">
                <tr>
                    <th width="20%">
                    	% Execução: <?php echo number_format($dadoObra['obrpercentultvistoria'],2,',','.');?>
                    	<br>
                    	<?php echo ($dadoObra['obrdtultvistoria'] ? '(' . $dadoObra['obrdtultvistoria'] . ')' : '') ?>
                    </th>
                    <th width="20%" class="ui-btn-up-b">Serviços Preliminares</th>
                    <th width="20%" class="ui-btn-up-b">Infraestrutura</th>
                    <th width="20%" class="ui-btn-up-b">Superestrutura</th>
                    <th width="20%" class="ui-btn-up-b">Serviços Complementares</th>
                </tr>
                <tr>
                    <td>
                        <b>
                        <div>Início</div>
                        <div>Fim</div>
                        </b>
                    </td>
                    <td>
                        <div>
                        <?php echo $dadosPreli['icodtinicioitem'] ?>
                        </div>
                        <div>
                        <?php echo $dadosPreli['icodterminoitem'] ?>
                        </div>
                    </td>
                    <td>
                        <div>
                        <?php echo $dadosInfra['icodtinicioitem'] ?>
                        </div>
                        <div>
                        <?php echo $dadosInfra['icodterminoitem'] ?>
                        </div>
                    </td>
                    <td>
                        <div>
                        <?php echo $dadosSuper['icodtinicioitem'] ?>
                        </div>
                        <div>
                        <?php echo $dadosSuper['icodterminoitem'] ?>
                        </div>
                    </td>
                    <td>
                        <div>
                        <?php echo $dadosCompl['icodtinicioitem'] ?>
                        </div>
                        <div>
                        <?php echo $dadosCompl['icodterminoitem'] ?>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td><b>Valor previsto</b></td>
                    <td><?php echo formataNumeroApresentacao($dadosPreli['icovlritemsoma']) ?></td>
                    <td><?php echo formataNumeroApresentacao($dadosInfra['icovlritemsoma']) ?></td>
                    <td><?php echo formataNumeroApresentacao($dadosSuper['icovlritemsoma']) ?></td>
                    <td><?php echo formataNumeroApresentacao($dadosCompl['icovlritemsoma']) ?></td>
                </tr>
                <tr>
                    <td><b>Executado (%)</b></td>
                    <td><?php echo number_format($dadosPreli['vlrexecutado'],2,',','.')?></td>
                    <td><?php echo number_format($dadosInfra['vlrexecutado'],2,',','.')?></td>
                    <td><?php echo number_format($dadosSuper['vlrexecutado'],2,',','.')?></td>
                    <td><?php echo number_format($dadosCompl['vlrexecutado'],2,',','.')?></td>
                </tr>
                <!--  
                <tr>
                    <td><b>Situação</b></td>
                    <td><?php //echo getSituacao($dadosPreli)?></td>
                    <td><?php //echo getSituacao($dadosInfra)?></td>
                    <td><?php //echo getSituacao($dadosSuper)?></td>
                    <td><?php //echo getSituacao($dadosCompl)?></td>
                </tr>
                -->
<!--                <tr>-->
<!--                    <td colspan="5"><b>Em andamento: </b></td>-->
<!--                </tr>-->
            </table>
            <hr>

<?php
endforeach;
die;
?>
            <div data-role="button">
                <a data-theme="b">
                    Contratos
                </a>
            </div>
           <table width="100%" border="0">
                <tr>
                    <th width="20%" class="ui-btn-up-b" style="color: #FFFFFF; font-weight: bold; text-shadow: 0 -1px 1px #145072;">Tipo</th>
                    <th width="20%" class="ui-btn-up-b" style="color: #FFFFFF; font-weight: bold; text-shadow: 0 -1px 1px #145072;">CNPJ</th>
                    <th width="20%" class="ui-btn-up-b" style="color: #FFFFFF; font-weight: bold; text-shadow: 0 -1px 1px #145072;">Nome</th>
                    <th width="20%" class="ui-btn-up-b" style="color: #FFFFFF; font-weight: bold; text-shadow: 0 -1px 1px #145072;">Valor</th>
                    <th width="20%" class="ui-btn-up-b" style="color: #FFFFFF; font-weight: bold; text-shadow: 0 -1px 1px #145072;">Vigência (Início – Fim)</th>
                </tr>
                <?php
                $contrato  = new Contrato();
                $arContratos = $contrato->recuperaDadosContrato($atiid);

                if(is_array( $arContratos ) && !empty( $arContratos )){
                foreach ($arContratos as $arContrato):
                ?>
                    <tr>
                        <td><?php echo $arContrato['tpcdsc'] ?></td>
                        <td><?php echo $arContrato['concnpj'] ?></td>
                        <td><?php echo $arContrato['conrazaosocial'] ?></td>
                        <td><?php echo formata_valor($arContrato['convlrglobal']) ?></td>
                        <td><?php echo $arContrato['condtinicio']." - ".$arContrato['condttermino'] ?></td>
                    </tr>
                <?php
                endforeach;
                }else{
                ?>
                <tr>
                    <td colspan="5" align="center"> Nenhum contrato cadastrado </td>
                </tr>
                <?php } ?>
            </table>

            </div>
        </div>
    </div>
    <!-- /content -->

    <!-- footer -->
     <div id="rodape">
		<a href="projetos.php?modulo=relatorio/mobile&acao=A"  class="linkVoltar" title="Voltar"></a>
     </div>

<!--    <div data-role="footer" data-theme="b" data-position="fixed">-->
<!--        <div data-role="controlgroup" data-type="horizontal">-->
<!--            <a href="projetos.php?modulo=relatorio/mobile&acao=A" data-icon="home" class="inicio-rodape">Voltar</a>-->
<!--        </div>-->
<!--    </div>-->
    <!-- /footer -->
</div>
<!-- /page -->

<div class="demo">
    <div id="dialog-message" title="Galeria">
        <?php
            $_GET['atiid'] = $atiid;
            include "mobile_dialog_album.inc";
        ?>

    </div>
</div><!-- End demo -->

<?php

function formataNumeroApresentacao($valor)
{
    if ( $valor / 1000 <= 1000 && $valor > 0)
        return "R$ " . number_format( $valor / 1000, "1", ",", ".") . " mil";
    elseif ( $valor / 1000000 <= 1000  && $valor > 0)
        return "R$ " . number_format( $valor / 1000000, "1", ",", ".") . " milhões";
    elseif ( $valor / 1000000000 <= 1000  && $valor > 0)
        return "R$ " . number_format( $valor / 1000000000, "1", ",", ".") . " bilhões";
    else
        return "R$ " . number_format( $valor, "2", ",", ".");
}

function getSituacao( Array $dados){
    if(count($dados) > 0){
        if ($dados['icopercexecutado'] >= 100){
            return 'Concluído';
        }elseif (strtotime($dados['icodterminoitembd']) >  strtotime("now")){
            return 'Atrasado';
        }else{
            return 'Em andamento';
        }
    }else{
        return '-';
    }

}
?>