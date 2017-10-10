<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['adesao'])){
    gerarGraficoAdesao($_REQUEST['adesao']);
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
            jQuery('.filtrar_adesao').click(function(){
                jQuery('#div_adesao').load('estrategico.php?modulo=principal/cockpit_etec&acao=A&adesao='+jQuery(this).val());
            });
        });
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')};
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_etec&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >E-TEC</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2393);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de matrículas (2011 - 2014)'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, sum(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (2393)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref BETWEEN '2011' AND '2014'
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
                    $total += Round($dado['valor']);
                    $arrTotal[] = $total;
                }

                $aDados = array(
                    array('name' => 'Matriculas', 'data'=>$arrA),
                    array('name' => 'Total de Matriculas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculas", "Matrículas",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao" width="33%">
            <div class="link" onclick="abreIndicadorPopUp(2297);">
                <?php exibirTitulo('indicador', 'Instituições que aderiram ao programa (2014)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_adesao" name="adesao" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_adesao" name="adesao" value="2" onclick="" />Federal
                <input type="radio" class="filtrar_adesao" name="adesao" value="3" onclick="" />Municipal
                <input type="radio" class="filtrar_adesao" name="adesao" value="4" onclick="" />Sistema S
            </div>
            <div id="div_adesao">
                <?php gerarGraficoAdesao(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3161);" width="34%" colspan="2">
            <div>
                <?php exibirTitulo('mapas', 'Polos E-tec por Município','Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select
						count(distinct dshcodmunicipio) as qtdemun,
						count(distinct dshuf) as qtdeuf,
						sum(dshqtde) as qtde
					from
						painel.seriehistorica seh
					inner join
						painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
					where
						indid = 3161
					and
						dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND seh.sehstatus <> 'I')
					and
						seh.sehstatus <> 'I'";
            $arrValores = $db->pegaLinha($sql,null,3200);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >Polos</span><br />
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtdemun'],0,",",".") ?> </span><span class="subtitulo_box" >Municípios</span><br />
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtdeuf'],0,",",".") ?> </span><span class="subtitulo_box" >UFs</span>
                    </td>
                    <td valign="bottom">
                        <img class="link" onclick="abreIndicadorPopUp(3161)" src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2298);" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas por Eixo Tecnológico (2011 - 2014)'); ?>
            </div>
            <?
            $sql = "select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (2298)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref between '2011' and '2014'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaMatEixo", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3161);" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Evolução de polos E-TEC (2011 - 2014)'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, sum(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (3161)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref BETWEEN '2011' AND '2014'
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
                    $total += Round($dado['valor']);
                    $arrTotal[] = $total;
                }

                $aDados = array(
                    array('name' => 'Polos', 'data'=>$arrA),
                    array('name' => 'Total de Polos', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoPolos", "Polos",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2393);" rowspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas por Rede Ofertante (2011 - 2014)'); ?>
            </div>
            <?
            $sql = "select tid1.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (2393)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref between '2011' and '2014'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaRedeOfertante", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2393);" rowspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas por Situação (2011 - 2014)'); ?>
            </div>
            <?
            $sql = "select tid2.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (2393)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref between '2011' and '2014'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaSituacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',2,33,'','');" align="center"  colspan="2">
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
    <tr>
        <!-- Tabela Processos -->
        <td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/')";>
        <div>
            <?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
        </div>
        </td>
        <!-- Tabela Processos -->
        <td class="fundo_padrao link" onclick="abreAcaoPainel(33);">
            <div>
                <?php exibirTitulo('indicador', 'Indicadores'); ?>
            </div>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="4">
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
function gerarGraficoAdesao($detalhe){

    global $db;

    if($detalhe==2){
        $where = "AND dsh.tidid1 IN (4850)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 IN (4851)";
    }elseif($detalhe==4){
        $where = "AND dsh.tidid1 IN (8805)";
    }

    $sql = "SELECT
                regdescricao AS descricao,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (2297)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref = '2014'
            $where
            GROUP BY descricao
            ORDER BY descricao";
    $dados = $db->carregar( $sql, null, 3200 );
    if($dados){
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColuna($dados, 'graficoColunaAdesao', null, null, $formatoValores, null, "", 1000, 600, null, 300, "", '#fff', true);
    }
}
?>