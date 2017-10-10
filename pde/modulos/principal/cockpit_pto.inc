<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_metas'])){
    gerarGraficoMetas($_REQUEST['detalhe_metas']);
    die;
}

if(!empty($_REQUEST['atualizarEtapas'])){
    carregarDetalhesEtapas(array('solid'=>$_REQUEST['solid']));
    die;
}

if ($_REQUEST['request'] == 'download') {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ($_REQUEST['arqid']){
        $file = new FilesSimec("", "", "pto");
        $file->getDownloadArquivo($_REQUEST['arqid']);
    }
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
            jQuery('.filtrar_metas').click(function(){
                jQuery('#div_metas').load('estrategico.php?modulo=principal/cockpit_pto&acao=A&detalhe_metas='+jQuery(this).val());
            });
        });

        function detalharEtapasSolucao(solid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 4;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+solid);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_pto&acao=A&solid='+solid+'&atualizarEtapas=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }

        function abrePortalPNE(){
            window.open('http://pne.mec.gov.br');
        }

        function abreDetalheAtividades(){
            window.open('/pde/estrategico.php?modulo=principal/popupDetalhesAtividades&acao=A','winDetalheAtividade');
        }
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
        th.esquerda
        {
            text-align: left;
            background-color: #cccccc;
            font: 8pt Arial,verdana;
            vertical-align: top;
        }
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pto&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >PNE</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao" width="40%" height="500" colspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Execução'); ?>
            </div>
            <?php
            $sql = "SELECT
                        substr(solucao,1,1000) AS descricao,
                        Round(((COALESCE(SUM(finalizado::numeric),0)*100) / COUNT(0)::numeric),0) AS valor
                    FROM (
                        SELECT
                            sol.solapelido AS solucao,
                            CASE WHEN doc.esdid IN (1301) THEN 1 END AS execucao,
                            CASE WHEN doc.esdid IN (1300) THEN 1 END AS finalizado
                        FROM pto.etapa etp
                        INNER JOIN pto.atividade atv ON atv.etpid = etp.etpid AND atv.atvstatus = 'A'
                        INNER JOIN workflow.documento doc ON doc.docid = atv.docid
                        INNER JOIN pto.solucao sol ON sol.solid = etp.solid AND sol.solstatus = 'A'
                        WHERE etp.etpstatus = 'A'
                    ) AS FOO
                    GROUP BY descricao
                    ORDER BY Round(((COALESCE(SUM(finalizado::numeric),0)*100) / COUNT(0)::numeric),0) DESC, descricao";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                geraGraficoBarra($arrDados, 'graficoBarraExecucao', null, null, null, null, "", 500, 300);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="30%">
            <div style="float: left; width: 50%;"> <?php exibirTitulo('indicador', 'Atividades Críticas'); ?></div>
            <div style="float: left; width: 50%;"> <?php exibirTitulo('doc', '<span class="link" onclick="abreDetalheAtividades();">Relatório</span>'); ?></div>
            <div class="clear"></div>
            <?php
            $sql = "SELECT
                        CASE WHEN sol.solprazo IS NOT NULL THEN TO_CHAR(sol.solprazo,'YYYY')
                        ELSE 'Não Informado'
                        END AS descricao,
                        COUNT(0) AS valor
                    FROM pto.solucao sol
                    INNER JOIN pto.etapa etp ON etp.solid = sol.solid AND etp.etpstatus = 'A'
                    INNER JOIN pto.atividade atv ON atv.etpid = etp.etpid AND atv.atvstatus = 'A'
                    INNER JOIN workflow.documento doc ON doc.docid = atv.docid
                    WHERE sol.solstatus = 'A'
                    AND doc.esdid = 1301
                    AND atv.atvcritico IS TRUE
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColuna($arrDados, 'graficoColunaAtividadesCriticas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            }
            ?>
        </td>
        <td class="fundo_padrao" width="30%" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Prazos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        CASE WHEN sol.solprazo IS NOT NULL THEN TO_CHAR(sol.solprazo,'YYYY')
                        ELSE 'Não Informado'
                        END AS descricao,
                        COUNT(0) AS valor
                    FROM pto.solucao sol
                    WHERE sol.solstatus = 'A'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaPrazos", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", true);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" rowspan="2">
            <div style="float: left; width: 50%;"> <?php exibirTitulo('executiverel', 'Corpo da Lei'); ?></div>
            <div style="float: left; width: 50%;"> <?php exibirTitulo('recycle', '<span class="link" onclick="abrePortalPNE();">Portal do PNE</span>'); ?></div>
            <?php
            $sql = "SELECT
                        sol.solid
                    FROM pto.solucao sol
                    WHERE sol.solstatus = 'A'
                    AND sol.solcorpolei IS TRUE
                    ORDER BY sol.solordem";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                foreach($arrDados as $dado){
                    echo montaQuadroPto($dado['solid']);
                    echo "<br>";
                    echo "<br>";
                }
            }
            ?>
        </td>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Comandos por área'); ?>
            </div>
            <?php
            $sql = "SELECT sec.secdsc AS descricao, COUNT(DISTINCT sol.solid) AS valor
                    FROM pto.solucao sol
                    INNER JOIN pto.secretariasolucao ses ON ses.solid = sol.solid
                    INNER JOIN painel.secretaria sec ON sec.secid = ses.secid
                    WHERE sol.solstatus = 'A'
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql );
            if($dados){
                $config = array('rotation'=>'-45', 'dataLabels'=>'true');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColuna($dados, 'graficoColunaArea', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Comandos por temática'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tem.temdsc AS descricao,
                        COUNT(DISTINCT sol.solid) AS valor
                    FROM pto.solucao sol
                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                    INNER JOIN pto.tema tem ON tem.temid = tes.temid
                    WHERE sol.solstatus = 'A'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                geraGraficoBarra($arrDados, 'graficoBarraTematica', null, null, null, null, "", 500, 300);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="3">
            <div>
                <?php exibirTitulo('alvo', 'Metas PNE'); ?>
            </div>
            <?php
            $sql = "SELECT mpneid, Replace(SUBSTR(mpnenome,1,7),':','') AS meta FROM pde.ae_metapne ORDER BY mpneid";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                echo "<div style='width:100%;text-align:center'>";
                echo "<input type='radio' class='filtrar_metas' name='metas' value='0' checked='checked' onclick='' />Todas";
                $contaLinhas = 1;
                foreach($arrDados as $dado){
                    echo "<input type='radio' class='filtrar_metas' name='metas' value='".$dado['mpneid']."' onclick='' />".$dado['meta']."";
                    if($contaLinhas == 10) {
                        echo "<br>";
                        $contaLinhas = 0;
                    }
                    $contaLinhas++;
                }
               echo "</div>";
            }
            ?>
            <br>
            <div id="div_metas">
                <?php gerarGraficoMetas(0); ?>
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
<?php
function montaQuadroPto($solid){

    global $db;

    $sql = "SELECT sec.secdsc
            FROM painel.secretaria sec
            INNER JOIN pto.secretariasolucao ses ON ses.secid = sec.secid
            WHERE sec.secstatus = 'A'
            AND ses.solid = {$solid}
            ORDER BY sec.secdsc";
    $arrSecretarias = $db->carregar( $sql );
    if($arrSecretarias){
        $count=0;
        $secretarias="";
        foreach($arrSecretarias as $sec){
            if($count==0){
                $secretarias = $sec['secdsc'];
            }else{
                $secretarias = $secretarias . ', ' . $sec['secdsc'];
            }
            $count++;
        }
    }

    $sql = "SELECT sol.solid, sol.solapelido, sol.soldsc, sol.solobs, to_char(sol.solprazo,'DD/MM/YYYY') AS prazo
            FROM pto.solucao sol
            WHERE sol.solstatus = 'A'
            AND sol.solid = {$solid}
            ORDER BY sol.solordem";
    $arrSolucao = $db->pegaLinha( $sql );

    $sql = "SELECT arqid FROM pto.anexosolucao WHERE solid = {$solid} AND anxstatus = 'A' ORDER BY anxid DESC LIMIT 1";
    $arqid = $db->pegaUm( $sql );

    $sql = "SELECT COUNT(0) AS total, COALESCE(SUM(finalizado),0) AS totalfinalizado, COALESCE(SUM(critico),0) AS totalcritico
            FROM (
                SELECT
                    CASE WHEN doc.esdid IN (1301) THEN 1 END AS execucao,
                    CASE WHEN doc.esdid IN (1300) THEN 1 END AS finalizado,
                    --CASE WHEN atvprazo < current_date AND atvcritico IS TRUE THEN 1 END AS critico
                    CASE WHEN atvcritico IS TRUE AND doc.esdid NOT IN (1300) THEN 1 END AS critico
                FROM pto.etapa etp
                INNER JOIN pto.atividade atv ON atv.etpid = etp.etpid AND atv.atvstatus = 'A'
                INNER JOIN workflow.documento doc ON doc.docid = atv.docid
                WHERE etp.etpstatus = 'A'
                AND etp.solid = {$solid}
            ) AS FOO";
    $arrPercentual = $db->pegaLinha( $sql );
    if($arrPercentual['total'] > 0){
        $porcentagem = number_format(($arrPercentual['totalfinalizado']*100)/$arrPercentual['total'],0,",",".");
    }else{
        $porcentagem = 0;
    }
    ?>
    <table id="solucao_<?=$solid?>" class="tabela_box" cellpadding="2" cellspacing="1" width="90%" >
        <tr>
            <th class="esquerda" title="<?=$arrSolucao['soldsc']?>" width="40%">
                <b><?=$arrSolucao['solapelido']?></b>
                <?php if($arrPercentual['totalcritico'] > 0){ ?>
                    &nbsp;<img src="../imagens/obras/atencao.png" height="20px" width="20px" title="Atividade crítica" align="right">
                <?php }?>
            </th>
            <th class="esquerda" width="20%"><b>Responsáveis:</b><br><?=$secretarias?></th>
            <th class="esquerda" width="20%"><b>Prazo:</b><br><?=$arrSolucao['prazo']?></th>
            <th class="esquerda" width="20%">
                <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid=<?=$solid?>" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="40px" width="40px" title="Detalhar Projeto"></a>&nbsp;
                <?php if($arqid){ ?>
                    <a href="estrategico.php?modulo=principal/cockpit_pto&acao=A&request=download&arqid=<?=$arqid?>"><img src="../imagens/icones/icons/attach.png" height="35px" width="35px" title="Boletim Estratégico"></a>
                <?php }else{ ?>
                    <img src="../imagens/icones/icons/attach.png" height="35px" width="35px" title="Boletim Estratégico">
                <?php } ?>
            </th>
        </tr>
        <tr>
            <th class="esquerda"><img src="../imagens/mais.gif" title="mais" style="cursor:pointer;" onclick="detalharEtapasSolucao(<?=$solid?>,this);">&nbsp;<b>Execução</b></th>
            <th class="esquerda" colspan="3">
                <table cellpadding="2" cellspacing="1" width="70%" border="0" >
                    <tr>
                        <th class="titulo_box bold">
                            <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 20px; width: 150px;'>
                                <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 20px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                    <?= $porcentagem ?>%
                                </div>
                            </div>
                        </th>
                    </tr>
                </table>
            </th>
        </tr>
    </table>
    <?php
}

function carregarDetalhesEtapas($dados){

    global $db;

    $sql = "SELECT etpdsc, etpobs, SUM(situacao) AS totalsituacao, COUNT(0) AS total, COALESCE(SUM(finalizado),0) AS totalfinalizado
            FROM (
                SELECT
                    etp.etpdsc,
                    etp.etpobs,
                    etp.etpordem,
                    CASE WHEN atv.atvprazo < CURRENT_DATE THEN 1 ELSE 0 END AS situacao,
                    CASE WHEN doc.esdid IN (1301) THEN 1 END AS execucao,
                    CASE WHEN doc.esdid IN (1300) THEN 1 END AS finalizado
                FROM pto.etapa etp
                INNER JOIN pto.atividade atv ON atv.etpid = etp.etpid AND atv.atvstatus = 'A'
                INNER JOIN workflow.documento doc ON doc.docid = atv.docid
                WHERE etp.etpstatus = 'A'
                AND etp.solid = {$dados['solid']}
            ) AS FOO
            GROUP BY etpordem, etpdsc, etpobs
            ORDER BY etpordem";
    $arrEtapas = $db->carregar( $sql );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr class="zebrado">
            <td class="bold" colspan="2" width="20%">Etapa</td>
        </tr>
        <?php
        if($arrEtapas){
            $count=1;
            foreach($arrEtapas as $dado){
                $count++;
                if($dado['total'] > 0){
                    $porcentagem = number_format(($dado['totalfinalizado']*100)/$dado['total'],0,",",".");
                }else{
                    $porcentagem = 0;
                }
                ?>
                <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                    <td class="" ><?php echo $dado['etpdsc'] ?></td>
                    <td class="numero" >
                        <table cellpadding="2" cellspacing="1" width="70%" border="0" >
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td class="titulo_box bold">
                                    <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 20px; width: 150px;'>
                                        <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 20px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                            <?= $porcentagem ?>%
                                        </div>
                                    </div>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <?php
            }
        }?>
    </table>
<?php
}

function gerarGraficoMetas($meta){

    global $db;

    if($meta > 0){
        $where = "AND met.mpneid = {$meta}";
    }

    $sql = "SELECT
                DISTINCT sol.solid, sol.solordem, Replace(SUBSTR(mpn.mpnenome,1,7),':','') AS meta, met.mpneid
            FROM pto.solucao sol
            INNER JOIN pto.metasolucao met ON met.solid = sol.solid
            INNER JOIN pde.ae_metapne mpn ON mpn.mpneid = met.mpneid
            WHERE sol.solstatus = 'A'
            $where
            ORDER BY met.mpneid, sol.solordem";
    $arrDados = $db->carregar( $sql );
    if($arrDados){
        $contaLinhas = 0;
        $metaId = 0;
        $fecharTabela = 0;
        foreach($arrDados as $dado){
            if($dado['mpneid']!=$metaId){
                if($fecharTabela > 0){
                    echo "</table>";
                    echo "<br><br>";
                    $contaLinhas = 0;
                }
                $fecharTabela = 1;
                echo "<table border='2' align='center' width='98%' cellspacing='4' cellpadding='5'>";
                echo "<tr>";
                echo "<td class='titulo_box center bold' colspan='2'>";
                echo $dado['meta'];
                echo "</td>";
                echo "</tr>";
            }
            if ($contaLinhas == 0) echo "<tr>";
            echo "<td width='50%'>";
            echo montaQuadroPto($dado['solid']);
            echo "</td>";
            $contaLinhas++;
            if ($contaLinhas == 2){
                echo "</tr>";
                $contaLinhas=0;
            }
            $metaId = $dado['mpneid'];
        }
        echo "</table>";
    }
}
?>