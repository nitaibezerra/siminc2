<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_basico'])){
    gerarGraficoBasico($_REQUEST['detalhe_basico']);
    die;
}
if(isset($_REQUEST['detalhe_estruturas'])){
    gerarGraficoEstrutura($_REQUEST['detalhe_estruturas']);
    die;
}
if(isset($_REQUEST['detalhe_qualidade'])){
    gerarGraficoQualidade($_REQUEST['detalhe_qualidade']);
    die;
}
if(isset($_REQUEST['detalhe_integral'])){
    gerarGraficoIntegral($_REQUEST['detalhe_integral']);
    die;
}
if(isset($_REQUEST['detalhe_recursos'])){
    gerarGraficoRecursoRepassado($_REQUEST['detalhe_recursos']);
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
            jQuery('.filtrar_basico').click(function(){
                jQuery('#div_basico').load('estrategico.php?modulo=principal/cockpit_pdde&acao=A&detalhe_basico='+jQuery(this).val());
            });
            jQuery('.filtrar_estruturas').click(function(){
                jQuery('#div_estruturas').load('estrategico.php?modulo=principal/cockpit_pdde&acao=A&detalhe_estruturas='+jQuery(this).val());
            });
            jQuery('.filtrar_qualidade').click(function(){
                jQuery('#div_qualidade').load('estrategico.php?modulo=principal/cockpit_pdde&acao=A&detalhe_qualidade='+jQuery(this).val());
            });
            jQuery('.filtrar_integral').click(function(){
                jQuery('#div_integral').load('estrategico.php?modulo=principal/cockpit_pdde&acao=A&detalhe_integral='+jQuery(this).val());
            });
            jQuery('.filtrar_recursos').click(function(){
                jQuery('#div_recursos').load('estrategico.php?modulo=principal/cockpit_pdde&acao=A&detalhe_recursos='+jQuery(this).val());
            });
        });
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_enem.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
        .div_fotos_padrao2{background-color:#152D56;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:400px;margin-bottom:2px}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pdde&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >PDDE - Programa Dinheiro Direto na Escola</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);" width="25%">
			<div>
				<?php exibirTitulo('casas', 'Quantidade de Escolas que<br>receberam recursos - 2012'); ?>
			</div>
            <?php
            $sql="SELECT COUNT(DISTINCT dsh.dshcod) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (678)
                AND sh.sehstatus <> 'I'
                AND dpe.dpeanoref = '2012'";
            $total_2012 = $db->pegaUm($sql,null,3200);
            ?>
            <div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
                <?php echo number_format($total_2012,0,",",".") ?>
            </div>
		</td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);" width="25%">
            <div>
                <?php exibirTitulo('casas', 'Quantidade de Escolas que<br>receberam recursos - 2013'); ?>
            </div>
            <?php
            $sql="SELECT COUNT(DISTINCT dsh.dshcod) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (678)
                AND sh.sehstatus <> 'I'
                AND dpe.dpeanoref = '2013'";
            $total_2013 = $db->pegaUm($sql,null,3200);
            ?>
            <div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
                <?php echo number_format($total_2013,0,",",".") ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);" width="25%">
            <div>
                <?php exibirTitulo('casas', 'Quantidade de Escolas que<br>receberam recursos - 2014'); ?>
            </div>
            <?php
            $sql="SELECT COUNT(DISTINCT dsh.dshcod) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (678)
                AND sh.sehstatus <> 'I'
                AND dpe.dpeanoref = '2014'";
            $total_2014 = $db->pegaUm($sql,null,3200);
            ?>
            <div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
                <?php echo number_format($total_2014,0,",",".") ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);" width="25%">
            <div>
                <?php exibirTitulo('financeiro', 'Esfera de governo'); ?>
            </div>
            <?php
            $sql = "SELECT tid1.tiddsc AS descricao, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (678)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar($sql,null,3200);
            $total_adesao=0;
            $count=-1;
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="" >Esfera de Governo</th>
                    <th class="" >R$</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $total_adesao+=$dado['valor'];
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['descricao'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
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
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);" colspan="3">
            <div>
                <?php exibirTitulo('financeiro', 'Recursos repassados - Total'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (678)
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
                echo geraGraficoLinha($aDados, $arrX, "graficoRecursosPDDETotal", "", 300, $formatoValores);
                echo "</td></tr></table>";
            }
            ?>
        </td>
        <td class="fundo_azul_padrao" rowspan="2" width="20%" rowspan="2">
            <div>
                <?php exibirTitulo('configs', 'Atividades 2014'); ?>
            </div>
            <?php
            $sql = "SELECT arqid, entnome, mundescricao, estuf
                        from (
                            select
                                mun.estuf,
                                mun.mundescricao,
                                ent.entnome,
                                (select arqid from pdeinterativo.galeriafoto gal where gal.pdeid = pde.pdeid and gfostatus = 'A' limit 1) as arqid
                            from pddeinterativo.pdinterativo pdi
                            inner join entidade.entidade ent on ent.entid = pdi.entid
                            inner join entidade.endereco ende on ende.entid = ent.entid
                            inner join territorios.municipio mun on mun.muncod = ende.muncod
                            inner join workflow.documento doc on doc.docid = pdi.docid
                            inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
                            inner join pdeinterativo.pdinterativo pde ON pde.pdicodinep::integer = ent.entcodent::integer
                            where pdi.pdeano = 2014 and pdi.pdistatus = 'A' and esd.esdid = 523 --Validado pelo MEC
                            group by mun.estuf,mun.muncod,ent.entnome,esd.esddsc,mun.mundescricao,pde.pdeid
                        ) foo
                        WHERE arqid IS NOT NULL
                        ORDER BY random()
                        LIMIT 50";
            $arrFotos = adapterConnection::pddeinterativo()->carregar($sql);
            ?>
            <div style="clear:both" id="div_fotos_padrao"  >
                <?php for($x=9;$x>=1;$x--): ?>
                    <div style="clear:both" class="div_fotos_interno"  >
                        <?php for($i=$x;$i<=(9+$x);$i++): ?>
                            <div class="div_fotos_padrao" >
                                <table border="0" >
                                    <tr>
                                        <td>
                                            <img width="80" height="60" src="http://pdeinterativo.mec.gov.br/slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=pdeinterativo" />
                                        </td>
                                        <td style="color:#FFFFFF">
                                            <b><?php echo $arrFotos[$i]['entnome'] ?></b><br/>
                                            <b>Município:</b> <?php echo $arrFotos[$i]['mundescricao'] ?>/<?php echo $arrFotos[$i]['estuf'] ?><br/>
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
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(678);" colspan="3">
            <div>
                <?php exibirTitulo('financeiro', 'Escolas beneficiadas - Por estado'); ?>
            </div>
            <?php
            $sql = "SELECT dsh.dshuf AS descricao, COUNT(DISTINCT dsh.dshcod) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid in (678)
                    AND sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaUF', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(678);">
                <?php exibirTitulo('indicador', 'PDDE Básico', 'Educação Infantil, Ensino Fundamental e Ensino Médio'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_basico" name="basico" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_basico" name="basico" value="2" onclick="" />Escolas beneficiadas
            </div>
            <div id="div_basico">
                <?php gerarGraficoBasico(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="25%" colspan="2" rowspan="2">
            <div class="link" onclick="abreIndicadorPopUp(678);">
                <?php exibirTitulo('financeiro', 'Recursos repassados - Total 2014'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_recursos" name="recursos" value="1" checked="checked" onclick="" />Total
                <!--<input type="radio" class="filtrar_recursos" name="recursos" value="2" onclick="" />PDDE Básico-->
                <input type="radio" class="filtrar_recursos" name="recursos" value="3" onclick="" />PDDE Estrutura
                <input type="radio" class="filtrar_recursos" name="recursos" value="4" onclick="" />PDDE Qualidade
            </div>
            <div id="div_recursos">
                <?php gerarGraficoRecursoRepassado(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(678);">
                <?php exibirTitulo('indicador', 'PDDE Estrutura', '(Água, campo e acessibilidade)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_estruturas" name="estruturas" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_estruturas" name="estruturas" value="2" onclick="" />Escolas beneficiadas
            </div>
            <div id="div_estruturas">
                <?php gerarGraficoEstrutura(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(678);">
                <?php exibirTitulo('indicador', 'PDDE Qualidade', '(ProEMI, Mais Cultura, Sustentável, Atleta na Escola e PDE Escola)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_qualidade" name="qualidade" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_qualidade" name="qualidade" value="2" onclick="" />Escolas beneficiadas
            </div>
            <div id="div_qualidade">
                <?php gerarGraficoQualidade(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(87);?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(1420);">
                <?php exibirTitulo('indicador', 'PDDE Educação Integral'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_integral" name="integral" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_integral" name="integral" value="2" onclick="" />Escolas beneficiadas
            </div>
            <div id="div_integral">
                <?php gerarGraficoIntegral(1); ?>
            </div>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,87,'','');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
        <!-- Tabela Processos -->
        <td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/')";>
            <div>
                <?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
            </div>
        </td>
        <!-- Tabela Processos -->
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
function gerarGraficoRecursoRepassado($detalhe){
    global $db;

    if($detalhe==1){
        $campo = "CASE
                    WHEN dsh.tidid2 IN (2484, 2483, 2485, 2486) THEN 'PDDE Básico'
                    WHEN dsh.tidid2 IN (7243, 7245, 7247) THEN 'PDDE Estrutura'
                    WHEN dsh.tidid2 IN (7244, 7246, 7248,7249,7871) THEN 'PDDE Qualidade'
                    WHEN sh.indid IN (1420) THEN 'PDDE Educação Integral'
                  END AS descricao";
        $where = "AND (
                        (sh.indid IN (678) AND dsh.tidid2 IN (2484, 2483, 2485, 2486, 7243, 7245, 7247, 7244, 7246, 7248,7249,7871))
                  OR
                        sh.indid IN (1420)
                       )";
    }elseif($detalhe==2){
        $where = "AND sh.indid IN (678) AND dsh.tidid2 IN (2484, 2483, 2485, 2486)";
        $campo = "tid2.tiddsc AS descricao";
    }elseif($detalhe==3){
        $where = "AND sh.indid IN (678) AND dsh.tidid2 IN (7243, 7245, 7247)";
        $campo = "tid2.tiddsc AS descricao";
    }elseif($detalhe==4){
        $where = "AND sh.indid IN (678) AND dsh.tidid2 IN (7244, 7246, 7248,7249,7871)";
        $campo = "tid2.tiddsc AS descricao";
    }

    $sql = "select
                $campo,
                sum(dsh.dshqtde) as valor
            from painel.seriehistorica sh
            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            LEFT JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            where sh.dpeid = 1250 --2014
            and sh.sehstatus <> 'I'
            $where
            group by descricao
            order by descricao";
    $arrDados = $db->carregar($sql, null, 3200);
    geraGrafico($arrDados, "graficoPizzaRecursosRepassados", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
}

function gerarGraficoBasico($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
    }else{
        $campo = "COUNT(DISTINCT dsh.dshcod) AS valor";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (678)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid2 IN (2484, 2483, 2485, 2486)
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

        if($detalhe==1){
            $aDados = array(
                array('name' => 'Recursos', 'data'=>$arrA),
                array('name' => 'Total', 'data'=>$arrTotal),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }else{
            $aDados = array(
                array('name' => 'Escolas', 'data'=>$arrA),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }
        echo "<table width='90%' border='0'><tr><td>";
        echo geraGraficoLinha($aDados, $arrX, "graficoRecursosPDDEBasico", "", 300, $formatoValores);
        echo "</td></tr></table>";
    }
}

function gerarGraficoEstrutura($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
    }else{
        $campo = "COUNT(DISTINCT dsh.dshcod) AS valor";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (678)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid2 IN (7243, 7245, 7247)
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

        if($detalhe==1){
            $aDados = array(
                array('name' => 'Recursos', 'data'=>$arrA),
                array('name' => 'Total', 'data'=>$arrTotal),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }else{
            $aDados = array(
                array('name' => 'Escolas', 'data'=>$arrA),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }
        echo "<table width='90%' border='0'><tr><td>";
        echo geraGraficoLinha($aDados, $arrX, "graficoRecursosPDDEEstrutura", "", 300, $formatoValores);
        echo "</td></tr></table>";
    }
}

function gerarGraficoQualidade($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
    }else{
        $campo = "COUNT(DISTINCT dsh.dshcod) AS valor";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (678)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid2 IN (7244, 7246, 7248,7249,7871)
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

        if($detalhe==1){
            $aDados = array(
                array('name' => 'Recursos', 'data'=>$arrA),
                array('name' => 'Total', 'data'=>$arrTotal),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }else{
            $aDados = array(
                array('name' => 'Escolas', 'data'=>$arrA),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }
        echo "<table width='90%' border='0'><tr><td>";
        echo geraGraficoLinha($aDados, $arrX, "graficoRecursosPDDEQualidade", "", 300, $formatoValores);
        echo "</td></tr></table>";
    }
}

function gerarGraficoIntegral($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
    }else{
        $campo = "COUNT(DISTINCT dsh.dshcod) AS valor";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
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

        if($detalhe==1){
            $aDados = array(
                array('name' => 'Recursos', 'data'=>$arrA),
                array('name' => 'Total', 'data'=>$arrTotal),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }else{
            $aDados = array(
                array('name' => 'Escolas', 'data'=>$arrA),
            );
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
        }
        echo "<table width='90%' border='0'><tr><td>";
        echo geraGraficoLinha($aDados, $arrX, "graficoRecursosPDDEIntegral", "", 300, $formatoValores);
        echo "</td></tr></table>";
    }
}
?>