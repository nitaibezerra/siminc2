<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_ensinosuperior1'])){
    gerarGraficoEnsinoSuperior1($_REQUEST['detalhe_ensinosuperior1']);
    die;
}
if(isset($_REQUEST['detalhe_ensinosuperior2'])){
    gerarGraficoEnsinoSuperior2($_REQUEST['detalhe_ensinosuperior2']);
    die;
}
if(isset($_REQUEST['detalhe_ensinosuperior3'])){
    gerarGraficoEnsinoSuperior3($_REQUEST['detalhe_ensinosuperior3']);
    die;
}
if(isset($_REQUEST['detalhe_ensinosuperior4'])){
    gerarGraficoEnsinoSuperior4($_REQUEST['detalhe_ensinosuperior4']);
    die;
}
if(isset($_REQUEST['detalhe_ensinosuperior5'])){
    gerarGraficoEnsinoSuperior5($_REQUEST['detalhe_ensinosuperior5']);
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
            jQuery('.filtrar_ensinosuperior1').click(function(){
                jQuery('#div_ensinosuperior1').load('estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A&detalhe_ensinosuperior1='+jQuery(this).val());
            });
            jQuery('.filtrar_ensinosuperior2').click(function(){
                jQuery('#div_ensinosuperior2').load('estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A&detalhe_ensinosuperior2='+jQuery(this).val());
            });
            jQuery('.filtrar_ensinosuperior3').click(function(){
                jQuery('#div_ensinosuperior3').load('estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A&detalhe_ensinosuperior3='+jQuery(this).val());
            });
            jQuery('.filtrar_ensinosuperior4').click(function(){
                jQuery('#div_ensinosuperior4').load('estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A&detalhe_ensinosuperior4='+jQuery(this).val());
            });
            jQuery('.filtrar_ensinosuperior5').click(function(){
                jQuery('#div_ensinosuperior5').load('estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A&detalhe_ensinosuperior5='+jQuery(this).val());
            });
        });

        function abreMapaMaisMedicos(){
            var url = "/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=32";
            window.open(url,'Mapa','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
        }
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
                <div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" >Expansão da Educação Superior</div></td>
    </tr>
    <tr>
        <td width="13%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_rede_federal_es&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Expansão das<br>Universidades<br>Federais'); ?>
            </div>
        </td>
        <td width="12%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Ciência<br>sem<br>Fronteiras'); ?>
            </div>
        </td>
        <td width="11%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_medicos_principal&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Mais Médicos'); ?>
            </div>
        </td>
        <td width="14%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_prouni&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Prouni -<br>Programa<br>Universidade<br>para todos'); ?>
            </div>
        </td>

        <td width="13%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_sisu&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Sisu -<br>Sistema<br>de Seleção<br>Unificada'); ?>
            </div>
        </td>
        <td width="15%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_fies&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'FIES -<br>Programa de<br>Financiamento<br>Estudantil'); ?>
            </div>
        </td>
        <td width="12%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_uab&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Universidade<br>Aberta<br>do Brasil'); ?>
            </div>
        </td>
        <td width="10%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_posgraduacao&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Pós-<br>Graduação'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="50%" colspan="4">
            <div class="link" onclick="abreIndicadorPopUp(439);">
                <?php exibirTitulo('indicador', 'Matrículas em graduação'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_ensinosuperior1" name="ensinosuperior1" value="1" checked="checked" onclick="" />Por dependência administrativa
                <input type="radio" class="filtrar_ensinosuperior1" name="ensinosuperior1" value="2" onclick="" />Por modalidade
            </div>
            <br>
            <div id="div_ensinosuperior1">
                <?php gerarGraficoEnsinoSuperior1(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="50%" colspan="4">
            <div class="link" onclick="abreIndicadorPopUp(674);">
                <?php exibirTitulo('indicador', 'Vagas em graduação'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_ensinosuperior2" name="ensinosuperior2" value="1" checked="checked" onclick="" />Por dependência administrativa
                <input type="radio" class="filtrar_ensinosuperior2" name="ensinosuperior2" value="2" onclick="" />Por área do conhecimento
            </div>
            <br>
            <div id="div_ensinosuperior2">
                <?php gerarGraficoEnsinoSuperior2(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="50%" colspan="4">
            <div class="link" onclick="abreIndicadorPopUp(675);">
                <?php exibirTitulo('indicador', 'Número de cursos de graduação'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_ensinosuperior3" name="ensinosuperior3" value="1" checked="checked" onclick="" />Por dependência administrativa
                <input type="radio" class="filtrar_ensinosuperior3" name="ensinosuperior3" value="2" onclick="" />Por área do conhecimento
            </div>
            <br>
            <div id="div_ensinosuperior3">
                <?php gerarGraficoEnsinoSuperior3(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="50%" colspan="4">
            <div class="link" onclick="abreIndicadorPopUp(441);">
                <?php exibirTitulo('indicador', 'Concluintes de graduação'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_ensinosuperior4" name="ensinosuperior4" value="1" checked="checked" onclick="" />Por dependência administrativa
                <input type="radio" class="filtrar_ensinosuperior4" name="ensinosuperior4" value="2" onclick="" />Por modalidade
            </div>
            <br>
            <div id="div_ensinosuperior4">
                <?php gerarGraficoEnsinoSuperior4(1); ?>
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao link" onclick="abreRedeFederal('S');" width="33%">
            <div>
                <?php exibirTitulo('mapas', 'Mapas das<br/> Universidades', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select
					count(mun.muncod) as qtde
					from entidade.entidade entcampus
					inner join entidade.endereco edc ON edc.entid = entcampus.entid
					LEFT JOIN territorios.municipio mun ON edc.muncod = mun.muncod
					LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
					LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod
					inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
					inner join entidade.funentassoc a on a.fueid = fn.fueid
					inner join entidade.entidade entinst on entinst.entid = a.entid
					inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
					inner join academico.campus cmp on cmp.entid = entcampus.entid
					inner join academico.existencia exi on exi.exiid = cmp.exiid
					left join academico.entidadedetalhe edt on edt.entid = entcampus.entid
					WHERE exi.exidsc IN ('Preexistentes','Criados em 2003/2010','Novos 2011/2012','Novos 2013/2014');";
            $totalMun = $db->pegaUm($sql, null, 3200 );
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
                    </td>
                    <td class="link" valign="bottom" onclick="abreRedeFederal('S');">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(382);" width="34%">
            <?php
            $sql = "select count(distinct fn.entid) as campus, count(distinct a.entid) as universidades
					from entidade.funcaoentidade fn
					inner join entidade.funentassoc a on a.fueid = fn.fueid
					where fn.fuestatus = 'A'
					and fn.entid in (
						select
							distinct dsh.entid
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join entidade.entidade entcampus ON entcampus.entid = dsh.entid
						where sh.indid in (382)
						and sh.sehstatus <> 'I'
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and dsh.tidid1 = 365
						and dsh.tidid2 NOT IN (2589) --INCORPORADO
					)";
            $arrDados = $db->pegaLinha($sql);
            $valorMetaCampus = 321;
            $porcentagemCampus = number_format(($arrDados['campus']/$valorMetaCampus)*100,0,",",".");
            $horizontalCampus = $porcentagemCampus * 2;
            $valorMetaUniversidades = 63;
            $porcentagemUniversidades = number_format(($arrDados['universidades']/$valorMetaUniversidades)*100,0,",",".");
            $horizontalUniversidades = $porcentagemUniversidades * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td>
                        <?php exibirTitulo('alvo', 'Metas e Execução 2014'); ?>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <?php echo number_format($valorMetaUniversidades,0,",",".") ?> Universidades
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalUniversidades ?>px;'><?= $porcentagemUniversidades ?>%&nbsp;</div>
                        </div>
                        <?php
                        if($arrDados['universidades'] < $valorMetaUniversidades){
                            echo number_format($arrDados['universidades'],0,",",".") . " Universidades";
                        }
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <?php echo number_format($valorMetaCampus,0,",",".") ?> Câmpus
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalCampus ?>px;'><?= $porcentagemCampus ?>%&nbsp;</div>
                        </div><?php echo number_format($arrDados['campus'],0,",",".") ?> Funcionando
                    </td>
                </tr>
            </table>
            <br>
            <br>
            <?php
            $sql = "SELECT
                            SUM(expansao) AS totalexpansao,
                            SUM(funcionando) AS totalfuncionando,
                            SUM(provisoria) AS totalprovisoria,
                            SUM(definitiva) AS totaldefinitiva
                        FROM (
                            SELECT
                                1 AS expansao,
                                CASE
                                    WHEN cmp.cmpsituacao = 'F' THEN 1
                                    ELSE 0
                                END AS funcionando,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'P' THEN 1
                                    ELSE 0
                                END AS provisoria,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'D' THEN 1
                                    ELSE 0
                                END AS definitiva
                            FROM entidade.entidade entcampus
                            INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                            INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                            INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                            INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                            INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                            INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                            WHERE cmp.exiid IN (3, 5)
                        ) AS foo";
            $arrDados = $db->pegaLinha($sql);
            $valorMetaCampus = $arrDados['totalexpansao'];
            $porcentagemCampus = number_format(($arrDados['totalfuncionando']/$valorMetaCampus)*100,0,",",".");
            $horizontalCampus = $porcentagemCampus * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td>
                        <?php exibirTitulo('alvo', 'Expansão 2011/2014'); ?>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <?php echo number_format($valorMetaCampus,0,",",".") ?> Câmpus
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalCampus ?>px;'><?= $porcentagemCampus ?>%&nbsp;</div>
                        </div><?php echo number_format($arrDados['totalfuncionando'],0,",",".") ?> Funcionando</td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'SISU', '<span class="link" onclick="abreIndicadorPopUp(1401);">Inscritos</span>, <span class="link" onclick="abreIndicadorPopUp(1435);">Inscrições</span>'); ?>
            </div>
            <?php
            $sql = "SELECT COUNT(DISTINCT dshcod)
					FROM painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where sh.indid in (1435)
					and sh.sehstatus <> 'I'";
            $totalIES = $db->pegaUm( $sql, null, 3200 );

            $sql = "select periodo, situacao, valor
					from(
						select
							replace(dpedsc, 'º Semestre', '') as periodo,
							'Inscritos' as situacao,
							sum(dsh.dshqtde) as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (1401)
						and sh.sehstatus <> 'I'
						and dpe.dpeordem = 1
						group by periodo
					union all
						select
							replace(dpedsc, 'º Semestre', '') as periodo,
							'Inscrições' as situacao,
							sum(dsh.dshqtde) as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (1435)
						and sh.sehstatus <> 'I'
						and dpe.dpeordem = 1
						group by periodo
					) as foo
					order by situacao desc, periodo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'situacao', 'periodo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaSisu', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
            <table align="center">
                <tr>
                    <td class="titulo_box bold"><?=$totalIES?> IES já participaram</td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreMapaMaisMedicos();">
            <div>
                <?php exibirTitulo('mapas', 'Mais Médicos'); ?>
            </div>
            <?php
            $sql = "select
						count(distinct uni.uniid) as ies,
						count(distinct unm.muncod) as muncod,
						(select count(distinct tut.tutid) from maismedicos.universidade uni_tut inner join maismedicos.tutor tut ON tut.uniid = uni_tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true where uni_tut.unistatus = 'A') as tutores,
						(select count(distinct sup.tutid) from maismedicos.universidade uni_sup inner join maismedicos.tutor sup ON sup.uniid = uni_sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true where uni_sup.unistatus = 'A') as supervisores,
						count(distinct mdc.mdcid) as medicos
					from
						maismedicos.universidade uni
					left join
						maismedicos.universidademunicipio unm  ON uni.uniid = unm.uniid
					left join
						territoriosgeo.municipio mun ON mun.muncod = uni.muncod
					left join
						territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
					inner join
						territorios.estado e ON e.estuf = mun2.estuf
					inner join
						  maismedicos.medico mdc ON mdc.muncod = unm.muncod
					where
						unistatus = 'A' and mdcstatus = 'A'";
            $arrValores = $db->pegaLinha($sql,null,3200);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['ies'],0,",",".") ?> </span><span class="subtitulo_box" >IES</span><br />
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['tutores'],0,",",".") ?> </span><span class="subtitulo_box" >Tutores</span><br />
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['supervisores'],0,",",".") ?> </span><span class="subtitulo_box" >Supervisores</span><br />
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['muncod'],0,",",".") ?> </span><span class="subtitulo_box" >Municípios</span><br />
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['medicos'],0,",",".") ?> </span><span class="subtitulo_box" >Médicos</span>
                    </td>
                    <td class="link" valign="bottom" onclick="abreMapaMaisMedicos();">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
            </table>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('executive', 'FIES', '<span class="link" onclick="abreIndicadorPopUp(86);">Contratos firmados até 2009</span>, <span class="link" onclick="abreIndicadorPopUp(778);">Contratos firmados a partir de 2010</span>'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde)
					FROM painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where sh.indid in (778)
					and sh.sehstatus <> 'I'";
            $totalContratos = $db->pegaUm( $sql, null, 3200 );

            $sql = "select periodo, modalidade, total
					from(
						select 'Até 2009<b>*</b>' as periodo, 'Modalidade não identificada' as modalidade, sum(dsh.dshqtde) as total, '2009' as dpeanoref
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (86)
						and sh.sehstatus <> 'I'
					union all
						select dpe.dpeanoref as periodo, tid1.tiddsc as modalidade, sum(dsh.dshqtde) as total, dpe.dpeanoref
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						where sh.indid in (778)
						and sh.sehstatus <> 'I'
						group by dpe.dpeanoref, modalidade
					) as foo
					order by dpeanoref, modalidade";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'modalidade', 'total');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.percentage, 2, ',', '.') + '%</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaSituacao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
            <table>
                <tr>
                    <td><b>*</b>Operacionalizado pela CAIXA</td>
                </tr>
                <tr>
                    <td class="titulo_box bold center">Total de contratos firmados Novo FIES: <?=number_format($totalContratos, 0, ',', '.') ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'PROUNI', '<span class="link" onclick="abreIndicadorPopUp(2893);">Bolsas ocupadas - Cor/raça e gênero</span>, <span class="link" onclick="abreIndicadorPopUp(2892);">Bolsas ativas - Por situação</span>'); ?>
            </div>
            <?
            $sql = "select to_char(MAX(sehdtcoleta), 'DD/MM/YYYY') AS ultimaatualizacao, sum(dsh.dshqtde)::integer as total
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (2892)
					and sh.sehstatus <> 'I'";
            $arrTotalBolsas = $db->pegaLinha( $sql, null, 3200 );

            $sql = "select
						(SELECT MAX(dpe.dpeanoref) FROM painel.seriehistorica sh inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid WHERE sh.indid = 2893) as periodo,
						tid1.tiddsc AS situacao,
						sum(dsh.dshqtde)::integer as total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    where sh.indid in (2893)
                    and sh.sehstatus <> 'I'
                    GROUP BY situacao";
            $arrDadosBarra = $db->carregar( $sql, null, 3200 );
            if($arrDadosBarra){
                $aBarra = array();
                foreach ($arrDadosBarra as $dados) {
                    $aBarra[$dados['periodo']][$dados['situacao']] = $dados['total'];
                }
                $dados = $categorias = '';

                $sql = "select dpe.dpeanoref as periodo, sum(dsh.dshqtde)::integer as total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        where sh.indid in (2893)
                        and sh.sehstatus <> 'I'
                        group by dpe.dpeanoref
                        order by dpe.dpeanoref";
                $dados = $db->carregar( $sql, null, 3200 );
                $arrX = array();
                $arrY = array();
                foreach ($dados as $dado)
                {
                    $acumulaBolsas += $dado['total'];
                    $arrX[] = $dado['periodo'];
                    $arrY[] = $acumulaBolsas;

                    //if($dado['periodo'] == 2014){
                    $arrBranca[]  = isset($aBarra[$dado['periodo']]['Branca']) ? (int)$aBarra[$dado['periodo']]['Branca'] : 0;
                    $arrPreta[]  = isset($aBarra[$dado['periodo']]['Preta']) ? (int)$aBarra[$dado['periodo']]['Preta'] : 0;
                    $arrAmarela[]  = isset($aBarra[$dado['periodo']]['Amarela']) ? (int)$aBarra[$dado['periodo']]['Amarela'] : 0;
                    $arrNaoInformada[]  = isset($aBarra[$dado['periodo']]['Não Informada']) ? (int)$aBarra[$dado['periodo']]['Não Informada'] : 0;
                    $arrIndigena[]  = isset($aBarra[$dado['periodo']]['Indígena']) ? (int)$aBarra[$dado['periodo']]['Indígena'] : 0;
                    $arrParda[]  = isset($aBarra[$dado['periodo']]['Parda']) ? (int)$aBarra[$dado['periodo']]['Parda'] : 0;
                    // } else {
                    $arrSuspensas[]  = 0;
                    $arrUtilizacao[] = 0;
                    // }
                }

                $aDados = array(
                    array('type' => 'column', 'name' => 'Branca', 'data'=>$arrBranca, 'stacking' => 'normal'),
                    array('type' => 'column', 'name' => 'Preta', 'data'=>$arrPreta, 'stacking' => 'normal'),
                    array('type' => 'column', 'name' => 'Amarela', 'data'=>$arrAmarela, 'stacking' => 'normal'),
                    array('type' => 'column', 'name' => 'Nao Informada', 'data'=>$arrNaoInformada, 'stacking' => 'normal'),
                    array('type' => 'column', 'name' => 'Indigena', 'data'=>$arrIndigena, 'stacking' => 'normal'),
                    array('type' => 'column', 'name' => 'Parda', 'data'=>$arrParda, 'stacking' => 'normal'),
                    array('type' => 'spline', 'name' => 'Bolsas', 'data'=>$arrY, 'color'=>'blue'),
                );

                echo geraGraficoLinha($aDados, $arrX, "grafico2", "Bolsas ocupadas no PROUNI",200);
                echo "<br><span class='numero titulo_box'>Total de bolsas ativas do PROUNI em ". $arrTotalBolsas['ultimaatualizacao'] .": ". number_format($arrTotalBolsas['total'],0,",",".") . "</span>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(547);">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas ativas na Universidade<br>Aberta do Brasil/UAB'); ?>
            </div>
            <?
            $sql = "SELECT
						tid1.tiddsc AS descricao,
						sum(dsh.dshqtde) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid in (547)
					AND sh.sehstatus <> 'I'
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					GROUP BY descricao
					ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaUAB", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(572);">
                <?php exibirTitulo('indicador', 'Matrículas de pós-graduação'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_ensinosuperior5" name="ensinosuperior5" value="1" checked="checked" onclick="" />Por dependência administrativa
                <input type="radio" class="filtrar_ensinosuperior5" name="ensinosuperior5" value="2" onclick="" />Por modalidade
            </div>
            <br>
            <div id="div_ensinosuperior5">
                <?php gerarGraficoEnsinoSuperior5(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1561);">
            <?php
            $sql = "select to_char(max(sehdtcoleta),'DD/MM/YYYY') as dataatualizacao from painel.seriehistorica where indid = 1561";
            $dataatualizacao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('alvo', 'Ciência sem Fronteiras<br>Bolsas concedidas por modalidade e ano', 'Atualizado em ' . $dataatualizacao); ?>
            </div>
            <?php
            $soma2011 = 0;
            $soma2012 = 0;
            $soma2013 = 0;
            $soma2014 = 0;
            $sql = "select
                        tid.tiddsc as tipo,
                        dpe.dpeanoref as ano,
                        sum(dsh.dshqtde) as total
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
                    where i.indid in (1561,2591)
                    and sh.sehstatus <> 'I'
                    and dpe.dpeanoref between '2011' and '2014'
                    group by tipo, ano
                    order by tipo, ano";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrTipo[$dado['tipo']][$dado['ano']]['total'][] = $dado['total'];
                }
            }
            ?>
            <table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Modalidade de Bolsa</td>
                    <th class="center bold" >2011</th>
                    <th class="center bold" >2012</th>
                    <th class="center bold" >2013</th>
                    <th class="center bold" >2014</th>
                    <th class="center bold" >Total</th>
                </tr>
                <?php
                $count = -1;
                if($arrDados){
                    foreach($arrTipo as $chave => $onb):
                        $total2011 = is_array($onb['2011']['total'])?array_sum($onb['2011']['total']):0;
                        $total2012 = is_array($onb['2012']['total'])?array_sum($onb['2012']['total']):0;
                        $total2013 = is_array($onb['2013']['total'])?array_sum($onb['2013']['total']):0;
                        $total2014 = is_array($onb['2014']['total'])?array_sum($onb['2014']['total']):0;
                        $count++;
                        ?>
                        <tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?>>
                            <td class="" ><?=$chave ?></td>
                            <td class="numero" ><?=number_format($total2011,0,",",".") ?></td>
                            <td class="numero" ><?=number_format($total2012,0,",",".") ?></td>
                            <td class="numero" ><?=number_format($total2013,0,",",".") ?></td>
                            <td class="numero" ><?=number_format($total2014,0,",",".") ?></td>
                            <td class="numero" ><?=number_format($total2011+$total2012+$total2013+$total2014,0,",",".") ?></td>
                        </tr>
                        <?php
                        $soma2011 += is_array($onb['2011']['total'])?array_sum($onb['2011']['total']):0;
                        $soma2012 += is_array($onb['2012']['total'])?array_sum($onb['2012']['total']):0;
                        $soma2013 += is_array($onb['2013']['total'])?array_sum($onb['2013']['total']):0;
                        $soma2014 += is_array($onb['2014']['total'])?array_sum($onb['2014']['total']):0;
                    endforeach;
                    $somaTotal = $soma2011 + $soma2012 + $soma2013 + $soma2014;
                }
                ?>
                <tr>
                    <th class="bold"  >Total</th>
                    <th class="numero bold"  ><?php echo number_format($soma2011,0,",",".")?></th>
                    <th class="numero bold"  ><?php echo number_format($soma2012,0,",",".")?></th>
                    <th class="numero bold"  ><?php echo number_format($soma2013,0,",",".")?></th>
                    <th class="numero bold"  ><?php echo number_format($soma2014,0,",",".")?></th>
                    <th class="numero bold"  ><?php echo number_format($somaTotal,0,",",".")?></th>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('pe',3,6,12,'');" align="center">
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2019);">
            <div>
                <?php exibirTitulo('casas', 'Bolsa Permanência'); ?>
            </div><br>
            <?php
            $sql = "select tipo, sum(valor) as valor, datainicio, datafim
					from(
						select
							'Recursos repassado às IES' as tipo,
							SUM(dshvalor) as valor,
							to_char(min(dpedatainicio),'MM/YYYY') AS datainicio,
							to_char(MAX(dpe.dpedatafim),'MM/YYYY') AS datafim
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (2019)
						and sh.sehstatus <> 'I'
					union all
						select
							'Bolsas autorizadas' as tipo,
							sum(dshqtde) as valor,
							--'' AS datainicio,
							to_char(min(dpedatainicio),'MM/YYYY') AS datainicio,
							to_char(MAX(dpe.dpedatafim),'MM/YYYY') AS datafim
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (2019)
						and sh.sehstatus <> 'I'
						--and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					union all
						select
							'Bolsas ativas' as tipo,
							sum(dshqtde) as valor,
							--'' AS datainicio,
							to_char(min(dpedatainicio),'MM/YYYY') AS datainicio,
							to_char(MAX(dpe.dpedatafim),'MM/YYYY') AS datafim
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (2019)
						and sh.sehstatus <> 'I'
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					) as foo
					group by tipo, datainicio, datafim
					order by tipo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
            ?>
            <table class="" cellpadding="5" cellspacing="4" width="80%" align="center" border="1" >
                <?php
                foreach($arrDados as $dado):
                    if($dado['tipo']=='Bolsas autorizadas'){
                        //$periodo = 'De '. $dado['datafim'];
                        $periodo = 'De '. $dado['datainicio'] . ' a ' . $dado['datafim'];
                        $total = number_format($dado['valor'],0,',','.');
                    }elseif($dado['tipo']=='Recursos repassado às IES'){
                        $periodo = 'De '. $dado['datainicio'] . ' a ' . $dado['datafim'];
                        $total = 'R$'.number_format($dado['valor'],2,',','.');
                    }elseif($dado['tipo']=='Bolsas ativas'){
                        $periodo = $dado['datainicio'];
                        $total = number_format($dado['valor'],0,',','.');
                    }
                    ?>
                    <tr>
                        <td class="titulo_box bold" width="60%"><?=$dado['tipo']?><br><span class="subtitulo_box" ><?=$periodo?></span></td>
                        <td class="titulo_box bold numero"><?=$total?></td>
                    </tr>
                <?php endforeach; ?>
            </table>

            <?php
            }
            $arrGrupo = array();
            $sql = "select tid2.tiddsc as grupo, sum(dshqtde)::integer as total
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (2019)
					and sh.sehstatus <> 'I'
					--and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					AND dsh.tidid2 IN (4610, 4611)
					GROUP BY grupo
					ORDER BY grupo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrGrupo[$dado['grupo']]['total'] = $dado['total'];
                }
            }
            ?>
            <table class="" cellpadding="5" cellspacing="4" width="80%" align="center" border="0" >
                <tr>
                    <td class="titulo_box bold" width="100%"><?=number_format($arrGrupo['Indígena']['total'],0,",",".") ?> indígenas e <?=number_format($arrGrupo['Quilombola']['total'],0,",",".")?> quilombolas</td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('casas', 'PNAES'); ?>
            </div><br>
            <?php
            $sql = "select id, tipo, sum(valor) as valor, datainicio, datafim
					from(
						select
							331 as id,
							'Benefícios distribuídos' as tipo,
							SUM(dshqtde) as valor,
							to_char(min(dpedatainicio),'YYYY') AS datainicio,
							to_char(MAX(dpe.dpedatafim),'YYYY') AS datafim
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (331)
						and sh.sehstatus <> 'I'
					union all
						select
							780 as id,
							'Recursos repassado às IES' as tipo,
							sum(dshqtde) as valor,
							'' AS datainicio,
							to_char(MAX(dpe.dpedatafim),'YYYY') AS datainicio
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where sh.indid in (780)
						and sh.sehstatus <> 'I'
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					) as foo
					group by id, tipo, datainicio, datafim
					order by tipo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
            ?>
            <table class="" cellpadding="5" cellspacing="4" width="80%" align="center" border="1" >
                <?php
                foreach($arrDados as $dado):
                    if($dado['tipo']=='Benefícios distribuídos'){
                        $periodo = 'De '. $dado['datainicio'] . ' a ' . $dado['datafim'];
                        $total = number_format($dado['valor'],0,',','.');
                    }else{
                        $periodo = 'De '. $dado['datafim'];
                        $total = 'R$'.number_format($dado['valor'],2,',','.');
                    }
                    ?>
                    <tr class="link" onclick="abreIndicadorPopUp(<?=$dado['id']?>)";>
					<td class="titulo_box bold" width="60%"><?=$dado['tipo']?><br><span class="subtitulo_box" ><?=$periodo?></span></td>
					<td class="titulo_box bold numero"><?=$total?></td>
				</tr>
			<?php endforeach; ?>
            </table>
            <?php } ?>
        </td>
    </tr>
    <!-- INÍCIO DO FINANCEIRO -->
    <?php
    // Parâmetros para a nova conexão com o banco do SIAFI
    $servidor_bd = $servidor_bd_siafi;
    $porta_bd    = $porta_bd_siafi;
    $nome_bd     = $nome_bd_siafi;
    $usuario_db  = $usuario_db_siafi;
    $senha_bd    = $senha_bd_siafi;
    // Cria o novo objeto de conexão
    $db2 = new cls_banco();
    // Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
    $dados_conexao = array(
        'servidor_bd' => $servidor_bd_siafi,
        'porta_bd' => $porta_bd_siafi,
        'nome_bd' => $nome_bd_siafi,
        'usuario_db' => $usuario_db_siafi,
        'senha_bd' => $senha_bd_siafi
    );
    ?>
    <tr>
        <td class="fundo_padrao">
            <?php
            $sql = "SELECT
                            ano,
                            sum(valor1) AS empenhado,
                            sum(valor2) AS dotacao,
                            sum(valor3) AS pago
                        FROM (
                            SELECT
                                2011 as ano,
                                CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor1,
                                CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor2,
                                CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor3
                            FROM
                                dw.saldo2011 sld
                            --WHERE substr(sld.plicod, 2, 4) in ('SS09','SS16')
                            WHERE sld.acacod in ('8282')
                            AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
                        UNION ALL
                            SELECT
                                2012 as ano,
                                CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor1,
                                CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor2,
                                CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor3
                            FROM
                                dw.saldo2012 sld
                            --WHERE substr(sld.plicod, 2, 4) in ('SS09','SS16','SS25')
                            WHERE sld.acacod in ('8282')
                            AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
                        UNION ALL
                            SELECT
                                2013 as ano,
                                CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor1,
                                CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor2,
                                CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor3
                            FROM
                                dw.saldo2013 sld
                            --WHERE substr(sld.plicod, 2, 4) in ('SS09','SS16','SS25','SS31')
                            WHERE sld.acacod in ('8282')
                            AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
                        ) as foo
                        GROUP BY ano
                        ORDER BY ano";
            $arrDados = $db2->carregar($sql,null,3200);
            $total_dotacao = 0;
            $total_empenhado = 0;
            $total_pago = 0;
            ?>
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário/Financeiro<br>REUNI'); ?>
            </div>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Ano</th>
                    <th class="center bold" >Empenhado</th>
                    <!--<th class="center bold" >Pago</th>-->
                </tr>
                <?php
                $count = -1;
                foreach($arrDados as $dado):
                    $count++;
                    ?>
                    <tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&acacod[0]=8282&alterar_ano=0')" >
                        <td><?php echo $dado['ano'] ?></td>
                        <td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
                        <!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
                    </tr>
                <?php endforeach; ?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
                    <!--<th class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></th>-->
                </tr>
            </table>
        </td>
        <td class="fundo_padrao">
            <?php
            $sql = "SELECT
                            ano,
                            sum(valor1) AS empenhado,
                            sum(valor2) AS dotacao,
                            sum(valor3) AS pago
                        FROM (
                            SELECT
                                2011 as ano,
                                CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor1,
                                CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor2,
                                CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor3
                            FROM
                                dw.saldo2011 sld
                            --WHERE substr(sld.plicod, 2, 4) in ('SS07')
                            WHERE sld.unicod in ('74902') AND sld.acacod in ('0579')
                            AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
                        UNION ALL
                            SELECT
                                2012 as ano,
                                CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor1,
                                CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor2,
                                CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor3
                            FROM
                                dw.saldo2012 sld
                            --WHERE substr(sld.plicod, 2, 4) in ('SS07')
                            WHERE sld.unicod in ('74902') AND sld.acacod in ('00IG')
                            AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
                        UNION ALL
                            SELECT
                                2013 as ano,
                                CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor1,
                                CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor2,
                                CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
                                    CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
                                ELSE 0
                                END AS valor3
                            FROM
                                dw.saldo2013 sld
                            --WHERE substr(sld.plicod, 2, 4) in ('SS07')
                            WHERE sld.unicod in ('74902') AND sld.acacod in ('00IG')
                            AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
                        ) as foo
                        GROUP BY ano
                        ORDER BY ano";
            $arrDados = $db2->carregar($sql,null,3200);
            $total_dotacao = 0;
            $total_empenhado = 0;
            $total_pago = 0;
            ?>
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário/Financeiro<br>FIES'); ?>
            </div>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Ano</th>
                    <th class="center bold" >Empenhado</th>
                    <!--<th class="center bold" >Pago</th>-->
                </tr>
                <?php
                $count = -1;
                foreach($arrDados as $dado):
                    $count++;
                    if($dado['ano']=='2011'){ ?>
                        <tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&acacod[0]=0579&uo[0]=74902&alterar_ano=0')" >
                    <?php }else{ ?>
                        <tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&acacod[0]=00IG&uo[0]=74902&alterar_ano=0')" >
                    <?php } ?>
                    <td><?php echo $dado['ano'] ?></td>
                    <td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
                    <!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
                    </tr>
                <?php endforeach; ?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
                    <!--<th class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></th>-->
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2290);">
            <div>
                <?php exibirTitulo('financeiro', 'Renúncia Fiscal - PROUNI'); ?>
            </div>
            <?php
            $sql="select
                        case
                            when dpe.dpeanoref between '2005' and '2012' then 'De 2005 a 2012: Renúncia efetiva'
                            when dpe.dpeanoref between '2013' and '2015' then 'De 2013 a 2015: Renúncia estimada'
                        end as periodo,
                        sum(dsh.dshqtde) as total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (2290)
                    and sh.sehstatus <> 'I'
                    group by periodo
                    order by periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Ano</th>
                    <th class="center bold" >Reais</th>
                </tr>
                <?php
                $count = -1;
                foreach($arrDados as $dado):
                    $count++;?>
                    <tr>
                        <td><?php echo $dado['periodo'] ?></td>
                        <td class="numero" ><?php $valorTotal+=$dado['total']; echo number_format($dado['total'],2,",",".") ?></td>
                    </tr>
                <?php endforeach; ?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero bold" ><?php echo number_format($valorTotal,2,",",".") ?></th>
                </tr>
            </table>
            <?php } ?>
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
function gerarGraficoEnsinoSuperior1($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "tid2.tiddsc AS situacao";
    }else{
        $campo = "tid1.tiddsc AS situacao";
    }

    $sql = "SELECT
                dpeanoref AS acao,
                $campo,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (439)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, situacao
            ORDER BY acao, situacao";
    $dados = $db->carregar( $sql, null, 3200 );
    if($dados){
        $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaGraduacao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
    }
}
function gerarGraficoEnsinoSuperior2($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "tid1.tiddsc AS situacao";
    }else{
        $campo = "tid2.tiddsc AS situacao";
    }

    $sql = "SELECT
                dpeanoref AS acao,
                $campo,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (674)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, situacao
            ORDER BY acao, situacao";
    $dados = $db->carregar( $sql, null, 3200 );
    if($dados){
        $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaVagasGraduacao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
    }
}
function gerarGraficoEnsinoSuperior3($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "tid1.tiddsc AS situacao";
    }else{
        $campo = "tid2.tiddsc AS situacao";
    }

    $sql = "SELECT
                dpeanoref AS acao,
                $campo,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (675)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, situacao
            ORDER BY acao, situacao";
    $dados = $db->carregar( $sql, null, 3200 );
    if($dados){
        $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaCursosGraduacao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
    }
}
function gerarGraficoEnsinoSuperior4($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "tid2.tiddsc AS situacao";
    }else{
        $campo = "tid1.tiddsc AS situacao";
    }

    $sql = "SELECT
                dpeanoref AS acao,
                $campo,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (441)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, situacao
            ORDER BY acao, situacao";
    $dados = $db->carregar( $sql, null, 3200 );
    if($dados){
        $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaConcluintesGraduacao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
    }
}
function gerarGraficoEnsinoSuperior5($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "tid1.tiddsc AS situacao";
    }else{
        $campo = "tid2.tiddsc AS situacao";
    }

    $sql = "SELECT
                dpeanoref AS acao,
                $campo,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (572)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, situacao
            ORDER BY acao, situacao";
    $dados = $db->carregar( $sql, null, 3200 );
    if($dados){
        $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
    }
}
?>