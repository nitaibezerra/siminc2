<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_alunos'])){
    gerarGraficoBolsasOfertadas($_REQUEST['detalhe_alunos']);
    die;
}
if(isset($_REQUEST['detalhe_escolas'])){
    gerarGraficoEscolasAderiram($_REQUEST['detalhe_escolas']);
    die;
}
if(isset($_REQUEST['detalhe_estados'])){
    gerarGraficoPorEstado($_REQUEST['detalhe_estados']);
    die;
}
if(isset($_REQUEST['detalhe_censo'])){
    gerarGraficoMatriculaIntegral($_REQUEST['detalhe_censo']);
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
            jQuery('.div_fotos_interno').cycle({
                fx: 'scrollDown'
            });
            jQuery('.filtrar_detalhe').click(function(){
                jQuery('#div_detalhe_alunos').load('estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A&detalhe_alunos='+jQuery(this).val());
            });
            jQuery('.filtrar_escolas').click(function(){
                jQuery('#div_detalhe_escolas').load('estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A&detalhe_escolas='+jQuery(this).val());
            });
            jQuery('.filtrar_estados').click(function(){
                jQuery('#div_detalhe_estados').load('estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A&detalhe_estados='+jQuery(this).val());
            });
            jQuery('.filtrar_censo').click(function(){
                jQuery('#div_detalhe_censo').load('estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A&detalhe_censo='+jQuery(this).val());
            });
        });
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_maiseducacao.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
        .div_fotos_padrao2{background-color:#152D56;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:400px;margin-bottom:2px}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="5" ><div style="margin:28px" >Mais Educação</div></td>
	</tr>
	<tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1205);" width="20%">
            <div>
                <?php exibirTitulo('casas', 'Escolas<br>que aderiram<br>em 2012'); ?>
            </div>
            <?php
            $sql = "SELECT SUM(dsh.dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (1205)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref = '2012'";
            $total_beneficiadas_2012 = $db->pegaUm($sql,null,3200);
            ?>
            <div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
                <?php echo number_format($total_beneficiadas_2012,0,",",".") ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1205);" width="20%">
            <div>
                <?php exibirTitulo('casas', 'Escolas<br>que aderiram<br>em 2013'); ?>
            </div>
            <?php
            $sql = "SELECT SUM(dsh.dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (1205)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref = '2013'";
            $total_beneficiadas_2013 = $db->pegaUm($sql,null,3200);
            ?>
            <div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
                <?php echo number_format($total_beneficiadas_2013,0,",",".") ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(761);" width="20%">
            <div>
                <?php exibirTitulo('casas', 'Brasil<br>Sem Miséria', 'Escolas que aderiram em 2014'); ?>
            </div>
            <?php
            $sql = "SELECT SUM(dsh.dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (761)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref = '2014'";
            $total_adesao_2014 = $db->pegaUm($sql);
            ?>
            <div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
                <?php echo number_format($total_adesao_2014,0,",",".") ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1205);" width="20%">
            <?php
            $valorMeta = 60000;
            $sql = "select to_char(max(sehdtcoleta),'DD/MM/YYYY às HH24:MI:SS') as dataatualizacao from painel.seriehistorica where indid = 1205";
            $dataatualizacao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('alvo', 'Meta 2014: '.number_format($valorMeta,0,",",".").' escolas', 'Atualizado em ' . $dataatualizacao); ?>
            </div>
            <?php
            $sql = "SELECT SUM(dsh.dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (1205)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref = '2014'";
            $total_beneficiadas_2014 = $db->pegaUm($sql);
            $porcentagem = number_format(($total_beneficiadas_2014/$valorMeta)*100,0,",",".");
            $horizontal = $porcentagem * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'><?= $porcentagem ?>%&nbsp;</div>
                        </div><?php echo number_format($total_beneficiadas_2014,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_azul_padrao" rowspan="2" width="20%">
            <div>
                <?php exibirTitulo('configs', 'Atividades 2014'); ?>
            </div>
            <?php
            $sql = "SELECT arqid, entid, memid, entnome, mundescricao, estuf, qtde_aluno
                    from (
						select
						me.entid,
						me.memid,
						mun.estuf,
						mun.muncod,
						mun.mundescricao,
						ent.entnome,
						esd.esddsc,
						sum( map.mapquantidade) as qtde_aluno,
						(select arqid from pdeinterativo.galeriafoto gal where gal.pdeid = pde.pdeid and gfostatus = 'A' limit 1) as arqid
						from pdeescola.memaiseducacao me
						inner join entidade.entidade ent on ent.entid = me.entid
						inner join entidade.endereco ende on ende.entid = ent.entid
						inner join territorios.municipio mun on mun.muncod = ende.muncod
						inner join workflow.documento doc on doc.docid = me.docid
						inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
						inner join pdeescola.mealunoparticipante map on map.memid = me.memid
						inner join pdeinterativo.pdinterativo pde ON pde.pdicodinep::integer = ent.entcodent::integer
						where memanoreferencia = 2014 and memstatus = 'A' and esd.esdid >= 34 -- igual ou além de finalizado
						group by mun.estuf,mun.muncod,ent.entnome,me.memid,me.entid,esd.esddsc,mun.mundescricao,pde.pdeid
				    ) foo
                    WHERE arqid IS NOT NULL
                    ORDER BY random()
                    LIMIT 50";
            $arrFotos = adapterConnection::pddeinterativo()->carregar($sql);
            ?>
            <div style="clear:both" id="div_fotos_padrao2"  >
                <?php for($x=9;$x>=1;$x--): ?>
                    <div style="clear:both" class="div_fotos_interno"  >
                        <?php for($i=$x;$i<=(9+$x);$i++): ?>
                            <div class="div_fotos_padrao2" >
                                <table border="0" >
                                    <tr>
                                        <td>
                                            <img width="80" height="60" src="http://pdeinterativo.mec.gov.br/slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=pdeinterativo" />
                                        </td>
                                        <td style="color:#FFFFFF">
                                            <b><?php echo $arrFotos[$i]['entnome'] ?></b><br/>
                                            <b>Município:</b> <?php echo $arrFotos[$i]['mundescricao'] ?>/<?php echo $arrFotos[$i]['estuf'] ?><br/>
                                            <b>Alunos participantes:</b> <?php echo $arrFotos[$i]['qtde_aluno'] ?><br/>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        <?php endfor; ?>
                    </div>
                <?php endfor; ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('mais', 'Adesões 2014'); ?>
            </div>
            <?php
            $sql = "SELECT
                        situacao as doc, quantidade AS qtde
                    FROM dblink
                    ('host=
                    user=
                    password=
                    port=5432
                    dbname=',
                    'SELECT
                        case when est.esddsc is null then ''Não iniciado'' else est.esddsc end as situacao,
                        COUNT(DISTINCT me.entcodent) AS quantidade,
                        case when est.esddsc is null then 0 else est.esdordem end as ordem
                    FROM pdeescola.memaiseducacao me
                    INNER JOIN pddeinterativo.listapdeinterativo lp ON lp.pdicodinep = me.entcodent
                    INNER JOIN pddeinterativo.pdinterativo pi ON pi.pdicodinep = lp.pdicodinep
                    LEFT JOIN workflow.documento doc ON doc.docid = me.docid
                    LEFT JOIN workflow.estadodocumento as est on est.esdid = doc.esdid
                    WHERE me.memanoreferencia = 2014
                    GROUP BY situacao, ordem
                    ORDER BY ordem
                    ') AS t1(
                    situacao varchar(100),
                    quantidade integer,
                    ordem integer
                    )";
            $arrDados = $db->carregar($sql,null,3200);
            $total_adesao=0;
            $count=-1;
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="" >Situação da Adesão</th>
                    <th class="" >Escolas</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $total_adesao+=$dado['qtde'];
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['doc'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['qtde'],0,",",".") ?></td>
                        </tr>
                    <?php
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total_adesao,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(102);">
                <?php exibirTitulo('indicador', 'Alunos das escolas participantes'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_detalhe" name="detalhe" value="1" checked="checked" onclick="" />Dependência Administrativa
                <input type="radio" class="filtrar_detalhe" name="detalhe" value="2" onclick="" />Localização
            </div>
            <div id="div_detalhe_alunos">
                <?php gerarGraficoBolsasOfertadas(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <!--td class="fundo_padrao link" onclick="abreIndicadorPopUp(102);" colspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Alunos das escolas participantes (por ano)'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (102)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                }

                $aDados = array(
                    array('name' => 'Alunos', 'data'=>$arrA),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoAdesaoAlunos", "", 300, $formatoValores);
            }
            ?>
        </td-->
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(690);" colspan="5">
            <div>
                <?php exibirTitulo('indicador', 'Escolas que aderiram (por ano)'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (690)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                }

                $aDados = array(
                    array('name' => 'Escolas', 'data'=>$arrA),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                echo "<table width='90%' border='0'><tr><td>";
                echo geraGraficoLinha($aDados, $arrX, "graficoAdesaoEscolas", "", 300, $formatoValores);
                echo "</td></tr></table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1420);" colspan="5">
            <div>
                <?php exibirTitulo('indicador', 'Recursos repassados via PDDE'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (1420)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $total = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $total +=  $dado['valor'];
                    $arrTotal[] = $total;
                }

                $aDados = array(
                    array('name' => 'Recursos', 'data'=>$arrA),
                    array('name' => 'Total', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                echo "<table width='90%' border='0'><tr><td>";
                echo geraGraficoLinha($aDados, $arrX, "graficoRecursosPDDE", "", 300, $formatoValores);
                echo "</td></tr></table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2" rowspan="3">
            <div class="link" onclick="abreIndicadorPopUp(1205);">
                <?php exibirTitulo('indicador', 'Escolas que aderiram'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_escolas" name="detalhe2" value="1" checked="checked" onclick="" />Dependência Administrativa
                <input type="radio" class="filtrar_escolas" name="detalhe2" value="2" onclick="" />Localização
            </div>
            <div id="div_detalhe_escolas">
                <?php gerarGraficoEscolasAderiram(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" colspan="2" rowspan="3">
            <div class="link" onclick="abreIndicadorPopUp(2955);">
                <?php exibirTitulo('indicador', 'Censo Educação Integral'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_censo" name="detalhe4" value="1" checked="checked" onclick="" />Ensino Fundamental
                <input type="radio" class="filtrar_censo" name="detalhe4" value="2" onclick="" />Ensino Medio
                <input type="radio" class="filtrar_censo" name="detalhe4" value="3" onclick="" />Educação Infantil
            </div>
            <br>
            <br>
            <div id="div_detalhe_censo">
                <?php gerarGraficoMatriculaIntegral(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(5);?>
        </td>
    </tr>
    <tr>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,5,'','');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
    <tr>
        <!-- Tabela Processos -->
        <td class="fundo_padrao center link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/')";>
            <div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >Mapa do Processo<br/></div>
            </div>
        </td>
        <!-- Tabela Processos -->
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="4">
            <div>
                <?php exibirTitulo('mapas', 'Total por Estado', '<span class="link" onclick="abreIndicadorPopUp(1205);">Escolas</span>, <span class="link" onclick="abreIndicadorPopUp(102);">Matrículas</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_estados" name="detalhe3" value="1" checked="checked" onclick="" />Escolas
                <input type="radio" class="filtrar_estados" name="detalhe3" value="2" onclick="" />Matrículas
            </div>
            <div id="div_detalhe_estados">
                <?php gerarGraficoPorEstado(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(680);">
            <div>
                <?php exibirTitulo('indicador', 'PNAE - Recursos repassados', 'Parcela complementar - Fundamental'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (680)
					and sh.sehstatus <> 'I'
					and dsh.tidid2 IN (2723) --Mais Educação (parc. complem.) - fundamental
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaPNAERecursos", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="5">
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
function gerarGraficoBolsasOfertadas($detalhe){
    global $db;

    if($detalhe==1){
        $sql = "select
                    tid1.tiddsc as descricao,
                    sum(dsh.dshqtde) as valor
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                where sh.indid in (102)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                and sh.sehstatus <> 'I'
                group by descricao
                order by descricao";
        $arrDados = $db->carregar($sql, null, 3200);
        geraGrafico($arrDados, "graficoPizzaDependencia", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
    }elseif($detalhe==2){
        $sql = "select
                    tid2.tiddsc as descricao,
                    sum(dsh.dshqtde) as valor
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                where sh.indid in (102)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                and sh.sehstatus <> 'I'
                group by descricao
                order by descricao";
        $arrDados = $db->carregar($sql, null, 3200);
        geraGrafico($arrDados, "graficoPizzaLocalizacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
    }
}

function gerarGraficoEscolasAderiram($detalhe){
    global $db;

    if($detalhe==1){
        $sql = "select
                    tid1.tiddsc as descricao,
                    sum(dsh.dshqtde) as valor
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                where sh.indid in (1205)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                and sh.sehstatus <> 'I'
                group by descricao
                order by descricao";
        $arrDados = $db->carregar($sql, null, 3200);
        geraGrafico($arrDados, "graficoPizzaDependenciaEscolas", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
    }elseif($detalhe==2){
        $sql = "select
                    tid2.tiddsc as descricao,
                    sum(dsh.dshqtde) as valor
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                where sh.indid in (1205)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                and sh.sehstatus <> 'I'
                group by descricao
                order by descricao";
        $arrDados = $db->carregar($sql, null, 3200);
        geraGrafico($arrDados, "graficoPizzaLocalizacaoEscolas", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
    }
}

function gerarGraficoPorEstado($detalhe){
    global $db;

    if($detalhe==1){
        $sql = "SELECT dsh.dshuf as descricao, tiddsc as rede, sum(dsh.dshqtde::integer) as valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
                WHERE sh.indid IN (1205)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                AND sh.sehstatus <> 'I'
                --AND dsh.tidid1 NOT IN (7646) --Não Informado
                GROUP BY descricao, rede
                ORDER BY descricao, rede";
        $arrDados = $db->carregar( $sql, null, 3200 );

        $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'rede', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Escolas: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaEstado', null, null, $formatoValores, null, "estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A", 1000, 600, null, 300);
    }elseif($detalhe==2){
        $sql = "SELECT dsh.dshuf as descricao, tiddsc as rede, sum(dsh.dshqtde::integer) as valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
                WHERE sh.indid IN (102)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                AND sh.sehstatus <> 'I'
                GROUP BY descricao, rede
                ORDER BY descricao, rede";
        $arrDados = $db->carregar( $sql, null, 3200 );

        $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'rede', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Matrículas: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaEscolaEstado', null, null, $formatoValores, null, "estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A", 1000, 600, null, 300);
    }
}

function gerarGraficoMatriculaIntegral($detalhe){
    global $db;

    if($detalhe==1){
        $where = "AND dsh.tidid1 IN (8023)";
    }elseif($detalhe==2){
        $where = "AND dsh.tidid1 IN (8024)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 IN (8022,8021)";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            WHERE sh.indid in (2955)
            AND sh.sehstatus <> 'I'
            $where
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $arrX = array();
    $arrA = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrX[] = $dado['periodo'];
            $arrA[] = Round($dado['valor']);
        }

        $aDados = array(
            array('name' => 'Matriculas', 'data'=>$arrA),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoCensoIntegral", "", 300, $formatoValores);
    }
}
?>
