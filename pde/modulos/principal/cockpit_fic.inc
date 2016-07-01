<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

//permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_professor1'])){
    gerarGraficoProfessor1($_REQUEST['detalhe_professor1']);
    die;
}
if(isset($_REQUEST['detalhe_professor2'])){
    gerarGraficoProfessor2($_REQUEST['detalhe_professor2']);
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
                jQuery('#div_professor1').load('estrategico.php?modulo=principal/cockpit_fic&acao=A&detalhe_professor1='+jQuery(this).val());
            });
            jQuery('.filtrar_professor2').click(function(){
                jQuery('#div_professor2').load('estrategico.php?modulo=principal/cockpit_fic&acao=A&detalhe_professor2='+jQuery(this).val());
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
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_fic&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Formação Inicial e Continuada</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1371);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Formação continuada em Educação<br>no Campo - Ingressantes'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (1371)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaIngressantes", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
            }
            ?>
        </td>
        <td class="fundo_padrao" width="34%">
            <div class="link" onclick="abreIndicadorPopUp(665);">
                <?php exibirTitulo('indicador', 'Professores em Formação - Procampo'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_professor1" name="professor1" value="1" checked="checked" onclick="" />Por ano
                <input type="radio" class="filtrar_professor1" name="professor1" value="2" onclick="" />Por edital
            </div>
            <div id="div_professor1">
                <?php gerarGraficoProfessor1(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="33%">
            <div class="link" onclick="abreIndicadorPopUp(745);">
                <?php exibirTitulo('indicador', 'Professores Formados - Procampo'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_professor2" name="professor2" value="1" checked="checked" onclick="" />Por ano
                <input type="radio" class="filtrar_professor2" name="professor2" value="2" onclick="" />Por edital
            </div>
            <div id="div_professor2">
                <?php gerarGraficoProfessor2(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(124);" colspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Professores formados em curso de<br>Formação Superior e Licenciaturas Indígenas'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
                        'Professores' AS tipo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (124)
					and sh.sehstatus <> 'I'
					group by ano, tipo
					order by ano, tipo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $valorAcumulado = 0;
                $testaAno="";
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    if($testaAno!=$dado['ano']){
                        $dados[] = array(
                            'ano'=>$dado['ano'],
                            'tipo'=>'Professores Acumulados',
                            'valor'=>$valorAcumulado,
                        );
                        $testaAno=$dado['ano'];
                    }
                }
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaProfessores', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
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
        $campo = "dpe.dpedsc as descricao";
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
            where sh.indid in (665)
            and sh.sehstatus <> 'I'
            group by descricao
            order by descricao";
    $arrDados = $db->carregar($sql, null, 3200);
    if($arrDados){
        geraGrafico($arrDados, "graficoPizzaProcampo", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
    }
}
function gerarGraficoProfessor2($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "dpe.dpedsc as descricao";
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
            where sh.indid in (745)
            and sh.sehstatus <> 'I'
            group by descricao
            order by descricao";
    $arrDados = $db->carregar($sql, null, 3200);
    if($arrDados){
        geraGrafico($arrDados, "graficoPizzaProfFormadosProcampo", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
    }
}
?>