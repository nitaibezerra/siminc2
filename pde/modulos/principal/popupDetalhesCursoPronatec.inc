<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

//permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if($_REQUEST['tipo']==1){
    $tipo = "FIC";
    $idDetalhe = 7598;
}else{
    $tipo = "Técnico";
    $idDetalhe = 7599;
}
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
    </script>

    <style type="text/css">
        #div-ciclos{
            height: 900px;
        }
        #div-qtd{
            height: 550px;
        }
        .fundo_titulo{
            background-image:url('../imagens/fundobrasilpro.jpg');
        }
    </style>

</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" >Pronatec<br>(2011 - 2014)</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" align="middle">
			<div>
				<?php exibirTitulo('executiverel', 'Principais Cursos Pronatec ('.$tipo.')', 'Exceto Acordo de Gratuidade'); ?>
			</div>
            <?php
            $sql = "SELECT
                        SUM(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2577)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 NOT IN (7529)
                    AND dpe.dpeanoref BETWEEN '2011' AND '2014'";
            $totalMatriculasTotal = $db->pegaUm( $sql );

            $sql = "SELECT
                        SUM(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2577)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 NOT IN (7529)
                    AND dsh.tidid2 IN ({$idDetalhe})
                    AND dpe.dpeanoref BETWEEN '2011' AND '2014'";
            $totalMatriculasTipo = $db->pegaUm( $sql );

            $sql = "SELECT ccpcurso, ccpmatricula FROM pde.cockpit_cursos_pronatec WHERE ccptipo = '{$tipo}' ORDER BY ccpmatricula DESC LIMIT 30";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Ordem</th>
                    <th>Cursos</th>
                    <th>Matrículas 2011-2014</th>
                    <th>Percentual<br>em Relação<br>ao Total</th>
                    <th>Percentual<br>em Relação<br>ao <?=$tipo?></th>
                </tr>
                <?php
                $count=0;
                $percentualtotal=0;
                $percentualtipo=0;
                foreach($arrDados as $dado){
                    $count++;
                    $percentualtotal = ($dado['ccpmatricula'] / $totalMatriculasTotal)*100;
                    $percentualtipo = ($dado['ccpmatricula'] / $totalMatriculasTipo)*100;
                    ?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td class="center"><?=$count?></td>
                            <td><?=$dado['ccpcurso']?></td>
                            <td class="numero" ><?php echo number_format($dado['ccpmatricula'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($percentualtotal,2,",",".") ?>%</td>
                            <td class="numero" ><?php echo number_format($percentualtipo,2,",",".") ?>%</td>
                    </tr>
                <?php
                }
            echo "</table>";
            }
            ?>
		</td>
	</tr>
</table>
</body>
</html>