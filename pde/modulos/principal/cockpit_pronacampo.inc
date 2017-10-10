<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_professor1'])){
    gerarGraficoProfessor1($_REQUEST['detalhe_professor1']);
    die;
}
?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=9" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="JavaScript" src="../includes/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <script>
        atualizaUsuario();
        jQuery(function(){
            jQuery('.filtrar_professor1').click(function(){
                jQuery('#div_professor1').load('estrategico.php?modulo=principal/cockpit_pronacampo&acao=A&detalhe_professor1='+jQuery(this).val());
            });
        });
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_pronacampo.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
                <div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pronacampo&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Pronacampo</div></td>
    </tr>
    <tr>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_construcao_escolas&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Construção de escolas'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_caminhos_escola&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Caminho da Escola'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_educacao_campo&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Mais Educação Campo'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_pdde_campo&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'PDDE Campo'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_pronatec_campo&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Pronatec Campo'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_fic&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Formação Inicial e Continuada'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_inclusao_digital&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Inclusão Digital'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" align="left" onclick="window.location='';">
            <div>
                <?php exibirTitulo('Presentation', 'PNLD'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" align="left" onclick="window.location='';">
            <div>
                <?php exibirTitulo('Presentation', 'PNBE'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2023);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Construção de escolas', 'Obras aprovadas'); ?>
            </div>
            <?php
            $sql = "select
						tid2.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (2023)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaConstrucaoEscola", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1865);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Caminho da Escola', 'Ônibus rurais adquiridos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS descricao, sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (1865)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 IN (4299) --Escolar rural
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaCaminhoEscola', null, null, $formatoValores, null, "", 1000, 600, null, 300, "", '#fff', true);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1205);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Mais Educação Campo', 'Escolas rurais que aderiram'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS descricao,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (1205)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (3192) --Rural
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaMaisEducacao', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);">
            <div>
                <?php exibirTitulo('indicador', 'PDDE Campo', 'Recursos Repassados'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
						tid1.tiddsc as dependencia,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (678)
					and sh.sehstatus <> 'I'
					and dsh.tidid2 IN (2492, 2495, 2494, 2493)
					group by ano, dependencia
					order by ano, dependencia";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'dependencia', 'valor');
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaPDDECampo', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2082);">
            <div>
                <?php exibirTitulo('indicador', 'Pronatec Campo', 'Matrículas'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS rede,
                        'Matriculas' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2082)
                    AND sh.sehstatus <> 'I'
                    GROUP BY rede, tipo
                    ORDER BY rede, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'rede'=>$dado['rede'],
                        'tipo'=>'Matriculas Acumuladas',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaPronatecCampo', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            ?>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(1460);">
                <?php exibirTitulo('indicador', 'Formação Inicial e Continuada', 'Professores Ingressantes Procampo'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_professor1" name="professor1" value="1" checked="checked" onclick="" />Por ano
                <input type="radio" class="filtrar_professor1" name="professor1" value="2" onclick="" />Por edital
            </div>
            <div id="div_professor1">
                <?php gerarGraficoProfessor1(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="3">
            <div class="link" onclick="abreIndicadorPopUp(227);">
                <?php exibirTitulo('indicador', 'Inclusão Digital', 'Laboratórios Adquiridos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS rede,
                        tid2.tiddsc AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (227)
                    AND sh.sehstatus <> 'I'
                    GROUP BY rede, tipo
                    ORDER BY rede, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $totalAgrupado = "";
                foreach ($dados as $dado){
                    $totalAgrupado += $dado['valor'];
                }

                $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.series.name + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaLaboratorios', null, null, $formatoValores, null, "", 1000, 600, null, 300);
                echo "<table align='center'>";
                echo "  <tr>";
                echo "      <td class='titulo_box bold'>Total: ".number_format($totalAgrupado,0,',','.')."</td>";
                echo "  </tr>";
                echo "</table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_td_laranja" colspan="3">
            <div style="text-align:center;" >
                <img src="../imagens/icones/icons/executiverel.png" style="vertical-align:middle;" />
                <input type="text" onclick="this.style.color='#000000';this.value='';" name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;' title='' class=' normal' />
                <img src="../imagens/icones/icons/Find.png" style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);" />
            </div>
        </td>
    </tr>
</table>
</body>
</html>
<?php
function gerarGraficoProfessor1($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "dpe.dpeanoref as descricao";
    }else{
        $campo = "tid1.tiddsc as descricao";
    }

    $sql = "select
                $campo,
                sum(dsh.dshqtde) as valor
            from painel.seriehistorica sh
            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
            inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            where sh.indid in (1460)
            and sh.sehstatus <> 'I'
            group by descricao
            order by descricao";
    $arrDados = $db->carregar($sql, null, 3200);
    if($arrDados){
        geraGrafico($arrDados, "graficoPizzaFIC", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
    }
}
?>