<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
include APPRAIZ . 'www/sispacto2/_constantes.php';
//include APPRAIZ . 'www/sispacto2/_funcoes.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_pacto1'])){
    gerarGraficoPacto1($_REQUEST['detalhe_pacto1']);
    die;
}

if(!empty($_REQUEST['atualizarAbrangencia'])){
    exibirPorcentagemPagamentoPerfil2(array('uncid'=>$_REQUEST['uncid'],'fpbid'=>$_REQUEST['fpbid']));
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
            jQuery('.filtrar_pacto1').click(function(){
                jQuery('#div_pacto1').load('estrategico.php?modulo=principal/cockpit_pacto&acao=A&detalhe_pacto1='+jQuery(this).val());
            });
        });

        function detalharPorcentagemPerfil2(fpbid, uncid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 8;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_pacto&acao=A&atualizarAbrangencia=1&uncid='+uncid+'&fpbid='+fpbid);
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_maiseducacao.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
        #popdiv{display:block;width:100%;height:500px;overflow-y: scroll;}
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
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pacto&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Pacto Nacional pela Alfabetização na Idade Certa</div></td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('executive', 'Professores Alfabetizadores - 2014'); ?>
            </div>
            <table class="" cellpadding="5" cellspacing="4" width="80%" align="center" border="0" >
                <?php
                $sql = "select sum(dsh.dshqtde) AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        where sh.indid in (2841)
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                        and sh.sehstatus <> 'I'";
                $valor2841 = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2841);">
                    <td class="titulo_box bold" width="100%"><?php exibirTitulo('doc', number_format($valor2841,0,",",".") . ' inscritos'); ?></td>
                </tr>
                <?php
                $sql = "select sum(dsh.dshqtde) AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        where sh.indid in (2845)
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                        and sh.sehstatus <> 'I'";
                $valor2845 = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2845);">
                    <td class="titulo_box bold" width="100%"><?php exibirTitulo('doc', number_format($valor2845,0,",",".") . ' participantes'); ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="34%">
            <div>
                <?php exibirTitulo('financeiro', 'Bolsas do Pacto - 2014', 'Sispacto 2014'); ?>
            </div>
            <?php
            $sql = "select count(*) from sispacto2.identificacaousuario i
                    inner join sispacto2.tipoperfil t on t.iusd = i.iusd
                    where t.pflcod=1118 and i.iusstatus='A' and iustipoprofessor='censo'";
            $totalProfessores = $db->pegaUm($sql);
            $sql = "select count(*) from sispacto2.identificacaousuario i
                    inner join sispacto2.tipoperfil t on t.iusd = i.iusd
                    inner join seguranca.perfil p on p.pflcod = t.pflcod
                    where t.pflcod in(1131,1168,1130,1129,1120,1117,1119) and i.iusstatus='A' and i.iuscpf not ilike 'SIS%'";
            $totalBolsistasPedagogica = $db->pegaUm($sql);
            $sql = "select sum(pbovlrpagamento) from sispacto2.pagamentobolsista p
                    inner join workflow.documento d on d.docid = p.docid
                    where d.esdid=1015";
            $valorBolsas = $db->pegaUm($sql);
            ?>
            <table class="" cellpadding="5" cellspacing="4" width="100%" align="center" border="0" >
                <tr>
                    <td class="titulo_box bold" width="50%"><?php exibirTitulo('executive', number_format($totalProfessores,0,",",".") . '<br>professores<br>bolsistas'); ?></td>
                    <td class="titulo_box bold" width="50%"><?php exibirTitulo('executive', number_format($totalBolsistasPedagogica,0,",",".") . '<br>bolsistas<br>equipe<br>pedagógica'); ?></td>
                </tr>
                <tr>
                    <td class="titulo_box bold center" colspan="2"><br><br><br><br><font size="+2">Bolsas - valor total pago<br>R$ <?=number_format($valorBolsas,2,",",".")?></font></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Professores Alfabetizadores Inscritos - 2013/2014', '<span class="link" onclick="abreIndicadorPopUp(2840);">Inscritos 2013</span>, <span class="link" onclick="abreIndicadorPopUp(2841);">Inscritos 2014</span>'); ?>
            </div>
            <?php
            $sql = "SELECT  case sh.indid
								when 2840 then 2013
								when 2841 then 2014
							end as descricao,
							sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2840,2841)
					AND sh.sehstatus <> 'I'
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					GROUP BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaProfessoresAlfInsc', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2841);" colspan="3">
            <div>
                <?php exibirTitulo('mapas', 'Número de professores alfabetizadores inscritos por Estado (2014)'); ?>
            </div>
            <?php
            $sql = "SELECT dsh.dshuf as descricao, tiddsc as detalhe, sum(dsh.dshqtde::integer) as valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (2841)
                --AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                AND dpe.dpeanoref = '2014'
                AND sh.sehstatus <> 'I'
                GROUP BY descricao, detalhe
                ORDER BY descricao, detalhe";
            $arrDados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'detalhe', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Matrículas: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaProfessorEstado', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="3">
            <div>
                <?php exibirTitulo('doc', 'Informações gerais sobre turmas - 2014', 'Sispacto 2014'); ?>
            </div>
            <?php
            $sql = "SELECT
                        SUM(tpatotalmeninos)+SUM(tpatotalmeninos) as nalunos,
                        COUNT(*) as nturmas,
                        COUNT(DISTINCT t.iusd) as nprofessores,
                        ROUND((SUM(tpatotalmeninos)+SUM(tpatotalmeninas))/count(*)::numeric,1) as media,
                        SUM(tpatotalmeninos) as totalmeninos,
                        ROUND((SUM(tpatotalmeninos)*100)/(SUM(tpatotalmeninos)+SUM(tpatotalmeninas))::numeric,1) as porc_meninos,
                        SUM(tpatotalmeninas) as totalmeninas,
                        ROUND((SUM(tpatotalmeninas)*100)/(SUM(tpatotalmeninos)+SUM(tpatotalmeninas))::numeric,1) as porc_meninas,
                        COALESCE(SUM(tpafaixaetariaabaixo6anos),0) as tpafaixaetariaabaixo6anos,
                        ROUND((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixaetariaabaixo6anos,
                        COALESCE(SUM(tpafaixaetaria6anos),0) as faixaetaria6anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria6anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa6anos,
                        COALESCE(SUM(tpafaixaetaria7anos),0) as tpafaixaetaria7anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria7anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa7anos,
                        COALESCE(SUM(tpafaixaetaria8anos),0) as tpafaixaetaria8anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria8anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa8anos,
                        COALESCE(SUM(tpafaixaetaria9anos),0) as tpafaixaetaria9anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria9anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa9anos,
                        COALESCE(SUM(tpafaixaetaria10anos),0) as tpafaixaetaria10anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria10anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa10anos,
                        COALESCE(SUM(tpafaixaetaria11anos),0) as tpafaixaetaria11anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria11anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa11anos,
                        COALESCE(SUM(tpafaixaetariaacima11anos),0) as tpafaixaetariaacima11anos,
                        ROUND((COALESCE(SUM(tpafaixaetariaacima11anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixaetariaacima11anos,
                        COALESCE(SUM(tpabolsafamiliaabaixo6anos),0)+COALESCE(SUM(tpabolsafamilia6anos),0)+COALESCE(SUM(tpabolsafamilia7anos),0)+COALESCE(SUM(tpabolsafamilia8anos),0)+COALESCE(SUM(tpabolsafamilia9anos),0)+COALESCE(SUM(tpabolsafamilia10anos),0)+COALESCE(SUM(tpabolsafamilia11anos),0)+COALESCE(SUM(tpabolsafamiliaacima11anos),0) as tpatotalbolsafamilia,
                        ROUND(((COALESCE(SUM(tpabolsafamiliaabaixo6anos),0)+COALESCE(SUM(tpabolsafamilia6anos),0)+COALESCE(SUM(tpabolsafamilia7anos),0)+COALESCE(SUM(tpabolsafamilia8anos),0)+COALESCE(SUM(tpabolsafamilia9anos),0)+COALESCE(SUM(tpabolsafamilia10anos),0)+COALESCE(SUM(tpabolsafamilia11anos),0)+COALESCE(SUM(tpabolsafamiliaacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_bolsafamilia,
                        COALESCE(SUM(tpavivemcomunidadeabaixo6anos),0)+COALESCE(SUM(tpavivemcomunidade6anos),0)+COALESCE(SUM(tpavivemcomunidade7anos),0)+COALESCE(SUM(tpavivemcomunidade8anos),0)+COALESCE(SUM(tpavivemcomunidade9anos),0)+COALESCE(SUM(tpavivemcomunidade10anos),0)+COALESCE(SUM(tpavivemcomunidade11anos),0)+COALESCE(SUM(tpavivemcomunidadeacima11anos),0) as tpatotalvivemcomunidade,
                        ROUND(((COALESCE(SUM(tpavivemcomunidadeabaixo6anos),0)+COALESCE(SUM(tpavivemcomunidade6anos),0)+COALESCE(SUM(tpavivemcomunidade7anos),0)+COALESCE(SUM(tpavivemcomunidade8anos),0)+COALESCE(SUM(tpavivemcomunidade9anos),0)+COALESCE(SUM(tpavivemcomunidade10anos),0)+COALESCE(SUM(tpavivemcomunidade11anos),0)+COALESCE(SUM(tpavivemcomunidadeacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_vivemcomunidade,
                        COALESCE(SUM(tpafreqcrecheabaixo6anos),0)+COALESCE(SUM(tpafreqcreche6anos),0)+COALESCE(SUM(tpafreqcreche7anos),0)+COALESCE(SUM(tpafreqcreche8anos),0)+COALESCE(SUM(tpafreqcreche9anos),0)+COALESCE(SUM(tpafreqcreche10anos),0)+COALESCE(SUM(tpafreqcreche11anos),0)+COALESCE(SUM(tpafreqcrecheacima11anos),0) as tpatotalfreqcreche,
                        ROUND(((COALESCE(SUM(tpafreqcrecheabaixo6anos),0)+COALESCE(SUM(tpafreqcreche6anos),0)+COALESCE(SUM(tpafreqcreche7anos),0)+COALESCE(SUM(tpafreqcreche8anos),0)+COALESCE(SUM(tpafreqcreche9anos),0)+COALESCE(SUM(tpafreqcreche10anos),0)+COALESCE(SUM(tpafreqcreche11anos),0)+COALESCE(SUM(tpafreqcrecheacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_freqcreche,
                        COALESCE(SUM(tpafreqpreescolaabaixo6anos),0)+COALESCE(SUM(tpafreqpreescola6anos),0)+COALESCE(SUM(tpafreqpreescola7anos),0)+COALESCE(SUM(tpafreqpreescola8anos),0)+COALESCE(SUM(tpafreqpreescola9anos),0)+COALESCE(SUM(tpafreqpreescola10anos),0)+COALESCE(SUM(tpafreqpreescola11anos),0)+COALESCE(SUM(tpafreqpreescolaacima11anos),0) as tpatotalfreqpreescola,
                        ROUND(((COALESCE(SUM(tpafreqpreescolaabaixo6anos),0)+COALESCE(SUM(tpafreqpreescola6anos),0)+COALESCE(SUM(tpafreqpreescola7anos),0)+COALESCE(SUM(tpafreqpreescola8anos),0)+COALESCE(SUM(tpafreqpreescola9anos),0)+COALESCE(SUM(tpafreqpreescola10anos),0)+COALESCE(SUM(tpafreqpreescola11anos),0)+COALESCE(SUM(tpafreqpreescolaacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_freqpreescola
                    FROM sispacto2.turmasprofessoresalfabetizadores t
                    INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola
                    INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd
                    WHERE tpastatus='A' AND tpaconfirmaregencia=true AND (tpatotalmeninos is not null or tpatotalmeninas is not null)";
            $arrDadosInicio = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>INÍCIO</th>
                    <th>Número de alunos</th>
                    <th>Número de turmas</th>
                    <th>Número de professores</th>
                    <th>Média Alunos/Turmas</th>
                    <th>% Meninos</th>
                    <th>% Meninas</th>
                    <th>% Abaixo de 6 anos</th>
                    <th>% 6 anos</th>
                    <th>% 7 anos</th>
                    <th>% 8 anos</th>
                    <th>% 9 anos</th>
                    <th>% 10 anos</th>
                    <th>% 11 anos</th>
                    <th>% Acima de 11 anos</th>
                    <th>% Recebem bolsas famílias</th>
                    <th>% Vive em comunidade</th>
                    <th>% Frequentam creche</th>
                    <th>% Frequentam pré-escola</th>
                </tr>
                <tr>
                    <td class="">Brasil</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['nalunos'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['nturmas'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['nprofessores'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['media'],2,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_meninos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_meninas'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixaetariaabaixo6anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixa6anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixa7anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixa8anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixa9anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixa10anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixa11anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_faixaetariaacima11anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_bolsafamilia'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_vivemcomunidade'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_freqcreche'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosInicio['porc_freqpreescola'],2,",",".") ?>%</td>
                </tr>
            </table>
            <BR>
            <BR>
            <?php
            $sql = "SELECT
                        SUM(tpatotalmeninos9)+SUM(tpatotalmeninos9) as nalunos,
                        COUNT(*) as nturmas,
                        COUNT(DISTINCT t.iusd) as nprofessores,
                        ROUND((SUM(tpatotalmeninos9)+SUM(tpatotalmeninas9))/count(*)::numeric,1) as media,
                        SUM(tpatotalmeninos9) as totalmeninos,
                        ROUND((SUM(tpatotalmeninos9)*100)/(SUM(tpatotalmeninos9)+SUM(tpatotalmeninas9))::numeric,1) as porc_meninos,
                        SUM(tpatotalmeninas9) as totalmeninas,
                        ROUND((SUM(tpatotalmeninas9)*100)/(SUM(tpatotalmeninos9)+SUM(tpatotalmeninas9))::numeric,1) as porc_meninas,
                        COALESCE(SUM(tpafaixaetariaabaixo6anos9),0) as tpafaixaetariaabaixo6anos,
                        ROUND((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixaetariaabaixo6anos,
                        COALESCE(SUM(tpafaixaetaria6anos9),0) as faixaetaria6anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria6anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa6anos,
                        COALESCE(SUM(tpafaixaetaria7anos9),0) as tpafaixaetaria7anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria7anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa7anos,
                        COALESCE(SUM(tpafaixaetaria8anos9),0) as tpafaixaetaria8anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria8anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa8anos,
                        COALESCE(SUM(tpafaixaetaria9anos9),0) as tpafaixaetaria9anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria9anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa9anos,
                        COALESCE(SUM(tpafaixaetaria10anos9),0) as tpafaixaetaria10anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria10anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa10anos,
                        COALESCE(SUM(tpafaixaetaria11anos9),0) as tpafaixaetaria11anos,
                        ROUND((COALESCE(SUM(tpafaixaetaria11anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa11anos,
                        COALESCE(SUM(tpafaixaetariaacima11anos9),0) as tpafaixaetariaacima11anos,
                        ROUND((COALESCE(SUM(tpafaixaetariaacima11anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixaetariaacima11anos
                    FROM sispacto2.turmasprofessoresalfabetizadores t
                    INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola
                    INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd
                    WHERE tpastatus='A' AND tpaconfirmaregencia=true AND (tpatotalmeninos9 is not null or tpatotalmeninas9 is not null)";
            $arrDadosFim = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>FIM</th>
                    <th>Número de alunos</th>
                    <th>Número de turmas</th>
                    <th>Número de professores</th>
                    <th>Média Alunos/Turmas</th>
                    <th>% Meninos</th>
                    <th>% Meninas</th>
                    <th>% Abaixo de 6 anos</th>
                    <th>% 6 anos</th>
                    <th>% 7 anos</th>
                    <th>% 8 anos</th>
                    <th>% 9 anos</th>
                    <th>% 10 anos</th>
                    <th>% 11 anos</th>
                    <th>% Acima de 11 anos</th>
                </tr>
                <tr>
                    <td class="">Brasil</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['nalunos'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['nturmas'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['nprofessores'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['media'],2,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_meninos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_meninas'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixaetariaabaixo6anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixa6anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixa7anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixa8anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixa9anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixa10anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixa11anos'],2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($arrDadosFim['porc_faixaetariaacima11anos'],2,",",".") ?>%</td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" rowspan="3">
            <div id = "popdiv">
                <div>
                    <?php exibirTitulo('indicador', 'IES participantes', 'Sispacto 2014'); ?>
                </div>
                <?php
                $sql = "select
                            foo.universidade,
                            foo.professoresparticipantes,
                            foo.orientadoresparticipantes,
                            round(foo.professoresparticipantes::numeric/foo.orientadoresparticipantes::numeric,2) as relacaoorientadorprofessor
                        from (
                            select
                                uu.unisigla||' - '||uu.uninome as universidade,
                                (select count(distinct i.iusd) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd inner join sispacto2.mensario m on m.iusd = i.iusd and m.pflcod=1118 inner join territorios.municipio mu on mu.muncod = i.muncodatuacao where i.iusstatus='A' and t.pflcod=1118 and i.uncid=u.uncid) as professoresparticipantes,
                                (select count(distinct i.iusd) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd inner join sispacto2.mensario m on m.iusd = i.iusd and m.pflcod=1120 inner join territorios.municipio mu on mu.muncod = i.muncodatuacao where i.iusstatus='A' and t.pflcod=1120 and i.iuscpf not ilike '%SIS%' and i.uncid=u.uncid) as orientadoresparticipantes
                            from sispacto2.universidadecadastro u
                            inner join sispacto2.universidade uu on uu.uniid = u.uniid
                        ) foo
                        order by foo.universidade";
                $arrDados = $db->carregar($sql, null, 3200);
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <tr>
                        <th>Instituição</th>
                        <th>Professores participantes</th>
                        <th>Orientadores de estudo</th>
                    </tr>
                    <?php
                    if($arrDados){
                        $totalprofessoresparticipantes = 0;
                        $totalorientadoresparticipantes = 0;
                        foreach($arrDados as $dado):
                            $totalprofessoresparticipantes+=$dado['professoresparticipantes'];
                            $totalorientadoresparticipantes+=$dado['orientadoresparticipantes'];
                            $count++;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $dado['universidade'] ?></td>
                                <td class="numero" ><?php echo number_format($dado['professoresparticipantes'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['orientadoresparticipantes'],0,",",".") ?></td>
                            </tr>
                        <?php
                        endforeach;
                    }?>
                    <tr>
                        <th class="bold" >Total</th>
                        <th class="numero" ><?php echo number_format($totalprofessoresparticipantes,0,",",".") ?></th>
                        <th class="numero" ><?php echo number_format($totalorientadoresparticipantes,0,",",".") ?></th>
                    </tr>
                </table>
            </div>
        </td>
        <td class="fundo_padrao" rowspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Pagamento de bolsas', 'Sispacto 2014'); ?>
            </div>
            <?=exibirPorcentagemPagamento2('');?>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(159);?>
        </td>
    </tr>
    <tr>
        <!-- Tabela Processos -->
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,159,'','');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
    <tr>
        <!-- Tabela Processos -->
        <td class="fundo_padrao link center" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/pacto/default.htm')";>
        <div>
            <img style="float:center" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
            <div style="float:center" class="titulo_box" >Mapa do Processo<br/></div>
        </div>
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
function exibirPorcentagemPagamento2($dados) {
    global $db;

    if(!is_array($dados)){
        $dados = array();
    }

    if($dados['uncid']) $wh[] = "i.uncid='".$dados['uncid']."'";
    if($dados['fpbid']) $wh1[] = "f.fpbid='".$dados['fpbid']."'";

    $sql = "SELECT f.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM sispacto2.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia
			".(($dados['uncid'])?"INNER JOIN sispacto2.folhapagamentouniversidade fp ON fp.fpbid = f.fpbid AND fp.uncid='".$dados['uncid']."' AND fp.pflcod IS NULL":"")."
			".(($wh1)?" WHERE ".implode(" AND ", $wh1):"");

    $folhapagamento = $db->carregar($sql, null, 3200);

    echo '<table class="tabela_box" cellSpacing="1" cellPadding="3" align="center" width="100%">';
    if($folhapagamento[0]) {

        echo '<tr>';
        echo '<th>&nbsp;</th>';
        echo '<th>Período de Referência</th>';
        echo '<th>Total bolsistas</th>';
        echo '<th colspan=2>Total em pagamento</th>';
        echo '<th colspan=2>Total concluído</th>';
        echo '<th>Restante</th>';
        echo '</tr>';

        foreach($folhapagamento as $fp) {
            echo '<tr>';

            $sql = "SELECT count(*) as tot, sum(foo.plpvalor) as vlr FROM (

 					SELECT i.iusd, i.uncid,  t.pflcod, p.plpmaximobolsas, (SELECT count(*) FROM sispacto2.mensario WHERE iusd=i.iusd) as numerobolsas, p.plpvalor
 					FROM sispacto2.identificacaousuario i
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
					INNER JOIN sispacto2.pagamentoperfil p ON p.pflcod = t.pflcod
					INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.uncid = i.uncid AND rf.pflcod = t.pflcod AND rf.fpbid='".$fp['fpbid']."'
					LEFT JOIN sispacto2.pactoidadecerta pa ON pa.picid = i.picid
					LEFT JOIN workflow.documento dc ON dc.docid = pa.docidturma
					WHERE i.iusstatus='A' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' AND dc.esdid='".ESD_FECHADO_TURMA."' ELSE true END AND CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END ".(($wh)?" AND ".implode(" AND ", $wh):"")."

 					) foo
					{$f}";
            $totalinsc = $db->pegaLinha($sql, null, 3200);
            $totalus = $totalinsc['tot'];


            $sql = "SELECT count(*) as tot FROM sispacto2.pagamentobolsista p
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid
					WHERE p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");

            $totalpag = $db->pegaUm($sql, null, 3200);

            $sql = "SELECT count(*) as tot, sum(p.pbovlrpagamento) as vlr FROM sispacto2.pagamentobolsista p
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid
					INNER JOIN workflow.documento d ON d.docid = p.docid
					WHERE d.esdid IN('".ESD_PAGAMENTO_EFETIVADO."','".ESD_PAGAMENTO_NAO_AUTORIZADO."') AND p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");

            $totalpagefetivado = $db->pegaLinha($sql, null, 3200);
            $totalpagef = $totalpagefetivado['tot'];


            echo '<td><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick="detalharPorcentagemPerfil2('.$fp['fpbid'].',\''.$dados['uncid'].'\',this);"></td>';
            echo '<td>'.$fp['periodo'].'</td>';
            echo '<td align=right>'.$totalus.'</td>';

            echo '<td align=right>'.(($totalpag)?$totalpag:'0').'</td>';
            if($totalus) $porc = round(($totalpag/$totalus)*100,0);
            else $porc = 0;
            echo '<td>';
            progressBar2($porc);
            echo '</td>';

            echo '<td align=right>'.(($totalpagef)?$totalpagef:'0').'</td>';
            if($totalus) $porc = round(($totalpagef/$totalus)*100,0);
            else $porc = 0;
            echo '<td>';
            progressBar2($porc);
            echo '</td>';

            $totalbolsasrestante += ($totalus-$totalpagef);
            $totalvalorrestante += ($totalinsc['vlr']-$totalpagefetivado['vlr']);

            echo '<td nowrap align=right style=font-size:x-small;>'.number_format($totalus-$totalpagef,0,",",".").'<br>R$ '.number_format($totalinsc['vlr']-$totalpagefetivado['vlr'],2,",",".").'</td>';

            echo '</tr>';

        }

        echo '<tr>';
        echo '<th colspan=7>Total</th>';
        echo '<th nowrap align=right style=font-size:x-small;>'.number_format($totalbolsasrestante,0,",",".").'<br>R$ '.number_format($totalvalorrestante,2,",",".").'</th>';
        echo '</tr>';

    }
    echo '</table>';
}

function exibirPorcentagemPagamentoPerfil2($dados) {
    global $db;

    if(!is_array($dados)){
        $dados = array();
    }

    if($dados['uncid']) $wh[] = "i.uncid='".$dados['uncid']."'";

    $sql = "SELECT p.pflcod, p.pfldsc FROM sispacto2.pagamentoperfil pp
			INNER JOIN seguranca.perfil p ON p.pflcod = pp.pflcod";
    $perfil = $db->carregar($sql, null, 3200);

    echo '<table class="tabela_box" cellSpacing="1" cellPadding="3" align="center" width="100%">';
    if($perfil) {

        echo '<tr>';
        echo '<th>Perfil</th>';
        echo '<th>Total bolsistas</th>';
        echo '<th>Total em pagamento</th>';
        echo '<th>&nbsp;</th>';
        echo '<th>Total concluído</th>';
        echo '<th>&nbsp;</th>';
        echo '</tr>';

        foreach($perfil as $p) {

            echo '<tr>';

            $uncids = $db->carregarColuna("SELECT DISTINCT uncid FROM sispacto2.tipoavaliacaoperfil WHERE fpbid <='".$dados['fpbid']."'");

            unset($f);

            if($uncids) {
                $f = "WHERE CASE WHEN foo.uncid IN('".implode("','",$uncids)."') THEN foo.numerobolsas < foo.plpmaximobolsas ELSE true END";
            }

            $sql = "SELECT count(*) as tot FROM (

					SELECT i.iusd, i.uncid,  t.pflcod, p.plpmaximobolsas, (SELECT count(*) FROM sispacto2.mensario WHERE iusd=i.iusd) as numerobolsas
					FROM sispacto2.identificacaousuario i
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
					INNER JOIN sispacto2.pagamentoperfil p ON p.pflcod = t.pflcod
					INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.uncid = i.uncid AND rf.pflcod = t.pflcod AND rf.fpbid='".$dados['fpbid']."'
					LEFT JOIN sispacto2.pactoidadecerta pa ON pa.picid = i.picid
					LEFT JOIN workflow.documento dc ON dc.docid = pa.docidturma
					WHERE i.iusstatus='A' AND t.pflcod='".$p['pflcod']."' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' AND dc.esdid='".ESD_FECHADO_TURMA."' ELSE true END AND CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' ) ".(($wh)?" AND ".implode(" AND ", $wh):"")."

					) foo
				   	{$f}";

            $totalus = $db->pegaUm($sql, null, 3200);

            $sql = "SELECT count(*) as tot FROM sispacto2.pagamentobolsista p
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid
					WHERE p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");

            $totalpag = $db->pegaUm($sql, null, 3200);


            $sql = "SELECT count(*) as tot FROM sispacto2.pagamentobolsista p
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid
					INNER JOIN workflow.documento d ON d.docid = p.docid
					WHERE d.esdid IN('".ESD_PAGAMENTO_EFETIVADO."','".ESD_PAGAMENTO_NAO_AUTORIZADO."') AND p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");

            $totalpagef = $db->pegaUm($sql, null, 3200);

            echo '<td>'.$p['pfldsc'].'</td>';
            echo '<td align=right>'.$totalus.'</td>';

            echo '<td align=right>'.(($totalpag)?$totalpag:'0').'</td>';
            if($totalus) $porc = round(($totalpag/$totalus)*100,0);
            else $porc = 0;
            echo '<td>';
            progressBar2($porc);
            echo '</td>';

            echo '<td align=right>'.(($totalpagef)?$totalpagef:'0').'</td>';
            if($totalus) $porc = round(($totalpagef/$totalus)*100,0);
            else $porc = 0;
            echo '<td>';
            progressBar2($porc);
            echo '</td>';


            echo '</tr>';

        }
    }
    echo '</table>';
}

function progressBar2($percentage) {

    global $db;

    $percentage = round($percentage,0);

    $percentage = $percentage > 100 ? 100 : $percentage;

    if($percentage==100) {
        $color = "#0000FF";
        print "<center><font color={$color}>Concluído</font></center>";
    } elseif($percentage==0) {
        $color = "#FF0000";
        print "<center><font color={$color}>Não iniciado</font></center>";
    } else {
        $color = "#215E21";
        print "<center><font color={$color}>Em Andamento</font></center>";
    }

    print "<div id=\"progress-bar\" style=\"-webkit-border-radius: 5px;-moz-border-radius: 5px;border-radius: 5px;width: 100px;margin: 0 auto;background: #cccccc;border: 3px solid #f2f2f2;\">\n";
    print "<div id=\"progress-bar-percentage\" class=\"all-rounded\" style=\"background: $color;padding: 1px 0px;color: #FFF;font-weight: bold;text-align: center;width: $percentage%\">";
    if ($percentage > 10) {
        print "&nbsp;$percentage%";
        print "</div></div>";
    } else {
        print "<div style=\"display: block;\">&nbsp;</div><div style=\"position:absolute;color:$color;margin-top:-14px;margin-left:".($percentage+10)."px;\" >$percentage%</div>";
        print "</div></div>";
    }

}
?>