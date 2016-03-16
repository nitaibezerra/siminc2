<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body onload="refreshAutomatico();">

<?php
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
if( $_REQUEST['useronline'] ){
	echo pegaUsuarioOnline(34);
	exit;
}
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<style>
	.fundo_td{background-color:#0F6D39}
	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
	.fundo_td:hover {background-color:#0D8845}
	.fundo_td{text-align:left;vertical-align:top;}
	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
	.lista_metas{float:left}
	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
	.tabela_box{color:#FFFFFF;}
	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px}
	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
	.fundo_td_azul{background-color:#2B86EE}
	.fundo_td_azul:hover{background-color:#01A2D8}
	.fundo_td_laranja{background-color:#EE9200}
	.fundo_td_laranja:hover{background-color:#EBB513}
	.fundo_td_vermelho{background-color:#BB0000}
	.fundo_td_vermelho:hover{background-color:#DD0000}
	.fundo_td_roxo{background-color:#5333AD}
	.fundo_td_roxo:hover{background-color:#6A5ACD}
	.fundo_td_azul_escuro{background-color:#152D56}
	.fundo_td_azul_escuro:hover{background-color:#1F3864}
	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
	body{background-image:url('../imagens/degrade-fundo-preto.png');background-repeat:repeat-x;background-color:#DF981A;margin:0px;padding-top:0px;}
	.fundo_titulo{background-image:url('../imagens/cockpit/fundo1_bolsa_formacao.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	.numero{text-align:right}
	.center{text-align:center}
	.titulo_box a{color:#FFFFFF;text-decoration:none;}
	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
	.div_fotos_interno{width:98%;height:85px}
	.bold{font-weight:bold}
	.link{cursor:pointer}
	.bold{font-weight:bold}
</style>

<script>
	jQuery.noConflict();
	jQuery('.div_fotos_interno').cycle({
		fx: 'scrollDown'
	});

	atualizaUsuario();

	function acessarMaisEducacao(entid,memid){
		url = "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A&painel=1&entid=" + entid + "&memid=" + entid;
		window.open(url,'Mais Educação','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	function abreMapasUAB()
	{
	 	url = "/painel/painel.php?modulo=principal/mapas/mapaUAB&acao=A&carregaMapaAutomativo=1";
	 	window.open(url,'_blank');
	}
</script>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_formacao_professores&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Formação de Professores</div></td>
	</tr>

	<tr>
		<td class="fundo_td">
			<div>
                <img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" >Indicadores<br/><span class="subtitulo_box" >Impacto | Produto | Processo</span></div>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td class="center bold link" onclick="abreIndicadores(129391,'')" >UAB</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="abreIndicadores(129267,'')" >PIBID</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="abreIndicadores(133995,'')" >PACTO</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="abreIndicadores(133996,'')" >PARFOR</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="abreIndicadores(132193,'')" >Procampo</td>
				</tr>
			</table>
		</td>

		<td class="fundo_td" rowspan="2">
			<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Formação Inicial: Formados</div>
			</div>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
				from painel.indicador i
				inner join painel.seriehistorica sh on sh.indid=i.indid
				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
				inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
				where i.indid in (1421)
				and sh.sehstatus <> 'I'
				and dpe.dpeanoref between '2011' and '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
			<div style="clear:both;" >
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%"  >
				<tr>
					<td class="center bold" >Iniciativa</td>
					<td class="center bold" >Formados</td>
					<td class="center bold" >Período</td>
				</tr>
				<tr  onclick="abreIndicadorPopUp(1421);">
					<td class="">PARFOR</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
					<td class="numero">2011-2012</td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (1423)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref between '2009' and '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(1423);">
					<td class="">UAB</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
					<td class="numero">2009-2012</td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (1425)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref between '2005' and '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(1425);">
					<td class="">Proinfantil</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
					<td class="numero">2005-2012</td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (745)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref between '2010' and '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
			<tr onclick="abreIndicadorPopUp(745);">
					<td class="">Procampo</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
					<td class="numero">2010-2012</td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (124)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref between '2005' and '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(124);">
					<td class="">Prolind</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
					<td class="numero">2005-2012</td>
				</tr>
				</table>
			</div>
		</td>

		<td class="fundo_td" rowspan="2">
			<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Formação Inicial: Matrículas Ativas em 2012</div>
			</div>

			<div style="clear:both;" >
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%"  >
				<tr>
					<td class="center bold" >Iniciativa</td>
					<td class="center bold" >Matrículas</td>
				</tr>
			<?php
			$sql = "select 0 as qtde";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(0);">
					<td class="">PARFOR</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
			<?php
			$sql = "select sh.sehqtde as qtde
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where i.indid in (547)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref = '2012'
					order by dpe.dpeordem desc
					limit 1";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(547);">
					<td class="">UAB</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (1426)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(1426);">
					<td class="">Proinfantil</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (665)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(665);">
					<td class="">Procampo</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (123)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(123);">
					<td class="">Prolind</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
			</table>
			</div>
		</td>

		<td class="fundo_td" rowspan="2">
			<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Formação Inicial: Ingressantes 2012</div>
			</div>
			<div style="clear:both;" >
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%"  >
				<tr>
					<td class="center bold" >Iniciativa</td>
					<td class="center bold" >Ingressantes</td>
				</tr>
							<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (483)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(483);">
					<td class="">PARFOR</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
											<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (1422)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(1422);">
					<td class="">UAB</td>
					<td class="numero"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
				<tr>
					<td class="">Proinfantil</td>
					<td class="numero">0</td>
				</tr>
				<tr>
					<td class="">Procampo</td>
					<td class="numero">0</td>
				</tr>
				<tr>
					<td class="">Prolind</td>
					<td class="numero">0</td>
				</tr>
			<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (1380)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(1380);">
					<td class="">Pedagogia (Decreto nº 5.626/05) </td>
					<td class="numero">0</td>
				</tr>
				<?php
			$sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (1377)
			and sh.sehstatus <> 'I'
			and dpe.dpeanoref = '2012'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr onclick="abreIndicadorPopUp(1377);">
					<td class="">Letras/Libras</td>
					<td class="numero">0</td>
				</tr>
			</table>
			</div>
		</td>
		</tr>

	<tr>
		<td class="fundo_td">
			<div>
                <img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" >Painéis estratégicos</div>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td class="center bold link" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_uab&acao=A'" >UAB</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_pronacampo&acao=A'" >Procampo</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_pacto&acao=A'" >Pacto</td>
				</tr>
			</table>
		</td>
	</tr>

	<tr>

	<td class="fundo_td" valign="top">
		<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >PIBID</div>
			</div>
			<div style="clear:both;" >
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%"  >

			<?php $sql = "select sum(dsh.dshqtde) as qtde
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where i.indid in (108)
			and sh.sehstatus <> 'I'";
			$total_ies = $db->pegaLinha($sql);
			?>
				<tr>
					<td class="center bold" >Meta 2012(Acumulada)</td>
					<td class="center bold link" onclick="abreIndicadorPopUp(108);">Bolsas Ofertadas (2007-2012)</td>
				</tr>
				<tr >
					<td class="numero">45.000</td>
					<td class="numero link" onclick="abreIndicadorPopUp(108);"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td>
				</tr>
			</table>
			</div>
		</td>
		<td class="fundo_td" colspan="2" rowspan="2">
			<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Formação Continuada (Secretarias do MEC)</div>
			</div>
			<div style="clear:both;" >
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%"  >
				<tr>
					<td class="center bold" >Área</td>
					<td class="center bold" >Especialização</td>
					<td class="center bold" >Extensao/Aperfeiçoamento</td>
				</tr>
					<tr>
					<td class="center bold" ></td>
					<td class="center bold" ><table width="100%"><tr><td align="center">Formados</td><td align = "center">Período</td></tr></table></td>
					<td class="center bold" ><table width="100%"><tr><td align="center">Formados</td><td align = "center">Período</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1429);">
					<td class="">Alfabetização e Letramento</td>
					<td class="numero"><table width="100%"><tr><td align="center">-</td><td align = "center">-</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where i.indid in (1429, 1431)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref between '2008' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2008-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1372);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where i.indid in (1372)
					and sh.sehstatus <> 'I'
					and (dsh.tidid1 = 3325)
					and sh.dpeid = (select dpe2.dpeid from painel.seriehistorica sh2 inner join painel.detalheperiodicidade dpe2 on
					dpe2.dpeid = sh2.dpeid where sh2.indid=i.indid and dpe2.dpeanoref between '2011' and '2012' order by dpe2.dpeordem desc limit 1 )";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Campo</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where i.indid in (1372)
					and sh.sehstatus <> 'I'
					and (dsh.tidid1 = 3324)
					and sh.dpeid = (select dpe2.dpeid from painel.seriehistorica sh2 inner join painel.detalheperiodicidade dpe2 on
					dpe2.dpeid = sh2.dpeid where sh2.indid=i.indid and dpe2.dpeanoref between '2011' and '2012' order by dpe2.dpeordem desc limit 1 )";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1365);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where i.indid in (1365)
					and sh.sehstatus <> 'I'
					and (dsh.tidid1 = 3312)
					and sh.dpeid = (select dpe2.dpeid from painel.seriehistorica sh2 inner join painel.detalheperiodicidade dpe2 on
					dpe2.dpeid = sh2.dpeid where sh2.indid=i.indid and dpe2.dpeanoref between '2011' and '2012' order by dpe2.dpeordem desc limit 1 )";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Direitos Humanos e Cidadania</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where i.indid in (1365)
					and sh.sehstatus <> 'I'
					and (dsh.tidid1 = 3313)
					and sh.dpeid = (select dpe2.dpeid from painel.seriehistorica sh2 inner join painel.detalheperiodicidade dpe2 on
					dpe2.dpeid = sh2.dpeid where sh2.indid=i.indid and dpe2.dpeanoref between '2011' and '2012' order by dpe2.dpeordem desc limit 1 )";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1399);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1399)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3312)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Disciplinas Curriculares</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center">-</td><td width="50%" align = "center">-</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (199)
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref between '2004' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2004-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1370);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1370)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3321)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Educação de Jovens e Adultos</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1370)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3320)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1363);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1363)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3309)
						and dpe.dpeanoref between '2007' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Educação Especial</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2007-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1363)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3308)
						and dpe.dpeanoref between '2007' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2007-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1427);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1427)
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref between '2008' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Gestão e Controle Social</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2008-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (311,312,424,585)
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref between '2003' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2003-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1376);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1376)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3351)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Indígena</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1376)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3350)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1368);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1368)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3312)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Juventude</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (1368)
						and sh.sehstatus <> 'I'
						and (dsh.tidid1 = 3317)
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
				</tr>
				<tr onclick="abreIndicadorPopUp(1417);">
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where ((i.indid = 1417 and dsh.tidid1 = 3345) or (i.indid = 1374 and dsh.tidid1 = 3329))
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="">Quilombola e Relações Étnicoraciais</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where ((i.indid = 1417 and dsh.tidid1 = 3344) or (i.indid = 1374 and dsh.tidid1 = 3328))
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref between '2011' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero"><table border=0 width="100%"><tr><td width="50%" align="center"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align = "center">2011-2012</td></tr></table></td>
				</tr>
				<tr>
					<td class="">Tecnologia</td>
					<td class="numero"><table width="100%"><tr><td width="50%" align="center">-</td><td width="50%" align = "center">-</td></tr></table></td>
					<?php
					$sql = "select sum(dsh.dshqtde) as qtde
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (479,196)
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref between '2006' and '2012'";
					$total_ies = $db->pegaLinha($sql);
					?>
					<td class="numero" ><table border=0 width="100%"><tr><td width="50%" align="center" onclick="abreIndicadorPopUp(479);"><?php echo number_format($total_ies['qtde'],0,",",".") ?></td><td width="50%" align="center" onclick="abreIndicadorPopUp(196);">2006-2012</td></tr></table></td>
				</tr>
			</table>
			</div>
		</td>

		<td class="fundo_td" valign="top">
		<div onclick="abreMapasUAB()" >
			<?php
			$sql = "SELECT count(distinct p.muncod) AS municipio, SUM(nummatriculasativas::integer) AS ativas
					FROM painel.consultaofertas co
					INNER JOIN painel.polo p ON p.polid = co.idpolo::integer";
			$arrValores = $db->pegaLinha($sql);
			?>
			<img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
			<div style="float:left;cursor:pointer;" class="titulo_box" >MAPA UAB<br/><span class="subtitulo_box" ><a href="#" >Clique no mapa para visualizar os detalhes.</a></span>
			<br /><br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['ativas'],0,",",".") ?> </span><span class="subtitulo_box" >matrículas ativas</span>
			<br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['municipio'],0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
			</div>
			</div>
			<img class="link" onclick="abreMapasUAB()" src="../imagens/cockpit/mapa_cockpit.png"  />
		</td>

	</tr>

	<tr>
		<td class="fundo_td">
			<div>
                <img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" >Mapas de Processos</div>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td class="center bold link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/pde-ae/Caminho%20Cr%C3%ADtico%20-%20Universidade%20Aberta%20do%20Brasil%20-%20UAB%20-%20Expans%C3%A3o%20do%20Programa.htm','UAB','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');">UAB</td>
				</tr>
				<tr>
                	<td class="center bold link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/pde-ae/Caminho%20Cr%C3%ADtico%20-%20PIBID.htm','PIBID','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');">PIBID</td>
				</tr>
			</table>
		</td>

		<td class="fundo_td link" onclick="abreIndicadorPopUp(547);">
			<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >UAB - Matrículas ativas por nível</div>
				<div style="clear:both;" >
				<?php
				$sql = "select sum(dsh.dshqtde) as qtde,
						case
							when dsh.tidid1 in (3164,3165) then 'Mestrado/Doutorado'
							when dsh.tidid1 in (3163) then 'Especialização'
							when dsh.tidid1 in (3159,3160,3161,3162,3167) then 'Graduação'
							when dsh.tidid1 in (3158,3166) then 'Extensão/Aperfeiçoamento'
						end as nivel
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						where i.indid in (547)
						and sh.sehstatus <> 'I'
						and dpe.dpeanoref = '2012'
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						group by nivel
						order by nivel";
				$arrNivel = $db->carregar($sql,null,3200);
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%"  >
				<tr>
					<td class="center bold" >Nível</td>
					<td class="center bold" >Matrículas Ativas</td>
				</tr>
				<?php
				if($arrNivel){
					foreach($arrNivel as $nivel): ?>
					<tr>
						<td class="" ><?php echo $nivel['nivel'] ?></td>
						<td class="numero" ><?php echo number_format($nivel['qtde'],0,",",".") ?></td>
					</tr>
					<?php
					$totalgeral+=$nivel['qtde'];
					?>
				<?php endforeach; }?>
				<tr>
					<td class="bold">Total</td>
					<td class="numero"><?php echo number_format($totalgeral,0,",",".") ?></td>
				</tr>
			</table>
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