<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['semestre_ofertadas'])){
    gerarGraficoInscricoesOfertadas($_REQUEST['semestre_ofertadas']);
    die;
}
if(isset($_REQUEST['semestre_origem'])){
    gerarGraficoInscritosOrigem($_REQUEST['semestre_origem']);
    die;
}
if(isset($_REQUEST['semestre_vagas'])){
    gerarGraficoVagas($_REQUEST['semestre_vagas']);
    die;
}
?>
<!DOCTYPE HTML>
<html>
<head>
	<meta http-equiv="X-UA-Compatible" content="IE=7" />
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
	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	</style>
    <script type="text/javascript">
        jQuery(function(){
            jQuery('.filtrar_semestre').click(function(){
                jQuery('#div_inscricoes_ofertadas').load('estrategico.php?modulo=principal/cockpit_sisu&acao=A&semestre_ofertadas='+jQuery(this).val());
            });
            jQuery('.filtrar_semestre2').click(function(){
                jQuery('#div_inscritos_origem').load('estrategico.php?modulo=principal/cockpit_sisu&acao=A&semestre_origem='+jQuery(this).val());
            });
            jQuery('.filtrar_semestre3').click(function(){
                jQuery('#div_vagas').load('estrategico.php?modulo=principal/cockpit_sisu&acao=A&semestre_vagas='+jQuery(this).val());
            });
        });
    </script>
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_prouni&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >SISU</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" width="25%">
			<?php
			montaTabelaExamesMec('SISU - Edição 1º/2014', '22');
			?>
		</td>
        <td class="fundo_padrao" width="40%">
            <div>
                <?php exibirTitulo('indicador', 'Indicadores'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (1435)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrInscricoes = $db->pegaLinha( $sql, null, 3200 );

            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (1401)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrInscritos = $db->pegaLinha( $sql, null, 3200 );

            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde) as valor,
                        count(DISTINCT dsh.dshcod) as ies
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (2671)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrVagas = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <div>
                <div class="link" onclick="abreIndicadorPopUp(1435);" style="float: left; width: 50%;"> <?php exibirTitulo('executiverel', 'Inscrições<br />'.number_format($arrInscricoes['valor'],0,",","."), 'Não cumulativo<br>('.$arrInscricoes['descricao'].')'); ?></div>
                <div class="link" onclick="abreIndicadorPopUp(2671);" style="float: left; width: 50%;"> <?php exibirTitulo('alvo', 'Vagas<br>disponibilizadas<br />'.number_format($arrVagas['valor'],0,",","."), 'Não cumulativo<br>('.$arrVagas['descricao'].')'); ?></div>
                <div class="link" onclick="abreIndicadorPopUp(1401);" style="float: left; width: 50%;"> <?php exibirTitulo('executive', 'Inscritos<br />'.number_format($arrInscritos['valor'],0,",","."), 'Não cumulativo<br>('.$arrInscritos['descricao'].')'); ?></div>
                <div class="link" onclick="abreIndicadorPopUp(2671);" style="float: left; width: 50%;"> <?php exibirTitulo('casas', 'Instituições<br>participantes<br />'.number_format($arrVagas['ies'],0,",","."), 'Não cumulativo<br>('.$arrVagas['descricao'].')'); ?></div>
            </div>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,122,'','');" align="center" width="15%">
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/prouni/default.htm');" width="15%">
            <div>
                <?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(1435);">
                <?php exibirTitulo('indicador', 'Inscrições', 'Município da oferta'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_semestre" name="semestre" value="1" checked="checked" onclick="" />1º Semestre
                <input type="radio" class="filtrar_semestre" name="semestre" value="2" onclick="" />2º Semestre
            </div>

            <div id="div_inscricoes_ofertadas">
                <?php gerarGraficoInscricoesOfertadas(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(1401);">
                <?php exibirTitulo('indicador', 'Inscritos', 'Município de origem'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_semestre2" name="semestre2" value="1" checked="checked" onclick="" />1º Semestre
                <input type="radio" class="filtrar_semestre2" name="semestre2" value="2" onclick="" />2º Semestre
            </div>

            <div id="div_inscritos_origem">
                <?php gerarGraficoInscritosOrigem(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(2671);">
                <?php exibirTitulo('indicador', 'Vagas Disponibilizadas', 'Por modalidade de concorrência'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_semestre3" name="semestre3" value="1" checked="checked" onclick="" />1º Semestre
                <input type="radio" class="filtrar_semestre3" name="semestre3" value="2" onclick="" />2º Semestre
            </div>

            <div id="div_vagas">
                <?php gerarGraficoVagas(1); ?>
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
function gerarGraficoInscricoesOfertadas($semestre)
{
    global $db;

    if($semestre){
        $where = "AND dpe.dpeordem = {$semestre}";
    }
    $sql = "select
				dpeanoref as descricao,
				sum(dsh.dshqtde) as valor
			from painel.seriehistorica sh
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where sh.indid in (1435)
			and sh.sehstatus <> 'I'
			$where
			group by descricao
			order by descricao";
    $dados = $db->carregar( $sql, null, 3200 );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
    );
    geraGraficoColuna($dados, 'graficoColunaOferta', null, null, $formatoValores, null, "", 1000, 600, null, 300);
}

function gerarGraficoInscritosOrigem($semestre)
{
    global $db;

    if($semestre){
        $where = "AND dpe.dpeordem = {$semestre}";
    }
    $sql = "select
				dpeanoref as descricao,
				sum(dsh.dshqtde) as valor
			from painel.seriehistorica sh
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where sh.indid in (1401)
			and sh.sehstatus <> 'I'
			$where
			group by descricao
			order by descricao";
    $dados = $db->carregar( $sql, null, 3200 );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
    );
    geraGraficoColuna($dados, 'graficoColunaOrigem', null, null, $formatoValores, null, "", 1000, 600, null, 300);
}

function gerarGraficoVagas($semestre)
{
    global $db;

    if($semestre){
        $where = "AND dpe.dpeordem = {$semestre}";
    }
    $sql = "select
				dpeanoref as descricao,
				tiddsc as modalidade,
				sum(dsh.dshqtde) as valor
			from painel.seriehistorica sh
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			where sh.indid in (2671)
			and sh.sehstatus <> 'I'
			$where
			group by descricao, modalidade
			order by descricao, modalidade";
    $arrDados = $db->carregar( $sql, null, 3200 );

    $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'modalidade', 'valor');

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
    );
    geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaVagas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
}
?>