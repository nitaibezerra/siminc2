<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_metas'])){
    exibirPrazosDispositivos($_REQUEST['detalhe_metas']);
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
            jQuery('.filtrar_metas').click(function(){
                jQuery('#div_metas').load('estrategico.php?modulo=principal/cockpit_prazos_pne&acao=A&detalhe_metas='+jQuery(this).val());
            });
        });
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_prazos_pne&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="1" ><div style="margin:28px" >Prazos PNE</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('alvo', 'Prazos PNE'); ?>
            </div>
            <?php
            $sql = "SELECT DISTINCT ano
                    FROM (
                        SELECT TO_CHAR(mpneprazo,'YYYY') AS ano FROM pde.ae_metapne
                    UNION ALL
                        SELECT TO_CHAR(estprazo,'YYYY') AS ano FROM pde.ae_estrategia
                    UNION ALL
                        SELECT TO_CHAR(artprazo,'YYYY') AS ano FROM pde.ae_artigo
                    ) AS FOO
                    ORDER BY ano";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                echo "<div style='width:100%;text-align:center'>";
                echo "<input type='radio' class='filtrar_metas' name='metas' value='0' checked='checked' onclick='' />Todos";
                $contaLinhas = 1;
                foreach($arrDados as $dado){
                    echo "<input type='radio' class='filtrar_metas' name='metas' value='".$dado['ano']."' onclick='' />".$dado['ano']."";
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
                <?php exibirPrazosDispositivos(0); ?>
            </div>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="1">
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
function exibirPrazosDispositivos($ano){

    global $db;

    if($ano > 0){
        $where = "WHERE SUBSTRING(prazo,7,4) = '{$ano}'";
        $label = $ano;
    }else{
        $label = "Todos";
    }

    $sql = "SELECT dataprazo, prazo, dispositivo, ordemdispositivo, ordem, situacao, endereco
            FROM (
                SELECT mpneprazo AS dataprazo, TO_CHAR(mpneprazo,'DD/MM/YYYY') AS prazo, mpnenome AS dispositivo, ' - ' AS situacao, mpneordem AS ordem, 1 AS ordemdispositivo, '''pne'','||temid||','||mpneid||','''',''''' AS endereco FROM pde.ae_metapne WHERE mpneid <> 99999999
            UNION ALL
                SELECT est.estprazo AS dataprazo, TO_CHAR(est.estprazo,'DD/MM/YYYY') AS prazo, est.estnome AS dispositivo, sit.sitdsc AS situacao, est.estordem AS ordem, 2 AS ordemdispositivo, '' AS endereco FROM pde.ae_estrategia est INNER JOIN pde.ae_situacao sit ON sit.sitid = est.sitid
            UNION ALL
                SELECT artprazo AS dataprazo, TO_CHAR(artprazo,'DD/MM/YYYY') AS prazo, art.artnome AS dispositivo, sit.sitdsc AS situacao, art.artordem AS ordem, 3 AS ordemdispositivo, '' AS endereco FROM pde.ae_artigo art INNER JOIN pde.ae_situacao sit ON sit.sitid = art.sitid
            ) AS FOO
            $where
            ORDER BY 1,4,5,6";
    $arrDados = $db->carregar($sql);
    if($arrDados){
        $count=-1;
        $anoInicio=0;
        foreach($arrDados as $dado){
            $count++;
            if($anoInicio!=substr($dado['prazo'], '6', '4')){
                if($anoInicio > 0){
                    echo "            </table>";
                    echo "        </td>";
                    echo "    </tr>";
                    echo "</table>";
                    echo "<br><br>";
                    $count=0;
                }
                $anoInicio = substr($dado['prazo'], '6', '4');
                echo "<table border='2' align='center' width='98%' cellspacing='4' cellpadding='5'>";
                echo "    <tr>";
                echo "        <td class='titulo_box center bold'>";
                echo            substr($dado['prazo'], '6', '4');
                echo "        </td>";
                echo "    </tr>";

                echo "    <tr>";
                echo "        <td>";
                echo "            <table class='tabela_box' cellpadding='2' cellspacing='1' width='100%'>";
                echo "                <tr>";
                echo "                  <th width='10%'>Data</th>";
                echo "                  <th width='85%'>Dispositivo</th>";
                echo "                  <th width='5%'>Situação</th>";
                echo "                </tr>";
            }
                echo "                <tr class='".(($count%2) ? 'zebrado' : '')." link' ".($dado['endereco'] != '' ? "onclick=abreAlinhamentoEstrategico(".$dado['endereco'].");" : "").">";
                echo "                  <td class='center'>".$dado['prazo']."</td>";
                echo "                  <td class=''>".$dado['dispositivo']."</td>";
                echo "                  <td class='center'>".($dado['situacao']=='Atendida' ? '<img src="../imagens/check_checklist.png" title="Atendida">' : ($dado['situacao']=='Não atendida' ? '<img src="../imagens/erro_checklist.png" title="Não atendida">' : ($dado['situacao']=='Em atendimento' ? '<img src="../imagens/lapis2.png" title="Em atendimento">' : '-')))."</td>";
                echo "                </tr>";
            if($anoInicio!=substr($dado['prazo'], '6', '4')){
                echo "            </table>";
                echo "        </td>";
                echo "    </tr>";
                echo "</table>";
            }
        }
        echo "            </table>";
        echo "        </td>";
        echo "    </tr>";
        echo "</table>";
    }
}
?>