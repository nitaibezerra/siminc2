<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A');
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
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('cockpit/images/banner/bannerMaisMedicos.jpg');}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_residencias&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Residências</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Vagas Criadas'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Graduação e Vagas'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Medicina da Família'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('mapas', 'Mapa'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3187);">
            <div>
                <?php exibirTitulo('indicador', 'Total de vagas de Residência'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3186);">
            <div>
                <?php exibirTitulo('indicador', 'Vagas de Residência'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3186);">
            <div>
                <?php exibirTitulo('indicador', 'Tipo de Residência'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Vagas autorizadas e ocupadas Medicina de Família'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Vagas solicitadas x autorizadas em Medicina da Família'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Vagas para demais especialidades'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2178);">
            <div>
                <?php exibirTitulo('indicador', 'Quantidade de programas de residência médica e em área profissional da saúde regulados pelo MEC'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2179);">
            <div>
                <?php exibirTitulo('indicador', 'Quantidade de programas de residência médica e em área profissional da saúde financiados pelo MEC'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3186);">
            <div>
                <?php exibirTitulo('indicador', 'Vagas de Residência'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Vagas de Medicina Geral de Família e Comunidade'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Concluintes de Residência'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);" colspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Quantidade de vagas de residência em multiprofissional, por ano'); ?>
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