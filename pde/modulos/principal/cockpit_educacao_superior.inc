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
  	.fundo_td_vermelho:hover{background-color:#DD0000}
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_azul_escuro{background-color:#152D56}
  	.fundo_td_azul_escuro:hover{background-color:#1F3864}
  	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
  	 body{background-image:url('../imagens/degrade-fundo-preto.png');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{width:98%;height:85px}
  	.bold{font-weight:bold}
  	.esquerda{text-align:left;}
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
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_educacao_superior&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Expansão da Educação Superior</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<!-- Tabela Indicadores-->
		<td class="fundo_td" >
		<?php
			montaTabelaEstrategico('Bolsa Permanência', 144340);
		?>
		</td>
		<td class="fundo_td">
			<div>
				<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Metas <?php echo date("Y") ?></div>
				<div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
					<table class="tabela_box" cellpadding="2" cellspacing="1" align="center" >
						<tr>
							<td>ProUni</td>
							<td class="numero" >Ofertar 260 mil bolsas de estudo</td>
						</tr>
						<tr>
							<td>Expansão da Rede Federal <br/>de Ensino Superior</td>
							<td class="numero" >Implantação de 20 novos câmpus até 2012 <br/>e mais 27 até 2014.</td>
						</tr>
					</table>
				</div>
			</div>
		</td>
		<td class="fundo_td" >
			<div onclick="abreRedeFederal();" >
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
				$totalMun = $db->pegaUm($sql);
				?>
				<img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Mapas das<br/> Universidadess<br/><span class="subtitulo_box" ><a href="#" >Clique no mapa para visualizar os detalhes.</a></span>
				<br /><br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
				</div>
				<img class="link" onclick="abreRedeFederal()" src="../imagens/cockpit/mapa_cockpit.png"  />
			</div>
		</td>
		<!-- Fim Tabela Indicadores-->
		<!-- Tabela Lateral-->
		<td class="fundo_td_azul_escuro" style="vertical-align:top;" rowspan="5" width="20%" >
			<div>
				<div style="float:left;" class="titulo_box" >Obras nos Câmpus</div>
			</div>
			<div style="clear:both" id="div_fotos"  >
			<?php
			$sql = "SELECT count(o.obrid)
					FROM obras.obrainfraestrutura o
					INNER JOIN obras.situacaoobra s on s.stoid = o.stoid
					WHERE o.obsstatus = 'A'
					AND o.orgid = 1
					AND o.stoid not in (99);";
			$totalObras = $db->pegaUm( $sql );

			$sql = "SELECT obr.obrid, obr.obrdesc,
					ent.entnome,
					mun.mundescricao AS municipio,
					mun.estuf AS uf,
					coalesce(obr.obrvlrrealobra,0) as valor,
					arqid
					FROM obras.obrainfraestrutura obr
					INNER JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
					INNER JOIN obras.tipoobra tob ON tob.tobaid = obr.tobraid
					INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
					INNER JOIN entidade.endereco edc ON edc.endid = obr.endid
					INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
					INNER JOIN obras.fotos fot ON fot.obrid = obr.obrid
					WHERE obr.obsstatus = 'A'
					AND obr.orgid = 1
					AND obr.obrpercexec >= 50
					ORDER BY random()
					LIMIT 50;";
			$arrFotos = $db->carregar($sql);
			?>
			<div>
				<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" ><?=number_format($totalObras,0,"",".") ?> Obras</div>
			</div>
			<div style="clear:both" id="div_fotos"  >
					<?php for($x=8;$x>=1;$x--): ?>
						<div style="clear:both" class="div_fotos_interno" >
						<?php for($i=$x;$i<=(10+$x);$i++): ?>
							<div class="div_fotos" style="cursor:pointer;" onclick="acessarObras(<?php echo $arrFotos[$i]['obrid'] ?>);">
								<table border="0" >
									<tr>
										<td>
											<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
										</td>
										<td style="color:#FFFFFF" >
											<?php echo substr($arrFotos[$i]['obrdesc'],0,60) ?>...<br/>
											<?php echo substr($arrFotos[$i]['entnome'],0,60) ?><br/>
											<?=$arrFotos[$i]['municipio'] ?> / <?=$arrFotos[$i]['uf'] ?><br/>
											R$ <?php echo number_format($arrFotos[$i]['valor'],2,",",".") ?>
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
		<td class="fundo_td">
			<div>
				<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div style="float:left; cursor:pointer;" class="titulo_box">Expansão da Rede Federal <br/>de Ensino Superior - Obras<br/></div>
				<div style="clear:both;width:98%" >
					<div style="float:left;width:90%;margin-left:5%;">
						<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
							<?php $sql = "	SELECT s.stodesc, o.stoid, count(s.stodesc) as qtd
											FROM obras.obrainfraestrutura o
											INNER JOIN obras.situacaoobra s on s.stoid = o.stoid
											WHERE o.obsstatus = 'A'
											AND o.orgid = 1
											AND o.stoid not in (11)
											GROUP BY o.stoid, s.stodesc, s.stoid";
							$arrObras = $db->carregar($sql) ?>
							<tr>
	                			<td class="center bold" >Situação</td>
	                			<td class="center bold" >Obras</td>
	                		</tr>
							<?php foreach($arrObras as $o): ?>
							<tr>
								<td style="cursor:pointer;" onclick="abreRelatorioObras(1, 2, '', '', <?=$o['stoid']?>);"><?php echo $o['stodesc'] ?></td>
								<td class="numero" style="cursor:pointer;" onclick="abreRelatorioObras(1, 2, '', '', <?=$o['stoid']?>);"><?php $total_obras+=$o['qtd'];echo number_format($o['qtd'],0,"",".") ?></td>
							</tr>
							<?php endforeach; ?>
							<tr>
	                			<td align="left" class="bold" style="cursor:pointer;" onclick="abreRelatorioObras(1, 2, '', '', '');" >Total</td>
	                			<td class="numero" style="cursor:pointer;" onclick="abreRelatorioObras(1, 2, '', '', '');" ><?php echo number_format($total_obras,0,"",".") ?></td>
	                		</tr>
						</table>
					</div>
				</div>
			</div>
		</td>
		<td class="fundo_td_azul" valign="top" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >ProUni<br/></div>
				<div style="clear:both;width:98%" >
					<div style="float:left;width:90%;margin-left:5%;">
						<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
							<?php $sql = "--Bolsas ofertadas em 2012
										select sum(sehqtde) AS Total
										from painel.indicador i
										inner join painel.seriehistorica sh on sh.indid=i.indid
										inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
										where i.indid in (564)
										and sehstatus <> 'I'" ?>
							<?php //$arrProUni[] = array("desc" => "Bolsas ofertadas em 2012", "qtde" => $db->pegaUm($sql), "indid" => "564") ?>
							<?php $sql = "--Bolsas ativas
											select sum(sehqtde) AS Total
										from painel.indicador i
										inner join painel.seriehistorica sh on sh.indid=i.indid
										inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
										where i.indid in (2892)
										and sehstatus <> 'I'" ?>
							<?php $arrProUni[] = array("desc" => "Bolsas ativas", "qtde" => $db->pegaUm($sql), "indid" => "2892") ?>
							<?php $sql = "select sum(sehqtde) AS Total
										from painel.indicador i
										inner join painel.seriehistorica sh on sh.indid=i.indid
										inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
										where i.indid in (768)
										and sehstatus <> 'I'" ?>
							<?php $arrProUni[] = array("desc" => "Bolsas ocupadas", "qtde" => $db->pegaUm($sql), "indid" => "768") ?>
							<?php $sql = "--Número de formados
											select sum(sehqtde) AS Total
										from painel.indicador i
										inner join painel.seriehistorica sh on sh.indid=i.indid
										inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
										where i.indid in (565)
										and sehstatus <> 'I'" ?>
							<?php $arrProUni[] = array("desc" => "Número de formados", "qtde" => $db->pegaUm($sql), "indid" => "565") ?>
							<tr>
								<td class="center bold" >Bolsas</td>
								<td class="center bold" >Quantidade</td>
							</tr>
							<?php foreach($arrProUni as $p): ?>
								<tr>
									<td style="cursor:pointer;" onclick="abreIndicadorPopUp(<?php echo $p['indid'] ?>)"><?php echo $p['desc'] ?></td>
									<td class="numero" ><?php echo number_format($p['qtde'],0,",",".") ?></td>
								</tr>
							<?php endforeach; ?>
						</table>
					</div>
				</div>
			</div>
		</td>
		<td style="cursor:pointer;" class="fundo_td_azul" valign="top" onclick="abreIndicadorPopUp(1131);" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Novos Câmpus</div>
				<div style="clear:both;width:98%" >
					<div style="float:left;width:90%;margin-left:5%;">
						<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
							<tr>
								<td class="center bold" >Instalações</td>
								<td class="center bold" >Quantidade</td>
							</tr>
							<?php $sql = "	SELECT	case ac.cmpinstalacao
											when 'P' then 'Provisória'
											when 'D' then 'Definitiva'
											when 'S' then 'Sem Instalações'
											end as instalacao,
										count(distinct efc.entid) as qtde_campus
										FROM entidade.funcaoentidade ef
										INNER JOIN entidade.entidade e on e.entid=ef.entid
										INNER JOIN entidade.funentassoc eac ON eac.entid = ef.entid
										INNER JOIN entidade.funcaoentidade efc ON efc.fueid = eac.fueid
										INNER JOIN academico.campus ac on ac.entid=efc.entid
										INNER JOIN entidade.entidade e2 on e2.entid=ac.entid
										WHERE ef.fuestatus='A'
										AND efc.fuestatus='A'
										AND e2.entstatus='A'
										AND ef.funid  in ('12') --Ensino Superior
										AND exiid in (3,5) --Novos
										group by instalacao";
							$arrIntalacoes = $db->carregar($sql) ?>
							<?php foreach($arrIntalacoes as $o): ?>
							<tr>
								<td><?php echo $o['instalacao'] ?></td>
								<td class="numero" ><?php echo number_format($o['qtde_campus'],0,",",".") ?></td>
							</tr>
						<?php
						$totalInstalacoes = $totalInstalacoes + $o['qtde_campus'];
						endforeach; ?>
						<tr>
								<td class="bold" >Total</td>
								<td class="numero bold" ><?=number_format($totalInstalacoes,0,"",".") ?></td>
						</tr>
						</table>
						<br><br>
						<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
							<tr>
								<td class="center bold" >Situação</td>
								<td class="center bold" >Quantidade</td>
							</tr>
							<?php $sql = "	SELECT	case ac.cmpsituacao
													   when 'F' then 'Funcionando'
													   when 'N' then 'Não Funcionando'
												end as funcionamento,
										count(distinct efc.entid) as qtde_campus
										FROM entidade.funcaoentidade ef
										INNER JOIN entidade.entidade e on e.entid=ef.entid
										INNER JOIN entidade.funentassoc eac ON eac.entid = ef.entid
										INNER JOIN entidade.funcaoentidade efc ON efc.fueid = eac.fueid
										INNER JOIN academico.campus ac on ac.entid=efc.entid
										INNER JOIN entidade.entidade e2 on e2.entid=ac.entid
										WHERE ef.fuestatus='A'
										AND efc.fuestatus='A'
										AND e2.entstatus='A'
										AND ef.funid  in ('12') --Ensino Superior
										AND exiid in (3,5) --Novos
										group by funcionamento
										order by funcionamento";
							$arrSituacao = $db->carregar($sql) ?>
							<?php foreach($arrSituacao as $o): ?>
							<tr>
								<td><?php echo $o['funcionamento'] ?></td>
								<td class="numero" ><?php echo number_format($o['qtde_campus'],0,",",".") ?></td>
							</tr>
						<?php
						$totalSituacao = $totalSituacao + $o['qtde_campus'];
						endforeach; ?>
						<tr>
								<td class="bold" >Total</td>
								<td class="numero bold" ><?=number_format($totalSituacao,0,"",".") ?></td>
						</tr>
						</table>
					</div>
				</div>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Novo FIES<span style="margin-left:40px" class="subtitulo_box" ><a href="http://sisfiesaluno.mec.gov.br/wap" target="_blank" >WAP</a></span></div>
				<div style="clear:both;width:100%;font-size:30px;font-weight:bold;text-align:center" >
					<table class="tabela_box" cellpadding="2" cellspacing="1" align="center" >
						<tr>
							<td></td>
							<td class="center bold" >Total</td>
							<td class="center bold" >FGEDUC</td>
						</tr>
						<!--<tr>
							<td class="esquerda" >Mantenedoras</td>
							<td class="numero">1.022</td>
							<td class="numero" >338</td>
						</tr>-->
							<?php
							$sql = "select SUM(sh.sehqtde) AS Total
									from painel.indicador i
									inner join painel.seriehistorica sh on sh.indid=i.indid
									where i.indid in (778)
									and sehstatus <> 'I'";
							$totalContrato = $db->pegaUm( $sql );

							$sql = "select SUM(sh.sehqtde) AS Total
									from painel.indicador i
									inner join painel.seriehistorica sh on sh.indid=i.indid
									where i.indid in (779)
									and sehstatus <> 'I'";
							$totalFGDUC = $db->pegaUm( $sql );
							?>
						<tr>
							<td class="esquerda" >Contratos firmados</td>
							<td style="cursor:pointer;" class="numero" onclick="abreIndicadorPopUp(778)"><?php echo number_format($totalContrato,0,",",".") ?></td>
							<td style="cursor:pointer;" class="numero" onclick="abreIndicadorPopUp(779)"><?php echo number_format($totalFGDUC,0,",",".") ?></td>
						</tr>
						<!--<tr>
							<td class="esquerda" >Valor contratado</td>
							<td class="numero" >X</td>
							<td class="numero" >X</td>
						</tr>-->
					</table>
				</div>
			</div>
		</td>
		<!-- Tabela Financeiro -->
		<td class="fundo_td" colspan="2" rowspan="3" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Orçamentário / Financeiro</div>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" rowspan="2" >Descrição</td>
					<td class="center bold" colspan="2" >2011</td>
					<td class="center bold" colspan="2" >2012</td>
				</tr>
				<tr>
					<td class="center bold" >Valor empenhado</td>
					<td class="center bold" >Valor pago</td>
					<td class="center bold" >Valor empenhado</td>
					<td class="center bold" >Valor pago</td>
				</tr>
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

			   $sql = "select
						 descricao,
						 sum(dotacao2011) as dotacao2011,
						 sum(empenhado2011) as empenhado2011,
						 sum(pago2011) as pago2011,
						 sum(dotacao2012) as dotacao2012,
						 sum(empenhado2012) as empenhado2012,
						 sum(pago2012) as pago2012
						from (

						select
						 Case --plicod
						  when plicod in ( 'PSS07G0101N' ) then 'FIES - FGEDUC'
						  when plicod in ( 'PSS07G99ADN' ) then 'FIES - EMISSÃO'
						  when plicod in ( 'PSS07G99RCN' ) then 'FIES - RECOMPRA'
						  when plicod in ( 'PSS07N01TXN' ) then 'FIES - TAXA DE ADMINISTRAÇÃO'
						 end as descricao,
						 sum(dotacao2011) as dotacao2011,
						 sum(empenhado2011) as empenhado2011,
						 sum(pago2011) as pago2011,
						 sum(dotacao2012) as dotacao2012,
						 sum(empenhado2012) as empenhado2012,
						 sum(pago2012) as pago2012
						from
						(
						SELECT
							plicod,
							plidsc,
							sum(valor1) AS dotacao2011, sum(valor2) AS empenhado2011, sum(valor4) + sum(valor5) AS pago2011,
							0 AS dotacao2012, 0 AS empenhado2012, 0 AS pago2012
							FROM
							(SELECT
							   sld.plicod,
							   pli.plidsc,
							   CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130201','292130202','292130209','292130301') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor4,CASE WHEN sld.sldcontacontabil in ('295210201','295210202') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor5
								  FROM
							   dw.saldo2011 sld
							   LEFT JOIN dw.planointerno pli ON pli.plicod = sld.plicod  and pli.unicod = sld.unicod
							   WHERE length(sld.plicod) = 11 AND sld.plicod in
							   (
							   'PSS07G0101N', --FIES - FGEDUC
							   'PSS07G99ADN', --FIES - EMISSÃO
							   'PSS07G99RCN', --FIES - RECOMPRA
							   'PSS07N01TXN' --FIES - TAXA DE ADMINISTRACAO
							   )

						 AND
						 sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302','292130100','292130201','292130202','292130203','292130301','292130201','292130202','292130209','292130301','292130301','292410403','295210201','295210202')
						) as foo
							WHERE
							valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 OR valor5 <> 0
							GROUP BY
							plicod,
							plidsc

						union all

						SELECT
							plicod,
							plidsc,
							0 AS dotacao2011, 0 AS empenhado2011, 0 AS pago2011,
							sum(valor1) AS dotacao2012, sum(valor2) AS empenhado2012, sum(valor4) + sum(valor5) AS pago2012
							FROM
							(SELECT
							   sld.plicod,
							   pli.plidsc,
							   CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130201','292130202','292130209','292130301') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor4,CASE WHEN sld.sldcontacontabil in ('295210201','295210202') THEN
							CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0 END AS valor5
								  FROM
							   dw.saldo2012 sld
							   LEFT JOIN dw.planointerno pli ON pli.plicod = sld.plicod  and pli.unicod = sld.unicod
						 WHERE length(sld.plicod) = 11 AND sld.plicod in
							   (
							   'PSS07G0101N', --FIES - FGEDUC
							   'PSS07G99ADN', --FIES - EMISSÃO
							   'PSS07G99RCN', --FIES - RECOMPRA
							   'PSS07N01TXN' --FIES - TAXA DE ADMINISTRACAO
							   )
						 AND sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302','292130100','292130201','292130202','292130203','292130301','292130201','292130202','292130209','292130301','292130301','292410403','295210201','295210202')) as foo
							WHERE
							valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 OR valor5 <> 0
							GROUP BY
							plicod,
							plidsc

						) as foo
						group by plicod
						) as f
						group by descricao";
			   $arrDadosFin = $db2->carregar($sql,null,3200);
				?>
				<?php foreach($arrDadosFin as $dado): ?>
				<tr>
					<td class="" ><?php echo $dado['descricao'] ?></td>
					<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=planointerno&agrupadorColunas[0]=6&agrupadorColunas[1]=168&agrupadorColunas[2]=7&agrupadorColunas[3]=92&planointerno[0]=PSS07G0101N&planointerno[1]=PSS07G99ADN&planointerno[2]=PSS07G99RCN&planointerno[3]=PSS07N01TXN&alterar_ano=0')" ><?php $total_empenhado2011+=$dado['empenhado2011']; echo number_format($dado['empenhado2011'],2,",",".") ?></td>
                	<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=planointerno&agrupadorColunas[0]=6&agrupadorColunas[1]=168&agrupadorColunas[2]=7&agrupadorColunas[3]=92&planointerno[0]=PSS07G0101N&planointerno[1]=PSS07G99ADN&planointerno[2]=PSS07G99RCN&planointerno[3]=PSS07N01TXN&alterar_ano=0')" ><?php $total_pago2011+=$dado['pago2011']; echo number_format($dado['pago2011'],2,",",".") ?></td>
                	<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=planointerno&agrupadorColunas[0]=6&agrupadorColunas[1]=168&agrupadorColunas[2]=7&agrupadorColunas[3]=92&planointerno[0]=PSS07G0101N&planointerno[1]=PSS07G99ADN&planointerno[2]=PSS07G99RCN&planointerno[3]=PSS07N01TXN&alterar_ano=0')" ><?php $total_empenhado2012+=$dado['empenhado2012']; echo number_format($dado['empenhado2012'],2,",",".") ?></td>
                	<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=planointerno&agrupadorColunas[0]=6&agrupadorColunas[1]=168&agrupadorColunas[2]=7&agrupadorColunas[3]=92&planointerno[0]=PSS07G0101N&planointerno[1]=PSS07G99ADN&planointerno[2]=PSS07G99RCN&planointerno[3]=PSS07N01TXN&alterar_ano=0')" ><?php $total_pago2012+=$dado['pago2012']; echo number_format($dado['pago2012'],2,",",".") ?></td>
				</tr>
				<?php endforeach; ?>
				<tr>
					<td class="bold" >Total</td>
					<td class="numero bold" ><?php echo number_format($total_empenhado2011,2,",",".") ?></td>
					<td class="numero bold" ><?php echo number_format($total_pago2011,2,",",".") ?></td>
					<td class="numero bold" ><?php echo number_format($total_empenhado2012,2,",",".") ?></td>
					<td class="numero bold" ><?php echo number_format($total_pago2012,2,",",".") ?></td>
				</tr>
			</table>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" style="cursor:pointer;" onclick="abreIndicadorPopUp(382);">
			<div>
				<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Total de câmpus</div>
			</div>
			<?php
			$sql = "select
						tdi.tiddsc,
						sum(dsh.dshqtde) AS qtde
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tdi ON tdi.tidid = dsh.tidid2
					where sh.indid = 382
					and sehstatus <> 'I'
					and tdi.tidid IN (1476, 2258, 2587, 2588)
					and dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					group by tdi.tiddsc";
			$arrCampus = $db->carregar($sql);
			?>
			<div style="clear:both;" >
			<table class="tabela_box" cellpadding="2" cellspacing="1" align="center" width="90%" >
				<tr>
					<td class="center bold" >Status</td>
					<td class="center bold" >Quantidade</td>
				</tr>
				<?php foreach($arrCampus as $c): ?>
					<tr>
						<td><?php echo $c['tiddsc'] ?></td>
						<td class="numero" ><?php $totalqtde+=$c['qtde']; echo number_format($c['qtde'],0,",",".") ?></td>
					</tr>
				<?php endforeach; ?>
				<tr>
					<td class="bold">Total</td>
					<td class="numero" ><?php echo number_format($totalqtde,0,",",".") ?></td>
				</tr>
			</table>
			</div>
		</td>
	</tr>
	<tr>
		<!-- Tabela Processos -->
		<td class="fundo_td" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Mapa do Processo<br/>
				<span class="subtitulo_box" ><a href = "http://escritoriodeprocessos.mec.gov.br/processos/pde-ae/Caminho%20Cr%C3%ADtico%20-%20Expans%C3%A3o%20da%20Rede%20Federal%20de%20Ensino%20Superior.htm" target="_blank">Expansão</a></span><br/>
				<span class="subtitulo_box" ><a href = "http://escritoriodeprocessos.mec.gov.br/processos/prouni/default.htm" target="_blank">Prouni</a></span><br/>
				<span class="subtitulo_box" ><a href = "http://escritoriodeprocessos.mec.gov.br/processos/sisfies/default.htm" target="_blank">FIES</a></span>
				</div>
			</div>
		</td>
		<!-- Tabela Processos -->
	</tr>
	<tr>
		<td class="fundo_td_laranja" colspan="4">
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