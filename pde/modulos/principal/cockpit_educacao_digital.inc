<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if ($_REQUEST['filtro_mapa']) {

    switch($_REQUEST['filtro_mapa']){
        case ('mapa_laptop'):

            $sql = "select sum(dsh.dshqtde) as quantidade, count(distinct dshuf) estado, count(distinct dshcodmunicipio) as municipio
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where i.indid in (631)
                    and sh.sehstatus <> 'I'";

            break;
        case ('mapa_labs'):

            $sql = "select sum(dsh.dshqtde) as quantidade, count(distinct dshuf) estado, count(distinct dshcodmunicipio) as municipio
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where i.indid in (224)
                    and sh.sehstatus <> 'I'";

            break;
        case ('mapa_banda'):

            $sql = "select sum(dsh.dshqtde) as quantidade, count(distinct dshuf) estado, count(distinct dshcodmunicipio) as municipio
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where i.indid in (742)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    and sh.sehstatus <> 'I'";

            break;
        default: //mapa_tablets

            $sql = "select sum(dshqtde) as quantidade, count(distinct dshuf) estado, count(distinct dshcodmunicipio) as municipio
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where i.indid in (1691)
                    and sh.sehstatus <> 'I'";
    }

    $dados = $db->pegaLinha($sql,null,3200);
    echo '<span style="font-weight:bold;font-size:14px" >' . number_format($dados['quantidade'],0,",",".") . '</span><span class="subtitulo_box" > Unidades</span>
          <br /><span style="font-weight:bold;font-size:14px" >' . number_format($dados['municipio'],0,",",".") . '</span><span class="subtitulo_box" > municípios</span>';

    die;
}

?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=9" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
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

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <script>
        jQuery.noConflict();

        jQuery(function(){

            jQuery('.filtro_mapa').click(function(){
                switch(jQuery(this).attr('id')){
                    case ('mapa_tablets'): jQuery('#img_mapa').attr('onclick', 'abreIndicadorPopUp(1691);'); break;
                    case ('mapa_laptop'):  jQuery('#img_mapa').attr('onclick', 'abreIndicadorPopUp(631);');  break;
                    case ('mapa_labs'):    jQuery('#img_mapa').attr('onclick', 'abreIndicadorPopUp(224);');  break;
                    case ('mapa_banda'):   jQuery('#img_mapa').attr('onclick', 'abreIndicadorPopUp(742);');  break;
                }

                jQuery('#info_mapa').html('<img src="../imagens/carregando.gif" style="vertical-align:middle;" />');
                jQuery('#info_mapa').load('/pde/estrategico.php?modulo=principal/cockpit_educacao_digital&acao=A&filtro_mapa=' + jQuery(this).attr('id'));
            });

            jQuery('#mapa_tablets').click();
        });

        atualizaUsuario();

        function abrePopUpObrasSituacao(tipo, situacao){
            window.open('/pde/estrategico.php?modulo=principal/popuplistaobrassituacao&acao=A&tipo=' + tipo + '&situacao='+situacao);
        }
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')};
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pronatec&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
    </table>
    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <!-- Título-->
    <tr>
    <td class="fundo_titulo" style="text-align:center" colspan="3" ><br>Educação Digital<br><br></td>
    </tr>
    <!-- Fim Título-->
    <tr>
        <td class="fundo_padrao" width="33%" rowspan="1" >
            <?php exibirTitulo('indicador', 'Tablets', '<span class="link" onclick="abreIndicadorPopUp(1982);">Adquiridos</span>, <span class="link" onclick="abreIndicadorPopUp(1691);">Entregues</span>, <span class="link" onclick="abreIndicadorPopUp(1983);">Ativados</span>'); ?>

            <?php
            $sql = "select
						case i.indid
							when 1982 then 'Adquiridos'
							when 1691 then 'Entregues'
							when 1983 then 'Ativados'
						end AS descricao,
						case i.indid
							when 1982 then 1
							when 1691 then 2
							when 1983 then 3
						end AS ordem,
						sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					where sh.sehstatus <> 'I'
					and i.indid in (1982, 1691, 1983)
					group by descricao, ordem
					order by ordem";
            $dados = $db->carregar($sql,null,3200);

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaTablet', null, null, $formatoValores, null, "", 1000, 600, null, 300);

            ?>

            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" width="70%">Situação</th>
                    <th class="center bold" width="30%" >Quantidade</th>
                </tr>
                <?php
                $total = 0;
                foreach($dados as $count => $dado):
                    $total += $dado['valor'];
                    ?>
                    <tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td><?php echo $dado['descricao']; ?></td>
                        <td class="numero"><?php echo number_format($dado['valor'], 0, ',', '.'); ?></td>
                    </tr>
                <?php endforeach; ?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero bold" ><?php echo number_format($total,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <!-- RETIRADO A PEDIDO DA JAANA DIA 29/12/2014
        <td class="fundo_padrao" width="33%">
            <?php exibirTitulo('indicador', 'Formação'); ?>

            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Curso</th>
                    <th class="center bold" >Meta</th>
                    <th class="center bold" >Execução</th>
                </tr>
                <tr>
                    <td>Cursos Proinfo (professores)</td>
                    <td class="numero">450.000</td>
                    <td class="numero">0</td>
                </tr>
                <tr>
                    <td>Especialização em Educação digital (NTEs)</td>
                    <td class="numero">5.000</td>
                    <td class="numero">0</td>
                </tr>
                <tr>
                    <td>Aluno Integrado (EMI)</td>
                    <td class="numero">8.000</td>
                    <td class="numero">0</td>
                </tr>
                <tr>
                    <td>Aluno Integrado (regular)</td>
                    <td class="numero">100.000</td>
                    <td class="numero">0</td>
                </tr>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero bold" >563.000</th>
                    <th class="numero bold" >0</th>
                </tr>
            </table>
        </td>
        -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae', '1', '20', '', '');" align="center">
            <?php exibirTitulo('indicador', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <td class="fundo_padrao" width="33%" rowspan="1" >
            <?php exibirTitulo('indicador', 'Mapa', 'Clique no mapa para visualizar os detalhes.'); ?>

            <div style="clear:both;width:100%; margin:20px 0;">
                <input type="radio" name="mapa" id="mapa_tablets" class="filtro_mapa" <?php echo $_REQUEST['filtro_mapa'] == 'mapa_tablets' ? 'checked="checked"': ''; ?> /><label for="mapa_tablets">Tablets</label><br />
                <input type="radio" name="mapa" id="mapa_laptop" class="filtro_mapa" <?php echo $_REQUEST['filtro_mapa'] == 'mapa_laptop' ? 'checked="checked"': ''; ?> /><label for="mapa_laptop">Laptops adquiridos PROUCA</label><br />
                <input type="radio" name="mapa" id="mapa_labs" class="filtro_mapa" <?php echo $_REQUEST['filtro_mapa'] == 'mapa_labs' ? 'checked="checked"': ''; ?> /><label for="mapa_labs">Laboratórios PROINFO entregues</label><br />
                <input type="radio" name="mapa" id="mapa_banda" class="filtro_mapa" <?php echo $_REQUEST['filtro_mapa'] == 'mapa_banda' ? 'checked="checked"': ''; ?> /><label for="mapa_banda">Banda Larga nas Escolas</label><br />
            </div>



            <div class="left">
                <img id="img_mapa" class="link" onclick="abreIndicadorPopUp(1418);" src="../imagens/cockpit/mapa_cockpit.png"  />
            </div>

            <div class="left" style="margin-left: 50px; padding-top: 50px" id="info_mapa">
                <span style="font-weight:bold;font-size:14px" ><?php echo number_format($tabletAdquirido,0,",",".") ?> </span><span class="subtitulo_box" >Unidades</span>
                <br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalUFTablet,0,",",".") ?> </span><span class="subtitulo_box" >UFs</span>
            </div>

        </td>
    </tr>


<!-- INÍCIO DO FINANCEIRO -->
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
?>
    <tr>
        <td class="fundo_padrao" colspan="3">
            <?php
			$sql = "SELECT
						ano,
						sum(valor1) AS empenhado,
						sum(valor2) AS dotacao,
						sum(valor3) AS pago
					FROM (
						SELECT
							2011 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2011 sld
						WHERE substr(sld.plicod, 2, 4) in ('FD12', 'FF61')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2012 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2012 sld
						WHERE substr(sld.plicod, 2, 4) in ('FD12', 'FF61')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2013 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2013 sld
						WHERE substr(sld.plicod, 2, 4) in ('FD12', 'FF61')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					) as foo
					GROUP BY ano
					ORDER BY ano";
			$arrDados = $db2->carregar($sql,null,3200);
			$total_dotacao = 0;
			$total_empenhado = 0;
			$total_pago = 0;
			?>
			<div>
				<?php exibirTitulo('financeiro', 'Orçamentário/Financeiro'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Ano</th>
					<th class="center bold" >Empenhado</th>
					<!--<th class="center bold" >Pago</th>-->
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $chave => $dado):
				    $count++;?>
					<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=FD12&subacao[1]=FF61&alterar_ano=0')" >
						<td><?php echo $dado['ano'] ?></td>
						<td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
						<!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
					</tr>
				<?php endforeach; ?>
				<tr>
					<th class="bold" >Total</th>
					<th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
					<!--<th class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></th>-->
				</tr>
			</table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2" >
            <div>
                <?php exibirTitulo('executive', 'Execução'); ?>
            </div>
			<?php
			$sql = "select
						case
							when i.indid IN (224, 227) then 'Laboratórios (Pronacampo)'
							when i.indid = 1748 then 'Laptops (Pronacampo)'
						end descricao,
						dpe.dpeanoref,
						sum(dsh.dshqtde) as quantidade
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where i.indid in (224, 227, 1748)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref BETWEEN '2007' AND '2013'
					group by descricao, dpe.dpeanoref
					order by dpe.dpeanoref";
			$arrDados = $db->carregar($sql,null,3200);
			?>
        </td>
        <td class="fundo_padrao" >
            <div>
                <?php exibirTitulo('configs', '<a target="_blank" href="http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos">Mapa do Processo</a>'); ?>
            </div>
        </td>
    </tr>
    <!-- Rodapé -->
    <tr>
        <td class="fundo_td_laranja" colspan="3">
            <div style="text-align:center;"  >
            <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
            <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
            <img src="../imagens/icones/icons/Find.png" style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
            </div>
        </td>
    </tr>
</table>
</body>
</html>