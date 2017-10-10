<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

//permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <link rel="stylesheet" type="text/css" href="/library/jquery/jquery-ui-1.10.3/themes/dark-hive/jquery-ui-1.10.3.custom.min.css"/>
    
    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
	<script language="javascript" type="text/javascript" src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>    
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="/estrutura/js/funcoes.js"></script>
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
        atualizaUsuario();
        $(function () {
            $('#bt_imprimir').on('click', function () {
                window.print();
            });
        })
    </script>

    <style type="text/css">
        #div-ciclos{
            height: 900px;
        }
        #div-qtd{
            height: 550px;
        }
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
    </style>

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
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="1" ><div style="margin:28px" >PNE - Atividades Críticas</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao">
            <div>
                <?php exibirTitulo('executiverel', 'Relatório de Atividades Críticas (Não Executadas)'); ?>
            </div>
            <?php
            $sql = "SELECT
                        sol.solid,
                        TO_CHAR(sol.solprazo,'YYYY') AS ano,
                        sol.solapelido AS apelido,
                        sol.solobs, atv.atvdsc,
                        TO_CHAR(atv.atvprazo,'DD/MM/YYYY') AS prazo,
                        usu.usunome AS responsavel,
                        atv.atvobs
                    FROM pto.solucao sol
                    INNER JOIN pto.etapa etp ON etp.solid = sol.solid AND etp.etpstatus = 'A'
                    INNER JOIN pto.atividade atv ON atv.etpid = etp.etpid AND atv.atvstatus = 'A'
                    INNER JOIN workflow.documento doc ON doc.docid = atv.docid
                    LEFT JOIN seguranca.usuario usu ON usu.usucpf = atv.usucpf
                    WHERE sol.solstatus = 'A'
                    AND doc.esdid = 1301
                    AND atv.atvcritico IS TRUE
                    ORDER BY ano, sol.solordem, etp.etpordem, atv.atvordem";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                $rowspan = array();
                $count = 0;
                $solid = 0;
                foreach($arrDados as $dado){
                    $count++;
                    if($dado['solid']!=$solid){
                        $sql = "SELECT COUNT(0)
                                FROM pto.etapa etp
                                INNER JOIN pto.atividade atv ON atv.etpid = etp.etpid AND atv.atvstatus = 'A'
                                INNER JOIN workflow.documento doc ON doc.docid = atv.docid
                                WHERE etp.etpstatus = 'A'
                                AND doc.esdid = 1301
                                AND atv.atvcritico IS TRUE
                                AND etp.solid = {$dado['solid']}";
                        $totalSolucao = $db->pegaUm( $sql );
                        $solid = $dado['solid'];
                    }else{
                        $totalSolucao = 0;
                    }
                    $rowspan[$count]=$totalSolucao;
                }
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <?php
                $count = 0;
                $solid = 0;
                $ano = 0;
                foreach($arrDados as $dado){
                    $count++;
                    if($ano != $dado['ano']){
                        $ano=$dado['ano'];
                        if($count > 1){
                            echo "<tr>";
                            echo "<td colspan='6'>&nbsp;</td>";
                            echo "</tr>";
                        }
                        echo "<tr>";
                        echo "<th colspan='6'><font size='2'>".$ano."</th>";
                        echo "</tr>";
                        echo "<tr>";
                        echo "<th><font size='2'>Iniciativa</th>";
                        echo "<th><font size='2'>Observação</th>";
                        echo "<th><font size='2'>Atividade Crítica</th>";
                        echo "<th><font size='2'>Prazo</th>";
                        echo "<th><font size='2'>Responsável</th>";
                        echo "<th><font size='2'>Observação</th>";
                        echo "</tr>";
                    }
                    echo "<tr>";
                    if($dado['solid']!=$solid){
                        echo "<td valign='middle' rowspan='".$rowspan[$count]."'><font size='2'>".$dado['apelido']."</div></td>";
                        echo "<td valign='middle' rowspan='".$rowspan[$count]."'><font size='2'>".$dado['solobs']."</td>";
                        $solid = $dado['solid'];
                    }
                    echo "<td><font size='2'>".$dado['atvdsc']."</td>";
                    echo "<td><font size='2'>".$dado['prazo']."</td>";
                    echo "<td><font size='2'>".$dado['responsavel']."</td>";
                    echo "<td><font size='2'>".$dado['atvobs']."</td>";
                    echo "</tr>";
                }
            echo "</table>";
            }
            ?>
		</td>
	</tr>
    <tr>
        <td class="fundo_padrao" align="right">
            <button class="btn btn-primary btn-sm" id="bt_imprimir" type="button">
                <span aria-hidden="true" class="glyphicon glyphicon-print"></span> imprimir
            </button>
        </td>
    </tr>
</table>
</body>
</html>