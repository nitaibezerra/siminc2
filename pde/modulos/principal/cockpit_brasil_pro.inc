<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

// Educação Básica Obras 2
$_SESSION['obras2']['orgid'] = 3;

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

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
		jQuery.noConflict();

		jQuery(function(){
			jQuery('.div_fotos_interno').cycle({
				fx: 'scrollDown'
			});

			jQuery('.scrollbar').perfectScrollbar({wheelPropagation:true});

			jQuery('.lista-noticias').totemticker({
				row_height  :   '100px',
				next        :   '#ticker-next',
				previous    :   '#ticker-previous',
				stop        :   '#stop',
				start       :   '#start',
				mousestop   :   true,
			});
		});
		
		atualizaUsuario();
	
		function abreObras(obrid){
    		window.open('/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid);
    	}
		
		function abrePopUpObrasSituacao(tipo, situacao, estuf){
	        window.open('/pde/estrategico.php?modulo=principal/popuplistaobrassituacao&acao=A&tipo=' + tipo + '&situacao='+situacao+'&filtro_estuf='+estuf+'&filtro_estuf_construcao='+estuf);
	    }

        function listaEstados(estuf){
            window.location.href = 'estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A&estuf=' + estuf;
        }
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')}
        .div_fotos_padrao2{background-color:#152D56;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:400px;margin-bottom:2px}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<?php
$estuf = $_REQUEST['estuf'];
if($estuf == 'Brasil') $estuf="";
if($estuf){
    $sql = "SELECT estdescricao || ' ('||estuf||')' FROM territorios.estado WHERE estuf = '{$estuf}' ";
    $titulo = $db->pegaUm($sql);
}else{
    $titulo = "Brasil";
}
?>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Brasil Profissionalizado<br><?=$titulo?></div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" rowspan="2" width="33%">
			<div>
				<?php exibirTitulo('busca', 'Clique na bandeira para filtrar o estado', ($titulo=='Nacional' ? '' : 'Selecionado: ' .$titulo)); ?>
			</div>
            <div  align=center style="width: 100%;" id="containerMapa" >
                <img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
                <map name="mapaBrasil" id="mapaBrasil">
                    <area shape="rect" coords="420,20,390,56"  href="javascript:void(0);" onclick="listaEstados('Brasil');" title="Brasil"/ >
                    <area shape="rect" coords="48,124,74,151"  href="javascript:void(0);" onclick="listaEstados('AC');" title="Acre"/>
                    <area shape="rect" coords="364,147,432,161" href="javascript:void(0);" onclick="listaEstados('AL');" title="Alagoas"/>
                    <area shape="rect" coords="202,27,233,56"   href="javascript:void(0);" onclick="listaEstados('AP');" title="Amapá"/>
                    <area shape="rect" coords="89,76,133,107"   href="javascript:void(0);" onclick="listaEstados('AM');" title="Amazonas"/>
                    <area shape="rect" coords="294,155,320,183" href="javascript:void(0);" onclick="listaEstados('BA');" title="Bahia"/>
                    <area shape="rect" coords="311,86,341,114"  href="javascript:void(0);" onclick="listaEstados('CE');" title="Ceará"/>
                    <area shape="rect" coords="244,171,281,197" href="javascript:void(0);" onclick="listaEstados('DF');" title="Distrito Federal"/>
                    <area shape="rect" coords="331,215,369,242" href="javascript:void(0);" onclick="listaEstados('ES');" title="Espírito Santo"/>
                    <area shape="rect" coords="217,187,243,218" href="javascript:void(0);" onclick="listaEstados('GO');" title="Goiás"/>
                    <area shape="rect" coords="154,155,210,186" href="javascript:void(0);" onclick="listaEstados('MT');" title="Mato Grosso"/>
                    <area shape="rect" coords="156,219,202,246" href="javascript:void(0);" onclick="listaEstados('MS');" title="Mato Grosso do Sul"/>
                    <area shape="rect" coords="248,80,301,111"  href="javascript:void(0);" onclick="listaEstados('MA');" title="Maranhão"/>
                    <area shape="rect" coords="264,206,295,235" href="javascript:void(0);" onclick="listaEstados('MG');" title="Minas Gerais"/>
                    <area shape="rect" coords="188,84,217,112"  href="javascript:void(0);" onclick="listaEstados('PA');" title="Pará"/>
                    <area shape="rect" coords="368,112,433,130" href="javascript:void(0);" onclick="listaEstados('PB');" title="Paraíba"/>
                    <area shape="rect" coords="201,262,231,289" href="javascript:void(0);" onclick="listaEstados('PR');" title="Paraná"/>
                    <area shape="rect" coords="369,131,454,147" href="javascript:void(0);" onclick="listaEstados('PE');" title="Pernambuco"/>
                    <area shape="rect" coords="285,116,313,146" href="javascript:void(0);" onclick="listaEstados('PI');" title="Piauí"/>
                    <area shape="rect" coords="349,83,383,108"  href="javascript:void(0);" onclick="listaEstados('RN');" title="Rio Grande do Norte"/>
                    <area shape="rect" coords="189,310,224,337" href="javascript:void(0);" onclick="listaEstados('RS');" title="Rio Grande do Sul"/>
                    <area shape="rect" coords="302,250,334,281" href="javascript:void(0);" onclick="listaEstados('RJ');" title="Rio de Janeiro"/>
                    <area shape="rect" coords="98,139,141,169"  href="javascript:void(0);" onclick="listaEstados('RO');" title="Rondônia"/>
                    <area shape="rect" coords="112,24,147,56"   href="javascript:void(0);" onclick="listaEstados('RR');" title="Roraima"/>
                    <area shape="rect" coords="228,293,272,313" href="javascript:void(0);" onclick="listaEstados('SC');" title="Santa Catarina"/>
                    <area shape="rect" coords="233,243,268,270" href="javascript:void(0);" onclick="listaEstados('SP');" title="São Paulo"/>
                    <area shape="rect" coords="337,161,401,178" href="javascript:void(0);" onclick="listaEstados('SE');" title="Sergipe"/>
                    <area shape="rect" coords="227,130,270,163" href="javascript:void(0);" onclick="listaEstados('TO');" title="Tocantins"/>
                </map>
            </div>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2539);" width="34%">
			<?
			$sql = "select sum(dsh.dshqtde) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (2539)
                    and sh.sehstatus <> 'I'
                    ".($estuf ? "AND dsh.dshuf = '". $estuf."'" : '')."
                    and dpe.dpeanoref between '2011' and '2014'";
            $totalMatriculas = $db->pegaUm( $sql, null, 3200 );
			?>
            <div>
                <?php exibirTitulo('doc', 'Matrículas Brasil Profissionalizado<br>no PRONATEC (2011-2014)<br><br><center>'.number_format($totalMatriculas,0,",",".")); ?>
            </div>
		</td>
		<!-- Tabela Lateral-->
		<td class="fundo_azul_padrao" rowspan="2" width="33%">
			<div>
				<?php exibirTitulo('configs', 'Obras'); ?>
			</div>
			<?php
			$sql = "SELECT o.obrnome as obrdesc,
						CASE e.empesfera
							WHEN 'E' THEN 'Estadual'
							WHEN 'M' THEN 'Municipal'
							ELSE 'Estadual'
						END AS dependenciaadministrativa,
						mun.mundescricao AS municipio,
						mun.estuf AS uf,
						esd.esddsc AS situacao,
						fot.arqid,
						o.obrid
					from obras2.obras o
					inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
					inner join workflow.documento d ON d.docid = o.docid
					inner join workflow.estadodocumento esd ON esd.esdid = d.esdid
					INNER JOIN
						(select MAX(arqid) AS arqid, obrid FROM obras2.fotos GROUP BY obrid) AS fot ON fot.obrid = o.obrid
					INNER JOIN entidade.endereco edc ON edc.endid = o.endid
					INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
					where o.obrstatus = 'A'
					and e.orgid=3
					and d.esdid not in (766,767,769,770) --Cancelamentos
					and o.obridpai is null
					and e.prfid = 40
					".($estuf ? "AND mun.estuf = '". $estuf."'" : '')."
					ORDER BY random()
					LIMIT 50";
			$arrFotos = $db->carregar($sql);
			?>
			<div style="clear:both" id="div_fotos_padrao2"  >
					<?php for($x=5;$x>=1;$x--): ?>
						<div style="clear:both" class="div_fotos_interno"  >
						<?php for($i=$x;$i<=(5+$x);$i++): ?>
							<div class="div_fotos_padrao2" >
								<table border="0" >
									<tr class="link" onclick="abreObras('<?=$arrFotos[$i]['obrid'] ?>');">
										<td>
											<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
										</td>
										<td style="color:#FFFFFF">
											<?php echo substr($arrFotos[$i]['obrdesc'],0,60) ?>...<br/>
											Dependência Adm.: <?php echo $arrFotos[$i]['dependenciaadministrativa'] ?><br/>
											Município: <?=$arrFotos[$i]['municipio'] ?> / <?=$arrFotos[$i]['uf'] ?><br/>
											Situação Convênio: <?php echo $arrFotos[$i]['situacao'] ?>
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
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('obras', 'Obras'); ?>
			</div>
			<?php
			$sql = "select obras.*, coalesce(dias.dias_sem_atualizacao, 0) as dias_sem_atualizacao from
                    (
                        SELECT
                            esd.esdid, esd.esddsc as situacao,
                            CASE
                                WHEN o.tobid IN (3,4,5) THEN 'Ampliação / Reforma'
                            ELSE 'Construção'
                            END AS tipo,
                       CASE
							WHEN esd.esddsc = 'Concluída'  THEN 1
							WHEN esd.esddsc = 'Execução'  THEN 2
							WHEN esd.esddsc = 'Contratação'  THEN 3
                            WHEN esd.esddsc = 'Licitação'  THEN 4
                            WHEN esd.esddsc = 'Aguardando 1º repasse'  THEN 5
                            WHEN esd.esddsc = 'Celebração de aditivo'  THEN 6
							WHEN esd.esddsc = 'Planejamento pelo proponente'  THEN 7
                            WHEN esd.esddsc = 'Paralisada'  THEN 8
                            WHEN esd.esddsc = 'Obra Cancelada'  THEN 9
                            ELSE 99999
                        END AS ordem,
                            count(0) as total,
                            mun.estuf
                        FROM obras2.obras o
                            INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                            INNER JOIN workflow.documento d ON d.docid = o.docid
                            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
                            INNER JOIN entidade.endereco edc ON edc.endid = o.endid
                            INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
                        WHERE o.obrstatus = 'A'
                        AND e.orgid=3
                        AND e.prfid=40 --Brasil Profissionalizado
                        AND o.obridpai is null
                        GROUP BY esd.esdid, situacao, tipo, ordem, mun.estuf
                    ) as obras
                        left join (
                            select max(DATE_PART('days', NOW() - obrdtultvistoria)) as dias_sem_atualizacao,
                                esd.esdid,
                                CASE
                                    WHEN o.tobid IN (3,4,5) THEN 'Ampliação / Reforma'
                                ELSE 'Construção'
                                END AS tipo
                            FROM obras2.obras o
                            INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                            INNER JOIN workflow.documento d ON d.docid = o.docid
                            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
                            INNER JOIN entidade.endereco edc ON edc.endid = o.endid
                            INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
                            WHERE o.obrstatus = 'A'
                            AND e.orgid=3
                            AND e.prfid=40 --Brasil Profissionalizado
                            AND o.obridpai is null
                            and obrpercentultvistoria < 100.00
                            group by esd.esdid, tipo
                        ) as dias on dias.esdid = obras.esdid and dias.tipo = obras.tipo
                        ".($estuf ? "WHERE estuf = '". $estuf."'" : '')."
                        order by ordem, situacao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			$somaApliacao = 0;
			$somaConstrucao = 0;
			$diasApliacao = 0;
			$diasConstrucao = 0;
			$somaTotal = 0;
			$totalApliacao = 0;
			$totalConstrucao = 0;
			$totalGeral = 0;
			$arrSituacao = array();
			if($arrDados){
				foreach($arrDados as $dado){
					$arrSituacao[$dado['situacao']][$dado['tipo']]['total'][] = $dado['total'];
					$arrSituacao[$dado['situacao']][$dado['tipo']]['dias'][] = $dado['dias_sem_atualizacao'];
					$arrSituacao[$dado['situacao']]['esdid'] = $dado['esdid'];
				}
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th class="center bold" >Situação</th>
                        <th class="center bold" >Reforma / Ampliações</th>
                        <th class="center bold" >Construções</th>
                        <th class="center bold" >Total</th>
                    </tr>
                    <?php
                    $count = -1;
                    foreach($arrSituacao as $chave => $sit):


                        $somaApliacao = (is_array($sit['Ampliação / Reforma']['total'])?array_sum($sit['Ampliação / Reforma']['total']):0);
                        $diasApliacao = (is_array($sit['Ampliação / Reforma']['total'])?array_sum($sit['Ampliação / Reforma']['dias']):0);

                        $somaConstrucao = (is_array($sit['Construção']['total'])?array_sum($sit['Construção']['total']):0);
                        $diasConstrucao = (is_array($sit['Construção']['total'])?array_sum($sit['Construção']['dias']):0);

                        $somaTotal = $somaApliacao + $somaConstrucao;
                        $count++;

                        if($sit['esdid']==693){
                            $concluida=true;
                        }else{
                            $concluida=false;
                        }

                        ?>
                        <tr class="<?php echo ($count%2) ? '"link zebrado"' : '"link"'; ?>" onclick="abrePopUpObrasSituacao(1, '<?php echo $sit['esdid']; ?>', '<?php echo $estuf; ?>');">
                            <td class="" ><?=$chave ?></td>
                            <td class="numero" ><?= geraLinha($somaApliacao, $diasApliacao, $concluida); ?></td>
                            <td class="numero" ><?= geraLinha($somaConstrucao, $diasConstrucao, $concluida); ?></td>
                            <td class="numero" ><?=number_format($somaTotal,0,",",".")?></td>
                        </tr>
                        <?php
                        $totalApliacao += $somaApliacao;
                        $totalConstrucao += $somaConstrucao;
                        $totalGeral = $totalApliacao + $totalConstrucao;
                    endforeach; ?>
                    <tr>
                        <th>Total</th>
                        <th><?=number_format($totalApliacao,0,",",".")?></th>
                        <th><?=number_format($totalConstrucao,0,",",".")?></th>
                        <th><?=number_format($totalGeral,0,",",".")?></th>
                    </tr>
                </table>
            <?php
            }
            ?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2539);">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas'); ?>
			</div>
            <?php
                $sql = "SELECT
                            dshuf AS uf,
                            dpeanoref AS ano,
                            SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (2539)
                        AND sh.sehstatus <> 'I'
                        ".($estuf ? "AND dsh.dshuf = '". $estuf."'" : '')."
                        GROUP BY uf, ano
                        ORDER BY uf, ano";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'uf', 'ano', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculas', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(771);" colspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Linha do Tempo'); ?>
			</div>
            <?
            $arrSituacao = array();
            $sql = "select dpe.dpedsc AS periodo, dpe.dpeanoref, dpe.dpeordem, sum(dsh.dshqtde) as total, dsh.tidid2 as situacao
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (771)
					and sh.sehstatus <> 'I'
					and dpe.dpedatainicio >= '2014/08/01'
					".($estuf ? "AND dsh.dshuf = '". $estuf."'" : '')."
					group by dpe.dpedsc, dpe.dpeanoref, dpe.dpeordem, situacao
					order by dpe.dpeanoref, dpe.dpeordem, situacao";
            $arrDados = $db->carregar($sql,null,3200);
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['situacao']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrC = array();
                $arrD = array();
                $arrE = array();
                $arrF = array();
                $arrG = array();
                $arrH = array();
                $arrI = array();
                $arrJ = array();
                $arrK = array();
                $arrL = array();
                $arrM = array();

                foreach($arrSituacao as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $arrA[] = Round((is_array($sit['2965']['total'])?array_sum($sit['2965']['total']):0));
                    $arrB[] = Round((is_array($sit['2966']['total'])?array_sum($sit['2966']['total']):0));
                    $arrC[] = Round((is_array($sit['2967']['total'])?array_sum($sit['2967']['total']):0));
                    $arrD[] = Round((is_array($sit['2968']['total'])?array_sum($sit['2968']['total']):0));
                    $arrE[] = Round((is_array($sit['2969']['total'])?array_sum($sit['2969']['total']):0));
                    $arrF[] = Round((is_array($sit['2970']['total'])?array_sum($sit['2970']['total']):0));
                    $arrG[] = Round((is_array($sit['2971']['total'])?array_sum($sit['2971']['total']):0));
                    $arrH[] = Round((is_array($sit['2972']['total'])?array_sum($sit['2972']['total']):0));
                    $arrI[] = Round((is_array($sit['4779']['total'])?array_sum($sit['4779']['total']):0));
                    $arrJ[] = Round((is_array($sit['4780']['total'])?array_sum($sit['4780']['total']):0));
                    $arrK[] = Round((is_array($sit['4781']['total'])?array_sum($sit['4781']['total']):0));
                    $arrL[] = Round((is_array($sit['4782']['total'])?array_sum($sit['4782']['total']):0));
                    $arrM[] = Round((is_array($sit['4783']['total'])?array_sum($sit['4783']['total']):0));
                }

                $aDados = array(
                    array('name' => 'Concluida', 'data'=>$arrA),
                    //array('name' => 'Contrato Cancelado', 'data'=>$arrB),
                    //array('name' => 'Convenio Cancelado', 'data'=>$arrC),
                    array('name' => 'Em Elaboracao de Projetos', 'data'=>$arrD),
                    array('name' => 'Em Execucao', 'data'=>$arrE),
                    array('name' => 'Em Licitacao', 'data'=>$arrF),
                    array('name' => 'Em Planejamento pelo Proponente', 'data'=>$arrG),
                    array('name' => 'Paralisada', 'data'=>$arrH),
                    array('name' => 'Em Reformulacao', 'data'=>$arrI),
                    array('name' => 'Contratacao', 'data'=>$arrJ),
                    array('name' => 'Obra Cancelada', 'data'=>$arrK),
                    //array('name' => 'Celebracao de aditivo', 'data'=>$arrL),
                    //array('name' => 'Aguardando 1o repasse', 'data'=>$arrM),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempo", "Obras",300, $formatoValores);
            }
            ?>
		</td>
    </tr>
    <tr>
		<td class="fundo_padrao" colspan="2" rowspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Repasse de recursos - 2014'); ?>
            </div>
			<?php
			$sql = "SELECT descricao, SUM(conveniado) AS conveniado, SUM(empenhado) AS empenhado, SUM(pago) AS pago
                    FROM (
                        SELECT
                            tid1.tiddsc AS descricao,
                            CASE WHEN sh.indid IN (1524) THEN dsh.dshqtde END AS conveniado,
                            CASE WHEN sh.indid IN (3158) THEN dsh.dshqtde END AS empenhado,
                            CASE WHEN sh.indid IN (3159) THEN dsh.dshqtde END AS pago
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (1524, 3158, 3159)
                        AND sh.sehstatus <> 'I'
                        ".($estuf ? "AND dsh.dshuf = '". $estuf."'" : '')."
                        AND dpe.dpeanoref = '2014'
                    ) AS FOO
                    GROUP BY descricao
                    ORDER BY descricao";
			$arrDados = $db->carregar($sql,null,3200);
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th>Destinação</th>
					<th>Conveniado</th>
					<th>Empenhado</th>
					<th>Pago</th>
				</tr>
				<?php
                if($arrDados){
                    $count = -1;
                    $total_conveniado=0;
                    $total_empenhado=0;
                    $total_pago=0;
                    foreach($arrDados as $dado):
                        $count++;?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['descricao'] ?></td>
                            <td class="numero link" onclick="abreIndicadorPopUp(1524);" ><?php echo number_format($dado['conveniado'],2,",",".") ?></td>
                            <td class="numero link" onclick="abreIndicadorPopUp(3158);" ><?php echo number_format($dado['empenhado'],2,",",".") ?></td>
                            <td class="numero link" onclick="abreIndicadorPopUp(3159);" ><?php echo number_format($dado['pago'],2,",",".") ?></td>
                        </tr>
				        <?php
                        $total_conveniado+=$dado['conveniado'];
                        $total_empenhado+=$dado['empenhado'];
                        $total_pago+=$dado['pago'];
                    endforeach;
                }
                ?>
				<tr>
					<th class="bold" >Total</th>
					<th class="numero bold link" onclick="abreIndicadorPopUp(1524);"><?php echo number_format($total_conveniado,2,",",".") ?></th>
					<th class="numero bold link" onclick="abreIndicadorPopUp(3158);"><?php echo number_format($total_empenhado,2,",",".") ?></th>
					<th class="numero bold link" onclick="abreIndicadorPopUp(3159);"><?php echo number_format($total_pago,2,",",".") ?></th>
				</tr>
			</table>
		</td>
        <!-- Tabela Processos -->
        <td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/')";>
            <div>
                <?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
            </div>
        </td>
        <!-- Tabela Processos -->
	</tr>
	<tr>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',2,16,'','');" align="center"  colspan="2">
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
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
function geraLinha($qtd, $dias, $concluida=false){

	$font = '#fff';
	if ($qtd > 60) {
		$cor = '#f00';
	} elseif ($qtd > 45) {
		$cor = '#FFD700';
		$font = '#000';
	} else {
		$cor = 'green';
	}
	
	if($concluida){
		$cor = 'green';
	}

	?>
	<center>
		<span style='padding: 0 5px; border-width:1px; border-style:solid; border-color:rgb(0, 0, 0); background-color:<?=$cor?>; text-align:center; color:<?php echo $font; ?>; height:15px;'>
			<?php echo number_format($qtd,0,",","."); ?>
		</span>
	</center>
	<?
}
?>