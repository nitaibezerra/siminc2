<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
    <?php
    include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
    $tipo = $_REQUEST['tipo'];
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
    	.fundo_td{text-align:left;vertical-align:top;}
    	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
    	.lista_metas{float:left}
    	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
    	.tabela_box{color:#FFFFFF;}
    	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px}
    	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
    	.fundo_td_azul{background-color:#2B86EE}
    	.fundo_td_azul:hover{background-color:#01A2D8}
    	.fundo_td_azul{text-align:left;vertical-align:top;}
    	.fundo_td_laranja{background-color:#EE9200}
    	.fundo_td_laranja:hover{background-color:#EBB513}
    	.fundo_td_vermelho{background-color:#BB0000}
    	.fundo_td_vermelho:hover{background-color:#DD0000}
    	.fundo_td_roxo{background-color:#5333AD}
    	.fundo_td_roxo:hover{background-color:#6A5ACD}
    	.fundo_td_azul_escuro{background-color:#152D56}
    	.fundo_td_azul_escuro:hover{background-color:#1F3864}
    	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
    	body{background-image:url('../imagens/fundo_cockpit.jpg');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
    	.fundo_titulo{background-image:url('../imagens/fundocreche.jpg');background-repeat:repeat-xt;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
    	.numero{text-align:right}
    	.center{text-align:center}
    	.titulo_box a{color:#FFFFFF;text-decoration:none;}
    	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
    	.div_fotos_interno{margin-bottom:2px;width:98%}
    	.link{cursor:pointer}
    	.bold{font-weight:bold}
    	.tabela_listagem {
    		background-color: #FFFFFF;
    		color: #000000;
    	}

    	.filtro_listagem{
    	   width: 80%;
    	}
    </style>
    <script>
    	jQuery.noConflict();
    	jQuery('.div_fotos_interno').cycle({
    		fx: 'scrollDown'
    	});

    	atualizaUsuario();

    	function abreObrasPac(preid,muncod){
    		window.open('/par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid+'&muncod='+muncod);
    	}
    	function abreObras(obrid){
    		window.open('/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid);
    	}

    	function submeterFormularioListagem(){
            jQuery('#formulario_filtro_listagem').attr('action', window.location.href).submit();
    	}

    	jQuery(function(){

            jQuery('.ordenador_listagem').click(function(){
            	jQuery('#ordenador_listagem').val(jQuery(this).attr('ordenar'));
            	submeterFormularioListagem();
            });

            jQuery('.img_filtro_listagem').click(function(){
            	submeterFormularioListagem();
            });

            jQuery('.filtro_listagem').keypress(function(e) {
                if(e.which == 13) {
                	submeterFormularioListagem();
                }
            });
        });
    </script>
</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<!-- Título-->
	<?php
	if($tipo=='MI'){
		$where = " AND pre.ptoid IN (42,43,44,45)";
		?>
		<tr>
			<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Ocorrências Pré-Obra - MI</div></td>
		</tr>
	<?php
	}else{
		$where = " AND pre.ptoid NOT IN (42,43,44,45)";
	?>
		<tr>
			<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Ocorrências Pré-Obra - Metodologia Convencional</div></td>
		</tr>
	<?php
	}
	switch( $_REQUEST['parametro'] ){
		case 'Terreno indisponível':
			$perid = 1299;
		break;
		case 'Terreno impróprio':
			$perid = 1301;
		break;
		case 'Fotos insuficientes':
			$perid = 1303;
		break;
		case 'Valores inadequados':
			$perid = 1305;
		break;
		case 'Período de execução inadequado':
			$perid = 1307;
		break;
		case 'Quantidade de crianças insuficiente':
			$perid = 1309;
		break;
		case 'Informações insuficientes para localização':
			$perid = 1311;
		break;
		case 'Dimensionamento, Arruamento e limitantes inadequados':
			$perid = 1313;
		break;
		case 'Locação não identificada':
			$perid = 1315;
		break;
		case 'Topografia não identificada':
			$perid = 1317;
		break;
		case 'Documentos fora do padrão':
			$perid = 1319;
		break;
		case 'Sem assinatura do prefeito':
			$perid = 1321;
		break;
		case 'Sem assinatura do engenheiro':
			$perid = 1322;
		break;
	}

    if ($_REQUEST['filtro_preid']) {
        $where .= " and pre.preid::text ilike ('%{$_REQUEST['filtro_preid']}%')";
    }
    if ($_REQUEST['filtro_predescricao']) {
        $where .= " and pre.predescricao ilike ('%{$_REQUEST['filtro_predescricao']}%')";
    }
    if ($_REQUEST['filtro_estuf']) {
        $where .= " and m.estuf ilike ('%{$_REQUEST['filtro_estuf']}%')";
    }
    if ($_REQUEST['filtro_mundescricao']) {
        $where .= " and m.mundescricao ilike ('%{$_REQUEST['filtro_mundescricao']}%')";
    }

    $order = ' ORDER BY pre.predescricao ';
    if ($_REQUEST['ordenador_listagem']) {
        $order = " order by " . str_replace('__', ' ', $_REQUEST['ordenador_listagem']);
    }

	$sql = "select DISTINCT
					o.obrid,
					pre.preid,
					pre.predescricao,
					m.estuf,
					m.mundescricao,
					pre.preano,
					m.muncod
			from
					obras.preobra pre
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra = 'P'
			INNER JOIN obras.preobraanalise po ON po.preid = pre.preid AND po.poastatus = 'A'
			INNER JOIN questionario.questionarioresposta qrp ON qrp.qrpid = po.qrpid AND qrp.queid = 49
			INNER JOIN questionario.grupopergunta gp ON gp.queid = qrp.queid
			INNER JOIN questionario.pergunta p on p.grpid = gp.grpid
			INNER JOIN questionario.itempergunta ip on ip.perid = p.perid
			INNER JOIN questionario.resposta r on r.itpid = ip.itpid AND r.qrpid = qrp.qrpid
			INNER JOIN territorios.municipio m on m.muncod = pre.muncod
			INNER JOIN workflow.documento d ON d.docid = pre.docid
			LEFT JOIN obras2.obras o ON o.obrid = pre.obrid
			WHERE pre.prestatus = 'A'
			AND UPPER(ip.itptitulo) = 'NÃO.'
			AND d.esdid NOT IN (193,368)
			AND p.perid = $perid
			{$where}
			{$order}";

	$dados = $db->carregar($sql);
	$totalObras=0;
	?>
	<tr>
		<td class="fundo_td">
			<div>
				<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" ><?=$_REQUEST['parametro']?><br/> </div>
			</div>
			<div style="clear: both;"></div>

			<form action="" method="post" name="formulario_filtro_listagem" id="formulario_filtro_listagem">
                <input type="hidden" name="ordenador_listagem" id="ordenador_listagem" value="<?php echo $_REQUEST['ordenador_listagem']; ?>"/>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >
                            Pré-ID
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="preid" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="preid__desc"/>
						</td>
						<td class="center bold" >
                            Pré-Obra
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="predescricao" />
						    <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="predescricao__desc" />
						</td>
						<td class="center bold" >
    						UF
    						<img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf" />
    						<img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf__desc" />
						</td>
						<td class="center bold" >
    						Município
    						<img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao" />
    						<img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao__desc" />
						</td>
					</tr>
					<tr>
						<td class="center" >
                        	<img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                        	<input type="text" class="filtro_listagem" name="filtro_preid" value="<?php echo $_REQUEST['filtro_preid']; ?>" />
       				    </td>
						<td class="center" >
						    <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_predescricao" value="<?php echo $_REQUEST['filtro_predescricao']; ?>" />
                        </td>
						<td class="center" >
						    <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
						    <input type="text" class="filtro_listagem" name="filtro_estuf" value="<?php echo $_REQUEST['filtro_estuf']; ?>" />
					    </td>
						<td class="center" >
						    <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
						    <input type="text" class="filtro_listagem" name="filtro_mundescricao" value="<?php echo $_REQUEST['filtro_mundescricao']; ?>" />
					    </td>
					</tr>
					<?php
					if ($dados) {
    					foreach($dados as $dado): ?>
    					<tr>
    						<td class="">
    							<img class="link" src="cockpit/images/icone_p.png" style="vertical-align:middle;" width="20" height="20" title="PAR" onclick="abreObrasPac(<?=$dado['preid']?>, <?=$dado['muncod']?>);" />
    							<?php
    							if($dado['obrid']){?>
    								<img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras(<?=$dado['obrid']?>);" />
    							<?php
    							}
    							?>

    							<?php echo $dado['preid'] ?>
    						</td>
    						<td class=""><?php echo $dado['predescricao'] ?></td>
    						<td class=""><?php echo $dado['estuf'] ?></td>
    						<td class=""><?php echo $dado['mundescricao'] ?></td>
    					</tr>
    					<?php
    					$totalObras+=1;
    					endforeach;
					} else { ?>
                        <tr>
                            <td colspan="4" style="text-align: center; font-size: 20px;">Nenhum resultado encontrado!</td>
                        </tr>
					<?php } ?>
					<tr>
						<td class="bold" colspan="3">Total</td>
						<td class="bold numero" ><?php echo $totalObras ?></td>
					</tr>
				</table>
			</form>
		</td>
	</tr>
	</table>
</body>
</html>