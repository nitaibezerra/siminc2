<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body> 
<?php
function pegaUsuarioOnline(){
	global $db;
	
	$sql = "select COALESCE(count(*),0) as usu_online
			from seguranca.usuariosonline
			where sisid in (99,15,23)";
	$usu = $db->pegaUm($sql);
	return	($usu ? $usu : 0) . ' <span class="subtitulo_box" >On-line<br/>'.date("d/m/Y").'<br>'.date("g:i:s").'</span>';
}

if( $_REQUEST['useronline'] ){
	echo pegaUsuarioOnline();
	exit;
}

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
 <style>
  	.fundo_td{background-color:#0F6D39}
  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
  	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF;}
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
  	.div_fotos{background-color:#7B68EE;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
  	body{background-image:url('../imagens/fundo_cockpit.jpg');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/fundoquadras.jpg');background-repeat:repeat-xt;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{margin-bottom:2px;width:98%}
  	.link{cursor:pointer}
  	.bold{font-weight:bold}
 </style>
<script>

	jQuery.noConflict();

	//jQuery(function() {
		jQuery('.div_fotos_interno').cycle({ 
		    fx: 'scrollDown' 
		});
	//});

	atualizaUsuario();
	
	function atualizaUsuario(){	
	  	jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "useronline=1",
		   success: function(msg){
		   		jQuery('#usuOnline').html( msg );
		   }
		});
		window.setTimeout('atualizaUsuario()', 5000);
	}
    function abreUsuarios(){
    	window.open(
						'../geral/usuarios_online2.php',
						'usuariosonline',
						'height=500,width=600,scrollbars=yes,top=50,left=200'
					);
    }
    function abreMapa(tipo, tooid){
    	window.open('../obras/obras.php?modulo=relatorio/mapa_resultado&acao=A&painel='+tipo+'&tooid='+tooid);
    }
    function acessarTermos() {
		window.open('/par/par.php?modulo=principal/termoPac&acao=A&assinado=1&tipoobra=Q&pesquisar=1', '_blank');
	}
    
	function acessarQuadras(supvid, obrid) {
		window.open('/obras/obras.php?modulo=principal/inserir_vistoria&acao=A&obrid='+obrid+'&supvid='+supvid, '_blank');
	}

	function acessarCallCenter() {
		window.open('/callcenter/callcenter.php?modulo=principal/temas/termopac&acao=A&temid=1&ligacao=1&pesquisar=1', '_blank');
	}

	function buscar(busca) {
		window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit='+busca,'Observações','scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
	}

	function abreIndicadores()
	{
		window.open('/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A&atiprojeto=129596','Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	function acessarPainelGerenciamento(situacao) {
		window.open('/par/par.php?modulo=principal/painelGerenciamento&acao=A&painel_estrategico=2&situacao='+situacao, '_blank');	
	}
	function abreMapasMeta(parametros)
	{
	 	url = "/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=24&carregaMapaAutomativo=1&cmb_tema=" + parametros;
	 	window.open(url,'_blank');
	}
	function abreRelatorio(params)
	{
	window.open('/financeiro/financeiro.php?modulo=relatorio/geral_teste&acao=R&'+params,'Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}

	function abrePainel(indid)
	{
		var url = "../painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&indid=" + indid + "&cockpit=1";
		window.open(url,'Painel','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	
	function abreRelatorioObras(orgid, filtroagrupador, prfid, tooid, stoid) {
		window.open('/obras/obras.php?modulo=relatorio/relatorio_geral&acao=A&orgid=' + orgid + '&filtroagrupador=' + filtroagrupador +'&prfid=' + prfid +'&tooid=' + tooid +'&stoid=' + stoid, '_blank');
	}
</script>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location.href='painel_par_cockpit.php';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Painel Estratégico</span></div>
		</div>
		<div style="float:right;cursor:pointer;" onclick="window.location.href=window.location;">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
  <!-- Título-->
  <tr>
                <td class="fundo_titulo" style="text-align:center" colspan="6" ><br>Construção e cobertura de quadras esportivas escolares<br><br></td>
  </tr>
  <!-- Fim Título-->
  <tr>
                <!-- Tabela Indicadores-->
                <td class="fundo_td" >
                	<?php $sql = "select mtinivel, mtidsc as descricao, count(*) as total from painel.indicador i
								inner join pde.monitoratipoindicador mti ON mti.mtiid = i.mtiid
								and acaid = (select atiacaid from pde.atividade where atiid = 129596)
								group by mtinivel, mtidsc
								order by mtinivel"; 
                			$dados = $db->carregar( $sql );?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><a href="#" onclick="abreIndicadores()"  >Indicadores</a><br/>
                			<?php if(1==2): ?>
                				<span class="subtitulo_box" ><?=$dados[0]['descricao'] ?>: <?=$dados[0]['total'] ?> | <?=$dados[1]['descricao'] ?>: <?=$dados[1]['total'] ?> | <?=$dados[2]['descricao'] ?>: <?=$dados[2]['total'] ?></span>
                			<?php else: ?>
                				<span class="subtitulo_box" >Impacto | Produto | Processo</span>
                			<?php endif; ?>
                		</div>
                	</div>
                </td>
                <td class="fundo_td" >
                <?
				$sql = "select 
							sehqtde 
						from 
							painel.seriehistorica seh
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						where 
							seh.indid = 635
						and
							dpedsc = '2011';";
				$qtde_2011 = $db->pegaUm($sql);
                ?>
                	<div style="cursor:pointer" onclick="abrePainel(635);" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="abrePainel(635);" class="titulo_box" ><?=number_format($qtde_2011,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>em 2011</span></div>
                	</div>
                </td>
                <td class="fundo_td" >
                <?
				$sql = "select 
							sehqtde 
						from 
							painel.seriehistorica seh
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						where 
							seh.indid = 635
						and
							dpedsc = '2012';";
				$qtde_2012 = $db->pegaUm($sql);
                ?>
                	<div style="cursor:pointer" onclick="abrePainel(635);" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="abrePainel(635);" class="titulo_box" ><?=number_format($qtde_2012,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>em 2012</span></div>
                	</div>
                </td>
                <td class="fundo_td" >
                <?
				$sql = "select 
							sum(sehqtde) 
						from 
							painel.seriehistorica seh
						where 
							seh.indid = 635
						and
							seh.sehstatus != 'I'";
				$quantidadeaprovada = $db->pegaUm($sql);
                ?>
                	<div style="cursor:pointer" onclick="abrePainel(635);" >
                		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="abrePainel(635);" class="titulo_box" ><?=number_format($quantidadeaprovada,0,"",".") ?><br/><span class="subtitulo_box" >Aprovadas<br/>até <?php echo date("d/m/Y") ?></span></div>
                	</div>
                </td>
                <td class="fundo_td">
                	<div>
                		<img style="float:left;width: 50px;height: 50px" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Meta 2014<br>10.116<br/><span class="subtitulo_box" >Contrução e cobertura</span></div>
                	</div>
                </td>
                <!-- Fim Tabela Indicadores-->
                <!-- Tabela Lateral-->
                <td class="fundo_td_roxo" style="vertical-align:top;"  rowspan="4">
                <?
                // necessita depara stoid = esdid
//				$sql = "select count(*) from obr as.ob rainfraestrutura o 
//						where obsstatus='A' and o.stoid in (1,3) and o.prfid in(50,55)";
				$sql = "SELECT count(*) FROM obras2.obras o 
						INNER JOIN workflow.documento d ON d.docid = o.docid
						WHERE obrstatus='A' AND d.esdid IN (".OBR_ESDID_EM_EXECUCAO.",".OBR_ESDID_CONCLUIDA.") AND o.prfid IN (50,55)";
				$crechesfuncionamento = $db->pegaUm($sql);
                ?>
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="window.open( '/obras/obras.php?modulo=relatorio/relatorio_geral&acao=A&prtid=1499&pesquisa=1&form=1', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );" class="titulo_box" ><?=number_format($crechesfuncionamento,0,"",".") ?> Quadras Concluídas <br/> ou Em Construção</div>
                	</div>
                	<div style="clear:both" id="div_fotos"  >
	                	<?php
	                	//necessita depara stoid = esdid
//	                	$sql = "select p.arqid, 
//	                				   p.arqdescricao, 
//	                				   o.obrdesc,
//	                				   mun.mundescricao,
//	                				   mun.estuf,
//	                				   tpodsc,
//	                				   o.obrid,
//	                				   f.supvid
//	                				    from obr as.ob rainfraestrutura o 
//								inner join obras.fotos f on f.obrid=o.obrid 
//								inner join public.arquivo p on p.arqid=f.arqid 
//								inner join entidade.endereco ed on o.endid = ed.endid 
//								inner join territorios.municipio mun on mun.muncod = ed.muncod 
//								inner join obras.tipologiaobra tpl on o.tpoid = tpl.tpoid
//								where obsstatus='A' and arqdescricao != ''
//								and ((o.obrpercexec >=50 and o.prfid in(50,55) and o.stoid not in (11)) or o.stoid in (3) )
//								and supvid in(select max(supvid) from obras.supervisao where supstatus='A' and obrid=o.obrid)
//								and o.obrid not in (1828)
//								order by random() limit 50";
								
							$sql = "SELECT 
										p.arqid, 
										p.arqdescricao, 
									   	o.obrnome,
									   	mun.mundescricao,
									   	mun.estuf,
									   	tpodsc,
									   	o.obrid,
									   	f.supvid
									FROM obras2.obras o 
									INNER JOIN workflow.documento 		doc ON doc.docid = o.docid
									INNER JOIN obras2.empreendimento 	emp ON emp.empid = o.empid
									INNER JOIN obras.fotos 				f   ON f.obrid=o.obrid 
									INNER JOIN public.arquivo 			p   ON p.arqid=f.arqid 
									INNER JOIN entidade.endereco 		ed  ON o.endid = ed.endid 
									INNER JOIN territorios.municipio 	mun ON mun.muncod = ed.muncod 
									INNER JOIN obras.tipologiaobra 		tpl ON o.tpoid = tpl.tpoid
									WHERE 
										obrstatus='A' AND arqdescricao != ''
										AND ((o.obrpercentultvistoria >=50 AND emp.prfid in(50,55) AND doc.esdid NOT IN (770)) OR doc.esdid IN (693) )
										AND supvid IN (SELECT max(supvid) FROM obras.supervisao WHERE supstatus='A' AND obrid=o.obrid)
										AND o.obrid NOT IN (1828)
									ORDER BY random() LIMIT 50";
	                	
               				$arrFotos = array(); //$db->carregar($sql);
	                		?>
		                	<?php for($x=8;$x>=1;$x--): ?>
		                		<div style="clear:both" class="div_fotos_interno"  >	
	                			<?php for($i=$x;$i<=(10+$x);$i++): ?>
		                			<div class="div_fotos" onclick="acessarQuadras('<?=$arrFotos[$i]['supvid'] ?>','<?=$arrFotos[$i]['obrid'] ?>');" >
				                		<table border="0" >
				                			<tr>
				                				<td>
				                					<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
				                				</td>
				                				<td style="color:#FFFFFF" >
				                					<?php echo substr($arrFotos[$i]['obrdesc'],0,20) ?>...<br/>
													<?php echo $arrFotos[$i]['mundescricao'] ?> / <?php echo $arrFotos[$i]['estuf'] ?><br/>
													<?php echo $arrFotos[$i]['tpodsc'] ?> 
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
    <!-- Tabela Mapa -->
                <td class="fundo_td" style="width: 20%;">
                	<div>
                		<div style="float:left; text-align: justify" class="titulo_box">O que é<br/><span class="subtitulo_box">Em um país com as dimensões continentais do Brasil, não basta oferecer a escola, 
                			é preciso ajudar os estudantes a chegarem a ela. E é isso o que o FNDE faz por meio do Programa Nacional de Apoio ao Transporte do Escolar (Pnate) e do Caminho da Escola. 
                			Esses dois programas oferecem aos alunos das escolas públicas do ensino básico, prioritariamente nas zonas rurais, os meios para vencer as distâncias e construir um futuro melhor.</span></div>
                	</div>
                </td>
                <!-- Fim Tabela Mapa -->
                <!-- Obras -->
                <td class="fundo_td_azul" colspan="4" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Pactuação<br/></div>
                		<div style="clear:both;width:98%" >
		                	<div style="float:left;width:90%;margin-left:5%;">	
			                	<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr>
		                			<td></td>
		                			<td class="center bold" ><b>Construção</b></td>
		                			<td class="center bold" ><b>Cobertura</b></td>
		                			<td class="center bold" ><b>Total</b></td>
		                		</tr>
			                	<?php
									$sql = "";
									
									$arrDados = array(); //$db->carregar( $sql, null, 3200 );
			                	
									foreach( $arrDados as $dado ){
			                	?>
			                		<tr>
			                			<td><img border="0" style="cursor:pointer" onclick="abreMapasMeta('<?=$dado['codigo'] ?>')" src="/imagens/icone_br.png" title="Exibir Mapa"> <img style="cursor:pointer;background-color:#FFFFFF" onclick="acessarPainelGerenciamento('<?=$dado['codigo'] ?>');" src="../imagens/consultar.gif"> <?=str_replace(array("1.","2.","3.","4.","5."),"",$dado['situacao'])?></td>
			                			<td class="numero" ><?=number_format($dado['quantidadequad'],0,"",".") ?></td>
			                			<td class="numero" ><?=number_format($dado['quantidadecobe'],0,"",".") ?></td>
			                			<td class="numero" ><?=number_format($dado['quantidadequad']+$dado['quantidadecobe'],0,"",".") ?></td>
			                		</tr>
			                	<?php } ?>
			                	</table>
			                </div>
		                </div>
                	</div>
                </td>
    			<!-- Fim Obras -->
  </tr>
  <tr>
                <!-- Tabela Financeiro -->
                 <td class="fundo_td" style="width: 20%;">
                	<div>
                		<div style="float:left; text-align: justify" class="titulo_box">Como participar<br/><span class="subtitulo_box">A transferência do recurso é automática, com base na quantidade de alunos transportados da zona rural e 
                			informada no Censo Escolar do ano anterior. Por isso, é muito importante que os dados do Censo, realizado pelo Instituto Nacional de Estudos e Pesquisas Educacionais Anísio 
                			Teixeira (Inep), sejam preenchidos corretamente. O valor transferido por aluno/ano varia de acordo com o percentual da população abaixo da linha de pobreza, 
                			o tamanho do município e a sua nota no Índice de Desenvolvimento da Educação Básica (Ideb). A lista dos repasses em todo o país está disponível em 
                			www.fnde.gov.br/pls/simad/internet-fnde.liberacoes-01-pc.</span></div>
                	</div>
                </td>
                <td class="fundo_td" colspan="4" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Orçamentário / Financeiro<br/></div>
                	</div>
                	<?
                	
					$sql = "";
					
					$dadosfinanceiros = array(); //$db->carregar($sql,null,3200);
					
					if($dadosfinanceiros[0]) {
						foreach($dadosfinanceiros as $fin) {
							$_financeiro[$fin['rofano']] = array("rofdot_ini"=>$fin['rofdot_ini'],"empenhado"=>$fin['empenhado'],"rofpago"=>$fin['rofpago']);
						}
					}
                	
                	
                	?>
                	<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr>
		                			<td>&nbsp;</td>
		                			<td class="center" ><b>2011<b></td>
		                			<td class="center" ><b>2012<b></td>
		                		</tr>
		                		<tr>
		                			<td>Dotação Inicial</td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2011']['rofdot_ini'],2,",",".") ?></td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2012']['rofdot_ini'],2,",",".") ?></td>
		                		</tr>

		                		<tr>
		                			<td>Empenhado</td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2011']['empenhado'],2,",",".") ?></td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2012']['empenhado'],2,",",".") ?></td>
		                		</tr>
		                		<tr>
		                			<td>Pago</td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2011']['rofpago'],2,",",".") ?></td>
		                			<td onclick="abreRelatorio('painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&acacod[0]=12KV&alterar_ano=0')" class="numero" ><?=number_format($_financeiro['2012']['rofpago'],2,",",".") ?></td>
		                		</tr>
		                		<tr>
		                			<td class="bold" >Total</td>
		                			<td class="numero bold" ><?=number_format($_financeiro['2011']['rofpago']+$_financeiro['2011']['empenhado']+$_financeiro['2011']['rofdot_ini'],2,",",".") ?></td>
		                			<td class="numero bold" ><?=number_format($_financeiro['2012']['rofpago']+$_financeiro['2012']['empenhado']+$_financeiro['2012']['rofdot_ini'],2,",",".") ?></td>
		                		</tr>
		                	</table>
                </td>
                <!-- Tabela Financeiro -->
  </tr>
  <!-- 3 QUADROS -->
  <tr>
                <td class="fundo_td" colspan="2">
                	<?
                	$sql = "select sum(prevalorobra) from obras.preobra  pre
							inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid
							inner join par.termoobra tob on tob.preid = pre.preid
							inner join par.termocompromissopac t on t.terid = tob.terid and t.terstatus = 'A'
							where  pto.ptoclassificacaoobra IN('Q','C') and pre.prestatus = 'A'";
                	$valor_contratado = $db->pegaUm($sql);
                	
					$sql = "SELECT count(*) FROM (
							SELECT te.tobid
									FROM 
										par.processoobra p 
									LEFT JOIN par.resolucao r ON r.resid=p.resid 
									INNER JOIN territorios.municipio m ON m.muncod=p.muncod 
									INNER JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' 
									INNER JOIN par.termoobra te ON te.terid = ter.terid
									WHERE 1=1
									and p.prostatus = 'A' 
								AND p.estuf IS NULL AND protipo in('C','Q') AND ter.terassinado = TRUE UNION ALL (SELECT te.tobid
									FROM 
										par.processoobra p 
									LEFT JOIN par.resolucao r ON r.resid=p.resid
									INNER JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' 
									INNER JOIN par.termoobra te ON te.terid = ter.terid
									INNER JOIN territorios.estado e ON e.estuf=p.estuf  
									WHERE 1=1
									and p.prostatus = 'A' 
								AND p.estuf IS NOT NULL AND protipo in('C','Q') AND ter.terassinado = TRUE)
							) foo";
					
					$termosassinados = $db->pegaUm($sql, 0, 3200);
                	?>
                	<div>
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" onclick="acessarTermos();" class="titulo_box" ><?=number_format($termosassinados,0,"",".") ?><br/><span class="subtitulo_box" >Obras com Termos Assinados</span><br /><br />R$ <?=number_format($valor_contratado,2,"",".") ?><br/><span class="subtitulo_box" >Valor Contratado</span></div>
                	</div>
                </td>
                <td class="fundo_td">
                	<?
                	$sql = "select count(ligid) as recebida from callcenter.ligacao where temid = 1 and tlgid = 1 group by tlgid";
                	$contatosefetuados = $db->pegaUm($sql);
                	?>
                	<div>
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/call.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" onclick="acessarCallCenter();" ><?=number_format($contatosefetuados,0,"",".") ?><br/><span class="subtitulo_box" >Contatos<br/>Efetuados</span></div>
                	</div>
                </td>
                <td class="fundo_td" nowrap="nowrap">
                	<div onclick="abreUsuarios()">
                		<img style="float:left;width: 40px;height: 40px;" src="../imagens/icones/icons/chat.png" style="vertical-align:middle;"  />
                		<div id="usuOnline" style="float:left;" class="titulo_box" ><?=0 . ' <span class="subtitulo_box" >On-line<br/>'.date("d/m/Y").'<br>'.date("g:i:s").'</span>' ?></div>
                	</div>
                </td>
  </tr>
  <!-- Fim 3 QUADROS -->
  <tr>
                <td class="fundo_td_laranja" colspan="6">
                	<div style="text-align:center;"  >
                		<img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
                		<input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" 
                			onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" 
                			onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" 
                			id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
                		<img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
                	</div>
                </td>
  </tr>
</table>
</body>
</html>