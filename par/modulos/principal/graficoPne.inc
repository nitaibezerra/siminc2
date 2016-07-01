<?php
include_once('funcoesgraficoPne.inc');

?>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>


    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>

    <!--    <script src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>-->
    <!--	<script src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>-->

    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>



    <!--	<script type="text/javascript" src="js/estrategico.js"></script>-->
    <script src="../includes/funcoes.js"></script>
    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />    
    <script language="javascript" src="js/javascript.js"></script>
    <link rel='stylesheet' type='text/css' href='css/style.css'>
    <style>
        .div_lbl_grfAno{
            float: left;
            margin-top: 5px;
            <?php if ($_SESSION['par']['itrid'] == 1) { ?> margin-left: 14px; <?php } else { ?> margin-left: 37px; <?php } ?>
            font-size: 20px;
        }

    </style>

    <script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

    <script type="text/javascript">

        
    </script>
</head>
<body>

<section id="ti" style="display: none;margin-top: 30px; width: 400px;">
    <p style="font-size: 22px;font-weight: bold; margin:0 0 5px 0;">Ministério da Educação</p>
    <p style="font-size: 16px;">
        Secretaria de Articulação com os Sistemas de Ensino <br/>
        Diretoria de Cooperação e Planos de Educação <br/>
        Projeção da Contribuição para a Meta Nacional
    </p>
</section>
<section id="barra-brasil">
    <section id="wrapper-barra-brasil">
        <section class="brasil-flag">
            <a href="http://brasil.gov.br" class="link-barra">
                Brasil
            </a>
        </section>
			<span class="acesso-info">
				<a href="http://brasil.gov.br/barra#acesso-informacao" class="link-barra">
                    Acesso à informação
                </a>
			</span>
        <ul class="list">
            <li class="list-item first">
                <a href="http://brasil.gov.br/barra#participe" class="link-barra">
                    Participe
                </a>
            </li>
            <li class="list-item">
                <a href="http://www.servicos.gov.br/" class="link-barra">
                    Serviços
                </a>
            </li>
            <li class="list-item">
                <a href="http://www.planalto.gov.br/legislacao" class="link-barra">
                    Legislação
                </a>
            </li>
            <li class="list-item last last-item">
                <a href="http://brasil.gov.br/barra#orgaos-atuacao-canais" class="link-barra">
                    Canais
                </a>
            </li>
        </ul>
    </section>
</section>

<!-- Header -->
<header id="top" class="header">
    <div class="topo">
        <div class="topo-left" style="margin-top: 5px;">
            <div class="text-left">
                <img src="../estrutura/temas/default/img/logo-simec.png" class="img-responsive" width="200">
            </div>
        </div>

        <div class="topo-right" style="margin-top: 5px;">
            <a href="http://www.brasil.gov.br/" class="brasil pull-right">
                <img style="margin-right: 10px;" src="http://portal.mec.gov.br/templates/mec2014/images/brasil.png" alt="Brasil - Governo Federal" class="img-responsive">
            </a>
        </div>
    </div>
</header>

<div id="dialog-detalhe"></div>

<div style="width: 70%;margin:0 auto; padding: 10px 0 0 0;">
    <h1 id="h1contribuicao" style="display:none; margin:15px auto 0 auto; text-align: center;">Metas PNE - <?= $_SESSION['par']['mundescricao'] ?></h1>
</div>
<div class="container">
    <form name='formulario' action='' method='post' style="width:909px;display:table;align:center;border:0; margin: 0 auto;"  >
        <input type='hidden' name='metid' value = ''  id = "metid" >
        <input type='hidden' name='requisicao' value = ''  id = "requisicao" >
        <input type='hidden' name='tipo' value = ''  id = "tipo" >
        <table style="width: 100%!important;">
            <tr>
                <td colspan="2">
                    <?php
                    echo montarAbasArrayLocal( criarAbasMetasPNE() , "" );
                    ?>
                </td>
            </tr>
            <tr>
                <td  id="esconder"  style="display:none;"class="" width="20%" valign="top">
                    <div style="margin-top:20px;" id="pesquisa">
                        <div style="float:left;" class="titulo_box" >
                            Pesquisa<br/><br/>
                        </div>
                    </div>
                    <table  cellpadding="5" cellspacing="1" width="100%" id="tabelaRegioes">
                        <?php
                        #Região
                        $sql = " Select	regcod AS codigo, regdescricao AS descricao From territorios.regiao order by regdescricao";

                        mostrarComboPopupLocal( 'Região', 'slRegiao',  $sql, "", 'Selecione as Regiões', null,'atualizarRelacionadosRegiao(1)',false);
                        ?>
                    </table>
                    <table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaEstados" class="filtro_combo">
                        <?php
                        listarEstados();
                        ?>
                    </table>
                    <table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaMesoregioes" class="filtro_combo">
                        <?php
                        listarMesoregioes();
                        ?>
                    </table>
<!--                    <table  cellpadding="5" cellspacing="1" width="100%" id ="tabelaMunicipios" class="filtro_combo">-->
<!--                        --><?php
//                        listarMunicipios();
//                        ?>
<!--                    </table>-->
                </td>
                <td class="" width="80%" id="divListagem" valign="top">
                </td>
            </tr>
        </table>
    </form>
</div>

<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

</body>
</html>