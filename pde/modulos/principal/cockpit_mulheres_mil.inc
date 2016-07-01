<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body onload="refreshAutomatico();">
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
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_vermelho:hover{background-color:#DD0000}
  	.div_fotos{background-color:#7B68EE;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
  	 body{background-image:url('../imagens/fundo_cockpit.jpg');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/cockpit/fundo_pronacampo.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{margin-bottom:2px;width:98%;height:60px}
  	.bold{font-weight:bold}
  	.link{cursor:pointer}
 </style>
<script>
	jQuery.noConflict();

	jQuery('.div_fotos_interno').cycle({
	    fx: 'scrollDown'
	});

	atualizaUsuario();
</script>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_mulheres_mil&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
  <!-- Título-->
  <tr>
                <td class="fundo_titulo" style="text-align:center" colspan="5" ><br>Mulheres Mil<br><br></td>
  </tr>
  <!-- Fim Título-->
  <tr>
                <!-- Tabela Indicadores-->
                <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><a href="#" onclick="abreIndicadores(129250,'')"  >Indicadores</a><br/>
                			<span class="subtitulo_box" >Impacto | Produto | Processo</span>
                		</div>
                	</div>
                </td>

				<td class="fundo_td" style="cursor:pointer;" onclick="abreIndicadorPopUp(728);">
					<div style="clear:both" >
						<img style="float:left;width: 50px;height: 50px" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
						<div style="float:left" class="titulo_box" >Metas e Matrículas</div>
					</div>
					<?php
					$sql = "select
							case dsh.tidid1
								when 3140 then 2011
								when 3141 then 2012
								when 3142 then 2013
							end as ano,
							case dsh.tidid1
								when 3140 then 10000
								when 3141 then 20000
								when 3142 then 30000
							end as meta,
							sum(dsh.dshqtde) as total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (728)
						and sh.sehstatus <> 'I'
						and dsh.tidid1 IN (3140, 3141)
						group by ano, meta
						order by ano, meta";
					$arrDados = $db->carregar($sql,null,3200);
					?>
					<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td class="center bold" >Ano</td>
						<td class="center bold" >Metas</td>
						<td class="center bold" >Matrículas</td>
					</tr>
					<?php foreach($arrDados as $dado): ?>
					<tr>
						<td class="" ><?php echo $dado['ano'] ?></td>
						<td class="numero" ><?php echo number_format($dado['meta'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['total'],0,",",".") ?></td>
					</tr>
					<?php
					$totalgeral += $dado['total'];
					endforeach;
					?>
					<tr>
						<td class="" >2013</td>
						<td class="numero" >30.000</td>
						<td class="numero" >-</td>
					</tr>
					<tr>
						<td class="" >2014</td>
						<td class="numero" >40.000</td>
						<td class="numero" >-</td>
					</tr>
					<tr>
						<td class="bold" >Total</td>
						<td class="numero" >100.000</td>
						<td class="numero" ><?php echo number_format($totalgeral,0,",",".") ?></td>
					</tr>
				</table>
				</td>

                <!-- Tabela Lateral-->
                <td class="fundo_td_roxo" style="vertical-align:top;" rowspan="3">
                	<div>
                		<img style="float:left;" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                		<div style="float:left;" class="titulo_box" ><div style="float:left;" class="titulo_box" >Câmpus participantes</div></div>
                	</div>
                	<div style="clear:both" id="div_fotos">
	                	<?php

	                	$sql = "select entcampus.entnome as campus, entinst.entnome as instituicao, sum(dsh.dshqtde) as total, UPPER(dsh.dshuf) as uf
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join entidade.entidade entcampus on entcampus.entid = dsh.entid
								inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 17
								inner join entidade.funentassoc a on a.fueid = fn.fueid
								inner join entidade.entidade entinst on entinst.entid = a.entid
								where i.indid in (728)
								and sh.sehstatus <> 'I'
								group by entcampus.entnome, entinst.entnome, uf
								ORDER BY random()
								LIMIT 50";
               				$arrFotos = $db->carregar($sql);
	                		?>
		                	<?php for($x=9;$x>=1;$x--): ?>
		                		<div style="clear:both;" class="div_fotos_interno"  >
	                			<?php for($i=$x;$i<=(10+$x);$i++): ?>
		                			<div class="div_fotos" >
				                		<table border="0" >
				                			<tr>
												<td style="color:#FFFFFF" >
													<img src="./cockpit/images/estados/<?php echo $arrFotos[$i]['uf'] ?>.jpg" width="90px" height="50px"  />
												</td>
				                				<td style="color:#FFFFFF;" >
				                					<?php echo $arrFotos[$i]['campus']?><br/>
													<?php echo $arrFotos[$i]['instituicao']?><br/>
													<?php echo number_format($arrFotos[$i]['total'],0,"",".") ?> matrículas
				                				</td>
				                			</tr>
				                		</table>
			                		</div>
		                		<?php endfor; ?>
	                			</div>
	                	<?php endfor; ?>
	                </div>
                </td>
                <!-- Fim Tabela Lateral-->
  </tr>
  <tr>

                <td class="fundo_td" valign="top">
                	<div onclick="abreIndicadorPopUp(1256);" >
					<?php
					$sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica where indid = 1256 and sehstatus != 'I';";
					$arrValores = $db->pegaLinha($sql);
					$sql = "select
								count(distinct dshcodmunicipio)
							from
								painel.seriehistorica seh
							inner join
								painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
							where
								indid = 1256
							and
								seh.sehstatus != 'I';";
					$totalMun = $db->pegaUm($sql);
					?>
	                <img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
					<div style="float:left;cursor:pointer;" class="titulo_box" >Mapa – Câmpus que aderiram<br/>ao Mulheres Mil<br/><span class="subtitulo_box" ><a href="#" >Clique no mapa para visualizar os detalhes.</a></span>
					<br /><br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >câmpus</span>
					<br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
					</div>
				</div>
				<img class="link" onclick="abreIndicadorPopUp(1256);" src="../imagens/cockpit/mapa_cockpit.png"  />
           </td>
		   <td class="fundo_td" style="cursor:pointer;" onclick="abreIndicadorPopUp(728);">
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Matrículas acumuladas por UF</div>
                	</div>
                	<?
					$sql = "select dsh.dshuf, sum(dsh.dshqtde) as qtde
							from painel.indicador i
							inner join painel.seriehistorica sh on sh.indid=i.indid
							inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
							where i.indid in (728)
							and sh.sehstatus <> 'I'
							group by dsh.dshuf
							order by dsh.dshuf";
					$arrUF = $db->carregar($sql);
                	?>
                	<div style="clear:both;" >
                			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
	                			<?php for($i=0;$i<=13;$i++): ?>
			                		<tr>
										<td class="bold"><?php echo $arrUF[$i]['dshuf'] ?></td>
										<td class="numero"><?php $total_UF+=$arrUF[$i]['qtde']; echo number_format($arrUF[$i]['qtde'],0,"",".") ?></td>
										<?php if($arrUF[$i+14]['dshuf']): ?>
											<td class="bold"><?php echo $arrUF[$i+14]['dshuf'] ?></td>
											<td class="numero"><?php $total_UF+=$arrUF[$i+14]['qtde'];echo number_format($arrUF[$i+14]['qtde'],0,"",".") ?></td>
										<?php else: ?>
											<td class="bold">Total</td>
											<td class="numero"><?php echo number_format($total_UF,0,"",".") ?></td>
										<?php endif; ?>
			                		</tr>
	                			<?php endfor; ?>
		                	</table>
                		</div>
                </td>
  </tr>
  <tr>

                <td class="fundo_td" colspan="2" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Orçamentário / Financeiro<br/></div>
                	</div>
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

					  $sql = "SELECT
    '2012' AS exercicio,
    sum(valor1) AS empenhado,sum(valor3) + sum(valor4) AS pago
    FROM
    (SELECT
       sac.sbacod,
       sac.sbatitulo,
       CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
    ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130201','292130202','292130209','292130301') THEN
    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
    ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
    ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('295210201','295210202') THEN
    CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
    ELSE 0 END AS valor4
          FROM
       dw.saldo2012 sld
       INNER JOIN financeiro.subacao sac ON sac.sbastatus = 'A' AND sac.sbacod = substr(sld.plicod, 2, 4)
       WHERE substr(sld.plicod, 2, 4) in ('PP12') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130201','292130202','292130209','292130301','292130301','292410403','295210201','295210202')) as foo
    WHERE
    valor1 <> 0 OR valor3 <> 0 OR valor4 <> 0
    GROUP BY
    sbacod,
    sbatitulo
    ORDER BY
    sbacod,
    sbatitulo";
                $arrDados = $db2->carregar($sql,null,3200);

                	?>
                	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                		<tr>
                			<td class="center bold" >Exercício</td>
                			<td class="center bold" >Empenhado</td>
                			<td class="center bold" >Pago</td>
                		</tr>
                		<?php foreach($arrDados as $dado): ?>
	                		<tr class="link" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['exercicio'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=168&agrupadorColunas[2]=7&agrupadorColunas[3]=92&subacao[0]=PP12&alterar_ano=0')">
	                			<td class="" ><?php echo $dado['exercicio'] ?></td>
	                			<td class="numero" ><?php echo number_format($dado['empenhado'],2,",",".") ?></td>
	                			<td class="numero" ><?php echo number_format($dado['pago'],2,",",".") ?></td>
	                		</tr>
		            	<?php endforeach; ?>
		            </table>
                </td>
                <!-- Tabela Financeiro -->
  </tr>
  <tr>
                <td class="fundo_td_laranja" colspan="5">
                	<div style="text-align:center;"  >
                		<img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
                		<input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
                		<img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
                	</div>
                </td>
  </tr>
</table>
</body>
</html>